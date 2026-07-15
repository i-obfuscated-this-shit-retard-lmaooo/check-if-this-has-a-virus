#pragma once
#include "misc/globals.h"
#include "cache/cache.h"

#include "cheats/visuals/helpers.h"

#include "features/basics/basics.h"
#include "features/crosshair/crosshair.h"

#include "cheats/combat/features/aimbot/aimbot.h" // fov
#include "cheats/combat/features/silent/silent.h" // fov

namespace rbx::cheats::visuals {
    void thread();
}