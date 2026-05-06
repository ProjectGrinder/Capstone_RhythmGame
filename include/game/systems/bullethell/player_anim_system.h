#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template<typename T>
    void player_anim_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState> &query2,
            System::ECS::Query<Player, Render::Transform, Render::Sprite, Physics::Velocity, Render::Animation_Controller> &query3
            )
    {

        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        for (auto &[id, comps]: query3)
        {
            auto &anim = comps.get<Render::Animation_Controller>();
            const auto &velos = comps.get<Velocity>();

            if (velos.vx != 0)
            {
                anim.to_id = 4;
                comps.get<Render::Sprite>().flipX = velos.vx > 0;
            }
            else if (velos.vy > 0)
                anim.to_id = 5;
            else if (velos.vy < 0)
                anim.to_id = 3;
            else if (anim.current_id>=3)
            {
                anim.to_id = anim.current_id-3;
            }
        }

    }
} // namespace Game::BulletHell
