#pragma once
#include <array>
#include <cstddef>
#include <vector>

#include "device_resources.hpp"
#include "renderer.h"
#include "renderer_types.h"
#include "system/compositor.h"

namespace System::Render
{

    class Dx11Adapter
    {
        std::vector<RenderObject> _items;
        RenderingEnvironment _environment;
        std::vector<D3D11_INPUT_ELEMENT_DESC> _input_layout_desc_scratch;


        ComPtr<ID3D11Buffer> _global_vb;
        ComPtr<ID3D11Buffer> _global_ib;
        ComPtr<ID3D11BlendState> _alpha_blend_state;
        ComPtr<ID3D11DepthStencilState> _depth_stencil_state;

    public:
        explicit Dx11Adapter(Windows::DeviceResources &resources);
        void convert(Windows::DeviceResources &resources, const std::vector<RenderItem> &items);
        void render_all_items(Windows::DeviceResources &resource);

        HRESULT load_vs_if_not_exist(ID3D11Device *device, AssetsRecord *vs_rec, VertexShaderCache **out);
        HRESULT load_ps_if_not_exist(ID3D11Device *device, AssetsRecord *ps_rec, PixelShaderCache **out);
        HRESULT load_sp_if_not_exist(ID3D11Device *device, AssetsRecord *sp_rec, SpriteCache **out);

        ~Dx11Adapter();
    };
} // namespace System::Render
