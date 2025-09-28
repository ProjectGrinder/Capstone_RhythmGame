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
    };
}