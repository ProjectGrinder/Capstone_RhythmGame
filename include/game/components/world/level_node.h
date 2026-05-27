#pragma once

namespace Game::World
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
        System::ECS::pid level_node_box_pid = INVALID_PID;
        std::vector<System::ECS::pid> level_node_texts_pid;
        LevelNode() = default;
        explicit LevelNode(const uint16_t id) : id(id) {}
    };
} // namespace Game::Overview
