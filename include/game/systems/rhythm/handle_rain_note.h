#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    using Material = Render::Material;

    template <typename T>
    void handle_rain_note(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState> &battle_query,
            System::ECS::Query<Battle::RhythmState> &rhythm_query,
            System::ECS::Query<KeyInput> &input_query,
            System::ECS::Query<Material, Timing, HoldStart, NoteType, NoteStatus> &note_query,
            System::ECS::Query<JudgeText> &judge_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
            return;

        if (input_query.begin() == input_query.end())
            return;

        const auto key_input = input_query.front().get<KeyInput>();

        constexpr auto catch_range = 50;

        for (auto &[id, comp] : note_query)
        {
            if (comp.get<NoteType>().type != 2 || comp.get<NoteStatus>().state == -1)
                continue;

            if ((comp.get<Timing>().lane == 0 && key_input.key1_hold == true)
            || (comp.get<Timing>().lane == 1 && key_input.key2_hold == true)
            || (comp.get<Timing>().lane == 2 && key_input.key3_hold == true)
            || (comp.get<Timing>().lane == 3 && key_input.key4_hold == true))
            {
                const auto time_diff = battle_query.front().get<Battle::BattleState>().clock_time / 1000 - comp.get<Timing>().timing;

                if (time_diff < catch_range && time_diff > -1 * catch_range)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                    battle_query.front().get<Battle::BattleState>().score += rhythm_query.front().get<Battle::RhythmState>().base_score / 2;
                    judge_query.front().get<JudgeText>().judge = JudgeText::PERFECT;
                    judge_query.front().get<JudgeText>().change = true;
                    battle_query.front().get<Battle::BattleState>().combo += 1;
                    comp.get<NoteStatus>().state = -1;
                    comp.get<Material>().visible = false;
                }
            }
        }
    }
}
