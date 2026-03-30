#pragma once

#include <string>

// TODO: Marked by Nack ("most painful shit to implement")
namespace Game::Render
{
    struct Text
    {
        const AssetsRecord *font = nullptr; //font
        std::string text; // text
        uint32_t layer = 0;     // Layer to render
        uint32_t order = 0;     // Order of render in layer
    };
} // namespace Game::Render
