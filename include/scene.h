#pragma once

#include "utils.h"

// declare scene templates here
#include "scene/demo.h"

// define configuration items
namespace Scene::Config
{
    // starting scene
    using StartingScene = Demo;

    // list of scenes to use
    using SceneTuple = std::tuple<Demo>;

    using SceneListT = Utils::from_tuple_t<SceneTuple>;
}

#include "scene/scene_manager.h"