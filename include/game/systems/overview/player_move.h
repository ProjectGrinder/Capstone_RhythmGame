#pragma once

namespace Game::Overview
{
    template<typename T>
    void player_move(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<BulletHell::Input> &query1,
            System::ECS::Query<Player, Physics::Velocity, Acceleration> &query2,
            System::ECS::Query<PlayerStat> &query3)
    {
        if (query1.begin() == query1.end())
            return;

        if (query3.begin() == query3.end())
            return;

        const auto player_stat = query3.front().components.get<PlayerStat>();

        for (auto &[id, comps] : query2)
        {
            const auto &input = query1.front().get<BulletHell::Input>();
            const float velocity_factor = (input.shift ? player_stat.run_speed : player_stat.walk_speed);

            const float x = input.axis_x;

            comps.get<Physics::Velocity>().vx = x * velocity_factor;

            if (input.z && comps.get<Player>().on_ground)
            {
                comps.get<Physics::Velocity>().vy = player_stat.jump_height;
            }

            if (!comps.get<Player>().on_ground)
                comps.get<Acceleration>().ay = -5.f;
            else
                comps.get<Acceleration>().ay = 0;
        }
    }
} // namespace Game::Overview
