#include "silent.h"
#include "cheats/combat/helpers.h"

bool c_silent::combat_checks(rbx::cache::cache_entity& player) {
	using namespace rbx::globals::cheats::combat::silent;
	using namespace rbx::globals::instances;
	// team
	if (checks_enabled[0] && player.is_teammate) return false;
	// making sure health data isnt outdated
	if (game::curr_game == rbx::supported_game::arsenal) {
		auto& ar = player.arsenal;
		player.health = (float)(ar.health.read_value<double>());
		player.max_health = (float)(ar.max_health.read_value<double>());
		player.is_alive = player.health > 0.f;
	}
	// alive
	if (checks_enabled[1] && !player.is_alive) return false;
	//if (checks_enabled[1] && player.health <= 0.f) return false;
	// health
	if (checks_enabled[2] && player.health < health_check_amount) return false;
	// distance
	if (checks_enabled[3] && player.distance > distance_check_amount) return false;
	// vis
	//if (checks_enabled[4] && /*visible_check_logic*/) continue;
	// within fov
	if (checks_enabled[5] && fov::enabled) {
		rbx::vector2_t center;
		switch (fov::position) {
		case 0: { POINT x; if (!GetCursorPos(&x)) return false; if (!ScreenToClient(rbx::globals::our_window, &x)) return false; center = { (float)(x.x), (float)(x.y) }; break; } // mouse
		case 1: { auto sz = ImGui::GetIO().DisplaySize; center = { sz.x * 0.5f, sz.y * 0.5f }; break; }
		}

		auto idx = rbx::cache::get_part_index(hitparts[hitpart]);
		auto it = player.parts_screen[(int)(idx)];
		if (!it.first) return false;
		rbx::vector2_t part_screen = it.second;

		if (!within_fov(center, fov::size, fov::segments, part_screen)) return false;
	}
	return true;
}

void c_silent::draw_fov() {
	using namespace rbx::globals::cheats::combat::silent::fov;
	if (!enabled) return;

	ImVec2 pos = { -50, -50 };
	auto current_alpha = 255.f;
	auto current_size = size;

	if (type == 1) {
		switch (animation) {
		case 0: {
			// pulsing
			auto min_alpha = 105.f;
			auto max_alpha = 255.f;
			current_alpha = pulse_value(min_alpha, max_alpha, 3.f);
			break;
		}
		case 1: {
			// breathing
			auto min_size = size - 10.f;
			auto max_size = size + 10.f;
			current_size = pulse_value(min_size, max_size, 3.f);
			break;
		}
		}
	}

	switch (position) {
	case 0: {
		// mouse
		POINT x;
		if (!GetCursorPos(&x)) return;
		if (!ScreenToClient(rbx::globals::our_window, &x)) return;
		pos = { (float)(x.x), (float)(x.y) };
		break;
	}
	case 1: {
		// center
		auto sz = ImGui::GetIO().DisplaySize;
		pos = { sz.x / 2, sz.y / 2 };
		break;
	}
	}

	auto draw = rbx::globals::draw;

	if (outline) {
		auto outline_col = ImColor(0.f, 0.f, 0.f, current_alpha / 255.f);
		draw->AddCircle(pos, current_size + 1.f, outline_col, segments);
		draw->AddCircle(pos, current_size - 1.f, outline_col, segments);
	}

	auto col = ImColor(color.Value.x, color.Value.y, color.Value.z, current_alpha / 255.f);
	draw->AddCircle(pos, current_size, col, segments, thickness);
}

rbx::part c_silent::get_best_part(rbx::cache::cache_entity& entity) {
	using namespace rbx::globals::cheats::combat::silent;
	using namespace rbx::globals::instances;
	rbx::part best_part;
	float closest_distance = FLT_MAX;

	if (!combat_checks(entity)) return best_part;

	switch (hitpart) {
	case 0: {
		// head
		best_part = entity.head;
		break;
	}
	case 1: {
		// hrp
		best_part = entity.humanoidrootpart;
		break;
	}
	case 2: {
		// random
		std::vector<rbx::part> validparts;
		validparts.reserve((int)(rbx::cache::rig_part::count));
		for (auto i = 0; i < (int)(rbx::cache::rig_part::count); ++i) {
			auto part = entity.parts[i];
			if (!part.addr) continue;
			validparts.push_back(part);
		}
		if (!validparts.size()) break;
		auto random = get_random_val((int)(validparts.size() - 1));
		best_part = validparts[random];
		break;
	}
	case 3: {
		// closest
		POINT x;
		if (!GetCursorPos(&x)) break;
		if (!ScreenToClient(rbx::globals::our_window, &x)) break;
		rbx::vector2_t mouse = { (float)(x.x), (float)(x.y) };
		for (auto i = 0; i < (int)(rbx::cache::rig_part::count); ++i) {
			auto part = entity.parts[i];
			if (!part.addr) continue;
			auto it = entity.parts_screen[i];
			if (!it.first) continue;
			auto screen = it.second;
			auto delta = mouse - screen;
			auto distance = delta.x * delta.x + delta.y * delta.y;
			if (distance < closest_distance) {
				closest_distance = distance;
				best_part = part;
			}
		}
		break;
	}
	}

	return best_part;
}

