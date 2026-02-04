#pragma once

// FIXME: May separate thread
namespace Game::Physics
{
    struct Velocity
    {
        float vx, vy;
        Velocity() : vx(0), vy(0)
        {}
        explicit Velocity(const float vx, const float vy) : vx(vx), vy(vy)
        {}
    };
} // namespace Game::Physics
