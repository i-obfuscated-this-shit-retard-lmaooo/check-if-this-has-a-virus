#pragma once
#include "imgui/imgui.h"

class c_colors
{
public:
	ImColor accent{ 140, 70, 220 };

	struct
	{
		ImColor background_one{ 26, 26, 26 };
		ImColor background_two{ 18, 18, 18 };
		ImColor stroke{ 40, 40, 40 };
	} window;

	struct
	{
		ImColor stroke_two{ 12, 12, 12 };
		ImColor text{ 200, 200, 200 };
		ImColor text_inactive{ 136, 136, 136 };
	} widgets;
};

inline c_colors* clr = new c_colors();