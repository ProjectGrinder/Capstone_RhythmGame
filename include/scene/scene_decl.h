#pragma once

// define scenes
namespace Scene
{
    struct Demo;
    struct DemoRender;
    struct DemoRhythm;
    struct BattleScene;
    struct DemoGame;

    template <typename T>
    void queue_change_scene();
}