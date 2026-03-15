#pragma once
#include "game.h"

namespace Scene
{
    struct DemoRender
    {
        static DemoRender instance();

        constexpr static auto name = "DemoRender";

        // declare scene parameters
        constexpr static size_t MaxResource = 2000;
        using ComponentTuple = std::tuple<
                Game::Render::Camera2D,
                Game::Render::Sprite,
                Game::Render::Material,
                Game::Render::Text,
                Game::Render::IntentHandle,
                Game::Render::Transform,
                Game::Render::Triangle>;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<
                ResourceManager,
                Syscall,
                Game::Render::spin_camera<Syscall>,
                Game::Render::spin_triangle<Syscall>,
                Game::Render::set_camera<Syscall>,
                Game::Render::material_intent<Syscall>,
                Game::Render::triangle_intent<Syscall>,
                Game::Render::transform_intent<Syscall>>;

        // declare functions
        static std::shared_ptr<TaskManager> init();
        static std::vector<ComponentTuple> exit();
    };
} // namespace Scene
