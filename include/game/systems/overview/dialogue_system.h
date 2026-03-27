#pragma once
#include <string>
namespace Game::Overview
{
    template<typename T>
    void dialogue_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Input> &input_query,
            System::ECS::Query<EventState, Dialogue> &query1,
            System::ECS::Query<DialogueBox> &query2)
    {
        if (input_query.begin() == input_query.end())
            return;

        const auto &input = input_query.front().components.get<Input>();

        for (auto &[id, comps] : query1)
        {
            auto &event_state = comps.get<EventState>();
            auto &dialogue = comps.get<Dialogue>();

            if (dialogue.dialogue_box_id != UNASSIGNED)
            {
                // TODO : More Render comps to add
                dialogue.dialogue_box_id = syscall.create_entity(DialogueBox(dialogue.dialogue));
            }

            else
            {
                // Know ID query access so this.
                for (auto &[id2, comps2] : query2)
                {
                    if (input.up_pressed || input.z_pressed && dialogue.dialogue_box_id == id2)
                    {
                        auto &dialogue_box = comps2.get<DialogueBox>();
                        if (!dialogue_box.isTyping)
                        {
                            event_state.has_event = false;
                            syscall.remove_component(id);
                            syscall.remove_entity(id2);
                        }
                        else
                        {
                            dialogue_box.current_text = dialogue_box.text_to_show;
                            dialogue_box.isTyping = false;
                        }
                    }
                }
            }
        }
    }

    template<typename T>
    void dialogue_box_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Input> &input_query,
            System::ECS::Query<DialogueBox> &query1)
    {
        if (input_query.begin() == input_query.end())
            return;

        const auto &input = input_query.front().components.get<Input>();

        for (auto &[id, comps] : query1)
        {
            auto &dialogue_box = comps.get<DialogueBox>();

            if (dialogue_box.isTyping)
            {
                if (dialogue_box.current_text.size() == dialogue_box.text_to_show.size())
                    dialogue_box.isTyping = false;
                else
                    dialogue_box.current_text = dialogue_box.text_to_show.substr(0, dialogue_box.current_text.size());
            }

            // TODO:Render text
        }
    }
} // namespace Game::Overview

