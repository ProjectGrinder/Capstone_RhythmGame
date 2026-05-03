#pragma once

namespace Game::Battle
{
    template<typename T>
    void phase_change(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<TransitionData> &transition_query,
        System::ECS::Query<BattleState> &battle_query,
        System::ECS::Query<UIDisplay, Render::Material> &ui_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        auto &battle_state = battle_query.front().get<BattleState>();

        // starting phase
        if (battle_state.clock_time <= 0)
        {
            if (battle_state.current_phase == BULLET_HELL)
            {
                for (auto &[id1, comp]: ui_query)
                {
                    if (comp.get<UIDisplay>().phase == UIDisplay::RHYTHM)
                        comp.get<Render::Material>().visible = false;
                    else
                        comp.get<Render::Material>().visible = true;
                }
            }
            else if (battle_state.current_phase == RHYTHM)
            {
                for (auto &[id1, comp]: ui_query)
                {
                    if (comp.get<UIDisplay>().phase == UIDisplay::BULLET)
                        comp.get<Render::Material>().visible = false;
                    else
                        comp.get<Render::Material>().visible = true;
                }
            }
        }

        for (auto &[id, comps] : transition_query)
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
                {
                    for (auto &[id2, comp2]: ui_query)
                    {
                        if (comp2.get<UIDisplay>().phase == UIDisplay::RHYTHM)
                            comp2.get<Render::Material>().visible = false;
                        else
                            comp2.get<Render::Material>().visible = true;
                    }
                    LOG_INFO("Scene has changed to CALL")
                }
                else if (transition_data.phase == RHYTHM)
                {
                    for (auto &[id2, comp2]: ui_query)
                    {
                        if (comp2.get<UIDisplay>().phase == UIDisplay::BULLET)
                            comp2.get<Render::Material>().visible = false;
                        else
                            comp2.get<Render::Material>().visible = true;
                    }
                    LOG_INFO("Scene has changed to RESPONSE")
                }
                battle_state.current_phase = transition_data.phase;
                syscall.remove_entity(id);
            }
        }


    }
} // namespace Game::Battle
