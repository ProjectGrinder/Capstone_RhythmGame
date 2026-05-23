#pragma once

namespace Game::Overview
{
    struct GlobalState
    {
        bool movementLocked = false;
        bool interactionLocked = false;
        bool menuLocked = false;
    };
} // namespace Game::Overview
