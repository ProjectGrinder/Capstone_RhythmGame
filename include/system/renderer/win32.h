#pragma once
#include <Windows.h>
#include "renderer_types.h"

namespace System::Renderer
{
  class Win32Renderer
  {
  private:
    Microsoft::WRL::ComPtr<ID3D11Device> _device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> _swap_chain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _render_target_view;

    uint32_t Win32Renderer::_initialize_directx(uint32_t width, uint32_t height,HWND window_handler, DisplayType display_type);

  public:
    Win32Renderer();

    void render(RenderItem &item);
  };
}