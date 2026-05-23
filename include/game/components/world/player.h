#pragma once

namespace Game::Overview
{
    struct Player
    {
        bool is_active;
        bool on_ground = false;
        Player() : is_active(true)
        {}
        explicit Player(const bool is_active) : is_active(is_active)
        {}
    };
} // namespace Game::Overview
