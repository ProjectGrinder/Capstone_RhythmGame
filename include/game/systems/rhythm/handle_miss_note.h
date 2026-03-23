#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void handle_miss_note(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Lane, Timing, TimingEnd, HoldActive, NoteType, NoteStatus> &note_query,
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
            auto &note_time = comp.get<Timing>().timing;

            if (current_timing - note_time >= 100)
            {
                if (comp.get<TimingEnd>().timing_end == 0) // tap note miss
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
                    auto &type = note_query.front().get<NoteType>().type;
                    if (type == 2)
                    {
                        battle_query.front().get<Battle::BattleState>().current_accept -= accept_loss.rain;
                    }
                    else
                    {
                        battle_query.front().get<Battle::BattleState>().current_accept -= accept_loss.normal;
                    }
                    LOG_INFO("Timing %d Lane %d: Tap Miss", note_time, comp.get<Lane>().lane);
                    LOG_INFO("Miss Count = %d", battle_query.front().get<Battle::BattleState>().judgement_count.miss_count);
                }
                else if (comp.get<HoldActive>().hold_active == false) // hold note miss (not held at all)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 2;
                    battle_query.front().get<Battle::BattleState>().current_accept -= accept_loss.normal;
                    LOG_INFO("Timing %d Lane %d: Hold Miss", note_time, comp.get<Lane>().lane);
                    LOG_INFO("Miss Count = %d", battle_query.front().get<Battle::BattleState>().judgement_count.miss_count);
                }
                else continue; // for hold notes that are still active

                comp.get<NoteStatus>().state = 0;
            }
            // Handle miss for an entire hold note, which counts as two misses (start and end)
            // If the note is held but released too early, it should be handled in handle_rhythm.h to be checked along
            // with other judgements
        }
    }
} // namespace Game::Rhythm
