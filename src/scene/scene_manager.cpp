#include "scene.h"

Scene::SceneManager &Scene::SceneManager::instance()
{
    static Scene::SceneManager instance;
    return instance;
}
