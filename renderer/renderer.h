#pragma once
#include <windows.h>
#include <string>
#include <algorithm>
#include <tchar.h>

#include <d3d11.h>
#include <dwmapi.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/misc/freetype/imgui_freetype.h"

#include "misc/globals.h"
#include "cheats/visuals/visuals.h"

#include "misc/assets/fonts.h"
#include "misc/assets/graphics.h"

#include "framework/settings/functions.h"
#include "framework/data/fonts.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class renderer_t {
public:
	static ID3D11Device* g_pd3dDevice;
	static ID3D11DeviceContext* g_pd3dDeviceContext;
	static IDXGISwapChain* g_pSwapChain;
	static bool g_SwapChainOccluded, g_OverlayVisible;
	static UINT g_ResizeWidth, g_ResizeHeight;
	static ID3D11RenderTargetView* g_mainRenderTargetView;

	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static bool is_fullscreen(HWND hwnd);
	static void move_window(HWND hwnd);

	static void initialize();
};

inline auto renderer = std::make_unique<renderer_t>();