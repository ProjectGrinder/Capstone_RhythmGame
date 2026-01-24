#pragma once

#include <cstdint>

namespace Game::Render
{
    struct Sprite
    {
        // Asset / resource identity
        unsigned int id;          // sprite/region id in your asset system (kept for compatibility)
        unsigned int texture_id;  // texture (or atlas) handle/id needed by the renderer
        unsigned int shader_id;

        // Sorting
        int render_layer;   // coarse ordering (background -> foreground)
        int render_prior;   // fine ordering within a layer

        // Sizing / mapping
        float size_x;       // quad width (world units)
        float size_y;       // quad height (world units)

        // UV rectangle (normalized 0..1) for atlas regions
        float uv0_x;
        float uv0_y;
        float uv1_x;
        float uv1_y;

        // Pivot/origin (0..1): (0,0)=top-left, (0.5,0.5)=center, (1,1)=bottom-right
        float pivot_x;
        float pivot_y;

        // Appearance
        float scale;        // additional uniform scale multiplier (kept)
        float tint_r;
        float tint_g;
        float tint_b;
        float tint_a;

        // Convenience flags
        bool visible;
        bool flip_x;
        bool flip_y;

        Sprite()
            : id(0),
              texture_id(0),
              shader_id(0),
              render_layer(0),
              render_prior(0),
              size_x(1.0f),
              size_y(1.0f),
              uv0_x(0.0f), uv0_y(0.0f),
              uv1_x(1.0f), uv1_y(1.0f),
              pivot_x(0.5f), pivot_y(0.5f),
              scale(1.0f),
              tint_r(1.0f), tint_g(1.0f), tint_b(1.0f), tint_a(1.0f),
              visible(true),
              flip_x(false),
              flip_y(false)
        {}

        explicit Sprite(const unsigned int id,
                        const unsigned int texture_id,
                        const unsigned int shader_id,
                        const int render_layer = 0,
                        const int render_prior = 0,
                        const float size_x = 1.0f,
                        const float size_y = 1.0f)
            : id(id),
              texture_id(texture_id),
              shader_id(shader_id),
              render_layer(render_layer),
              render_prior(render_prior),
              size_x(size_x),
              size_y(size_y),
              uv0_x(0.0f), uv0_y(0.0f),
              uv1_x(1.0f), uv1_y(1.0f),
              pivot_x(0.5f), pivot_y(0.5f),
              scale(1.0f),
              tint_r(1.0f), tint_g(1.0f), tint_b(1.0f), tint_a(1.0f),
              visible(true),
              flip_x(false),
              flip_y(false)
        {}
    };
}