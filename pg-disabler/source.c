#include <ntifs.h>
#include <ntddk.h>
#include <intrin.h>
#include "utils.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);
void DriverUnload(PDRIVER_OBJECT DriverObject);
BOOLEAN IsPatchGuardDpc(__int64 DeferredContext);
VOID CustomTimerDpcRoutine(
	_In_ struct _KDPC* Dpc,
	_In_opt_ PVOID DeferredContext,
	_In_opt_ PVOID SystemArgument1,
	_In_opt_ PVOID SystemArgument2
);

BOOLEAN IsPatchGuardDpc(__int64 DeferredContext)
{
	return DeferredContext >> 47 != -1 && DeferredContext >> 47 != 0;
}

VOID CustomTimerDpcRoutine(
	_In_ struct _KDPC* Dpc,
	_In_opt_ PVOID DeferredContext,
	_In_opt_ PVOID SystemArgument1,
	_In_opt_ PVOID SystemArgument2
)
{
	UNREFERENCED_PARAMETER(Dpc);
	UNREFERENCED_PARAMETER(DeferredContext);
	UNREFERENCED_PARAMETER(SystemArgument1);
	UNREFERENCED_PARAMETER(SystemArgument2);

	PVOID stackFrames[254];
	USHORT framesCaptured = RtlCaptureStackBackTrace(1, 254, stackFrames, NULL);

	DbgPrint("captured %d frames \n", framesCaptured);

	void* kernel_base = get_kernel_base();

	for (USHORT idx = 0; idx < framesCaptured; idx++)
	{
		DbgPrint("frame @ %p inside module %wZ \n", stackFrames[idx], get_module_name(stackFrames[idx]));
	}

	for (USHORT idx = 0; idx < framesCaptured; idx++)
	{
		PVOID offseted_address = (PVOID)((PUCHAR)stackFrames[idx] - (PUCHAR)kernel_base + 0x140000000);
		DbgPrint("%p \n", offseted_address);
	}


}

void DriverUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);

	DbgPrint("DriverUnload called @ %p \n", &DriverUnload);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgPrint("DriverEntry called @ %p \n", &DriverEntry);

	DriverObject->DriverUnload = DriverUnload;

	DbgPrint("kernel base %p \n", get_kernel_base());

	KDPC kDpc;
	ULONG processorCount = KeQueryActiveProcessorCount(NULL);


	for (int idx = 0; idx < processorCount; idx++) {
		PROCESSOR_NUMBER procNumber;
		if (NT_SUCCESS(KeGetProcessorNumberFromIndex(idx, &procNumber)))
		{
			KeInitializeDpc(
				&kDpc,
				CustomTimerDpcRoutine,
				NULL
			);
			KeSetTargetProcessorDpc(&kDpc, (CCHAR)&procNumber);
			KeInsertQueueDpc(&kDpc, NULL, NULL);

			DbgPrint("queued dpc on core %d \n", idx);
		}
	}

	return STATUS_SUCCESS;
}