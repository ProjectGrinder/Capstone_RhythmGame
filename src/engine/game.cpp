#include <system.h>

using System::Game;

Game Game::_instance;

void Game::change_scene(SceneType next_scene)
{
    next_scene();
}