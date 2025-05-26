#include "scene.h"
#include "utils.h"
#include "system.h"

namespace Scene::Rhythm
{
	void Start()
	{
		LOG_DEBUG("Rhythm time!");
		test();
	}

	void test()
	{
		LOG_DEBUG("test call in Rhythm scene");
	}
}