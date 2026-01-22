#pragma once

// TODO: Later map into real render component to optimize for rendering
namespace Game::Render
{
    struct Rotation
    {
        float angleX,angleY,angleZ;
        Rotation() : angleX(0),angleY(0),angleZ(0)
        {}
        explicit Rotation(const float angle) : angleX(0),angleY(0),angleZ(angle)
        {}
        explicit Rotation(const float angleX,const float angleY,const float angleZ) : angleX(angleX),angleY(angleY),angleZ(angleZ)
        {}
    };
}