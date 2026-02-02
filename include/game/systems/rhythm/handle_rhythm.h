#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void HandleRhythm(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState> &battle_query,
            System::ECS::Query<KeyInput> &input_query,
            [[maybe_unused]] System::ECS::Query<Lane, Timing, TimingEnd, HoldActive> &note_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
        {
            return;
        }

        if (input_query.begin() == input_query.end())
            return;

        // This system should check the input, remove a note if it was hit correctly, and update score/hp/etc. correctly
        // If you need to adjust components or data structures you can ask what structures implement what data to
        // Midfield
        for (auto &[id, comp] : input_query)
        {
            if (comp.get<KeyInput>().input1 == true)
            {
                HandleNoteFromLane(0, syscall, note_query, battle_query, input_query);
            }
            if (comp.get<KeyInput>().input2 == true)
            {
                HandleNoteFromLane(1, syscall, note_query, battle_query, input_query);
            }
            if (comp.get<KeyInput>().input3 == true)
            {
                HandleNoteFromLane(2, syscall, note_query, battle_query, input_query);
            }
            if (comp.get<KeyInput>().input4 == true)
            {
                HandleNoteFromLane(3, syscall, note_query, battle_query, input_query);
            }
        }
    }

    template<typename T>
    void HandleNoteFromLane(
        int lane_num, // assume that first lane is 0
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive> &note_query,
        [[maybe_unused]] System::ECS::Query<Battle::BattleState> &battle_query,
        [[maybe_unused]] System::ECS::Query<KeyInput> &input_query)
    {
        auto first_timing = 99999;
        int note_id = -1;
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive>::StoredTuple *note_comp = nullptr;

        for (auto &[id2, comp2] : note_query)
        {
            if (comp2.get<Lane>().lane == lane_num)
            {
                if (comp2.get<Timing>().timing < first_timing)
                {
                    first_timing = comp2.get<Timing>().timing;
                    note_id = static_cast<int>(id2);
                    note_comp = &comp2;
                }
            }
        }

        if (note_id < 0 || note_comp == nullptr) return;
        
        auto &current_time = battle_query.front().get<Battle::BattleState>().clock_time;
        auto &note_time = note_comp->get<Timing>().timing;
        
        if (note_comp->get<TimingEnd>().timing_end == 0)
        {
            // it's a tap note
            auto time_diff = current_time - note_time;
                
            if (time_diff >= -50 && time_diff <= 50)
            {
                battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                LOG_INFO("Timing %d Lane %d: Perfect", note_time, lane_num);
            }
            else if (time_diff >= -75 && time_diff <= 75)
            {
                battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
                LOG_INFO("Timing %d Lane %d: Great", note_time, lane_num);
            }
            else if (time_diff >= -100 && time_diff <= 100)
            {
                battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
                LOG_INFO("Timing %d Lane %d: Fine", note_time, lane_num);
            }
            else return;

            syscall.remove_entity(note_id);
        }
        else
        {
            // it's a hold note
            auto hold_active = note_comp->get<HoldActive>().hold_active;

            if (!hold_active)
            {
                auto time_diff_start = current_time - note_time;
                
                if (time_diff_start >= -50 && time_diff_start <= 50)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                    LOG_INFO("Timing %d Lane %d: Start Hold Perfect", note_time, lane_num);
                }
                else if (time_diff_start >= -75 && time_diff_start <= 75)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
                    LOG_INFO("Timing %d Lane %d: Start Hold Great", note_time, lane_num);
                }
                else if (time_diff_start >= -100 && time_diff_start <= 100)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
                    LOG_INFO("Timing %d Lane %d: Start Hold Fine", note_time, lane_num);
                }
                else return;

                note_comp->get<HoldActive>().hold_active = true; // start holding
            }
            else
            {
                for (auto &[id3, comp3] : input_query) // check when stop holding
                {
                    auto &end_time = note_comp->get<TimingEnd>().timing_end;
                    auto time_diff_end = current_time - end_time;

                    if ((lane_num == 0 && comp3.get<KeyInput>().input1 == false) ||
                        (lane_num == 1 && comp3.get<KeyInput>().input2 == false) ||
                        (lane_num == 2 && comp3.get<KeyInput>().input3 == false) ||
                        (lane_num == 3 && comp3.get<KeyInput>().input4 == false))
                        {
                            if (time_diff_end >= -50)
                            {
                                battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                                LOG_INFO("Timing %d Lane %d: End Hold Perfect", end_time, lane_num);
                            }
                            else if (time_diff_end >= -75)
                            {
                                battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
                                LOG_INFO("Timing %d Lane %d: End Hold Great", end_time, lane_num);
                            }
                            else if (time_diff_end >= -100)
                            {
                                battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
                                LOG_INFO("Timing %d Lane %d: End Hold Fine", end_time, lane_num);
                            }
                            else
                            {
                                battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
                                LOG_INFO("Timing %d Lane %d: End Hold Miss", end_time, lane_num);
                            }
                            syscall.remove_entity(note_id);
                            break;
                        }

                    if (time_diff_end == 0) // check for perfect end timing
                    {
                        battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                        LOG_INFO("Timing %d Lane %d: End Hold Perfect", end_time, lane_num);
                        
                        syscall.remove_entity(note_id);
                        break;
                    }
                }
            }
        }
    }
} // namespace Game::Rhythm
