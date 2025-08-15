#ifndef processor_h
#define processor_h

#include "../include.h"

namespace processor {
	u32 get_core_count();
	KAFFINITY get_active_core_count();
	bool set_active_core(u32 core);
	u64 get_processor_block();
}

#endif // !processor_h