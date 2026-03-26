#pragma once

#include "game/components.h"
#include "utils/print_debug.h"

namespace Game::Rhythm
{
    inline void handle_hold_note_release(
        const int &time_diff,
        System::ECS::Query<Lane>::StoredTuple *lane,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Timing, HoldStart, NoteType, NoteStatus> &note_query,
        System::ECS::Query<Battle::RhythmState> &rhythm_query)
    {
        const auto base_score = rhythm_query.front().get<Battle::RhythmState>().base_score;
        if (time_diff > -50)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
            battle_query.front().get<Battle::BattleState>().score += base_score / 2;
            LOG_INFO("Perfect");
        }
        else if (time_diff > -75)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
            battle_query.front().get<Battle::BattleState>().score += base_score / 4;
            LOG_INFO("Great");
        }
        else if (time_diff > -100)
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
            battle_query.front().get<Battle::BattleState>().score += base_score / 4;
            LOG_INFO("Fine");
        }
        else
        {
            battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
            battle_query.front().get<Battle::BattleState>().current_accept -= rhythm_query.front().get<Battle::RhythmState>().accept_loss.hold_end;
            LOG_INFO("Miss Count = %d", battle_query.front().get<Battle::BattleState>().judgement_count.miss_count);
        }

        for (auto &[id, comp] : note_query)
        {
            if (comp.get<NoteType>().type != -1)
                continue;

            if (comp.get<Timing>().timing == lane->get<Lane>().hold_end_time && comp.get<Timing>().lane == lane->get<Lane>().lane_number)
            {
                comp.get<NoteStatus>().state = 0;
                break;
            }
        }
        lane->get<Lane>().hold_active = false;
        lane->get<Lane>().hold_end_time = 0;
    }

    template<typename T>
    void handle_holding(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Battle::RhythmState> &rhythm_query,
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

        const auto key_input = input_query.front().get<KeyInput>();

        for (auto &[id, comp] : lane_query)
        {
            if (comp.get<Lane>().hold_active == true && comp.get<Lane>().hold_end_time > 0)
            {
                const auto current_timing = battle_query.front().get<Battle::BattleState>().clock_time / 1000;
                const auto time_end = comp.get<Lane>().hold_end_time;
                int time_diff = current_timing - time_end;
                if ((comp.get<Lane>().lane_number == 0 && key_input.key1_hold == false)
                    || (comp.get<Lane>().lane_number == 1 && key_input.key2_hold == false)
                    || (comp.get<Lane>().lane_number == 2 && key_input.key3_hold == false)
                    || (comp.get<Lane>().lane_number == 3 && key_input.key4_hold == false))
                {
                    handle_hold_note_release(time_diff, &comp, battle_query, note_query, rhythm_query);
                }

                if (time_diff >= 0)
                {
                    handle_hold_note_release(time_diff, &comp, battle_query, note_query, rhythm_query);
                }
            }
        }
    }
}