#include "aimbot.h"
#include "cheats/combat/helpers.h"

bool c_aimbot::combat_checks(rbx::cache::cache_entity& player) {
	using namespace rbx::globals::cheats::combat::aimbot;
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

void c_aimbot::draw_fov() {
	using namespace rbx::globals::cheats::combat::aimbot::fov;
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

rbx::part c_aimbot::get_best_part(rbx::cache::cache_entity& entity) {
	using namespace rbx::globals::cheats::combat::aimbot;
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

bool c_aimbot::get_best_target() {
	using namespace rbx::globals::cheats::combat::aimbot;
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

rbx::matrix3_t look_at(rbx::vector3_t pos_a, rbx::vector3_t pos_b) {
	auto forward = rbx::vector3_t(pos_b - pos_a).normalize();
	auto right = rbx::vector3_t(0.f, 1.f, 0.f).cross(forward).normalize();
	auto up = forward.cross(right);
	rbx::matrix3_t result;
	result.data[0] = -right.x; result.data[1] = up.x; result.data[2] = -forward.x;
	result.data[3] = right.y; result.data[4] = up.y; result.data[5] = -forward.y;
	result.data[6] = -right.z; result.data[7] = up.z; result.data[8] = -forward.z;
	return result;
}

bool c_aimbot::aim_camera(rbx::vector3_t world_pos) {
	using namespace rbx::globals::cheats::combat::aimbot;
	using namespace rbx::globals::instances;
	auto cam = g_camera;
	auto cam_pos = cam.get_camera_position();
	auto target_rot = look_at(cam_pos, world_pos);
	if (!smoothing || smoothing_val < 0.1f) { c_transition_active = false; return cam.set_camera_rotation(target_rot); }
	c_target_world_pos = world_pos;
	if (!c_transition_active) {
		c_transition_start_rot = cam.get_camera_rotation();
		c_transition_target_rot = target_rot;
		c_transition_elapsed = 0.f;
		auto base_duration = smoothing_val * 0.02f;
		c_transition_duration = base_duration / speed;
		c_transition_active = true;
	}
	else c_transition_target_rot = target_rot;
	return true;
}

bool mouse_helper(rbx::vector2_t delta) {
	if (abs(delta.x) < 1 && abs(delta.y) < 1) return false;
	auto hdc = GetDC(rbx::globals::our_window);
	if (!hdc) return false;
	auto curr_dpi = GetDeviceCaps(hdc, LOGPIXELSX);
	ReleaseDC(rbx::globals::our_window, hdc);
	if (!curr_dpi) return false;
	auto key = HKEY{};
	auto sens = 10.f;
	if (!RegOpenKeyEx(HKEY_CURRENT_USER, "Control Panel\\Mouse", 0, KEY_READ, &key)) { 
		DWORD sz = sizeof(DWORD);
		RegQueryValueEx(key, "MouseSensitivity", 0, 0, (LPBYTE)(&sens), &sz);
		RegCloseKey(key);
	}
	auto scale = (sens / 10.f) * (curr_dpi / 96.f);
	delta *= scale;
	INPUT tuff = {};
	tuff.type = INPUT_MOUSE;
	tuff.mi.dx = (LONG)(delta.x);
	tuff.mi.dy = (LONG)(delta.y);
	tuff.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &tuff, sizeof(tuff));
	return true;
}

bool c_aimbot::aim_mouse(rbx::vector2_t screen_pos) {
	using namespace rbx::globals::cheats::combat::aimbot;
	using namespace rbx::globals::instances;

	POINT x;
	if (!GetCursorPos(&x)) return false;
	if (!ScreenToClient(rbx::globals::our_window, &x)) return false;
	rbx::vector2_t cursor = { (float)(x.x), (float)(x.y) };

	if (!smoothing) {
		auto delta = screen_pos - cursor;
		m_smoothing_enabled = false;
		return mouse_helper(delta);
	}

	if (!m_smoothing_enabled) { m_last_pos = cursor; m_smoothing_enabled = true; }

	auto& io = ImGui::GetIO();
	auto dt = io.DeltaTime;

	auto smooth_factor = 10.f / smoothing_val;
	auto lerp_factor = 1.f - exp(-smooth_factor * dt);
	m_last_pos = m_last_pos.lerp(screen_pos, lerp_factor);
	auto delta = m_last_pos - cursor;

	return mouse_helper(delta);
}

void c_aimbot::update(float dt) {
	using namespace rbx::globals::cheats::combat::aimbot;
	using namespace rbx::globals::instances;

	if (c_transition_active) {
		if (!combat_checks(target::entity)) { c_transition_active = false; return; }
		if (!target::part.addr) { c_transition_active = false; return; }
		auto cam = g_camera;
		auto cam_pos = cam.get_camera_position(); 
		c_target_world_pos = target::part.get_part_position();
		c_transition_target_rot = look_at(cam_pos, c_target_world_pos);
		c_transition_elapsed += dt;
		float t = min(c_transition_elapsed / c_transition_duration, 1.f);
		float eased_t = apply_easing(t, easing);
		auto current_rot = c_transition_start_rot.lerp(c_transition_target_rot, eased_t);
		cam.set_camera_rotation(current_rot);
		if (t >= 1.f) { c_transition_elapsed = 0.f; c_transition_start_rot = current_rot; }
	}
}

void c_aimbot::reset() {
	using namespace rbx::globals::cheats::combat::aimbot;
	using namespace rbx::globals::instances;

	auto should_aimbot = gui->is_key_enabled(&keybind, &keybind_type);

	if (!should_aimbot) {
		target::entity = {};
		target::part = {};
		c_transition_active = false;
		m_smoothing_enabled = false;
		return;
	}

	if (sticky_aim && target::entity.player.addr != 0) if (combat_checks(target::entity)) return;

	target::entity = {};
	target::part = {};
	c_transition_active = false;
}

bool c_aimbot::perform_aimbot() {
	using namespace rbx::globals::cheats::combat::aimbot;
	using namespace rbx::globals::instances;

	if (!enabled) return true;

	auto should_aimbot = gui->is_key_enabled(&keybind, &keybind_type);

	reset();

	{
		std::lock_guard<std::mutex> lock(rbx::cache::cache_mutex);
		if (!should_aimbot) return false;
		if (!get_best_target()) { reset(); return false; }
		if (!combat_checks(target::entity)) { reset(); return false; }
		auto part_pos = target::part.get_part_position();
		switch (type) {
		case 0: {
			// mouse
			auto screen_pos = g_visual_engine.world_to_screen(part_pos);
			if (screen_pos.x > g_dimensions.x || screen_pos.x < 0 ||
				screen_pos.y > g_dimensions.y || screen_pos.y < 0)
				return false;

			aim_mouse(screen_pos);
			break;
		}
		case 1: {
			// camera
			static rbx::cache::cache_entity last_target_entity;
			bool new_target = (last_target_entity.player.addr != target::entity.player.addr);

			static bool was_toggled_off = false;
			if (!should_aimbot) {
				was_toggled_off = true;
				c_transition_active = false;
				break;
			}

			if (was_toggled_off || new_target || !c_transition_active) {
				aim_camera(part_pos);
				last_target_entity = target::entity;
				was_toggled_off = false;
			}
			break;
		}
		}
	}

	return true;
}

