#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template<typename T>
    void bullet_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Bullet> &query,
            System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
        {
            return;
        }

        constexpr auto frame_time = 1; // 1ms

        for (auto &[id, comps]: query)
        {
            auto &bullet = comps.get<Bullet>();
            if (!bullet.is_active)
            {
                continue;
            }

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
} // namespace Game::BulletHell
