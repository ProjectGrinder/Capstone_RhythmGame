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
        int timer;
        JudgeText() : timer(0) {}
    };
} // namespace Game::Rhythm
