#include "basics.h"
#include <clipper2/clipper.h>


namespace rbx::cheats::visuals {
	void v_basics::draw_bounding_box(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::box;
		using namespace rbx::globals::cheats::combat;
		if (!enabled) return;
		auto col = color;
		if (aimbot::enabled && aimbot::highlight_target && aimbot::target::entity.player.addr == player.player.addr) col = aimbot::target_color;
		if (silent::enabled && silent::highlight_target && silent::target::entity.player.addr == player.player.addr) col = silent::target_color;
		auto draw = rbx::globals::draw;
		auto top_left = ImVec2(player.top_left.x, player.top_left.y);
		auto bottom_right = ImVec2(player.bottom_right.x, player.bottom_right.y);
		draw_rect(draw, top_left, bottom_right, col, outline, thickness, rounding);
	}

	void v_basics::draw_chams(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::chams;
		using part_data = std::pair<bool, rbx::vector2_t>;
		using cham_data = std::vector<part_data>;

		if (!enabled) return;
		auto draw = rbx::globals::draw;

		auto GetHull = [](std::vector<ImVec2>& points) -> std::vector<ImVec2> {
			auto cross = [](const ImVec2& a, const ImVec2& b, const ImVec2& c) {
				return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
				};

			std::vector<ImVec2> hull;

			std::sort(points.begin(), points.end(),
				[](const ImVec2& a, const ImVec2& b) {
					if (a.x != b.x) return a.x < b.x;
					return a.y < b.y;
				});


			for (auto& p : points) {
				while (hull.size() >= 2 && cross(hull[hull.size() - 2], hull[hull.size() - 1], p) <= 0)
					hull.pop_back();
				hull.push_back(p);
			}

			size_t t = hull.size() + 1;
			for (int i = (int)points.size() - 1; i >= 0; --i) {
				auto& p = points[i];
				while (hull.size() >= t && cross(hull[hull.size() - 2], hull[hull.size() - 1], p) <= 0)
					hull.pop_back();
				hull.push_back(p);
			}

			hull.pop_back();
			return hull;
			};

		// non clipped
		if (style == 0) {
			for (const auto& cham_vector : player.chams_screen) {
				if (cham_vector.empty()) continue;

				std::vector<ImVec2> points;
				points.reserve(cham_vector.size());
				for (const auto& [is_visible, pos] : cham_vector)
					points.push_back({pos.x, pos.y});
				auto hull = GetHull(points);
				

				if (points.size() >= 3) {
					draw->AddConvexPolyFilled(hull.data(), (int)(hull.size()), color);
				}
				if (outline) {
					draw->AddPolyline(hull.data(), (int)(hull.size()), outline_color, 1, 3.f);
				}
			}
		}
		else if (style == 1) {
			Clipper2Lib::Paths64 all_parts;
			for (const auto& cham_vector : player.chams_screen) {
				if (cham_vector.empty()) continue;

				std::vector<ImVec2> points;
				points.reserve(cham_vector.size());
				for (const auto& [is_visible, pos] : cham_vector)
					points.push_back({ pos.x, pos.y });

				auto hull = GetHull(points);
			

				Clipper2Lib::Path64 path;
				for (const auto& pos : hull) {
					path.push_back({
						static_cast<int64_t>(pos.x * 1000.0),
						static_cast<int64_t>(pos.y * 1000.0)
					});
				}
				
				
				all_parts.push_back(path);
			}

			if (all_parts.empty()) return;

			auto unified_solution = Clipper2Lib::Union(all_parts, Clipper2Lib::FillRule::NonZero);
			for (auto& sp : unified_solution) {
				std::vector<ImVec2> poly;
				for (auto& pt : sp)
					poly.push_back(ImVec2(pt.x / 1000.0f, pt.y / 1000.0f));
				if (poly.size() < 3) continue;
				
				draw->AddConcavePolyFilled(poly.data(), (int)(poly.size()), color);
				
				if (outline)
					draw->AddPolyline(poly.data(), (int)(poly.size()), outline_color, true, 2.f);

			}
		}
		// mesh
		else if (style == 2) {
			
		}
	}

