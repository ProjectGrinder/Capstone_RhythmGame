#pragma once

namespace Game::World
{
    struct PlayerStat
    {
        float walk_speed;
        float run_speed;
        float jump_height;
        float gravity;

        PlayerStat() : walk_speed(100), run_speed(400), jump_height(20), gravity(0)//gravity(0.098)
        {}

        PlayerStat(const float walk_speed, const float run_speed, const float jump_height, const float gravity) : walk_speed(walk_speed), run_speed(run_speed), jump_height(jump_height), gravity(gravity) {}
    };
} // namespace Game::Overview
