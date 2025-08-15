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