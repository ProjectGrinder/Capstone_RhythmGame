#include "Windows.h"
#include "exception"

#include "system.h"

namespace System::Renderer
{
    void Directx::render([[maybe_unused]]RenderItem &item)
    {

    }

    uint32_t Directx::init(WindowInfo &window_info)
    {
        uint32_t error = ERROR_SUCCESS;

        using Microsoft::WRL::ComPtr;

        ComPtr<ID3D11Device> device;
        ComPtr<ID3D11DeviceContext> context;
        ComPtr<IDXGIDevice> dxgi_device;
        ComPtr<IDXGIAdapter> adapter;
        ComPtr<IDXGIFactory2> factory;
        ComPtr<IDXGISwapChain1> swap_chain1;
        ComPtr<IDXGISwapChain3> final_swap_chain;
        ComPtr<ID3D11Texture2D> backbuffer;
        ComPtr<ID3D11RenderTargetView> rtv;

        D3D_FEATURE_LEVEL level = {};
        D3D_FEATURE_LEVEL feature_levels[] = {D3D_FEATURE_LEVEL_11_0};

        DXGI_SWAP_CHAIN_DESC1 sc_desc = {0};
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fs_desc = {0};

        sc_desc.BufferCount = 3; // <- Triple buffering
        sc_desc.Width = window_info.width;
        sc_desc.Height = window_info.height;
        sc_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sc_desc.Stereo = false;
        sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sc_desc.SampleDesc.Count = 1;
        sc_desc.Scaling = DXGI_SCALING_STRETCH;
        sc_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        sc_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
        sc_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

        fs_desc.Windowed = window_info.display_type == DisplayType::WINDOW;

    #ifdef _DEBUG
        error = D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED,
                feature_levels,
                _countof(feature_levels),
                D3D11_SDK_VERSION,
                &device,
                &level,
                &context);
    #else
        error = D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                D3D11_CREATE_DEVICE_SINGLETHREADED,
                feature_levels,
                _countof(feature_levels),
                D3D11_SDK_VERSION,
                &device,
                &level,
                &context);
    #endif
        if (FAILED(error))
            goto Exit;

        error = device.As(&dxgi_device);
        if (FAILED(error))
            goto Exit;

        error = dxgi_device->GetAdapter(&adapter);
        if (FAILED(error))
            goto Exit;

        error = adapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void **>(factory.GetAddressOf()));
        if (FAILED(error))
            goto Exit;

        error = factory->CreateSwapChainForHwnd(device.Get(), window_info.window_handler, &sc_desc, &fs_desc, nullptr, &swap_chain1);
        if (FAILED(error))
            goto Exit;

        error = swap_chain1.As(&final_swap_chain);
        if (FAILED(error))
            goto Exit;

        error = final_swap_chain->GetBuffer(
                0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(backbuffer.GetAddressOf()));
        if (FAILED(error))
            goto Exit;

        error = device->CreateRenderTargetView(backbuffer.Get(), nullptr, &rtv);
        if (FAILED(error))
            goto Exit;

        this->_device = device;
        this->_context = context;
        this->_swap_chain = final_swap_chain;
        this->_render_target_view = rtv;

Exit:
        return(error);
    }
}