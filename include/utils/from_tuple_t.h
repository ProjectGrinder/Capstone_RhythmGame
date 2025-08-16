#pragma once
#include <tuple>
namespace Utils
{
    template<typename T>
    struct from_tuple;

    template<typename... Ts>
    struct from_tuple<std::tuple<Ts...>>
    {
        using type = std::tuple<Ts...>;
    };

    template<typename T>
    using from_tuple_t = typename from_tuple<T>::type;
}