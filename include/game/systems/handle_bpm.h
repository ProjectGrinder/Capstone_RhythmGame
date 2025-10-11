#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void HandleBPM(T &syscall, System::ECS::Query<Battle::BattleState> &query)
    {
        auto &state = query.front().get<Battle::BattleState>();
        auto &[bpm_list, idx] = state.level_data.bpm_info;

        if (idx >= bpm_list.size())
        {
            return;
        }

        if (auto &clock = state.clock_time; clock > bpm_list.at(idx).timing)
        {
            auto old_bpm = bpm_list.at(idx).bpm;
            ++idx;
            auto new_bpm = bpm_list.at(idx).bpm;
            state.rhythm_state.note_speed *= new_bpm / old_bpm;
        }
    }
}