#pragma once

namespace Game::Overview
{
    struct DialogueBox
    {
        std::string text_to_show;
        std::string current_text;
        bool isTyping = true;
        DialogueBox() : text_to_show(""), current_text("")
        {}
        explicit DialogueBox(const std::string &text) : text_to_show(text), current_text("") {}
    };
} // namespace Game::Overview
