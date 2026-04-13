#pragma once
#include <any>
#include <variant>

#include "scene/scene_decl.h"
#include "scene/demo.h"
#include "scene/battle_scene.h"
#include "scene/demo_game.h"
#include "scene/demo_render.h"
#include "scene/demo_rhythm.h"
#include "utils/print_debug.h"
#include "utils/scene_utils.h"

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
            LOG_INFO("Info: Changing scene to %s", T::name);
            std::visit(
                    []<typename S>(S &&scene)
                    {
                        if constexpr (!std::is_same_v<std::decay_t<S>, std::monostate>)
                        {
                            // using TaskManager = typename std::decay_t<S>::TaskManager;
                            using ComponentTuple = std::decay_t<S>::ComponentTuple;
                            // const std::shared_ptr<TaskManager> &manager = std::get<std::shared_ptr<TaskManager>>(instance()._current_manager);
                            std::vector<ComponentTuple> data = scene.exit();
                            instance()._current_scene_template = T::instance();
                            instance()._current_manager = T::init();
                        }
                        else
                        {
                            instance()._current_scene_template = T::instance();
                            instance()._current_manager = T::init();
                        }
                    },
                    scene_template);

        }

        template<typename T>
        static void queue_change_scene()
        {
            instance()._next_scene_template = T::instance();
        }

        static void update();
        ~SceneManager();
        // static void init(System::Renderer::VertexGeneratorQueue<System::Config::VertexQueueSize> *queue);

        static SceneManager &instance();
    };

    template<typename T>
    void queue_change_scene()
    {
        SceneManager::queue_change_scene<T>();
    }
} // namespace Scene
