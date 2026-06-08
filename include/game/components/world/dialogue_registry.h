#pragma once

namespace Game::World
{
    struct DialogueRegistry
    {
        // Could leads to error
        std::vector<std::string> text_register;
        DialogueRegistry()
        {}
        explicit DialogueRegistry(const std::vector<std::string> &text_register) : text_register(text_register) {}
    };
} // namespace Game::Overview
