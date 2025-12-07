#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

namespace Windows
{
    struct DirectXConfig
    {
        HWND window_handler;
        UINT width;
        UINT height;
        bool is_window;
    };

    class DeviceResources
    {
    public:
        explicit DeviceResources() = default;
        ~DeviceResources() = default;

        HRESULT init(const DirectXConfig *config);

        HRESULT create_device_resources();
        HRESULT create_window_resources(HWND hwnd);

        HRESULT configure_back_buffer();
        HRESULT release_back_buffer();
        HRESULT go_full_screen();
        HRESULT go_windowed();

        float get_aspect_ratio();

        ID3D11Device *get_device();
        ID3D11DeviceContext *get_context();
        ID3D11RenderTargetView *get_rtv();
        ID3D11DepthStencilView *get_depth_stencil();

        void present();

    private:
        /* Direct3D device */
        Microsoft::WRL::ComPtr<ID3D11Device> _device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
        Microsoft::WRL::ComPtr<IDXGISwapChain> _swap_chain;

        /* DXGI swap chain device resources */
        Microsoft::WRL::ComPtr<ID3D11Texture2D> _back_buffer;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _render_target_view;

        /* Direct3D device resources for the depth stencil */
        Microsoft::WRL::ComPtr<ID3D11Texture2D> _depth_stencil;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _depth_stencil_view;

        /* Direct3D device metadata and device resource metadata */
        D3D_FEATURE_LEVEL _feature_level;
        D3D11_TEXTURE2D_DESC _texture_desc;
        D3D11_VIEWPORT _viewport;
    };
} // namespace Windows
