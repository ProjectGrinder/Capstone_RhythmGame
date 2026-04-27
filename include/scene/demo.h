#pragma once
#include "game.h"
#include "system.h"
#include "scene/scene_decl.h"

namespace Scene
{

    struct test_component
    {
        int number;
    };

    template<typename T>
    void please_work([[maybe_unused]] T &syscall, System::ECS::Query<test_component> &query)
    {
        for (auto &entry: query)
        {
            auto &comp = entry.get<test_component>();
            LOG_INFO("comp.number = %d", comp.number)
            comp.number += 1;
            if (comp.number > 1000)
            {
                queue_change_scene<Scene::DemoSceneChange>();
            }
        }
    }

    // template <typename T>
    // void please_work2([[maybe_unused]] T &syscall, System::ECS::Query<System::Renderer::Render2D>& query)
    // {
    //     for (auto& entry : query)
    //     {
    //         entry.get<System::Renderer::Render2D>();
    //     }
    // }

    struct Demo
    {

        static Demo instance();

        constexpr static auto name = "Demo";
        // declare scene parameters
        constexpr static size_t MaxResource = 1000;
        using ComponentTuple = std::tuple<test_component>;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall, please_work<Syscall>>;

        // declare functions
        static void test();
        static std::shared_ptr<TaskManager> init();
        static std::shared_ptr<TaskManager> init([[maybe_unused]] ResourceManager &data)
        {
            return (init());
        }
        static ResourceManager exit(std::shared_ptr<TaskManager> &manager);
    };
} // namespace Scene
