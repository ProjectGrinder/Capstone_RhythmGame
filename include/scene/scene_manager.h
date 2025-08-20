#pragma once
#include <variant>
#include <any>
namespace Scene
{
    class SceneManager
    {
        std::variant<std::monostate, Scene::Demo> _current_scene_template;
        std::any _current_manager;
    public:
        template<typename T>
        static void change_scene()
        {
            auto& scene_template = instance()._current_scene_template;
            std::visit([](auto&& scene) {
                if constexpr (!std::is_same_v<std::decay_t<decltype(scene)>, std::monostate>) {
                    scene.Exit();
                }
            }, scene_template);

            LOG_DEBUG("Info: Changing scene to {}", T::name);
            scene_template = T::instance();
            instance()._current_manager = T::Init();
            auto manager = std::any_cast<typename T::TaskManager>(instance()._current_manager);
            manager.run_all();
        }

        static SceneManager& instance();
    };
}