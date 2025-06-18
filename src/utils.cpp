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

void* get_kernel_function_export(const wchar_t* module_name, const char* function_name) {
    if (!module_name || !function_name)
        return nullptr;

    void* module_base = get_module_base(module_name);
    if (!module_base)
        return nullptr;

    return RtlFindExportedRoutineByName(module_base, function_name);
}

void* signature_scan(void* base, size_t size, const char* pattern, const char* mask) {
    static const auto check_mask = [&](const char* base, const char* pattern,
        const char* mask) -> bool {
            for (; *mask; ++base, ++pattern, ++mask)
                if (*mask == 'x' && *base != *pattern)
                    return false;
            return true;
        };

    static const auto _strlen = [&](const char* str) -> unsigned {
        const char* s;
        for (s = str; *s; ++s)
            ;
        return (s - str);
        };

    size -= _strlen(mask);
    for (auto i = 0; i <= size; ++i) {
        void* addr = (void*)&(((char*)base)[i]);
        if (check_mask((char*)addr, pattern, mask))
            return addr;
    }
    return nullptr;
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

