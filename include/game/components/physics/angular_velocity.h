#pragma once

// FIXME: May separate thread
namespace Game::Physics
{
    struct AngularVelocity
    {
        float v;
        AngularVelocity() : v(0)
        {}
        explicit AngularVelocity(const float v) : v(v)
        {}
    };
}