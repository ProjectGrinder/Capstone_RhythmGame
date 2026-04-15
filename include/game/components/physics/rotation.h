#pragma once

// FIXME: May separate thread
namespace Game::Physics
{
    struct Rotation
    {
        float angleX, angleY, angleZ;
        bool no_render = false;
        Rotation() : angleX(0), angleY(0), angleZ(0)
        {}
        explicit Rotation(const float angle, const bool no_render = false) : angleX(0), angleY(0), angleZ(angle), no_render(no_render)
        {}
        explicit Rotation(const float angleX, const float angleY, const float angleZ, const bool no_render = false) :
            angleX(angleX), angleY(angleY), angleZ(angleZ), no_render(no_render)
        {}
    };
} // namespace Game::Physics
