#include "timer.h"

NTSTATUS timer::initialize_encryption_values() {
	if (timer::KiWaitAlways && timer::KiWaitNever)
		return STATUS_SUCCESS;

	u64 KeInitializeTimer2 = kutil::get_routine(L"KeInitializeTimer2");
	if (!KeInitializeTimer2)
		return STATUS_INVALID_ADDRESS;

	u8 disassembly_x64[256] = { };
	constexpr u32 disassembly_x64_sz = sizeof(disassembly_x64);

	memcpy(disassembly_x64, (const void*)KeInitializeTimer2, disassembly_x64_sz);

	u64 KiInitializeTimer2 = 0;

	/*
		 KeInitializeTimer2 -- function

		 sub     rsp, 28h
		 and     word ptr [rcx+2], 0
		 call    KiInitializeTimer2
		 add     rsp, 28h
		 retn
	*/

	for (u32 idx = 0; idx < disassembly_x64_sz - 6; idx++) {
		if (disassembly_x64[idx] == 0xe8) {
			s32 relative_offset = *(s32*)&disassembly_x64[idx + 1];
			u64 instruction_address = KeInitializeTimer2 + idx;
			KiInitializeTimer2 = instruction_address + 5 + relative_offset;
			break;
		}
	}

	if (!KiInitializeTimer2 || !MmIsAddressValid((void*)KiInitializeTimer2))
		return STATUS_INVALID_ADDRESS;

	memcpy(disassembly_x64, (const void*)KiInitializeTimer2, disassembly_x64_sz);

	/*
		48 8b 05 00 00 00 00     mov     rax, qword ptr [ntkrnlmp!KiWaitNever (0)]
		48 8b 15 00 00 00 00     mov     rdx, qword ptr [ntkrnlmp!KiWaitAlways (0)]
		8b c8					 mov     ecx, eax
	*/

	const u8 compare_assembly_0[] = {
		0x48,
		0x8b,
		0x05
	};

	constexpr u32 compare_assembly_0_sz = sizeof(compare_assembly_0);

	const u8 compare_assembly_1[] = {
		0x48,
		0x8b,
		0x15
	};

	constexpr u32 compare_assembly_1_sz = sizeof(compare_assembly_1);

	for (u32 idx = 0; idx < disassembly_x64_sz - 16; idx++) {
		if (RtlCompareMemory(&disassembly_x64[idx], compare_assembly_0, compare_assembly_0_sz) == compare_assembly_0_sz &&
			RtlCompareMemory(&disassembly_x64[idx + 7], compare_assembly_1, compare_assembly_1_sz) == compare_assembly_1_sz) {
			
			s32 relative_offset = *(s32*)&disassembly_x64[idx + 3];
			u64 instruction_address = KiInitializeTimer2 + idx;

			u64 final_address = instruction_address + 7 + relative_offset;

			if (!final_address || !MmIsAddressValid((void*)final_address))
				break;

			timer::KiWaitNever = *(s64*)final_address;

			relative_offset = *(s32*)&disassembly_x64[idx + 10];
			instruction_address = KiInitializeTimer2 + idx + 7; 

			final_address = instruction_address + 7 + relative_offset;
			if (!final_address || !MmIsAddressValid((void*)final_address))
				break;

			timer::KiWaitAlways = *(s64*)final_address;

			break;
		}
	}

	if (!timer::KiWaitNever || !timer::KiWaitNever)
		return STATUS_UNSUCCESSFUL;

	return STATUS_SUCCESS;
}

PKDPC timer::decrypt_dpc_from_timer(const PKTIMER timer) {
	if (!timer || !MmIsAddressValid(timer))
		return nullptr;

	if (!NT_SUCCESS(timer::initialize_encryption_values()))
		return nullptr;

	u64 dpc_address = KiWaitAlways ^ _byteswap_uint64((u64)timer ^ __ROL8__((s64)timer->Dpc ^ KiWaitNever, KiWaitNever));

	if (!MmIsAddressValid((void*)dpc_address))
		return nullptr;

	return (PKDPC)dpc_address;
}