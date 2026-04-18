#pragma once

namespace Game::Render
{
    struct Transform
    {
        Math::Point position{};
        float angleX, angleY, angleZ;
        float scaleX, scaleY, scaleZ;

        Transform() = default;

        Transform(const float posX, const float posY, const float angleZ = 0) : angleX(0), angleY(0), scaleX(0), scaleY(0), scaleZ(0)
        {
            position.x = posX;
            position.y = posY;
            this->angleZ = angleZ;
        }

        Transform(
                const float posX,
                const float posY,
                const float angleX,
                const float angleY,
                const float angleZ,
                const float scaleX,
                const float scaleY,
                const float scaleZ) :
            position{posX,posY}, angleX(angleX), angleY(angleY), angleZ(angleZ), scaleX(scaleX), scaleY(scaleY), scaleZ(scaleZ)
        {}
    };
} // namespace Game::Render
