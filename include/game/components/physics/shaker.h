#pragma once

namespace Game::Physics
{
    struct Shaker
    {
        float shakingTime;
        float delay;
        float magnitudeX;
        float magnitudeY;
        Position last_position;
        explicit Shaker(const float shakingTime, const float delay, const float magnitude):
            shakingTime(shakingTime),delay(delay),magnitudeX(magnitude),magnitudeY(magnitude)
        {}
        explicit Shaker(const float shakingTime, const float delay, const float magnitudeX, const float magnitudeY):
            shakingTime(shakingTime),delay(delay),magnitudeX(magnitudeX),magnitudeY(magnitudeY)
        {}
    };
}
