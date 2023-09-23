#include "hooks.h"
#include "util/log.h"

FARPROC menu::hooks::get_proc_address(HMODULE mod, LPCSTR name) {
	if (mod == (HMODULE)0x63686970) {
		


		LOG_CUSTOM_WARN("ScriptHook", "Failed to verify %s", name);
	}

	return get_proc_address_t(mod, name);
}