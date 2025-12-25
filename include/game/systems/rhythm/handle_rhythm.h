#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void HandleRhythm([[maybe_unused]] T &syscall, System::ECS::Query<Battle::BattleState> &query1)
    {
        if (query1.begin() == query1.end())
            return;

        if (query1.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
        {
            return;
        }

        // intentionally left blank to await the rest of the implementation

    }
}