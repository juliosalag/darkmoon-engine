#pragma once

#include <cstdint>

namespace MP{

    // Define constant value for compile-time
    template <typename T, T VAL>
    struct constant { static constexpr T value{ VAL }; };
    struct true_type  : constant<bool, true>  {};
    struct false_type : constant<bool, false> {};

    // Forward declaration
    template <typename... Ts>
    struct TypeList;

    // is_same
    template <typename T, typename U> struct is_same : false_type {};
    template <typename T>             struct is_same<T, T> : true_type {};
    template <typename T, typename U>
    constexpr bool is_same_v{ is_same<T, U>::value };

    // type_id
    template <typename T>
    struct type_id { using type = T; };

    // nth_type
    template <std::size_t N, typename... Ts>
    struct nth_type { static_assert(sizeof...(Ts) > 0, "TypeList is empty"); };

    template <std::size_t N, typename... Ts>
    using nth_type_t = typename nth_type<N, TypeList<Ts...>>::type;

    template <typename T, typename... Ts>
    struct nth_type<0, T, Ts...> : type_id<T> {};

    template <std::size_t N, typename T, typename... Ts>
    struct nth_type<N, T, Ts...> : type_id<nth_type_t<N - 1, Ts...>> {};

    // pos
    template <typename T, typename... Ts>
    struct pos { static_assert(sizeof...(Ts) > 0, "TypeList is empty"); };

    template <typename T, typename... Ts>
    constexpr std::size_t pos_v{ pos<T, Ts...>::value };

    template <typename T, typename... Ts>
    struct pos<T, T, Ts...> : constant<std::size_t, 0> {};

    template <typename T, typename U, typename... Ts>
    struct pos<T, U, Ts...> : constant<std::size_t, 1 + pos_v<T, Ts...>> {};

    // IFT
    template <bool Condition, typename T, typename F> struct IFT : type_id<F> {};
    template <typename T, typename F> struct IFT<true, T, F> : type_id<T> {};
    template <bool Condition, typename T, typename F>
    using IFT_t = typename IFT<Condition, T, F>::type;

    // TypeList
    template <typename... Ts>
    struct TypeList {
        consteval static std::size_t size()    noexcept { return sizeof...(Ts); }

        template <typename T>
        consteval static bool contains() noexcept { return (false || ... || is_same_v<T, Ts>); }

        template <typename T>
        consteval static std::size_t pos() noexcept {
            static_assert(contains<T>(), "Type not found");
            return pos_v<T, Ts...>;
        }
    };

    // Get the smallest type
    template <typename LIST>
    using smallest_type =
        IFT_t<LIST::size() <= 8, std::uint8_t,
        IFT_t<LIST::size() <= 16, std::uint16_t,
        IFT_t<LIST::size() <= 32, std::uint32_t,
        std::uint64_t>>>;

    // replace_t: TypeList<A,B,C> --> New<A,B,C>
    template <template <typename...> class New, typename List> struct replace {};
    template <template <typename...> class New, typename... Ts>
    struct replace<New, TypeList<Ts...>> : type_id<New<Ts...>> {};
    template <template <typename...> class New, typename List>
    using replace_t = typename replace<New, List>::type;

    // forall_insert_template_t: TypeList<A,B,C> --> TypeList<Wrap<A>, Wrap<B>, Wrap<C>>
    template <template <typename> class New, typename List> struct forall_insert_template {};
    template <template <typename> class New, typename... Ts>
    struct forall_insert_template<New, TypeList<Ts...>> : type_id<TypeList<New<Ts>...>> {};
    template <template <typename> class New, typename List>
    using forall_insert_template_t = typename forall_insert_template<New, List>::type;

    template <typename TLIST>
    struct cmp_tag_traits
    {
        static_assert(TLIST::size() <= 64, "Component tag list is too large");

        // Metafunction to determine the best data type to store the mask (its size)
        using mask_type = smallest_type<TLIST>;

        // Compile-time function to determine the list size
        consteval static uint8_t size() noexcept { return TLIST::size(); }

        // Template that tells us the position of a type (component or tag) in the list
        template <typename CMPTAG>
        consteval static uint8_t id() noexcept
        {
            static_assert(TLIST::template contains<CMPTAG>(), "Component or Tag not found");
            return TLIST::template pos<CMPTAG>();
        }

        // Template that generates a bitmask at compile-time with the types you pass it
        template <typename... Ts>
        consteval static mask_type mask() noexcept
        {
            return (0 | ... | (1LL << id<Ts>()));
        }
    };

    // void typelist
    template <>
    struct cmp_tag_traits<TypeList<>> {
        using mask_type = std::uint8_t;
        consteval static std::uint8_t size() noexcept { return 0; }
        template <typename...> consteval static mask_type mask() noexcept { return 0; }
    };

    // for_each_in_tuple (const)
    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    for_each_in_tuple(const std::tuple<Tp...>&, auto) {}

    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<(I < sizeof...(Tp)), void>::type
    for_each_in_tuple(const std::tuple<Tp...>& t, auto f) {
        f(std::get<I>(t));
        for_each_in_tuple<I + 1, Tp...>(t, f);
    }

    // for_each_in_tuple (non-const)
    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    for_each_in_tuple(std::tuple<Tp...>&, auto) {}

    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<(I < sizeof...(Tp)), void>::type
    for_each_in_tuple(std::tuple<Tp...>& t, auto f) {
        f(std::get<I>(t));
        for_each_in_tuple<I + 1, Tp...>(t, f);
    }

};