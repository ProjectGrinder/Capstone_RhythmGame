#pragma once
#include "game/systems/global_clock.h"

namespace Game::BulletHell
{
    template<typename T>
    void player_animation_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Player, Render::Animator, Physics::Velocity> &player_query,
            System::ECS::Query<Battle::BattleState> &query3)
    {
        // if (query3.begin() == query3.end())
        //     return;
        //
        // if (query3.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
        //     return;

        // for (auto &[id, comps] : player_query)
        // {
        //     auto &anim = comps.get<Render::Animator>();
        //     const auto &player = comps.get<Player>();
        //     const auto &velos = comps.get<Velocity>();
        //     if (velos.vx <= 0 && velos.vy <= 0)
        //     {
        //         switch (player.face_dir)
        //         {
        //             case Direction::Up:
        //
        //             default:;
        //         }
        //     }
        //     else
        //     {
        //
        //     }
        //
        // }
    }
} // namespace Game::BulletHell
