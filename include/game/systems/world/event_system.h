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

            LOG_INFO("Event : %d", event_state.event_idx);
            if (event_register[event_state.event_id].events.size() == event_state.event_idx)
            {
                event_state.event_occupied = false;
                event_state.event_idx = 0;
            }
            else
            {
                event_state.has_event = true;
                auto& e = event_register[event_state.event_id].events[event_state.event_idx];

                // Idk what this does. It like... turn variant object into component???
                std::visit([&](auto&& evt)
                {
                    using T = std::decay_t<decltype(evt)>;
                    syscall.add_component(id, T(evt));   // force clean type
                }, e);

                event_state.event_idx++;
            }

        }
    }

    template<typename T>
    void lock_event_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<GlobalState> &query1,
            System::ECS::Query<EventState, LockInputEvent> &lock_query,
            System::ECS::Query<EventState, UnlockInputEvent> &unlock_query
            )
    {

        if (query1.begin() == query1.end())
            return;

        auto &[movementLocked, interactionLocked, menuLocked] = query1.front().components.get<GlobalState>();

        for (auto &[id, comps] : lock_query)
        {
            auto &event_state = comps.get<EventState>();
            const auto &lock_input = comps.get<LockInputEvent>();

            if ((lock_input.lockBit & 4) > 0)
                movementLocked = true;

            if ((lock_input.lockBit & 2) > 0)
                interactionLocked = true;

            if ((lock_input.lockBit & 1) > 0)
                menuLocked = true;

            LOG_INFO("Entity ID : %d",id);

            event_state.has_event = false;
            syscall.template remove_component<LockInputEvent>(id);
        }

        for (auto &[id, comps] : unlock_query)
        {
            auto &event_state = comps.get<EventState>();
            const auto &unlock_input = comps.get<UnlockInputEvent>();

            if ((unlock_input.lockBit & 4) > 0)
                movementLocked = false;

            if ((unlock_input.lockBit & 2) > 0)
                interactionLocked = false;

            if ((unlock_input.lockBit & 1) > 0)
                menuLocked = false;

            event_state.has_event = false;
            syscall.template remove_component<UnlockInputEvent>(id);
        }
    }

    template<typename T>
    void change_event_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<EventState, ChangeNextEvent> &query1
            )
    {

        for (auto &[id, comps] : query1)
        {
            auto &event_state = comps.get<EventState>();

            event_state.event_id = comps.get<ChangeNextEvent>().event_id;
            event_state.has_event = false;
            syscall.template remove_component<ChangeNextEvent>(id);
        }
    }

} // namespace Game::Overview

