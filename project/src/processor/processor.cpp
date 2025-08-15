#include "processor.h"

u32 processor::get_core_count() {
	return KeQueryMaximumProcessorCount();
}

KAFFINITY processor::get_active_core_count() {
	return KeQueryActiveProcessors();
}

bool processor::set_active_core(u32 core) {
	if (core > processor::get_core_count())
		return false;

	KeSetSystemAffinityThread(1ULL << core);

	return true;
}

u64 processor::get_processor_block() {
	u8 disassembly_x64[256] = { };
	constexpr u32 disassembly_x64_sz = sizeof(disassembly_x64);

	memcpy(disassembly_x64, (const void*)IoFreeMdl, disassembly_x64_sz);

	/*
		ALMOSTRO:0000000140CFDCC0 ; _KPRCB *KiProcessorBlock[] // every core has one of these index it by core number
		ALMOSTRO:0000000140CFDCC0 KiProcessorBlock dq ?  

		lea rdx, KiProcessorBlock
		48 8D 15 00 00 00 00
	*/

	for (u32 idx = 0; idx < disassembly_x64_sz - 8; idx++) {
		if (disassembly_x64[idx] == 0x48 && disassembly_x64[idx + 1] == 0x8D && disassembly_x64[idx + 2] == 0x15) {
			s32 relative_offset = *(s32*)&disassembly_x64[idx + 3];
			u64 instruction_address = (u64)IoFreeMdl + idx;
			u64 address = instruction_address + 7 + relative_offset;

			if (!address || !MmIsAddressValid((void*)address))
				return 0;

			return address;
		}
	}

	return 0;
}