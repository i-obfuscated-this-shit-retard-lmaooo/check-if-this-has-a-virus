#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "renderer.h"

ID3D11Device* renderer_t::g_pd3dDevice = nullptr;
ID3D11DeviceContext* renderer_t::g_pd3dDeviceContext = nullptr;
IDXGISwapChain* renderer_t::g_pSwapChain = nullptr;
bool renderer_t::g_SwapChainOccluded = false, renderer_t::g_OverlayVisible = false;
UINT renderer_t::g_ResizeWidth = 0, renderer_t::g_ResizeHeight = 0;
ID3D11RenderTargetView* renderer_t::g_mainRenderTargetView = nullptr;

bool renderer_t::is_fullscreen(HWND hwnd)
{
    MONITORINFO mi = { sizeof(mi) };
    if (GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi))
    {
        RECT rect;
        if (GetWindowRect(hwnd, &rect)) {
            return rect.left == mi.rcMonitor.left
                && rect.right == mi.rcMonitor.right
                && rect.top == mi.rcMonitor.top
                && rect.bottom == mi.rcMonitor.bottom;
        }
    }
}

void renderer_t::move_window(HWND hwnd)
{
    HWND rbx = FindWindow("WINDOWSCLIENT", nullptr);
    HWND fg = GetForegroundWindow();

    if (rbx != fg && hwnd != fg) { MoveWindow(hwnd, 0, 0, 0, 0, TRUE); return; }

    RECT game_rect;
    POINT top_left;

    if (!GetClientRect(rbx, &game_rect)) return;

    top_left.x = game_rect.left;
    top_left.y = game_rect.top;
    if (!ClientToScreen(rbx, &top_left)) return; // adjust the topleft to the inner client area

    int w = game_rect.right - game_rect.left;
    int h = game_rect.bottom - game_rect.top;

    MoveWindow(hwnd, top_left.x, top_left.y, w, h, TRUE);

    // copying the window region to make clipped rendering
    auto reg = CreateRectRgn(0, 0, 0, 0);
    if (!GetWindowRgn(rbx, reg)) { DeleteObject(reg); return; }
    SetWindowRgn(hwnd, reg, TRUE);
}

// taken from imgui example
bool LoadTextureFromMemory(const void* data, size_t data_size, ID3D11ShaderResourceView** out_srv) {
    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    renderer_t::g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    renderer_t::g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    stbi_image_free(image_data);

    return true;
}

void renderer_t::initialize() {
    ImGui_ImplWin32_EnableDpiAwareness();

    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_VREDRAW | CS_HREDRAW,
        WndProc,
        0,
        0,
        GetModuleHandle(nullptr),
        LoadIcon(nullptr, IDI_APPLICATION),
        LoadCursor(nullptr, IDC_ARROW),
        CreateSolidBrush(RGB(0, 0, 0)),
        nullptr,
        "ImGui Example",
        LoadIcon(nullptr, IDI_APPLICATION)
    };

    ::RegisterClassEx(&wc);
    auto hwnd = ::CreateWindowEx(WS_EX_TOPMOST, wc.lpszClassName, "Dear ImGui DirectX11 Example", WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);
    rbx::globals::our_window = hwnd;

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margin);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    auto& style = ImGui::GetStyle();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    float scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
    io.IniFilename = nullptr;
    style.ScaleAllSizes(scale);

    // font init
    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_MonoHinting;
    cfg.FontDataOwnedByAtlas = false;

    rbx::globals::fonts::visitor_s = io.Fonts->AddFontFromMemoryTTF((void*)(visitor1), sizeof(visitor1), 12.f, &cfg);
    rbx::globals::fonts::visitor_m = io.Fonts->AddFontFromMemoryTTF((void*)(visitor1), sizeof(visitor1), 14.f, &cfg);
    rbx::globals::fonts::visitor_l = io.Fonts->AddFontFromMemoryTTF((void*)(visitor1), sizeof(visitor1), 18.f, &cfg);

    var->font.icons[0] = io.Fonts->AddFontFromMemoryTTF(section_icons_hex, sizeof section_icons_hex, 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    var->font.icons[1] = io.Fonts->AddFontFromMemoryTTF(icons_hex, sizeof icons_hex, 5.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    var->font.tahoma = io.Fonts->AddFontFromMemoryTTF(tahoma_hex, sizeof tahoma_hex, 13.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    auto def = io.Fonts->AddFontDefault();
    io.FontDefault = def;

    {
        // waifu images init
        ID3D11ShaderResourceView* mai_srv = nullptr;
        LoadTextureFromMemory(mai, sizeof(mai), &mai_srv);
        rbx::globals::waifu_images::mai = (ImTextureID)(mai_srv);

        ID3D11ShaderResourceView* waguri_srv = nullptr;
        LoadTextureFromMemory(waguri, sizeof(waguri), &waguri_srv);
        rbx::globals::waifu_images::waguri = (ImTextureID)(waguri_srv);

        ID3D11ShaderResourceView* alya_srv = nullptr;
        LoadTextureFromMemory(alya, sizeof(alya), &alya_srv);
        rbx::globals::waifu_images::alya = (ImTextureID)(alya_srv);

        ID3D11ShaderResourceView* hori_srv = nullptr;
        LoadTextureFromMemory(hori, sizeof(hori), &hori_srv);
        rbx::globals::waifu_images::hori = (ImTextureID)(hori_srv);
    }

    {
        ID3D11ShaderResourceView* logo_srv = nullptr;
        LoadTextureFromMemory(logo, sizeof(logo), &logo_srv);
        rbx::globals::logo = (ImTextureID)(logo_srv);
    }

    rbx::globals::dt.store(io.DeltaTime, std::memory_order_relaxed);

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    while (rbx::globals::g_running) {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT) rbx::globals::g_running = false;
        }
        
        if (!rbx::globals::g_running) break;

        auto rbx = FindWindow("WINDOWSCLIENT", nullptr);
        if (!rbx) { rbx::globals::g_running = false; break; }

        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        rbx = FindWindow("WINDOWSCLIENT", nullptr);
        auto fg = GetForegroundWindow();

        if (fg == rbx || fg == hwnd) {
            move_window(hwnd);

            gui->render();

            ImGui::SetNextWindowPos(ImVec2());
            auto flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMouseInputs |
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
            ImGui::Begin("##visuals", nullptr, flags);
            rbx::globals::draw = ImGui::GetBackgroundDrawList();
            using namespace rbx::globals::instances;
            g_view_matrix = g_visual_engine.get_view_matrix();
            g_dimensions = g_visual_engine.get_dimensions();
            rbx::cheats::visuals::thread();
            ImGui::End();
        }

        auto a = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW;
        auto b = a | WS_EX_TRANSPARENT;
        SetWindowLong(hwnd, GWL_EXSTYLE, (fg == rbx || fg == hwnd) && var->gui.menu_opened ? a : b);

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        HRESULT hr = g_pSwapChain->Present(rbx::globals::vsync, 0);
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

bool renderer_t::CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void renderer_t::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void renderer_t::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void renderer_t::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

LRESULT WINAPI renderer_t::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}