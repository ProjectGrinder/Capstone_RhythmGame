#pragma once

#include <functional>

namespace System
{
    using SceneType = std::function<void()>;

    class Game
    {
    private:
        static Game _instance;
    public:
        static void change_scene(SceneType scene);
    };
}