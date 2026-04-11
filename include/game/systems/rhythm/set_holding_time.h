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
        [[maybe_unused]] System::ECS::Query<Timing, HoldStart, NoteType, NoteStatus> &note_query)
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
            check_lane = check_lane || (comp.get<Lane>().hold_active && comp.get<Lane>().hold_end_time == 0);
            if (check_lane == true) // only check if we need to assign timing for holding
                break;
        }
        if (check_lane == false)
            return;

        int first_timing1 = 999999;
        int first_timing2 = 999999;
        int first_timing3 = 999999;
        int first_timing4 = 999999;

        for (auto &[id, comp] : note_query)
        {
            if (comp.get<NoteStatus>().state == -1 || comp.get<NoteType>().type != -1)
                continue;

            const int first_timing = comp.get<Timing>().timing;
            // With this logic, it will check the first "hold end" note of the lane regardless if it's technically the correct pair
            // Therefore, this logic doesn't work with overlapping hold notes in the same lane, but that's considered as
            // illegal charting, so that can be ignored
            if (comp.get<Timing>().lane == 0 && first_timing < first_timing1)
            {
                first_timing1 = first_timing;
            }
            if (comp.get<Timing>().lane == 1 && first_timing < first_timing2)
            {
                first_timing2 = first_timing;
            }
            if (comp.get<Timing>().lane == 2 && first_timing < first_timing3)
            {
                first_timing3 = first_timing;
            }
            if (comp.get<Timing>().lane == 3 && first_timing < first_timing4)
            {
                first_timing4 = first_timing;
            }
        }

        for (auto &[id, comp] : lane_query)
        {
            if (comp.get<Lane>().lane_number == 0 && comp.get<Lane>().hold_active == true)
            {
                comp.get<Lane>().hold_end_time = first_timing1;
            }
            if (comp.get<Lane>().lane_number == 1 && comp.get<Lane>().hold_active == true)
            {
                comp.get<Lane>().hold_end_time = first_timing2;
            }
            if (comp.get<Lane>().lane_number == 2 && comp.get<Lane>().hold_active == true)
            {
                comp.get<Lane>().hold_end_time = first_timing3;
            }
            if (comp.get<Lane>().lane_number == 3 && comp.get<Lane>().hold_active == true)
            {
                comp.get<Lane>().hold_end_time = first_timing4;
            }
        }
    }
}