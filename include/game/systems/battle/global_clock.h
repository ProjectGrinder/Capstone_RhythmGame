#pragma once

#include "utils/print_debug.h"
extern "C" long double get_delta_time();
namespace Game
{

    template<typename T>
    void update_global_clock(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<World::GlobalState> &world_query,
        System::ECS::Query<Battle::BattleState> &battle_query)
    {
        int time = 0;

        if (world_query.begin() != world_query.end())
        {
            world_query.front().get<World::GlobalState>().clock_time += static_cast<int>(get_delta_time() * 1000);
            time = world_query.front().get<World::GlobalState>().clock_time / 1000;
        }

        if (battle_query.begin() != battle_query.end())
        {
            battle_query.front().get<Battle::BattleState>().clock_time += static_cast<int>(get_delta_time() * 1000);
            time = battle_query.front().get<Battle::BattleState>().clock_time / 1000;
        }


        if (time % 1000 == 0)
        LOG_INFO("Time: %d s", time / 1000);
    }
}