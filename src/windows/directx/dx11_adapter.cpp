#include "dx11_adapter.h"
#include <d3d11.h>
#include "utils/input_attribute_description.h"

#include "../../assets_manager/assets_manager.h"
#include "device_resources.hpp"
#include "utils/print_debug.h"

namespace System::Render
{
    typedef void *DX11AdapterHandler;
    extern "C" DX11AdapterHandler get_dx11_adapter();

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
    }

    Dx11Adapter &Dx11Adapter::instance()
    {
        auto *instance = static_cast<Dx11Adapter *>(get_dx11_adapter());
        if (instance == nullptr)
        {
            LOG_ERROR("Dx11Adapter used before initialization or after cleanup");
            std::abort();
        }
        return (*instance);
    }

    void
    Dx11Adapter::convert([[maybe_unused]] Windows::DeviceResources &resources, const std::vector<CompositorItem> &items)
    {
        auto device = resources.get_device();

        for (auto &[kind, common, special]: items)
        {
            RenderObject render_object{};
            HRESULT hr = S_OK;

            const auto vs = get_assets_record(common.vert_shader);
            const auto vs_input_attribute_desc = vs.info.info.as_shader.data;
            const auto vs_input_attribute_count = vs.info.info.as_shader.count;

            ID3D11VertexShader *vertex_shader = nullptr;
            ID3D11PixelShader *pixel_shader = nullptr;
            ID3D11InputLayout *vert_input_layout = nullptr;
            std::vector<D3D11_INPUT_ELEMENT_DESC> vert_input_element_desc{};


            hr = device->CreateVertexShader(
                vs.data->data,
                vs.data->size,
                nullptr,
                &vertex_shader
            );

            if (FAILED(hr))
            {
                LOG_ERROR("Failed to create vertex shader, Code 0x%08lx", hr);
                continue;
            }

            render_object.vertex_shader = vertex_shader;



            for (size_t i = 0; i < vs_input_attribute_count; ++i)
            {
                vert_input_element_desc.push_back(create_input_element_desc(vs_input_attribute_desc[i]));
            }

            hr = device->CreateInputLayout(
                    vert_input_element_desc.data(),
                    static_cast<UINT>(vert_input_element_desc.size()),
                    vs.data->data,
                    vs.data->size,
                    &vert_input_layout);

            if (FAILED(hr))
            {
                LOG_ERROR("Failed to create input layout, Code 0x%08lx", hr);
                continue;
            }
            render_object.input_layout = vert_input_layout;

            const auto ps = get_assets_record(common.pixel_shader);

            hr = device->CreatePixelShader(
                ps.data->data,
                ps.data->size,
                nullptr,
                &pixel_shader
            );
            if (FAILED(hr))
            {
                LOG_ERROR("Failed to create pixel shader, Code 0x%08lx", hr);
                continue;
            }
            render_object.pixel_shader = pixel_shader;

            switch (kind)
            {
            case DrawKind::KIND_TRIANGLE: {
                render_object.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                const auto &[points] = std::get<TriangleDrawDesc>(special);

                D3D11_BUFFER_DESC vb_desc{};
                vb_desc.Usage = D3D11_USAGE_DEFAULT;
                vb_desc.ByteWidth = static_cast<UINT>(sizeof(points));
                vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                vb_desc.CPUAccessFlags = 0;
                vb_desc.MiscFlags = 0;
                vb_desc.StructureByteStride = sizeof(Math::Point);

                D3D11_SUBRESOURCE_DATA vb_data{};
                vb_data.pSysMem = points;

                hr = device->CreateBuffer(&vb_desc, &vb_data, &render_object.vertex_buffer);
                if (FAILED(hr))
                {
                    LOG_ERROR("Failed to create vertex buffer, Code 0x%08lx", hr);
                    continue;
                }

                render_object.stride = sizeof(Math::Point);
                render_object.offset = 0;
                render_object.count.vertex_count = 3;
                break;
            }
            default:
                break;
            }

            instance()._items.push_back(render_object);
        }

        const auto compositor_items_count = items.size();
        const auto render_objects_count = instance()._items.size();
        LOG_INFO("Converted %d composed items into %d render objects", compositor_items_count, render_objects_count);
    }

    void Dx11Adapter::render_all_items()
    {
        const auto &items = instance()._items;
        auto env = instance()._environment;
        for (auto &item: items)
        {
            render(&env, &item);
        }
    }
} // namespace System::Render
