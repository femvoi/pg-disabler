#pragma once	

#include <ntifs.h>
#include <ntddk.h>
#include <intrin.h>
#include "utils.h"
#include "hexrays.h"

PKPRCB KeGetCurrentPrcb();
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);
void DriverUnload(PDRIVER_OBJECT DriverObject);
BOOLEAN IsPatchGuardDpc(__int64 DeferredContext);