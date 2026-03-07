#pragma once
#include <vector>

#include "device_resources.hpp"
#include "renderer_types.h"
#include "system/compositor.h"

namespace System::Render
{
    class Dx11Adapter
    {
        std::vector<RenderObject> _items{};
    public:
        static Dx11Adapter& instance();
        static void convert(Windows::DeviceResources &resources, const std::vector<CompositorItem> &items);
    };
}