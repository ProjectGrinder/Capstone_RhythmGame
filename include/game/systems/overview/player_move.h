#pragma once

namespace Game::Overview
{
    template<typename T>
    void player_move(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<BulletHell::Input> &query1,
            System::ECS::Query<Player, Physics::Velocity> &query2,
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

            float x = input.axis_x;
            float y = input.axis_y;
            if (const float length = sqrtf(x * x + y * y); length < 0.0f)
            {
                x/=length;
                y/=length;
            }

            comps.get<Physics::Velocity>().vx = x * velocity_factor;
            comps.get<Physics::Velocity>().vy = y * velocity_factor;

        }
    }
} // namespace Game::BulletHell
