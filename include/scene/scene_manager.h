#pragma once
#include <any>
#include <variant>
#include "demo.h"

namespace Scene
{
    class SceneManager
    {
        std::variant<std::monostate, Demo> _current_scene_template;
        std::any _current_manager;

    public:
        template<typename T>
        static void change_scene()
        {
            auto &scene_template = instance()._current_scene_template;
            std::visit(
                    []<typename S>(S &&scene)
                    {
                        if constexpr (!std::is_same_v<std::decay_t<S>, std::monostate>)
                        {
                            scene.Exit();
                        }
                    },
                    scene_template);

            LOG_DEBUG("Info: Changing scene to {}", T::name);
            scene_template = T::instance();
            instance()._current_manager.reset();
            instance()._current_manager = T::Init();
        }

        static void update()
        {
            std::visit(
                    []<typename S>([[maybe_unused]] S &&scene)
                    {
                        if constexpr (!std::is_same_v<std::decay_t<S>, std::monostate>)
                        {
                            using SceneType = std::decay_t<S>;
                            auto &manager = std::any_cast<typename SceneType::TaskManager&>(instance()._current_manager);
                            manager.run_all();
                        }
                        else
                        {
                            LOG_DEBUG("Warn: No scene loaded during update");
                        }
                    },
                    instance()._current_scene_template);
        }

        static void clean_up()
        {
            LOG_DEBUG("Info: Cleaning up...");
            auto &scene_template = instance()._current_scene_template;
            instance()._current_manager.reset();
            std::visit(
                    []<typename S>(S &&scene)
                    {
                        if constexpr (!std::is_same_v<std::decay_t<S>, std::monostate>)
                        {
                            scene.Exit();
                        }
                    },
                    scene_template);
        }

        static SceneManager &instance();
    };
} // namespace Scene
