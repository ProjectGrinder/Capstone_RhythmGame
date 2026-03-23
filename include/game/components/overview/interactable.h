#pragma once

namespace Game::Overview
{
    struct Interactable
    {
        bool active = true;
        bool in_range = false;
        bool event_occupied = false;
        uint16_t event_id;

        explicit Interactable(const uint16_t event_id) : event_id(event_id) {}
    };
} // namespace Game::BulletHell
