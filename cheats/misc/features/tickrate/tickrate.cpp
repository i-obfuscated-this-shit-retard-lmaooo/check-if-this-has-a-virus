#include "tickrate.h"

bool c_tickrate::set_tickrate(float tickrate) {
	// tickrate is input from slider so * 4
	using namespace rbx::globals::instances;
	return g_world.set_tickrate(tickrate * 4.f);
}

bool c_tickrate::update() {
	using namespace rbx::globals::cheats::misc::tickrate;
	if (!enabled) return false;
	if (gui->is_key_enabled(&keybind, &keybind_type)) set_tickrate(value);
	else set_tickrate(60.f);
	return true;
}