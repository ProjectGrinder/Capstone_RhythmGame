#pragma once

namespace Game::BulletHell
{
    enum PatternOp : uint8_t
    {
        OP_SET,
        OP_ADD
    };
    constexpr int MAX_PATTERNS = 16;
    constexpr int MAX_PATTERN_SEQUENCE = 16;



    struct PatternStep
    {
        float delay;
        PatternOp op;
        uint8_t mask;
        float p[];
        PatternStep() : delay(0),op(OP_SET), mask(0)
        {}
        explicit PatternStep(
                const float delay,
                const PatternOp op,
                const uint8_t mask = 0,
                const float p = {}) :
            delay(delay), op(op), mask(mask), p(p)
        {}
    };

    struct PatternSequence
    {
        bool isLoop;
        uint16_t stepCount;
        PatternStep *step;
        PatternSequence() = default;
        explicit PatternSequence(PatternStep steps[16], const uint16_t stepCount, const bool isLoop = false) :
            isLoop(isLoop), stepCount(stepCount), step(steps)
        {}
    };

    inline PatternSequence pattern_sequences[MAX_PATTERN_SEQUENCE];
}
