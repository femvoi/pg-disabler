#ifndef timer_h
#define timer_h

#include "../include.h"

namespace timer {
	// guessing these are signed ?
	inline s64 KiWaitNever = 0;
	inline s64 KiWaitAlways = 0;

	NTSTATUS initialize_encryption_values();
	PKDPC decrypt_dpc_from_timer(const PKTIMER timer);
}

#endif