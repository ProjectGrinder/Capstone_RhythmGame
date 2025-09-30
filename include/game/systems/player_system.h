#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template <typename T>
    void PlayerSystem([[maybe_unused]] T &syscall, System::ECS::Query<Battle::BattleState> &query)
    {

        constexpr auto frame_time = 0.001f;

        for (auto &[id, comp]: query)
        {
            auto &state = comp.get<Battle::BattleState>();
            if (state.bullet_hell_state.iframe_time > 0)
            {
                state.bullet_hell_state.iframe_time -= frame_time;
            }

            if (state.bullet_hell_state.iframe_time < 0)
            {
                state.bullet_hell_state.iframe_time = 0;
            }
        }
    }
}