#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template <typename T>
    void DelayBulletSystem([[maybe_unused]] T &syscall, System::ECS::Query<Delay> &query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        constexpr auto frame_time = 1; // 1ms

        for (auto &[id, comps] : query)
        {
            auto &delay_comp = comps.get<Delay>();
            if (delay_comp.delay>0)
            {
                delay_comp.delay -= frame_time;
            }
        }
    }
}