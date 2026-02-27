#pragma once
#include "game/utils/physics_util.h"

namespace Game::BulletHell
{
    constexpr int MAX_PATTERNS = 16;
    constexpr int MAX_PATTERN_SEQUENCE = 16;
    struct PatternStep;
    struct PatternSequence;

    inline PatternSequence pattern_sequences[MAX_PATTERN_SEQUENCE];
    using PatternFunc = void(*)(uint64_t, const PatternStep&);
    struct PatternStep
    {
        float delay;
        PatternFunc func;
        uint8_t mask;
        float p1,p2,p3,p4;
        PatternStep() : delay(0),func({}), mask(0), p1(-1), p2(-1), p3(-1), p4(-1)
        {}
        explicit PatternStep(
                const float delay,
                const PatternFunc func,
                const uint8_t mask = 0,
                const float p1 = 0,
                const float p2 = 0,
                const float p3 = 0,
                const float p4 = 0) :
            delay(delay), func(func), mask(mask), p1(p1), p2(p2), p3(p3), p4(p4)
        {}
    };

    struct PatternSequence
    {
        PatternFunc funcs[MAX_PATTERN_SEQUENCE];
    };

    // This is a component
    struct Pattern
    {
        float delay;
        float sequenceID;
        uint16_t sequenceIdx;
        float p1, p2, p3;
        bool isLoop;

        Pattern() : delay(0), sequenceID(0), sequenceIdx(0), p1(0), p2(0), p3(0), isLoop(false)
        {}

        explicit Pattern(
                const float delay,
                const float sequenceID,
                const uint16_t sequenceIdx,
                const float p1 = 0,
                const float p2 = 0,
                const float p3 = 0,
                const bool isLoop = false) :
            delay(delay), sequenceID(sequenceID), sequenceIdx(sequenceIdx), p1(p1), p2(p2), p3(p3), isLoop(isLoop)
        {}
    };

    inline void Pattern_AddSpeed(uint64_t entityId, const PatternStep& step)
    {
        // Call syscall to change movement field
    }
}