	/*
	void v_basics::draw_skeleton(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::skeleton;
		if (!enabled) return;
		auto draw = rbx::globals::draw;
		auto& connections = player.rig == rbx::rig_type::r6 ? rbx::cheats::visuals::connections_r6 : rbx::cheats::visuals::connections_r15;
		/*
		for (auto connection : connections) {
			auto from_bone = player.parts.find(connection.from);
			auto to_bone = player.parts.find(connection.to);

			if (!from_bone->second.addr || !to_bone->second.addr) continue;

			auto from_pos = rbx::globals::instances::g_visual_engine.world_to_screen(from_bone->second.get_part_position());
			auto to_pos = rbx::globals::instances::g_visual_engine.world_to_screen(to_bone->second.get_part_position());
			if (!rbx::globals::instances::g_visual_engine.on_screen(from_pos) ||
				!rbx::globals::instances::g_visual_engine.on_screen(to_pos)) continue;

			draw_line(draw, { from_pos.x, from_pos.y }, { to_pos.x, to_pos.y }, color, thickness, outline);
		}

		for (auto& connection : connections) {
			auto from_b = player.parts_screen.find(connection.from);
			auto to_b = player.parts_screen.find(connection.to);
			/*
			if (from_b == player.parts_screen.end() || to_b == player.parts_screen.end()) continue;

			auto from_pos = from_b->second;
			auto to_pos = to_b->second;
			
			if (!from_b->second.first || !to_b->second.first) continue;
			auto from_pos = from_b->second.second;
			auto to_pos = to_b->second.second;
			draw_line(draw, { from_pos.x, from_pos.y }, { to_pos.x, to_pos.y }, color, thickness, outline);
		}
	}
	*/

	void v_basics::draw_skeleton(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::skeleton;
		using namespace rbx::globals::cheats::combat;
		if (!enabled) return;
		auto col = color;
		if (aimbot::enabled && aimbot::highlight_target && aimbot::target::entity.player.addr == player.player.addr) col = aimbot::target_color;
		if (silent::enabled && silent::highlight_target && silent::target::entity.player.addr == player.player.addr) col = silent::target_color;
		auto draw = rbx::globals::draw;
		auto& connections = player.rig == rbx::rig_type::r6 ? rbx::cheats::visuals::connections_r6 : rbx::cheats::visuals::connections_r15;
		for (auto& connection : connections) {
			auto from_b = player.parts_screen[(int)(connection.from)];
			auto to_b = player.parts_screen[(int)(connection.to)];
			if (!from_b.first || !to_b.first) continue;
			auto from_pos = from_b.second;
			auto to_pos = to_b.second;
			draw_line(draw, { from_pos.x, from_pos.y }, { to_pos.x, to_pos.y }, col, thickness, outline);
		}

		draw_skeleton_dots(player);
	}

	void v_basics::draw_name(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::name;
		if (!enabled) return;
		auto draw = rbx::globals::draw;
		auto width = player.top_left.x + player.bottom_right.x;
		auto center = (width - ImGui::CalcTextSize(player.name.c_str()).x) / 2;
		auto pos = ImVec2{ center, player.top_left.y - 20.f };
		draw_text(draw, pos, color, player.name.c_str(), outline);
		//draw_text(draw, visitor_s, pos, color, player.name.c_str(), outline);
	}

	void v_basics::draw_distance(rbx::cache::cache_entity& player, float& offset) {
		using namespace rbx::globals::cheats::visuals::distance;
		if (!enabled) return;
		auto draw = rbx::globals::draw;
		auto distance = std::vformat(player.distance < 10.f ? "[{:.0f}m]" : "[{:.1f}m]", std::make_format_args(player.distance));
		auto width = player.top_left.x + player.bottom_right.x;
		auto center = (width - ImGui::CalcTextSize(distance.c_str()).x) / 2;
		auto pos = ImVec2{ center, player.bottom_right.y + offset };
		draw_text(draw, pos, color, distance.c_str(), outline);
		//draw_text(draw, visitor_s, pos, color, distance.c_str(), outline);
		offset += ImGui::CalcTextSize(distance.c_str()).y + 3.f;
	}

