#pragma once

#include <string>
#include <utility>

namespace Game::Render
{
    struct Text
    {
        std::string text;
        explicit Text::Text(std::string text) : text(std::move(text))
        {}

    };
}