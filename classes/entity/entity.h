#pragma once
#include "classes/instance/instance.h"

#include "classes/part/part.h"

namespace rbx {
	enum rig_type : int {
		r6 = 0,
		r15 = 1
	};

	struct entity : instance {
		part get_part(std::string part_name);

		entity get_character(); // modelinstance

		part get_humanoid();
		part get_humanoid_root_part();
		part get_head();

		std::string get_display_name();
		float get_health();
		float get_max_health();
		instance get_team();
		bool is_teammate();
		rig_type get_rig_type();
		uint64_t get_user_id();
		vector3_t get_move_direction();
		int get_humanoid_state_id();
	};
}