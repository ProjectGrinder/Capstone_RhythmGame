#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template<typename T>
    void bullet_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Bullet, Delay> &query,
            System::ECS::Query<Battle::BattleState> &query2)
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
            const auto &delay = comps.get<Delay>();

            if (bullet.pierce<=0)
            {
                bullet.is_active = false;
                bullet.is_damageable = false;

                // Queue for removal
                syscall.remove_entity(id);
            }

            if (!bullet.is_active)
            {
                bullet.is_damageable = false;
            }

            if (bullet.is_active && !bullet.is_damageable && delay.delay<=0)
            {
                bullet.is_damageable = true;
            }
        }
    }

    template <typename T>
    void shot_delay_system([[maybe_unused]] T &syscall, System::ECS::Query<Bullet, Delay> &query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
        {
            return;
        }

        for (auto &[id, comps]: query)
        {
            auto &bullet = comps.get<Bullet>();
            auto &delay_comp = comps.get<Delay>();
            if (!bullet.is_damageable && delay_comp.delay <= 0)
            {
                bullet.is_damageable = true;
                delay_comp.delay = 0;
            }
        }
    }
} // namespace Game::BulletHell
