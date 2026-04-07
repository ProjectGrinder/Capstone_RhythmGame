#pragma once
#include "game/components.h"

// TODO: Marked by Nack (his part)
namespace Game::BulletHell
{
    template<typename T>
    void particle_system(T &syscall, System::ECS::Query<Particle> &query)
    {
        for (auto &[id, comps]: query)
        {
            auto &comp = comps.get<Particle>();
            comp.lifetime -= static_cast<int>(get_delta_time());
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
                syscall.remove_entity(id);
            }
        }
    }
} // namespace Game::BulletHell
