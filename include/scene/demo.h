#pragma once
#include "system.h"
#include "game.h"

namespace Scene
{

    struct Demo
    {
        static Demo instance();

        constexpr static auto name = "Demo";
        // declare scene parameters
        constexpr static size_t MaxResource = 1000;
        using ComponentTuple = std::tuple<Game::Position, Game::Velocity, Game::Rotation, Game::AngularVelocity>;
        using ResourceManager = System::ECS::ResourceManager<MaxResource, Game::Position, Game::Velocity, Game::Rotation, Game::AngularVelocity>;
        using Syscall = System::ECS::Syscall<MaxResource, Game::Position, Game::Velocity, Game::Rotation, Game::AngularVelocity>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall, Game::MovementSystem<Syscall>, Game::RotationSystem<Syscall>>;

        // declare functions
        static void test();
        static TaskManager Init();
        static std::vector<ComponentTuple> Exit();
    };
}
