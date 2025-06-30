#pragma once

#include <vector>

namespace System
{
    /* TODO: Finish Constructor */
    struct Render2D
    {};

    struct Render3D
    {};

    struct RenderItem
    {
        RenderItem(const Render2D &) {};
        RenderItem(const Render3D &) {};
    };

    template<typename T>
    concept IsValidReturn = std::is_same_v<T, Render2D> || std::is_same_v<T, Render3D>;

    template<typename ResourceManager, auto... RenderFunction>
    struct Renderer
    {
        explicit Renderer(const ResourceManager &resource_manager) : _resource_manager{resource_manager} {};

        std::vector<RenderItem> render_snapshot()
        {
            return (_concat_vector(_render(RenderFunction)...));
        }

    private:
        const ResourceManager &_resource_manager;

        template<typename Func>
        struct function_traits;

        template<typename Return, typename... Args>
        struct function_traits<Return(Args...)>
        {
            using component_tuple = std::tuple<Args...>;
        };

        template<typename Return, typename... Args>
        struct function_traits<Return (*)(Args...)>
        {
            using component_tuple = std::tuple<Args...>;
        };

        template<typename Func>
        struct function_traits : function_traits<decltype(&Func::operator())>
        {};

        template<typename Func, std::size_t... I>
            requires IsValidReturn<std::invoke_result_t<
                    Func,
                    std::tuple_element_t<I, typename function_traits<Func>::component_tuple>...>>
        std::vector<RenderItem> _render_impl(Func render_function, std::index_sequence<I...>)
        {
            using FnTraits = function_traits<Func>;
            using ComponentTuple = typename FnTraits::component_tuple;
            using First = std::remove_reference_t<std::tuple_element_t<0, ComponentTuple>>;

            auto &main_pool = _resource_manager.template query<First>();
            auto cached_pools = std::tie(
                    _resource_manager
                            .template query<std::remove_reference_t<std::tuple_element_t<I, ComponentTuple>>>()...);

            /* TODO: Optimize this by preallocate to the size of max entity count within pool */
            std::vector<RenderItem> items;
            for (const auto &[id, data]: main_pool)
            {
                if ((... && std::get<I>(cached_pools).has(id)))
                {
                    items.push_back(render_function(std::get<I>(cached_pools).get(id)...));
                }
            }

            return (items);
        }

        template<typename Func>
        std::vector<RenderItem> _render(Func render_function)
        {
            using FnTraits = function_traits<Func>;
            using ComponentTuple = typename FnTraits::component_tuple;
            constexpr std::size_t N = std::tuple_size_v<ComponentTuple>;
            return ((_render_impl(render_function, std::make_index_sequence<N>{})));
        }

        template<typename... Vectors>
        std::vector<RenderItem> _concat_vector(Vectors &&...vectors)
        {
            std::vector<RenderItem> result;
            result.reserve((0 + ... + vectors.size()));
            (result.insert(
                     result.end(), std::make_move_iterator(vectors.begin()), std::make_move_iterator(vectors.end())),
             ...);

            return (result);
        }
    };
}
