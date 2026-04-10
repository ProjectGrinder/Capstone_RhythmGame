#pragma once
#include <any>
#include <variant>

// define configuration items
namespace Scene::Config
{
    // starting scene
    using StartingScene = DemoRender;

    using SceneTuple = std::tuple<Demo, DemoRender, DemoRhythm, BattleScene, DemoGame>;
    using SceneVariant = Utils::make_scene_variant_t<SceneTuple>;
    using TaskManagerVariant = Utils::make_task_manager_variant_t<SceneTuple>;

} // namespace Scene::Config


namespace Scene
{
    class SceneManager
    {
        Config::SceneVariant _current_scene_template = std::monostate{};
        Config::TaskManagerVariant _current_manager = std::monostate{};
        Config::SceneVariant _next_scene_template = std::monostate{};
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

        template <typename T>
        static void queue_change_scene()
        {
            instance()._next_scene_template = T{};
        }

        static void update();
        ~SceneManager();
        // static void init(System::Renderer::VertexGeneratorQueue<System::Config::VertexQueueSize> *queue);

        static SceneManager &instance();
    };

    template <typename T>
    void queue_change_scene()
    {
        SceneManager::queue_change_scene<T>();
    }
} // namespace Scene