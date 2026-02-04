#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template<typename T>
    void PlayerSystem(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BulletHellState> &query,
            System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        constexpr auto frame_time = 1;

        for (auto &[id, comp]: query)
        {
            auto &state = comp.get<Battle::BulletHellState>();
            if (state.iframe_time > 0)
            {
                state.iframe_time -= frame_time;
            }

            if (state.iframe_time < 0)
            {
                state.iframe_time = 0;
            }
        }
    }
} // namespace Game::BulletHell