	void v_basics::draw_tool_name(rbx::cache::cache_entity& player, float& offset) {
		using namespace rbx::globals::cheats::visuals::tool_name;
		if (!enabled) return;
		auto draw = rbx::globals::draw;
		auto width = player.top_left.x + player.bottom_right.x;
		auto center = (width - ImGui::CalcTextSize(player.tool.c_str()).x) / 2;
		auto pos = ImVec2{ center, player.bottom_right.y + offset };
		draw_text(draw, pos, color, player.tool.c_str(), outline);
		//draw_text(draw, visitor_s, pos, color, player.tool.c_str(), outline);
		offset += ImGui::CalcTextSize(player.tool.c_str()).y + 3.f;
	}

	void v_basics::draw_health_bar(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::health_bar;
		if (!enabled) return;
		auto draw = rbx::globals::draw;
		auto health = std::clamp(player.health, 0.f, player.max_health);
		auto top_left = ImVec2(player.top_left.x - 5.f, player.top_left.y);
		auto bottom_right = ImVec2(player.top_left.x - 2.f, player.bottom_right.y);
		auto health_height = (health / player.max_health) * (bottom_right.y - top_left.y);
		/*
		if (outline) draw->AddRectFilled(top_left, bottom_right, ImColor(0, 0, 0));
		draw->AddRectFilled({ top_left.x, bottom_right.y - health_height }, bottom_right, color);
		*/
		if (outline) draw_rect_filled(draw, top_left, bottom_right, ImColor(0, 0, 0));
		draw_rect_filled(draw, { top_left.x + 1.f, bottom_right.y - health_height + 1.f }, { bottom_right.x - 1.f, bottom_right.y - 1.f }, color);
	}

	// idfk what to call it :broken_heart:
	void v_basics::draw_movement_status(rbx::cache::cache_entity& player, float& offset) {
		using namespace rbx::globals::cheats::visuals::movement_status;
		if (!enabled) return;
		auto draw = rbx::globals::draw;
		std::string move_text;
		if (player.move_direction.magnitude() == 0) move_text = "idle";
		if (player.move_direction.magnitude() > 0) move_text = "walking";
		switch (player.state_id) {
		case 0: case 1: move_text = "ragdolled"; break;
		case 3: case 5: case 6: move_text = "in air"; break;
		case 4: move_text = "swimming"; break;
		case 12: move_text = "climbing"; break;
		case 13: move_text = "sitting"; break;
		case 14: move_text = "platform standing"; break;
			// case 11: case 15: case 16: case 18: // none break;
		}
		if (move_text.empty()) move_text = "unknown";
		auto pos = ImVec2{ player.bottom_right.x + 5.f, player.top_left.y + offset };
		draw_text(draw, pos, color, move_text.c_str(), outline);
		offset += ImGui::CalcTextSize(move_text.c_str()).y + 3.f;
	}

	void v_basics::draw_player_rig(rbx::cache::cache_entity& player, float& offset) {
		using namespace rbx::globals::cheats::visuals::player_rig;
		if (!enabled) return;
		auto draw = rbx::globals::draw;
		std::string rig_text;
		if (player.rig == rbx::rig_type::r6) rig_text = "r6";
		else if (player.rig == rbx::rig_type::r15) rig_text = "r15";
		else rig_text = "unknown";
		auto pos = ImVec2{ player.bottom_right.x + 5.f, player.top_left.y + offset };
		draw_text(draw, pos, color, rig_text.c_str(), outline);
		offset += ImGui::CalcTextSize(rig_text.c_str()).y + 3.f;
	}

