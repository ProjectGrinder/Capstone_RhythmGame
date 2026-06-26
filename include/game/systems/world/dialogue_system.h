#pragma once
#include <string>

#include "dialogue_system.h"
#include "game/utils/constant.h"
namespace Game::World
{
    // For resize box later
    // void RenderTextAndBox(std::string current_text, std::string full_text)
    // {
    //
    // }

    template<typename T>
    System::ECS::pid InitTextBox(T &syscall, const std::string text, Math::Point &position)
    {
        System::ECS::pid id = syscall.create_entity(DialogueBox(text));

        const float box_size = (float)text.length()*15;
        syscall.add_components(id, Render::Material{
            get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))},
            Render::Transform{position.x,position.y + DIALOGUE_BOX_OFFSET_Y},
            Render::Resize{{1, 1}, 500});

        syscall.add_components(id, Render::Sprite{
            // I WANT TEXT FIXED RIGHT NOWWW
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-box_size/2, DIALOGUE_BOX_HEIGHT/2+DIALOGUE_BOX_HEIGHT/4, 0}, {box_size/2, DIALOGUE_BOX_HEIGHT/2+DIALOGUE_BOX_HEIGHT/4, 0}, {box_size/2, -DIALOGUE_BOX_HEIGHT/2+DIALOGUE_BOX_HEIGHT/4, 0}, {-box_size/2, -DIALOGUE_BOX_HEIGHT/2+DIALOGUE_BOX_HEIGHT/4, 0}}, .layer = 40}
        });

        syscall.add_components(id, Render::Text{
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "", .color = Math::Color{0, 0, 0, 1}, .layer = 41, .align = Render::Center}
        });
        return id;
    }

    template<typename T>
    void dialogue_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Input> &input_query,
            System::ECS::Query<DialogueRegistry> &dialogue_query,
            System::ECS::Query<EventState, DialogueEvent, Render::Transform> &query1,
            System::ECS::Query<DialogueBox> &query2,
            System::ECS::Query<Audio::SoundRegistry> &audio_query)
    {
        if (input_query.begin() == input_query.end())
            return;

        if (dialogue_query.begin() == dialogue_query.end())
            return;

        const auto &input = input_query.front().components.get<Input>();
        const auto &dialogue_registry = dialogue_query.front().components.get<DialogueRegistry>().text_register;
        const auto &sound_reg = audio_query.front().components.get<Audio::SoundRegistry>();
        auto sounds = sound_reg.audios;

        for (auto &[id, comps] : query1)
        {
            auto &event_state = comps.get<EventState>();
            auto &dialogue = comps.get<DialogueEvent>();

            if (dialogue.dialogue_box_id == INVALID_PID)
            {
                dialogue.dialogue_box_id = InitTextBox(syscall, dialogue_registry[dialogue.dialogue_id], comps.get<Render::Transform>().position);
                Audio::audio_play(sounds["sound_select"]);
            }

            else
            {
                // No ID query access so this.
                for (auto &[id2, comps2] : query2)
                {
                    if ((input.z_pressed || input.enter_pressed) && dialogue.dialogue_box_id == id2)
                    {
                        auto &dialogue_box = comps2.get<DialogueBox>();
                        if (!dialogue_box.is_typing)
                        {
                            Audio::audio_play(sounds["sound_select"]);
                            event_state.has_event = false;
                            syscall.template remove_component<DialogueEvent>(id);
                            dialogue_box.is_destroyed = true;
                        }
                        else
                        {
                            dialogue_box.current_text = dialogue_box.text_to_show;
                            dialogue_box.is_typing = false;
                        }
                    }
                }
            }
        }
    }

    template<typename T>
    void dialogue_box_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<DialogueBox, Render::Text, Render::Sprite> &query1)
    {
        for (auto &[id, comps] : query1)
        {
            auto &dialogue_box = comps.get<DialogueBox>();

            if (dialogue_box.is_typing)
            {
                if (dialogue_box.current_text.size() == dialogue_box.text_to_show.size())
                    dialogue_box.is_typing = false;

                else
                {
                    dialogue_box.type_delay_time -= (int)get_delta_time();
                    if (dialogue_box.type_delay_time <= 0)
                    {
                        dialogue_box.current_text = dialogue_box.text_to_show.substr(0, dialogue_box.current_text.size()+1);
                        dialogue_box.type_delay_time = DIALOGUE_TYPE_DELAY;
                    }
                }

            }

            if (dialogue_box.is_destroyed)
                // syscall.template add_component<BulletHell::Particle>(id, BulletHell::Particle(100, BulletHell::Shrink, 500));
                syscall.remove_entity(id);

            comps.get<Render::Text>().text = dialogue_box.current_text;
        }
    }
} // namespace Game::Overview

