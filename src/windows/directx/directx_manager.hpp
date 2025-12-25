#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d11.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

namespace Windows
{
    struct DirectxConfig
    {
        HWND window_handler;
        UINT width;
        UINT height;
        bool is_window;
    };

    class DirectxManager
    {
    private:
        Microsoft::WRL::ComPtr<ID3D11Device> _device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
        Microsoft::WRL::ComPtr<IDXGISwapChain3> _swap_chain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _render_target_view;

    public:
        explicit DirectxManager() = default;

        uint32_t init(const DirectxConfig *config);
    };
}