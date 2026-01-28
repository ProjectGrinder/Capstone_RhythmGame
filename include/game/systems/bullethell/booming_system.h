#pragma once
#include "game/components.h"

namespace Game::BulletHell
{
        // Default FadeIn/Out
    template <typename T>
    void Boomer_System([[maybe_unused]] T &syscall, System::ECS::Query<Booming,Delay, Physics::Scale, Render::Material>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        constexpr auto frame_time = 1;
        for (auto &[id, comps] : query)
        {
            auto &boomer = comps.get<Booming>();
            const auto &delay_comp = comps.get<Delay>();
            auto &scl = comps.get<Physics::Scale>();
            auto &render = comps.get<Render::Material>();

            if (boomer.lifetime <= 0)
            {
                if (scl.scaleX > 0)
                {
                    scl.scaleX += (0 - scl.scaleX)/boomer.divider;
                    scl.scaleY += (0 - scl.scaleY)/boomer.divider;
                }
                else
                {
                    // Destroy entity? ObjectPool? Later
                }
            }
            else
            {
                //Change after frame time. This logic sucks.
                if (delay_comp.delay > 1)
                {
                    scl.scaleX = boomer.max_size;
                    scl.scaleY = boomer.max_size;
                    render.color.a = 0.25;
                }
                else if (delay_comp.delay == 1)
                {
                    scl.scaleX = boomer.init_size;
                    scl.scaleY = boomer.init_size;
                    render.color.a = 1;
                }

                else if (delay_comp.delay <= 0)
                {
                    if (scl.scaleX < boomer.max_size)
                    {
                        scl.scaleX += (boomer.max_size - scl.scaleX)/boomer.divider;
                        scl.scaleY += (boomer.max_size - scl.scaleY)/boomer.divider;
                    }
                    boomer.lifetime -= frame_time;
                }
            }

        }
    }
}