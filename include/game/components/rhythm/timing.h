#pragma once

namespace Game::Rhythm
{
    struct Timing
    {
        int lane;
        int timing;
        explicit Timing(const int lane, const int timing) : lane(lane), timing(timing)
        {}
    };
} // namespace Game::Rhythm
