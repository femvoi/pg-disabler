#ifndef ktuil_h
#define ktuil_h

#include "../include.h"

namespace kutil {
	inline KLDR_DATA_TABLE_ENTRY* get_module_entry(const wchar_t* module_name) {
		if (!PsLoadedModuleList || !module_name)
			return nullptr;

		UNICODE_STRING module_string = { 0 };
		RtlInitUnicodeString(&module_string, module_name);

		LIST_ENTRY* list_entry = PsLoadedModuleList->Flink;

		while (list_entry != PsLoadedModuleList) {
			KLDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(list_entry, KLDR_DATA_TABLE_ENTRY, in_load_order_links);
			if (entry) {
				if (RtlCompareUnicodeString(&module_string, &entry->image_base_name, false) == 0)
					return entry;
			}

			list_entry = list_entry->Flink;
		}

		return nullptr;
	}

	inline KLDR_DATA_TABLE_ENTRY* get_module_entry(const u64 module_base) {
		if (!PsLoadedModuleList || !module_base)
			return nullptr;

		LIST_ENTRY* list_entry = PsLoadedModuleList->Flink;

		while (list_entry != PsLoadedModuleList) {
			KLDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(list_entry, KLDR_DATA_TABLE_ENTRY, in_load_order_links);
			if (entry) {
				if (entry->image_base == module_base)
					return entry;
			}

			list_entry = list_entry->Flink;
		}

		return nullptr;
	}

	inline PDRIVER_OBJECT get_driver_object(const wchar_t* object_name) {
		if (!object_name)
			return nullptr;

		UNICODE_STRING object_string = { 0 };
		RtlInitUnicodeString(&object_string, object_name);

		PDEVICE_OBJECT device_object;
		PFILE_OBJECT file_object;

		if (!NT_SUCCESS(IoGetDeviceObjectPointer(&object_string, FILE_READ_DATA, &file_object, &device_object)))
			return nullptr;

		if (file_object)
			ObfDereferenceObject(file_object);

		return device_object->DriverObject;
	}

	inline u64 get_module_base(const wchar_t* module_name) {
		if (!module_name)
			return 0;

		KLDR_DATA_TABLE_ENTRY* entry = get_module_entry(module_name);
		if (!entry)
			return 0;

		return entry->image_base;
	}

	inline bool is_inside_module(const wchar_t* module_name, u64 address) {
		if (!module_name || !address)
			return false;

		KLDR_DATA_TABLE_ENTRY* entry = get_module_entry(module_name);
		if (!entry)
			return false;

		u64 base = entry->image_base;
		u64 end = base + entry->image_size;

		return address >= base && address < end;
	}

	inline bool is_inside_module(u64 module_address, u32 module_size, u64 address) {
		if (!module_address || !module_size || !address)
			return false;

		u64 base = module_address;
		u64 end = base + module_size;

		return address >= base && address < end;
	}

	inline bool is_inside_module(u64 address) {
		if (!address)
			return false;

		if (!PsLoadedModuleList)
			return false;

		LIST_ENTRY* list_entry = PsLoadedModuleList->Flink;
		while (list_entry != PsLoadedModuleList) {
			KLDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(list_entry, KLDR_DATA_TABLE_ENTRY, in_load_order_links);
			if (entry) {
				u64 base = entry->image_base;
				u64 end = base + entry->image_size;

				return address >= base && address < end;
			}
			list_entry = list_entry->Flink;
		}

		return false;
	}

	inline UNICODE_STRING get_module(u64 address) {
		if (!PsLoadedModuleList)
			return { 0 };

		LIST_ENTRY* list_entry = PsLoadedModuleList->Flink;

		while (list_entry != PsLoadedModuleList) {
			KLDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(list_entry, KLDR_DATA_TABLE_ENTRY, in_load_order_links);

			if (entry && entry->image_base && entry->image_size) {
				u64 base = entry->image_base;
				u64 end = base + entry->image_size;

				if (address >= base && address < end) {
					return entry->image_base_name;
				}
			}

			list_entry = list_entry->Flink;
		}

		return { 0 };
	}


