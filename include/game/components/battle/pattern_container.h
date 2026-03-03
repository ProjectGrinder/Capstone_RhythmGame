#pragma once

namespace Game::Battle
{
    enum PatternOp : uint8_t
    {
        OP_SET,
        OP_ADD
    };
    constexpr int MAX_PATTERNS = 128;
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
        uint32_t *step;
        PatternSequence() = default;
        explicit PatternSequence(uint32_t *steps, const uint16_t stepCount, const bool isLoop = false) :
            isLoop(isLoop), stepCount(stepCount), step(steps)
        {}
    };

    struct PatternContainer
    {
        PatternSequence pattern_sequences[MAX_PATTERN_SEQUENCE];
        PatternStep pattern_steps[MAX_PATTERNS];
    };

}