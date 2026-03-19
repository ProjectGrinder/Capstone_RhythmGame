#pragma once
#include "game/utils/physics_util.h"

namespace Game::BulletHell
{
    struct BulletClearer
    {
        float max_size;
        int speed;
        int lifetime = static_cast<int>(UNASSIGNED);
        BulletClearer() : max_size(10), speed(1) {}
        BulletClearer(const float size, const int speed) : max_size(size), speed(speed) {}
    };
} // namespace Game::BulletHell
