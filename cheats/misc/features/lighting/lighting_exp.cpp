#include "lighting_exp.h"

#pragma region lighting helper shit

static float earth_tilt = 0.4101523742186675f; // 23.44 deg in rad
static float moon_tilt = 0.089803443f;         // 5.145 deg in rad
static float pi = 3.1415927f;                  // uh, pi?
static float day = 86400.f;                    // seconds in a day
static float solar_year = 31558152.96f;        // seconds in a solar year
static float half_solar_year = 182.6282f;      // days in half of a solar year
static float moon_phase_interval = 2551442.8f; // seconds in a lunar month (29.53 days)

inline rbx::color3_t linear_spline(float t, const float* times, const rbx::color3_t* colors, int count) {
    if (t <= times[0]) return colors[0];
    if (t >= times[count - 1]) return colors[count - 1];
    for (int i = 0; i < count - 1; i++) {
        if (t >= times[i] && t <= times[i + 1]) {
            auto t0 = times[i];
            auto t1 = times[i + 1];
            auto alpha = float((t - t0) / (t1 - t0));

            return rbx::color3_t{
                colors[i].r * (1.f - alpha) + colors[i + 1].r * alpha,
                colors[i].g * (1.f - alpha) + colors[i + 1].g * alpha,
                colors[i].b * (1.f - alpha) + colors[i + 1].b * alpha
            };
        }
    }
    return colors[0];
}

enum { SECOND = 1, MINUTE = 60, HOUR = 60 * 60, DAY = 24 * 60 * 60, SUNRISE = 24 * 60 * 60 / 4, SUNSET = 24 * 60 * 60 * 3 / 4, MIDNIGHT = 0, METER = 1, KILOMETER = 1000 };
inline rbx::color3_t calculate_sky_ambient(float time, float diffuse_scale) {
    constexpr float HOUR = 3600.f;
    constexpr float MIDNIGHT = 0.f;
    constexpr float DAY = 86400.f;
    constexpr float SUNRISE = 6.f * HOUR;
    constexpr float SUNSET = 18.f * HOUR;
    constexpr float sunRiseAndSetTime = HOUR;

    {
        static const float times[] = { MIDNIGHT,           SUNRISE - 2 * HOUR,        SUNRISE - HOUR,         SUNRISE - HOUR / 2,       SUNRISE,                       SUNRISE + sunRiseAndSetTime,  SUNSET - sunRiseAndSetTime, SUNSET,                  SUNSET + HOUR / 3,     DAY };
        static const rbx::color3_t color[] = { rbx::color3_t(0.f,0.f,0.f),      rbx::color3_t(0.f,0.f,0.f),    rbx::color3_t(.07f,.07f,.1f),      rbx::color3_t(.2f, .15f, .01f),   rbx::color3_t(.2f, .15f, .01f),          rbx::color3_t(1.f,1.f,1.f),                rbx::color3_t(1.f,1.f,1.f),              rbx::color3_t(.4f, .2f, .05f),     rbx::color3_t(0.f,0.f,0.f),       rbx::color3_t(0.f,0.f,0.f) };
        return linear_spline(time, times, color, sizeof(times) / sizeof(times[0]));
    }


}

