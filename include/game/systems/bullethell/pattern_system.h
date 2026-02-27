#pragma once
#include "game/components.h"

// FIXME: Part of physics, consider separation into its own thread
namespace Game::BulletHell
{
    template<typename T>
    void PatternSystem(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState> &query2)
    {

    }
}