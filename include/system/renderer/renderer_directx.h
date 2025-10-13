#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include "renderer_types.h"

namespace System::Renderer
{
    enum DisplayType
    {
        FULLSCREEN,
        BORDERLESS,
        WINDOW,
    };

    struct WindowInfo
    {
        uint32_t width, height, monitor_width, monitor_height;
        HWND window_handler;
        DisplayType display_type;
        bool is_running;
    };

    class Directx
    {
    private:
        Microsoft::WRL::ComPtr<ID3D11Device> _device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
        Microsoft::WRL::ComPtr<IDXGISwapChain3> _swap_chain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _render_target_view;

    public:
        explicit Directx() = default;

        uint32_t init(
                WindowInfo &window_info);

        void render(RenderItem &item);
    };
} // namespace System::Renderer
