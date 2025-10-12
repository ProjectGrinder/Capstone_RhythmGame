#pragma once

namespace Game::BulletHell
{
    struct Bullet
    {
        int telegraph_time;
        bool is_active;
        bool is_damageable;
        int damage;
    };
}