#pragma once

namespace Game::Battle
{
    enum PatternOp : uint8_t
    {
        OP_SET,
        OP_ADD
    };
    constexpr size_t MAX_PATTERNS = 128;
    constexpr size_t MAX_PATTERN_SEQUENCE = 16;

    struct PatternStep
    {
        float delay;
        PatternOp op;
        uint8_t mask;
        std::array<float, 4> p;
        PatternStep() : delay(0), op(OP_SET), mask(0), p({0,0,0,0})
        {}
        PatternStep(
                const float delay,
                const PatternOp op,
                const uint8_t mask = 0,
                const float p1 = 0, const float p2 = 0, const float p3 = 0, const float p4 = 0) :
            delay(delay), op(op), mask(mask), p({p1,p2,p3,p4})
        {}
    };

    struct PatternSequence
    {
        uint32_t* steps;
        size_t stepCount;
        bool isLoop;

        PatternSequence() : steps(nullptr), stepCount(0), isLoop(false) {}

        template<typename... Step>
        explicit PatternSequence(const bool loop, Step... step)
            : stepCount(sizeof...(step)), isLoop(loop)
        {
            if constexpr (sizeof...(step) > 0)
            {
                steps = new uint32_t[stepCount];

                uint32_t temp[] = { static_cast<uint32_t>(step)... };
                std::copy(temp, temp + stepCount, steps);
            }
            else steps = nullptr;
        }
    };

    struct PatternContainer
    {
        PatternSequence pattern_sequences[MAX_PATTERN_SEQUENCE];
        PatternStep pattern_steps[MAX_PATTERNS];

        PatternContainer(
        PatternStep steps[], const int step_count,
        PatternSequence seqs[], const int seq_count)
        {
            for (int i = 0; i < seq_count && i < MAX_PATTERN_SEQUENCE; i++)
                pattern_sequences[i] = seqs[i];

            for (int i = 0; i < step_count && i < MAX_PATTERNS; i++)
                pattern_steps[i] = steps[i];
        }
    };

}