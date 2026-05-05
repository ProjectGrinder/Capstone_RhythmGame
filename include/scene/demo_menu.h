#pragma once

#include "game.h"

namespace Scene
{
    struct DemoMenu
    {
        static DemoMenu instance();

        constexpr static auto name = "DemoMenu";
        constexpr static size_t MaxResource = 1000;

        using ComponentTuple = std::tuple<
            Game::Render::Transform,
            Game::Render::Triangle,
            Game::Render::Sprite,
            Game::Render::Material,
            Game::Render::Text,
            Game::Render::Camera2D,
            Game::Render::Flicker,
            Game::Audio::SoundRegistry
        >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::Menu::enter_game<Syscall>,
            Game::Render::flickering_system<Syscall>,
            Game::Render::set_camera<Syscall>,
            Game::Render::draw_sprite<Syscall>,
            Game::Render::draw_text<Syscall>
        >;

        static std::shared_ptr<TaskManager> init();

        static std::shared_ptr<TaskManager> init([[maybe_unused]] ResourceManager &data)
        {
            return (init());
        }
        static ResourceManager exit(std::shared_ptr<TaskManager> &manager);
    };
}