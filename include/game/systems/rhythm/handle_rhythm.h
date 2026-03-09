#pragma once

#include "game/components.h"
#include "utils/print_debug.h"

namespace Game::Rhythm
{
    template<typename T>
    void handle_normal_note(
        [[maybe_unused]] T &syscall,
        const int &time_diff,
        System::ECS::pid &id,
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType>::StoredTuple *comp,
        System::ECS::Query<Battle::BattleState> &battle_query)
    {
        constexpr auto perfect_judge = 50;
        constexpr auto great_judge = 75;
        constexpr auto fine_judge = 100;
        // const auto timing = battle_query.front().get<Battle::BattleState>().clock_time;
        if (time_diff > -1 * perfect_judge && time_diff < perfect_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
        }
        else if (time_diff > -1 * great_judge && time_diff < great_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
        }
        else if (time_diff > -1 * fine_judge && time_diff < fine_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
        }
        else return;

        if (comp->get<TimingEnd>().timing_end == 0)
        {
            syscall.remove_entity(id); // remove tap notes
        }
        else
        {
            comp->get<HoldActive>().hold_active = true; // start holding
        }
    }

    template<typename T>
    void handle_accent_note(
        [[maybe_unused]] T &syscall,
        const int &time_diff,
        System::ECS::pid &id,
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType>::StoredTuple *comp,
        System::ECS::Query<Battle::BattleState> &battle_query)
    {
        if (time_diff > -100 && time_diff < 100)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
        }
        else return;

