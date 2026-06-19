#pragma once

#include "game.h"

namespace Scene
{
    struct DemoMenu
    {
        template<typename T>
        static void enter_game([[maybe_unused]] T &syscall)
        {
            constexpr auto ENTER = 0x0D;

            if (get_key_state(ENTER))
            {
                LOG_INFO("Enter pressed");
                Scene::queue_change_scene<DemoWorld>();
            }
        }

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
            enter_game<Syscall>,
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