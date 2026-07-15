#include "triggerbot.h"
#include "cheats/combat/helpers.h"

void mouse_left_down() {
	INPUT tuff = {};
	tuff.type = INPUT_MOUSE;
	tuff.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &tuff, sizeof(INPUT));
}

void mouse_left_up() {
	INPUT tuff = {};
	tuff.type = INPUT_MOUSE;
	tuff.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &tuff, sizeof(INPUT));
}

void mouse_left_click() {
	INPUT tuff[2] = {};
	tuff[0].type = INPUT_MOUSE;
	tuff[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	tuff[1].type = INPUT_MOUSE;
	tuff[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(2, tuff, sizeof(INPUT));
}

bool mouse_click_helper(bool real) {
    using namespace rbx::globals;
    using namespace rbx::globals::cheats::combat::triggerbot;

    static auto click_timer = 0.f;
    static auto hold_timer = 0.f;
    static auto holding = false;

    auto delta = dt.load(std::memory_order_relaxed);

    if (!real) {
        click_timer = 0.f;
        hold_timer = 0.f;
        if (holding) { mouse_left_up(); holding = false; }
        return false;
    }

    switch (type) {
    case 0: { 
        // click
        auto dur = 1.f / clicks_per_second;
        click_timer += delta;
        if (click_timer >= dur) { mouse_left_click(); click_timer -= dur; return true; }
        break;
    }

    case 1: { 
        // hold
        if (!holding) { mouse_left_down(); holding = true; hold_timer = 0.f; }
        hold_timer += delta;
        if (hold_timer >= hold_duration) { mouse_left_up(); holding = false; hold_timer = 0.f; }
        return true;
    }
    }

    return false;
}

bool c_triggerbot::perform_triggerbot() {
	using namespace rbx::globals::cheats::combat::triggerbot;
    if (!enabled) return true;
	if (!gui->is_key_enabled(&keybind, &keybind_type)) { mouse_click_helper(false); return true; }
	POINT x;
	if (!GetCursorPos(&x)) return false;
	if (!ScreenToClient(rbx::globals::our_window, &x)) return false;
	rbx::vector2_t cursor = { (float)(x.x), (float)(x.y) };
    {
        std::lock_guard<std::mutex> lock(rbx::cache::cache_mutex);
        if (rbx::cache::cached_entities.empty()) return false;
        for (auto& player : rbx::cache::cached_entities) {
            for (auto& it : player.parts_screen) {
                if (!it.first) continue;
                auto delta = it.second - cursor;
                auto dist = delta.x * delta.x + delta.y * delta.y;
                if (dist <= range) return mouse_click_helper(true);
            }
        }
        mouse_click_helper(false);
    }
	return true;
}