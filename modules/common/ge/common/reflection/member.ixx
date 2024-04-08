module;

#include <type_traits>
#include <string_view>
#include <utility>

export module member;

namespace ge
{
    export template <auto MemberPtr>
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
