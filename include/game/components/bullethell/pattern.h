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

        Pattern(const uint16_t sequenceID,
                const float p0 = 0,
                const float p1 = 0,
                const float p2 = 0,
                const float p3 = 0) : sequenceID(sequenceID), params{p0,p1,p2,p3}
        {}


        //TODO : Constructor with params
    };
} // namespace Game::BulletHell
