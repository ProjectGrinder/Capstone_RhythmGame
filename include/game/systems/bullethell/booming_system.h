#pragma once
#include "game/components.h"
#include "game/systems/global_clock.h"
#include "game/utils/audio_util.h"

namespace Game::BulletHell
{
        // Default FadeIn/Out
    template <typename T>
    void boomer_system([[maybe_unused]] T &syscall, System::ECS::Query<Booming , Particle,Delay, Render::Transform, Render::Sprite>& query, System::ECS::Query<Battle::BattleState> &query2, System::ECS::Query<Audio::SoundRegistry> &sound_query)
    {
        if (query2.begin() == query2.end())
            return;

        if (sound_query.begin() == sound_query.end())
            return;

        auto sound_registry = sound_query.front().components.get<Audio::SoundRegistry>().audios;

        for (auto &[id, comps] : query)
        {
            auto &boomer = comps.get<Booming>();
            const auto &particle = comps.get<Particle>();
            auto &delay_comp = comps.get<Delay>();
            auto &scl = comps.get<Render::Transform>();
            auto &render = comps.get<Render::Sprite>();

            if (!boomer.is_activated)
            {
                //Change after frame time. This logic sucks.
                if (delay_comp.delay > 10)
                {
                    scl.scaleX = boomer.max_size;
                    scl.scaleY = boomer.max_size;
                    render.color.a = 0.25;
                    // Audio::audio_play(sound_registry["sound_bullet_boomer"]);
                }
                else if (delay_comp.delay <= 0)
                {
                    scl.scaleX += static_cast<float>(get_delta_time() * boomer.max_size / boomer.boom_frame);
                    scl.scaleY += static_cast<float>(get_delta_time() * boomer.max_size/ boomer.boom_frame);
                    if (scl.scaleX >= boomer.max_size)
                    {
                        scl.scaleX = boomer.max_size;
                        scl.scaleY = boomer.max_size;
                        boomer.is_activated = true;
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
                if (particle.lifetime < 500)
                {
                    scl.scaleX -= static_cast<float>(get_delta_time() * boomer.max_size / 50);
                    scl.scaleY -= static_cast<float>(get_delta_time() * boomer.max_size/ 50);
                }
            }
        }
    }
}