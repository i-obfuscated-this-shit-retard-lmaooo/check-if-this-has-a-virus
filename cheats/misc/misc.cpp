#include "misc.h"

void rbx::cheats::misc::thread() {
	while (true) {
		if (!globals::g_running) break;
		if (globals::instances::game::curr_game == rbx::supported_game::none) { sleep_ms(100); continue; }

		// tickrate
		tickrate.update();

		// animation changer
		animation_changer.update();

		// lighting
		lighting_cheat.update();

		sleep_ms(50);
	}
}