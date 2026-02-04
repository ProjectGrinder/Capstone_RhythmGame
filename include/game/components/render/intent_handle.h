#pragma once
#include "system/intent_storage.h"

namespace Game::Render
{
    struct IntentHandle
    {
        size_t handle_id;

        IntentHandle() : handle_id(System::RenderStorage::alloc_slot())
        {}
        explicit IntentHandle(const size_t handle_id) : handle_id(handle_id)
        {}
    };
} // namespace Game::Render
