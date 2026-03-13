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
        int delay;
        PatternOp op;
        uint8_t mask;
        std::array<float, 4> p;
        PatternStep() : delay(0), op(OP_SET), mask(0), p({0,0,0,0})
        {}
        PatternStep(
                const int delay,
                const PatternOp op,
                const uint8_t mask = 0,
                const float p1 = 0, const float p2 = 0, const float p3 = 0, const float p4 = 0) :
            delay(delay), op(op), mask(mask), p({p1,p2,p3,p4})
        {}
    };

    struct PatternSequence
    {
        std::vector<uint32_t> steps;
        bool isLoop;

        PatternSequence() : isLoop(false) {}

        template<typename... Step>
        explicit PatternSequence(const bool loop, Step... step)
            : steps{ static_cast<uint32_t>(step)... }, isLoop(loop)
        {}
    };

    struct PatternContainer
    {
        std::vector<PatternSequence> pattern_sequences;
        std::vector<PatternStep> pattern_steps;

        PatternContainer(
            std::vector<PatternStep> steps,
            std::vector<PatternSequence> seqs)
            : pattern_sequences(std::move(seqs)),
              pattern_steps(std::move(steps))
        {}
    };

}