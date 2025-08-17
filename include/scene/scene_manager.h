#pragma once
#include <variant>
namespace Scene
{
    class SceneManager
    {
        std::variant<std::monostate, Utils::from_tuple_t<Config::SceneListT>> _current_scene_template;
        static SceneManager _instance;
    public:
        template<typename T>
        static void change_scene()
        {
            // TODO: invoke Exit of old scene and Init of new scene
            LOG_DEBUG("Info: Changing scene to {}", T::name);
            _instance._current_scene_template = T();
        }
    };
}