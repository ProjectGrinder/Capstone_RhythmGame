#pragma once

#include "utils.h"

// declare scene templates here
#include "scene/demo.h"

// define configuration items
namespace Scene::Config
{
    // starting scene
    using StartingScene = Demo;

}

#include "scene/scene_manager.h"