#pragma once

#include "game/components.h"

namespace Game::Overview
{
    template <typename T>
    void logging_system([[maybe_unused]] T &syscall,
        System::ECS::Query<Battle::BattleState> &global_query,
        System::ECS::Query<Player, Position, Velocity> &player_query,
        System::ECS::Query<Interactable, EventState> &interact_query,
        System::ECS::Query<DialogueBox, EventState> &dialog_query)
    {
        if (global_query.begin() == global_query.end())
            return;

        auto &battle_state = global_query.front().get<Battle::BattleState>();

        if (battle_state.clock_time/1000 % 250 > 2) return;

        LOG_INFO("----------------------------------");

        // const auto &bullet_loader = query.front().get<Battle::BulletLoader>();
        const auto player_pos = player_query.front().get<Position>();
        const auto player_vel = player_query.front().get<Velocity>();
        LOG_INFO("Player Pos : (%d,%d), Vel : (%d,%d)", static_cast<int>(player_pos.x), static_cast<int>(player_pos.y), static_cast<int>(player_vel.vx), static_cast<int>(player_vel.vy));

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