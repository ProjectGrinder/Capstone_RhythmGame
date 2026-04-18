#pragma once

// FIXME: May separate thread
namespace Game::Physics
{
    struct Rotation
    {
        float angleX, angleY, angleZ;
        bool attach_render = true;
        Rotation() : angleX(0), angleY(0), angleZ(0)
        {}
        explicit Rotation(const float angle, const bool attach_render = false) : angleX(0), angleY(0), angleZ(angle), attach_render(attach_render)
        {}
        explicit Rotation(const float angleX, const float angleY, const float angleZ, const bool attach_render = true) :
            angleX(angleX), angleY(angleY), angleZ(angleZ), attach_render(attach_render)
        {}
    };
} // namespace Game::Physics
