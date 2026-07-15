#include "visuals.h"

bool get_player_bounds(rbx::cache::cache_entity& player, int type) {
	using namespace rbx::globals::instances;

	if (type == 0) { // static
		auto head = player.head.get_part_position();
		auto hrp = player.humanoidrootpart.get_part_position();
		head.y += 1.f;
		hrp.y -= 0.4f;
		auto hpos = g_visual_engine.world_to_screen(head);
		auto hrppos = g_visual_engine.world_to_screen(hrp);
		auto width = abs(hpos.y - hrppos.y) * 0.8f;
		auto height = abs(hpos.y - hrppos.y) * 2.f;
		player.top_left = { hrppos.x - width, hpos.y };
		player.bottom_right = { hrppos.x + width, hpos.y + height };
	}
	else if (type == 1) { // bounding
		static const rbx::vector3_t vertices[8] = {
			rbx::vector3_t(1, 1, 1), rbx::vector3_t(1, 1, -1),
			rbx::vector3_t(1, -1, 1), rbx::vector3_t(1, -1, -1),
			rbx::vector3_t(-1, 1, 1), rbx::vector3_t(-1, 1, -1),
			rbx::vector3_t(-1, -1, 1), rbx::vector3_t(-1, -1, -1)
		};

		auto min_x = FLT_MAX, min_y = FLT_MAX, max_x = -FLT_MAX, max_y = -FLT_MAX;
		auto valid = false;

		for (auto index = 0; index < (int)(rbx::cache::rig_part::count); ++index) {
			auto& part = player.parts[index];
			if (!part.addr) continue;

			auto cframe = part.get_part_cframe();
			auto& position = cframe.position;
			auto& rotation = cframe.rotation;
			auto size = player.partsizes[index];

			for (auto vertex : vertices) {
				auto world = position + rotation * rbx::vector3_t(vertex.x * size.x / 2, vertex.y * size.y / 2, vertex.z * size.z / 2);
				auto screen = g_visual_engine.world_to_screen(world);
				if (!g_visual_engine.on_screen(screen)) continue;

				min_x = min(min_x, screen.x);
				min_y = min(min_y, screen.y);
				max_x = max(max_x, screen.x);
				max_y = max(max_y, screen.y);

				valid = true;
			}
		}

		if (!valid) return false;

		player.top_left = { min_x, min_y };
		player.bottom_right = { max_x, max_y };
	}
	else {
		player.top_left = {};
		player.bottom_right = {};
		return false;
	}

	return true;
}

bool calculate_straw_hat(rbx::cache::cache_entity& player) {
	using namespace rbx::globals::instances;
	auto head = player.head;
	if (!head.addr) {
		//printf("failed to get head - %s\n", player.name.c_str());
		return false;
	}
	auto head_pos = head.get_part_position();
	auto head_size = head.get_part_size();
	auto head_screen = g_visual_engine.world_to_screen(head_pos);
	if (!g_visual_engine.on_screen(head_screen)) {
		//printf("head not visible - %s\n", player.name.c_str());
		return false;
	}
	auto cam_pos = g_camera.get_camera_position();
	auto distance = cam_pos.distance(head_pos);
	auto scale = std::clamp(80.f / distance, 0.5f, 2.f);
	auto rad = scale * 0.85f;
	auto top = head_pos;
	top.y += head_size.y;
	auto center = head_pos;
	center.y += head_size.y * 0.4f;
	auto top_screen = g_visual_engine.world_to_screen(top);
	if (!g_visual_engine.on_screen(top_screen)) {
		//printf("head top not visible - %s\n", player.name.c_str());
		return false;
	}
	auto s = rbx::globals::cheats::visuals::straw_hat::segments;
	std::vector<rbx::vector3_t> brim;
	brim.reserve(s);
	for (auto i = 0; i < s; ++i) {
		auto theta = (2.f * IM_PI * i) / s;
		auto b = center;
		b.x += rad * cos(theta);
		b.z += rad * sin(theta);
		auto screen = g_visual_engine.world_to_screen(b);
		if (!g_visual_engine.on_screen(screen)) {
			//printf("brim not visible - %s\n", player.name.c_str());
			return false;
		}
		brim.push_back(b);
	}
	if (brim.size() < 3) {
		//printf("brim too small - %s\n", player.name.c_str());
		return false;
	}
	for (auto i = 0; i < brim.size(); ++i) {
		auto j = (i + 1) % brim.size(); // if i + 1 == brim.size(), j = brim.size() % brim.size() = 0 thus loop
		player.hat.circumference_lines.push_back({ brim[i], brim[j] });
	}
	for (auto p : brim) player.hat.connection_lines.push_back({ p, top });
	return true;
}

