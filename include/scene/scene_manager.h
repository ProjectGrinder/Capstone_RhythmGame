#pragma once
#include <any>
#include <variant>

namespace Scene
{
    /* Optimizable */
    class SceneManager
    {
        Config::SceneVariant _current_scene_template = std::monostate{};
        Config::TaskManagerVariant _current_manager = std::monostate{};

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
                            scene.exit();
                        }
                    },
                    scene_template);

            LOG_INFO("Info: Changing scene to %s", T::name);
            scene_template = T::instance();
            // instance()._current_manager.reset();
            instance()._current_manager = T::init();
        }

        static void update();
        ~SceneManager();
        // static void init(System::Renderer::VertexGeneratorQueue<System::Config::VertexQueueSize> *queue);

        static SceneManager &instance();
    };
} // namespace Scene
