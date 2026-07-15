#pragma once
#include "misc/globals.h"
#include "cache/cache.h"

#include "cheats/visuals/helpers.h"

static bool within_fov(rbx::vector2_t center, float radius, int segments, rbx::vector2_t point) {
    auto dir = point - center;
    auto dist = point.distance(center);
    auto angle = atan2f(dir.y, dir.x);
    auto segment_step = (2.f * IM_PI) / (float)segments;
    auto segment_angle = fmodf(angle, segment_step);
    if (segment_angle < 0) segment_angle += segment_step;
    auto half_step = segment_step * 0.5f;
    auto new_rad = (radius * cosf(half_step)) / cosf(segment_angle - half_step);
    return dist <= new_rad;
}

namespace rbx::cheats::visuals {
#pragma region some_stuff

    /*
    struct connection {
        std::string from;
        std::string to;
    };

    inline std::vector<connection> connections_r6 = {
        { "Head", "Torso" },
        { "Torso", "Left Arm" },
        { "Torso", "Right Arm" },
        { "Torso", "Left Leg" },
        { "Torso", "Right Leg" }
    };

    inline std::vector<connection> connections_r15 = {
        { "Head", "UpperTorso" },
        { "UpperTorso", "LowerTorso" },
        { "UpperTorso", "LeftUpperArm" },
        { "UpperTorso", "RightUpperArm" },
        { "LeftUpperArm", "LeftLowerArm" },
        { "RightUpperArm", "RightLowerArm" },
        { "LeftLowerArm", "LeftHand" },
        { "RightLowerArm", "RightHand" },
        { "LowerTorso", "LeftUpperLeg" },
        { "LowerTorso", "RightUpperLeg" },
        { "LeftUpperLeg", "LeftLowerLeg" },
        { "RightUpperLeg", "RightLowerLeg" },
        { "LeftLowerLeg", "LeftFoot" },
        { "RightLowerLeg", "RightFoot" }
    };

    inline std::vector<std::string> bones_r6 = {
        "Head",
        "Torso",
        "Right Arm",
        "Left Arm",
        "Right Leg",
        "Left Leg"
    };

    inline std::vector<std::string> bones_r15 = {
        "Head",
        "UpperTorso",
        "LowerTorso",
        "RightUpperLeg",
        "LeftUpperLeg",
        "RightLowerLeg",
        "LeftLowerLeg",
        "RightFoot",
        "LeftFoot",
        "RightUpperArm",
        "LeftUpperArm",
        "RightLowerArm",
        "LeftLowerArm",
        "RightHand",
        "LeftHand"
    };
    */

    struct connection {
        rbx::cache::rig_part from;
        rbx::cache::rig_part to;
    };

    inline std::vector<connection> connections_r6 = {
        { rbx::cache::rig_part::head, rbx::cache::rig_part::torso },
        { rbx::cache::rig_part::torso, rbx::cache::rig_part::left_arm },
        { rbx::cache::rig_part::torso, rbx::cache::rig_part::right_arm },
        { rbx::cache::rig_part::torso, rbx::cache::rig_part::left_leg },
        { rbx::cache::rig_part::torso, rbx::cache::rig_part::right_leg }
    };

    inline std::vector<connection> connections_r15 = {
        { rbx::cache::rig_part::head, rbx::cache::rig_part::upper_torso },
        { rbx::cache::rig_part::upper_torso, rbx::cache::rig_part::lower_torso },
        { rbx::cache::rig_part::upper_torso, rbx::cache::rig_part::left_upper_arm },
        { rbx::cache::rig_part::upper_torso, rbx::cache::rig_part::right_upper_arm },
        { rbx::cache::rig_part::left_upper_arm, rbx::cache::rig_part::left_lower_arm },
        { rbx::cache::rig_part::right_upper_arm, rbx::cache::rig_part::right_lower_arm },
        { rbx::cache::rig_part::left_lower_arm, rbx::cache::rig_part::left_hand },
        { rbx::cache::rig_part::right_lower_arm, rbx::cache::rig_part::right_hand },
        { rbx::cache::rig_part::lower_torso, rbx::cache::rig_part::left_upper_leg },
        { rbx::cache::rig_part::lower_torso, rbx::cache::rig_part::right_upper_leg },
        { rbx::cache::rig_part::left_upper_leg, rbx::cache::rig_part::left_lower_leg },
        { rbx::cache::rig_part::right_upper_leg, rbx::cache::rig_part::right_lower_leg },
        { rbx::cache::rig_part::left_lower_leg, rbx::cache::rig_part::left_foot },
        { rbx::cache::rig_part::right_lower_leg, rbx::cache::rig_part::right_foot }
	};

    inline std::vector<rbx::cache::rig_part> bones_r6 = {
        rbx::cache::rig_part::head,
        rbx::cache::rig_part::torso,
        rbx::cache::rig_part::right_arm,
        rbx::cache::rig_part::left_arm,
        rbx::cache::rig_part::right_leg,
        rbx::cache::rig_part::left_leg
	};

    inline std::vector<rbx::cache::rig_part> bones_r15 = {
        rbx::cache::rig_part::head,
        rbx::cache::rig_part::upper_torso,
        rbx::cache::rig_part::lower_torso,
        rbx::cache::rig_part::right_upper_arm,
        rbx::cache::rig_part::right_lower_arm,
        rbx::cache::rig_part::right_hand,
        rbx::cache::rig_part::left_upper_arm,
        rbx::cache::rig_part::left_lower_arm,
        rbx::cache::rig_part::left_hand,
        rbx::cache::rig_part::right_upper_leg,
        rbx::cache::rig_part::right_lower_leg,
        rbx::cache::rig_part::right_foot,
        rbx::cache::rig_part::left_upper_leg,
        rbx::cache::rig_part::left_lower_leg,
        rbx::cache::rig_part::left_foot,
	};

#pragma endregion

    class v_basics {
    private:
        void draw_bounding_box(rbx::cache::cache_entity& player);
        void draw_skeleton(rbx::cache::cache_entity& player);
        void draw_health_bar(rbx::cache::cache_entity& player);

        void draw_name(rbx::cache::cache_entity& player);
        void draw_movement_status(rbx::cache::cache_entity& player, float& offest);
        void draw_player_rig(rbx::cache::cache_entity& player, float& offest);
        void draw_distance(rbx::cache::cache_entity& player, float& offset);
        void draw_tool_name(rbx::cache::cache_entity& player, float& offset);
        void draw_snaplines(rbx::cache::cache_entity& player);
        void draw_in_fov_snaplines_aimbot(rbx::cache::cache_entity& player);
        void draw_in_fov_snaplines_silent(rbx::cache::cache_entity& player);

        void draw_chams(rbx::cache::cache_entity& player);
        void draw_waifu_esp(rbx::cache::cache_entity& player);
        void draw_straw_hat(rbx::cache::cache_entity& player);
        void draw_skeleton_dots(rbx::cache::cache_entity& player);
        void draw_3d_box(rbx::cache::cache_entity& player);
    public:
        void draw_locked_indicator();
        void draw(rbx::cache::cache_entity& player);
    };

    inline v_basics basic_features;
}