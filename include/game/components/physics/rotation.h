#pragma once

// FIXME: May separate thread
namespace Game::Physics
{
    struct Rotation
    {
        float angle;
        Rotation() : angle(0)
        {}
        explicit Rotation(const float angle) : angle(angle)
        {}
    };
}
