#pragma once

namespace Game::Overview
{
    template<typename T>
    void player_move(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<GlobalState> &global_query,
            System::ECS::Query<Input> &query1,
            System::ECS::Query<Player, Velocity, Acceleration> &query2,
            System::ECS::Query<PlayerStat> &query3)
    {
        if (query1.begin() == query1.end())
            return;

        if (query3.begin() == query3.end())
            return;

        const auto &player_stat = query3.front().components.get<PlayerStat>();
        const auto &input = query1.front().get<Input>();
        const auto &global_state = global_query.front().components.get<GlobalState>();

        for (auto &[id, comps] : query2)
        {
            if (!comps.get<Player>().on_ground)
                comps.get<Acceleration>().ay = -0.098f;
            else
                comps.get<Acceleration>().ay = 0;

            if (global_state.movementLocked) continue;

            const float velocity_factor = (input.shift ? player_stat.run_speed : player_stat.walk_speed);

            const float x = input.axis_x;

            comps.get<Velocity>().vx = x * velocity_factor;

            if (input.z_pressed && comps.get<Player>().on_ground)
            {
                LOG_INFO("Player Jump");
                comps.get<Velocity>().vy = player_stat.jump_height;
            }


        }
    }
} // namespace Game::Overview
