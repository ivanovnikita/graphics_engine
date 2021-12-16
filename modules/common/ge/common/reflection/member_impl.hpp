#pragma once

#include "member.hpp"

#include <utility>

namespace ge
{
    template <auto MemberPtr>
        requires std::is_member_object_pointer_v<decltype(MemberPtr)>
    template <typename U>
    constexpr decltype(auto) Member<MemberPtr>::get(U&& obj)
    {
        return obj.*MemberPtr;
    }

    template <auto MemberPtr>
        requires std::is_member_object_pointer_v<decltype(MemberPtr)>
    template <typename C, typename V>
    constexpr void Member<MemberPtr>::set(C& obj, V&& value)
    {
        obj.*MemberPtr = std::forward<V>(value);
    }
}
