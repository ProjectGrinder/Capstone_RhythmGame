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

    /// <summary>
    /// Tuple unpacking for template parameters
    /// </summary>
    /// <param name="T">the tuple to unpack</param>
    template<typename T>
    using from_tuple_t = typename from_tuple<T>::type;
} // namespace Utils
