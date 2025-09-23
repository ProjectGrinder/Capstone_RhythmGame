#pragma once
#include <queue>

namespace System::Renderer
{
    /*
     *  Using this as component in order to render you entity
     */
    struct Render2D{};

    struct Render3D{};

    struct RenderItem
    {
    public:
        RenderItem(Render2D render_2d);
        RenderItem(Render3D render_3d);
    };

    template<typename ResourceManager>
    std::priority_queue<RenderItem> create_render_items(ResourceManager &resource_manager);
}