#include "hooks.h"
#include "util/log.h"
#include "util/caller.h"
#include "util/util.h"
#include "../../rage/invoker/natives.h"



char __cdecl menu::hooks::request_control(__int64 netGameEvent, __int64 sender, __int64 receiver)
{
	Player sender_id = *(__int8*)(sender + 33);
	LOG("%s Tried to control you!", native::get_player_name(sender_id));

	return true;

	return request_control_t(netGameEvent, sender, receiver);
}