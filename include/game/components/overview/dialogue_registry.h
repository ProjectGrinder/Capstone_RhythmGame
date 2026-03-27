#pragma once

namespace Game::Overview
{
    struct DialogueRegistry
    {
        // Could leads to error
        std::vector<std::vector<std::string>> text_register;
        DialogueRegistry()
        {}
        explicit DialogueRegistry(const std::vector<std::vector<std::string>> &text_register) : text_register(text_register) {}
    };
} // namespace Game::Overview
