#include "device_resources.hpp"
extern "C"
{
#include <dxgi1_4.h>
#include "../utils/windows_utils.h"
}

HRESULT Windows::DeviceResources::init(const DirectXConfig *config)
{
    HRESULT hr = S_OK;

    hr = create_device_resources();
    if (FAILED(hr))
    {
        LOG_ERROR("create_deivce_resources failed, Code 0x%081x", hr);
        goto exit;
    }

    hr = create_window_resources(config->window_handler);
    if (FAILED(hr))
    {
        LOG_ERROR("create_window_resources failed, Code 0x%081x", hr);
    }
exit:
    return (hr);
}

HRESULT Windows::DeviceResources::create_device_resources()
{
    HRESULT hr = S_OK;
    D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_9_1,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_11_1};

    UINT device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

    hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            device_flags,
            levels,
            ARRAYSIZE(levels),
            D3D11_SDK_VERSION,
            &device,
            &_feature_level,
            &context);
    if (FAILED(hr))
    {
        LOG_ERROR("D3D11CreateDevice failed, Code 0x%081x", hr);
        goto exit;
    }

    device.As(&_device);
    context.As(&_context);

exit:
    return (hr);
}

HRESULT Windows::DeviceResources::create_window_resources(HWND hwnd)
{
    HRESULT hr = S_OK;

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Windowed = TRUE;
    desc.BufferCount = 3;
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;

    desc.SampleDesc.Quality = 0;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    desc.OutputWindow = hwnd;

    Microsoft::WRL::ComPtr<IDXGIDevice3> dxgi_device;
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter3;
    Microsoft::WRL::ComPtr<IDXGIFactory> factory;

    _device.As(&dxgi_device);

    hr = dxgi_device->GetAdapter(&adapter);

    if (SUCCEEDED(hr))
    {
        adapter->GetParent(IID_PPV_ARGS(&factory));

        hr = factory->CreateSwapChain(_device.Get(), &desc, &_swap_chain);
    }
    hr = configure_back_buffer();

    return (hr);
}

HRESULT Windows::DeviceResources::configure_back_buffer()
{
    HRESULT hr = S_OK;
    CD3D11_TEXTURE2D_DESC dsd;
    CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);

    hr = _swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **) &_back_buffer);
    if (FAILED(hr))
        goto exit;

    hr = _device->CreateRenderTargetView(_back_buffer.Get(), nullptr, _render_target_view.GetAddressOf());
    if (FAILED(hr))
        goto exit;

    _back_buffer->GetDesc(&_texture_desc);

    dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsd.Width = static_cast<UINT>(_texture_desc.Width);
    dsd.Height = static_cast<UINT>(_texture_desc.Height);
    dsd.ArraySize = 1;
    dsd.MipLevels = 1;
    dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    _device->CreateTexture2D(&dsd, nullptr, &_depth_stencil);

    _device->CreateDepthStencilView(_depth_stencil.Get(), &dsvd, _depth_stencil_view.GetAddressOf());

    ZeroMemory(&_viewport, sizeof(_viewport));
    _viewport.Height = (float) _texture_desc.Height;
    _viewport.Width = (float) _texture_desc.Width;

    _context->RSSetViewports(1, &_viewport);
exit:
    return (hr);
}

HRESULT Windows::DeviceResources::release_back_buffer()
{
    HRESULT hr = S_OK;

    _render_target_view.Reset();

    _back_buffer.Reset();

    _depth_stencil_view.Reset();
    _depth_stencil.Reset();

    _context->Flush();
    return (hr);
}

HRESULT Windows::DeviceResources::go_full_screen()
{
    HRESULT hr = S_OK;

    hr = _swap_chain->SetFullscreenState(TRUE, NULL);

    release_back_buffer();

    hr = _swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    hr = configure_back_buffer();
    return (hr);
}

HRESULT Windows::DeviceResources::go_windowed()
{
    HRESULT hr = S_OK;

    hr = _swap_chain->SetFullscreenState(FALSE, NULL);

    release_back_buffer();

    hr = _swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    hr = configure_back_buffer();
    return (hr);
}

float Windows::DeviceResources::get_aspect_ratio()
{
    return (static_cast<float>(_texture_desc.Width) / static_cast<float>(_texture_desc.Height));
}

ID3D11Device *Windows::DeviceResources::get_device()
{
    return (_device.Get());
}

ID3D11DeviceContext *Windows::DeviceResources::get_context()
{
    return (_context.Get());
}

ID3D11RenderTargetView *Windows::DeviceResources::get_rtv()
{
    return (_render_target_view.Get());
}

ID3D11DepthStencilView *Windows::DeviceResources::get_depth_stencil()
{
    return (_depth_stencil_view.Get());
}

void Windows::DeviceResources::present()
{
    _swap_chain->Present(1, 0);
}
