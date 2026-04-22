#pragma once

namespace Game::Rhythm
{
    struct HoldConnect
    {
        int timing_start;
        int timing_end;
        explicit HoldConnect(const int timing_start, const int timing_end) : timing_start(timing_start), timing_end(timing_end)
        {}
    };
} // namespace Game::Rhythm
