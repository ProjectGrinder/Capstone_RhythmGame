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

        constexpr auto miss_timing = 500;
        for (auto &[id, comps] : query)
        {
            if (comps.get<Timing>().timing < -1 * miss_timing)
            {
                syscall.remove_entity(id);
            }
        }

        for (auto &[id, comps] : query2)
        {
            if (comps.get<TimingEnd>().timing_end < -1 * miss_timing)
            {
                syscall.remove_entity(id);
            }
        }
    }
}