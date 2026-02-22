#pragma once

#include <cmath>
#include <maths/vector2.h>
#include "system/intent_storage.h"
#include <array>

// helpers
namespace Math
{

    static inline Vector2<float> rotate(const Vector2<float> &p, const float radians)
    {
        const float c = std::cos(radians);
        const float s = std::sin(radians);
        return (Vector2{p.x * c - p.y * s, p.x * s + p.y * c});
    }

    // World space -> camera view space
    static inline Vector2<float> world_to_view(const Vector2<float> &world, const System::Render::Camera &cam)
    {
        // Move world relative to camera position
        Vector2 p{world.x - cam.offsetX, world.y - cam.offsetY};

        // Apply inverse camera rotation (i.e., rotate the world opposite the camera)
        p = rotate(p, -cam.rotation);

        // Zoom in means things appear bigger => multiply coordinates by zoom in view space
        p.x *= cam.zoom;
        p.y *= cam.zoom;

        return (p);
    }

    // Camera view space -> Normalized device coordinates
    static inline Vector2<float> view_to_ndc(const Vector2<float> &view, const System::Render::Camera &cam)
    {
        const float halfW = cam.scaleX * 0.5f;
        const float halfH = cam.scaleY * 0.5f;

        const float invHalfW = (halfW != 0.0f) ? (1.0f / halfW) : 0.0f;
        const float invHalfH = (halfH != 0.0f) ? (1.0f / halfH) : 0.0f;

        // Map view-space to NDC. Flip Y so +Y is up in NDC.
        return (Vector2{view.x * invHalfW, -view.y * invHalfH});
    }

    // Takes a world-space axis-aligned rect and returns its 4 corners in NDC:
    // order: (x0,y0), (x1,y0), (x1,y1), (x0,y1) in the rect's local orientation
    static inline std::array<Vector2<float>, 4>
    project_rect_world_to_ndc(const System::Render::Rect &dstWorld, const System::Render::Camera &cam)
    {
        const float x0 = dstWorld.u0;
        const float y0 = dstWorld.v0;
        const float x1 = dstWorld.u1;
        const float y1 = dstWorld.v1;

        const Vector2 w0{x0, y0};
        const Vector2 w1{x1, y0};
        const Vector2 w2{x1, y1};
        const Vector2 w3{x0, y1};

        const Vector2 v0 = world_to_view(w0, cam);
        const Vector2 v1 = world_to_view(w1, cam);
        const Vector2 v2 = world_to_view(w2, cam);
        const Vector2 v3 = world_to_view(w3, cam);

        return (std::array{view_to_ndc(v0, cam), view_to_ndc(v1, cam), view_to_ndc(v2, cam), view_to_ndc(v3, cam)});
    }

    static inline bool ndc_in_camera(const std::array<Vector2<float>, 4> &coords)
    {
        for (const auto& coord : coords)
        {
            if (coord.x < -1.0f || coord.x > 1.0f || coord.y < -1.0f || coord.y > 1.0f)
            {
                return (false);
            }
        }
        return (true);
    }

    // TextDrawDesc -> NDC (projects the text's anchor point position).
    // Note: Without font metrics we can't compute a full NDC quad here; this gives you the
    //       projected position you can feed into your text renderer / batching logic.
    static inline Vector2<float>
    project_text_anchor_world_to_ndc(const System::Render::TextDrawDesc &text, const System::Render::Camera &cam)
    {
        const Vector2 world{text.x, text.y};
        const Vector2 view = world_to_view(world, cam);
        return (view_to_ndc(view, cam));
    }
} // namespace Math

namespace System::Render
{
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

        // Rotation (what do you mean you still haven't rotated?)
        // TODO: figure out if rotation happens here
        float rotation_z = 0.0f;
    };

    struct ComposedSpriteDesc
    {
        assets_id texture = static_cast<assets_id>(-1);
        Rect src_rect{};
        std::array<Math::Vector2<float>, 4> dst_rect{};

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
        std::variant<ComposedSpriteDesc, ComposedTextDesc> special;
    };

    class Compositor
    {
        std::vector<CompositorItem> _items;
    public:
        static void compose(const std::vector<std::optional<DrawIntent>> &intents, const Camera &camera);
        static std::vector<CompositorItem>& items()
        {
            return instance()._items;
        };
        static Compositor& instance();
    };
} // namespace System::Render
