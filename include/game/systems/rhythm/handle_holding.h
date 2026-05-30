#pragma once

#include "game/components.h"
#include "update_judge_text.h"
#include "utils/print_debug.h"

namespace Game::Rhythm
{
    using Sprite = Render::Sprite;

    template<typename T>
    void handle_hold_note_release(
        [[maybe_unused]] T &syscall,
        const int &time_diff,
        System::ECS::Query<Lane>::StoredTuple *lane,
        System::ECS::Query<Battle::BattleState, Battle::RhythmState> &battle_query,
        System::ECS::Query<Timing, HoldStart, NoteType, NoteStatus, Render::Sprite> &note_query,
        System::ECS::Query<HoldConnect, NoteStatus, Sprite> &hold_query,
        System::ECS::Query<JudgeText, Render::Sprite, Render::Material> &judge_query)
    {
        constexpr auto perfect_judge = -100;
        constexpr auto great_judge = -250;
        // constexpr auto fine_judge = -100;

        const auto max_accept = battle_query.front().get<Battle::BattleState>().max_accept_gauge;
        const auto max_hp = battle_query.front().get<Battle::BattleState>().max_hp;
        const auto apn = battle_query.front().get<Battle::RhythmState>().apn;

        if (time_diff > perfect_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
            set_judge(PERFECT, judge_query);
            create_note_effect(syscall, lane->get<Lane>().lane_number, PERFECT);

            battle_query.front().get<Battle::BattleState>().current_accept += battle_query.front().get<Battle::RhythmState>().accept_gain / 2;
            if (battle_query.front().get<Battle::BattleState>().current_accept > max_accept)
                battle_query.front().get<Battle::BattleState>().current_accept = max_accept;

            // heal only perfect hold end
            battle_query.front().get<Battle::BattleState>().hp += battle_query.front().get<Battle::RhythmState>().heal_hp;
            if (battle_query.front().get<Battle::BattleState>().hp > max_hp)
                battle_query.front().get<Battle::BattleState>().hp = max_hp;
        }
        else if (time_diff > great_judge)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
            set_judge(GREAT, judge_query);
            create_note_effect(syscall, lane->get<Lane>().lane_number, GREAT);

            battle_query.front().get<Battle::BattleState>().current_accept += battle_query.front().get<Battle::RhythmState>().accept_gain / 2;
            if (battle_query.front().get<Battle::BattleState>().current_accept > max_accept)
                battle_query.front().get<Battle::BattleState>().current_accept = max_accept;
            battle_query.front().get<Battle::RhythmState>().accuracy -= apn / 4;
        }
        // else if (time_diff > fine_judge)
        // {
        //     battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
        //     battle_query.front().get<Battle::BattleState>().score += base_score / 4;
        // }
        else
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
            battle_query.front().get<Battle::BattleState>().current_accept -= battle_query.front().get<Battle::RhythmState>().accept_loss.hold_end;
            if (battle_query.front().get<Battle::BattleState>().current_accept < 0)
                battle_query.front().get<Battle::BattleState>().current_accept = 0;

            set_judge(MISS, judge_query);
            battle_query.front().get<Battle::RhythmState>().accuracy -= apn;

            for (auto &[id2, comp2] : hold_query)
            {
                if (comp2.get<HoldConnect>().timing_start == lane->get<Lane>().hold_start_time && comp2.get<HoldConnect>().timing_end == lane->get<Lane>().hold_end_time)
                {
                    const auto sp = get_assets_record_ptr(get_assets_id("hold_disabled"));
                    comp2.get<Sprite>().sp = sp;
                    comp2.get<NoteStatus>().state = -1;
                }
            }
            for (auto &[id, comp] : note_query)
            {
                if (comp.get<NoteType>().type != -1)
                    continue;

                if (comp.get<Timing>().timing == lane->get<Lane>().hold_end_time && comp.get<Timing>().lane == lane->get<Lane>().lane_number)
                {
                    const auto sp = get_assets_record_ptr(get_assets_id("disabled"));
                    comp.get<Render::Sprite>().sp = sp;
                }
            }
        }

        lane->get<Lane>().hold_start_time = 0;
        lane->get<Lane>().hold_end_time = 0;
    }

    template<typename T>
    void handle_holding(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Battle::BattleState, Battle::RhythmState> &battle_query,
        System::ECS::Query<KeyInput> &input_query,
        System::ECS::Query<Lane> &lane_query,
        System::ECS::Query<Timing, HoldStart, NoteType, NoteStatus, Render::Sprite> &note_query,
        System::ECS::Query<HoldConnect, NoteStatus, Sprite> &hold_query,
        System::ECS::Query<JudgeText, Render::Sprite, Render::Material> &judge_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
            return;

        if (input_query.begin() == input_query.end())
            return;

        const auto key_input = input_query.front().get<KeyInput>();

        for (auto &[id, comp] : lane_query)
        {
            if (comp.get<Lane>().hold_start_time > 0 && comp.get<Lane>().hold_end_time > 0)
            {
                const auto current_timing = battle_query.front().get<Battle::BattleState>().clock_time / 1000;
                const auto time_end = comp.get<Lane>().hold_end_time;
                int time_diff = current_timing - time_end;
                if ((comp.get<Lane>().lane_number == 0 && key_input.key1_hold == false)
                    || (comp.get<Lane>().lane_number == 1 && key_input.key2_hold == false)
                    || (comp.get<Lane>().lane_number == 2 && key_input.key3_hold == false)
                    || (comp.get<Lane>().lane_number == 3 && key_input.key4_hold == false))
                {
                    handle_hold_note_release(syscall, time_diff, &comp, battle_query, note_query, hold_query, judge_query);
                }

                if (time_diff >= 0)
                {
                    handle_hold_note_release(syscall, time_diff, &comp, battle_query, note_query, hold_query, judge_query);
                }
            }
        }
    }
}