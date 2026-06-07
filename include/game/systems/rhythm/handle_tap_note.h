#pragma once

#include "game/components.h"
#include "update_judge_text.h"
#include "utils/print_debug.h"

namespace Game::Rhythm
{
    using Material = Render::Material;

    template<typename T>
    void handle_normal_note(
        [[maybe_unused]] T &syscall,
        const int &time_diff,
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *comp,
        System::ECS::Query<Lane>::StoredTuple *lane,
        System::ECS::Query<Battle::BattleState, Battle::RhythmState> &battle_query,
        System::ECS::Query<JudgeText, Render::Sprite, Render::Material> &judge_query,
        const Audio::SoundRegistry &sound_registry)
    {
        constexpr auto perfect_judge = 50;
        constexpr auto great_judge = 75;
        constexpr auto fine_judge = 100;

        // const auto timing = battle_query.front().get<Battle::BattleState>().clock_time;
        int heal_amount = battle_query.front().get<Battle::RhythmState>().heal_hp;
        int accept = battle_query.front().get<Battle::RhythmState>().accept_gain;
        const auto apn = battle_query.front().get<Battle::RhythmState>().apn;
        if (time_diff > -1 * perfect_judge && time_diff < perfect_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
            set_judge(PERFECT, judge_query);
            battle_query.front().get<Battle::RhythmState>().accuracy += apn;
            create_note_effect(syscall, lane->get<Lane>().lane_number, PERFECT);
        }
        else if (time_diff > -1 * great_judge && time_diff < great_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
            set_judge(GREAT, judge_query);
            battle_query.front().get<Battle::RhythmState>().accuracy += apn * 3/4;
            create_note_effect(syscall, lane->get<Lane>().lane_number, GREAT);
        }
        else if (time_diff > -1 * fine_judge && time_diff < fine_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
            set_judge(FINE, judge_query);
            accept = accept / 2;
            heal_amount = 0;
            battle_query.front().get<Battle::RhythmState>().accuracy += apn / 2;
            create_note_effect(syscall, lane->get<Lane>().lane_number, FINE);
        }
        else return;

        const auto max_accept = battle_query.front().get<Battle::BattleState>().max_accept_gauge;
        const auto max_hp = battle_query.front().get<Battle::BattleState>().max_hp;

        battle_query.front().get<Battle::BattleState>().hp += heal_amount;
        if (battle_query.front().get<Battle::BattleState>().hp > max_hp)
            battle_query.front().get<Battle::BattleState>().hp = max_hp;

        battle_query.front().get<Battle::BattleState>().current_accept += accept;
        if (battle_query.front().get<Battle::BattleState>().current_accept > max_accept)
            battle_query.front().get<Battle::BattleState>().current_accept = max_accept;

        if (comp->get<HoldStart>().is_hold == true)
        {
            lane->get<Lane>().hold_start_time = comp->get<Timing>().timing; // start holding
            LOG_INFO("Start Holding");
        }
        auto sounds = sound_registry.audios;
        Audio::audio_play(sounds["sound_note_hit"]);
        comp->get<NoteStatus>().state = -1; // remove tap notes
        comp->get<Material>().visible = false;
    }

    template<typename T>
    void handle_accent_note(
        [[maybe_unused]] T &syscall,
        const int &time_diff,
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *comp,
        System::ECS::Query<Lane>::StoredTuple *lane,
        System::ECS::Query<Battle::BattleState, Battle::RhythmState> &battle_query,
        System::ECS::Query<JudgeText, Render::Sprite, Render::Material> &judge_query,
        const Audio::SoundRegistry &sound_registry)
    {

        constexpr auto hit_judge = 100;
        const auto apn = battle_query.front().get<Battle::RhythmState>().apn;

        if (time_diff > -1 * hit_judge && time_diff < hit_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
            set_judge(PERFECT, judge_query);
            battle_query.front().get<Battle::RhythmState>().accuracy += apn;
            create_note_effect(syscall, lane->get<Lane>().lane_number, PERFECT);
        }
        else return;

        const auto max_accept = battle_query.front().get<Battle::BattleState>().max_accept_gauge;
        const auto max_hp = battle_query.front().get<Battle::BattleState>().max_hp;

        battle_query.front().get<Battle::BattleState>().hp += battle_query.front().get<Battle::RhythmState>().heal_hp;
        if (battle_query.front().get<Battle::BattleState>().hp > max_hp)
            battle_query.front().get<Battle::BattleState>().hp = max_hp;

        battle_query.front().get<Battle::BattleState>().current_accept += battle_query.front().get<Battle::RhythmState>().accept_gain;
        if (battle_query.front().get<Battle::BattleState>().current_accept > max_accept)
            battle_query.front().get<Battle::BattleState>().current_accept = max_accept;

        if (comp->get<HoldStart>().is_hold == true)
        {
            lane->get<Lane>().hold_start_time = comp->get<Timing>().timing; // start holding
            LOG_INFO("Start Holding");
        }
        auto sounds = sound_registry.audios;
        Audio::audio_play(sounds["sound_note_hit"]);
        Audio::audio_play(sounds["sound_accent_effect"]);
        comp->get<NoteStatus>().state = -1; // remove tap notes
        comp->get<Material>().visible = false;
    }

    template<typename T>
    void handle_note_from_lane(
        [[maybe_unused]] T &syscall,
        const int &lane_num, // assume that first lane is 0
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *comp,
        System::ECS::Query<Battle::BattleState, Battle::RhythmState> &battle_query,
        System::ECS::Query<Lane> &lane_query,
        System::ECS::Query<JudgeText, Render::Sprite, Render::Material> &judge_query,
        const Audio::SoundRegistry &sound_registry)
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
                    handle_accent_note(syscall, time_diff, comp, lane, battle_query, judge_query, sound_registry);
                }
                else if (note_type == 0)
                {
                    handle_normal_note(syscall, time_diff, comp, lane, battle_query, judge_query, sound_registry);
                }
                break;
            }
        }
    }

    template<typename T>
    void handle_tap_note(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState, Battle::RhythmState> &battle_query,
            System::ECS::Query<KeyInput> &input_query,
            System::ECS::Query<Lane> &lane_query,
            System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus> &note_query,
            System::ECS::Query<JudgeText, Render::Sprite, Render::Material> &judge_query,
            System::ECS::Query<Audio::SoundRegistry> &sound_query)
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
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *note_comp1 = nullptr;
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *note_comp2 = nullptr;
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *note_comp3 = nullptr;
        System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus>::StoredTuple *note_comp4 = nullptr;

        auto sound_registry = sound_query.front().get<Audio::SoundRegistry>();

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
            handle_note_from_lane(syscall, 0, note_comp1, battle_query, lane_query, judge_query, sound_registry);
        }
        if (key_input.key2_pressed == true && note_comp2 != nullptr)
        {
            handle_note_from_lane(syscall, 1, note_comp2, battle_query, lane_query, judge_query, sound_registry);
        }
        if (key_input.key3_pressed == true && note_comp3 != nullptr)
        {
            handle_note_from_lane(syscall, 2, note_comp3, battle_query, lane_query, judge_query, sound_registry);
        }
        if (key_input.key4_pressed == true && note_comp4 != nullptr)
        {
            handle_note_from_lane(syscall, 3, note_comp4, battle_query, lane_query, judge_query, sound_registry);
        }
    }
} // namespace Game::Rhythm
