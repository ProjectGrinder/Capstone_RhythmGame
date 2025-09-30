#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template <typename T>
    void BulletSystem(T &syscall, System::ECS::Query<Bullet> &query)
    {

        constexpr auto frame_time = 0.001f;

        for (auto &[id, comps] : query)
        {
            auto &bullet = comps.get<Bullet>();
            if (bullet.is_active)
            {
                if (!bullet.is_damageable)
                {
                    bullet.telegraph_time -= frame_time;
                    if (bullet.telegraph_time < 0)
                    {
                        bullet.is_damageable = true;
                        bullet.telegraph_time = 0;
                    }
                }
            }
        }
    }
}