#pragma once

#include "utils.h"

// declare scene templates here
#include "scene/demo.h"
#include "scene/battle_scene.h"

// define configuration items
namespace Scene::Config
{
    // starting scene
    using StartingScene = BattleScene;

}

#include "scene/scene_manager.h"