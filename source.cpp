#include "source.h"

PKDPC decrypt_dpc_routine(PKTIMER timer) {
	if (!timer)
		return nullptr;

	static __int64 KiWaitNever;
	static __int64 KiWaitAlways;
	
	if (!KiWaitNever || !KiWaitAlways) {
		unsigned char function_dism[64];
		RtlCopyMemory(function_dism, KeSetTimerEx, sizeof(function_dism));

		if (function_dism[0x1C] == 0x48 && function_dism[0x1D] == 0x8B && function_dism[0x1E] == 0x05) {
			LONG displacement = *(LONG*)&function_dism[0x1F];
			PVOID KiWaitNeverAddr = (PVOID)((PUCHAR)KeSetTimerEx + 0x23 + displacement);
			KiWaitNever = *(PINT64)KiWaitNeverAddr;
		}
		if (function_dism[0x26] == 0x48 && function_dism[0x27] == 0x8B && function_dism[0x28] == 0x35) {
			LONG displacement = *(LONG*)&function_dism[0x29];
			PVOID KiWaitAlwaysAddr = (PVOID)((PUCHAR)KeSetTimerEx + 0x2D + displacement);
			KiWaitAlways = *(PINT64)KiWaitAlwaysAddr;
		}
	}

	PKDPC dpc_routine = (PKDPC)(
		KiWaitAlways ^ _byteswap_uint64((unsigned __int64)timer ^ __ROL8__(
			(__int64)timer->Dpc ^ KiWaitNever, KiWaitNever
		)));

	return dpc_routine;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	KAFFINITY active_processers = KeQueryActiveProcessors();

	for (int core = 0; active_processers; core++, active_processers >>= 1) {
		if (active_processers & 1) {
			KeSetSystemAffinityThread(1ULL << core);

			PKPRCB processer_block = KeGetCurrentPrcb();
			if (!processer_block) {
				continue;
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

					if ((__int64)dpc->DeferredContext >> 47 != -1 && (__int64)dpc->DeferredContext >> 47 != 0) {
						if (!KeCancelTimer(timer)) {
							return STATUS_UNSUCCESSFUL;
						}
					}

				}
			}
		}
	}

	return STATUS_SUCCESS;
}