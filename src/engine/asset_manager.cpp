#include "system.h"

using System::AssetManager;

AssetManager AssetManager::_instance;

AssetManager& System::AssetManager::instance()
{
	return (_instance);
}