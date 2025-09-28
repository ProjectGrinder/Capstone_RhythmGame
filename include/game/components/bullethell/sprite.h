#pragma once

namespace Game::BulletHell
{
    struct Sprite
    {
        unsigned int id;
        unsigned int shader_id;
        int render_prior;
        float scale;
    };
}