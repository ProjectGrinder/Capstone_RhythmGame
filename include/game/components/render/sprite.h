#pragma once

namespace Game::Render
{
    struct Sprite
    {
        unsigned int id;
        unsigned int shader_id;
        int render_prior;
        float scale;
        Sprite() : id(0), shader_id(0), render_prior(0), scale(1.0f)
        {}
        explicit Sprite(const unsigned int id, const unsigned int shader_id,const int render_prior = 0,const float scale = 1.0f) :
        id(id), shader_id(shader_id), render_prior(render_prior), scale(scale)
        {}
    };
}