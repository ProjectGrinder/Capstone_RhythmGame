#pragma once

#include "game/components.h"

namespace Game::Overview
{
    template <typename T>
    void logging_system([[maybe_unused]] T &syscall,
        System::ECS::Query<Player, Position> &player_query,
        System::ECS::Query<Interactable, EventState> &interact_query,
        System::ECS::Query<DialogueBox, EventState> &dialog_query)
    {

        LOG_INFO("----------------------------------");

        // const auto &bullet_loader = query.front().get<Battle::BulletLoader>();
        const auto player_pos = player_query.front().get<Position>();
        LOG_INFO("Player Pos : (%d,%d)", static_cast<int>(player_pos.x), static_cast<int>(player_pos.y));

        for (auto &[id, comps] : interact_query)
        {
            if (comps.get<EventState>().event_occupied) continue;
            if (comps.get<Interactable>().in_range) LOG_INFO("Entity %d is in range. Press UP to interact.", id);
        }

        for (auto &[id, comps] : dialog_query)
        {
            if (!comps.get<EventState>().has_event) continue;
            LOG_INFO(comps.get<DialogueBox>().current_text.c_str());
        }
        LOG_INFO("----------------------------------");
    }
}