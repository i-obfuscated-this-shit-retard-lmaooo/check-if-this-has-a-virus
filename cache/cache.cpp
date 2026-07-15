#include "cache.h"
#include "misc/globals.h"

// old random shit
/*
// old method
bool get_player_bounds(rbx::entity& player, rbx::vector2_t& top_left, rbx::vector2_t& bottom_right) {
	std::vector<rbx::part> parts;
	auto character = player.get_character();
	if (!character.addr) return false;

	auto children = character.get_children();
	if (children.empty()) return false;
	for (auto c : children) {
		auto class_name = c.get_class_name();
		if (class_name == "Part" || class_name == "MeshPart") parts.emplace_back(rbx::part(c.addr));
	}

	if (parts.empty()) return false;

	auto min_x = FLT_MAX, min_y = FLT_MAX, max_x = -FLT_MAX, max_y = -FLT_MAX;
	auto valid = false;

	for (auto part : parts) {
		auto position = part.get_part_position();
		if (position.magnitude() == 0) continue;
		auto screen = rbx::globals::instances::g_visual_engine.world_to_screen(position);
		if (screen.x == -1 && screen.y == -1) continue;

		min_x = min(min_x, screen.x);
		min_y = min(min_y, screen.y);
		max_x = max(max_x, screen.x);
		max_y = max(max_y, screen.y);

		valid = true;
	}

	if (!valid) return false;

	auto hrp = character.find_first_child("HumanoidRootPart");
	auto head = character.find_first_child("Head");
	if (!hrp.addr || !head.addr) return false;
	auto hrp_pos = rbx::globals::instances::g_visual_engine.world_to_screen(rbx::part(hrp.addr).get_part_position());
	auto head_pos = rbx::globals::instances::g_visual_engine.world_to_screen(rbx::part(head.addr).get_part_position());

	auto margin = std::abs(hrp_pos.y - head_pos.y);

	top_left = { min_x - margin / 2, min_y - margin };
	bottom_right = { max_x + margin / 2, max_y + margin };

	return true;
}
*/

/*
bool get_player_bounds_3d(rbx::cache::cache_entity& player, std::vector<rbx::vector2_t>& screen_points) {
	using namespace rbx::globals::instances;

	screen_points.clear();

	// vertices of a 2x2x2 cube lying on origin (0, 0, 0)
	std::vector<rbx::vector3_t> local_vertices = {
		rbx::vector3_t(1, 1, 1),
		rbx::vector3_t(1, 1, -1),
		rbx::vector3_t(1, -1, 1),
		rbx::vector3_t(1, -1, -1),
		rbx::vector3_t(-1, 1, 1),
		rbx::vector3_t(-1, 1, -1),
		rbx::vector3_t(-1, -1, 1),
		rbx::vector3_t(-1, -1, -1),
	};

	if (player.parts.empty()) return false;

	rbx::vector3_t _min = { FLT_MAX, FLT_MAX, FLT_MAX };
	rbx::vector3_t _max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	auto valid = false;

	for (auto part_table : player.parts) {
		auto part = part_table.second;
		if (!part.addr) continue;

		auto position = part.get_part_position();
		auto rotation = part.get_part_rotation();
		auto size = part.get_part_size();

		for (auto vertex : local_vertices) {
			auto world = position + rotation * rbx::vector3_t(vertex.x * size.x / 2, vertex.y * size.y / 2, vertex.z * size.z / 2);
			auto screen = g_visual_engine.world_to_screen(world);
			if (!g_visual_engine.on_screen(screen)) continue;

			_min = {
				min(_min.x, world.x),
				min(_min.y, world.y),
				min(_min.z, world.z),
			};

			_max = {
				max(_max.x, world.x),
				max(_max.y, world.y),
				max(_max.z, world.z),
			};

			valid = true;
		}
	}

	if (!valid) return false;

	auto sx = std::abs(_max.x - _min.x);
	auto sy = std::abs(_max.y - _min.y);
	auto sz = std::abs(_max.z - _min.z);

	rbx::vector3_t center = {
		_min.x + (sx / 2),
		_min.y + (sy / 2),
		_min.z + (sz / 2),
	};

	for (auto v : local_vertices) {
		auto world = center + rbx::vector3_t(v.x * sx / 2, v.y * sy / 2, v.z * sz / 2);
		auto screen = g_visual_engine.world_to_screen(world);
		if (!g_visual_engine.on_screen(screen)) continue;
		screen_points.push_back(screen);
	}

	return true;
}
*/

