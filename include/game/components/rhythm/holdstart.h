#pragma once

namespace Game::Rhythm
{
    struct HoldStart
    {
        bool is_hold;
        explicit HoldStart(const bool is_hold) : is_hold(is_hold)
        {}
    };
} // namespace Game::Rhythm
