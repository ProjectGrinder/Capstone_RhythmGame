#pragma once

namespace Game::Rhythm
{
    struct Lane
    {
        int lane_number;
        bool hold_active;
        int hold_end_time;
        explicit Lane(const int lane_number) : lane_number(lane_number), hold_active(false), hold_end_time(0)
        {}
    };
} // namespace Game::Rhythm
