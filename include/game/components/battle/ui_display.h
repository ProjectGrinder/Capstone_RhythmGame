#pragma once

namespace Game::Battle
{
    struct UIDisplay
    {
        enum Phase
        {
            BULLET,
            RHYTHM,
            BOTH
        };
        Phase phase;
        explicit UIDisplay(const Phase phase) : phase(phase) {}
    };
}