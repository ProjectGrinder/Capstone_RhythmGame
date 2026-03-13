#pragma once

#include <array>
#include <cmath>
#include <maths/vector2.h>

#include "game/components/render/sprite.h"
#include "system/intent_storage.h"

// helpers
namespace Math
{
    // rotates a point around the origin with angle radians in the Z clockwise direction
    static inline Point rotate(const Point &p, const float radians)
    {
        const float c = std::cos(radians);
        const float s = std::sin(radians);
        return (
                Point{{p.pos[0] * c - p.pos[1] * s, p.pos[0] * s + p.pos[1] * c, p.pos[2]},
                      {p.color[0], p.color[1], p.color[2], p.color[3]}});
    }

    // transfers a local coordinate to world coordinates using a center-of-object coordinate
    static inline Point local_to_world(const Point &p, const Point &center)
    {
        return (
                Point{{p.pos[0] + center.pos[0], p.pos[1] + center.pos[1], p.pos[2]},
                      {p.color[0], p.color[1], p.color[2], p.color[3]}});
    }

    // transfer coordinates from world space to camera view space
    static inline Point world_to_view(const Point &world, const System::Render::Camera &cam)
    {
        // Move world relative to camera position
        Point p{{world.pos[0] - cam.offset.pos[0], world.pos[1] - cam.offset.pos[1], world.pos[2]},
                {world.color[0], world.color[1], world.color[2], world.color[3]}};

        // Apply inverse camera rotation (i.e., rotate the world opposite the camera)
        p = rotate(p, -cam.rotation);

        // Zoom in means things appear bigger -> multiply coordinates by zoom in view space
        p.pos[0] *= cam.zoom;
        p.pos[1] *= cam.zoom;

        return (p);
    }

    static inline Point view_to_ndc(const Point &view, const System::Render::Camera &cam)
    {
        const float halfW = cam.scaleX * 0.5f;
        const float halfH = cam.scaleY * 0.5f;

        const float invHalfW = (halfW != 0.0f) ? (1.0f / halfW) : 0.0f;
        const float invHalfH = (halfH != 0.0f) ? (1.0f / halfH) : 0.0f;

        return (
                Point{{view.pos[0] * invHalfW, view.pos[1] * invHalfH, view.pos[2]},
                      {view.color[0], view.color[1], view.color[2], view.color[3]}});
    }

    // Takes a local coordinate TriangleDrawDesc, add rotation in Z clockwise direction, then transform to NDC.
    static inline System::Render::TriangleDrawDesc local_to_ndc(
            const System::Render::TriangleDrawDesc &desc,
            const float rotation_z,
            const Point &pivot,
            const System::Render::Camera &cam)
    {
        return System::Render::TriangleDrawDesc{
                view_to_ndc(world_to_view(local_to_world(rotate(desc.points[0], rotation_z), pivot), cam), cam),
                view_to_ndc(world_to_view(local_to_world(rotate(desc.points[1], rotation_z), pivot), cam), cam),
                view_to_ndc(world_to_view(local_to_world(rotate(desc.points[2], rotation_z), pivot), cam), cam)};
    }

    static inline std::array<Math::Point, 4> project_rect_local_to_ndc(
        const System::Render::Rect &rect,
        const float rotation_z,
        const Point &pivot,
        const System::Render::Camera &cam)
    {
        const float u0 = rect.u0, u1 = rect.u1, v0 = rect.v0, v1 = rect.v1;
        const Point p0 = Point{{u0, v0, 0}, {1, 1, 1, 1}};
        const Point p1 = Point{{u1, v0, 0}, {1, 1, 1, 1}};
        const Point p2 = Point{{u0, v1, 0}, {1, 1, 1, 1}};
        const Point p3 = Point{{u1, v1, 0}, {1, 1, 1, 1}};

        return std::array<Math::Point, 4>{
            view_to_ndc(world_to_view(local_to_world(rotate(p0, rotation_z), pivot), cam), cam),
            view_to_ndc(world_to_view(local_to_world(rotate(p1, rotation_z), pivot), cam), cam),
            view_to_ndc(world_to_view(local_to_world(rotate(p2, rotation_z), pivot), cam), cam),
            view_to_ndc(world_to_view(local_to_world(rotate(p3, rotation_z), pivot), cam), cam),
        };
    }

} // namespace Math

namespace System::Render
{
    extern "C"
    {
        typedef uint32_t assets_id;
    }

    struct ComposedDrawCommon
    {
        // Pipeline selection (names)
        assets_id vert_shader = static_cast<assets_id>(-1);
        assets_id pixel_shader = static_cast<assets_id>(-1);

        /* render_prior: coarse render priority */
        uint32_t render_prior = 0;

        // Per-draw parameters
        Color color{};

        // Sorting (kept here so both sprite/text can be sorted uniformly)
        uint32_t layer = 0;
        uint32_t order = 0;
    };

    struct ComposedSpriteDesc
    {
        assets_id texture = static_cast<assets_id>(-1);
        Rect src_rect{};
        Math::Point dst_rect[4];

        bool flipX = false;
        bool flipY = false;
    };

    struct ComposedTextDesc
    {
        std::string_view text{};
        assets_id font = static_cast<assets_id>(-1);
        uint32_t font_size = 0;
        Math::Vector2<float> position{};
        Math::Vector2<float> anchor{};
    };

    struct CompositorItem
    {
        DrawKind kind = DrawKind::KIND_UNKNOWN;
        ComposedDrawCommon common;
        std::variant<ComposedSpriteDesc, ComposedTextDesc, TriangleDrawDesc> special;
    };

    class Compositor
    {
        std::vector<CompositorItem> _items;

    public:
        static void compose(const std::vector<std::optional<DrawIntent>> &intents, const Camera &camera);
        static std::vector<CompositorItem> &items()
        {
            return instance()._items;
        };
        static Compositor &instance();
    };
} // namespace System::Render
