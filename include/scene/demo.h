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
        using ComponentTuple = std::tuple<Game::BulletHell::Position, Game::BulletHell::Velocity, Game::BulletHell::Rotation, Game::BulletHell::AngularVelocity>;
        using ResourceManager = System::ECS::ResourceManager<MaxResource, Game::BulletHell::Position, Game::BulletHell::Velocity, Game::BulletHell::Rotation, Game::BulletHell::AngularVelocity>;
        using Syscall = System::ECS::Syscall<MaxResource, Game::BulletHell::Position, Game::BulletHell::Velocity, Game::BulletHell::Rotation, Game::BulletHell::AngularVelocity>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall, Game::BulletHell::MovementSystem<Syscall>, Game::BulletHell::RotationSystem<Syscall>>;

        // declare functions
        static void test();
        static TaskManager Init();
        static std::vector<ComponentTuple> Exit();
    };
}
