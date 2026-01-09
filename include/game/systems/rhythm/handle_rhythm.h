#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void HandleRhythm(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState> &battle_query,
            System::ECS::Query<Rhythm::KeyInput> &input_query,
            [[maybe_unused]] System::ECS::Query<Rhythm::Lane, Rhythm::Timing> &note_query,
            [[maybe_unused]] System::ECS::Query<Rhythm::Lane, Rhythm::Timing, Rhythm::TimingEnd> &hold_note_query)
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
                HandleTapNote(0, syscall, battle_query, note_query);
            }
            if (comp.get<KeyInput>().input2 == true)
            {
                HandleTapNote(1, syscall, battle_query, note_query);
            }
            if (comp.get<KeyInput>().input3 == true)
            {
                HandleTapNote(2, syscall, battle_query, note_query);
            }
            if (comp.get<KeyInput>().input4 == true)
            {
                HandleTapNote(3, syscall, battle_query, note_query);
            }
        }
    }

    template<typename T>
    void HandleTapNote(
        int lane_num, // assume that first lane is lane 0
        [[maybe_unused]] T &syscall,
        [[maybe_unused]] System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Rhythm::Lane, Rhythm::Timing> &note_query)
    {   
        for (auto &[id2, comp2] : note_query)
        {
            if (comp2.get<Lane>().lane == lane_num)
            {
                auto &time_diff = comp2.get<Timing>().timing - battle_query.front().get<Battle::BattleState>().clock_time;
                
                if (time_diff >= -50 && time_diff <= 50)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                    syscall.remove_entity(id2);
                }
                else if (time_diff >= -75 && time_diff <= 75)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
                    syscall.remove_entity(id2);
                }
                else if (time_diff >= -100 && time_diff <= 100)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
                    syscall.remove_entity(id2);
                }
            }
        }
    }

    template<typename T>
    void HandleHoldNote(
        int lane_num, // assume that first lane is lane 0
        [[maybe_unused]] T &syscall,
        [[maybe_unused]] System::ECS::Query<Rhythm::KeyInput> &input_query,
        [[maybe_unused]] System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Rhythm::Lane, Rhythm::Timing, Rhythm::TimingEnd> &note_query)
    {
        for (auto &[id2, comp2] : note_query)
        {
            if (comp2.get<Lane>().lane == lane_num) // check when start holding
            {
                auto &time_diff_start = comp2.get<Timing>().timing - battle_query.front().get<Battle::BattleState>().clock_time;
                
                if (time_diff_start >= -50 && time_diff_start <= 50)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                }
                else if (time_diff_start >= -75 && time_diff_start <= 75)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
                }
                else if (time_diff_start >= -100 && time_diff_start <= 100)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
                }

                for (auto &[id3, comp3] : input_query) // check when stop holding
                {
                    auto &time_diff_end = comp2.get<TimingEnd>().timing - battle_query.front().get<Battle::BattleState>().clock_time;

                    if ((lane_num == 0 && comp3.get<KeyInput>().input1 == false) ||
                        (lane_num == 1 && comp3.get<KeyInput>().input2 == false) ||
                        (lane_num == 2 && comp3.get<KeyInput>().input3 == false) ||
                        (lane_num == 3 && comp3.get<KeyInput>().input4 == false))
                        {
                            if (time_diff_end <= 50)
                            {
                                battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                            }
                            else if (time_diff_end <= 75)
                            {
                                battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
                            }
                            else if (time_diff_end <= 100)
                            {
                                battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
                            }
                            else
                            {
                                battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
                            }
                        }

                    if (time_diff_end == 0) // check for perfect end timing
                    {
                        battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                    }
                    syscall.remove_entity(id2);
                }
            }
        }
    }
} // namespace Game::Rhythm
