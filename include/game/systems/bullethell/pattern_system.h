#pragma once
#include "game/components.h"
#include "game/utils/physics_util.h"

// FIXME: Part of physics, consider separation into its own thread
namespace Game::BulletHell
{
    using Position = Physics::Position;
    using Velocity = Physics::Velocity;
    using Rotation = Physics::Rotation;
    using Acceleration = Physics::Acceleration;
    using AngularVelocity = Physics::AngularVelocity;

    template<typename T>
    void pattern_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Patterns, Rotation, Velocity, Acceleration, AngularVelocity> &query,
            System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        constexpr auto frame_time = 1;

        for (auto &[id, comps]: query)
        {
            auto &patterns = comps.get<Patterns>().patterns;
            for (auto itr = patterns.begin(); itr != patterns.end();)
            {
                if (itr->first < frame_time)
                {
                    const auto pattern = itr->second;
                    comps.get<Velocity>().vx = pattern.speed;
                    comps.get<Velocity>().vy = pattern.speed;
                    comps.get<Rotation>().angleZ = pattern.angle;
                    comps.get<Acceleration>().ax = pattern.accel;
                    comps.get<Acceleration>().ay = pattern.accel;
                    comps.get<AngularVelocity>().v = pattern.angular_velocity;
                    if (pattern.loopDelay <= 0)
                        itr = patterns.erase(itr);
                    else
                    {
                        itr->first = pattern.loopDelay;
                    }
                }
                else
                {
                    itr->first -= frame_time;
                }
            }
        }
    }

    template<typename T>
    void bounce_pattern_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Bounce, Position, Rotation> &query,
            System::ECS::Query<Battle::BattleState> &query2)
    {
        // TODO: Replace with real bound
        constexpr float stage_bound_x_min = 0;
        constexpr float stage_bound_x_max = 640;
        constexpr float stage_bound_y_min = 0;
        constexpr float stage_bound_y_max = 480;


        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;


        for (auto &[id, comps]: query)
        {
            const auto &pos = comps.get<Position>();
            auto &rot = comps.get<Rotation>();
            const auto &bounce_c = comps.get<Bounce>();

            if (bounce_c.reflect_side[0] && pos.x < stage_bound_x_min ||
                bounce_c.reflect_side[1] && pos.x > stage_bound_x_max)
                rot.angleZ = 180 - rot.angleZ;

            else if (
                    bounce_c.reflect_side[2] && pos.y < stage_bound_y_min ||
                    bounce_c.reflect_side[3] && pos.y > stage_bound_y_max)
                rot.angleZ = 360 - rot.angleZ;
        }
    }

    template<typename T>
    void homing_pattern_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Homing, Position, Rotation> &query,
            System::ECS::Query<Player, Position> &query2,
            System::ECS::Query<Battle::BattleState> &query3)
    {
        if (query2.begin() == query2.end())
            return;

        if (query3.begin() == query3.end())
            return;

        if (query3.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        for (auto &[id, comps]: query)
        {
            const auto &pos = comps.get<Position>();
            auto &rot = comps.get<Rotation>();
            const auto &homing_c = comps.get<Homing>();
            Position target_pos;
            if (homing_c.targetPlayer)
            {
                target_pos = query2.front().get<Position>();
            }
            else
            {
                target_pos = homing_c.targetPosition;
            }

            float target_angle = Physics::get_direction(pos, target_pos);
            if (abs(target_angle - rot.angleZ) < homing_c.strength)
            {
                rot.angleZ = target_angle;
            }
            else
            {
                int dist = static_cast<int>(target_angle - rot.angleZ + 540) % 360 - 180;
                dist = dist > 0 ? -1 : 1;
                rot.angleZ = rot.angleZ + homing_c.strength * static_cast<float>(dist);
            }
        }
    }

} // namespace Game::BulletHell
