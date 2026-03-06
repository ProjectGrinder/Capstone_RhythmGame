#include "dx11_adapter.h"
#include "dxgiformat.h"
#include "utils/input_attribute_description.h"
#include "utils/print_debug.h"
#include "../../assets_manager/assets_manager.h"

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

    void Dx11Adapter::convert([[maybe_unused]] const std::vector<CompositorItem> &items)
    {
        /*
        for (auto &[kind, common, special]: items)
        {
            auto vs = get_assets_record(common.vert_shader);
            auto vs_input_attribute_desc = vs.info.info.as_shader.data;

            auto ps = get_assets_record(common.pixel_shader);
            auto ps_input_attribute_desc = ps.info.info.as_shader.data;

        }
        */
    }
} // namespace System::Render
