#pragma once

#include "game/components.h"
#include "game/systems/global_clock.h"

namespace Game::BulletHell
{
    template<typename T>
    void bullet_clearer_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<BulletClearer, Render::Material, Render::Transform, Particle> &query,
            System::ECS::Query<Bullet, Render::Transform> &bullet_query,
            System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        for (auto &[id, comps] : query)
        {
            auto &bullet_clearer = comps.get<BulletClearer>();
            auto &tra = comps.get<Render::Transform>();
            auto &material = comps.get<Render::Material>();

            if (bullet_clearer.lifetime == UNASSIGNED) bullet_clearer.lifetime = comps.get<Particle>().lifetime;

            // Growing
            tra.scaleX = tra.scaleY = Physics::clamp(static_cast<float>(tra.scaleX + bullet_clearer.speed * get_delta_time()),0.f,bullet_clearer.max_size);
            material.color.a = static_cast<float>(comps.get<Particle>().lifetime/bullet_clearer.lifetime);

            //Check Bullet in Range
            for (auto &[id2, comps2] : bullet_query)
            {
                const float dx = comps2.get<Render::Transform>().position.x - comps.get<Render::Transform>().position.x;
                const float dy = comps2.get<Render::Transform>().position.y - comps.get<Render::Transform>().position.y;
                const float distance_squared = dx * dx + dy * dy;
                if (distance_squared <= tra.scaleX*tra.scaleX)
                {
                    syscall.remove_entity(id2);
                }
            }

        }
    }
} // namespace Game::BulletHell
