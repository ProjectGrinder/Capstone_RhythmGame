#pragma once
#include <string>

#include "dialogue_system.h"
namespace Game::Overview
{
    template<typename T>
    void dialogue_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Input> &input_query,
            System::ECS::Query<DialogueRegistry> &dialogue_query,
            System::ECS::Query<EventState, DialogueEvent> &query1,
            System::ECS::Query<DialogueBox> &query2)
    {
        if (input_query.begin() == input_query.end())
            return;

        const auto &input = input_query.front().components.get<Input>();
        const auto &dialogue_registry = dialogue_query.front().components.get<DialogueRegistry>().text_register;

        for (auto &[id, comps] : query1)
        {
            auto &event_state = comps.get<EventState>();
            auto &dialogue = comps.get<DialogueEvent>();

            if (dialogue.dialogue_box_id != UNASSIGNED)
            {
                // TODO : More Render comps to add
                dialogue.dialogue_box_id = syscall.create_entity(DialogueBox(dialogue_registry[dialogue.dialogue_id]));
            }

            else
            {
                // No ID query access so this.
                for (auto &[id2, comps2] : query2)
                {
                    if (input.up_pressed || input.z_pressed && dialogue.dialogue_box_id == id2)
                    {
                        auto &dialogue_box = comps2.get<DialogueBox>();
                        if (!dialogue_box.is_typing)
                        {
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
            System::ECS::Query<DialogueBox> &query1)
    {
        for (auto &[id, comps] : query1)
        {
            auto &dialogue_box = comps.get<DialogueBox>();

            if (dialogue_box.is_typing)
            {
                if (dialogue_box.current_text.size() == dialogue_box.text_to_show.size())
                    dialogue_box.is_typing = false;
                else
                    dialogue_box.current_text = dialogue_box.text_to_show.substr(0, dialogue_box.current_text.size());
            }

            if (dialogue_box.is_destroyed)
            {
                // TODO : Anim Destroy
                syscall.remove_entity(id);
            }

            // TODO:Render text
        }
    }
} // namespace Game::Overview

