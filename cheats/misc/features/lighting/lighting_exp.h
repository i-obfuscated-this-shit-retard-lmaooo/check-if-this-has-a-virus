#pragma once
#include "misc/globals.h"

#include "classes/lighting/lighting.h"

class c_lighting {
private:
	bool set_clock_time(float time); // seconds
public:
	bool update();
};

inline c_lighting lighting_cheat;