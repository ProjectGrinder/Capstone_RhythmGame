#pragma once
#include "battlestate.h"

namespace Game::Battle
{
    struct BattleObject
    {
        CurrentPhase phase;
        float start_alpha = 1.f;
    };
} // namespace Game::Battle
