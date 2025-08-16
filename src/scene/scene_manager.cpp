#include "scene.h"

Scene::SceneManager Scene::SceneManager::_instance;

template<typename T>
void Scene::SceneManager::change_scene()
{
    // TODO: invoke Exit of old scene and Init of new scene
    _instance._current_scene_template = T();
}
