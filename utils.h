#pragma once

#include <ntifs.h>
#include <ntddk.h>
#include "types.h"

extern "C" PLIST_ENTRY PsLoadedModuleList;
extern "C" NTKERNELAPI void* NTAPI RtlFindExportedRoutineByName(
	_In_ PVOID ImageBase,
	_In_ PCCH RoutineName
);


void* get_kernel_base();
void* get_module_base(const wchar_t* module_name);
void* get_kernel_function(const wchar_t* function_name);
void* get_kernel_function_export(const wchar_t* module_name, const char* function_name);
void* signature_scan(void* base, size_t size, const char* pattern, const char* mask);
BOOLEAN is_inside_module(void* address);
UNICODE_STRING get_module_name(void* address);
PKPRCB KeGetCurrentPrcb();