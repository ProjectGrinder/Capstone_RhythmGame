#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void update_combo(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Combo, Render::Text> &combo_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
            return;

        if (combo_query.begin() == combo_query.end())
            return;

        auto combo = battle_query.front().get<Battle::BattleState>().combo;

        if (combo > 0)
        {
            combo_query.front().get<Render::Text>().text = std::to_string(combo);
        }
        else
        {
            combo_query.front().get<Render::Text>().text = "";
        }
    }
}