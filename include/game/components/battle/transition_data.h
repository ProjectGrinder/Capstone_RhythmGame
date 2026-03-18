#pragma once

namespace Game::Battle
{
    struct TransitionData
    {
        int timing_start;
        int duration;
        CurrentPhase phase;
    };
}