#include "dx11_adapter.h"
#include <d3d11.h>
#include <vector>
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
    const UINT MAX_BYTES = 100000 * sizeof(Math::Point);
    const UINT MAX_INDICES = 300000;

    inline constexpr DXGI_FORMAT common_to_dxgi_format(const InputType &type)
    {
        switch (type)
        {
        case R32G32B32A32_FLOAT:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case R32G32B32_FLOAT:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        case R32G32_FLOAT:
            return DXGI_FORMAT_R32G32_FLOAT;
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
            return (true);
        default:
            return (false);
        }
    }

    static inline UINT get_mip_pitch(const UINT &width, const DXGI_FORMAT &texture_format)
    {
        if (is_block_compressed(texture_format))
        {
            const UINT blockSize =
                    (texture_format == DXGI_FORMAT_BC1_UNORM || texture_format == DXGI_FORMAT_BC1_UNORM_SRGB ||
                     texture_format == DXGI_FORMAT_BC4_UNORM || texture_format == DXGI_FORMAT_BC4_SNORM)
                            ? 8u
                            : 16u;
            return (max(1u, (width + 3u) / 4u) * blockSize);
        }

        const UINT bitsPerPixel = dds::getBitsPerPixel(texture_format);
        return (max(1u, (width * bitsPerPixel + 7u) / 8u));
    }

    Dx11Adapter::Dx11Adapter(Windows::DeviceResources &resources)
    {
        _environment.context = resources.get_context();
        auto device = resources.get_device();

        D3D11_BUFFER_DESC vbd = {};
        vbd.Usage = D3D11_USAGE_DYNAMIC;
        vbd.ByteWidth = MAX_BYTES;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        device->CreateBuffer(&vbd, nullptr, &_global_vb);

        D3D11_BUFFER_DESC ibd = vbd;
        ibd.ByteWidth = MAX_INDICES * sizeof(UINT);
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        device->CreateBuffer(&ibd, nullptr, &_global_ib);

        _items.reserve(1000);
        _input_layout_desc_scratch.reserve(16);
    }

    HRESULT Dx11Adapter::load_vs_if_not_exist(ID3D11Device *device, AssetsRecord *vs_rec, VertexShaderCache **out)
    {
        HRESULT hr = S_OK;
        if (vs_rec->gpu_extension != NULL)
        {
            *out = static_cast<VertexShaderCache *>(vs_rec->gpu_extension);
            goto _exit;
        }
        else
        {
            if (vs_rec && vs_rec->data)
            {
                VertexShaderCache *extension = new (heap_alloc(sizeof(VertexShaderCache))) VertexShaderCache();

                hr = device->CreateVertexShader(vs_rec->data->data, vs_rec->data->size, nullptr, &extension->shader);

                if (FAILED(hr))
                {
                    LOG_ERROR("Failed to create vertex shader, Code 0x%08lx", hr);
                    goto _exit;
                }

                _input_layout_desc_scratch.clear();
                for (size_t i = 0; i < vs_rec->info.info.as_shader.count; ++i)
                    _input_layout_desc_scratch.push_back(
                            create_input_element_desc(vs_rec->info.info.as_shader.data[i]));

                hr = device->CreateInputLayout(
                        _input_layout_desc_scratch.data(),
                        (UINT) _input_layout_desc_scratch.size(),
                        vs_rec->data->data,
                        vs_rec->data->size,
                        &extension->layout);

                if (FAILED(hr))
                {
                    LOG_ERROR("Failed to create input layout, Code 0x%08lx", hr);
                    goto _exit;
                }

                vs_rec->gpu_extension = (void *) extension;
                *out = extension;
            }
        }

_exit:
        return (hr);
    }

    HRESULT Dx11Adapter::load_ps_if_not_exist(ID3D11Device *device, AssetsRecord *ps_rec, PixelShaderCache **out)
    {
        HRESULT hr = S_OK;
        if (ps_rec->gpu_extension != NULL)
        {
            *out = ((PixelShaderCache *) ps_rec->gpu_extension);
        }
        else
        {
            PixelShaderCache *extension = new (heap_alloc(sizeof(PixelShaderCache))) PixelShaderCache();
            if (ps_rec && ps_rec->data)
            {
                hr = device->CreatePixelShader(ps_rec->data->data, ps_rec->data->size, nullptr, &extension->shader);

                if (FAILED(hr))
                {
                    LOG_ERROR("Failed to create pixel shader, Code 0x%08lx", hr);
                    goto _exit;
                }

                ps_rec->gpu_extension = (void *) extension;
                *out = extension;
            }
        }
_exit:
        return (hr);
    }

    HRESULT Dx11Adapter::load_sp_if_not_exist(ID3D11Device *device, AssetsRecord *sp_rec, SpriteCache **out)
    {
        HRESULT hr = S_OK;

        if (sp_rec == nullptr)
        {
            *out = nullptr;
            return S_OK;
        }

        if (sp_rec->gpu_extension != NULL)
        {
            *out = static_cast<SpriteCache *>(sp_rec->gpu_extension);
            return hr;
        }

        if (sp_rec->data != nullptr)
        {
            SpriteCache *extension = new (heap_alloc(sizeof(SpriteCache))) SpriteCache();

            dds::Image image{};
            const auto *raw_bytes = reinterpret_cast<const std::uint8_t *>(sp_rec->data->data);
            const std::size_t raw_size = sp_rec->data->size;

            if (dds::readImage(raw_bytes, raw_size, &image) == dds::ReadResult::Success && !image.mipmaps.empty())
            {
                D3D11_TEXTURE2D_DESC tex_desc{};
                tex_desc.Width = image.width;
                tex_desc.Height = image.height;
                tex_desc.MipLevels = image.numMips;
                tex_desc.ArraySize = image.arraySize;
                tex_desc.Format = image.format;
                tex_desc.SampleDesc.Count = 1;
                tex_desc.SampleDesc.Quality = 0;
                tex_desc.Usage = D3D11_USAGE_DEFAULT;
                tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
                tex_desc.CPUAccessFlags = 0;
                tex_desc.MiscFlags = 0;

                std::vector<D3D11_SUBRESOURCE_DATA> subresource;
                subresource.reserve(image.mipmaps.size());

                UINT mipWidth = image.width;
                for (const auto &mip: image.mipmaps)
                {
                    D3D11_SUBRESOURCE_DATA init{};
                    init.pSysMem = mip.data();
                    init.SysMemPitch = get_mip_pitch(mipWidth, image.format);
                    init.SysMemSlicePitch = 0;
                    subresource.push_back(init);

                    mipWidth = max(1u, mipWidth >> 1);
                }

                hr = device->CreateTexture2D(&tex_desc, subresource.data(), &extension->texture);
                if (FAILED(hr))
                {
                    LOG_ERROR("Failed to create sprite texture, Code 0x%08lx", hr);
                    goto _exit;
                }

                D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc{};
                srv_desc.Format = image.format;
                srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                srv_desc.Texture2D.MostDetailedMip = 0;
                srv_desc.Texture2D.MipLevels = image.numMips;

                hr = device->CreateShaderResourceView(extension->texture.Get(), &srv_desc, &extension->texture_view);
                if (FAILED(hr))
                {
                    LOG_ERROR("Failed to create sprite SRV, Code 0x%08lx", hr);
                    goto _exit;
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

                hr = device->CreateSamplerState(&sampler_desc, &extension->sampler_state);
                if (FAILED(hr))
                {
                    LOG_ERROR("Failed to create sprite sampler, Code 0x%08lx", hr);
                    goto _exit;
                }

                sp_rec->gpu_extension = (void *) extension;
                *out = extension;
            }
            else
            {
                LOG_ERROR("Failed to parse DDS image or image has no mipmaps.");
                hr = E_FAIL;
            }
        }

_exit:
        return hr;
    }

    void Dx11Adapter::convert(Windows::DeviceResources &resources, const std::vector<RenderItem> &items)
    {
        ID3D11Device *device = resources.get_device();
        size_t v_idx = 0;
        size_t i_idx = 0;
        D3D11_MAPPED_SUBRESOURCE v_map, i_map;

        uint8_t *v_ptr = nullptr;
        uint32_t *i_ptr = nullptr;

        _items.clear();
        _items.reserve(items.size());

        if (FAILED(_environment.context->Map(_global_vb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &v_map)))
            return;
        if (FAILED(_environment.context->Map(_global_ib.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &i_map)))
        {
            _environment.context->Unmap(_global_vb.Get(), 0);
            return;
        }

        v_ptr = static_cast<uint8_t *>(v_map.pData);
        i_ptr = static_cast<uint32_t *>(i_map.pData);

        for (auto &item: items)
        {
            RenderObject render_object{};
            AssetsRecord *vs_rec = item.vs;
            AssetsRecord *ps_rec = item.ps;
            AssetsRecord *sp_rec = item.sp;

            VertexShaderCache *vs_cache = nullptr;
            PixelShaderCache *ps_cache = nullptr;
            SpriteCache *sp_cache = nullptr;

            auto const points = item.points;
            auto base = i_idx;
            const uint8_t *extra_data = item.param_data.data();

            this->load_vs_if_not_exist(device, vs_rec, &vs_cache);
            this->load_ps_if_not_exist(device, ps_rec, &ps_cache);
            this->load_sp_if_not_exist(device, sp_rec, &sp_cache);

            render_object.vertex_base = 0;
            render_object.byte_offset = (UINT) v_idx;
            render_object.offset = (UINT) base;
            render_object.count.index_count = (UINT) item.indices.size();

            render_object.render_id.sort_key = item.sort_key.as_key;
            render_object.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            render_object.vertex_shader = vs_cache->shader;
            render_object.input_layout = vs_cache->layout;
            render_object.pixel_shader = ps_cache->shader;
            render_object.stride = (UINT) (sizeof(Math::Point) + item.params_size);

            render_object.texture_view = sp_cache ? sp_cache->texture_view : nullptr;
            render_object.sampler_state = sp_cache ? sp_cache->sampler_state : nullptr;


            for (size_t i = 0; i < points.size(); ++i)
            {
                memcpy(v_ptr + v_idx, &points[i], sizeof(Math::Point));
                v_idx += sizeof(Math::Point);

                if (extra_data && item.params_size > 0)
                {
                    memcpy(v_ptr + v_idx, extra_data + (i * item.params_size), item.params_size);
                    v_idx += item.params_size;
                }
            }

            for (size_t i = 0; i < item.indices.size(); ++i)
            {
                i_ptr[i_idx++] = item.indices[i];
            }

            _items.push_back(std::move(render_object));
        }

        _environment.context->Unmap(_global_vb.Get(), 0);
        _environment.context->Unmap(_global_ib.Get(), 0);
    }

    void Dx11Adapter::render_all_items(Windows::DeviceResources &device)
    {
        auto ctx = device.get_context();
        auto rtv = device.get_rtv();
        auto dsv = device.get_depth_stencil();

        ctx->OMSetRenderTargets(1, &rtv, dsv);

        float clearColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
        _environment.context->ClearRenderTargetView(rtv, clearColor);
        _environment.context->OMSetRenderTargets(1, &rtv, dsv);

        if (dsv)
        {
            _environment.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        }

        ctx->IASetIndexBuffer(_global_ib.Get(), DXGI_FORMAT_R32_UINT, 0);

        auto &env = _environment;
        for (const auto &item: _items)
        {

            if (env.vertex_buffer != _global_vb.Get() || env.stride != item.stride ||
                env.byte_offset != item.byte_offset)
            {
                env.vertex_buffer = _global_vb.Get();
                env.stride = item.stride;
                env.byte_offset = item.byte_offset;
                env.context->IASetVertexBuffers(0, 1, &env.vertex_buffer, &env.stride, &env.byte_offset);
            }
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

        _global_vb.Reset();
        _global_ib.Reset();
    }
} // namespace System::Render

extern "C"
{
    void heap_free(void *ptr);

    void vertex_shader_release(void **shader_handler)
    {
        auto *cache = static_cast<VertexShaderCache *>(*shader_handler);
        if (!cache)
            return;

        cache->~VertexShaderCache();
        heap_free(cache);
        *shader_handler = NULL;
    }

    void pixel_shader_release(void **shader_handler)
    {
        auto *cache = static_cast<PixelShaderCache *>(*shader_handler);
        if (!cache)
            return;

        cache->~PixelShaderCache();
        heap_free(cache);
        *shader_handler = NULL;
    }

    void sprite_resource_release(void **sprite_resource)
    {
        auto *cache = static_cast<SpriteCache *>(*sprite_resource);
        if (!cache)
            return;

        cache->~SpriteCache();
        heap_free(cache);
        *sprite_resource = NULL;
    }
}
