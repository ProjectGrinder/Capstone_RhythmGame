#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void HandleMissNote(T &syscall, System::ECS::Query<Lane, NoteSpeed, Timing> &query, System::ECS::Query<Lane, NoteSpeed, Timing, TimingEnd> &query2, System::ECS::Query<Battle::BattleState> &query3)
    {

        if (query3.begin() == query3.end())
            return;

        if (query3.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
        {
            return;
        }

        constexpr auto miss_timing = 100;
        for (auto &[id, comps] : query)
        {
            if (comps.get<Timing>().timing < -1 * miss_timing)
            {
                query3.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
                syscall.remove_entity(id);
            }
        }

        for (auto &[id, comps] : query2)
        // Handle miss for an entire hold note, which counts as two misses (start and end)
        // If the note is held but released too early, it should be handled in handle_rhythm.h to be checked along with other judgements
        {
            if (comps.get<Timing>().timing_end < -1 * miss_timing)
            {
                query3.front().get<Battle::BattleState>().judgement_count.miss_count += 2;
                syscall.remove_entity(id);
            }
        }
    }
}