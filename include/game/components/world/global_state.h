#pragma once

namespace Game::World
{
    struct GlobalState
    {
        int clock_time = 0;
        bool movementLocked = false;
        bool interactionLocked = false;
        bool menuLocked = false;

    };
} // namespace Game::Overview
