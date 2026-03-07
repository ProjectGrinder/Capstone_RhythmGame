#include "dx11_adapter.h"
#include "utils/input_attribute_description.h"
#include <d3d11.h>

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

    void Dx11Adapter::convert([[maybe_unused]] Windows::DeviceResources &resources, const std::vector<CompositorItem> &items)
    {
        auto device = resources.get_device();

        for (auto &[kind, common, special]: items)
        {
            HRESULT hr = S_OK;
            const auto vs = get_assets_record(common.vert_shader);
            const auto vs_input_attribute_desc = vs.info.info.as_shader.data;
            const auto vs_input_attribute_count = vs.info.info.as_shader.count;
            std::vector<D3D11_INPUT_ELEMENT_DESC> vert_input_element_desc{};
            for (size_t i = 0; i < vs_input_attribute_count; ++i)
            {
                vert_input_element_desc.push_back(create_input_element_desc(vs_input_attribute_desc[i]));
            }
            ID3D11InputLayout *vert_input_layout = nullptr;
            hr = device->CreateInputLayout(
                vert_input_element_desc.data(),
                static_cast<UINT>(vert_input_element_desc.size()),
                vs.data->data,
                vs.data->size,
                &vert_input_layout
            );

            const auto ps = get_assets_record(common.pixel_shader);
        }
    }
} // namespace System::Render
