#pragma once
#include <cstddef>
#include <cstdint>
#include <concepts>

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
    concept SyscallType = requires(T t) {
            { t.exec() } -> std::same_as<void>;
    };

    template<typename Registry, SyscallType Syscall, auto... Task>
    class TaskManager;

}