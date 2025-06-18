#pragma once	

#include <ntifs.h>
#include <ntddk.h>
#include <intrin.h>
#include "hook.hpp"
#include "utils.h"
#include "hexrays.h"

extern "C" NTSTATUS FxDriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);