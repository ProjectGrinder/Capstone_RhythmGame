#pragma once

// define scenes
namespace Scene
{

    struct Demo;
    struct DemoSceneChange;
    struct Level1;
    struct Level2;
    struct DemoWorld;
    struct DemoMenu;

    template <typename T>
    void queue_change_scene();
}