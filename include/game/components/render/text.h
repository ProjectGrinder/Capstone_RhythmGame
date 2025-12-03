#pragma once

#include <string>
#include <utility>

// TODO: Marked by Nack ("most painful shit to implement")
namespace Game::Render
{
    struct Text
    {
        std::string text;
        explicit Text(std::string value) : text(std::move(value))
        {}

    };
}