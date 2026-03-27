#pragma once

namespace Game::Overview
{
    struct LevelNode
    {
        enum class Selection
        {
            Confirm = 0,
            Cancel = 1,
            Count = 2
        };
        uint16_t id;
        Selection sel = Selection::Confirm;
        bool is_destroyed = false;
        LevelNode() = default;
        explicit LevelNode(const uint16_t id) : id(id) {}
    };
} // namespace Game::Overview
