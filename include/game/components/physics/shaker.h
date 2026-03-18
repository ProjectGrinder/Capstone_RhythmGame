#pragma once

namespace Game::Physics
{
    struct Shaker
    {
        int shakingTime;
        int delay;
        float magnitudeX;
        float magnitudeY;
        Position last_position;
        explicit Shaker(const int shakingTime, const int delay, const float magnitude) :
            shakingTime(shakingTime), delay(delay), magnitudeX(magnitude), magnitudeY(magnitude)
        {}
        explicit Shaker(const int shakingTime, const int delay, const float magnitudeX, const float magnitudeY) :
            shakingTime(shakingTime), delay(delay), magnitudeX(magnitudeX), magnitudeY(magnitudeY)
        {}
    };
} // namespace Game::Physics
