#pragma once
#include "../../components/audio/audio.h"
#include "game/components.h"

// TODO : Render stuffs
namespace Game::BulletHell
{
    template<typename T>
    void spawn_bullet(T &syscall, const Battle::BulletData& bullet_data, const Battle::BulletRegistry& bullet_registry, const Battle::BulletHellState bhs, const Audio::SoundRegistry sound_registry)
    {
        if (bullet_data.bullet_id >= bullet_registry.bulletStageMaps.size()) return;

        const auto bullet_stage_data = bullet_registry.bulletStageMaps[bullet_data.bullet_id];
        const auto bullet_graphic_data = bullet_registry.bulletGraphicMaps[bullet_stage_data.graphicID];

        auto bullet_movement = bullet_data.movement_data;
        auto bullet_timing = bullet_stage_data.bullet_timing_data;

        const System::ECS::pid bullet = syscall.create_entity(Render::Transform(bullet_movement.posX,bullet_movement.posY,0,0,0,bullet_stage_data.size.x,bullet_stage_data.size.y,1), Delay(bullet_timing.delay_frame));

        syscall.add_components(bullet, Rotation(bullet_movement.rot,true), Velocity(bullet_movement.vel), Acceleration(bullet_movement.acc), AngularVelocity(bullet_movement.wvel), Pattern(bullet_data.pattern));

        syscall.add_components(bullet, Bullet(static_cast<int>(bullet_stage_data.damage_mul * static_cast<float>(bhs.damage)), bullet_stage_data.pierce), Particle(bullet_timing.lifetime));

        if (bullet_stage_data.special_bullet_data.type == Battle::Booming) syscall.add_components(bullet, Booming(bullet_stage_data.special_bullet_data.size, bullet_stage_data.special_bullet_data.frame));
        else if (bullet_stage_data.special_bullet_data.type == Battle::Laser) syscall.add_components(bullet, Laser(bullet_movement.posX, bullet_movement.posY, bullet_stage_data.special_bullet_data.size, bullet_stage_data.special_bullet_data.frame));

        const Battle::GraphicData bullet_graphic = bullet_graphic_data.graphic_data;
        syscall.add_components(bullet, Render::Sprite{
                    .sp = get_assets_record_ptr(get_assets_id("bullet_sprite")),
                    .pos = {{bullet_graphic.dest_rect[0], bullet_graphic.dest_rect[3], 0}, {bullet_graphic.dest_rect[2], bullet_graphic.dest_rect[3], 0}, {bullet_graphic.dest_rect[2], bullet_graphic.dest_rect[1], 0}, {bullet_graphic.dest_rect[0], bullet_graphic.dest_rect[1], 0}},
                    .layer = 5,
                    .u0 = static_cast<float>(bullet_graphic.src_rect[0]) /512,
                    .v0 = static_cast<float>(bullet_graphic.src_rect[1])/512,
                    .u1 = static_cast<float>(bullet_graphic.src_rect[2])/512,
                    .v1 = static_cast<float>(bullet_graphic.src_rect[3])/512}
                    , Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")),get_assets_record_ptr(get_assets_id("sprite_ps")), {bullet_graphic.r, bullet_graphic.g, bullet_graphic.b, bullet_graphic.a}));
        if (const Battle::ColliderData bullet_collider = bullet_graphic_data.collider_data;
            bullet_collider.type == Physics::RECTANGLE) syscall.add_components(bullet, Physics::RectangularCollider(bullet_collider.offsetX,bullet_collider.offsetY, bullet_collider.colX, bullet_collider.colY));
        else if (bullet_collider.type == Physics::CIRCLE) syscall.add_components(bullet, Physics::CircularCollider(bullet_collider.offsetX,bullet_collider.offsetY, bullet_collider.colX, bullet_collider.colY));

        auto sounds = sound_registry.audios;
        switch (bullet_graphic_data.graphic_data.bullet_spawn_sound)
        {
            case 1 :
                // Audio::audio_play(sounds["sound_bullet_spawn_0"]);
                break;
            case 2 :
                // Audio::audio_play(sounds["sound_bullet_spawn_1"]);
                break;
            case 3 :
                //  Audio::audio_play(sounds["sound_bullet_spawn_2"]);
                break;
            default:;
        }
    }

    template<typename T>
    void load_bullets([[maybe_unused]] T &syscall,[[maybe_unused]]  System::ECS::Query<Battle::BulletLoader> &query,[[maybe_unused]]  System::ECS::Query<Battle::BulletRegistry> &query2,[[maybe_unused]]  System::ECS::Query<Battle::BattleState,Battle::BulletHellState> &query3, System::ECS::Query<Audio::SoundRegistry> &sound_query)
    {
        if (query3.begin() == query3.end())
            return;

        if (sound_query.begin() == sound_query.end())
            return;

        auto &bullet_loader = query.front().get<Battle::BulletLoader>();
        auto &pointer = bullet_loader.pointer;
        auto &batches = bullet_loader.batches;
        const auto &current_frame = query3.front().get<Battle::BattleState>().clock_time / 1000;

        // Sort bullet spawn frame
        if (!bullet_loader.initialized)
        {
            std::sort(batches.begin(), batches.end(),
            [](const Battle::BulletBatch & a, const Battle::BulletBatch& b)
            {
                return a.frame < b.frame;
            });
            bullet_loader.initialized = true;
        }

        while ((pointer < static_cast<int>(batches.size())) && (batches[pointer].frame <= current_frame))
        {
            for (auto& b : batches[pointer].bullets)
                spawn_bullet(syscall, b, query2.front().get<Battle::BulletRegistry>(), query3.front().get<Battle::BulletHellState>(), sound_query.front().components.get<Audio::SoundRegistry>());

            pointer++;
        }
    }
} // namespace Game::BulletHell