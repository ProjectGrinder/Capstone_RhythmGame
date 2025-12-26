#pragma once
#include "game/components.h"
#include "game/components/physics/acceleration.h"

// FIXME: Part of physics, consider separation into its own thread
namespace Game::BulletHell
{
    using Position = Physics::Position;
    using Velocity = Physics::Velocity;
    using Rotation = Physics::Rotation;
    using Acceleration = Physics::Acceleration;
    using AngularVelocity = Physics::AngularVelocity;

    template <typename T>
    void PatternSystem([[maybe_unused]] T &syscall, System::ECS::Query<Patterns, Position, Rotation, Velocity, Acceleration, AngularVelocity>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        constexpr auto frame_time = 1;

        for (auto &[id, comps] : query)
        {
            auto &patterns = comps.get<Patterns>().patterns;
            for (auto itr = patterns.begin(); itr != patterns.end();)
            {
                if (itr->first < frame_time)
                {
                    auto pattern = itr->second;
                    comps.get<Velocity>().vx = pattern.velocity.vx;
                    comps.get<Velocity>().vy = pattern.velocity.vy;
                    comps.get<Rotation>().angle = pattern.angle;
                    comps.get<Acceleration>().ax = pattern.acceleration.ax;
                    comps.get<Acceleration>().ay = pattern.acceleration.ay;
                    comps.get<AngularVelocity>().v = pattern.angular_velocity;
                }
                else
                {
                    itr->first -= frame_time;
                }
            }
        }
    }
}