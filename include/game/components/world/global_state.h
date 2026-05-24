#pragma once

namespace Game::World
{
    struct GlobalState
    {
        bool movementLocked = false;
        bool interactionLocked = false;
        bool menuLocked = false;
    };
} // namespace Game::Overview
