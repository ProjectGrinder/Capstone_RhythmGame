#pragma once
#include <any>
#include <variant>
#include "demo.h"

namespace Scene
{
    /* Optimizable */
    class SceneManager
    {
        std::variant<std::monostate, Demo, BattleScene, DemoRhythm> _current_scene_template;
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

            LOG_INFO("Info: Changing scene to %s", T::name);
            scene_template = T::instance();
            instance()._current_manager.reset();
            instance()._current_manager = T::Init();
        }

        static void update();
        ~SceneManager();
        // static void init(System::Renderer::VertexGeneratorQueue<System::Config::VertexQueueSize> *queue);

        static SceneManager &instance();
    };
} // namespace Scene
