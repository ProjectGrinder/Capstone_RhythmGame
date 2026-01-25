#pragma once

namespace Game::BulletHell
{
    struct Bullet
    {
        bool is_active;
        bool is_damageable;
        int damage;
        Bullet() : is_active(true), is_damageable(false), damage(0)
        {}
        explicit Bullet(const int damage) : is_active(true), is_damageable(false), damage(damage)
        {}
    };
}