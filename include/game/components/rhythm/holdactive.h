#pragma once

namespace Game::Rhythm
{
    struct HoldActive
    {
        bool hold_active;
        HoldActive() : hold_active(false)
        {}
        explicit HoldActive(const bool hold_active) : hold_active(hold_active)
        {}
    };
}