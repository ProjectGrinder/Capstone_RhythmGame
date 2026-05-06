#pragma once

#include "game/components.h"

namespace Game::Battle
{
    template<typename T>
    void update_score(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState> &battle_query,
        System::ECS::Query<Score, Render::Text> &score_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (score_query.begin() == score_query.end())
            return;

        auto score = battle_query.front().get<BattleState>().score;

        score_query.front().get<Render::Text>().text = std::to_string(score);
    }
}