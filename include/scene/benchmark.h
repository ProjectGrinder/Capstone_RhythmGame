#pragma once
#include "game.h"
#include "system.h"
#include "scene/scene_decl.h"

extern "C" long double get_delta_time();

namespace Scene
{

    struct benchmark_position
    {
        float x, y;
    };

    struct benchmark_velocity
    {
        float x, y;
    };

    template<typename T>
    void benchmark_movement([[maybe_unused]] T &syscall, System::ECS::Query<benchmark_position, benchmark_velocity> &query)
    {
        for (auto &entry: query)
        {
            auto &pos = entry.get<benchmark_position>();
            auto &vel = entry.get<benchmark_velocity>();

            auto clock_time = static_cast<int>(get_delta_time() * 1000) / 1000.0f;
            pos.x += vel.x * clock_time / 1000.0f;
            pos.y += vel.y * clock_time / 1000.0f;
        }
    }

    struct Benchmark
    {

        static Benchmark instance();

        constexpr static auto name = "Benchmark";
        // declare scene parameters
        constexpr static size_t MaxResource = 1000;
        using ComponentTuple = std::tuple<benchmark_position, benchmark_velocity>;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall, benchmark_movement<Syscall>>;

        static std::shared_ptr<TaskManager> init();
        static std::shared_ptr<TaskManager> init([[maybe_unused]] ResourceManager &data)
        {
            return (init());
        }
        static ResourceManager exit(std::shared_ptr<TaskManager> &manager);
    };
} // namespace Scene
