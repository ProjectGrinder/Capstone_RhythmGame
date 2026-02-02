#pragma once

namespace Game::Render
{
    struct IntentHandle
    {
        size_t handle_id;

        IntentHandle() : handle_id(System::IntentStorage::alloc_slot()) {}
        explicit IntentHandle(const size_t handle_id) : handle_id(handle_id) {}
    };
}