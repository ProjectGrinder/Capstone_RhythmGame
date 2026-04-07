#pragma once

#include <array>
#include <cmath>
#include <maths/vector2.h>

#include "game/components/render/sprite.h"
#include "system/intent_storage.h"

// helpers
namespace Math
{
    static inline Point rotate_fast(const Point &p, float cos_val, float sin_val)
    {
        return Point{
                p.x * cos_val - p.y * sin_val,
                p.x * sin_val + p.y * cos_val,
                p.z,
        };
    }

    static inline Point transform_pipe_fast(
            const Point &local,
            float obj_cos,
            float obj_sin,
            const Point &pivot,
            const System::Render::Camera &cam,
            float cam_cos,
            float cam_sin,
            float invHalfW,
            float invHalfH)
    {
        // 1. Rotate Local
        Point p = rotate_fast(local, obj_cos, obj_sin);
        // 2. World Space
        p.x += pivot.x;
        p.y += pivot.y;
        // 3. View Space (Relative to Camera)
        p.x -= cam.offset.x;
        p.y -= cam.offset.y;
        // 4. Rotate World by Inverse Camera
        p = rotate_fast(p, cam_cos, cam_sin);
        // 5. Zoom and NDC
        p.x *= (cam.zoom * invHalfW);
        p.y *= (cam.zoom * invHalfH);

        return p;
    }

    // rotates a point around the origin with angle radians in the Z clockwise direction
    static inline Point rotate(const Point &p, const float radians)
    {
        const float c = std::cos(radians);
        const float s = std::sin(radians);
        return (Point{p.x * c - p.y * s, p.x * s + p.y * c, p.z});
    }

    // transfers a local coordinate to world coordinates using a center-of-object coordinate
    static inline Point local_to_world(const Point &p, const Point &center)
    {
        return (Point{p.x + center.x, p.y + center.y, p.z});
    }

    // transfer coordinates from world space to camera view space
    static inline Point world_to_view(const Point &world, const System::Render::Camera &cam)
    {
        // Move world relative to camera position
        Point p{
                world.x - cam.offset.x,
                world.y - cam.offset.y,
                world.z,
        };

        // Apply inverse camera rotation (i.e., rotate the world opposite the camera)
        p = rotate(p, -cam.rotation);

        // Zoom in means things appear bigger -> multiply coordinates by zoom in view space
        p.x *= cam.zoom;
        p.y *= cam.zoom;

        return (p);
    }

    static inline Point view_to_ndc(const Point &view, const System::Render::Camera &cam)
    {
        const float halfW = cam.scaleX * 0.5f;
        const float halfH = cam.scaleY * 0.5f;

        const float invHalfW = (halfW != 0.0f) ? (1.0f / halfW) : 0.0f;
        const float invHalfH = (halfH != 0.0f) ? (1.0f / halfH) : 0.0f;

        return (Point{
                view.x * invHalfW,
                view.y * invHalfH,
                view.z,
        });
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
        const Point p0 = Point{u0, v0, 0};
        const Point p1 = Point{u1, v0, 0};
        const Point p2 = Point{u0, v1, 0};
        const Point p3 = Point{u1, v1, 0};

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

    const uint32_t PRESIZE = 1000; // Radix 8 passes
    const uint32_t PASSES = 8;     // Radix 8 passes

    extern "C"
    {
        typedef uint32_t assets_id;
    }

    struct ComposedTextDesc
    {
        std::string_view text;
        const AssetsRecord *font;
    };

    struct ComposedDrawCommon
    {
        const AssetsRecord *vs;
        const AssetsRecord *ps;
        const AssetsRecord *sp;
        Color color;
        uint64_t key;
    };

    struct RenderItem
    {
        size_t params_size;
        std::vector<uint8_t> param_data;
        std::vector<Math::Point> points;
        std::vector<uint32_t> indices;

        union Key
        {
            struct DetailKey
            {
                uint16_t sp_id;
                uint16_t ps_id;
                uint16_t vs_id;
                uint8_t padding;
                uint8_t layer;
            } as_data;

            uint64_t as_key;
        } sort_key;

        AssetsRecord *vs;
        AssetsRecord *ps;
        AssetsRecord *sp;
    };

    struct SortEntry
    {
        uint64_t key;
        uint32_t intent_index;
    };

    class Compositor
    {
        std::vector<RenderItem> _items;

        std::vector<SortEntry> _sort_entries;
        std::vector<SortEntry> _sort_temp;

        std::array<size_t, 256> _counts;
        std::array<size_t, 256> _offsets;

        size_t _reserve_size = 0;
        size_t _total_count = 0;

    public:
        Compositor();

        static Compositor &instance();
        static void compose(const std::vector<DrawIntent> &intents, const Camera &camera);

        static std::vector<SortEntry> &get_sort_entries()
        {
            return instance()._sort_entries;
        }

        static const std::vector<RenderItem> &get_items()
        {
            return instance()._items;
        }

        void radix_sort();
        void resize(size_t size);
        size_t get_capacity()
        {
            return _reserve_size;
        }

        void add_sort_entry(SortEntry &&entry)
        {
            _sort_entries.push_back(entry);
        }

        void reset()
        {
            _sort_entries.clear();
            _sort_temp.clear();
            _items.clear();
        }

        size_t get_total_count()
        {
            return (_total_count);
        }

        SortEntry *get_sort_data()
        {
            return (_sort_entries.data());
        }
    };
} // namespace System::Render
