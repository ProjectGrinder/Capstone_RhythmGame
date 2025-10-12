#pragma once
#include <any>
#include <variant>
#include "system.h"
#include "demo.h"

namespace Scene
{
    /* Optimizable */
    class SceneManager
    {
        std::variant<std::monostate, Demo> _current_scene_template;
        std::any _current_manager;
        System::Renderer::VertexGeneratorQueue<System::Config::VertexQueueSize>* _generator_queue = nullptr;

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

        static void update();
        static void clean_up();
        static void init(System::Renderer::VertexGeneratorQueue<System::Config::VertexQueueSize> *queue);

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
