#pragma once
#include "component_base.h"
#include <map>

namespace ECS
{
    template <ComponentType... Components>
    using EntityMap = std::map<entity_id, std::tuple<Components&...>>;

    template <ComponentType... Components>
    using SystemFunction = void (*)(EntityMap<Components...>);
}