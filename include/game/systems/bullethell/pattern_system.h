#pragma once
#include "game/components.h"
#include "game/utils/physics_util.h"

// FIXME: Part of physics, consider separation into its own thread
namespace Game::BulletHell
{
    template<typename T>
    void PatternSystem(
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
            auto &patt_c = comps.get<Patterns>();

            auto &patterns = patt_c.patterns;

            for (auto itr = patterns.begin(); itr != patterns.end(); )
            {
                // Execute
                if (itr->first <= 0)
                {
                    const auto pattern = itr->second;
                    comps.get<Velocity>().vx = pattern.speed != UNASSIGNED ? pattern.speed: comps.get<Velocity>().vx;
                    comps.get<Rotation>().angleZ = pattern.angle != UNASSIGNED ? pattern.angle: comps.get<Rotation>().angleZ;
                    comps.get<Acceleration>().ax = pattern.accel != UNASSIGNED ? pattern.accel: comps.get<Acceleration>().ax;
                    comps.get<AngularVelocity>().v = pattern.angular_velocity != UNASSIGNED ? pattern.angular_velocity: comps.get<AngularVelocity>().v;
                    comps.get<Acceleration>().max_speed_x = pattern.max_speed != UNASSIGNED ? pattern.max_speed: comps.get<Acceleration>().max_speed_x;
                    if (pattern.loopDelay <= 0)
                        itr = patterns.erase(itr);
                    else
                    {
                        itr->first = pattern.loopDelay;
                        ++itr;
                    }
                }
                else
                {
                    itr->first -= frame_time;
                    ++itr;
                }
            }
        }
    }
}