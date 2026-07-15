#pragma once
#include "misc/globals.h"
#include "cheats/visuals/features/basics/basics.h" // within_fov + locked indicator

static std::mt19937 g(0);
static std::uniform_int_distribution<int> d;

inline int get_random_val(int cap) {
	d = std::uniform_int_distribution<int>(0, cap);
	auto r = d(g);
	return r;
}

inline float pulse_value(float min, float max, float p) {
	auto original = (max + min) * 0.5f;
	auto amp = (max - min) * 0.5f;
	auto f = 2.f * IM_PI / p;
	auto time = (float)(ImGui::GetTime());
	return original + amp * sinf(f * time);
}

inline float cubic_bezier(float t, float p0, float p1, float p2, float p3) {
	auto u = 1.f - t;
	auto tt = t * t;
	auto uu = u * u;
	return (uu * u * p0) + (3.f * uu * t * p1) + (3.f * u * tt * p2) + (tt * t * p3);
}

inline float solve_bezier(float x, float p1x, float p2x) {
	auto t = x;
	for (int i = 0; i < 8; i++) {
		auto xt = cubic_bezier(t, 0.f, p1x, p2x, 1.f);
		auto dt = xt - x;
		if (std::abs(dt) < 0.001f) break;
		auto u = 1.f - t;
		auto derivative = 3.f * u * u * p1x + 6.f * u * t * (p2x - p1x) + 3.f * t * t * (1.f - p2x);
		if (std::abs(derivative) < 0.000001f) break;
		t -= dt / derivative;
		t = std::clamp(t, 0.f, 1.f);
	}
	return t;
}

inline float apply_easing(float t, const float easing[4]) {
	if (t <= 0.0f) return 0.f;
	if (t >= 1.0f) return 1.f;
	auto solved = solve_bezier(t, easing[0], easing[2]);
	return cubic_bezier(solved, 0.0f, easing[1], easing[3], 1.f);
}