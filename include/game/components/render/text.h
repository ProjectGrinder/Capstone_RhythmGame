#pragma once

#include <string>
#include <utility>

namespace Game::Render
{
    struct Text
    {
        // Content
        std::string text;

        // Asset / pipeline
        unsigned int font_id;     // font/atlas handle/id
        unsigned int shader_id;   // shader used to render glyphs

        // Sorting
        int render_layer;
        int render_prior;

        // Layout (high-level)
        float size_x;             // layout box width (world units)
        float size_y;             // layout box height (world units)
        float pivot_x;            // 0..1
        float pivot_y;            // 0..1

        // Appearance
        float scale;              // uniform scale multiplier for glyph size
        float color_r;
        float color_g;
        float color_b;
        float color_a;

        bool visible;

        /*
         * Possibly needed later, but not for current minimal case
         * enum alignment (top/center/bottom, left/right/middle)
         * bool wrap or int max_width (text wrapping)
         */

        explicit Text(std::string value)
            : text(std::move(value)),
              font_id(0),
              shader_id(0),
              render_layer(0),
              render_prior(0),
              size_x(1.0f),
              size_y(1.0f),
              pivot_x(0.0f),
              pivot_y(0.0f),
              scale(1.0f),
              color_r(1.0f), color_g(1.0f), color_b(1.0f), color_a(1.0f),
              visible(true)
        {}

        Text(std::string value,
             const unsigned int font_id,
             const unsigned int shader_id,
             const float size_x = 1.0f,
             const float size_y = 1.0f,
             const int render_layer = 0,
             const int render_prior = 0)
            : text(std::move(value)),
              font_id(font_id),
              shader_id(shader_id),
              render_layer(render_layer),
              render_prior(render_prior),
              size_x(size_x),
              size_y(size_y),
              pivot_x(0.0f),
              pivot_y(0.0f),
              scale(1.0f),
              color_r(1.0f), color_g(1.0f), color_b(1.0f), color_a(1.0f),
              visible(true)
        {}
    };
}