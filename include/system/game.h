#pragma once

namespace System
{
    // template<typename Scene>
    // concept SceneTraits = requires
    // {
    //     // {Scene::MaxEntity} -> std::convertible_to<std::size_t>;
    //     //
    //     // typename Scene::Components;
    //     // typename Scene::OnInit;
    //     // typename Scene::OnUpdate;
    //     // typename Scene::OnRender;
    // };

    class Game
    {
    public:
        template<typename Scene>
        static void change_scene()
        {
        }
    };
}