/*
bool calculate_china_hat(rbx::cache::cache_entity& player, rbx::cache::china_hat& hat) {
	auto it = player.parts.find("Head");
	auto head = it->second;
	if (!head.addr) {
		//printf("failed to get head - %s\n", player.name.c_str());
		return false;
	}
	auto head_pos = head.get_part_position();
	auto head_size = head.get_part_size();
	auto head_screen = rbx::globals::instances::g_visual_engine.world_to_screen(head_pos);
	if (!rbx::globals::instances::g_visual_engine.on_screen(head_screen)) {
		//printf("head not visible - %s\n", player.name.c_str());
		return false;
	}
	auto cam = rbx::globals::instances::g_workspace.get_current_camera();
	auto cam_pos = cam.get_camera_position();
	auto distance = cam_pos.distance(head_pos);
	auto scale = std::clamp(100.f / distance, 0.5f, 2.f);
	auto rad = head_size.x * 0.8f * scale;
	auto top = head_pos;
	top.y += head_size.y;
	auto center = head_pos;
	center.y += head_size.y * 0.3f;
	auto top_screen = rbx::globals::instances::g_visual_engine.world_to_screen(top);
	if (!rbx::globals::instances::g_visual_engine.on_screen(top_screen)) {
		//printf("head top not visible - %s\n", player.name.c_str());
		return false;
	}
	auto s = rbx::globals::cheats::visuals::china_hat::segments;
	std::vector<rbx::vector2_t> brim;
	brim.reserve(s);
	for (auto i = 0; i < s; ++i) {
		auto theta = (2.f * IM_PI * i) / s;
		auto b = center;
		b.x += rad * cos(theta);
		b.z += rad * sin(theta);
		auto screen = rbx::globals::instances::g_visual_engine.world_to_screen(b);
		if (!rbx::globals::instances::g_visual_engine.on_screen(screen)) {
			//printf("brim not visible - %s\n", player.name.c_str());
			return false;
		}
		brim.push_back(screen);
	}
	if (brim.size() < 3) {
		//printf("brim too small - %s\n", player.name.c_str());
		return false;
	}
	for (auto i = 0; i < brim.size(); ++i) {
		auto j = (i + 1) % brim.size(); // if i + 1 == brim.size(), j = brim.size() % brim.size() = 0 thus loop
		hat.circumference_lines.push_back({ brim[i], brim[j] });
	}
	for (auto p : brim) hat.connection_lines.push_back({ p, top_screen });
	return true;
}
*/

// moved to visuals
/*
// new method - axis aligned bounding box (aabb)
bool get_player_bounds(rbx::cache::cache_entity& player) {
	using namespace rbx::globals::instances;

	// vertices of a 2x2x2 cube lying on origin (0, 0, 0)
	std::vector<rbx::vector3_t> local_vertices = {
		rbx::vector3_t(1, 1, 1),
		rbx::vector3_t(1, 1, -1),
		rbx::vector3_t(1, -1, 1),
		rbx::vector3_t(1, -1, -1),
		rbx::vector3_t(-1, 1, 1),
		rbx::vector3_t(-1, 1, -1),
		rbx::vector3_t(-1, -1, 1),
		rbx::vector3_t(-1, -1, -1),
	};

	if (player.parts.empty()) return false;

	auto min_x = FLT_MAX, min_y = FLT_MAX, max_x = -FLT_MAX, max_y = -FLT_MAX;
	auto valid = false;

	for (auto part_table : player.parts) {
		auto part = part_table.second;
		if (!part.addr) continue;

		auto position = part.get_part_position();
		auto rotation = part.get_part_rotation();
		auto size = part.get_part_size();

		for (auto vertex : local_vertices) {
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

	return true;
}

bool calculate_straw_hat(rbx::cache::cache_entity& player) {
	using namespace rbx::globals::instances;
	auto it = player.parts.find("Head");
	auto head = it->second;
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
*/

int get_waifu_hash(std::string name) {
	auto hash = 0;
	for (auto c : name) hash = hash + 31 * c % 10007;
	return abs(hash) % 4;
}

