#pragma once
#include "game/components.h"

namespace Game::BulletHell
{
        // Default FadeIn/Out
    template <typename T>
    void Boomer_System([[maybe_unused]] T &syscall, System::ECS::Query<Booming , Particle,Delay, Physics::Scale, Render::Material>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        for (auto &[id, comps] : query)
        {
            auto &boomer = comps.get<Booming>();
            const auto &particle = comps.get<Particle>();
            auto &delay_comp = comps.get<Delay>();
            auto &scl = comps.get<Physics::Scale>();
            auto &render = comps.get<Render::Material>();

            if (!boomer.is_activated)
            {
                //Change after frame time. This logic sucks.
                if (delay_comp.delay > 1000)
                {
                    scl.scaleX = boomer.max_size;
                    scl.scaleY = boomer.max_size;
                    render.color.a = 0.25;
                }
                else if (delay_comp.delay <= 0)
                {
                    scl.scaleX += boomer.max_size / static_cast<float>(boomer.boom_frame);
                    scl.scaleY += boomer.max_size/ static_cast<float>(boomer.boom_frame);
                    if (scl.scaleX >= boomer.max_size)
                    {
                        scl.scaleX = boomer.max_size;
                        scl.scaleY = boomer.max_size;
                        boomer.is_activated = true;
                        delay_comp.delay = particle.lifetime - boomer.boom_frame - 1;
                        delay_comp.delay = delay_comp.delay > 0 ? delay_comp.delay : 0;
                    }
                }
                else
                {
                    scl.scaleX = 0;
                    scl.scaleY = 0;
                    render.color.a = 1;
                }
            }
            else
            {
                if (delay_comp.delay <= 0)
                {
                    scl.scaleX -= boomer.max_size / static_cast<float>(boomer.boom_frame);
                    scl.scaleY -= boomer.max_size/ static_cast<float>(boomer.boom_frame);
                }
            }
        }
    }
}