inline rbx::color3_t calculate_sky_ambient_2(float time, float diffuse_scale) {
    constexpr float HOUR = 3600.f;
    constexpr float MIDNIGHT = 0.f;
    constexpr float DAY = 86400.f;
    constexpr float SUNRISE = 6.f * HOUR;  // 6:00 AM
    constexpr float SUNSET = 18.f * HOUR; // 6:00 PM
    constexpr float sunRiseAndSetTime = HOUR;

    {
        static const float times[] = { MIDNIGHT,                  SUNRISE - 3 * HOUR,          SUNRISE - 2 * HOUR,              SUNRISE - HOUR / 2,             SUNRISE,                       SUNRISE + sunRiseAndSetTime,  SUNSET - sunRiseAndSetTime,             SUNSET,                  SUNSET + HOUR / 3,  SUNSET + 2 * HOUR,     SUNSET + 3 * HOUR,    DAY };
        static const rbx::color3_t color[] = { rbx::color3_t(0.f,0.f,0.f),   rbx::color3_t(0.f,0.f,0.f) * 0.7f ,     rbx::color3_t(0.3f,0.3f,0.4f) * 0.7f ,    rbx::color3_t(0.4f, 0.3f, 0.3f),     rbx::color3_t(0.3f, 0.2f, 0.3f),            rbx::color3_t(1.f,1.f,1.f),                rbx::color3_t(1.f,1.f,1.f),                   rbx::color3_t(.4f, .3f, .2f),          rbx::color3_t(0.3f,0.2f,0.3f),   rbx::color3_t(0.3f,0.2f,0.3f), rbx::color3_t(0.f,0.f,0.f),    rbx::color3_t(0.f,0.f,0.f) };
        return linear_spline(time, times, color, sizeof(times) / sizeof(times[0]));
    }


}

inline rbx::color3_t calculate_light_color(float time, float diffuse_scale) {
    constexpr float HOUR = 3600.f;
    constexpr float MIDNIGHT = 0.f;
    constexpr float DAY = 86400.f;
    constexpr float SUNRISE = 6.f * HOUR;  // 6:00 AM
    constexpr float SUNSET = 18.f * HOUR; // 6:00 PM
    constexpr float sunRiseAndSetTime = HOUR;

    const rbx::color3_t dayDiffuse = rbx::color3_t{ 1.f * 0.75f,1.f * 0.75f,1.f * 0.75f };
    static const float times[] = { MIDNIGHT,               SUNRISE - HOUR,         SUNRISE,              SUNRISE + sunRiseAndSetTime / 4,  SUNRISE + sunRiseAndSetTime,    SUNSET - sunRiseAndSetTime,     SUNSET - sunRiseAndSetTime / 2, SUNSET,                SUNSET + HOUR / 2,       DAY };
    static const rbx::color3_t color[] = { rbx::color3_t(.2f, .2f, .2f),  rbx::color3_t(.1f, .1f, .1f),    rbx::color3_t(0.f,0.f,0.f),        rbx::color3_t(.6f, .6f, 0.f),                dayDiffuse,                     dayDiffuse,                   rbx::color3_t(.1f, .1f, .075f),           rbx::color3_t(.1f, .05f, .05f),  rbx::color3_t(.1f, .1f, .1f), rbx::color3_t(.2f, .2f, .2f) };


    return linear_spline(time, times, color, 10);
}

