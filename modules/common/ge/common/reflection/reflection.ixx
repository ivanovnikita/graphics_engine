module;

#include <tuple>

export module reflection;

export import member;

namespace ge
{
/* Members registration example:

    constexpr auto register_members(const SomeType*) noexcept
    {
        return std::tuple
        {
            Member<&SomeType::member>{"member_name"},
            Member<&SomeType::other_member>{"other_member_name"}
        };
    }
*/

    export template <typename T>
    concept RegisteredMembers = requires
    {
        register_members(static_cast<const std::remove_cvref_t<T>*>(nullptr));
    };

    export template <typename T>
        requires RegisteredMembers<T>
    constexpr auto get_members() noexcept;

    export template <typename T, typename F>
        requires RegisteredMembers<T>
    constexpr void for_each_member(F&&);
}

namespace ge
{
    template <typename T>
        requires RegisteredMembers<T>
    constexpr auto get_members() noexcept
    {
        return register_members(static_cast<const std::remove_cvref_t<T>*>(nullptr));
    }

    namespace refl::detail
    {
        template <typename... Members, typename F, size_t... Ids>
        constexpr void for_each_member(const std::tuple<Members...>& members, F&& func, std::index_sequence<Ids...>)
        {
            (func(std::get<Ids>(members)), ...);
        }

        template <typename... Members, typename F>
        constexpr void for_each_member(const std::tuple<Members...>& members, F&& func)
        {
            for_each_member(members, std::forward<F>(func), std::index_sequence_for<Members...>{});
        }
    }

    template <typename T, typename F>
        requires RegisteredMembers<T>
    constexpr void for_each_member(F&& func)
    {
        refl::detail::for_each_member(get_members<T>(), std::forward<F>(func));
    }
}
