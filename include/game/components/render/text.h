#pragma once

#include <string>

// TODO: Marked by Nack ("most painful shit to implement")
namespace Game::Render
{
    struct Text
    {
        const AssetsRecord *font = nullptr; //font
        std::string text; // text
    };
} // namespace Game::Render
