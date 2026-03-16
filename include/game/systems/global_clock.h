#pragma once

#include "utils/print_debug.h"

extern "C" long double get_delta_time();

namespace Game::Battle
{
    template<typename T>
    void update_global_clock(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState> &battle_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        battle_query.front().get<BattleState>().clock_time += static_cast<int>(get_delta_time() * 1000);

        auto time = battle_query.front().get<BattleState>().clock_time / 1000;
        if (time % 1000 < 3)
        LOG_INFO("Time: %d s", time / 1000);
    }
}