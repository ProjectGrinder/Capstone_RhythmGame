#pragma once
#include <concepts>

namespace ECS
{
    struct ComponentBase {
        virtual ~ComponentBase() = default;
    };

    template<typename T>
    concept ComponentType = std::derived_from<T, ComponentBase>;
}