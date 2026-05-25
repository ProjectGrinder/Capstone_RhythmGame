#pragma once

namespace Game::World
{
    struct SaveState
    {
        uint16_t world_id = 0;
        uint16_t world_state = 0b0000000000000000; // Mask?
    };
} // namespace Game::Overview
