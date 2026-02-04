#pragma once

namespace Game::BulletHell
{
    struct Bullet
    {
        bool is_active;
        bool is_damageable;
        int damage;
        int pierce;
        Bullet() : is_active(true), is_damageable(true), damage(0), pierce(1)
        {}

        explicit Bullet(const int damage) : is_active(true), is_damageable(true), damage(damage), pierce(1)
        {}

        explicit Bullet(const bool damageable, const int damage) : is_active(true), is_damageable(damageable), damage(damage), pierce(1)
        {}

        explicit Bullet(const bool damageable, const int damage, const int pierce) : is_active(true), is_damageable(damageable), damage(damage), pierce(pierce)
        {}
    };
}