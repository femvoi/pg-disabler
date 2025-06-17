#include "source.h"

const uint64_t o_KiWaitNever = 0xCFC808;
const uint64_t o_KiWaitAlways = 0xCFCA10;

PKDPC decrypt_dpc_routine(PKTIMER timer) {
	static __int64 KiWaitNever;
	static __int64 KiWaitAlways;

	UNICODE_STRING routineName;
	RtlInitUnicodeString(&routineName, L"KeSetTimerEx");
	PVOID KeSetTimerEx = MmGetSystemRoutineAddress(&routineName);
	if (!KeSetTimerEx)
		return nullptr;
	
	if (!KiWaitNever || !KiWaitAlways) {
		unsigned char function_dism[32];
		RtlCopyMemory(function_dism, KeSetTimerEx, sizeof(function_dism));

		for (int idx = 0; idx < 32; idx++) {
			if (function_dism[idx] == 0x2e && function_dism[idx++] == 0x48 && function_dism[idx + 2] == 0x8b) {
				uint32_t* addr = (uint32_t*)&function_dism[idx + 5]; 
				if (KiWaitNever == 0) {
					KiWaitNever = *(__int64*)(*addr);
					DbgPrint("KiWaitNever set to: 0x%llx\n", KiWaitNever);
					DbgPrint("KiWaitNever real to: 0x%llx\n", (void*)((unsigned*)get_kernel_base() + o_KiWaitNever));

				}
				idx += 8;
			}
		}
	}

	PKDPC dpc_routine = nullptr;

	return dpc_routine;
}

uint64_t DecryptDpcRoutine(PKTIMER pTimer) {
	__int64 KiWaitNever = 0;
	__int64 KiWaitAlways = 0;

	if (!KiWaitNever || !KiWaitAlways) {
		uint64_t pKiWaitNever = (uint64_t)get_kernel_base() + o_KiWaitNever;
		uint64_t pKiWaitAlways = (uint64_t)get_kernel_base() + o_KiWaitAlways;

		if (!pKiWaitNever || !pKiWaitAlways)
			return 0;

		KiWaitNever = *(__int64*)pKiWaitNever;
		KiWaitAlways = *(__int64*)pKiWaitAlways;
	}

	_KDPC* DecryptedDpc = (_KDPC*)(KiWaitAlways ^ _byteswap_uint64((unsigned __int64)pTimer ^ __ROL8__((__int64)pTimer->Dpc ^ KiWaitNever, KiWaitNever)));

	return (uint64_t)DecryptedDpc;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgPrint("driver entry called @ %p \n", &DriverEntry);
	DbgPrint("kernel base %p \n", get_kernel_base());

	KAFFINITY active_processers = KeQueryActiveProcessors();

	for (int core = 0; active_processers; core++, active_processers >>= 1) {
		if (active_processers & 1) {
			DbgPrint("\n");
			KeSetSystemAffinityThread(1ULL << core);

			PKPRCB processer_block = KeGetCurrentPrcb();
			if (!processer_block) {
				DbgPrint("invaild processer block on core %d", core);
				continue;
			}

			PKTIMER_TABLE timer_table = &processer_block->TimerTable;
			if (!timer_table || !MmIsAddressValid(timer_table)) {
				DbgPrint("invaild timer table for core %d", core);
				continue;
			}

			DbgPrint("current core %d", core);
			DbgPrint("process block %p", processer_block);
			DbgPrint("timer table %p", timer_table);

			PKTIMER_TABLE_ENTRY timer_entry = nullptr;

			for (int wheel = 0; wheel < 2; wheel++) {
				for (int entry = 0; entry < 256; entry++) {
					timer_entry = &timer_table->TimerEntries[wheel][entry];
					if (!timer_entry || !MmIsAddressValid(timer_entry)) {
						DbgPrint("invaild timer entry | wheel %d : entry : %d", wheel, entry);
						continue;
					}

					//DbgPrint("timer entry %p", timer_entry);
				
						PKTIMER timer = CONTAINING_RECORD(timer_entry->Entry.Flink, KTIMER, TimerListEntry);
						if (!timer || !MmIsAddressValid(timer)) {
							DbgPrint("invaild timer | wheel %d : entry : %d", wheel, entry);
							continue;
						}

						//DbgPrint("timer %p", timer);

						PKDPC dpc = (PKDPC)DecryptDpcRoutine(timer);
						if (!dpc || !MmIsAddressValid(dpc)) {
							continue;
						}

						if ((__int64)dpc->DeferredContext >> 47 != -1 && (__int64)dpc->DeferredContext >> 47 != 0) {
							DbgPrint("dpc %p", dpc);
							if (KeCancelTimer(timer)) {
								DbgPrint("canceled the patchguard");
							}

						}
					
						
					
				
				}
			}


		}
	}

	return STATUS_SUCCESS;
}

