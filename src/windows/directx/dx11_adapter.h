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
        std::vector<RenderObject> _batched;
        RenderingEnvironment _environment;

        std::vector<SpriteRenderObject> _sprites;

        struct VS_Entry
        {
            uint32_t full_id = 0;
            Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
        };
        struct PS_Entry
        {
            uint32_t full_id = 0;
            Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
        };
        struct IL_Entry
        {
            uint32_t full_id = 0;
            Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
        };

        std::vector<VS_Entry> _vs_fast_cache;
        std::vector<PS_Entry> _ps_fast_cache;
        std::vector<IL_Entry> _input_fast_cache;
        std::vector<D3D11_INPUT_ELEMENT_DESC> _input_layout_desc_scratch;

        std::vector<uint32_t> _indices_a;
        std::vector<uint32_t> _indices_b;
        std::array<size_t, 256> _counts;
        std::array<size_t, 256> _offsets;

        std::vector<Math::Point> _v_staging;
        std::vector<UINT> _i_staging;

        ComPtr<ID3D11Buffer> _global_vb;
        ComPtr<ID3D11Buffer> _global_ib;

    public:
        explicit Dx11Adapter(Windows::DeviceResources &resources);
        void convert(Windows::DeviceResources &resources, const std::vector<CompositorItem> &items);
        void render_all_items(Windows::DeviceResources &resource);
        ~Dx11Adapter();
    };
} // namespace System::Render
