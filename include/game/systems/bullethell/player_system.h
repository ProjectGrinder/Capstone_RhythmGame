#pragma once

#include "game/components.h"
#include "game/utils/constant.h"

namespace Game::BulletHell
{
    template<typename T>
    void player_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Input> &input_query,
            System::ECS::Query<Battle::BulletHellState> &query,
            System::ECS::Query<Battle::BattleState> &query2,
            System::ECS::Query<Player, Render::Transform, Render::Sprite> &query3,
            System::ECS::Query<PlayerHitbox, Render::Transform, Render::Sprite> &query4)
    {
        if (query.begin() == query.end())
            return;

        if (query2.begin() == query2.end())
            return;

        if (query3.begin() == query3.end())
            return;

        if (query4.begin() == query4.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        if (!query3.front().get<Player>().is_active)
            return;

        auto &state = query.front().get<Battle::BulletHellState>();
        if (state.iframe_time > 0)
            state.iframe_time -= static_cast<int>(get_delta_time());

        if (state.iframe_time < 0)
            state.iframe_time = 0;

        // Assume 1 player
        query4.front().get<Render::Transform>() = query3.front().get<Render::Transform>();
        float& alpha = query4.front().get<PlayerHitbox>().alpha;
        if (input_query.front().get<Input>().shift && alpha < 1)
                alpha += (float)get_delta_time()/1000.f * query4.front().get<PlayerHitbox>().show_speed;
        else if (!input_query.front().get<Input>().shift && alpha > 0)
                alpha -= (float)get_delta_time()/1000.f * query4.front().get<PlayerHitbox>().show_speed;
        query3.front().get<Render::Sprite>().color.a = 1.f-(alpha*0.6f);
        query4.front().get<Render::Sprite>().color.a = alpha;
    }
} // namespace Game::BulletHell
