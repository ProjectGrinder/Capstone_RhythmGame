#pragma once

#include "result_screen.h"

namespace Game::Battle
{
    template<typename T>
    void check_player_state(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState, BulletHellState, RhythmState> &battle_query,
        System::ECS::Query<LevelData> &level_data_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        auto &battle_state = battle_query.front().get<BattleState>();
        const auto level_data = level_data_query.front().get<LevelData>();

        if (battle_state.player_state == PlayerState::DEAD || battle_state.player_state == PlayerState::FINISH)
            return;


        if (battle_state.clock_time >= 0)
        {
            if (battle_state.clock_time/1000 > level_data.duration + 2000)
            {
                battle_state.player_state = PlayerState::FINISH;
                create_result_ui(syscall, battle_query);
            }
            // else if (battle_state.hp > 0)
            // {
            //     battle_state.player_state = PlayerState::PLAY;
            // }
            // else
            // {
            //     battle_state.player_state = PlayerState::DEAD;
            //     create_result_ui(syscall, battle_query);
            // }
            else
                battle_state.player_state = PlayerState::PLAY;
        }
    }
}
