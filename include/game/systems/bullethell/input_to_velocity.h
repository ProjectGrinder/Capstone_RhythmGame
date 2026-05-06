#pragma once
#include "game/systems/global_clock.h"
#include "utils/constant.h"

namespace Game::BulletHell
{
    template<typename T>
    void input_to_velocity(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Input> &query1,
            System::ECS::Query<Player, Physics::Velocity, Render::Transform> &query2,
            System::ECS::Query<Battle::BattleState> &query3)
    {
        if (query1.begin() == query1.end())
            return;

        if (query3.begin() == query3.end())
            return;

        if (query3.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        for (auto &[id, comps] : query2)
        {
            if (!comps.get<Player>().is_active) return;

            const auto &input = query1.front().get<Input>();
            const float velocity_factor = (input.shift ? 200.f : 500.f);

            float x = input.axis_x;
            float y = input.axis_y;

            if (const float length = sqrtf(x * x + y * y); length != 0.0f)
            {
                x/=length;
                y/=length;
            }

            comps.get<Physics::Velocity>().vx = x * velocity_factor;
            comps.get<Physics::Velocity>().vy = y * velocity_factor;

            // Prevent OoB
            auto &tra = comps.get<Render::Transform>();
            if (tra.position.x > BOX_SIZE*4/5) tra.position.x = BOX_SIZE*4/5;
            else if (tra.position.x < -BOX_SIZE*4/5) tra.position.x = -BOX_SIZE*4/5;
            if (tra.position.y > BOX_SIZE*4/5 - HALF_HEIGHT*1/3) tra.position.y = BOX_SIZE*4/5 - HALF_HEIGHT*1/3;
            else if (tra.position.y < -BOX_SIZE*4/5 - HALF_HEIGHT*1/3) tra.position.y = -BOX_SIZE*4/5 - HALF_HEIGHT*1/3;
        }
    }
} // namespace Game::BulletHell