	void v_basics::draw_snaplines(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::snaplines;
		using namespace rbx::globals::instances;
		using namespace rbx::globals::cheats::combat;
		if (!enabled) return;
		auto col = color;
		if (aimbot::enabled && aimbot::highlight_target && aimbot::target::entity.player.addr == player.player.addr) col = aimbot::target_color;
		if (silent::enabled && silent::highlight_target && silent::target::entity.player.addr == player.player.addr) col = silent::target_color;
		auto draw = rbx::globals::draw;
		POINT x;
		if (!GetCursorPos(&x)) return;
		if (!ScreenToClient(rbx::globals::our_window, &x)) return;
		rbx::vector2_t cursor_pos = { (float)x.x, (float)x.y };
	    rbx::vector2_t origin_pos, destination_pos;
		switch (origin) {
		case 0: origin_pos = cursor_pos; break;
		case 1: origin_pos = g_dimensions * 0.5f; break;
		case 2: origin_pos = { g_dimensions.x * 0.5f, 0.f }; break;
		case 3: origin_pos = { g_dimensions.x * 0.5f, g_dimensions.y }; break;
		case 4: {
			auto our_head = g_local_player.get_head();
			auto our_head_pos = g_visual_engine.world_to_screen(our_head.get_part_position());
			origin_pos = our_head_pos;
			break;
		}
		case 5: {
			auto our_hrp = g_local_player.get_humanoid_root_part();
			auto our_hrp_pos = g_visual_engine.world_to_screen(our_hrp.get_part_position());
			origin_pos = our_hrp_pos;
			break;
		}
		}
		switch (destination) {
		case 0: {
			auto idx = rbx::cache::get_part_index("Head");
			auto it = player.parts_screen[(int)(idx)];
			if (!it.first) return;
			destination_pos = it.second;
			break;
		}
		case 1: {
			auto idx = rbx::cache::get_part_index("HumanoidRootPart");
			auto it = player.parts_screen[(int)(idx)];
			if (!it.first) return;
			destination_pos = it.second;
			break;
		}
		case 2: {
			rbx::vector2_t best;
			float closest = FLT_MAX;
			for (auto i = 0; i < (int)(rbx::cache::rig_part::count); ++i) {
				auto it = player.parts_screen[i];
				if (!it.first) continue;
				auto screen = it.second;
				auto delta = origin_pos - screen;
				auto distance = delta.x * delta.x + delta.y * delta.y;
				if (distance < closest) {
					closest = distance;
					best = screen;
				}
			}
			destination_pos = best;
			break;
		}
		}
		switch (style) {
		case 0: {
			draw_line(draw, { origin_pos.x, origin_pos.y }, { destination_pos.x, destination_pos.y }, col, thickness, outline);
			break;
		}
		case 1: {
			auto drop = 180.f;
			auto segments = 24;
			ImVec2 prev = { origin_pos.x, origin_pos.y };
			auto control = ImVec2(
				(origin_pos.x + destination_pos.x) * 0.5f,
				(origin_pos.y + destination_pos.y) * 0.5f + drop
			);
			for (auto i = 1; i <= segments; ++i)
			{
				auto t = (float)(i) / segments;
				auto it = 1.0f - t;
				// quad bezier : B(t) = it² * start + 2 * it * t * control + t² * end
				ImVec2 point;
				point.x = it * it * origin_pos.x + 2 * it * t * control.x + t * t * destination_pos.x;
				point.y = it * it * origin_pos.y + 2 * it * t * control.y + t * t * destination_pos.y;
				draw_line(draw, prev, point, col, thickness, outline);
				prev = point;
			}
			break;
		}
		case 2: {
			auto delta = destination_pos - origin_pos;
			auto length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
			auto dir = ImVec2(delta.x / length, delta.y / length);
			auto dash_len = length / (10 * 2.f);
			for (auto i = 0; i < 10; ++i) {
				auto start = i * 2.f * dash_len;
				auto end = start + dash_len;
				auto s = ImVec2{ origin_pos.x, origin_pos.y } + dir * start;
				auto e = ImVec2{ origin_pos.x, origin_pos.y } + dir * end;
				draw_line(draw, s, e, col, thickness, outline);
			}
			break;
		}
		}
	}

	void v_basics::draw_waifu_esp(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::waifu_esp;
		if (!enabled) return;
		ImTextureID waifu = 0;
		switch (player.waifu_idx) {
		case 0: waifu = rbx::globals::waifu_images::waguri; break;
		case 1: waifu = rbx::globals::waifu_images::alya;   break;
		case 2: waifu = rbx::globals::waifu_images::hori;   break;
		case 3: waifu = rbx::globals::waifu_images::mai;    break;
		}
		if (!waifu) return;
		auto draw = rbx::globals::draw;
		draw->AddImage(waifu, { player.top_left.x, player.top_left.y }, { player.bottom_right.x, player.bottom_right.y });
	}

