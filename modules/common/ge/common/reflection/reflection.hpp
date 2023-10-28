#pragma once

#include "member.hpp"

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

    template <typename T>
    concept RegisteredMembers = requires
    {
        register_members(static_cast<const std::remove_cvref_t<T>*>(nullptr));
    };

    template <typename T>
        requires RegisteredMembers<T>
    constexpr auto get_members() noexcept;

    template <typename T, typename F>
        requires RegisteredMembers<T>
    constexpr void for_each_member(F&&);
}

#include "reflection_impl.hpp"
