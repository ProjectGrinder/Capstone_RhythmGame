#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template <typename T>
    void PlayerSystem([[maybe_unused]] T &syscall, System::ECS::Query<Battle::BulletHellState> &query)
    {

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
}