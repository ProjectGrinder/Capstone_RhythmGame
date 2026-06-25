#pragma once
#include "game/utils/constant.h"

namespace Game::World
{
    struct DialogueBox
    {
        std::string text_to_show;
        std::string current_text;
        bool is_typing = true;
        bool is_destroyed = false;
        int type_delay_time = DIALOGUE_TYPE_DELAY;
        DialogueBox() : text_to_show(""), current_text("")
        {}
        explicit DialogueBox(const std::string &text) : text_to_show(text), current_text("") {}
    };
} // namespace Game::Overview
