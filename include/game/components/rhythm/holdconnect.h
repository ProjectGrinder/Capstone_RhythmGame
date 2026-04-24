#pragma once

namespace Game::Rhythm
{
    struct HoldConnect
    {
        int lane;
        int timing_start;
        int timing_end;
        bool hit;
        explicit HoldConnect(const int lane, const int timing_start, const int timing_end) :
        lane(lane), timing_start(timing_start), timing_end(timing_end), hit(false)
        {}
    };
} // namespace Game::Rhythm
