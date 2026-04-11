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
        // ex. [(180.00, 0), (90.00, 60000), (150.00, 90000), (180.00, 100000)]
        // Every level must have at least one BPM entry
        // For songs with constant BPM, it should look like this: [(180.00, 0)]
        // Timing 0 must have starting chart BPM (not necessarily equal to main BPM)
        if (bpm_list.empty() || idx + 1 > bpm_list.size())
        {
            return;
        }

        auto &battle_state = battle_query.front().get<Battle::BattleState>();
        auto clock = battle_state.clock_time / 1000;

        if (clock >= bpm_list.at(idx).timing)
        {
            LOG_INFO("Title: %s", level_data.title.c_str());
            LOG_INFO("Artist: %s", level_data.artist_name.c_str());
            LOG_INFO("Genre: %s", level_data.genre_name.c_str());
            LOG_INFO("BPM: %d", static_cast<int>(bpm_list.at(idx).bpm));
            rhythm_query.front().get<Battle::RhythmState>().current_speed =
                rhythm_query.front().get<Battle::RhythmState>().base_speed * (bpm_list.at(idx).bpm / level_data.main_bpm);
            if (bpm_list.at(idx).bpm != level_data.main_bpm || idx == 0)
            {
                rhythm_query.front().get<Battle::RhythmState>().speed_change = true;
            }
            ++idx;
        }
    }
} // namespace Game::Rhythm
