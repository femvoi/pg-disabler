#include "src/include.h"

extern "C" NTSTATUS FxDriverEntry(DRIVER_OBJECT* driver_object, UNICODE_STRING* registry_path) {
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);

	return STATUS_SUCCESS;
}