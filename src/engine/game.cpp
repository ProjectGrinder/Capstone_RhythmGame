#include <system.h>
#include <ecs.h>

using System::Game;

Game Game::_instance;

void Game::change_scene(SceneType next_scene)
{
    next_scene();
}