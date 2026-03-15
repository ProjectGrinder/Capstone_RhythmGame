#pragma once

#include "utils.h"

// declare scene templates here
#include "scene/battle_scene.h"
#include "scene/demo.h"
#include "scene/demo_render.h"
#include "scene/demo_bullethell.h"
#include "scene/demo_rhythm.h"

// define configuration items
namespace Scene::Config
{
    // starting scene
    using StartingScene = DemoBulletHell;

} // namespace Scene::Config

#include "scene/scene_manager.h"

