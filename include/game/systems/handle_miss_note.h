#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void HandleMissNote(T &task_manager, System::ECS::Query<Lane, NoteSpeed, Timing> &query, System::ECS::Query<Lane, NoteSpeed, Timing, TimingEnd> &query2)
    {
        constexpr auto miss_timing = 500;
        for (auto &[id, comps] : query)
        {
            if (comps.get<Timing>().timing < -1 * miss_timing)
            {
                task_manager.delete_entity(id);
            }
        }

        for (auto &[id, comps] : query2)
        {
            if (comps.get<TimingEnd>().timing_end < -1 * miss_timing)
            {
                task_manager.delete_entity(id);
            }
        }
    }
}