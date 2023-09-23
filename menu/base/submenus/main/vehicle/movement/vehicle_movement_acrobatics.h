#pragma once
#include "menu/base/submenu.h"

class vehicle_movement_acrobatics_menu : public menu::submenu::submenu {
public:
	static vehicle_movement_acrobatics_menu* get();

	void load() override;
	void update_once() override;
	void update() override;
	void feature_update() override;

	vehicle_movement_acrobatics_menu()
		: menu::submenu::submenu() {}
};

namespace vehicle::movement::acrobatics::vars {
	struct variables {

	};

	extern variables m_vars;
}