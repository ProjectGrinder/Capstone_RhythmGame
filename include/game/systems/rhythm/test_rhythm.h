#pragma once
#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void test_rhythm(
        [[maybe_unused]] const T& syscall,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<KeyInput> &input_query)
    {}
}
