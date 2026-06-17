#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template<typename T>
    void player_anim_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState> &query2,
            System::ECS::Query<Player, Render::Transform, Render::Sprite, Physics::Velocity,Render::Animator, Render::Animation_Controller> &query3
            )
    {

        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        for (auto &[id, comps]: query3)
        {
            auto &anim = comps.get<Render::Animator>();
            auto &anim_con = comps.get<Render::Animation_Controller>();
            const auto &velos = comps.get<Velocity>();

            if (velos.vx != 0)
            {
                anim_con.to_anim = "Player_Move_Left";
                comps.get<Render::Sprite>().flipX = velos.vx > 0;
            }
            else if (velos.vy > 0)
                anim_con.to_anim = "Player_Move_Back";
            else if (velos.vy < 0)
                anim_con.to_anim = "Player_Move_Front";
            else if (anim.anim_name.find("Move") != std::string::npos)
            {
                anim_con.to_anim = "Player_Idle_" + anim.anim_name.substr(12,anim.anim_name.size()-12+1);
            }
        }

    }
} // namespace Game::BulletHell
