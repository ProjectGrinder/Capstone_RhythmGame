#pragma once
#include <variant>
namespace Scene
{
    class SceneManager
    {
        std::variant<std::monostate, Config::SceneType> _current_scene;
    };
}