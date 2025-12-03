#pragma once

// TODO: Later map into real render component to optimize for rendering
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