inline bool set_time_helper(float geographic_lattitude, float diffuse_scale, float time) {
    auto time_of_day = time - floor(time / day) * day;
    auto source_angle = (float)(time_of_day * 2 * pi) / day;

    rbx::vector3_t sun_position = {
        sinf(source_angle),
        -cosf(source_angle),
        0.f
    };

    rbx::vector3_t moon_position = {
        sinf(source_angle + pi),
        -cosf(source_angle + pi),
        0.f
    };

    auto geographic_lattitude_radians = (geographic_lattitude * pi) / 180.f;
    auto day_of_year = (time - floor(time / solar_year) * time) / day;
    auto sun_offset = -earth_tilt * cos((day_of_year - half_solar_year) * pi / half_solar_year) - geographic_lattitude_radians;
    rbx::vector3_t sun_axis = rbx::vector3_t{ 0.f, 0.f, 1.f }.cross(sun_position);

    auto sun_axis_length = sqrtf(sun_axis.x * sun_axis.x + sun_axis.y * sun_axis.y + sun_axis.z * sun_axis.z);
    if (sun_axis_length > 0.f) {
        sun_axis.x /= sun_axis_length;
        sun_axis.y /= sun_axis_length;
        sun_axis.z /= sun_axis_length;
    }

    auto sun_rotation = rbx::matrix3_t::from_axis_angle(sun_axis, sun_offset);
    auto true_sun_position = sun_rotation * sun_position;

    auto initial_moon_phase = 0.5f; // half moon
    auto moon_phase = floor(time / moon_phase_interval) + initial_moon_phase;
    auto curr_moon_phase = moon_phase * 2 * pi;

    rbx::vector3_t moon_position_with_phase = {
        sinf(curr_moon_phase + source_angle),
        -cosf(curr_moon_phase + source_angle),
        0.f
    };

    auto moon_offset = ((-earth_tilt + moon_tilt) * sinf(moon_phase * 4.f)) - geographic_lattitude_radians;
    auto moon_axis = rbx::vector3_t{ 0.f, 0.f, 1.f }.cross(moon_position_with_phase);
    auto moon_axis_length = sqrtf(moon_axis.x * moon_axis.x + moon_axis.y * moon_axis.y + moon_axis.z * moon_axis.z);
    if (moon_axis_length > 0.f) {
        moon_axis.x /= moon_axis_length;
        moon_axis.y /= moon_axis_length;
        moon_axis.z /= moon_axis_length;
    }

    auto moon_rotation = rbx::matrix3_t::from_axis_angle(moon_axis, moon_offset);
    auto true_moon_position = moon_rotation * moon_position_with_phase;
    rbx::lighting_source source = rbx::lighting_source::sun;
    rbx::vector3_t light_direction;
    if (true_sun_position.y > -0.3f) {
        source = rbx::lighting_source::sun;
        light_direction = true_sun_position;
    }
    else {
        source = rbx::lighting_source::moon;
        light_direction = true_moon_position;
    }

    auto light_color = calculate_light_color((float)time_of_day, diffuse_scale);
    auto sky_ambient = calculate_sky_ambient((float)time_of_day, diffuse_scale);
    auto sky_ambient_2 = calculate_sky_ambient_2((float)time_of_day, diffuse_scale);

    auto min_light_intensity = diffuse_scale * 0.34999999f;
    light_color.r = max(light_color.r, min_light_intensity);
    light_color.g = max(light_color.g, min_light_intensity);
    light_color.b = max(light_color.b, min_light_intensity);

    using namespace rbx::globals::instances;

    if (!g_lighting.set_light_direction(light_direction)) return false;
    if (!g_lighting.set_light_color(light_color)) return false;
    if (!g_lighting.set_sun_position(true_sun_position)) return false;
    if (!g_lighting.set_moon_position(true_moon_position)) return false;
    if (!g_lighting.set_sky_ambient(sky_ambient)) return false;
    if (!g_lighting.set_sky_ambient_2(sky_ambient_2)) return false;
    if (!g_lighting.set_source(source)) return false;
    if (!g_lighting.set_clock_time(time)) return false; // avoid detections :shhhh:

    return true;
}

#pragma endregion

bool c_lighting::set_clock_time(float time) {
    // reminder to my niga self, time is in seconds
    using namespace rbx::globals::instances;
    using namespace rbx::globals::cheats::misc::lighting::clock_time;
    auto geographic_lattitude = g_lighting.get_geographic_latitude();
    auto diffuse_scale = g_lighting.get_environment_diffuse_scale();
    return set_time_helper(geographic_lattitude, diffuse_scale, time);;
}

static bool save_time = true;
static float saved_time = 0.f; // seconds

bool c_lighting::update() {
    using namespace rbx::globals::instances;
    using namespace rbx::globals::cheats::misc::lighting;
    if (!g_lighting.addr) return false;
    if (!g_render_view.addr) return false;
    if (!enabled) return true;

    if (!g_render_view.set_lighting_valid(false)) return false;

    {
        using namespace clock_time;
        if (!clock_time::enabled) return true;
        if (save_time) { saved_time = g_lighting.get_clock_time(); save_time = false; }
        if (gui->is_key_enabled(&keybind, &keybind_type)) set_clock_time(current_time);
        else { set_clock_time(saved_time); saved_time = 0.f; save_time = true; }
    }

    if (!g_render_view.set_lighting_valid(false)) return false;

    return true;
}
