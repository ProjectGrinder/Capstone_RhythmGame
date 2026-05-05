#pragma once

namespace Game::Rhythm
{
    struct Lane
    {
        int lane_number;
        int hold_start_time;
        int hold_end_time;
        explicit Lane(const int lane_number) : lane_number(lane_number), hold_start_time(0), hold_end_time(0)
        {}
    };
} // namespace Game::Rhythm
