#pragma once
#include "game/components.h"

// TODO: Marked by Nack (his part)
namespace Game::BulletHell
{
    template<typename T>
    void particle_system(T &syscall, System::ECS::Query<Particle> &query)
    {
        constexpr auto frame_time = 1;

        for (auto &[id, comps]: query)
        {
            auto &comp = comps.get<Particle>();
            comp.lifetime -= frame_time;
            if (comp.lifetime < 0)
            {
                // TODO: Animate this after renderer
                switch (comp.destroy_effect)
                {
                case Disappear:
                    break;
                default:
                    break;
                }
                syscall.template remove_entity<Particle>(id);
            }
        }
    }
} // namespace Game::BulletHell
