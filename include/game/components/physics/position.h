#pragma once

// FIXME: May separate thread
namespace Game::Physics
{
    struct Position
    {
        float x,y,z;
        Position() : x(0), y(0),z(0)
        {}
        explicit Position(const float x, const float y) : x(x), y(y),z(0)
        {}
        explicit Position(const float x, const float y,const float z) : x(x), y(y),z(z)
        {}
    };
}