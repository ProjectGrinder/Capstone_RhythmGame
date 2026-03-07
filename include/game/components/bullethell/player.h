#pragma once

namespace Game::BulletHell
{
    struct Player
    {
        bool is_active;
        Player() : is_active(true)
        {}
        explicit Player(const bool is_active) : is_active(is_active)
        {}
    };
} // namespace Game::BulletHell