	void v_basics::draw_straw_hat(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::straw_hat;
		using namespace rbx::globals::cheats::combat;
		if (!enabled) return;
		auto col = color;
		if (aimbot::enabled && aimbot::highlight_target && aimbot::target::entity.player.addr == player.player.addr) col = aimbot::target_color;
		if (silent::enabled && silent::highlight_target && silent::target::entity.player.addr == player.player.addr) col = silent::target_color;
		auto draw = rbx::globals::draw;
		/*
		for (auto line : player.hat.circumference_lines)
			draw_line(draw, { line.first.x, line.first.y }, { line.second.x, line.second.y }, color, 1.f, outline);

		for (auto line : player.hat.connection_lines)
			draw_line(draw, { line.first.x, line.first.y }, { line.second.x, line.second.y }, color, 1.f, outline);
		*/
		for (const auto& line : player.hat.circumference_lines) {
			auto from = rbx::globals::instances::g_visual_engine.world_to_screen(line.first);
			auto to = rbx::globals::instances::g_visual_engine.world_to_screen(line.second);
			draw_line(draw, { from.x, from.y }, { to.x, to.y }, col, 1.f, outline);
		}
		for (const auto& line : player.hat.connection_lines) {
			auto from = rbx::globals::instances::g_visual_engine.world_to_screen(line.first);
			auto to = rbx::globals::instances::g_visual_engine.world_to_screen(line.second);
			draw_line(draw, { from.x, from.y }, { to.x, to.y }, col, 1.f, outline);
		}
	}

	void v_basics::draw_skeleton_dots(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::skeleton::skeleton_dots;
		using namespace rbx::globals::cheats::combat;
		if (!enabled) return;
		auto col = color;
		if (aimbot::enabled && aimbot::highlight_target && aimbot::target::entity.player.addr == player.player.addr) col = aimbot::target_color;
		if (silent::enabled && silent::highlight_target && silent::target::entity.player.addr == player.player.addr) col = silent::target_color;
		auto draw = rbx::globals::draw;
		auto& bones = player.rig == rbx::rig_type::r6 ? rbx::cheats::visuals::bones_r6 : rbx::cheats::visuals::bones_r15;
		for (auto& b : bones) {
			auto bone = player.parts_screen[(int)(b)];
			if (!bone.first) continue;
			auto bone_pos = bone.second;
			draw->AddCircle({ bone_pos.x, bone_pos.y }, 4.f, ImColor(0.f, 0.f, 0.f));
			draw->AddCircleFilled({ bone_pos.x, bone_pos.y }, 3.f, col);
		}
	}

	void v_basics::draw_in_fov_snaplines_aimbot(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::combat::aimbot;
		using namespace rbx::globals::cheats::combat::aimbot::fov::snaplines;
		using namespace rbx::globals::instances;
		using namespace rbx::globals::cheats::combat;
		if (!fov::enabled) return;
		if (!fov::snaplines::enabled) return;
		auto col = color;
		if (aimbot::enabled && aimbot::highlight_target && aimbot::target::entity.player.addr == player.player.addr) col = aimbot::target_color;
		auto draw = rbx::globals::draw;
		POINT x;
		if (!GetCursorPos(&x)) return;
		if (!ScreenToClient(rbx::globals::our_window, &x)) return;
		rbx::vector2_t cursor_pos = { (float)x.x, (float)x.y };
		rbx::vector2_t origin_pos, destination_pos;

		switch (aimbot::fov::position) {
		case 0: { origin_pos = cursor_pos; break; } // mouse
		case 1: {
			// center
			auto sz = ImGui::GetIO().DisplaySize;
			origin_pos = { sz.x / 2, sz.y / 2 };
			break;
		}
		}

		switch (destination) {
		case 0: {
			auto idx = rbx::cache::get_part_index("Head");
			auto it = player.parts_screen[(int)(idx)];
			if (!it.first) return;
			destination_pos = it.second;
			break;
		}
		case 1: {
			auto idx = rbx::cache::get_part_index("HumanoidRootPart");
			auto it = player.parts_screen[(int)(idx)];
			if (!it.first) return;
			destination_pos = it.second;
			break;
		}
		case 2: {
			rbx::vector2_t best;
			float closest = FLT_MAX;
			for (auto i = 0; i < (int)(rbx::cache::rig_part::count); ++i) {
				auto it = player.parts_screen[i];
				if (!it.first) continue;
				auto screen = it.second;
				auto delta = origin_pos - screen;
				auto distance = delta.x * delta.x + delta.y * delta.y;
				if (distance < closest) {
					closest = distance;
					best = screen;
				}
			}
			destination_pos = best;
			break;
		}
		}

		if (!within_fov(origin_pos, aimbot::fov::size, aimbot::fov::segments, destination_pos)) return;
		draw_line(draw, { origin_pos.x, origin_pos.y }, { destination_pos.x, destination_pos.y }, col, thickness, outline);
	}

