#pragma once
#include <ntifs.h>
#include <ntddk.h>
#include <intrin.h>

namespace hook {
#if _M_IX86
    /*
        0:  b8 00 00 00 00          mov    eax,0x0
        5:  ff e0                   jmp    eax
    */
    const unsigned char jmp_code[] = { 0xB8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
    const size_t jmp_size = sizeof(jmp_code);
#elif _M_X64
    /*
        0:  48 b8 00 00 00 00 00    movabs rax,0x0
        7:  00 00 00
        a:  ff e0                   jmp    rax
    */
    const unsigned char jmp_code[] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
    const size_t jmp_size = sizeof(jmp_code);
#endif
    inline bool RtlForceCopyMemory(void* address, const void* buffer, size_t size) {
        PHYSICAL_ADDRESS physical_address = MmGetPhysicalAddress(address);
        if (!physical_address.QuadPart)
            return false;

        void* mapped = MmMapIoSpaceEx(physical_address, size, PAGE_EXECUTE_READWRITE);
        if (!mapped)
            return false;

        RtlCopyMemory(mapped, buffer, size);
        MmUnmapIoSpace(mapped, size);
        return true;
    }

    struct hook_data {
        void* function;
        void* target;
        unsigned char original_bytes[16];
        bool enabled;

        bool is_empty() const {
            return !function && !target && original_bytes[0] == 0;
        }
    };

    const __int32 max_hook_entries = 64;
    hook_data hooks[max_hook_entries] = {};

    hook_data* get_hook(void* address) {
        for (__int32 idx = 0; idx < max_hook_entries; idx++) {
            hook_data* entry = &hooks[idx];
            if (entry->function == address || entry->target == address)
                return entry;
        }
        return nullptr;
    }

    bool is_hooked(void* address) {
        hook_data* entry = get_hook(address);
        return entry && entry->enabled;
    }

    bool add_hook(void* function, void* target) {
        for (__int32 idx = 0; idx < max_hook_entries; idx++) {
            hook_data* entry = &hooks[idx];
            if (!entry->is_empty())
                continue;

            RtlCopyMemory(entry->original_bytes, function, jmp_size);
            entry->function = function;
            entry->target = target;
            entry->enabled = false;
            return true;
        }
        return false;
    }

    bool enable_hook(void* address) {
        hook_data* entry = get_hook(address);
        if (!entry || entry->enabled)
            return false;

        unsigned char jump_buffer[jmp_size];
        RtlCopyMemory(jump_buffer, jmp_code, jmp_size);
#if _M_IX86
        * (void**)(jump_buffer + 1) = entry->target;
#elif _M_X64
        * (void**)(jump_buffer + 2) = entry->target;
#endif

        if (!RtlForceCopyMemory(entry->function, jump_buffer, jmp_size))
            return false;

        entry->enabled = true;
        return true;
    }

    bool disable_hook(void* address) {
        hook_data* entry = get_hook(address);
        if (!entry || !entry->enabled)
            return false;

        if (!RtlForceCopyMemory(entry->function, entry->original_bytes, jmp_size))
            return false;

        entry->enabled = false;
        return true;
    }

    void remove_hook(void* address) {
        hook_data* entry = get_hook(address);
        if (!entry)
            return;

        if (entry->enabled)
            disable_hook(address);

        RtlZeroMemory(entry, sizeof(hook_data));
    }

    void enable_all_hooks() {
        for (__int32 idx = 0; idx < max_hook_entries; idx++) {
            hook_data* entry = &hooks[idx];
            if (!entry->is_empty() && !entry->enabled)
                enable_hook(entry->function);
        }
    }

    void disable_all_hooks() {
        for (__int32 idx = 0; idx < max_hook_entries; idx++) {
            hook_data* entry = &hooks[idx];
            if (!entry->is_empty() && entry->enabled)
                disable_hook(entry->function);
        }
    }
}