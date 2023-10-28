#pragma once

#include <type_traits>
#include <string_view>

namespace ge
{
    template <auto MemberPtr>
        requires std::is_member_object_pointer_v<decltype(MemberPtr)>
    struct Member final
    {
        template <typename U>
        static constexpr decltype(auto) get(U&& obj);

        template <typename C, typename V>
        static constexpr void set(C& obj, V&& value);

        std::string_view name;
    };
}

#include "member_impl.hpp"
