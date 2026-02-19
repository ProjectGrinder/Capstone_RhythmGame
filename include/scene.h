#pragma once

#include "utils.h"

// declare scene templates here
#include "scene/battle_scene.h"
#include "scene/demo.h"
#include "scene/demo_rhythm.h"

// define configuration items
namespace Scene::Config
{
    // starting scene
    using StartingScene = BattleScene;

} // namespace Scene::Config

#include "scene/scene_manager.h"
