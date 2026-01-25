#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template <typename T>
    void DelayBulletSystem([[maybe_unused]] T &syscall, System::ECS::Query<Bullet, Delay> &query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
        {
            return;
        }

        for (auto &[id, comps] : query)
        {
            auto &bullet = comps.get<Bullet>();
            auto &delay_comp = comps.get<Delay>();
            if (!bullet.is_active)
            {
                continue;
            }

            if (!bullet.is_damageable && delay_comp.delay <= 0)
            {
                bullet.is_damageable = true;
                delay_comp.delay = 0;
            }
        }
    }
}