rbx::cache::rig_part rbx::cache::get_part_index(const std::string& name) {
	using namespace rbx::cache;
	if (name == "Head") return rig_part::head;
	if (name == "HumanoidRootPart") return rig_part::humanoidrootpart;
	if (name == "Torso") return rig_part::torso;
	if (name == "Right Arm") return rig_part::right_arm;
	if (name == "Left Arm") return rig_part::left_arm;
	if (name == "Right Leg") return rig_part::right_leg;
	if (name == "Left Leg") return rig_part::left_leg;

	// r15 only
	if (name == "UpperTorso") return rig_part::upper_torso;
	if (name == "LowerTorso") return rig_part::lower_torso;
	if (name == "RightUpperArm") return rig_part::right_upper_arm;
	if (name == "RightLowerArm") return rig_part::right_lower_arm;
	if (name == "RightHand") return rig_part::right_hand;
	if (name == "LeftUpperArm") return rig_part::left_upper_arm;
	if (name == "LeftLowerArm") return rig_part::left_lower_arm;
	if (name == "LeftHand") return rig_part::left_hand;
	if (name == "RightUpperLeg") return rig_part::right_upper_leg;
	if (name == "RightLowerLeg") return rig_part::right_lower_leg;
	if (name == "RightFoot") return rig_part::right_foot;
	if (name == "LeftUpperLeg") return rig_part::left_upper_leg;
	if (name == "LeftLowerLeg") return rig_part::left_lower_leg;
	if (name == "LeftFoot") return rig_part::left_foot;

	return rig_part::count;
}

/*
bool get_player_parts(rbx::cache::cache_entity& player) {
	using namespace rbx;
	using namespace rbx::globals::instances;
	auto character = player.player.get_character();
	if (!character.addr) return false;
	auto children = character.get_children();
	if (children.empty()) return false;
	bool any = false;
	for (auto& p : children) {
		if (!p.addr) continue;
		auto part = rbx::part(p.addr);
		auto class_name = part.get_class_name();
		if (class_name == "Part" || class_name == "MeshPart") {
			auto pos = part.get_part_position();
			auto screen = g_visual_engine.world_to_screen(pos);
			auto valid = g_visual_engine.on_screen(screen);
			auto name = part.get_name();
			if (name == "Head") player.head = part;
			if (name == "Humanoid") player.humanoid = part;
			if (name == "HumanoidRootPart") player.humanoidrootpart = part;
			player.parts.insert({ name, part });
			player.parts_screen.insert({ name, std::pair<bool, rbx::vector2_t>(valid, screen) });
			any = true;
		}
	}
	return any;
}
*/

bool get_player_parts(rbx::cache::cache_entity& player) {
	using namespace rbx;
	using namespace rbx::globals::instances;
	auto character = player.player.get_character();
	if (!character.addr) return false;
	auto children = character.get_children();
	if (children.empty()) return false;
	auto any = false;
	for (auto& child : children) {
		if (!child.addr) continue;
		auto part = rbx::part(child.addr);
		auto partid = rbx::cache::get_part_index(part.get_name());
		if (partid == rbx::cache::rig_part::count) continue;

		auto index = (int)(partid);
		player.parts[index] = part;
		player.partsizes[index] = part.get_part_size();

		if (partid == rbx::cache::rig_part::head) player.head = part;
		if (partid == rbx::cache::rig_part::humanoidrootpart) player.humanoidrootpart = part;
		any = true;
	}
	return any;
}

bool update_player_parts_positions(rbx::cache::cache_entity& player) {
	using namespace rbx;
	using namespace rbx::globals::instances;
	auto any = false;
	for (auto index = 0; index < (int)(rbx::cache::rig_part::count); ++index) {
		auto& part = player.parts[index];
		if (!part.addr) {
			player.parts_screen[index] = { false, {} };
			continue;
		}

		auto cframe = part.get_part_cframe();
		auto screen = g_visual_engine.world_to_screen(cframe.position);
		auto valid = g_visual_engine.on_screen(screen);
		player.parts_screen[index] = { valid, screen };
		player.partsizes[index] = part.get_part_size();

		if (index == (int)(rbx::cache::rig_part::head)) player.head = part;
		if (index == (int)(rbx::cache::rig_part::humanoidrootpart)) player.humanoidrootpart = part;
		any = true;
	}
	return any;
}

