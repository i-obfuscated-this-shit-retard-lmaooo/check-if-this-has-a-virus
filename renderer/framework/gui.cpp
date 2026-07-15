#include "settings/functions.h"
#include <windows.h>

#pragma region helpers

bool easing_preset_dropdown(const char* label, rbx::easing_function* current_easing, bool wo_label = false) {
	using namespace rbx;
	int idx = static_cast<int>(*current_easing);
	if (gui->dropdown(label, &idx, easing_names, IM_ARRAYSIZE(easing_names), wo_label)) {
		*current_easing = static_cast<easing_function>(idx);
		return true;
	}
	return false;
}

#pragma endregion

void c_gui::render()
{
	if (GetAsyncKeyState(var->gui.menu_key) & 1 ||
		GetAsyncKeyState(var->gui.menu_key_alt) & 1 ||
		GetAsyncKeyState(var->gui.menu_key_alt2) & 1)
		var->gui.menu_opened = !var->gui.menu_opened;

	if (GetAsyncKeyState(var->gui.exit_key) & 1) 
		rbx::globals::g_running = false;

	if (var->watermark.enabled) {
		gui->set_next_window_pos(ImVec2(GetIO().DisplaySize.x / 2 - var->watermark.width / 2, GetIO().DisplaySize.y - 60.f));
		gui->set_next_window_size(ImVec2(var->watermark.width, 0));
		gui->begin("##watermark", nullptr, var->watermark.flags);
		{
			draw->window_decorations();

			const ImVec2 pos = GetWindowPos();
			const ImVec2 size = GetWindowSize();
			ImDrawList* draw_list = GetWindowDrawList();
			ImGuiStyle* style = &GetStyle();

			{
				style->WindowPadding = var->watermark.padding;
				style->PopupBorderSize = var->watermark.border_size;
				style->WindowBorderSize = var->watermark.border_size;
				style->ItemSpacing = var->watermark.spacing;
				style->WindowShadowSize = var->watermark.shadow_size;
				style->Colors[ImGuiCol_WindowShadow] = { clr->accent.Value.x, clr->accent.Value.y, clr->accent.Value.z, var->watermark.shadow_alpha };
			}

			{
				draw->rect(GetBackgroundDrawList(), pos - ImVec2(1, 1), pos + size + ImVec2(1, 1), draw->get_clr({ 0, 0, 0, 0.5f }));
				draw->fade_rect_filled(draw_list, pos, pos + size, draw->get_clr(clr->window.background_one), draw->get_clr(clr->window.background_two), fade_direction::vertically);
				//draw->rect_filled(draw_list, pos, pos + size, draw->get_clr(clr->window.background_one));
				draw->rect(draw_list, pos, pos + size, draw->get_clr(clr->window.stroke));
			}

			ImVec2 cursor_pos = ImGui::GetCursorPos();
			gui->set_cursor_pos({ style->ItemSpacing.x + cursor_pos.x, style->ItemSpacing.y + cursor_pos.y - 1 });
			Image(rbx::globals::logo, { 20, 20 });
			gui->sameline();
			char text[256];
			auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			tm tm;
			localtime_s(&tm, &now);
			strcpy_s(text, " | lilith.wtf");
			if (var->watermark.date) {
				char temp[64];
				sprintf_s(temp, " | %s", Offsets::ClientVersion.c_str());
				strcat_s(text, temp);
			}
			if (var->watermark.date) {
				char temp[64];
				strftime(temp, sizeof(temp), " | %d/%m/%y", &tm);
				strcat_s(text, temp);
			}
			if (var->watermark.fps) {
				char temp[64];
				sprintf_s(temp, " | %.0f fps", ImGui::GetIO().Framerate);
				strcat_s(text, temp);
			}
			ImGui::PushFont(var->font.tahoma);
			auto text_size = ImGui::CalcTextSize(text);
			ImGui::PopFont();
			draw->text_outline(draw_list, var->font.tahoma, var->font.tahoma->FontSize, { pos.x + (2 * style->ItemSpacing.x) + 10, pos.y + style->ItemSpacing.y + 2 }, draw->get_clr(clr->widgets.text), text);

			var->watermark.width = GetCurrentWindow()->ContentSize.x + text_size.x + style->ItemSpacing.x;
		}
		gui->end();
	}

	var->gui.menu_alpha = ImClamp(var->gui.menu_alpha + (gui->fixed_speed(8.f) * (var->gui.menu_opened ? 1.f : -1.f)), 0.f, 1.f);
	if (var->gui.menu_alpha <= 0.01f)
		return;

	gui->set_next_window_pos(ImVec2(GetIO().DisplaySize.x / 2 - var->window.width / 2, 20));
	gui->set_next_window_size(ImVec2(var->window.width, elements->section.size.y + var->window.spacing.y * 2 - 1));
	gui->push_style_var(ImGuiStyleVar_Alpha, var->gui.menu_alpha);
	gui->begin("##window", nullptr, var->window.main_flags);
	{
		const ImVec2 pos = GetWindowPos();
		const ImVec2 size = GetWindowSize();
		ImDrawList* draw_list = GetWindowDrawList();
		ImGuiStyle* style = &GetStyle();

		{
			style->WindowPadding = var->window.padding;
			style->PopupBorderSize = var->window.border_size;
			style->WindowBorderSize = var->window.border_size;
			style->ItemSpacing = var->window.spacing;
			style->WindowShadowSize = var->window.shadow_size;
			style->ScrollbarSize = var->window.scrollbar_size;
			style->Colors[ImGuiCol_WindowShadow] = { clr->accent.Value.x, clr->accent.Value.y, clr->accent.Value.z, var->window.shadow_alpha };
		}

		{
			draw->rect(GetBackgroundDrawList(), pos - ImVec2(1, 1), pos + size + ImVec2(1, 1), draw->get_clr({ 0, 0, 0, 0.5f }));
			draw->rect_filled(draw_list, pos, pos + size, draw->get_clr(clr->window.background_one));
			draw->line(draw_list, pos + ImVec2(1, 1), pos + ImVec2(size.x - 1, 1), draw->get_clr(clr->accent), 1);
			draw->line(draw_list, pos + ImVec2(1, 2), pos + ImVec2(size.x - 1, 2), draw->get_clr(clr->accent, 0.4f), 1);
			draw->rect(draw_list, pos, pos + size, draw->get_clr(clr->window.stroke));
		}

		{
			gui->set_cursor_pos(style->ItemSpacing);
			gui->begin_group();
			{
				for (int i = 0; i < IM_ARRAYSIZE(var->gui.current_section); i++)
					gui->section(var->gui.section_icons[i], &var->gui.current_section[i]);
			}
			gui->end_group();
		}

		{
			if (var->gui.current_section[0])
			{
				gui->set_next_window_size_constraints(ImVec2(640, 650), GetIO().DisplaySize);
				gui->begin("lilith.wtf - #1", nullptr, var->window.flags);
				{
					//draw->window_decorations();
					draw->window_decorations(true, rbx::globals::logo);

					{
						static int subtabs;
						gui->set_cursor_pos(elements->content.window_padding + ImVec2(0, var->window.titlebar));
						gui->begin_group();
						{
							gui->sub_section("Combat", 0, subtabs, 6);
							gui->sub_section("Visuals", 1, subtabs, 6);
							gui->sub_section("Miscellaneous", 2, subtabs, 6);
							gui->sub_section("Lightning", 3, subtabs, 6);
							gui->sub_section("Waypoints", 4, subtabs, 6);
							gui->sub_section("Settings", 5, subtabs, 6);
						}
						gui->end_group();

						gui->set_cursor_pos(elements->content.window_padding + ImVec2(0, var->window.titlebar + elements->section.height - 1));
						if (subtabs == 0) {
							using namespace rbx::globals::cheats::combat;
							gui->begin_content();
							{
								gui->begin_group();
								{
									gui->begin_child("Aimbot", 2, 2);
									{
										// aimbot
										{
											using namespace aimbot;
											gui->checkbox("Enabled##aimbot", &aimbot::enabled);
											gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
											if (gui->begin_extension("Options##aimbot")) {
												gui->checkbox("Smoothing##aimbot", &aimbot::smoothing);
												gui->slider_float("Smoothing Amount##aimbot", &aimbot::smoothing_val, 1.f, 20.f, false, "%.1f");
												// camera
												if (aimbot::type == 1) gui->slider_float("Speed", &aimbot::speed, 1.f, 5.f, false, "%.1f");
												gui->checkbox("Prediction##aimbot", &aimbot::prediction);
												gui->slider_float("Prediction Amount##aimbot", &aimbot::prediction_val, 1.f, 20.f, false, "%.1f");
												if (aimbot::type == 1) { // camera
													if (easing_preset_dropdown("Easing Function##aimbot", &aimbot::easing_type))
														memcpy(aimbot::easing, rbx::easing_values[aimbot::easing_type], sizeof(float) * 4);
													if (aimbot::easing_type == rbx::easing_function::custom) {
														float temp[5];
														memcpy(temp, aimbot::easing, sizeof(float) * 5);
														if (ImGui::Bezier("##custom", temp)) memcpy(aimbot::easing, temp, sizeof(float) * 5);
														memcpy(rbx::easing_values[rbx::easing_function::custom], temp, sizeof(float) * 4);
													}
												}
												gui->end_extension();
											}
											gui->sameline(GetWindowWidth() - 52.f - GetStyle().WindowPadding.x);
											gui->keybind("aimbot", &aimbot::keybind, &aimbot::keybind_type);
											gui->checkbox("Sticky Aim##aimbot", &aimbot::sticky_aim);
											gui->dropdown("Type##aimbot", &aimbot::type, aimbot::types, IM_ARRAYSIZE(aimbot::types));
											gui->dropdown("Target Mode##aimbot", &aimbot::target_mode, aimbot::target_modes, IM_ARRAYSIZE(aimbot::target_modes));
											gui->dropdown("HitPart##aimbot", &aimbot::hitpart, aimbot::hitparts, IM_ARRAYSIZE(aimbot::hitparts));
											gui->multi_dropdown("Checks", aimbot::checks_enabled, aimbot::checks, IM_ARRAYSIZE(aimbot::checks));
											if (aimbot::checks_enabled[2]) gui->slider_float("Minimum Health##aimbot", &aimbot::health_check_amount, 0, 100, false, "%.0f");
											if (aimbot::checks_enabled[3]) gui->slider_float("Maximum Distance##aimbot", &aimbot::distance_check_amount, 0, 5000, false, "%.0f");

											gui->checkbox("Highlight Target##aimbot", &rbx::globals::cheats::combat::aimbot::highlight_target);
											gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
											if (gui->begin_extension("Options##highlightaimbot")) {
												gui->label_color_edit("Color##highlightaimbot", rbx::globals::cheats::combat::aimbot::target_color);
												gui->end_extension();
											}
										}

										gui->separator();
										ImGui::Text("FOV");

										// aimbot fov
										{
											using namespace aimbot::fov;
											gui->checkbox("Enabled##aimbotfov", &enabled);
											gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
											if (gui->begin_extension("Options##aimbotfov")) {
												gui->dropdown("Type##aimbotfov", &type, types, IM_ARRAYSIZE(types));
												if (type == 1) gui->dropdown("Animation##aimbotfov", &animation, animations, IM_ARRAYSIZE(animations));
												gui->dropdown("Theme##aimbotfov", &aimbot::fov::style, styles, IM_ARRAYSIZE(styles));
												gui->dropdown("Position##aimbotfov", &position, positions, IM_ARRAYSIZE(positions));
												gui->slider_float("Size##aimbotfov", &aimbot::fov::size, 10.f, rbx::globals::instances::g_dimensions.x * 0.5f);
												gui->slider_float("Thickness##aimbotfov", &aimbot::fov::thickness, 0.5f, 3.f);
												gui->slider_int("Segments##aimbotfov", &segments, 3, 32);
												gui->checkbox("Outline##aimbotfov", &outline);
												gui->label_color_edit("Color##aimbotfov", color);
												gui->end_extension();
											}

											gui->checkbox("In-FOV Snaplines##aimbotfov_snaplines", &snaplines::enabled);
											gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
											if (gui->begin_extension("Options##aimbotfov_snaplines")) {
												gui->dropdown("Destination##aimbotfov_snaplines", &snaplines::destination, snaplines::destinations, IM_ARRAYSIZE(snaplines::destinations));
												gui->slider_float("Thickness##aimbotfov_snaplines", &snaplines::thickness, 0.5f, 3.f);
												gui->checkbox("Outline##aimbotfov_snaplines", &snaplines::outline);
												gui->label_color_edit("Color##aimbotfov_snaplines", snaplines::color);
												gui->end_extension();
											}
										}
									}
									gui->end_child();

									gui->begin_child("Triggerbot", 2, 2);
									{
										using namespace triggerbot;
										gui->checkbox("Enabled##triggerbot", &triggerbot::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##triggerbot")) {
											gui->dropdown("Type##triggerbot", &triggerbot::type, triggerbot::types, IM_ARRAYSIZE(triggerbot::types));
											if (type == 0) gui->slider_float("Clicks Per Second##triggerbot", &triggerbot::clicks_per_second, 1.f, 20.f, false, "%.0f/s");
											else if (type == 1) gui->slider_float("Hold Duration##triggerbot", &triggerbot::hold_duration, 1.f, 20.f, false, "%.0fs");
											gui->slider_float("Range##triggerbot", &triggerbot::range, 10.f, rbx::globals::instances::g_dimensions.x * 0.5f);
											gui->end_extension();
										}
										gui->sameline(GetWindowWidth() - 52.f - GetStyle().WindowPadding.x);
										gui->keybind("triggerbot", &triggerbot::keybind, &triggerbot::keybind_type);
									}
									gui->end_child();
								}
								gui->end_group();

								gui->sameline();

								gui->begin_group();
								{
									gui->begin_child("Silent", 2, 2);
									{
										// silent
										{
											using namespace silent;
											gui->checkbox("Enabled##silentaimbot", &enabled);
											gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
											if (gui->begin_extension("Options##silentaimbot")) {
												gui->checkbox("Smoothing##silentaimbot", &smoothing);
												gui->slider_float("Smoothing Amount##silentaimbot", &smoothing_val, 1.f, 20.f, false, "%.1f");
												// camera
												gui->checkbox("Prediction##silentaimbot", &prediction);
												gui->slider_float("Prediction Amount##silentaimbot", &prediction_val, 1.f, 20.f, false, "%.1f");
												gui->end_extension();
											}
											gui->sameline(GetWindowWidth() - 52.f - GetStyle().WindowPadding.x);
											gui->keybind("silent", &silent::keybind, &silent::keybind_type);
											gui->checkbox("Sticky Aim##silentaimbot", &sticky_aim);
											gui->dropdown("Type##silentaimbot", &type, types, IM_ARRAYSIZE(types));
											gui->dropdown("Target Mode##silentaimbot", &target_mode, target_modes, IM_ARRAYSIZE(target_modes));
											gui->dropdown("HitPart##silentaimbot", &hitpart, hitparts, IM_ARRAYSIZE(hitparts));
											gui->multi_dropdown("Checks##silentaimbot", checks_enabled, checks, IM_ARRAYSIZE(checks));
											if (checks_enabled[2]) gui->slider_float("Minimum Health##silentaimbot", &health_check_amount, 0, 100, false, "%.0f");
											if (checks_enabled[3]) gui->slider_float("Maximum Distance##silentaimbot", &distance_check_amount, 0, 5000, false, "%.0f");

											gui->checkbox("Highlight Target##silentaimbot", &highlight_target);
											gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
											if (gui->begin_extension("Options##highlightsilent")) {
												gui->label_color_edit("Color##highlightsilent", target_color);
												gui->end_extension();
											}

											gui->checkbox("Locked Indicator##silentaimbot", &locked_indicator::enabled);
											gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
											if (gui->begin_extension("Options##locked_indicatorsilent")) {
												gui->checkbox("Outline##locked_indicatorsilent", &locked_indicator::outline);
												gui->slider_float("Thickness##locked_indicatorsilent", &locked_indicator::thickness, 0.5f, 3.f);
												gui->label_color_edit("Color##locked_indicatorsilent", locked_indicator::color);
												gui->end_extension();
											}
										}

										gui->separator();
										ImGui::Text("FOV");

										// silent fov
										{
											using namespace silent::fov;
											gui->checkbox("Enabled##silentfov", &enabled);
											gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
											if (gui->begin_extension("Options##silentfov")) {
												gui->dropdown("Type##silentfov", &type, types, IM_ARRAYSIZE(types));
												if (type == 1) gui->dropdown("Animation##silentfov", &animation, animations, IM_ARRAYSIZE(animations));
												gui->dropdown("Theme##silentfov", &silent::fov::style, styles, IM_ARRAYSIZE(styles));
												gui->dropdown("Position##silentfov", &position, positions, IM_ARRAYSIZE(positions));
												gui->slider_float("Size##silentfov", &silent::fov::size, 10.f, rbx::globals::instances::g_dimensions.x * 0.5f);
												gui->slider_float("Thickness##silentfov", &thickness, 0.5f, 3.f);
												gui->slider_int("Segments##silentfov", &segments, 3, 32);
												gui->checkbox("Outline##silentfov", &outline);
												gui->label_color_edit("Color##silentfov", color);
												gui->end_extension();
											}

											gui->checkbox("In-FOV Snaplines##silent", &snaplines::enabled);
											gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
											if (gui->begin_extension("Options##silentfov_snaplines")) {
												gui->dropdown("Destination##silentfov_snaplines", &snaplines::destination, snaplines::destinations, IM_ARRAYSIZE(snaplines::destinations));
												gui->slider_float("Thickness##silentfov_snaplines", &snaplines::thickness, 0.5f, 3.f);
												gui->checkbox("Outline##silentfov_snaplines", &snaplines::outline);
												gui->label_color_edit("Color##silentfov_snaplines", snaplines::color);
												gui->end_extension();
											}
										}
									}
									gui->end_child();

									gui->begin_child("Misc", 2, 2);
									{

									}
									gui->end_child();
								}
								gui->end_group();
							}
							gui->end_content();
						}
						if (subtabs == 1) {
							using namespace rbx::globals::cheats::visuals;
							gui->begin_content();
							{
								gui->begin_group();
								{
									gui->begin_child("Visuals", 2, 1);
									{
										gui->checkbox("Box", &box::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##box")) {
											gui->dropdown("Style##box", &box::style, box::styles, IM_ARRAYSIZE(box::styles));
											gui->dropdown("Type##box", &box::type, box::types, IM_ARRAYSIZE(box::types));
											gui->label_color_edit("Color##box", box::color);
											gui->slider_float("Rounding##box", &box::rounding, 0.f, 10.f, false, "%.0f");
											gui->slider_float("Thickness##box", &box::thickness, 0.5f, 3.f, false, "%.0f");
											gui->checkbox("Outline##box", &box::outline);
											gui->end_extension();
										}
										gui->checkbox("Health Bar", &health_bar::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##health_bar")) {
											gui->label_color_edit("Color##health_bar", health_bar::color);
											gui->checkbox("Outline##health_bar", &health_bar::outline);
											gui->end_extension();
										}
										gui->checkbox("Skeleton", &skeleton::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x); 
										if (gui->begin_extension("Options##skelly")) {
											gui->label_color_edit("Color##skelly", skeleton::color);
											gui->checkbox("Outline##skelly", &skeleton::outline);
											gui->slider_float("Thickness##skelly", &skeleton::thickness, 0.5f, 3.f, false);
											gui->separator();
											gui->checkbox("Dots##skelly", &skeleton::skeleton_dots::enabled);
											gui->label_color_edit("Color##skellydots", skeleton::skeleton_dots::color);
											gui->checkbox("Outline##skellydots", &skeleton::skeleton_dots::outline);
											gui->slider_float("Radius##skellydots", &skeleton::skeleton_dots::radius, 0.f, 10.f, false);
											gui->end_extension();
										}
										gui->checkbox("Snaplines", &snaplines::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##snaplines")) {
											gui->dropdown("Origin##snaplines", &snaplines::origin, snaplines::origins, IM_ARRAYSIZE(snaplines::origins));
											gui->dropdown("Destination##snaplines", &snaplines::destination, snaplines::destinations, IM_ARRAYSIZE(snaplines::destinations));
											gui->dropdown("Style##snaplines", &snaplines::style, snaplines::styles, IM_ARRAYSIZE(snaplines::styles));
											gui->checkbox("Outline##snaplines", &snaplines::outline);
											gui->slider_float("Thickness##snaplines", &snaplines::thickness, 0.5f, 3.f);
											gui->label_color_edit("Color##snaplines", snaplines::color);
											gui->end_extension();
										}
					
										gui->checkbox("Chams", &chams::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##chams")) {
											gui->checkbox("Outline", &chams::outline);
											gui->dropdown("Style##chams", &chams::style, chams::styles, IM_ARRAYSIZE(chams::styles));
	
											gui->label_color_edit("Color##chams", chams::color);
											gui->label_color_edit("Outline Color##chams", chams::outline_color);
											gui->end_extension();
										}
										gui->separator();
										gui->checkbox("Name", &name::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##name")) {
											gui->dropdown("Style##name", &name::type, name::types, IM_ARRAYSIZE(name::types));
											gui->checkbox("Outline##name", &name::outline);
											gui->label_color_edit("Color##name", name::color);
											gui->end_extension();
										}
										gui->checkbox("Distance", &distance::enabled); 
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##distance")) {
											gui->checkbox("Outline##distance", &distance::outline);
											gui->label_color_edit("Color##distance", distance::color);
											gui->end_extension();
										}
										gui->checkbox("Tool Name", &tool_name::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##tool_name")) {
											gui->checkbox("Outline##tool_name", &tool_name::outline);
											gui->label_color_edit("Color##tool_name", tool_name::color);
											gui->end_extension();
										}
										gui->checkbox("Movement Status", &movement_status::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##movement_status")) {
											gui->checkbox("Outline##movement_status", &movement_status::outline);
											gui->label_color_edit("Color##movement_status", movement_status::color);
											gui->end_extension();
										}
										gui->checkbox("Player Rig", &player_rig::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##player_rig")) {
											gui->checkbox("Outline##player_rig", &player_rig::outline);
											gui->label_color_edit("Color##player_rig", player_rig::color);
											gui->end_extension();
										}

										gui->multi_dropdown("Checks", checks_enabled, checks, IM_ARRAYSIZE(checks));
										if (checks_enabled[2]) gui->slider_float("Minimum Health", &health_check_amount, 0, 100, false, "%.0f");
										if (checks_enabled[3]) gui->slider_float("Maximum Distance", &distance_check_amount, 0, 5000, false, "%.0f");
									}
									gui->end_child();
								}
								gui->end_group();

								gui->sameline();

								gui->begin_group();
								{
									gui->begin_child("Advanced", 2, 2);
									{

									}
									gui->end_child();

									gui->begin_child("Extras", 2, 2);
									{
										gui->checkbox("Draw LocalPlayer", &self);
										gui->checkbox("Crosshair", &crosshair::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##crosshair")) {
											gui->dropdown("Style##crosshair", &crosshair::type, crosshair::types, IM_ARRAYSIZE(crosshair::types));
											gui->dropdown("Position##crosshair", &crosshair::position, crosshair::positions, IM_ARRAYSIZE(crosshair::positions));
											gui->slider_float("Size##crosshair", &crosshair::size, 10.f, 30.f);
											gui->slider_float("Gap##crosshair", &crosshair::gap, 5.f, 15.f);
											gui->slider_float("Thickness##crosshair", &crosshair::thickness, 0.5f, 3.f);
											gui->label_color_edit("Color##crosshair", crosshair::color);
											gui->checkbox("Outline##crosshair", &crosshair::outline);
											gui->end_extension();
										}
										gui->checkbox("3D Box", &box_3d::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##3dbox")) {
											gui->dropdown("Style##3dbox", &box_3d::style, box_3d::styles, IM_ARRAYSIZE(box_3d::styles));
											gui->dropdown("Type##3dbox", &box_3d::type, box_3d::types, IM_ARRAYSIZE(box_3d::types));
											gui->label_color_edit("Color##3dbox", box_3d::color);
											gui->slider_float("Thickness##3dbox", &box_3d::thickness, 0.5f, 3.f, false, "%.0f");
											gui->checkbox("Outline##3dbox", &box_3d::outline);
											gui->end_extension();
										}
										gui->checkbox("Straw Hat", &straw_hat::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##hat")) {
											gui->checkbox("Outline##hat", &straw_hat::outline);
											gui->label_color_edit("Color##hat", straw_hat::color);
											gui->slider_int("Brim Segments##hat", &straw_hat::segments, 3, 32);
											gui->end_extension();
										}
										gui->checkbox("Waifu ESP", &waifu_esp::enabled);
									}
									gui->end_child();
								}
								gui->end_group();
							}
							gui->end_content();
						}
						if (subtabs == 2) {
							using namespace rbx::globals::cheats::misc;
							gui->begin_content();
							{
								gui->begin_group();
								{
									gui->begin_child("Movement", 2, 1);
									{

									}
									gui->end_child();
								}
								gui->end_group();

								gui->sameline();

								gui->begin_group();
								{
									gui->begin_child("Exploits", 2, 2);
									{
										gui->checkbox("Tickrate", &tickrate::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##tickrate")) {
											gui->slider_float("Value##tickrate", &tickrate::value, 1.f, 660.f, false, "%.0f/s");
											gui->end_extension();
										}
										gui->sameline(GetWindowWidth() - 52.f - GetStyle().WindowPadding.x);
										gui->keybind("tickrate", &tickrate::keybind, &tickrate::keybind_type);

										gui->checkbox("Animation Changer", &animation_changer::enabled);
										gui->sameline(GetWindowWidth() - 14.f - GetStyle().WindowPadding.x);
										if (gui->begin_extension("Options##animation_changer")) {
											auto i = (int)(animation_changer::pack);
											if (gui->dropdown("Animation", &i, rbx::animation_pack_names, IM_ARRAYSIZE(rbx::animation_pack_names)))
												animation_changer::pack.store((rbx::animation_pack)(i));
											gui->end_extension();
										}
										gui->sameline(GetWindowWidth() - 52.f - GetStyle().WindowPadding.x);
										gui->keybind("animation_changer", &animation_changer::keybind, &animation_changer::keybind_type);
									}
									gui->end_child();
								}
								gui->end_group();
							}
							gui->end_content();
						}
						if (subtabs == 3) {
							gui->begin_content();
							{
								using namespace rbx::globals::cheats::misc::lighting;
								gui->begin_group();
								{
									gui->begin_child("Lightning", 2, 1);
									{
										gui->checkbox("Enabled##lighting", &enabled);
										
										{
											using namespace clock_time;
											gui->checkbox("Clock Time##clocktime", &clock_time::enabled);
											gui->sameline(GetWindowWidth() - 28.f - GetStyle().WindowPadding.x);
											gui->keybind("clock_time", &clock_time::keybind, &clock_time::keybind_type);
											static char time_buff[6] = "";
											if (gui->text_field("00:00##clocktime", time_buff, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue)) {
												auto hours = 0, minutes = 0;
												if (sscanf_s(time_buff, "%2d:%2d", &hours, &minutes) == 2) {
													hours = std::clamp(hours, 0, 23);
													minutes = std::clamp(minutes, 0, 59);
													auto seconds = 0.f;
													seconds += hours * 3600.f;
													seconds += minutes * 60.f;
													current_time = seconds;
												}
											}
										}
									}
									gui->end_child();
								}
								gui->end_group();

								gui->sameline();

								gui->begin_group();
								{
									gui->begin_child("Sky", 2, 2);
									{

									}
									gui->end_child();
								}
								gui->end_group();
							}
							gui->end_content();
						}
						if (subtabs == 4) {
							gui->begin_content();
							{
								gui->begin_group();
								{
									gui->begin_child("Waypoints", 2, 1);
									{

									}
									gui->end_child();
								}
								gui->end_group();

								gui->sameline();

								gui->begin_group();
								{
									gui->begin_child("Saved Waypoints", 2, 2);
									{

									}
									gui->end_child();
								}
								gui->end_group();
							}
							gui->end_content();
						}
						if (subtabs == 5) {
							gui->begin_content();
							{
								using namespace rbx::globals;
								gui->begin_group();
								{
									gui->begin_child("Options", 2, 1);
									{
										gui->checkbox("VSync", &vsync);
										gui->checkbox("Cache Cham Polygons", &chams_caching);
									}
									gui->end_child();
								}
								gui->end_group();

								gui->sameline();

								gui->begin_group();
								{
									gui->begin_child("Menu Keybinds", 2, 1);
									{
										gui->label_keybind("Menu Key", &var->gui.menu_key, nullptr);
										gui->label_keybind("Menu Key Alt", &var->gui.menu_key_alt, nullptr);
										gui->label_keybind("Menu Key Alt 2", &var->gui.menu_key_alt2, nullptr);
										gui->label_keybind("Panic Key", &var->gui.exit_key, nullptr);
									}
									gui->end_child();
								}
								gui->end_group();
							}
							gui->end_content();
						}
					}

				}
				gui->end();
			}

			if (var->gui.current_section[1])
			{
				gui->set_next_window_size_constraints(ImVec2(640, 650), GetIO().DisplaySize);
				gui->begin("Misc", nullptr, var->window.flags);
				{
					draw->window_decorations();
				}
				gui->end();
			}

			if (var->gui.current_section[2])
			{
				gui->set_next_window_size_constraints(ImVec2(640, 650), GetIO().DisplaySize);
				gui->begin("Players", nullptr, var->window.flags);
				{
					draw->window_decorations();

					{
						gui->set_cursor_pos(elements->content.window_padding + ImVec2(0, var->window.titlebar + 1));
						gui->push_style_var(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
						gui->push_style_var(ImGuiStyleVar_ItemSpacing, elements->content.spacing);
						gui->begin_def_child("table test", ImVec2(GetWindowWidth() - elements->content.window_padding.x * 2, GetContentRegionAvail().y - elements->content.window_padding.y * 2), 0, ImGuiWindowFlags_NoMove);
						{
							gui->push_font(var->font.tahoma);
							static int selected_row = -1;

							gui->push_style_color(ImGuiCol_TableBorderLight, draw->get_clr(clr->window.stroke));
							gui->push_style_color(ImGuiCol_TableBorderStrong, draw->get_clr(clr->window.stroke));
							gui->push_style_color(ImGuiCol_TableRowBg, draw->get_clr(clr->window.background_one));
							gui->push_style_color(ImGuiCol_TableRowBgAlt, draw->get_clr(clr->window.background_one));
							gui->push_style_color(ImGuiCol_Text, draw->get_clr(clr->widgets.text_inactive));
							if (gui->begin_table("Table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg, ImVec2(GetContentRegionAvail().x - 1, 0)))
							{
								for (int row = 0; row < 10; row++)
								{
									gui->table_next_row();

									gui->table_set_column_index(0);
									{
										if (selected_row == row)
											gui->push_style_color(ImGuiCol_Text, draw->get_clr(clr->accent));

										draw->text_outline((std::stringstream{} << "name " << row).str().c_str());

										if (selected_row == row)
											gui->pop_style_color();

										if (IsItemClicked())
											selected_row = row;
									}

									gui->table_set_column_index(1);
									{
										draw->text_outline((std::stringstream{} << "pos " << row).str().c_str());
									}

									gui->table_set_column_index(2);
									{
										draw->text_outline((std::stringstream{} << "info " << row).str().c_str());
									}
								}
								gui->end_table();

								Text((std::stringstream{} << "selected row - " << std::to_string(selected_row)).str().c_str());
								gui->pop_font();
							}
							gui->pop_style_color(5);
						}
						gui->end_def_child();
						gui->pop_style_var(2);
					}

				}
				gui->end();

			}

			if (var->gui.current_section[3])
			{
				gui->set_next_window_size_constraints(ImVec2(640, 650), GetIO().DisplaySize);
				gui->begin("Menu", nullptr, var->window.flags);
				{
					draw->window_decorations();
				}
				gui->end();
			}

			if (var->gui.current_section[4])
			{
				gui->set_next_window_size_constraints(ImVec2(640, 650), GetIO().DisplaySize);
				gui->begin("Code", nullptr, var->window.flags);
				{
					draw->window_decorations();
				}
				gui->end();
			}

			if (var->gui.current_section[5])
			{
				gui->set_next_window_size_constraints(ImVec2(640, 650), GetIO().DisplaySize);
				gui->begin("Style Editor", nullptr, var->window.flags);
				{
					draw->window_decorations();

					{
						gui->set_cursor_pos(elements->content.window_padding + ImVec2(0, var->window.titlebar + 1));
						gui->begin_content();
						{
							gui->begin_child("Theme", 1, 2);
							{
								static float menu_accent[4] = { clr->accent.Value.x, clr->accent.Value.y, clr->accent.Value.z, 1.f };
								if (gui->label_color_edit("Menu Accent", menu_accent, false))
								{
									clr->accent.Value.x = menu_accent[0];
									clr->accent.Value.y = menu_accent[1];
									clr->accent.Value.z = menu_accent[2];
								}

								static float contrast_one[4] = { clr->window.background_one.Value.x, clr->window.background_one.Value.y, clr->window.background_one.Value.z, 1.f };
								if (gui->label_color_edit("Contrast One", contrast_one, false))
								{
									clr->window.background_one.Value.x = contrast_one[0];
									clr->window.background_one.Value.y = contrast_one[1];
									clr->window.background_one.Value.z = contrast_one[2];
								}

								static float contrast_two[4] = { clr->window.background_two.Value.x, clr->window.background_two.Value.y, clr->window.background_two.Value.z, 1.f };
								if (gui->label_color_edit("Contrast Two", contrast_two, false))
								{
									clr->window.background_two.Value.x = contrast_two[0];
									clr->window.background_two.Value.y = contrast_two[1];
									clr->window.background_two.Value.z = contrast_two[2];
								}

								static float inline_c[4] = { clr->window.stroke.Value.x, clr->window.stroke.Value.y, clr->window.stroke.Value.z, 1.f };
								if (gui->label_color_edit("Inline", inline_c, false))
								{
									clr->window.stroke.Value.x = inline_c[0];
									clr->window.stroke.Value.y = inline_c[1];
									clr->window.stroke.Value.z = inline_c[2];
								}

								static float outline_c[4] = { clr->widgets.stroke_two.Value.x, clr->widgets.stroke_two.Value.y, clr->widgets.stroke_two.Value.z, 1.f };
								if (gui->label_color_edit("Outline", outline_c, false))
								{
									clr->widgets.stroke_two.Value.x = outline_c[0];
									clr->widgets.stroke_two.Value.y = outline_c[1];
									clr->widgets.stroke_two.Value.z = outline_c[2];
								}

								static float text_active[4] = { clr->widgets.text.Value.x, clr->widgets.text.Value.y, clr->widgets.text.Value.z, 1.f };
								if (gui->label_color_edit("Text Active", text_active, false))
								{
									clr->widgets.text.Value.x = text_active[0];
									clr->widgets.text.Value.y = text_active[1];
									clr->widgets.text.Value.z = text_active[2];
								}

								static float text_inactive[4] = { clr->widgets.text_inactive.Value.x, clr->widgets.text_inactive.Value.y, clr->widgets.text_inactive.Value.z, 1.f };
								if (gui->label_color_edit("Text Inctive", text_inactive, false))
								{
									clr->widgets.text_inactive.Value.x = text_inactive[0];
									clr->widgets.text_inactive.Value.y = text_inactive[1];
									clr->widgets.text_inactive.Value.z = text_inactive[2];
								}
							}
							gui->end_child();

							gui->begin_child("Style", 1, 2);
							{
								gui->checkbox("Hover Highlight", &var->window.hover_hightlight);

								static bool window_glow = true;
								gui->checkbox("Window Glow", &window_glow);
								if (window_glow)
								{
									gui->slider_float("Glow Thickness", &var->window.shadow_size, 1, 100);
									gui->slider_float("Glow Alpha", &var->window.shadow_alpha, 0, 1);
								}
							}
							gui->end_child();
						}
						gui->end_content();
					}

				}
				gui->end();
			}
		}

		var->window.width = GetCurrentWindow()->ContentSize.x + style->ItemSpacing.x;

		if (IsMouseHoveringRect(pos, pos + size))
			SetWindowFocus();
	}
	gui->end();
	gui->pop_style_var();
}