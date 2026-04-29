#pragma once
#include "game/components.h"

namespace Game::Render
{
    template<typename T>
    void flickering_system(T &syscall, System::ECS::Query<Sprite, Flicker> &query)
    {
        for (auto &[id, comps]: query)
        {
            auto &sprite = comps.get<Sprite>();
            auto &flicker = comps.get<Flicker>();

            flicker.time -= static_cast<int>(get_delta_time());
            flicker.flicker_delay -= static_cast<int>(get_delta_time());
            if (flicker.time < 0)
            {
                syscall.template remove_component<Flicker>(id);
            }
            else
            {
                if (flicker.init_alpha == UNASSIGNED)
                {
                    flicker.init_alpha = sprite.color.a;
                }
                else
                {
                    if (flicker.flicker_delay<=0)
                    {
                        sprite.color.a = flicker.flicker?flicker.init_alpha:0;
                        flicker.flicker = !flicker.flicker;
                        flicker.flicker_delay = flicker.flicker_rate;
                    }
                }
            }
        }
    }
} // namespace Game::BulletHell
