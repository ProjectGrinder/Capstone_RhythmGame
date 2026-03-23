#pragma once

namespace Game::Overview
{
    struct PlayerStat
    {
        float walk_speed;
        float run_speed;
        float jump_height;
        float gravity;

        PlayerStat() : walk_speed(1), run_speed(4), jump_height(3), gravity(1)
        {}

        PlayerStat(const float walk_speed, const float run_speed, const float jump_height, const float gravity) : walk_speed(walk_speed), run_speed(run_speed), jump_height(jump_height), gravity(gravity) {}
    };
} // namespace Game::BulletHell
