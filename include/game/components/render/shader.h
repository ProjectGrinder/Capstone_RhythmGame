#pragma once
// Not implemented for render (I don't think this should be here)
namespace Game::Render
{
    struct Shader
    {
        unsigned int id;
        explicit Shader(const unsigned int id) : id(id)
        {}
    };
};