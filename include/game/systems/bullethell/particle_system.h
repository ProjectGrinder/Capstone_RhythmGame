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
                if (comp.lifetime < -comp.destroy_frame) syscall.remove_entity(id);

                switch (comp.destroy_effect)
                {
                case Disappear:
                    syscall.remove_entity(id);
                    break;
                case Fade:
                    if (auto* spr = syscall.template try_query<Render::Sprite>(id)) spr->color = {spr->color.r, spr->color.g, spr->color.b, Physics::lerp_at_frame(spr->color.a,0.f,comp.destroy_frame)};
                    else if (auto* txt = syscall.template try_query<Render::Text>(id)) txt->color = {txt->color.r, txt->color.g, txt->color.b, Physics::lerp_at_frame(txt->color.a,0.f,comp.destroy_frame)};
                    break;

                case Shrink:
                    if (auto* tr = syscall.template try_query<Render::Transform>(id))
                    {
                        tr->scaleX = Physics::lerp_at_frame(tr->scaleX, 0.f, comp.destroy_frame);
                        tr->scaleY = Physics::lerp_at_frame(tr->scaleY, 0.f, comp.destroy_frame);
                        tr->scaleZ = Physics::lerp_at_frame(tr->scaleZ, 0.f, comp.destroy_frame);
                    }
                default:
                    syscall.remove_entity(id);
                    break;
                }
            }
        }
    }
} // namespace Game::BulletHell
