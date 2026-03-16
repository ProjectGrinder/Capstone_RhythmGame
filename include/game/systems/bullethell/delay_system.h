#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template <typename T>
    void delay_system([[maybe_unused]] T &syscall, System::ECS::Query<Delay> &query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        for (auto &[id, comps] : query)
        {
            auto &delay_comp = comps.get<Delay>();
            if (delay_comp.delay>0)
            {
                delay_comp.delay -= static_cast<int>(get_delta_time());
            }
        }
    }
}