#include "scene.h"

#include <random>

Scene::Benchmark Scene::Benchmark::instance()
{
    static Benchmark instance;
    return instance;
}

std::shared_ptr<Scene::Benchmark::TaskManager> Scene::Benchmark::init()
{
    std::random_device r;
    std::default_random_engine engine(r());
    std::uniform_real_distribution roll(0.0f, 1.0f);

    auto tm = std::make_shared<TaskManager>();
    for (size_t i = 0; i < MaxResource; i++)
    {
        tm->create_entity<benchmark_position, benchmark_velocity>(
            benchmark_position{
                -100 + (200 * roll(engine)),
                -100 + (200 * roll(engine))
            },
            benchmark_velocity{
                -1 + (2 * roll(engine)),
                -1 + (2 * roll(engine))
            }
        );
    }

    return (tm);
}

Scene::Benchmark::ResourceManager Scene::Benchmark::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting Demo Render Scene.");
    return ResourceManager();
}
