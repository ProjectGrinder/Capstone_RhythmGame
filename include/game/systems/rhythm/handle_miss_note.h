#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void HandleMissNote(
        T &syscall,
        System::ECS::Query<Lane, Timing, TimingEnd, HoldActive> &note_query,
        System::ECS::Query<Battle::BattleState> &battle_query)
    {

        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
        {
            return;
        }

        auto &current_timing = battle_query.front().get<Battle::BattleState>().clock_time;

        for (auto &[id, comps] : note_query)
        {
            auto &note_time = comps.get<Timing>().timing;

            if (current_timing - note_time > 100)
            {
                if (comps.get<TimingEnd>().timing_end == 0) // tap note miss
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 1;
                    LOG_INFO("Timing %d Lane %d: Tap Miss", note_time, comps.get<Lane>().lane);
                }
                else if (comps.get<HoldActive>().hold_active == false) // hold note miss (not held at all)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.miss_count += 2;
                    LOG_INFO("Timing %d Lane %d: Hold Miss", note_time, comps.get<Lane>().lane);
                }
                syscall.remove_entity(id);
            }
        // Handle miss for an entire hold note, which counts as two misses (start and end)
        // If the note is held but released too early, it should be handled in handle_rhythm.h to be checked along with other judgements
        }
    }
}