#pragma once

namespace Game::BulletHell
{
    template <typename T>
    void InputToVelocity([[maybe_unused]] T &syscall, System::ECS::Query<Input> &query1, System::ECS::Query<Player, Physics::Velocity> &query2)
    {

        for (auto &[id, comps] : query2)
        {
            const auto &input = query1.front().get<Input>();
            const float velocity_factor = input.shift ? 10.0f : 1.0f;

            comps.get<Physics::Velocity>().vx = input.axis_x * velocity_factor;
            comps.get<Physics::Velocity>().vy = input.axis_y * velocity_factor;
        }
    }
}