	void v_basics::draw_in_fov_snaplines_silent(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::combat::silent;
		using namespace rbx::globals::cheats::combat::silent::fov::snaplines;
		using namespace rbx::globals::instances;
		using namespace rbx::globals::cheats::combat;
		if (!fov::enabled) return;
		if (!fov::snaplines::enabled) return;
		auto col = color;
		if (silent::enabled && silent::highlight_target && silent::target::entity.player.addr == player.player.addr) col = silent::target_color;
		auto draw = rbx::globals::draw;
		POINT x;
		if (!GetCursorPos(&x)) return;
		if (!ScreenToClient(rbx::globals::our_window, &x)) return;
		rbx::vector2_t cursor_pos = { (float)x.x, (float)x.y };
		rbx::vector2_t origin_pos, destination_pos;

		switch (silent::fov::position) {
		case 0: { origin_pos = cursor_pos; break; } // mouse
		case 1: {
			// center
			auto sz = ImGui::GetIO().DisplaySize;
			origin_pos = { sz.x / 2, sz.y / 2 };
			break;
		}
		}

		switch (destination) {
		case 0: {
			auto idx = rbx::cache::get_part_index("Head");
			auto it = player.parts_screen[(int)(idx)];
			if (!it.first) return;
			destination_pos = it.second;
			break;
		}
		case 1: {
			auto idx = rbx::cache::get_part_index("HumanoidRootPart");
			auto it = player.parts_screen[(int)(idx)];
			if (!it.first) return;
			destination_pos = it.second;
			break;
		}
		case 2: {
			rbx::vector2_t best;
			float closest = FLT_MAX;
			for (auto i = 0; i < (int)(rbx::cache::rig_part::count); ++i) {
				auto it = player.parts_screen[i];
				if (!it.first) continue;
				auto screen = it.second;
				auto delta = origin_pos - screen;
				auto distance = delta.x * delta.x + delta.y * delta.y;
				if (distance < closest) {
					closest = distance;
					best = screen;
				}
			}
			destination_pos = best;
			break;
		}
		}

		if (!within_fov(origin_pos, silent::fov::size, silent::fov::segments, destination_pos)) return;
		draw_line(draw, { origin_pos.x, origin_pos.y }, { destination_pos.x, destination_pos.y }, col, thickness, outline);
	}

	void v_basics::draw_locked_indicator() {
		using namespace rbx::globals::instances;
		using namespace rbx::globals::cheats::combat;
		using namespace rbx::globals::cheats::combat::silent::locked_indicator;
		if (!silent::enabled) return;
		if (!enabled) return;
		if (!silent::target::entity.player.addr) return;
		auto draw = rbx::globals::draw;
		POINT x;
		if (!GetCursorPos(&x)) return;
		if (!ScreenToClient(rbx::globals::our_window, &x)) return;
		rbx::vector2_t cursor_pos = { (float)x.x, (float)x.y };
	    rbx::vector2_t origin_pos, destination_pos;
		switch (silent::fov::position) {
		case 0: { origin_pos = cursor_pos; break; } // mouse
		case 1: {
			// center
			auto sz = ImGui::GetIO().DisplaySize;
			origin_pos = { sz.x / 2, sz.y / 2 };
			break;
		}
		}

		auto& curr_ent = silent::target::entity;
		auto idx = rbx::cache::get_part_index(silent::target::part.get_name());
		auto it = curr_ent.parts_screen[(int)(idx)];
		if (!it.first) return;
		destination_pos = it.second;

		draw_line(draw, { origin_pos.x, origin_pos.y }, { destination_pos.x, destination_pos.y }, color, thickness, outline);
	}

