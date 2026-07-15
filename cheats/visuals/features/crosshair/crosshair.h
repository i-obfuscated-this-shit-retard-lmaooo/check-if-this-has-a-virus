#pragma once
#include "cheats/visuals/helpers.h"

namespace rbx::cheats::visuals {
	class v_crosshair {
	private:
		ImVec2 rotate_point_around_center(ImVec2 center, ImVec2 point, float angle);
		float get_animated_rotation(float time);
		float get_animated_distance(float original, float amp, float f, float t);
	public:
		void draw_crosshair();
	};

	inline v_crosshair crosshair;
}