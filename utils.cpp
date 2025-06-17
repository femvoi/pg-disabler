#include "utils.h"

void* get_kernel_base() {
    if (!PsLoadedModuleList)
        return NULL;

    UNICODE_STRING compare_string = RTL_CONSTANT_STRING(L"ntoskrnl.exe");
    PLIST_ENTRY pEntry = PsLoadedModuleList->Flink;

    while (pEntry != PsLoadedModuleList) {
        PKLDR_DATA_TABLE_ENTRY pLdrData = CONTAINING_RECORD(pEntry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
        if (RtlCompareUnicodeString(&compare_string, &pLdrData->BaseDllName, FALSE) == 0)
            return pLdrData->DllBase;

        pEntry = pEntry->Flink;
    }

    return NULL;
}

void* get_module_base(const wchar_t* module_name) {
    if (!PsLoadedModuleList || !module_name)
        return NULL;

    UNICODE_STRING compare_string;
    RtlInitUnicodeString(&compare_string, module_name);
    PLIST_ENTRY pEntry = PsLoadedModuleList->Flink;

    while (pEntry != PsLoadedModuleList) {
        PKLDR_DATA_TABLE_ENTRY pLdrData = CONTAINING_RECORD(pEntry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
        if (RtlCompareUnicodeString(&compare_string, &pLdrData->BaseDllName, FALSE) == 0)
            return pLdrData->DllBase;

        pEntry = pEntry->Flink;
    }

    return NULL;
}

void* get_kernel_function(const wchar_t* function_name) {
    if (!function_name)
        return NULL;

    UNICODE_STRING function_string;
    RtlInitUnicodeString(&function_string, function_name);
    return MmGetSystemRoutineAddress(&function_string);
}

BOOLEAN is_inside_module(void* address) {
    if (!PsLoadedModuleList || !address || !MmIsAddressValid(address))
        return FALSE;

    PLIST_ENTRY pEntry = PsLoadedModuleList->Flink;

    while (pEntry != PsLoadedModuleList) {
        PKLDR_DATA_TABLE_ENTRY pLdrData = CONTAINING_RECORD(pEntry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
        if (MmIsAddressValid(pLdrData->DllBase) &&
            address >= pLdrData->DllBase &&
            address < (PVOID)((PUCHAR)pLdrData->DllBase + pLdrData->SizeOfImage))
            return TRUE;

        pEntry = pEntry->Flink;
    }

    return FALSE;
}

UNICODE_STRING get_module_name(void* address) {
    UNICODE_STRING result = { 0 };

    if (!PsLoadedModuleList || !address || !MmIsAddressValid(address))
        return result;

    PLIST_ENTRY pEntry = PsLoadedModuleList->Flink;

    while (pEntry != PsLoadedModuleList) {
        PKLDR_DATA_TABLE_ENTRY pLdrData = CONTAINING_RECORD(pEntry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
        if (MmIsAddressValid(pLdrData->DllBase) &&
            address >= pLdrData->DllBase &&
            address < (PVOID)((PUCHAR)pLdrData->DllBase + pLdrData->SizeOfImage))
        {
            result = pLdrData->BaseDllName;
            break;
        }

        pEntry = pEntry->Flink;
    }

    return result;
}

PKPRCB KeGetCurrentPrcb() {
    return (PKPRCB)__readgsqword(0x20);
}