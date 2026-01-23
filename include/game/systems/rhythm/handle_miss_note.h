#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void HandleMissNote(
        T &syscall,
        System::ECS::Query<Lane, NoteSpeed, Timing, TimingEnd> &note_query,
        System::ECS::Query<Battle::BattleState> &battle_query)
    {

        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
        {
            return;
        }

        constexpr auto miss_timing = 100;
        auto &current_timing = battle_query.front().get<Battle::BattleState>().clock_time;

        for (auto &[id, comps] : note_query)
        {
            if (comps.get<Timing>().timing - current_timing < -1 * miss_timing)
            {
                if (comps.get<TimingEnd>().timing_end == 0)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
                }
                else
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 2;
                }
                syscall.remove_entity(id);
            }
        // Handle miss for an entire hold note, which counts as two misses (start and end)
        // If the note is held but released too early, it should be handled in handle_rhythm.h to be checked along with other judgements
        }
    }
}