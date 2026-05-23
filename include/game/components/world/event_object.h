#pragma once

namespace Game::Overview
{
    struct EventState
    {
        uint16_t event_id;
        uint16_t event_idx = 0;
        bool event_occupied = false;
        bool has_event = false;
        explicit EventState(const uint16_t event_id) : event_id(event_id) {}
    };
} // namespace Game::Overview
