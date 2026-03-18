#include "dx11_adapter.h"
#include <algorithm>
#include <d3d11.h>
#include "device_resources.hpp"
#include "library/dds.hpp"
#include "renderer_types.h"
#include "system/asset_manager.h"
#include "utils/input_attribute_description.h"
#include "utils/print_debug.h"

namespace System::Render
{
    typedef void *DX11AdapterHandler;
    extern "C" void *heap_alloc(size_t size);
    extern "C" DX11AdapterHandler get_dx11_adapter();
    const UINT MAX_VERTICES = 100000;
    const UINT MAX_INDICES = 300000;
    const INT PASSES = 8; // Radix 8 passes

    inline constexpr DXGI_FORMAT common_to_dxgi_format(const InputType &type)
    {
        switch (type)
        {
        case R32G32B32A32_FLOAT:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case R32G32B32_FLOAT:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        case FLOAT32BITS:
            return DXGI_FORMAT_R32_FLOAT;
        case FLOAT16BITS:
            return DXGI_FORMAT_R16_FLOAT;
        case UINT32BITS:
            return DXGI_FORMAT_R32_UINT;
        case UINT16BITS:
            return DXGI_FORMAT_R16_UINT;
        case UINT8BITS:
            return DXGI_FORMAT_R8_UINT;
        default:
            return DXGI_FORMAT_UNKNOWN;
        }
    }

    static inline D3D11_INPUT_ELEMENT_DESC create_input_element_desc(const InputAttributeDescription &desc)
    {
        D3D11_INPUT_ELEMENT_DESC desc_out;
        desc_out.SemanticName = desc.semantic;
        desc_out.SemanticIndex = 0;
        desc_out.Format = common_to_dxgi_format(desc.type);
        desc_out.InputSlot = 0;
        desc_out.AlignedByteOffset = static_cast<UINT>(desc.offset);
        desc_out.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        desc_out.InstanceDataStepRate = 0;
        return (desc_out);
    }

    inline constexpr bool is_block_compressed(DXGI_FORMAT format)
    {
        switch (format)
        {
        case DXGI_FORMAT_BC1_UNORM:
        case DXGI_FORMAT_BC1_UNORM_SRGB:
        case DXGI_FORMAT_BC2_UNORM:
        case DXGI_FORMAT_BC2_UNORM_SRGB:
        case DXGI_FORMAT_BC3_UNORM:
        case DXGI_FORMAT_BC3_UNORM_SRGB:
        case DXGI_FORMAT_BC4_UNORM:
        case DXGI_FORMAT_BC4_SNORM:
        case DXGI_FORMAT_BC5_UNORM:
        case DXGI_FORMAT_BC5_SNORM:
        case DXGI_FORMAT_BC6H_UF16:
        case DXGI_FORMAT_BC6H_SF16:
        case DXGI_FORMAT_BC7_UNORM:
        case DXGI_FORMAT_BC7_UNORM_SRGB:
            return true;
        default:
            return false;
        }
    }

    static inline UINT get_mip_pitch(const UINT &width, const DXGI_FORMAT &texture_format)
    {
        if (is_block_compressed(texture_format))
        {
            const UINT blockSize =
                    (texture_format == DXGI_FORMAT_BC1_UNORM ||
                     texture_format == DXGI_FORMAT_BC1_UNORM_SRGB ||
                     texture_format == DXGI_FORMAT_BC4_UNORM ||
                     texture_format == DXGI_FORMAT_BC4_SNORM)
                            ? 8u
                            : 16u;
            return max(1u, (width + 3u) / 4u) * blockSize;
        }

        const UINT bitsPerPixel = dds::getBitsPerPixel(texture_format);
        return max(1u, (width * bitsPerPixel + 7u) / 8u);
    }

