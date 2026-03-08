#pragma once
#include <vector>

#include "device_resources.hpp"
#include "renderer.h"
#include "renderer_types.h"
#include "system/compositor.h"

namespace System::Render
{
    class Dx11Adapter
    {
        std::vector<RenderObject> _items{};
        RenderingEnvironment _environment{};

    public:
        explicit Dx11Adapter(Windows::DeviceResources &resources);
        static Dx11Adapter &instance();
        static void convert(Windows::DeviceResources &resources, const std::vector<CompositorItem> &items);
        static void render_all_items();
    };
} // namespace System::Render

