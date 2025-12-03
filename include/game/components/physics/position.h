#pragma once

// FIXME: May separate thread
namespace Game::Physics
{
    struct Position
    {
        float x,y;
        Position() : x(0), y(0)
        {}
        explicit Position(const float x, const float y) : x(x), y(y)
        {}
    };
}