void rbx::cache::thread() {
	using namespace rbx::globals::instances;
	while (true) {
		if (!globals::g_running) break;
		if (globals::instances::game::curr_game == rbx::supported_game::none) { sleep_ms(100); continue; }

		auto players = g_players.get_active_players();
		if (!players.size()) { sleep_ms(100); continue; }

		static std::vector<cache_entity> local;
		local.clear();
		local.reserve(players.size());

		vector3_t local_position;
		auto lp_hrp = g_local_player.get_humanoid_root_part();
		if (lp_hrp.addr) local_position = lp_hrp.get_part_position();
		else local_position = g_camera.get_camera_position();

		for (auto& player : players) {
			if (!player.addr) continue;

			auto character = player.get_character();
			if (!character.addr) continue;

			auto humanoid = player.get_humanoid();
			if (!humanoid.addr) continue;

			auto humanoid_root_part = player.get_humanoid_root_part();
			if (!humanoid_root_part.addr) continue;

			auto screen = g_visual_engine.world_to_screen(humanoid_root_part.get_part_position());
			if (!g_visual_engine.on_screen(screen)) continue;

			cache_entity entity;
			entity.player = player;
			entity.name = rbx::globals::cheats::visuals::name::type == 0 ? player.get_name() : player.get_display_name();
			std::string tool_name;
			auto tool = character.find_first_child_of_class("Tool");
			if (!tool.addr) tool_name = "none"; else tool_name = tool.get_name();
			entity.tool = std::format("[{}]", tool_name);
			entity.team = player.get_team();
			entity.team_name = entity.team.addr ? entity.team.get_name() : "Neutral";
			entity.is_teammate = player.is_teammate();
			entity.distance = humanoid_root_part.get_part_position().distance(local_position);
			entity.health = player.get_health();
			entity.is_alive = entity.health > 0.f;
			entity.max_health = player.get_max_health();
			entity.move_direction = player.get_move_direction();
			entity.rig = player.get_rig_type();
			entity.state_id = player.get_humanoid_state_id();
			entity.waifu_idx = get_waifu_hash(entity.name);

			entity.humanoid = humanoid;
			entity.humanoidrootpart = humanoid_root_part;

			if (!get_player_parts(entity)) continue;
			if (!update_player_parts_positions(entity)) continue;
//			if (!get_player_bounds(entity)) continue;
//			if (!calculate_straw_hat(entity)) continue;

			switch (game::curr_game) {
			case rbx::supported_game::arsenal: {
				auto& ar = entity.arsenal;
				ar.nrpbs = player.find_first_child("NRPBS");
				ar.health = value_object(ar.nrpbs.find_first_child("Health").addr);
				ar.max_health = value_object(ar.nrpbs.find_first_child("MaxHealth").addr);
				entity.health = (float)(ar.health.read_value<double>());
				entity.max_health = (float)(ar.max_health.read_value<double>());
				entity.is_alive = entity.health > 0.f;
				ar.status = player.find_first_child("Status");
				entity.team = ar.status.find_first_child("Team");
				ar.team = value_object(entity.team);
				entity.team_name = ar.team_name = ar.team.read_string_value();
				break;
			}
			case rbx::supported_game::rivals: {
				
				break;
			}
			case rbx::supported_game::bad_business:
				break;
			case rbx::supported_game::da_hood:
				break;
			case rbx::supported_game::hood_customs:
				break;
			case rbx::supported_game::phantom_forces:
				break;
			case rbx::supported_game::universal: {
				break;
			}
			}

			printf("player - 0x%llx, name - %s, tool - %s, team - %s%s, teammate - %s, alive - %s, distance - %.2f, health - %.1f/%.1f, rig - %d, state_id - %d\n",
				entity.player.addr,
				entity.name.c_str(),
				entity.tool.c_str(),
				entity.team_name.c_str(),
				(game::curr_game == rbx::supported_game::arsenal && !entity.arsenal.team_name.empty()) ?
				std::format(" (Arsenal: {})", entity.arsenal.team_name).c_str() : "",
				entity.is_teammate ? "true" : "false",
				entity.is_alive ? "true" : "false",
				entity.distance,
				entity.health,
				entity.max_health,
				(int)(entity.rig),
				entity.state_id
			);

			local.push_back(entity);
		}

		{
			std::lock_guard<std::mutex> lock(cache_mutex);
			cached_entities.swap(local);
		}

		sleep_ms(3);
	}
}

