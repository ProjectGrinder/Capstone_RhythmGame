#pragma once

#include <functional>

namespace Scene
{
    using SceneType = std::function<void()>;

    /* Include Your Scene Here */
    #include "scene/demo.h"
    #include "scene/demo2.h"

    /* Config Here (You can add more if you want to) */
    namespace SceneConfig
    {
        const SceneType starting_scene = Demo::Demo;
    }
}
