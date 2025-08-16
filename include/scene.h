#pragma once

#include "utils.h"

// declare scene templates here
#include "scene/demo.h"

// define configuration items
namespace Scene::Config
{
    using StartingScene = Demo;
    using SceneTuple = std::tuple<Demo>;
    using SceneType = Utils::from_tuple_t<SceneTuple>;
}

// declare SceneManager here, which sees the Scene::Config namespace
#include "scene/scene_manager.h"