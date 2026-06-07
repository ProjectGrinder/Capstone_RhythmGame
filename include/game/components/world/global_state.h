#pragma once

namespace Game::World
{
    struct GlobalState
    {
        int clock_time = 0;
        int level_selected = -1;
        int diff_selected = -1;
        bool movementLocked = false;
        bool interactionLocked = false;
        bool menuLocked = false;

    };
} // namespace Game::Overview