        if (comp->get<TimingEnd>().timing_end == 0)
        {
            syscall.remove_entity(id); // remove tap notes
        }
        else
        {
            comp->get<HoldActive>().hold_active = true; // start holding
        }
    }

    template<typename T>
    void handle_rain_note(
        [[maybe_unused]] T &syscall,
        const int &time_diff,
        System::ECS::pid &id,
        System::ECS::Query<Battle::BattleState> &battle_query)
    {
        if (time_diff > -100 && time_diff < 50)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
            syscall.remove_entity(id);
        }
    }

    template<typename T>
    void handle_hold_note_release(
        T &syscall,
        const int &time_diff_end,
        System::ECS::pid &id,
        System::ECS::Query<Battle::BattleState> &battle_query)
    {
        // const auto timing = battle_query.front().get<Battle::BattleState>().clock_time;
        if (time_diff_end > -50)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
        }
        else if (time_diff_end > -75)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
        }
        else if (time_diff_end > -100)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
        }
        else
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
        }
        syscall.remove_entity(id);
    }

    template<typename T>
    void handle_note_from_lane(
        const int lane_num, // assume that first lane is 0
        [[maybe_unused]] T &syscall,
        System::ECS::pid &id,
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType>::StoredTuple *comp,
        [[maybe_unused]] System::ECS::Query<Battle::BattleState> &battle_query,
        [[maybe_unused]] System::ECS::Query<KeyInput> &input_query)
    {
        if (comp == nullptr) return;

        const auto &current_time = battle_query.front().get<Battle::BattleState>().clock_time / 1000;
        const auto &note_time = comp->get<Timing>().timing;
        const auto note_type = comp->get<NoteType>().type;
        auto &hold_active = comp->get<HoldActive>().hold_active;

        if (!hold_active)
        {
            const auto time_diff = current_time - note_time;

            if (time_diff <= -100 || time_diff >= 100)
            {
                return;
            }

            // handle both tap notes and "un-hold" hold notes
            for (auto &[id3, comp3] : input_query) // check when stop holding
            {
                if ((lane_num == 0 && comp3.get<KeyInput>().key1_pressed == true) ||
                (lane_num == 1 && comp3.get<KeyInput>().key2_pressed == true) ||
                (lane_num == 2 && comp3.get<KeyInput>().key3_pressed == true) ||
                (lane_num == 3 && comp3.get<KeyInput>().key4_pressed == true))
                {
                    if (note_type == 1)
                    {
                        handle_accent_note(syscall, time_diff, id, comp, battle_query);
                    }
                    else if (note_type == 0)
                    {
                        handle_normal_note(syscall, time_diff, id, comp, battle_query);
                    }
                }
                if (note_type == 2)
                {
                    handle_rain_note(syscall, time_diff, id, battle_query);
                }
            }
        }
        else
        {
            const auto &end_time = comp->get<TimingEnd>().timing_end;
            const auto time_diff_end = current_time - end_time;

            // handle holding
            for (auto &[id4, comp4] : input_query) // check when stop holding
            {
                if ((lane_num == 0 && comp4.get<KeyInput>().key1_hold == false) ||
                (lane_num == 1 && comp4.get<KeyInput>().key2_hold == false) ||
                (lane_num == 2 && comp4.get<KeyInput>().key3_hold == false) ||
                (lane_num == 3 && comp4.get<KeyInput>().key4_hold == false))
                {
                    handle_hold_note_release(syscall, time_diff_end, id, battle_query);
                    break;
                }

                if (time_diff_end <= 0) // check for perfect end timing
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;

                    syscall.remove_entity(id);
                    break;
                    }
            }
        }
    }

    template<typename T>
    void handle_rhythm(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState> &battle_query,
            System::ECS::Query<KeyInput> &input_query,
            [[maybe_unused]] System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType> &note_query)
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

        const auto key_input = input_query.front().get<KeyInput>();
        if (key_input.key1_hold == false && key_input.key2_hold == false
            && key_input.key3_hold == false && key_input.key4_hold == false)
        {
            return;
        }
        // Create variable
        int first_timing1 = 999999;
        int first_timing2 = 999999;
        int first_timing3 = 999999;
        int first_timing4 = 999999;
        System::ECS::pid note_id1 = 0;
        System::ECS::pid note_id2 = 0;
        System::ECS::pid note_id3 = 0;
        System::ECS::pid note_id4 = 0;
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType>::StoredTuple *note_comp1 = nullptr;
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType>::StoredTuple *note_comp2 = nullptr;
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType>::StoredTuple *note_comp3 = nullptr;
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType>::StoredTuple *note_comp4 = nullptr;



        // Check first note of each lane where input is true
        for (auto &[id, comp] : note_query)
        {
            const int first_timing = comp.get<Timing>().timing;

            if (key_input.key1_hold == true && comp.get<Lane>().lane == 0 && first_timing < first_timing1)
            {
                first_timing1 = first_timing;
                note_id1 = id;
                note_comp1 = &comp;
            }
            if (key_input.key2_hold == true && comp.get<Lane>().lane == 1 && first_timing < first_timing2)
            {
                first_timing2 = first_timing;
                note_id2 = id;
                note_comp2 = &comp;
            }
            if (key_input.key3_hold == true && comp.get<Lane>().lane == 2 && first_timing < first_timing3)
            {
                first_timing3 = first_timing;
                note_id3 = id;
                note_comp3 = &comp;
            }
            if (key_input.key4_hold == true && comp.get<Lane>().lane == 3 && first_timing < first_timing4)
            {
                first_timing4 = first_timing;
                note_id4 = id;
                note_comp4 = &comp;
            }
        }

        // Check if note is in hit range
        if (key_input.key1_hold == true && first_timing1 < 999999)
        {
            handle_note_from_lane(0, syscall, note_id1, note_comp1, battle_query, input_query);
        }
        if (key_input.key2_hold == true && first_timing2 < 999999)
        {
            handle_note_from_lane(1, syscall, note_id2, note_comp2, note_query, battle_query, input_query);
        }
        if (key_input.key3_hold == true && first_timing3 < 999999)
        {
            handle_note_from_lane(2, syscall, note_id3, note_comp3, note_query, battle_query, input_query);
        }
        if (key_input.key4_hold == true && first_timing4 < 999999)
        {
            handle_note_from_lane(3, syscall, note_id4, note_comp4, note_query, battle_query, input_query);
        }
    }
} // namespace Game::Rhythm
