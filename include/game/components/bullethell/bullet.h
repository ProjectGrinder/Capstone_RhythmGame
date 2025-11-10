#pragma once

namespace Game::BulletHell
{
    struct Bullet
    {
        int telegraph_time;
        bool is_active;
        bool is_damageable;
        int damage;
        Bullet() : telegraph_time(0), is_active(false), is_damageable(false), damage(0)
        {}
        explicit Bullet(const int telegraph_time, const int damage) : telegraph_time(telegraph_time), is_active(false), is_damageable(false), damage(damage)
        {}
    };
}