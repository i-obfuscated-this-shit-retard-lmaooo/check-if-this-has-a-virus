#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <array>

#include "classes/entity/entity.h"
#include "classes/value_object/value_object.h"
#include "misc/math.h"

namespace rbx {
	namespace cache {
		struct straw_hat {
			std::vector<std::pair<vector3_t, vector3_t>> circumference_lines;
			std::vector<std::pair<vector3_t, vector3_t>> connection_lines;
			float scale;
		};

		enum rig_part : int {
			head = 0,
			humanoidrootpart,
			torso,
			right_arm,
			left_arm,
			right_leg,
			left_leg,

			// r15 only
			upper_torso,
			lower_torso,
			right_upper_arm,
			right_lower_arm,
			right_hand,
			left_upper_arm,
			left_lower_arm,
			left_hand,
			right_upper_leg,
			right_lower_leg,
			right_foot,
			left_upper_leg,
			left_lower_leg,
			left_foot,

			count
		};

		rbx::cache::rig_part get_part_index(const std::string& name);

		struct cache_entity {
			entity player;
			part head;
			part humanoid;
			part humanoidrootpart;
			std::string name;
			std::string tool;
			instance team;
			std::string team_name;
			bool is_teammate;
			bool is_alive;
			float distance;
			float health;
			float max_health;
			vector2_t top_left;
			vector2_t bottom_right;
			vector3_t move_direction;
			rig_type rig;
			int state_id;
			int waifu_idx;
			straw_hat hat;
			std::array<part, rig_part::count> parts;
			std::array<vector3_t, rig_part::count> partsizes;
			using part_data = std::pair<bool, vector2_t>;
			std::array<part_data, rig_part::count> parts_screen;

			std::vector<part_data> box_3d_points;

			using cham_data = std::vector<part_data>;
			std::array<cham_data, rig_part::count> chams_screen;
	

			struct {
				instance nrpbs;
				value_object health;
				value_object max_health;
				instance status;
				value_object team;
				std::string team_name;
			} arsenal;

			struct {

			} rivals;
		};

		void thread();

		inline std::mutex cache_mutex;
		inline std::vector<cache_entity> cached_entities;
	}
}
