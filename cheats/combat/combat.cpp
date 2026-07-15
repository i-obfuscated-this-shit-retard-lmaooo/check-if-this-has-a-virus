#include "combat.h"

void rbx::cheats::combat::thread() {
	while (true) {
		if (!globals::g_running) break;
		if (globals::instances::game::curr_game == rbx::supported_game::none) { sleep_ms(100); continue; }

		{	
			// aimbot
			using namespace globals::cheats::combat::aimbot;
			if (type == 1) { // camera
				auto& io = ImGui::GetIO();
				auto dt = io.DeltaTime;
				aimbot.update(dt);
			}
			aimbot.perform_aimbot();
		}

		{
			// aimbot
			using namespace globals::cheats::combat::silent;
			silent.perform_silent();
		}

		{
			// triggerbot
			triggerbot.perform_triggerbot();
		}
	}
}