#pragma once
#include <vector>
#include "renderer_types.h"
#include "system/compositor.h"

namespace System::Render
{
    class Dx11Adapter
    {
        std::vector<RenderObject> _items{};
    public:
        static Dx11Adapter& instance();
        static void convert(const std::vector<CompositorItem> &items);
    };
}