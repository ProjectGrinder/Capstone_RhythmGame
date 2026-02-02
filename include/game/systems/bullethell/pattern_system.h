#pragma once
#include "game/components.h"
#include "game/utils/physics_util.h"

// FIXME: Part of physics, consider separation into its own thread
namespace Game::BulletHell
{
    void InitializePatterns(Patterns& patterns, float speed, float angle, float acceleration, float angular_velocity, float max_speed);

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
            auto &vel_c = comps.get<Velocity>();
            auto &rot_c = comps.get<Rotation>();
            auto &acc_c = comps.get<Acceleration>();
            auto &ang_c = comps.get<AngularVelocity>();

            auto &patterns = patt_c.patterns;

            if (!comps.get<Patterns>().isInitialized)
            {
                InitializePatterns(patt_c, vel_c.vx, rot_c.angleZ, acc_c.ax, ang_c.v, acc_c.max_speed_x);
                comps.get<Patterns>().isInitialized = true;
            }

            for (auto itr = patterns.begin(); itr != patterns.end();)
            {
                // Execute
                if (itr->first < frame_time)
                {
                    const auto pattern = itr->second;
                    comps.get<Velocity>().vx = pattern.speed;
                    comps.get<Rotation>().angleZ = pattern.angle;
                    comps.get<Acceleration>().ax = pattern.accel;
                    comps.get<AngularVelocity>().v = pattern.angular_velocity;
                    comps.get<Acceleration>().max_speed_x = pattern.max_speed;
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

    inline void InitializePatterns(Patterns& patterns, const float speed, const float angle, const float acceleration, const float angular_velocity, const float max_speed)
    {
        MoveParam prev{speed,angle,acceleration,angular_velocity,max_speed};

        for (auto& [time, pat] : patterns.patterns)
        {
            if (pat.speed == UNASSIGNED) pat.speed = prev.speed;
            if (pat.angle == UNASSIGNED) pat.angle = prev.angle;
            if (pat.accel == UNASSIGNED) pat.accel = prev.accel;
            if (pat.angular_velocity == UNASSIGNED) pat.angular_velocity = prev.angular_velocity;
            if (pat.max_speed == UNASSIGNED) pat.max_speed = prev.max_speed;

            prev = pat;
        }
    }
}