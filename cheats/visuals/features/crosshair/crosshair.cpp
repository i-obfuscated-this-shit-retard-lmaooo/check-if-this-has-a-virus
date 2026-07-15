#include "crosshair.h"

namespace rbx::cheats::visuals {
	ImVec2 v_crosshair::rotate_point_around_center(ImVec2 center, ImVec2 point, float angle) {
		auto cos_a = cosf(angle);
		auto sin_a = sinf(angle);
		auto dx = point.x - center.x;
		auto dy = point.y - center.y;
		return { center.x + dx * cos_a - dy * sin_a, center.y + dx * sin_a + dy * cos_a };
	}

	float v_crosshair::get_animated_rotation(float time) {
		auto t1 = 5.f;
		auto t2 = fmodf(time, t1) / t1;
		auto pi = 3.14159f;
		auto deg_to_rad = [&pi](float deg) { return deg * pi / 180.f; };

		auto t90 = deg_to_rad(90.f);
		auto pos = deg_to_rad(105.f);
		auto neg = deg_to_rad(-25.f);

		if (t2 < 0.3f) {
			auto rot = t2 / 0.3f;
			auto smooth = 0.5f * (1.f - cosf(rot * pi));
			return smooth * pos;
		}
		else if (t2 < 0.4f) {
			auto rot = (t2 - 0.3f) / 0.1f;
			auto smooth = 0.5f * (1.f - cosf(rot * pi));
			return pos * (1.f - smooth) + t90 * smooth;
		}
		else if (t2 < 0.5f) {
			return t90;
		}
		else if (t2 < 0.75f) {
			auto rot = (t2 - 0.5f) / 0.25f;
			auto smooth = 0.5f * (1.f - cosf(rot * pi));
			return t90 * (1.f - smooth) + neg * smooth;
		}
		else if (t2 < 0.85f) {
			auto rot = (t2 - 0.75f) / 0.1f;
			auto smooth = 0.5f * (1.f - cosf(rot * pi));
			return neg * (1.f - smooth) + 0.f * smooth;
		}
		else
			return 0.f;
	}

	float v_crosshair::get_animated_distance(float original, float amp, float f, float t) {
		return original + amp * sinf(f * t);
	}

	void v_crosshair::draw_crosshair() {
		using namespace rbx::globals::cheats::visuals::crosshair;
		if (!enabled) return;

		//ImGui::PushFont(var->font.tahoma);

		auto curr = (float)(ImGui::GetTime());
		auto& io = ImGui::GetIO();
		static rbx::vector2_t last_pos;
		static bool first = true;

		ImVec2 mouse_pos;
		switch (position) {
		case 0: {
			POINT m;
			if (!GetCursorPos(&m)) return;
			if (!ScreenToClient(rbx::globals::our_window, &m)) return;
			rbx::vector2_t target_pos = { (float)m.x, (float)m.y };
			if (first) { last_pos = target_pos; first = false; }
			auto delta_time = io.DeltaTime;
			auto speed = 10.f;
			auto lerp_factor = 1.f - exp(-speed * delta_time);
			last_pos = last_pos.lerp(target_pos, lerp_factor);
			mouse_pos = { last_pos.x, last_pos.y };
			break;
		}
		case 1:
			mouse_pos = { io.DisplaySize.x / 2.f, io.DisplaySize.y / 2.f };
			break;
		}

		auto _gap = gap;
		if (type == 1) {
			_gap = get_animated_distance(gap, 0.8f * gap, 0.4f, curr);
			_gap = max(_gap, gap * 0.5f);
		}

		auto half_gap = _gap / 2.f;

		ImVec2 l1_start = { mouse_pos.x - half_gap - size, mouse_pos.y };
		ImVec2 l1_end = { mouse_pos.x - half_gap, mouse_pos.y };

		ImVec2 l2_start = { mouse_pos.x, mouse_pos.y + half_gap + size };
		ImVec2 l2_end = { mouse_pos.x, mouse_pos.y + half_gap };

		ImVec2 l3_start = { mouse_pos.x + half_gap + size, mouse_pos.y };
		ImVec2 l3_end = { mouse_pos.x + half_gap, mouse_pos.y };

		ImVec2 l4_start = { mouse_pos.x, mouse_pos.y - half_gap - size };
		ImVec2 l4_end = { mouse_pos.x, mouse_pos.y - half_gap };

		if (type == 1) {
			auto angle = get_animated_rotation(curr);

			l1_start = rotate_point_around_center(mouse_pos, l1_start, angle);
			l1_end = rotate_point_around_center(mouse_pos, l1_end, angle);

			l2_start = rotate_point_around_center(mouse_pos, l2_start, angle);
			l2_end = rotate_point_around_center(mouse_pos, l2_end, angle);

			l3_start = rotate_point_around_center(mouse_pos, l3_start, angle);
			l3_end = rotate_point_around_center(mouse_pos, l3_end, angle);

			l4_start = rotate_point_around_center(mouse_pos, l4_start, angle);
			l4_end = rotate_point_around_center(mouse_pos, l4_end, angle);
		}

		auto overload = true;
		draw_line(rbx::globals::draw, l1_start, l1_end, color, thickness, outline, overload);
		draw_line(rbx::globals::draw, l2_start, l2_end, color, thickness, outline, overload);
		draw_line(rbx::globals::draw, l3_start, l3_end, color, thickness, outline, overload);
		draw_line(rbx::globals::draw, l4_start, l4_end, color, thickness, outline, overload);

		auto label1 = "lilith";
		auto label2 = ".wtf";

		auto label1_size = ImGui::CalcTextSize(label1);
		auto label2_size = ImGui::CalcTextSize(label2);

		auto total_width = label1_size.x + label2_size.x;

		ImVec2 label1_pos = { mouse_pos.x - total_width / 2, mouse_pos.y + size + 8.f };
		ImVec2 label2_pos = { label1_pos.x + label1_size.x, mouse_pos.y + size + 8.f };

		auto col2 = ImColor(102, 23, 163);
		draw_text(rbx::globals::draw, label1_pos, color, label1, outline);
		draw_text(rbx::globals::draw, label2_pos, col2, label2, outline);

		//ImGui::PopFont();
	}
}