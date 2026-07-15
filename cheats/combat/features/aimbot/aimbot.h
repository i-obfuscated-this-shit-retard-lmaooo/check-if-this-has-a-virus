#pragma once
#include "misc/globals.h"
#include "cache/cache.h"

class c_aimbot {
private:
	rbx::matrix3_t c_transition_start_rot;
	rbx::matrix3_t c_transition_target_rot;
	rbx::vector3_t c_target_world_pos;
	float c_transition_elapsed;
	float c_transition_duration;
	bool c_transition_active;

	rbx::vector2_t m_last_pos;
	bool m_smoothing_enabled;

	bool aim_camera(rbx::vector3_t world);
	bool aim_mouse(rbx::vector2_t screen);
	bool combat_checks(rbx::cache::cache_entity& player);
	rbx::part get_best_part(rbx::cache::cache_entity& entity);
	bool get_best_target();
	void reset();
public:
	c_aimbot() : c_transition_elapsed(0), c_transition_duration(0), c_transition_active(false),
		m_smoothing_enabled(false) {
	}
	void draw_fov();
	bool perform_aimbot();
	void update(float delta_time);
};

inline c_aimbot aimbot;