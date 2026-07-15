#pragma once
#include <string>
#include <vector>
#include "imgui/imgui.h"

class c_variables
{
public:
	struct
	{
		ImGuiWindowFlags main_flags{ 
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_Tooltip
		};
		ImGuiWindowFlags flags{ 
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoBackground 
		};
		ImVec2 padding{ 0, 0 };
		ImVec2 spacing{ 5, 6 };
		float shadow_size{ 20 };
		float shadow_alpha{ 0.2f };
		float border_size{ 0 };
		float rounding{ 6 };
		float width{ 0 };
		float titlebar{ 20 };
		float scrollbar_size{ 2 };
		bool hover_hightlight{ true };
	} window;

	struct {
		ImGuiWindowFlags flags{
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_NoTitleBar
		};
		ImVec2 padding{ 0, 0 };
		ImVec2 spacing{ 5, 6 };
		float shadow_size{ 20 };
		float shadow_alpha{ 0.3f };
		float border_size{ 0 };
		float rounding{ 6 };
		float width{ 0 };
		bool enabled{ true };
		bool version{ true };
		bool fps{ true };
		bool date{ true };
	} watermark;

	struct
	{
		bool current_section[7] = {true,false,false,false,false,false,false};
		const char* section_icons[IM_ARRAYSIZE(current_section)] = {"A", "B", "C", "D", "E", "F", "G"};

		float menu_alpha{ 0 };
		bool menu_opened{ false };
		int menu_key{ 36 };       // home
		int menu_key_alt{ 45 };   // insert
		int menu_key_alt2{ 161 }; // rshift
		int exit_key{ 35 };       // end
	} gui;

	struct
	{
		ImFont* icons[2];
		ImFont* tahoma;
	} font;
};

inline c_variables* var = new c_variables();