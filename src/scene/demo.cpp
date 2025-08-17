#include "scene.h"

void Scene::Demo::test()
{
    test_component val;
    val.number = 1;
    LOG_DEBUG("Info: test call with val = {}", val.number);
}