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

        Text() : font_name(nullptr), font_size(0)
        {}
        explicit Text(std::string value) : text(std::move(value)), font_name(nullptr), font_size(0)
        {}
        explicit Text(std::string value, char *font_name, const uint32_t font_size) :
            text(std::move(value)), font_name(font_name), font_size(font_size)
        {}
    };
} // namespace Game::Render
