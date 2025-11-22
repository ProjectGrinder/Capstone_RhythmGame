#pragma once

namespace Game::Render
{
    struct Shader
    {
        unsigned int id;
        explicit Shader(const unsigned int id) : id(id)
        {}
    };
};