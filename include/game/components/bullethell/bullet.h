#pragma once

namespace Game::BulletHell
{
    struct Bullet
    {
        float telegraph_time;
        bool is_active;
        bool is_damageable;
        int damage;
    };
}