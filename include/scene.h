#pragma once

#include "utils.h"

// declare scene templates here
#include "scene/battle_scene.h"
#include "scene/demo.h"
#include "scene/demo_bullethell.h"
#include "scene/demo_game.h"
#include "scene/demo_render.h"
#include "scene/demo_rhythm.h"

// define configuration items
namespace Scene::Config
{
    // starting scene
    using StartingScene = DemoBulletHell;

    using SceneTuple = std::tuple<Demo, DemoRender, DemoRhythm, BattleScene, DemoGame, DemoBulletHell>;
    using SceneVariant = Utils::make_scene_variant_t<SceneTuple>;
    using TaskManagerVariant = Utils::make_task_manager_variant_t<SceneTuple>;

} // namespace Scene::Config

#include "scene/scene_manager.h"
