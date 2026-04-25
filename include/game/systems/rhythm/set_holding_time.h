#pragma once

#include "game/components.h"
#include "utils/print_debug.h"

namespace Game::Rhythm
{
    // Must run after handle rhythm to check for holding
    template<typename T>
    void set_holding_time(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<KeyInput> &input_query,
        System::ECS::Query<Lane> &lane_query,
        [[maybe_unused]] System::ECS::Query<HoldConnect, NoteStatus> &hold_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
            return;

        if (input_query.begin() == input_query.end())
            return;

        bool check_lane = false;

        for (auto &[id, comp] : lane_query)
        {
            check_lane = comp.get<Lane>().hold_start_time > 0 && comp.get<Lane>().hold_end_time == 0;
            if (check_lane == true) // only check if we need to assign timing for holding
                break;
        }
        if (check_lane == false)
            return;

        for (auto &[id, comp] : lane_query)
        {
            for (auto &[id2, comp2] : hold_query)
            {
                if (comp2.get<NoteStatus>().state == -1)
                    continue;

                if (comp2.get<HoldConnect>().lane == comp.get<Lane>().lane_number &&
                    comp2.get<HoldConnect>().timing_start == comp.get<Lane>().hold_start_time)
                {
                    comp.get<Lane>().hold_end_time = comp2.get<HoldConnect>().timing_end;
                    comp2.get<NoteStatus>().state = -1;
                    break;
                }
            }
        }
    }
}