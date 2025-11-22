#pragma once

namespace Game::Render
{
    struct Material
    {
        int time_offset;
        float color[4];
        float uv[2];
        Material() : time_offset(0), color{}, uv{}
        {}
    };
}