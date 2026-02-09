#pragma once
#include "game/components.h"


namespace Game::BulletHell
{
    template<typename T>
    void SpawnBullet(T &syscall, const Battle::BulletData& bullet_data, const Battle::BulletHellState bhs)
    {
        const System::ECS::pid bullet = syscall.template create_entity<Position, Patterns, Delay>(Position(bullet_data.posX,bullet_data.posY), Patterns(bullet_data.patterns), Delay(bullet_data.delay_frame));

        syscall.addcomponent(bullet, Rotation(), Physics::Scale(), Velocity(), Acceleration(), AngularVelocity());

        const Battle::BulletGraphicMap bullet_info = Battle::bulletGraphicMap[bullet_data.graphicID];
        syscall.addcomponent(bullet, Bullet(false, static_cast<int>(bullet_info.damage_mul * static_cast<float>(bhs.damage)), bullet_info.pierce), Particle(bullet_info.lifetime));

        if (bullet_info.special_bullet_data.type == Battle::Booming) syscall.addcomponent(bullet, Booming(bullet_info.special_bullet_data.size, bullet_info.special_bullet_data.frame));
        else if (bullet_info.special_bullet_data.type == Battle::Laser) syscall.addcomponent(bullet, Laser(bullet_data.posX, bullet_data.posY, bullet_info.special_bullet_data.size, bullet_info.special_bullet_data.frame));

        const Battle::GraphicData bullet_graphic = bullet_info.graphic_data;
        syscall.addcomponent(bullet, Render::Sprite(bullet_graphic.sprite), Render::Material(nullptr,nullptr, Render::Color(bullet_graphic.r,bullet_graphic.g,bullet_graphic.b)));

        if (const Battle::ColliderData bullet_collider = bullet_info.collider_data;
            bullet_collider.type == Physics::RECTANGLE) syscall.addcomponent(bullet, Physics::RectangularCollider(bullet_collider.offsetX,bullet_collider.offsetY, bullet_collider.colX, bullet_collider.colY));
        else if (bullet_collider.type == Physics::CIRCLE) syscall.addcomponent(bullet, Physics::CircularCollider(bullet_collider.offsetX,bullet_collider.offsetY, bullet_collider.colX, bullet_collider.colY));


    }

    template<typename T>
    void LoadBullets(T &syscall, System::ECS::Query<Battle::BulletLoader> &query, System::ECS::Query<Battle::BulletHellState> &query2)
    {
        constexpr auto frame_time = 1;

        if (query2.begin() == query2.end())
            return;

        auto &bullet_loader = query.front().get<Battle::BulletLoader>();
        auto &pointer = bullet_loader.pointer;
        auto &batches = bullet_loader.batches;
        auto &current_frame = bullet_loader.current_frame;

        while (pointer < batches.size() && batches[pointer].frame <= current_frame)
        {
            for (auto& b : batches[pointer].bullets)
                SpawnBullet(syscall, b, query2.front());

            pointer++;
        }

        current_frame++;
    }
} // namespace Game::BulletHell