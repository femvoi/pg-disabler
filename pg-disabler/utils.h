#pragma once

#include <ntifs.h>
#include <ntddk.h>
#include "types.h"


extern VOID KeInitializeAffinityEx(PKAFFINITY_EX affinity);
extern VOID KeAddProcessorAffinityEx(PKAFFINITY_EX affinity, INT number);
extern VOID HalSendNMI(PKAFFINITY_EX affinity);
extern PLIST_ENTRY PsLoadedModuleList;

void* get_kernel_base();
void* get_module_base(const wchar_t* module_name);
void* get_kernel_function(const wchar_t* function_name);
BOOLEAN is_inside_module(void* address);
UNICODE_STRING get_module_name(void* address);