    bool create_sprite_texture(ID3D11Device *device, dds::Image &image, DXGI_FORMAT texture_format, SpriteRenderObject &sprite_render_object)
    {
        if (image.mipmaps.empty())
            return false;

        D3D11_TEXTURE2D_DESC tex_desc{};
        tex_desc.Width = image.width;
        tex_desc.Height = image.height;
        tex_desc.MipLevels = image.numMips;
        tex_desc.ArraySize = image.arraySize;
        tex_desc.Format = texture_format;
        tex_desc.SampleDesc.Count = 1;
        tex_desc.SampleDesc.Quality = 0;
        tex_desc.Usage = D3D11_USAGE_DEFAULT;
        tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        tex_desc.CPUAccessFlags = 0;
        tex_desc.MiscFlags = 0;

        std::vector<D3D11_SUBRESOURCE_DATA> subresource;
        subresource.reserve(image.mipmaps.size());

        UINT mipWidth = image.width;
        for (const auto &mip : image.mipmaps)
        {
            D3D11_SUBRESOURCE_DATA init{};
            init.pSysMem = mip.data();
            init.SysMemPitch = get_mip_pitch(mipWidth, texture_format);
            init.SysMemSlicePitch = 0;
            subresource.push_back(init);

            mipWidth = max(1u, mipWidth >> 1);
        }

        HRESULT tex_hr = device->CreateTexture2D(&tex_desc, subresource.data(), &sprite_render_object.texture);
        if (FAILED(tex_hr))
        {
            LOG_ERROR("Failed to create sprite texture, Code 0x%08lx", tex_hr);
            return false;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc{};
        srv_desc.Format = texture_format;
        srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D.MostDetailedMip = 0;
        srv_desc.Texture2D.MipLevels = image.numMips;

        tex_hr = device->CreateShaderResourceView(
                sprite_render_object.texture.Get(), &srv_desc, &sprite_render_object.texture_view);
        if (FAILED(tex_hr))
        {
            LOG_ERROR("Failed to create sprite SRV, Code 0x%08lx", tex_hr);
            sprite_render_object.texture.Reset();
            return false;
        }

        D3D11_SAMPLER_DESC sampler_desc{};
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampler_desc.MipLODBias = 0.0f;
        sampler_desc.MaxAnisotropy = 1;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampler_desc.BorderColor[0] = 0.0f;
        sampler_desc.BorderColor[1] = 0.0f;
        sampler_desc.BorderColor[2] = 0.0f;
        sampler_desc.BorderColor[3] = 0.0f;
        sampler_desc.MinLOD = 0.0f;
        sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

        tex_hr = device->CreateSamplerState(&sampler_desc, &sprite_render_object.sampler_state);
        if (FAILED(tex_hr))
        {
            LOG_ERROR("Failed to create sprite sampler, Code 0x%08lx", tex_hr);
            sprite_render_object.texture_view.Reset();
            sprite_render_object.texture.Reset();
            return false;
        }

        return true;
    };

    Dx11Adapter::Dx11Adapter(Windows::DeviceResources &resources)
    {
        _environment.context = resources.get_context();
        auto device = resources.get_device();

        D3D11_BUFFER_DESC vbd = {};
        vbd.Usage = D3D11_USAGE_DYNAMIC;
        vbd.ByteWidth = MAX_VERTICES * sizeof(Math::Point);
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        device->CreateBuffer(&vbd, nullptr, &_global_vb);

        D3D11_BUFFER_DESC ibd = vbd;
        ibd.ByteWidth = MAX_INDICES * sizeof(UINT);
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        device->CreateBuffer(&ibd, nullptr, &_global_ib);

        _v_staging.reserve(MAX_VERTICES);
        _i_staging.reserve(MAX_INDICES);
        _items.reserve(1000);
        _batched.reserve(1000);
        _input_layout_desc_scratch.reserve(16);

        _vs_fast_cache.assign(65536, {(uint32_t) -1, nullptr});
        _ps_fast_cache.assign(65536, {(uint32_t) -1, nullptr});
        _input_fast_cache.assign(65536, {(uint32_t) -1, nullptr});
    }

    void Dx11Adapter::convert(Windows::DeviceResources &resources, const std::vector<CompositorItem> &items)
    {
        _sprites.clear();

        _items.clear();
        _items.reserve(items.size());
        _batched.clear();
        _batched.reserve(items.size());

        D3D11_MAPPED_SUBRESOURCE v_map, i_map;
        if (FAILED(_environment.context->Map(_global_vb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &v_map)))
            return;
        if (FAILED(_environment.context->Map(_global_ib.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &i_map)))
        {
            _environment.context->Unmap(_global_vb.Get(), 0);
            return;
        }

        Math::Point *v_ptr = static_cast<Math::Point *>(v_map.pData);
        UINT *i_ptr = static_cast<UINT *>(i_map.pData);

        size_t v_idx = 0;
        size_t i_idx = 0;

        auto device = resources.get_device();

        for (auto &[kind, common, special]: items)
        {
            if (_v_staging.size() + 3 > MAX_VERTICES || _i_staging.size() + 3 > MAX_INDICES)
            {
                LOG_WARNING("Too much geometry! Skipping items.");
                break;
            }

            RenderObject render_object{};
            HRESULT hr = S_OK;

            const AssetsRecord *vs_rec = common.vs;
            const AssetsRecord *ps_rec = common.ps;

            uint16_t vs_idx = ASSET_INDEX(common.vs->id);

            if (vs_idx >= _vs_fast_cache.size())
            {
                LOG_ERROR("Invalid Shader ID: %u", common.vs->info.name);
                continue;
            }

            if (vs_rec->gpu_extension != NULL)
            {
                render_object.vertex_shader = ((VertexShaderCache *) vs_rec->gpu_extension)->shader;
                render_object.input_layout = ((VertexShaderCache *) vs_rec->gpu_extension)->layout;
            }
            else if (_vs_fast_cache[vs_idx].full_id == vs_rec->id)
            {
                render_object.vertex_shader = _vs_fast_cache[vs_idx].shader;
                render_object.input_layout = _input_fast_cache[vs_idx].layout;
            }
            else
            {
                if (vs_rec && vs_rec->data)
                {
                    hr = device->CreateVertexShader(
                            vs_rec->data->data, vs_rec->data->size, nullptr, &render_object.vertex_shader);

                    if (FAILED(hr))
                    {
                        LOG_ERROR("Failed to create vertex shader, Code 0x%08lx", hr);
                        continue;
                    }

                    _input_layout_desc_scratch.clear();
                    for (size_t i = 0; i < vs_rec->info.info.as_shader.count; ++i)
                        _input_layout_desc_scratch.push_back(
                                create_input_element_desc(vs_rec->info.info.as_shader.data[i]));

                    device->CreateInputLayout(
                            _input_layout_desc_scratch.data(),
                            (UINT) _input_layout_desc_scratch.size(),
                            vs_rec->data->data,
                            vs_rec->data->size,
                            &render_object.input_layout);

                    if (FAILED(hr))
                    {
                        LOG_ERROR("Failed to create input layout, Code 0x%08lx", hr);
                        continue;
                    }

                    _vs_fast_cache[vs_idx] = {vs_rec->id, render_object.vertex_shader};
                    _input_fast_cache[vs_idx] = {vs_rec->id, render_object.input_layout};
                    VertexShaderCache *extension = (VertexShaderCache *) heap_alloc(sizeof(VertexShaderCache));
                    extension->shader = render_object.vertex_shader;
                    extension->layout = render_object.input_layout;

                    AssetsRecord *unsafe = const_cast<AssetsRecord *>(vs_rec);
                    unsafe->gpu_extension = (void *) extension;
                }
            }

            uint16_t ps_idx = ASSET_INDEX(ps_rec->id);
            if (ps_rec->gpu_extension != NULL)
            {
                render_object.pixel_shader = ((PixelShaderCache *) ps_rec->gpu_extension)->shader;
            }
            else if (_ps_fast_cache[ps_idx].full_id == ps_rec->id)
            {
                render_object.pixel_shader = _ps_fast_cache[ps_idx].shader;
            }
            else
            {
                if (ps_rec && ps_rec->data)
                {
                    hr = device->CreatePixelShader(
                            ps_rec->data->data, ps_rec->data->size, nullptr, &render_object.pixel_shader);

                    if (FAILED(hr))
                    {
                        LOG_ERROR("Failed to create pixel shader, Code 0x%08lx", hr);
                        continue;
                    }

                    _ps_fast_cache[ps_idx] = {ps_rec->id, render_object.pixel_shader};
                    PixelShaderCache *extension = (PixelShaderCache *) heap_alloc(sizeof(PixelShaderCache));
                    extension->shader = render_object.pixel_shader;
                    AssetsRecord *unsafe = const_cast<AssetsRecord *>(ps_rec);
                    unsafe->gpu_extension = (void *) extension;
                }
            }

            switch (kind)
            {
            case DrawKind::KIND_TRIANGLE: {
                auto const &[points] = std::get<TriangleDrawDesc>(special);
                auto base = i_idx;
                render_object.vertex_base = (INT) v_idx;
                render_object.offset = (UINT) base;
                render_object.count.index_count = 3;

                render_object.render_id.sort_key = common.key;
                render_object.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

                v_ptr[v_idx++] = points[0];
                v_ptr[v_idx++] = points[1];
                v_ptr[v_idx++] = points[2];

                i_ptr[i_idx++] = (UINT) base + 0;
                i_ptr[i_idx++] = (UINT) base + 1;
                i_ptr[i_idx++] = (UINT) base + 2;

                break;
            }
            case DrawKind::KIND_SPRITE: {
                const AssetsRecord *sp_rec = common.sp;
                auto const &[points, flipX, flipY] = std::get<SpriteDrawDesc>(special);

                SpriteRenderObject sprite_render_object{};
                sprite_render_object.render_id.sort_key = common.key;
                sprite_render_object.vertex_shader = render_object.vertex_shader;
                sprite_render_object.pixel_shader = render_object.pixel_shader;
                sprite_render_object.input_layout = nullptr;
                sprite_render_object.vertex_buffer = nullptr;
                sprite_render_object.index_buffer = nullptr;
                sprite_render_object.count.index_count = 6;
                sprite_render_object.offset = 0;
                sprite_render_object.vertex_base = 0;
                sprite_render_object.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                sprite_render_object.index_format = DXGI_FORMAT_R32_UINT;

                constexpr Rect src_rect = {0, 0, 1, 1};

                // Build UV quad vertices
                const float u0 = flipX ? src_rect.u1 : src_rect.u0;
                const float u1 = flipX ? src_rect.u0 : src_rect.u1;
                const float v0 = flipY ? src_rect.v1 : src_rect.v0;
                const float v1 = flipY ? src_rect.v0 : src_rect.v1;

                Math::PointUv quad[4]{};
                quad[0] = {points[0].pos[0], points[0].pos[1], points[0].pos[2], u0, v0};
                quad[1] = {points[1].pos[0], points[1].pos[1], points[1].pos[2], u1, v0};
                quad[2] = {points[2].pos[0], points[2].pos[1], points[2].pos[2], u1, v1};
                quad[3] = {points[3].pos[0], points[3].pos[1], points[3].pos[2], u0, v1};

                // DDS-backed texture upload.
                // 1) get raw bytes from AssetsRecord
                // 2) dds::readImage(...)
                // 3) CreateTexture2D + CreateShaderResourceView (whenever that happens)
                if (sp_rec != nullptr && sp_rec->data != nullptr)
                {
                    dds::Image image{};
                    const auto *raw_bytes = reinterpret_cast<const std::uint8_t *>(sp_rec->data->data);
                    const std::size_t raw_size = sp_rec->data->size;

                    bool success = false;

                    if (dds::readImage(raw_bytes, raw_size, &image) == dds::ReadResult::Success)
                    {
                        switch (image.format)
                        {
                        case DXGI_FORMAT_R8G8B8A8_UNORM: {
                            success = create_sprite_texture(device, image, DXGI_FORMAT_R8G8B8A8_UNORM, sprite_render_object);
                            break;
                        }
                        case DXGI_FORMAT_BC1_UNORM: {
                            success = create_sprite_texture(device, image, DXGI_FORMAT_BC1_UNORM, sprite_render_object);
                            break;
                        }
                        case DXGI_FORMAT_BC3_UNORM: {
                            success = create_sprite_texture(device, image, DXGI_FORMAT_BC3_UNORM, sprite_render_object);
                            break;
                        }
                        default: {
                            LOG_ERROR("Unsupported SPRITE format %d", image.format);
                            break;
                        }
                        }
                    }

                    if (!success)
                    {
                        LOG_ERROR("Failed to create sprite texture");
                        continue;
                    }
                }

                _sprites.push_back(std::move(sprite_render_object));
                // go back to prevent duplicating render_object and messing with pipeline
                continue;
            }
            default:
                break;
            }

            _items.push_back(std::move(render_object));
        }

        _environment.context->Unmap(_global_vb.Get(), 0);
        _environment.context->Unmap(_global_ib.Get(), 0);

        // O(nlogn)
        //        std::sort(
        //                _items.begin(),
        //                _items.end(),
        //                [](const RenderObject &a, const RenderObject &b) { return a.get_sort_key() <
        //                b.get_sort_key();
        //                });

        // LOG_INFO("Converted %ld items into %ld render objects (Sorted)", items.size(), _items.size());

        size_t total_count = _items.size();
        if (total_count == 0)
            return;

        if (_indices_a.size() < total_count)
        {
            _indices_a.resize(total_count);
            _indices_b.resize(total_count);
        }

        for (uint32_t i = 0; i < (uint32_t) total_count; ++i)
            _indices_a[i] = i;


        uint32_t *idx_src = _indices_a.data();
        uint32_t *idx_dst = _indices_b.data();

        for (int p = 0; p < PASSES; ++p)
        {
            _counts.fill(0);
            int shift = p * 8;

            for (size_t i = 0; i < total_count; ++i)
                ++_counts[(_items[idx_src[i]].render_id.sort_key >> shift) & 0xFF];

            _offsets[0] = 0;
            for (int i = 1; i < 256; ++i)
                _offsets[i] = _offsets[i - 1] + _counts[i - 1];

            for (size_t i = 0; i < total_count; ++i)
            {
                uint8_t bucket = (_items[idx_src[i]].render_id.sort_key >> shift) & 0xFF;
                idx_dst[_offsets[bucket]++] = idx_src[i];
            }
            std::swap(idx_src, idx_dst);
        }

        _batched.clear();
        for (size_t i = 0; i < total_count; ++i)
        {
            RenderObject &current = _items[idx_src[i]];

            if (!_batched.empty() && _batched.back().render_id.sort_key == current.render_id.sort_key)
            {
                if (_batched.back().vertex_base + (INT) (_batched.back().count.index_count) == current.vertex_base)
                {
                    _batched.back().count.index_count += current.count.index_count;
                    continue;
                }
            }
            _batched.push_back(std::move(current));
        }

        _items = std::move(_batched);
        _batched.clear();

        LOG_INFO("Radix-Batched %lu items into %lu draw calls", items.size(), _items.size());
    }

    void Dx11Adapter::render_all_items(Windows::DeviceResources &device)
    {
        auto ctx = device.get_context();
        auto rtv = device.get_rtv();
        auto dsv = device.get_depth_stencil();

        float clearColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
        _environment.context->ClearRenderTargetView(rtv, clearColor);
        _environment.context->OMSetRenderTargets(1, &rtv, dsv);

        if (dsv)
        {
            _environment.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        }

        UINT stride = sizeof(Math::Point);
        UINT offset = 0;
        ctx->IASetVertexBuffers(0, 1, _global_vb.GetAddressOf(), &stride, &offset);
        ctx->IASetIndexBuffer(_global_ib.Get(), DXGI_FORMAT_R32_UINT, 0);

        for (const auto &item: _items)
        {
            render(&_environment, &item);
        }
    }

    Dx11Adapter::~Dx11Adapter()
    {
        if (_environment.context)
        {
            _environment.context->ClearState();
            _environment.context->Flush();
        }
        _items.clear();
        _batched.clear();

        _vs_fast_cache.clear();
        _ps_fast_cache.clear();
        _input_fast_cache.clear();

        _global_vb.Reset();
        _global_ib.Reset();
    }
} // namespace System::Render

extern "C"
{
    void vertex_shader_release(void **shader_handler)
    {
        auto *cache = static_cast<VertexShaderCache *>(*shader_handler);
        if (cache->shader)
            cache->shader->Release();
        if (cache->layout)
            cache->layout->Release();

        *shader_handler = NULL;
    }

    void pixel_shader_release(void **shader_handler)
    {
        auto *cache = static_cast<PixelShaderCache *>(*shader_handler);
        if (cache->shader)
            cache->shader->Release();
        *shader_handler = NULL;
    }
}
