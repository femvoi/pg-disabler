#pragma once
#include <ntifs.h>
#include <ntddk.h>
#include <intrin.h>

namespace hook {
	inline bool write_to_read_only_memory(void* address, void* buffer, const size_t size) {
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
		unsigned char original_bytes[12];
		unsigned char jump_bytes[12];
	};

	hook_data hooks[64] = {};

}