	inline NTSTATUS dump_memory_to_disk(u64 address, u32 size, const wchar_t* file_path) {
		if (!address || !size || !file_path)
			return STATUS_INVALID_PARAMETER;

		UNICODE_STRING file_string = { 0 };
		RtlInitUnicodeString(&file_string, file_path);

		OBJECT_ATTRIBUTES object_attributes = { 0 };
		InitializeObjectAttributes(&object_attributes, &file_string, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

		HANDLE file_handle = NULL;
		IO_STATUS_BLOCK io_status = { 0 };

		NTSTATUS status = ZwOpenFile(
			&file_handle,
			FILE_READ_DATA | SYNCHRONIZE,
			&object_attributes,
			&io_status,
			FILE_SHARE_READ,
			FILE_SYNCHRONOUS_IO_NONALERT
		);

		if (NT_SUCCESS(status)) {
			ZwClose(file_handle);
			return STATUS_FILES_OPEN; // we don't want to possibly overwrite a file already on disk. insure it just does not exist
		}

		status = ZwCreateFile(
			&file_handle,
			FILE_WRITE_DATA | SYNCHRONIZE,
			&object_attributes,
			&io_status,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ,
			FILE_OVERWRITE_IF,
			FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			0
		);

		if (!NT_SUCCESS(status))
			return status;

		status = ZwWriteFile(
			file_handle,
			NULL,
			NULL,
			NULL,
			&io_status,
			(PVOID)address,
			size,
			NULL,
			NULL
		);

		ZwClose(file_handle);
		return status;
	}

	inline u64 get_kernel_base() {
		if (!PsLoadedModuleList)
			return 0;

		KLDR_DATA_TABLE_ENTRY* entry = get_module_entry(L"ntoskrnl.exe");
		if (!entry)
			return 0;

		return entry->image_base;
	}

	inline u32 get_kernel_size() {
		if (!PsLoadedModuleList)
			return 0;

		KLDR_DATA_TABLE_ENTRY* entry = get_module_entry(L"ntoskrnl.exe");
		if (!entry)
			return 0;

		return entry->image_size;
	}

	inline u64 get_routine(const wchar_t* routine_name) {
		if (!routine_name)
			return 0;

		UNICODE_STRING routine_string = { 0 };
		RtlInitUnicodeString(&routine_string, routine_name);
		return (u64)MmGetSystemRoutineAddress(&routine_string);
	}

	inline u64 get_exported_routine(const wchar_t* module_name, const char* routine_name) {
		if (!module_name || !routine_name)
			return 0;

		KLDR_DATA_TABLE_ENTRY* entry = get_module_entry(module_name);
		if (!entry)
			return 0;

		return RtlFindExportedRoutineByName(entry->image_base, routine_name);
	}

	inline u64 get_exported_routine(const u64 module_base, const char* routine_name) {
		if (!module_base || !routine_name)
			return 0;

		KLDR_DATA_TABLE_ENTRY* entry = get_module_entry(module_base);
		if (!entry)
			return 0;

		return RtlFindExportedRoutineByName(entry->image_base, routine_name);
	}

	inline u64 get_process_base(u32 pid) {
		if (!pid)
			return 0;

		PEPROCESS target_process = nullptr;

		if (NT_SUCCESS(PsLookupProcessByProcessId(ULongToHandle(pid), &target_process))) {
			u64 process_base = PsGetProcessSectionBaseAddress(target_process);
			ObfDereferenceObject(target_process);
			return process_base;
		}

		return 0;
	}

	inline u64 get_process_peb(u32 pid) {
		if (!pid)
			return 0;

		PEPROCESS target_process = nullptr;

		if (NT_SUCCESS(PsLookupProcessByProcessId(ULongToHandle(pid), &target_process))) {
			u64 process_peb = PsGetProcessPeb(target_process);
			ObfDereferenceObject(target_process);
			return process_peb;
		}

		return 0;
	}

	inline PEPROCESS get_process(const wchar_t* process_name) {
		if (!process_name)
			return nullptr;

		UNICODE_STRING target_name;
		RtlInitUnicodeString(&target_name, process_name);

		PEPROCESS process = nullptr;

		for (u32 process_id = 4; process_id <= 60000; process_id++) {
			if (!NT_SUCCESS(PsLookupProcessByProcessId(ULongToHandle(process_id), &process)))
				continue;

			PUNICODE_STRING image_name = { 0 };
			if (NT_SUCCESS(SeLocateProcessImageName(process, &image_name)) || !image_name)
				continue;

			// image name looks some shit like ??\C:\Windows\System32\notepad.exe

			ExFreePool(image_name);
		}

		return nullptr;
	}

	inline u64 signature_scan(u64 base_address, u32 scan_region, const char* pattern, const char* mask) {
		if (!base_address || !scan_region || !pattern || !mask)
			return 0;

		static const auto check_mask =
			[&](const char* base, const char* pattern, const char* mask) -> bool
			{
				for (; *mask; ++base, ++pattern, ++mask)
					if (*mask == 'x' && *base != *pattern)
						return false;
				return true;
			};

		size_t pattern_length = strlen(mask);
		if (scan_region < pattern_length)
			return 0;

		scan_region -= static_cast<u32>(pattern_length);

		for (u32 i = 0; i <= scan_region; ++i) {
			const char* current = reinterpret_cast<const char*>(base_address) + i;
			if (check_mask(current, pattern, mask)) {
				return base_address + i;
			}
		}

		return 0;
	}

	inline u64 signature_scan(u64 base_address, const char* section_name, const char* pattern, const char* mask) {
		if (!base_address || !section_name || !pattern || !mask)
			return 0;

		PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)base_address;
		if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
			return 0;

		PIMAGE_NT_HEADERS nt_headers = (PIMAGE_NT_HEADERS)(base_address + dos_header->e_lfanew);
		if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
			return 0;

		static const auto check_mask = [&](const char* base, const char* pattern, const char* mask) -> bool {
			for (; *mask; ++base, ++pattern, ++mask)
				if (*mask == 'x' && *base != *pattern)
					return false;
			return true;
			};

		size_t pattern_length = strlen(mask);
		PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt_headers);
		u32 section_count = nt_headers->FileHeader.NumberOfSections;

