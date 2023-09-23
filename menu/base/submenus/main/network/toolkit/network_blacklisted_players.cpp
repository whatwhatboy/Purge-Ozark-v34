#include "network_blacklisted_players.h"
#include "menu/base/submenu_handler.h"
#include "../network_host_toolkit.h"
#include "menu/base/util/timers.h"
#include "menu/base/util/panels.h"

using namespace network::htoolkit::blacklist::vars;

namespace network::htoolkit::blacklist::vars {
	variables m_vars;

	void remove_player(uint64_t rockstar_id) {
	}
		
}

void network_blacklisted_players_menu::load() {
}

void network_blacklisted_players_menu::update() {  
}

void network_blacklisted_players_menu::update_once() {}

void network_blacklisted_players_menu::feature_update() {}

network_blacklisted_players_menu* g_instance;
network_blacklisted_players_menu* network_blacklisted_players_menu::get() {
	if (g_instance == nullptr) {
		g_instance = new network_blacklisted_players_menu();
		g_instance->load();
		menu::submenu::handler::add_submenu(g_instance);
	}

	return g_instance;
}