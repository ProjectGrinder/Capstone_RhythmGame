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
    std::priority_queue<RenderItem> create_render_items(ResourceManager &resource_manager)
    {
        std::priority_queue<RenderItem> render_items;

        /* 2D Render */
        if (resource_manager.template has_resource_type<Render2D>())
        {
            for (auto &query = resource_manager.template query<Render2D>(); auto &item : query)
            {
                auto &render2d_comp = query.template get<Render2D>();
                /* Create RenderItem (i.e. snapshot) */
                render_items.push(RenderItem(render2d_comp));
            }
        }

        /* 3D Render */
        if (resource_manager.template has_resource_type<Render3D>())
        {
            for (auto &query = resource_manager.template query<Render3D>(); auto &item : query)
            {
                auto &render3d_comp = query.template get<Render3D>();
                /* Create RenderItem (i.e. snapshot) */
                render_items.push(RenderItem(render3d_comp));
            }
        }

        return (render_items);
    }

}