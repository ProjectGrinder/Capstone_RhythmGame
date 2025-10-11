#pragma once

#include "game/components.h"

namespace Game::Rhythm
{

    template<typename T>
    void HandleBPM(T &syscall, System::ECS::Query<Battle::LevelData> &query, System::ECS::Query<Battle::RhythmState> &query2, System::ECS::Query<Battle::BattleState> &query3)
    {
        auto &battle_state = query3.front().get<Battle::BattleState>();
        auto &rhythm_state = query2.front().get<Battle::RhythmState>();
        auto &level_data = query.front().get<Battle::LevelData>();
        auto &[bpm_list, idx] = level_data.bpm_info;

        if (idx >= bpm_list.size())
        {
            return;
        }

        if (auto &clock = battle_state.clock_time; clock > bpm_list.at(idx).timing)
        {
            auto old_bpm = bpm_list.at(idx).bpm;
            ++idx;
            auto new_bpm = bpm_list.at(idx).bpm;
            rhythm_state.note_speed *= new_bpm / old_bpm;
        }
    }
}