bool get_player_bounds_3d(rbx::cache::cache_entity& player) {
	using namespace rbx::globals::instances;

	static const rbx::vector3_t vertices[8] = {
		rbx::vector3_t(1, 1, 1), rbx::vector3_t(1, 1, -1),
		rbx::vector3_t(1, -1, 1), rbx::vector3_t(1, -1, -1),
		rbx::vector3_t(-1, 1, 1), rbx::vector3_t(-1, 1, -1),
		rbx::vector3_t(-1, -1, 1), rbx::vector3_t(-1, -1, -1)
	};

	auto min_x = FLT_MAX, min_y = FLT_MAX, min_z = FLT_MAX,
		 max_x = -FLT_MAX, max_y = -FLT_MAX, max_z = -FLT_MAX;
	auto valid = false;

	for (auto index = 0; index < (int)(rbx::cache::rig_part::count); ++index) {
		auto& part = player.parts[index];
		if (!part.addr) continue;

		auto cframe = part.get_part_cframe();
		auto& position = cframe.position;
		auto& rotation = cframe.rotation;
		auto size = player.partsizes[index];

		for (auto vertex : vertices) {
			auto world = position + rotation * rbx::vector3_t(vertex.x * size.x / 2, vertex.y * size.y / 2, vertex.z * size.z / 2);
			auto screen = g_visual_engine.world_to_screen(world);
			if (!g_visual_engine.on_screen(screen)) continue;

			min_x = min(min_x, world.x);
			min_y = min(min_y, world.y);
			min_z = min(min_z, world.z);
			max_x = max(max_x, world.x);
			max_y = max(max_y, world.y);
			max_z = max(max_z, world.z);

			valid = true;
		}
	}

	if (!valid) return false;

	auto w = std::abs(min_x - max_x) * 0.5f;
	auto h = std::abs(min_y - max_y) * 0.5f;
	auto d = std::abs(min_z - max_z) * 0.5f;

	auto center = rbx::vector3_t((min_x + max_x) * 0.5f, (min_y + max_y) * 0.5f, (min_z + max_z) * 0.5f);

	auto front_top_left     = center + rbx::vector3_t(-w, h, d);
	auto front_top_right    = center + rbx::vector3_t(w, h, d);
	auto front_bottom_left  = center + rbx::vector3_t(-w, -h, d);
	auto front_bottom_right = center + rbx::vector3_t(w, -h, d);

	auto back_top_left     = center + rbx::vector3_t(-w, h, -d);
	auto back_top_right    = center + rbx::vector3_t(w, h, -d);
	auto back_bottom_left  = center + rbx::vector3_t(-w, -h, -d);
	auto back_bottom_right = center + rbx::vector3_t(w, -h, -d);

	auto ftl_s = g_visual_engine.world_to_screen(front_top_left);
	auto ftr_s = g_visual_engine.world_to_screen(front_top_right);
	auto fbl_s = g_visual_engine.world_to_screen(front_bottom_left);
	auto fbr_s = g_visual_engine.world_to_screen(front_bottom_right);

	auto btl_s = g_visual_engine.world_to_screen(back_top_left);
	auto btr_s = g_visual_engine.world_to_screen(back_top_right);
	auto bbl_s = g_visual_engine.world_to_screen(back_bottom_left);
	auto bbr_s = g_visual_engine.world_to_screen(back_bottom_right);

	auto ftl_os = g_visual_engine.on_screen(ftl_s);
	auto ftr_os = g_visual_engine.on_screen(ftr_s);
	auto fbl_os = g_visual_engine.on_screen(fbl_s);
	auto fbr_os = g_visual_engine.on_screen(fbr_s);

	auto btl_os = g_visual_engine.on_screen(btl_s);
	auto btr_os = g_visual_engine.on_screen(btr_s);
	auto bbl_os = g_visual_engine.on_screen(bbl_s);
	auto bbr_os = g_visual_engine.on_screen(bbr_s);

	player.box_3d_points.clear();
	player.box_3d_points.reserve(8);
	player.box_3d_points.push_back({ ftl_os, ftl_s });
	player.box_3d_points.push_back({ ftr_os, ftr_s });
	player.box_3d_points.push_back({ fbl_os, fbl_s });
	player.box_3d_points.push_back({ fbr_os, fbr_s });
	player.box_3d_points.push_back({ btl_os, btl_s });
	player.box_3d_points.push_back({ btr_os, btr_s });
	player.box_3d_points.push_back({ bbl_os, bbl_s });
	player.box_3d_points.push_back({ bbr_os, bbr_s });

	// 3d box wireframe:
	// 0 - 1
	// 1 - 5
	// 5 - 4
	// 4 - 0
	// 
	// 2 - 3
	// 3 - 7
	// 7 - 6
	// 6 - 2
	//
	// 0 - 2
	// 1 - 3
	// 4 - 6
	// 5 - 7

	// 3d box faces:
	// 
	// top face
	// 0 - 1
	// 1 - 5
	// 5 - 4
	// 4 - 0
	//
	// bottom face
	// 2 - 3
	// 3 - 7
	// 7 - 6
	// 6 - 2
	//
	// front face
	// 0 - 2
	// 2 - 3
	// 3 - 1
	// 1 - 0
	//
	// back face
	// 4 - 6
	// 6 - 7
	// 7 - 5
	// 5 - 4
	//
	// right face
	// 1 - 3
	// 3 - 7
	// 7 - 5
	// 5 - 1
	//
	// left face
	// 0 - 2
	// 2 - 6
	// 6 - 4
	// 4 - 0

	return true;
}

