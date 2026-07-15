#pragma once
#include "misc/globals.h"

class c_tickrate {
private:
	bool set_tickrate(float tickrate);
public:
	bool update();
};

inline c_tickrate tickrate;