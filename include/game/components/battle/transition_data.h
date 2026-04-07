#pragma once

namespace Game::Battle
{
    struct TransitionData
    {
        int timing_start;
        int duration;
        CurrentPhase phase;

        // State
        uint8_t state = 4;
        TransitionData(const int timing_start, const int duration, const CurrentPhase phase) : timing_start(timing_start), duration(duration), phase(phase)
        {}
    };
}