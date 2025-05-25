#pragma once

#include <ntifs.h>
#include <ntddk.h>

typedef struct _KLDR_DATA_TABLE_ENTRY
{
    struct _LIST_ENTRY InLoadOrderLinks;
    VOID* ExceptionTable;
    ULONG ExceptionTableSize;
    VOID* GpValue;
    struct _NON_PAGED_DEBUG_INFO* NonPagedDebugInfo;
    VOID* DllBase;
    VOID* EntryPoint;
    ULONG SizeOfImage;
    struct _UNICODE_STRING FullDllName;
    struct _UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    union
    {
        USHORT SignatureLevel : 4;
        USHORT SignatureType : 3;
        USHORT Unused : 9;
        USHORT EntireField;
    } u1;
    VOID* SectionPointer;
    ULONG CheckSum;
    ULONG CoverageSectionSize;
    VOID* CoverageSection;
    VOID* LoadedImports;
    VOID* Spare;
    ULONG SizeOfImageNotRounded;
    ULONG TimeDateStamp;
}KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;

typedef struct _KAFFINITY_EX
{
    USHORT Count;
    USHORT Size;
    ULONG Reserved;
    ULONGLONG Bitmap[20];
}KAFFINITY_EX, *PKAFFINITY_EX;