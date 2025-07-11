#pragma once
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace System::ECS
{
    using pid = std::uint64_t;

    template <std::size_t MaxResource, typename Resource>
    class ResourcePool;

    template <std::size_t MaxResource, typename... Resources>
    class ResourceManager;

    template<size_t MaxResource, typename... Resources>
    class Syscall;

    template<typename T>
    struct is_syscall : std::false_type
    {};

    template<size_t MaxResource, typename... Resources>
    struct is_syscall<Syscall<MaxResource, Resources...>> : std::true_type
    {};

    template<typename T>
    concept SyscallType = is_syscall<std::remove_cvref_t<T>>::value;

    template<typename... Components>
    struct Query;

    template<typename T>
    struct is_query : std::false_type
    {};

    template<typename... Components>
    struct is_query<Query<Components...>> : std::true_type
    {};

    template<typename T>
    concept QueryType = is_query<std::remove_cvref_t<T>>::value;

    template<typename Registry, SyscallType Syscall, auto... Task>
    class TaskManager;


}