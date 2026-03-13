#include "dx11_adapter.h"
#include <algorithm>
#include <d3d11.h>
#include "utils/input_attribute_description.h"

#include "../../assets_manager/assets_manager.h"
#include "device_resources.hpp"
#include "utils/print_debug.h"

namespace System::Render
{
    typedef void *DX11AdapterHandler;
    extern "C" DX11AdapterHandler get_dx11_adapter();
    const UINT MAX_VERTICES = 100000;
    const UINT MAX_INDICES = 300000;

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
        _input_layout_desc_scratch.reserve(16);

        _vs_fast_cache.assign(65536, {(uint32_t) -1, nullptr});
        _ps_fast_cache.assign(65536, {(uint32_t) -1, nullptr});
        _input_fast_cache.assign(65536, {(uint32_t) -1, nullptr});
    }

    void Dx11Adapter::convert(Windows::DeviceResources &resources, const std::vector<CompositorItem> &items)
    {
        _items.clear();
        _items.reserve(items.size());
        _v_staging.clear();
        _i_staging.clear();

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

            uint16_t vs_idx = ASSET_INDEX(common.vert_shader);

            if (common.vert_shader == (uint32_t) -1 || vs_idx >= _vs_fast_cache.size())
            {
                LOG_ERROR("Invalid Shader ID: %u", common.vert_shader);
                continue;
            }

            if (_vs_fast_cache[vs_idx].full_id == common.vert_shader)
            {
                render_object.vertex_shader = _vs_fast_cache[vs_idx].shader;
                render_object.input_layout = _input_fast_cache[vs_idx].layout;
            }
            else
            {
                const auto vs_rec = get_assets_record_ptr(common.vert_shader);
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

                    _vs_fast_cache[vs_idx] = {common.vert_shader, render_object.vertex_shader};
                    _input_fast_cache[vs_idx] = {common.vert_shader, render_object.input_layout};
                }
            }

            uint16_t ps_idx = ASSET_INDEX(common.pixel_shader);
            if (_ps_fast_cache[ps_idx].full_id == common.pixel_shader)
            {
                render_object.pixel_shader = _ps_fast_cache[ps_idx].shader;
            }
            else
            {
                const auto ps_rec = get_assets_record_ptr(common.pixel_shader);
                if (ps_rec && ps_rec->data)
                {
                    hr = device->CreatePixelShader(
                            ps_rec->data->data, ps_rec->data->size, nullptr, &render_object.pixel_shader);

                    if (FAILED(hr))
                    {
                        LOG_ERROR("Failed to create pixel shader, Code 0x%08lx", hr);
                        continue;
                    }

                    _ps_fast_cache[ps_idx] = {common.pixel_shader, render_object.pixel_shader};
                }
            }

            switch (kind)
            {
            case DrawKind::KIND_TRIANGLE: {
                //                render_object.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                //                const auto &[points] = std::get<TriangleDrawDesc>(special);
                //
                //                D3D11_BUFFER_DESC vb_desc = {};
                //                vb_desc.Usage = D3D11_USAGE_DEFAULT;
                //                vb_desc.ByteWidth = static_cast<UINT>(sizeof(points));
                //                vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                //                vb_desc.StructureByteStride = sizeof(Math::Point);
                //
                //                D3D11_SUBRESOURCE_DATA vb_data = {};
                //                vb_data.pSysMem = points;
                //
                //                hr = device->CreateBuffer(&vb_desc, &vb_data,
                //                render_object.vertex_buffer.GetAddressOf()); if (FAILED(hr))
                //                {
                //                    LOG_ERROR("Failed to create vertex buffer, Code 0x%08lx", hr);
                //                    continue;
                //                }
                //
                //                render_object.stride = sizeof(Math::Point);
                //                render_object.offset = 0;
                //                render_object.count.vertex_count = 3;
                //                break;
                auto const &[points] = std::get<TriangleDrawDesc>(special);
                render_object.vertex_base = (INT) _v_staging.size();
                render_object.offset = (UINT) _i_staging.size();
                render_object.count.index_count = 3;
                render_object.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

                _v_staging.push_back(points[0]);
                _v_staging.push_back(points[1]);
                _v_staging.push_back(points[2]);

                _i_staging.push_back(0);
                _i_staging.push_back(1);
                _i_staging.push_back(2);
                break;
            }
            case DrawKind::KIND_SPRITE: {
                auto const &[texture, src_rect, points, flipX, flipY] = std::get<ComposedSpriteDesc>(special);
                render_object.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
                render_object.vertex_base = (INT) _v_staging.size();
                render_object.offset = (UINT) _i_staging.size();
                render_object.count.index_count = 4;

                // attempt to put a triangle strip in?
                _v_staging.push_back(points[0]);
                _v_staging.push_back(points[1]);
                _v_staging.push_back(points[2]);
                _v_staging.push_back(points[3]);

                _i_staging.push_back(0);
                _i_staging.push_back(1);
                _i_staging.push_back(2);
                _i_staging.push_back(3);

                // TODO: help how do i use the rest
                // Note: texture probably needs the width and height from when load_sprite is called
                // where do i even get that stuff from actually
                // i'll ask chatgpt later
                break;
            }
            default:
                break;
            }

            _items.push_back(std::move(render_object));
        }

        D3D11_MAPPED_SUBRESOURCE mapped;
        if (SUCCEEDED(_environment.context->Map(_global_vb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
        {
            memcpy(mapped.pData, _v_staging.data(), _v_staging.size() * sizeof(Math::Point));
            _environment.context->Unmap(_global_vb.Get(), 0);
        }

        if (SUCCEEDED(_environment.context->Map(_global_ib.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
        {
            memcpy(mapped.pData, _i_staging.data(), _i_staging.size() * sizeof(UINT));
            _environment.context->Unmap(_global_ib.Get(), 0);
        }

        std::sort(
                _items.begin(),
                _items.end(),
                [](const RenderObject &a, const RenderObject &b) { return a.get_sort_key() < b.get_sort_key(); });

        LOG_INFO("Converted %ld items into %ld render objects (Sorted)", items.size(), _items.size());
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
    }
} // namespace System::Render
