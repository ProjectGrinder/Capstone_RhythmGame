#pragma once

namespace Game::Rhythm
{
    enum Judge
    {
        NONE,
        MISS,
        FINE,
        GREAT,
        PERFECT
    };

    struct JudgeText
    {
        Judge judge;
        bool change;
        int timer;
        JudgeText() : judge(NONE), change(false), timer(0) {}
        explicit JudgeText(const Judge judge) : judge(judge), change(false), timer(0)
        {}
    };
} // namespace Game::Rhythm
