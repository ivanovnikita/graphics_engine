#pragma once

#include <type_traits>
#include <utility>

namespace ge
{
    namespace detail
    {
        template <typename What, template <typename...> typename Template>
        struct IsSpecializationOf
            : std::false_type
        {
        };

        template <template <typename...> typename Template, typename... Args>
        struct IsSpecializationOf<Template<Args...>, Template>
            : std::true_type
        {
        };
    }

    template <typename What, template <typename...> typename Template>
    constexpr bool isSpecializationOf = detail::IsSpecializationOf<What, Template>::value;

    namespace detail
    {
        template <typename What, typename List>
        struct Prepend;

        template <typename What, template <typename...> typename List, typename... Elements>
        struct Prepend<What, List<Elements...>>
        {
            using Type = List<What, Elements...>;
        };
    }

    template <typename What, typename List>
    using Prepend = typename detail::Prepend<What, List>::Type;

    namespace detail
    {
        template
        <
            template <typename...> typename Predicate,
            typename ListOfElements,
            typename... FixedArgs
        >
        struct Filter;

        template
        <
            template <typename...> typename Predicate,
            template <typename...> typename List,
            typename... FixedArgs
        >
        struct Filter<Predicate, List<>, FixedArgs...>
        {
            using Type = List<>;
        };

        template
        <
            template <typename...> typename Predicate,
            template <typename...> typename List,
            typename Head,
            typename... Tail,
            typename... FixedArgs
        >
        struct Filter<Predicate, List<Head, Tail...>, FixedArgs...>
        {
        private:
            using FilteredTail = typename Filter<Predicate, List<Tail...>, FixedArgs...>::Type;

        public:
            using Type = std::conditional_t
            <
                  Predicate<Head, FixedArgs...>::value
                , Prepend<Head, FilteredTail>
                , FilteredTail
            >;
        };
    }

    template
    <
        template <typename...> typename Predicate,
        typename ListOfElements,
        typename... FixedArgs
    >
    using Filter = typename detail::Filter<Predicate, ListOfElements, FixedArgs...>::Type;

    namespace detail
    {
        template <typename List, typename Default>
        struct Head
        {
            using Type = Default;
        };

        template <template <typename...> typename List, typename HeadT, typename... TailT, typename Default>
        struct Head<List<HeadT, TailT...>, Default>
        {
            using Type = HeadT;
        };
    }

    template <typename List, typename Default>
    using HeadOr = typename detail::Head<List, Default>::Type;

    struct NotFound;

    template <template <typename...> typename Predicate, typename List, typename... FixedArgs>
    using Find =  HeadOr<Filter<Predicate, List, FixedArgs...>, NotFound>;

    namespace detail
    {
        template <typename... T>
        struct MergeUnique;

        template <typename List>
        struct MergeUnique<List>
        {
            using Type = List;
        };

        template <template <typename...> typename List, typename ElementToInsert, typename... Elements>
            requires
                std::is_same_v
                <
                    Find<std::is_same, List<Elements...>, ElementToInsert>,
                    NotFound
                >
        struct MergeUnique<List<Elements...>, ElementToInsert>
        {
            using Type = List<Elements..., ElementToInsert>;
        };

        template <template <typename...> typename List, typename ElementToInsert, typename... Elements>
            requires
                (
                    not std::is_same_v
                    <
                        Find<std::is_same, List<Elements...>, ElementToInsert>,
                        NotFound
                    >
                )
        struct MergeUnique<List<Elements...>, ElementToInsert>
        {
            using Type = List<Elements...>;
        };

        template
        <
            template <typename...> typename List,
            typename... LhsElements,
            typename RhsElementsHead,
            typename... RhsElementsTail
        >
        struct MergeUnique<List<LhsElements...>, RhsElementsHead, RhsElementsTail...>
        {
            using Type = typename MergeUnique
            <
                typename MergeUnique<List<LhsElements...>, RhsElementsHead>::Type,
                RhsElementsTail...
            >::Type;
        };

        template
        <
            template <typename...> typename List,
            typename... LhsElements,
            typename... RhsElements
        >
        struct MergeUnique<List<LhsElements...>, List<RhsElements...>>
        {
            using Type = typename MergeUnique<List<LhsElements...>, RhsElements...>::Type;
        };

        template <typename List1, typename List2, typename... Lists>
        struct MergeUnique<List1, List2, Lists...>
        {
            using Type = typename MergeUnique
            <
                typename MergeUnique<List1, List2>::Type,
                Lists...
            >::Type;
        };
    }

    template <typename... Lists>
    using MergeUnique = typename detail::MergeUnique<Lists...>::Type;

    template <typename T>
    concept CopyableFrom = requires
    (T x)
    {
        T::copy_from(x);
    };

    template <typename T>
    struct CopyableFromT final
    {
        static constexpr bool value = CopyableFrom<T>;
    };

    template <typename T, typename... Args>
    concept Creatable = requires
    (Args&&... args)
    {
        T::create(std::forward<Args>(args)...);
    };

    template <typename T>
    concept Resultable = requires
    (T x)
    {
        x.is_ok();
        x.is_error();
        // TODO: add more checks
    };

    template <typename T>
    concept Optionable = requires
    (T x)
    {
        x.is_some();
        x.is_none();
        // TODO: add more checks
    };

    template <typename T>
    using Ref = std::reference_wrapper<T>;
}
