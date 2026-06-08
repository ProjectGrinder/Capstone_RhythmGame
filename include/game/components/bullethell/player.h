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

    struct PlayerHitbox
    {
        float alpha = 0.f;
        float show_speed;
        PlayerHitbox(const float show_speed) : show_speed(show_speed){}
    };
} // namespace Game::BulletHell
