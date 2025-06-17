#pragma once	

#include <ntifs.h>
#include <ntddk.h>
#include <intrin.h>
#include "utils.h"
#include "hexrays.h"

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);