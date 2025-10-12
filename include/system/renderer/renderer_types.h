#pragma once
#include <concepts>
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
        uint16_t priority = 0;

        explicit RenderItem(Render2D &render_2d);
        explicit RenderItem(Render3D &render_3d);

        bool operator < (const RenderItem &other) const;
        bool operator > (const RenderItem &other) const;
    };

    template<size_t BufferSize>
    class VertexGeneratorQueue;

    class VertexGenerator;

    template<typename T>
    concept RendererConcept = requires(T renderer, RenderItem &item)
    {
        { renderer.render(item) } -> std::same_as<void>;
    };
}