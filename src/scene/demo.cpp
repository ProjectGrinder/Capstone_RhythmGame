#include "scene.h"
#include "system.h"
#include "utils.h"

Scene::Demo Scene::Demo::instance()
{
    static Demo instance;
    return (instance);
}

void Scene::Demo::test()
{
    test_component val{1};
    LOG_INFO("test call with val = {}", val.number);
}

Scene::Demo::TaskManager Scene::Demo::Init()
{
    LOG_INFO("Initialization called");
    auto tm = TaskManager{};
    tm.create_entity(test_component{1});
    tm.run_all();

    assets_id vertex_id = load_assets("./shaders/vs/rainbow.cso", "rainbow_vertex_shader", AssetsType::VERTEX_SHADER);
    assets_id pixel_id = load_assets("./shaders/ps/rainbow.cso", "rainbow_pixels_shader", AssetsType::VERTEX_SHADER);

    UNUSED(vertex_id);
    UNUSED(pixel_id);

    return (tm);
}

std::vector<Scene::Demo::ComponentTuple> Scene::Demo::Exit()
{
    LOG_INFO("Exiting Demo Scene.");
    return {};
}
