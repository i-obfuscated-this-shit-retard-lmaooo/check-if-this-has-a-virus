#pragma once
#include "misc/globals.h"
#include "classes/animator/animator.h"

class c_animation_changer {
private:
    rbx::animator animator;
    rbx::animation_pack last_pack = rbx::animation_pack::original;
public:
    bool update();
};

inline c_animation_changer animation_changer;