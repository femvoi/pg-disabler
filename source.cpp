#include "source.h"

PKPRCB KeGetCurrentPrcb() {
	return (PKPRCB)__readgsqword(0x20);
}

BOOLEAN IsPatchGuardDpc(__int64 DeferredContext) {
	return DeferredContext >> 47 != -1 && DeferredContext >> 47 != 0;
}

void DriverUnload(PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);

	DbgPrint("DriverUnload called @ %p \n", &DriverUnload);
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgPrint("DriverEntry called @ %p \n", &DriverEntry);

	DbgPrint("kernel base %p \n", get_kernel_base());


	return STATUS_SUCCESS;
}

PKDPC decrypt_dpc(PKTIMER timer, PKDPC dpc) {
	uint64_t KiWaitNever = 0;
	uint64_t KiWaitAlways = 0;

	unsigned char kesettimerex_bytes[64];

	RtlCopyMemory(kesettimerex_bytes, KeSetTimerEx, sizeof(kesettimerex_bytes));

	/*
	fffff805`20ee8bac 488b05553ca100       mov     rax, qword ptr [ntkrnlmp!KiWaitNever (fffff805218fc808)]
	fffff805`20ee8bb3 488bf9               mov     rdi, rcx
	fffff805`20ee8bb6 488b35533ea100       mov     rsi, qword ptr [ntkrnlmp!KiWaitAlways (fffff805218fca10)]
	*/

	for (int i = 0; i < sizeof(kesettimerex_bytes); i++) {
		if (kesettimerex_bytes[i] == 0x48 && kesettimerex_bytes[i + 1] == 0x8b && kesettimerex_bytes[i + 2] == 0x05) {
			uint32_t relative_offset = *(uint32_t*)&kesettimerex_bytes[i + 3];

			uint64_t next_instruction = (uint64_t)KeSetTimerEx + i + 7;

			uint64_t extracted_address = next_instruction + (int32_t)relative_offset;

			//DbgPrint("mov rax, qword ptr [%p]\n", (void*)extracted_address);

			KiWaitNever = extracted_address;
		}
		if (kesettimerex_bytes[i] == 0x48 && kesettimerex_bytes[i + 1] == 0x8b && kesettimerex_bytes[i + 2] == 0x35) {
			uint32_t relative_offset = *(uint32_t*)&kesettimerex_bytes[i + 3];

			uint64_t next_instruction = (uint64_t)KeSetTimerEx + i + 7;

			uint64_t extracted_address = next_instruction + (int32_t)relative_offset;

			//DbgPrint("mov rsi, qword ptr [%p]\n", (void*)extracted_address);

			KiWaitAlways = extracted_address;
		}
	}

	PKDPC decrypted_dpc =
		(PKDPC)(KiWaitAlways ^
			_byteswap_uint64(
				(unsigned __int64)timer ^
				__ROL8__((__int64)timer->Dpc ^ KiWaitNever, KiWaitNever)));

	return decrypted_dpc;
}