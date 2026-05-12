#pragma once
#include "game/utils/physics_util.h"

namespace Game::BulletHell
{
    // This is a component
    struct Pattern
    {
        uint16_t sequenceID;
        int sequenceIdx = -1; // Would not be good but require for init
        int delay = 0;

        float params[4];

        Pattern() : sequenceID(0), params{}
        {}

        explicit Pattern(const uint16_t sequenceID) : sequenceID(sequenceID), params{}
        {}

        //TODO : Constructor with params
    };
} // namespace Game::BulletHell
