#pragma once

namespace Game::BulletHell
{
    enum PatternType
    {
        NO_PATTERN,
    };

    struct Pattern
    {
        PatternType type;
        float pattern_args[2];
        Pattern() : type(NO_PATTERN), pattern_args{0,0}
        {}
        explicit Pattern(const PatternType type, const float arg1 = 0, const float arg2 = 0) : type(type),pattern_args{arg1,arg2}
        {}
    };
}