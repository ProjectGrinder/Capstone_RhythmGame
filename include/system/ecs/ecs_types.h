#pragma once
#include <cstdint>
#include <vector>

namespace System::ECS
{
    using pid = std::uint64_t;

    template<std::size_t MaxResource, typename Resource>
    class ResourcePool;

    template<std::size_t MaxResource, typename... Resources>
    class ResourceManager;

    template<std::size_t MaxResource, typename... Resources>
    class SyscallResource;

    template<std::size_t MaxResource, typename... Resources>
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
    class Query;

    template<typename T>
    struct is_query : std::false_type
    {};

    template<typename... Components>
    struct is_query<Query<Components...>> : std::true_type
    {};

    template<typename T>
    concept QueryType = is_query<std::remove_cvref_t<T>>::value;

    template<typename T>
    struct function_traits;

    template<typename Ret, typename... Args>
    struct function_traits<Ret(Args...)>
    {
        using args_tuple = std::tuple<Args...>;
    };

    template<typename Ret, typename... Args>
    struct function_traits<Ret (*)(Args...)>
    {
        using args_tuple = std::tuple<Args...>;
    };

    template<typename Ret, typename Class, typename... Args>
    struct function_traits<Ret (Class::*)(Args...) const>
    {
        using args_tuple = std::tuple<Args...>;
    };

    template<typename T>
    struct function_traits : function_traits<decltype(&T::operator())>
    {};

    template<typename T>
    concept TaskConcept =
            requires { typename function_traits<std::remove_cvref_t<T>>::args_tuple; } && []<typename F>(F *)
    {
        using traits = function_traits<std::remove_cvref_t<F>>;
        using args = typename traits::args_tuple;
        constexpr std::size_t N = std::tuple_size_v<args>;
        if constexpr (N < 1)
            return false;
        using First = std::tuple_element_t<0, args>;
        if constexpr (!std::is_lvalue_reference_v<First>)
            return false;
        if constexpr (!SyscallType<std::remove_reference_t<First>>)
            return false;
        if constexpr (N == 1)
            return true;
        // Check all remaining args
        return (
                []<std::size_t... I>(std::index_sequence<I...>)
                {
                    return ((std::is_lvalue_reference_v<std::tuple_element_t<I + 1, args>> &&
                             QueryType<std::remove_reference_t<std::tuple_element_t<I + 1, args>>>) &&
                            ...);
                })(std::make_index_sequence<N - 1>{});
    }(static_cast<T *>(nullptr));

    template<typename T, typename... Ts>
    constexpr bool contains_type_v = (std::is_same_v<T, Ts> || ...);

    template<typename Registry, SyscallType Syscall, auto... Tasks>
        requires(TaskConcept<decltype(Tasks)> && ...)
    class TaskManager;
} // namespace System::ECS
