#pragma once
#include "game/utils/physics_util.h"

namespace Game::BulletHell
{
    // This is a component
    struct Pattern
    {
        uint16_t sequenceID;
        uint16_t sequenceIdx = -1;
        float delay = 0;

        Pattern() : sequenceID(0)
        {}

        explicit Pattern(
                const float sequenceID) :
            sequenceID(sequenceID)
        {}
    };
}