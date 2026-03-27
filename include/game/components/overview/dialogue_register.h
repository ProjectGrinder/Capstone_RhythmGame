#pragma once

namespace Game::Overview
{
    struct DialogueRegister
    {
        // Could leads to error
        std::vector<std::vector<std::string>> text_register;
        DialogueRegister()
        {}
        explicit DialogueRegister(const std::vector<std::vector<std::string>> &text_register) : text_register(text_register) {}
    };
} // namespace Game::Overview
