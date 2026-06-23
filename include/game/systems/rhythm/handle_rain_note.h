#pragma once

#include "game/components.h"
#include "update_judge_text.h"

namespace Game::Rhythm
{
    using Material = Render::Material;

    template <typename T>
    void handle_rain_note(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState, Battle::RhythmState> &battle_query,
            System::ECS::Query<Input> &input_query,
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

        const auto key_input = input_query.front().get<Input>();

        constexpr auto catch_range = 50;

        for (auto &[id, comp] : note_query)
        {
            if (comp.get<NoteType>().type != 2 || comp.get<NoteStatus>().state == -1)
                continue;

            const int lane = comp.get<Timing>().lane;

            if ((lane == 0 && key_input.key1_held == true)
            || (lane == 1 && key_input.key2_held == true)
            || (lane == 2 && key_input.key3_held == true)
            || (lane == 3 && key_input.key4_held == true))
            {
                const auto time_diff = battle_query.front().get<Battle::BattleState>().clock_time / 1000 - comp.get<Timing>().timing;

                if (time_diff < catch_range && time_diff > -1 * catch_range)
                {
                    const auto max_accept = battle_query.front().get<Battle::BattleState>().max_accept_gauge;
                    const auto apn = battle_query.front().get<Battle::RhythmState>().apn;

                    battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                    set_judge(PERFECT, judge_query);
                    battle_query.front().get<Battle::RhythmState>().accuracy += apn.perfect;
                    create_note_effect(syscall, lane, PERFECT);

                    battle_query.front().get<Battle::BattleState>().hp += battle_query.front().get<Battle::RhythmState>().heal_hp;
                    battle_query.front().get<Battle::BattleState>().current_accept += battle_query.front().get<Battle::RhythmState>().accept_gain / 2;
                    if (battle_query.front().get<Battle::BattleState>().current_accept > max_accept)
                        battle_query.front().get<Battle::BattleState>().current_accept = max_accept;

                    auto sounds = sound_query.front().get<Audio::SoundRegistry>().audios;
                    Audio::audio_play(sounds["sound_rain_note"]);

                    comp.get<NoteStatus>().state = -1;
                    comp.get<Material>().visible = false;
                }
            }
        }
    }
}