	void v_basics::draw_3d_box(rbx::cache::cache_entity& player) {
		using namespace rbx::globals::cheats::visuals::box_3d;
		using namespace rbx::globals::cheats::combat;
		if (!enabled) return; 

		auto col = color;
		if (aimbot::enabled && aimbot::highlight_target && aimbot::target::entity.player.addr == player.player.addr) col = aimbot::target_color;
		if (silent::enabled && silent::highlight_target && silent::target::entity.player.addr == player.player.addr) col = silent::target_color;

		auto draw = rbx::globals::draw;

		auto points = player.box_3d_points;
		if (points.size() < 8) return;

		// it tables
		auto ftl_it = points.at(0); // front top left
		auto ftr_it = points.at(1); // front top right
		auto fbl_it = points.at(2); // front bottom left
		auto fbr_it = points.at(3); // front bottom right
		auto btl_it = points.at(4); // back top left
		auto btr_it = points.at(5); // back top right
		auto bbl_it = points.at(6); // back bottom left
		auto bbr_it = points.at(7); // back bottom right

		auto all = ftl_it.first && ftr_it.first && fbl_it.first && fbr_it.first && 
			btl_it.first && btr_it.first && bbl_it.first && bbr_it.first;
		if (!all) return;

		// screen pos
		auto ftl = ftl_it.second;
		auto ftr = ftr_it.second;
		auto fbl = fbl_it.second;
		auto fbr = fbr_it.second;
		auto btl = btl_it.second;
		auto btr = btr_it.second;
		auto bbl = bbl_it.second;
		auto bbr = bbr_it.second;

		// top face
		draw_line(draw, { ftl.x, ftl.y }, { ftr.x, ftr.y }, col, thickness, outline);
		draw_line(draw, { ftr.x, ftr.y }, { btr.x, btr.y }, col, thickness, outline);
		draw_line(draw, { btr.x, btr.y }, { btl.x, btl.y }, col, thickness, outline);
		draw_line(draw, { btl.x, btl.y }, { ftl.x, ftl.y }, col, thickness, outline);

		// bottom face
		draw_line(draw, { fbl.x, fbl.y }, { fbr.x, fbr.y }, col, thickness, outline);
		draw_line(draw, { fbr.x, fbr.y }, { bbr.x, bbr.y }, col, thickness, outline);
		draw_line(draw, { bbr.x, bbr.y }, { bbl.x, bbl.y }, col, thickness, outline);
		draw_line(draw, { bbl.x, bbl.y }, { fbl.x, fbl.y }, col, thickness, outline);

		// vertical edges
		draw_line(draw, { ftl.x, ftl.y }, { fbl.x, fbl.y }, col, thickness, outline);
		draw_line(draw, { ftr.x, ftr.y }, { fbr.x, fbr.y }, col, thickness, outline);
		draw_line(draw, { btl.x, btl.y }, { bbl.x, bbl.y }, col, thickness, outline);
		draw_line(draw, { btr.x, btr.y }, { bbr.x, bbr.y }, col, thickness, outline);
	}

	void v_basics::draw(rbx::cache::cache_entity& player) {
		if (!player.player.addr) return;

		draw_bounding_box(player);
		draw_chams(player);
		draw_skeleton(player);
		draw_health_bar(player);

		draw_name(player);
		auto right_offest = 0.f;
		draw_movement_status(player, right_offest);
		draw_player_rig(player, right_offest);
		auto bottom_offset = 5.f;
		draw_distance(player, bottom_offset);
		draw_tool_name(player, bottom_offset);
		draw_snaplines(player);
		draw_in_fov_snaplines_aimbot(player);
		draw_in_fov_snaplines_silent(player);

		draw_waifu_esp(player);
		draw_straw_hat(player);
		draw_3d_box(player);
	}
}