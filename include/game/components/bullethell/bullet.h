#pragma once

namespace Game::BulletHell
{
    struct Bullet
    {
        bool is_active;
        bool is_damageable;
        int damage;
        int pierce;
        bool is_grazed = false;
        Bullet() : is_active(true), is_damageable(false), damage(0), pierce(1)
        {}

        explicit Bullet(const int damage) : is_active(true), is_damageable(false), damage(damage), pierce(1)
        {}


        explicit Bullet(const int damage, const int pierce) : is_active(true), is_damageable(false), damage(damage), pierce(pierce)
        {}
    };
} // namespace Game::BulletHell
