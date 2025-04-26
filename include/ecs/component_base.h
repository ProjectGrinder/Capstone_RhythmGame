#pragma once
#include <concepts>

namespace ECS
{
	struct ComponentBase {

	};

	template<typename T>
	concept ComponentType = std::derived_from<T, ComponentBase>;
}