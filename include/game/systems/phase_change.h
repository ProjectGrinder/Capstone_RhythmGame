#pragma once

namespace Game::Battle
{
    template<typename T>
    void phase_change(
            [[maybe_unused]] T &syscall, System::ECS::Query<TransitionData> &query, System::ECS::Query<BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        auto &battle_state = query2.front().get<BattleState>();

        for (auto &[id, comps] : query)
        {
            auto &transition_data = comps.get<TransitionData>();
            if (transition_data.timing_start > battle_state.clock_time/1000)
            {
                continue;
            }

            if (transition_data.state == 4 && transition_data.timing_start < battle_state.clock_time/1000)
            {
                LOG_INFO("Scene changing in 3");
                transition_data.state = 3;
            }
            else if (transition_data.state == 3 && transition_data.timing_start + transition_data.duration/3 < battle_state.clock_time/1000)
            {
                LOG_INFO("Scene changing in 2");
                transition_data.state = 2;
            }
            else if (transition_data.state == 2 && transition_data.timing_start + transition_data.duration*2/3 < battle_state.clock_time/1000)
            {
                LOG_INFO("Scene changing in 1");
                transition_data.state = 1;
            }
            else if (transition_data.state == 1 && transition_data.timing_start + transition_data.duration < battle_state.clock_time/1000)
            {
                if (transition_data.phase == BULLET_HELL)
                    LOG_INFO("Scene has changed to CALL")
                else if (transition_data.phase == RHYTHM)
                    LOG_INFO("Scene has changed to RESPONSE")
                battle_state.current_phase = transition_data.phase;
                syscall.remove_entity(id);
            }
        }


    }
} // namespace Game::Battle
