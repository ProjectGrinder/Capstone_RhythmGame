#pragma once
#include "event_object.h"

namespace Game::Overview
{
    struct Interactable
    {
        bool active = true;
        bool in_range = false;
    };
} // namespace Game::Overview
