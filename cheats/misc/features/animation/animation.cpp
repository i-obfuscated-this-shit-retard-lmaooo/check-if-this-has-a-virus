#include "animation.h"

bool c_animation_changer::update() {
	using namespace rbx::globals::cheats::misc::animation_changer;
	auto& lp = rbx::globals::instances::g_local_player;
	if (!lp.addr) return false;
	if (!enabled) {
		if (last_pack != rbx::animation_pack::original) {
			animator.apply_pack(lp, rbx::animation_pack::original);
			last_pack = rbx::animation_pack::original;
		}
		return true;
	}
	if (gui->is_key_enabled(&keybind, &keybind_type)) {
		if (last_pack != pack.load()) {
			animator.apply_pack(lp, pack.load());
			last_pack = pack.load();
		}
	}
	else {
		if (last_pack != rbx::animation_pack::original) {
			animator.apply_pack(lp, rbx::animation_pack::original);
			last_pack = rbx::animation_pack::original;
		}
	}
	return true;
}