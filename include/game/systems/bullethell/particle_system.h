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
                if (comp.lifetime <= -comp.destroy_frame || comp.destroy_effect == Disappear)
                    syscall.remove_entity(id);
            }
        }
    }

    template<typename T>
    void particle_destroy_system(T &syscall, System::ECS::Query<Particle, Render::Transform, Render::Sprite> &query)
    {
        for (auto &[id, comps]: query)
        {
            auto &comp = comps.get<Particle>();
            if (comp.lifetime < 0)
            {
                if (comp.destroy_effect == Fade)
                {
                    auto &color = comps.get<Render::Sprite>().color;
                    color = {color.r, color.g, color.b, Physics::lerp(color.a,0.f,comp.destroy_frame)};
                }
                else if (comp.destroy_effect == Shrink)
                {
                    comps.get<Render::Transform>().scaleX = Physics::lerp_at_frame(comps.get<Render::Transform>().scaleX, 0.f, comp.destroy_frame);
                    comps.get<Render::Transform>().scaleY = Physics::lerp_at_frame(comps.get<Render::Transform>().scaleY, 0.f, comp.destroy_frame);
                    comps.get<Render::Transform>().scaleZ = Physics::lerp_at_frame(comps.get<Render::Transform>().scaleZ, 0.f, comp.destroy_frame);
                }
            }
        }
    }
} // namespace Game::BulletHell
