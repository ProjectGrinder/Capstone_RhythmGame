#pragma once

#include <string>
#include <utility>

namespace Game::Render
{
    struct Text
    {
        std::string text;
        explicit Text(std::string value) : text(std::move(value))
        {}

    };
}