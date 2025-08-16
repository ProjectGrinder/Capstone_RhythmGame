#include "scene.h"

void Scene::Demo::test()
{
    test_component val;
    val.number = 1;
    LOG_DEBUG("Info: test call with val = {}", val.number);
}

void Scene::Demo::please_work([[maybe_unused]] Syscall &syscall, System::ECS::Query<test_component>& query)
{
    for (auto& entry : query)
    {
        auto& comp = entry.get<test_component>();
        comp.number = 1;
        LOG_DEBUG("Info: WE WIN THIS!!!");
    }
}