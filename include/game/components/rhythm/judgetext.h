#pragma once

namespace Game::Rhythm
{
    enum Judge
    {
        PERFECT,
        GREAT,
        FINE,
        MISS
    };

    struct JudgeText
    {
        float pulse_time = 100.00f;
        float stay_time = 800.00f;
        bool change = false;
    };
} // namespace Game::Rhythm
