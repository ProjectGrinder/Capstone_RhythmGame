#pragma once
#include "game.h"

namespace Scene
{
    struct DemoRender
    {
        static DemoRender instance();

        constexpr static auto name = "DemoRender";

        // declare scene parameters
        constexpr static size_t MaxResource = 12000;
        using ComponentTuple = std::tuple<
                Game::Physics::Rotation,
                Game::Render::Camera2D,
                Game::Render::Sprite,
                Game::Render::Material,
                Game::Render::Text,
                Game::Render::Transform,
                Game::Render::Triangle>;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<
                ResourceManager,
                Syscall,
                Game::Render::spin_sprite<Syscall>,
                Game::Render::set_camera<Syscall>,
                Game::Render::draw_triangle<Syscall>,
                Game::Render::draw_sprite<Syscall>,
                Game::Render::draw_text<Syscall>>;

        // declare functions
        static std::shared_ptr<TaskManager> init();
        static std::vector<ComponentTuple> exit();
    };
} // namespace Scene
