#include "hooks.h"
#include "menu/base/base.h"
#include "util/util.h"
#include "util/log.h"

const char* menu::hooks::get_gxt_label(uint64_t table, const char* label) {

	if (strcmp("PM_SOLO_FM", label) == 0)
		return "Purge Solo Session";

	if (strcmp("PM_NCREW_FM", label) == 0)
		return "Crew Session With Purge";

	if (strcmp("PM_CREW_FM", label) == 0)
		return "Closed Crew Session With Purge";

	if (strcmp("PM_FRIEND_FM", label) == 0)
		return "Closed Friend Session With Purge";

	if (strcmp("CTALERT_F_1", label) == 0)
		return "Game Alert";

	if (strcmp("LOADING_SPLAYER_L", label) == 0)
		return "Loading GTA with Purge";

	if (strcmp("LOADING_MPLAYER_L", label) == 0)
		return "Loading GTA Online with Purge";

	if (strcmp("PM_INF_PGOT", label) == 0)
		return "Play GTA Online With Purge";

	if (strcmp("PM_ENTER_MP", label) == 0)
		return "Launch Online With Purge";

	if (strcmp("HUD_JOINING", label) == 0)
		return "Loading GTA Online with Purge";

	if (strcmp("PM_INVO_FM", label) == 0)
		return "Launch Invite only With Purge";

	if (strcmp("PM_INF_PGOT5", label) == 0)
		return "Purge Invite only session";

	if (strcmp("PM_INF_PGOT0", label) == 0)
		return "Go with Purge";

	if (strcmp("PM_INF_PGOB0", label) == 0)
		return "Head straight into GTA Online with the Purge Menu.";

	if (strcmp("HUD_QUITTING", label) == 0)
		return "Loading Purge for Singleplayer";

	if (strcmp("PM_FIND_SESS", label) == 0)
		return "Find a new session with Purge";

	if (strcmp("HUD_MPREENTER", label) == 0)
		return "Joining a new GTA Online session with Purge";

	if (strcmp("PM_QUIT_MP", label) == 0)
		return "Leave GTA Online with Purge";

	if (strcmp("NT_INV_CONFIG", label) == 0)
		return "Getting GTA Online session details with Purge";

	if (strcmp("HUD_BAIL4", label) == 0)
		return "Failed to find a compatible GTA Online session with Purge";

	if (strcmp("HUD_JOINEVENT", label) == 0)
		return "Joining GTA Online Event with Purge";

	if (strcmp("PM_FRESES", label) == 0)
		return "Join Friends with Purge";

	if (strcmp("PM_GO", label) == 0)
		return "Join Open Lobby with Purge";

	if (strcmp("HUD_TRANSPEND", label) == 0)
		return "Transfer Pending with Purge";

	if (strcmp("HUD_TRANSP", label) == 0)
		return "Transaction Pending with Purge";

	if (strcmp("PM_QUIT_GAME", label) == 0)
		return "Are you sure you want to stop playing with Purge?";


	return get_gxt_label_t(table, label);
}

const char* menu::hooks::get_gxt_label_from_hash(uint64_t table, uint32_t label_hash) {

	

	return get_gxt_label_from_hash_t(table, label_hash);
}