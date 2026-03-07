#pragma once

namespace Game::Battle
{
    template<typename T>
    void phase_change(
            [[maybe_unused]] T &syscall, System::ECS::Query<LevelData> &query, System::ECS::Query<BattleState> &query2)
    {
        if (query.begin() == query.end())
            return;
        if (query2.begin() == query2.end())
            return;

        auto &battle_state = query2.front().get<BattleState>();
        auto &level_data = query.front().get<LevelData>();
        auto &phase_info = level_data.phase_info;

        while (phase_info.idx < phase_info.phase_list.size() &&
               phase_info.phase_list.at(phase_info.idx).timing <= battle_state.clock_time)
        {
            battle_state.current_phase = phase_info.phase_list.at(phase_info.idx).phase;
            phase_info.idx++;
        }
    }
} // namespace Game::Battle
