#pragma once

#include "game/components.h"
#include "utils/print_debug.h"

namespace Game::Rhythm
{
    using Material = Render::Material;

    inline void handle_normal_note(
        const int &time_diff,
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *comp,
        System::ECS::Query<Lane>::StoredTuple *lane,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Battle::RhythmState> &rhythm_query,
        System::ECS::Query<JudgeText> &judge_query)
    {
        constexpr auto perfect_judge = 50;
        constexpr auto great_judge = 75;
        constexpr auto fine_judge = 100;
        auto base_score = rhythm_query.front().get<Battle::RhythmState>().base_score;

        // const auto timing = battle_query.front().get<Battle::BattleState>().clock_time;
        if (time_diff > -1 * perfect_judge && time_diff < perfect_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
            battle_query.front().get<Battle::BattleState>().score += base_score;
            judge_query.front().get<JudgeText>().judge = JudgeText::PERFECT;
            judge_query.front().get<JudgeText>().change = true;
        }
        else if (time_diff > -1 * great_judge && time_diff < great_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
            battle_query.front().get<Battle::BattleState>().score += base_score / 2;
            judge_query.front().get<JudgeText>().judge = JudgeText::GREAT;
            judge_query.front().get<JudgeText>().change = true;
        }
        else if (time_diff > -1 * fine_judge && time_diff < fine_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
            battle_query.front().get<Battle::BattleState>().score += base_score / 4;
            judge_query.front().get<JudgeText>().judge = JudgeText::FINE;
            judge_query.front().get<JudgeText>().change = true;
        }
        else return;

        if (comp->get<HoldStart>().is_hold == true)
        {
            lane->get<Lane>().hold_start_time = comp->get<Timing>().timing; // start holding
            LOG_INFO("Start Holding");
        }
            comp->get<NoteStatus>().state = -1; // remove tap notes
            comp->get<Material>().visible = false;
    }

    inline void handle_accent_note(
        const int &time_diff,
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *comp,
        System::ECS::Query<Lane>::StoredTuple *lane,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Battle::RhythmState> &rhythm_query,
        System::ECS::Query<JudgeText> &judge_query)
    {
        auto base_score = rhythm_query.front().get<Battle::RhythmState>().base_score;

        constexpr auto hit_judge = 100;

        if (time_diff > -1 * hit_judge && time_diff < hit_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
            battle_query.front().get<Battle::BattleState>().score += base_score;
            judge_query.front().get<JudgeText>().judge = JudgeText::PERFECT;
            judge_query.front().get<JudgeText>().change = true;
        }
        else return;

        if (comp->get<HoldStart>().is_hold == true)
        {
            lane->get<Lane>().hold_start_time = comp->get<Timing>().timing; // start holding
            LOG_INFO("Start Holding");
        }
        comp->get<NoteStatus>().state = -1; // remove tap notes
        comp->get<Material>().visible = false;
    }

    inline void handle_note_from_lane(
        const int lane_num, // assume that first lane is 0
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *comp,
        [[maybe_unused]] System::ECS::Query<Battle::BattleState> &battle_query,
        [[maybe_unused]] System::ECS::Query<KeyInput> &input_query,
        System::ECS::Query<Lane> &lane_query,
        [[maybe_unused]] System::ECS::Query<Battle::RhythmState> &rhythm_query,
        System::ECS::Query<JudgeText> &judge_query)
    {
        if (comp == nullptr) return;

        const auto &current_time = battle_query.front().get<Battle::BattleState>().clock_time / 1000;
        const auto &note_time = comp->get<Timing>().timing;
        const auto note_type = comp->get<NoteType>().type;
        System::ECS::Query<Lane>::StoredTuple *lane = nullptr;

        for (auto &[id2, comp2] : lane_query)
        {
            if (comp2.get<Lane>().lane_number == lane_num)
            {
                lane = &comp2;
                const auto time_diff = current_time - note_time;

                if (time_diff <= -100 || time_diff >= 100)
                {
                    return;
                }
                if (note_type == 1)
                {
                    handle_accent_note(time_diff, comp, lane, battle_query, rhythm_query, judge_query);
                }
                else if (note_type == 0)
                {
                    handle_normal_note(time_diff, comp, lane, battle_query, rhythm_query, judge_query);
                }
                break;
            }
        }
    }

    template<typename T>
    void handle_tap_note(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState> &battle_query,
            System::ECS::Query<Battle::RhythmState> &rhythm_query,
            System::ECS::Query<KeyInput> &input_query,
            [[maybe_unused]] System::ECS::Query<Lane> &lane_query,
            [[maybe_unused]] System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus> &note_query,
            System::ECS::Query<JudgeText> &judge_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
            return;

        if (input_query.begin() == input_query.end())
            return;

        // This system should check the input, remove a note if it was hit correctly, and update score/hp/etc. correctly
        // If you need to adjust components or data structures you can ask what structures implement what data to
        // Midfield

        const auto key_input = input_query.front().get<KeyInput>();

        if (key_input.key1_pressed == false && key_input.key2_pressed == false
            && key_input.key3_pressed == false && key_input.key4_pressed == false)
            return;

        // Create variable
        int first_timing1 = 999999;
        int first_timing2 = 999999;
        int first_timing3 = 999999;
        int first_timing4 = 999999;
        // System::ECS::pid note_id1 = 0;
        // System::ECS::pid note_id2 = 0;
        // System::ECS::pid note_id3 = 0;
        // System::ECS::pid note_id4 = 0;
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *note_comp1 = nullptr;
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *note_comp2 = nullptr;
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *note_comp3 = nullptr;
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *note_comp4 = nullptr;

        // Check first note of each lane where input is true
        for (auto &[id, comp] : note_query)
        {
            if (comp.get<NoteStatus>().state == -1)
                continue;

            if (comp.get<NoteType>().type == -1 || comp.get<NoteType>().type == 2)
                continue;

            const int first_timing = comp.get<Timing>().timing;

            if (comp.get<Timing>().lane == 0 && first_timing < first_timing1)
            {
                first_timing1 = first_timing;
                note_comp1 = &comp;
            }
            if (comp.get<Timing>().lane == 1 && first_timing < first_timing2)
            {
                first_timing2 = first_timing;
                note_comp2 = &comp;
            }
            if (comp.get<Timing>().lane == 2 && first_timing < first_timing3)
            {
                first_timing3 = first_timing;
                note_comp3 = &comp;
            }
            if (comp.get<Timing>().lane == 3 && first_timing < first_timing4)
            {
                first_timing4 = first_timing;
                note_comp4 = &comp;
            }
        }

        // Check if note is in hit range
        if (key_input.key1_pressed == true && note_comp1 != nullptr)
        {
            handle_note_from_lane(0, note_comp1, battle_query, input_query, lane_query, rhythm_query, judge_query);
        }
        if (key_input.key2_pressed == true && note_comp2 != nullptr)
        {
            handle_note_from_lane(1, note_comp2, battle_query, input_query, lane_query, rhythm_query, judge_query);
        }
        if (key_input.key3_pressed == true && note_comp3 != nullptr)
        {
            handle_note_from_lane(2, note_comp3, battle_query, input_query, lane_query, rhythm_query, judge_query);
        }
        if (key_input.key4_pressed == true && note_comp4 != nullptr)
        {
            handle_note_from_lane(3, note_comp4, battle_query, input_query, lane_query, rhythm_query, judge_query);
        }
    }
} // namespace Game::Rhythm
