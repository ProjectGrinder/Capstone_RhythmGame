#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void handle_miss_note(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Timing, HoldStart, NoteType, NoteStatus, Render::Sprite> &note_query,
            System::ECS::Query<Battle::BattleState> &battle_query,
            System::ECS::Query<Battle::RhythmState> &rhythm_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
        {
            return;
        }

        const auto &current_timing = battle_query.front().get<Battle::BattleState>().clock_time / 1000;
        const auto &accept_loss = rhythm_query.front().get<Battle::RhythmState>().accept_loss;

        for (auto &[id, comp]: note_query)
        {
            if (comp.get<NoteStatus>().state == -1)
                continue;

            auto &note_time = comp.get<Timing>().timing;

            if (current_timing - note_time >= 100 && comp.get<NoteType>().type != -1)
            {
                if (comp.get<HoldStart>().is_hold == false) // tap note miss
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
                    auto &type = comp.get<NoteType>().type;
                    if (type == 2)
                    {
                        battle_query.front().get<Battle::BattleState>().current_accept -= accept_loss.rain;
                    }
                    else if (type == 1)
                    {
                        battle_query.front().get<Battle::BattleState>().current_accept -= accept_loss.accent;
                    }
                    else if (type == 0)
                    {
                        battle_query.front().get<Battle::BattleState>().current_accept -= accept_loss.normal;
                    }
                    LOG_INFO("Timing %d Lane %d: Tap Miss", note_time, comp.get<Timing>().lane);
                }
                else // hold note miss (not held at all)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 2;
                    battle_query.front().get<Battle::BattleState>().current_accept -= accept_loss.hold;
                    LOG_INFO("Timing %d Lane %d: Hold Miss", note_time, comp.get<Timing>().lane);
                    int first_timing = 999999;
                    System::ECS::Query<Timing, HoldStart, NoteType, NoteStatus, Render::Sprite>::StoredTuple *note = nullptr;
                    for (auto &[id2, comp2]: note_query) // find ending note for the hold
                    {
                        if (comp2.get<NoteType>().type == -1 && comp2.get<NoteStatus>().state != -1
                            && comp.get<Timing>().lane == comp2.get<Timing>().lane && comp2.get<Timing>().timing < first_timing)
                        {
                            first_timing = comp2.get<Timing>().timing;
                            note = &comp2;
                        }
                    }
                    if (note != nullptr)
                    {
                        auto sp = load_sprite("img/rhythm/base_disabled.dds", "disabled", 200, 40);
                        note->get<Render::Sprite>().sp = sp;
                    }
                }
                comp.get<NoteStatus>().state = -1;
                LOG_INFO("Miss Count = %d", battle_query.front().get<Battle::BattleState>().judgement_count.miss_count);
            }
            else if (current_timing - note_time >= 0) // for hold end notes (stop rendering when time diff is 0)
            {
                comp.get<NoteStatus>().state = -1;
            }
            // Handle miss for an entire hold note, which counts as two misses (start and end)
            // If the note is held but released too early, it should be handled in handle_holding.h to be checked along
            // with other judgements
        }
    }
} // namespace Game::Rhythm