bool player_checks(rbx::cache::cache_entity& player) {
	using namespace rbx::globals::cheats::visuals;
	using namespace rbx::globals::instances;
	if (!self && player.player.addr == g_local_player.addr) return false;
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
	//if (checks_enabled[1] && && player.health <= 0.f) return false;
	// health
	if (checks_enabled[2] && player.health < health_check_amount) return false;
	// distance
	if (checks_enabled[3] && player.distance > distance_check_amount) return false;
	// vis
	//if (checks_enabled[4] && /*visible_check_logic*/) return false;
	return true;
}

bool get_player_cham_part_data(rbx::cache::cache_entity& player) {
	using namespace rbx::globals::instances;

	static const rbx::vector3_t corners[8] = {
		{-1,-1,-1}, {1,-1,-1}, {-1,1,-1}, {1,1,-1},
		{-1,-1, 1}, {1,-1, 1}, {-1,1, 1}, {1,1, 1}
	};

	auto valid = false;
	for (auto index = 0; index < (int)(rbx::cache::rig_part::count); ++index) {
		auto& projected = player.chams_screen[index];
		projected.clear();

		auto& part = player.parts[index];
		if (!part.addr) continue;

		auto cframe = part.get_part_cframe();
		auto& pos = cframe.position;
		auto& rot = cframe.rotation;
		auto size = player.partsizes[index];
		projected.reserve(8);

		for (auto& corner : corners) {
			auto world = pos + rot * rbx::vector3_t(
				corner.x * size.x * 0.5f,
				corner.y * size.y * 0.5f,
				corner.z * size.z * 0.5f
			);
			auto screen = g_visual_engine.world_to_screen(world);
			projected.push_back({ g_visual_engine.on_screen(screen), screen });
		}
		valid = true;
	}

	return valid;
}

void rbx::cheats::visuals::thread() {
	crosshair.draw_crosshair();
	aimbot.draw_fov();
	silent.draw_fov();
	std::vector<rbx::cache::cache_entity> local;
	{
		std::lock_guard<std::mutex> lock(rbx::cache::cache_mutex);
		local = rbx::cache::cached_entities;
	}
	if (globals::instances::game::curr_game == rbx::supported_game::none) return;
	ImGui::PushFont(rbx::globals::fonts::visitor_s);
	if (!local.size()) { ImGui::PopFont(); return; }
	for (auto& player : local) {
		if (!player_checks(player)) continue;
		if (!get_player_bounds(player, rbx::globals::cheats::visuals::box::type)) continue;
		if (!get_player_bounds_3d(player)) continue;
		if (rbx::globals::chams_caching.load(std::memory_order::relaxed))
			if (!get_player_cham_part_data(player)) continue;
		if (!calculate_straw_hat(player)) continue;
		basic_features.draw(player);
	}
	rbx::cheats::visuals::basic_features.draw_locked_indicator();
	ImGui::PopFont();
}