bool c_silent::get_best_target() {
	using namespace rbx::globals::cheats::combat::silent;
	using namespace rbx::globals::instances;
	rbx::cache::cache_entity closest_entity;
	float closest_distance = FLT_MAX;
	rbx::vector2_t mouse;

	if (sticky_aim && (target::entity.player.addr != 0 && target::part.addr != 0)) { if (combat_checks(target::entity)) return true; else reset(); }

	if (target_mode == 0) {
		POINT x;
		if (!GetCursorPos(&x)) return false;
		if (!ScreenToClient(rbx::globals::our_window, &x)) return false;
		mouse = { (float)(x.x), (float)(x.y) };
	}

	for (auto& entity : rbx::cache::cached_entities) {
		if (!entity.player.addr) continue;
		if (entity.player.addr == g_local_player.addr) continue;
		rbx::part hrp = entity.humanoidrootpart;
		if (!hrp.addr) continue;

		if (!combat_checks(entity)) continue;

		switch (target_mode) {
		case 0: {
			// closest to mouse
			auto idx = rbx::cache::get_part_index("HumanoidRootPart");
			auto it = entity.parts_screen[(int)(idx)];
			if (!it.first) continue;
			auto pos = it.second;

			auto delta = mouse - pos;
			auto distance = delta.x * delta.x + delta.y * delta.y;
			if (distance < closest_distance) {
				closest_distance = distance;
				closest_entity = entity;
			}
			break;
		}
		case 1: {
			// closest to camera
			auto cam = g_camera;
			auto cam_pos = cam.get_camera_position();
			auto distance = hrp.get_part_position().distance(cam_pos);
			if (distance < closest_distance) {
				closest_distance = distance;
				closest_entity = entity;
			}
			break;
		}
		case 2: {
			// closest ingame
			auto local_hrp = g_local_player.get_humanoid_root_part();
			auto local_hrp_pos = local_hrp.get_part_position();
			auto distance = hrp.get_part_position().distance(local_hrp_pos);
			if (distance < closest_distance) {
				closest_distance = distance;
				closest_entity = entity;
			}
			break;
		}
		}
	}

	if (!closest_entity.player.addr) return false;

	target::entity = closest_entity;
	target::part = get_best_part(closest_entity);

	if (!target::entity.player.addr || !target::part.addr) return false;

	return true;
}

bool c_silent::aim_mouse(rbx::vector2_t screen) {
	using namespace rbx::globals::cheats::combat::silent;
	using namespace rbx::globals::instances;
	if (!g_mouse_service.get_input_object().addr) return false;
	return g_mouse_service.set_mouse_pos(screen);
}

bool c_silent::aim_camera(rbx::vector2_t screen) {
	using namespace rbx::globals::cheats::combat::silent;
	using namespace rbx::globals::instances;
	if (!g_camera.addr) return false;
	auto target_viewport = g_visual_engine.calculate_viewport(screen, g_dimensions);
	return g_camera.set_viewport_size(target_viewport);
}

void c_silent::reset() {
	using namespace rbx::globals::cheats::combat::silent;
	using namespace rbx::globals::instances;

	auto should_silent = gui->is_key_enabled(&keybind, &keybind_type);

	if (!should_silent) {
		target::entity = {};
		target::part = {};
		return;
	}

	if (sticky_aim && target::entity.player.addr != 0) if (combat_checks(target::entity)) return;

	target::entity = {};
	target::part = {};
}

bool c_silent::perform_silent() {
	using namespace rbx::globals::cheats::combat::silent;
	using namespace rbx::globals::instances;

	if (!enabled) return true;

	auto should_silent = gui->is_key_enabled(&keybind, &keybind_type);

	reset();

	{
		std::lock_guard<std::mutex> lock(rbx::cache::cache_mutex);
		if (!should_silent) return false;
		if (!get_best_target()) { reset(); return false; }
		if (!combat_checks(target::entity)) { reset(); return false; }
		auto world = target::part.get_part_position();
		auto screen = g_visual_engine.world_to_screen(world);
		if (screen.x > g_dimensions.x || screen.x < 0 || screen.y > g_dimensions.y || screen.y < 0) return false;
		if (type == 0) return aim_mouse(screen);
		else if (type == 1) return aim_camera(screen);
		else return false;
	}

	return true;
}

