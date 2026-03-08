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
        const auto timing = battle_query.front().get<Battle::BattleState>().clock_time;
        if (time_diff >= -1 * perfect_judge && time_diff <= perfect_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
            LOG_INFO("Timing %d: Perfect", timing);
        }
        else if (time_diff >= -1 * great_judge && time_diff <= great_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
            LOG_INFO("Timing %d: Great", timing);
        }
        else if (time_diff >= -1 * fine_judge && time_diff <= fine_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
            LOG_INFO("Timing %d: Fine", timing);
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
        if (time_diff >= -100 && time_diff <= 100)
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
        if (time_diff >= -100 && time_diff <= 50)
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
        const auto timing = battle_query.front().get<Battle::BattleState>().clock_time;
        if (time_diff_end >= -50)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
            LOG_INFO("Timing %d: End Hold Perfect", timing);
        }
        else if (time_diff_end >= -75)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
            LOG_INFO("Timing %d: End Hold Great", timing);
        }
        else if (time_diff_end >= -100)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
            LOG_INFO("Timing %d: End Hold Fine", timing);
        }
        else
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
            LOG_INFO("Timing %d: End Hold Miss", timing);
        }
        syscall.remove_entity(id);
    }

    template<typename T>
    void handle_note_from_lane(
        const int lane_num, // assume that first lane is 0
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType> &note_query,
        [[maybe_unused]] System::ECS::Query<Battle::BattleState> &battle_query,
        [[maybe_unused]] System::ECS::Query<KeyInput> &input_query)
    {
        int first_timing = 99999;
        System::ECS::pid note_id = 0;
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType>::StoredTuple *note_comp = nullptr;

        for (auto &[id2, comp2] : note_query)
        {
            if (comp2.get<Lane>().lane == lane_num)
            {
                if (comp2.get<Timing>().timing < first_timing)
                {
                    first_timing = comp2.get<Timing>().timing;
                    note_id = id2;
                    note_comp = &comp2;
                }
            }
        }

        if (note_id == 0 || note_comp == nullptr) return;

        const auto &current_time = battle_query.front().get<Battle::BattleState>().clock_time;
        const auto &note_time = note_comp->get<Timing>().timing;
        const auto note_type = note_comp->get<NoteType>().type;
        auto &hold_active = note_comp->get<HoldActive>().hold_active;

        if (!hold_active)
        {
            const auto time_diff = current_time - note_time;

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
                        handle_accent_note(syscall, time_diff, note_id, note_comp, battle_query);
                    }
                    else
                    {
                        handle_normal_note(syscall, time_diff, note_id, note_comp, battle_query);
                    }
                }
                else if (note_type == 2)
                {
                    handle_rain_note(syscall, time_diff, note_id, battle_query);
                }
            }



        }
        else
        {
            const auto &end_time = note_comp->get<TimingEnd>().timing_end;
            const auto time_diff_end = current_time - end_time;

            // handle holding
            for (auto &[id4, comp4] : input_query) // check when stop holding
            {
                if ((lane_num == 0 && comp4.get<KeyInput>().key1_hold == false) ||
                (lane_num == 1 && comp4.get<KeyInput>().key2_hold == false) ||
                (lane_num == 2 && comp4.get<KeyInput>().key3_hold == false) ||
                (lane_num == 3 && comp4.get<KeyInput>().key4_hold == false))
                {
                    handle_hold_note_release(syscall, time_diff_end, note_id, battle_query);
                    break;
                }

                if (time_diff_end <= 0) // check for perfect end timing
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                    LOG_INFO("Timing %d: End Hold Perfect", end_time);

                    syscall.remove_entity(note_id);
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
        // Check key down in lane first. If it's tap note then we have to check again
        for (auto &[id, comp] : input_query)
        {
            if (comp.get<KeyInput>().key1_hold == true)
            {
                handle_note_from_lane(0, syscall, note_query, battle_query, input_query);
            }
            if (comp.get<KeyInput>().key2_hold == true)
            {
                handle_note_from_lane(1, syscall, note_query, battle_query, input_query);
            }
            if (comp.get<KeyInput>().key3_hold == true)
            {
                handle_note_from_lane(2, syscall, note_query, battle_query, input_query);
            }
            if (comp.get<KeyInput>().key4_hold == true)
            {
                handle_note_from_lane(3, syscall, note_query, battle_query, input_query);
            }
        }
    }
} // namespace Game::Rhythm
