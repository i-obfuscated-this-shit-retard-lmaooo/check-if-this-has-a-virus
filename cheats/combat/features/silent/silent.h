#pragma once
#include "misc/globals.h"
#include "cache/cache.h"

class c_silent {
private:
	bool aim_mouse(rbx::vector2_t screen);
	bool aim_camera(rbx::vector2_t screen);
	bool combat_checks(rbx::cache::cache_entity& player);
	rbx::part get_best_part(rbx::cache::cache_entity& entity);
	bool get_best_target();
	void reset();
public:
	void draw_fov();
	bool perform_silent();
};

inline c_silent silent;