		for (u32 s = 0; s < section_count; ++s) {
			if (strncmp((const char*)section[s].Name, section_name, IMAGE_SIZEOF_SHORT_NAME) != 0)
				continue;

			u64 section_base = base_address + section[s].VirtualAddress;
			u32 scan_region = section[s].SizeOfRawData;

			if (scan_region < pattern_length)
				continue;

			scan_region -= static_cast<u32>(pattern_length);
			for (u32 i = 0; i <= scan_region; ++i) {
				const char* current = reinterpret_cast<const char*>(section_base) + i;
				if (check_mask(current, pattern, mask)) {
					return section_base + i;
				}
			}
		}

		return 0;
	}

	inline void sleep(u32 milliseconds) {
		LARGE_INTEGER timeout = { 0 };
		timeout.QuadPart = -(milliseconds * 10 * 1000);
		KeDelayExecutionThread(KernelMode, FALSE, &timeout);
	}

	namespace pe {
		inline PIMAGE_DOS_HEADER get_dos_header(u64 base_address) {
			if (!base_address)
				return nullptr;

			PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)base_address;
			if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
				return nullptr;

			return dos_header;
		}

		inline PIMAGE_NT_HEADERS32 get_nt_header_32(u64 base_address) {
			if (!base_address)
				return nullptr;

			PIMAGE_DOS_HEADER dos_header = get_dos_header(base_address);
			if (!dos_header)
				return nullptr;

			PIMAGE_NT_HEADERS32 nt_header = (PIMAGE_NT_HEADERS32)(base_address + dos_header->e_lfanew);
			if (nt_header->Signature != IMAGE_NT_SIGNATURE)
				return nullptr;

			if (nt_header->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
				return nullptr;

			return nt_header;
		}

		inline PIMAGE_NT_HEADERS32 get_nt_header_32(PIMAGE_DOS_HEADER dos_header) {
			if (!dos_header || dos_header->e_magic != IMAGE_DOS_SIGNATURE)
				return nullptr;

			PIMAGE_NT_HEADERS32 nt_header = (PIMAGE_NT_HEADERS32)((u8*)dos_header + dos_header->e_lfanew);
			if (nt_header->Signature != IMAGE_NT_SIGNATURE)
				return nullptr;

			if (nt_header->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
				return nullptr;

			return nt_header;
		}

		inline PIMAGE_NT_HEADERS64 get_nt_header_64(u64 base_address) {
			if (!base_address)
				return nullptr;

			PIMAGE_DOS_HEADER dos_header = get_dos_header(base_address);
			if (!dos_header)
				return nullptr;

			PIMAGE_NT_HEADERS64 nt_header = (PIMAGE_NT_HEADERS64)(base_address + dos_header->e_lfanew);
			if (nt_header->Signature != IMAGE_NT_SIGNATURE)
				return nullptr;

			if (nt_header->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
				return nullptr;

			return nt_header;
		}

		inline PIMAGE_NT_HEADERS64 get_nt_header_64(PIMAGE_DOS_HEADER dos_header) {
			if (!dos_header || dos_header->e_magic != IMAGE_DOS_SIGNATURE)
				return nullptr;

			PIMAGE_NT_HEADERS64 nt_header = (PIMAGE_NT_HEADERS64)((u8*)dos_header + dos_header->e_lfanew);
			if (nt_header->Signature != IMAGE_NT_SIGNATURE)
				return nullptr;

			if (nt_header->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
				return nullptr;

			return nt_header;
		}

		inline PIMAGE_SECTION_HEADER get_image_section(u64 base_address, const char* section_name) {
			if (!base_address || !section_name)
				return nullptr;

			PIMAGE_DOS_HEADER dos_header = get_dos_header(base_address);
			if (!dos_header)
				return nullptr;

			PIMAGE_NT_HEADERS32 nt_header32 = get_nt_header_32(dos_header);
			if (nt_header32) {
				PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt_header32);
				for (u16 i = 0; i < nt_header32->FileHeader.NumberOfSections; i++, section++) {
					if (strncmp((const char*)section->Name, section_name, IMAGE_SIZEOF_SHORT_NAME) == 0)
						return section;
				}
			}

			PIMAGE_NT_HEADERS64 nt_header64 = get_nt_header_64(dos_header);
			if (nt_header64) {
				PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt_header64);
				for (u16 i = 0; i < nt_header64->FileHeader.NumberOfSections; i++, section++) {
					if (strncmp((const char*)section->Name, section_name, IMAGE_SIZEOF_SHORT_NAME) == 0)
						return section;
				}
			}

			return nullptr;
		}

		inline PIMAGE_SECTION_HEADER get_image_section(PIMAGE_DOS_HEADER dos_header, const char* section_name) {
			if (!dos_header || !section_name || dos_header->e_magic != IMAGE_DOS_SIGNATURE)
				return nullptr;

			PIMAGE_NT_HEADERS32 nt_header32 = get_nt_header_32(dos_header);
			if (nt_header32) {
				PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt_header32);
				for (u16 i = 0; i < nt_header32->FileHeader.NumberOfSections; i++, section++) {
					if (strncmp((const char*)section->Name, section_name, IMAGE_SIZEOF_SHORT_NAME) == 0)
						return section;
				}
			}

			PIMAGE_NT_HEADERS64 nt_header64 = get_nt_header_64(dos_header);
			if (nt_header64) {
				PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt_header64);
				for (u16 i = 0; i < nt_header64->FileHeader.NumberOfSections; i++, section++) {
					if (strncmp((const char*)section->Name, section_name, IMAGE_SIZEOF_SHORT_NAME) == 0)
						return section;
				}
			}

			return nullptr;
		}
	}
}

#endif // !ktuil_h
