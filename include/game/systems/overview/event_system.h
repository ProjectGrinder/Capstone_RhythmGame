#pragma once

namespace Game::Overview
{
    template<typename T>
    void event_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<EventState> &query1,
            System::ECS::Query<EventRegister> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        const auto &event_register = query2.front().components.get<EventRegister>().event_sequences;

        for (auto &[id, comps] : query1)
        {
            auto &event_state = comps.get<EventState>();
            if (!event_state.event_occupied || event_state.has_event) continue;

            event_state.event_idx++;
            if (event_register[event_state.event_id].events.size() == event_state.event_idx)
            {
                event_state.event_occupied = false;
                event_state.event_idx = 0;
            }
            else
            {
                event_state.has_event = true;
                syscall.add_component(id,event_register[event_state.event_id].events[event_state.event_idx]);
            }
        }
    }

    template<typename T>
    void lock_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<GlobalState> &query1,
            System::ECS::Query<EventState, LockInput> &lock_query,
            System::ECS::Query<EventState, UnlockInput> &unlock_query
            )
    {

        if (query1.begin() == query1.end())
            return;

        auto &global_state = query1.front().components.get<GlobalState>();

        for (auto &[id, comps] : lock_query)
        {
            auto &event_state = comps.get<EventState>();
            const auto &lock_input = comps.get<LockInput>();

            if (lock_input.lockBit & 4 > 0)
                global_state.movementLocked = true;

            if (lock_input.lockBit & 2 > 0)
                global_state.interactionLocked = true;

            if (lock_input.lockBit & 1 > 0)
                global_state.menuLocked = true;

            event_state.has_event = false;
        }

        for (auto &[id, comps] : unlock_query)
        {
            auto &event_state = comps.get<EventState>();
            const auto &unlock_input = comps.get<UnlockInput>();

            if (unlock_input.lockBit & 4 > 0)
                global_state.movementLocked = false;

            if (unlock_input.lockBit & 2 > 0)
                global_state.interactionLocked = false;

            if (unlock_input.lockBit & 1 > 0)
                global_state.menuLocked = false;

            event_state.has_event = false;
        }
    }
} // namespace Game::Overview

