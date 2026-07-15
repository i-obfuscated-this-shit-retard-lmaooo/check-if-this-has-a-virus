#pragma once
#include "misc/globals.h"

inline void draw_text(ImDrawList* draw, ImVec2& pos, ImU32 color, const char* text, bool outline) {
	auto outline_col = ImColor(0, 0, 0);
	if (outline) {
		//draw->AddText({ pos.x - 1.f, pos.y }, outline_col, text);
		//draw->AddText({ pos.x + 1.f, pos.y }, outline_col, text);
		//draw->AddText({ pos.x, pos.y - 1.f }, outline_col, text);
		//draw->AddText({ pos.x, pos.y + 1.f }, outline_col, text);

		draw->AddText({ pos.x - 1.f, pos.y - 1.f }, outline_col, text);
		draw->AddText({ pos.x + 1.f, pos.y + 1.f }, outline_col, text);
		draw->AddText({ pos.x + 1.f, pos.y - 1.f }, outline_col, text);
		draw->AddText({ pos.x - 1.f, pos.y + 1.f }, outline_col, text);
	}
	draw->AddText(pos, color, text);
}

// overload with font
inline void draw_text(ImDrawList* draw, ImFont* font, ImVec2& pos, ImU32 color, const char* text, bool outline) {
	auto outline_col = ImColor(0, 0, 0);
	if (outline) {
		//draw->AddText(font, font->FontSize, { pos.x - 1.f, pos.y }, outline_col, text);
		//draw->AddText(font, font->FontSize, { pos.x + 1.f, pos.y }, outline_col, text);
		//draw->AddText(font, font->FontSize, { pos.x, pos.y - 1.f }, outline_col, text);
		//draw->AddText(font, font->FontSize, { pos.x, pos.y + 1.f }, outline_col, text);

		draw->AddText(font, font->FontSize, { pos.x - 1.f, pos.y - 1.f }, outline_col, text);
		draw->AddText(font, font->FontSize, { pos.x + 1.f, pos.y + 1.f }, outline_col, text);
		draw->AddText(font, font->FontSize, { pos.x + 1.f, pos.y - 1.f }, outline_col, text);
		draw->AddText(font, font->FontSize, { pos.x - 1.f, pos.y + 1.f }, outline_col, text);
	}
	draw->AddText(font, font->FontSize, pos, color, text);
}

// overload with font & fontsize
inline void draw_text(ImDrawList* draw, ImFont* font, float font_size, ImVec2& pos, ImU32 color, const char* text, bool outline) {
	auto outline_col = ImColor(0, 0, 0);
	if (outline) {
		//draw->AddText(font, font_size, { pos.x - 1.f, pos.y }, outline_col, text);
		//draw->AddText(font, font_size, { pos.x + 1.f, pos.y }, outline_col, text);
		//draw->AddText(font, font_size, { pos.x, pos.y - 1.f }, outline_col, text);
		//draw->AddText(font, font_size, { pos.x, pos.y + 1.f }, outline_col, text);

		draw->AddText(font, font_size, { pos.x - 1.f, pos.y - 1.f }, outline_col, text);
		draw->AddText(font, font_size, { pos.x + 1.f, pos.y + 1.f }, outline_col, text);
		draw->AddText(font, font_size, { pos.x + 1.f, pos.y - 1.f }, outline_col, text);
		draw->AddText(font, font_size, { pos.x - 1.f, pos.y + 1.f }, outline_col, text);
	}
	draw->AddText(font, font_size, pos, color, text);
}

inline void draw_line(ImDrawList* draw, ImVec2 start, ImVec2 end, ImU32 color, float thickness, bool outline) {
	auto outline_col = ImColor(0, 0, 0);
	if (outline)
		draw->AddLine(start, end, outline_col, thickness + 2.f);
	draw->AddLine(start, end, color, thickness);
}

// overload for special case
inline void draw_line(ImDrawList* draw, ImVec2 start, ImVec2 end, ImU32 color, float thickness, bool outline, bool overload) {
	auto outline_col = ImColor(0, 0, 0);
	if (outline)
		if (overload) {
			draw->AddLine({ start.x - 1.f, start.y }, { end.x - 1.f, end.y }, outline_col, thickness);
			draw->AddLine({ start.x + 1.f, start.y }, { end.x + 1.f, end.y }, outline_col, thickness);
			draw->AddLine({ start.x, start.y - 1.f }, { end.x, end.y - 1.f }, outline_col, thickness);
			draw->AddLine({ start.x, start.y + 1.f }, { end.x, end.y + 1.f }, outline_col, thickness);
		}
		else draw->AddLine(start, end, outline_col, thickness + 2.f);
	draw->AddLine(start, end, color, thickness);
}

inline void draw_rect(ImDrawList* draw, ImVec2 top_left, ImVec2 bottom_right, ImU32 color, bool outline, float thickness = 1.f, float rounding = 0.f) {
	auto outline_col = ImColor(0, 0, 0);
	if (outline) {
		//draw->AddRect({ top_left.x - 1, top_left.y - 1 }, { bottom_right.x + 1, bottom_right.y + 1 }, outline_col, rounding, 0, thickness);
		//draw->AddRect({ top_left.x + 1, top_left.y + 1 }, { bottom_right.x - 1, bottom_right.y - 1 }, outline_col, rounding, 0, thickness);
		draw->AddRect(top_left, bottom_right, outline_col, rounding, 0, thickness);
		draw->AddRect({ top_left.x + 2, top_left.y + 2 }, { bottom_right.x - 2, bottom_right.y - 2 }, outline_col, rounding, 0, thickness);
	}
	//draw->AddRect(top_left, bottom_right, color, rounding, 0, thickness);
	draw->AddRect({ top_left.x + 1, top_left.y + 1 }, { bottom_right.x - 1, bottom_right.y - 1 }, color, rounding, 0, thickness);
}

inline void draw_rect_filled(ImDrawList* draw, ImVec2 top_left, ImVec2 bottom_right, ImU32 color, float rounding = 0.f) {
	draw->AddRectFilled(top_left, bottom_right, color, rounding);
}