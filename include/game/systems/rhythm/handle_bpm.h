#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void handle_bpm(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::LevelData> &level_query,
            System::ECS::Query<Battle::RhythmState> &rhythm_query,
            System::ECS::Query<Battle::BattleState> &battle_query)
    {
        if (level_query.begin() == level_query.end())
            return;

        if (rhythm_query.begin() == rhythm_query.end())
            return;

        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
        {
            return;
        }

        auto &level_data = level_query.front().get<Battle::LevelData>();
        auto &[bpm_list, idx] = level_data.bpm_info;
        if (idx + 1 >= bpm_list.size() || bpm_list.empty())
        {
            return;
        }

        auto &battle_state = battle_query.front().get<Battle::BattleState>();
        auto clock = battle_state.clock_time / 1000;

        if (clock >= bpm_list.at(idx).timing)
        {
            auto old_bpm = bpm_list.at(idx).bpm;
            ++idx;
            auto new_bpm = bpm_list.at(idx).bpm;
            rhythm_query.front().get<Battle::RhythmState>().note_speed *= new_bpm / old_bpm;
        }
    }
} // namespace Game::Rhythm
