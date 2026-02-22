#pragma once

#include <string>
#include <utility>

// TODO: Marked by Nack ("most painful shit to implement")
namespace Game::Render
{
    struct Text
    {
        std::string text; // text

        char *font_name;    // font name in asset
        uint32_t font_size; // font size

        // World-space position
        float x;
        float y;
        // Anchor
        float anchor_x;
        float anchor_y;

        Text() : font_name(nullptr), font_size(0), x(0.0f), y(0.0f), anchor_x(0.5f), anchor_y(0.5f)
        {}
        explicit Text(std::string value, char *font_name, const uint32_t font_size, const float x, const float y, const float anchor_x, const float anchor_y) :
            text(std::move(value)), font_name(font_name), font_size(font_size), x(x), y(y), anchor_x(anchor_x), anchor_y(anchor_y)
        {}
    };
} // namespace Game::Render
