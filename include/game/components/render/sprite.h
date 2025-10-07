#pragma once

namespace Game::Render
{
    struct Sprite
    {
        unsigned int id;
        unsigned int shader_id;
        int render_prior;
        float scale;
    };
}