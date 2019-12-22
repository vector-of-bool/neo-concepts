#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace neo {

// clang-format off

/*
######## ##    ## ########  ########         ######## ########     ###    #### ########  ######
   ##     ##  ##  ##     ## ##                  ##    ##     ##   ## ##    ##     ##    ##    ##
   ##      ####   ##     ## ##                  ##    ##     ##  ##   ##   ##     ##    ##
   ##       ##    ########  ######              ##    ########  ##     ##  ##     ##     ######
   ##       ##    ##        ##                  ##    ##   ##   #########  ##     ##          ##
   ##       ##    ##        ##                  ##    ##    ##  ##     ##  ##     ##    ##    ##
   ##       ##    ##        ######## #######    ##    ##     ## ##     ## ####    ##     ######
*/

namespace detail {

template <typename A, typename B>
concept same_impl = std::is_same_v<A, B>;

}  // namespace detail

template <typename T, typename U>
concept same_as = neo::detail::same_impl<T, U> && neo::detail::same_impl<U, T>;

template <typename From, typename To>
concept convertible_to =
    std::is_convertible_v<From, To>
    && requires(From (&func)()) {
        static_cast<To>(func());
    };

template <typename Derived, typename Base>
concept derived_from =
    std::is_base_of_v<Base, Derived>
    && std::is_convertible_v<const volatile Derived*, const volatile Base*>;

template <typename T>
concept integral = std::is_integral_v<T>;

template <typename T>
concept signed_integral = integral<T> && std::is_signed_v<T>;

template <typename T>
concept unsigned_integral = integral<T> && !signed_integral<T>;

template <typename T>
concept destructible = std::is_nothrow_destructible_v<T>;

template <typename T, typename... Args>
concept constructible_from =
    destructible<T>
    && std::is_constructible_v<T, Args...>;

template <typename T>
concept default_constructible = constructible_from<T>;

template <typename T>
concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

template <typename T>
concept copy_constructible =
    move_constructible<T>
    && constructible_from<T, T&> && convertible_to<T&, T>
    && constructible_from<T, const T&> && convertible_to<const T&, T>
    && constructible_from<T, const T> && convertible_to<const T, T>;

template <typename Left, typename Right>
concept assignable_from =
    std::is_lvalue_reference_v<Left> &&
    requires(Left lhs, Right&& rhs) {
        lhs = std::forward<Right>(rhs);
        requires same_as<decltype(lhs = std::forward<Right>(rhs)), Left>;
    };

template <typename T>
concept movable =
    std::is_object_v<T> &&
    move_constructible<T> &&
    assignable_from<T&, T>;

template <typename T>
concept copyable =
    copy_constructible<T> &&
    movable<T> &&
    assignable_from<T&, const T&>;

template <typename T>
concept trivially_copyable = copyable<T> && std::is_trivially_copyable_v<T>;

template <typename T>
concept trivial_type = trivially_copyable<T> && std::is_trivial_v<T>;

/*
 #######  ########  ######## ########     ###    ########  #######  ########   ######
##     ## ##     ## ##       ##     ##   ## ##      ##    ##     ## ##     ## ##    ##
##     ## ##     ## ##       ##     ##  ##   ##     ##    ##     ## ##     ## ##
##     ## ########  ######   ########  ##     ##    ##    ##     ## ########   ######
##     ## ##        ##       ##   ##   #########    ##    ##     ## ##   ##         ##
##     ## ##        ##       ##    ##  ##     ##    ##    ##     ## ##    ##  ##    ##
 #######  ##        ######## ##     ## ##     ##    ##     #######  ##     ##  ######
*/

template <typename B>
concept simple_boolean = requires(const B b) {
    b ? 0 : 0;
};

namespace detail {

template <typename T, typename U>
concept weakly_equality_comparable_with = requires(const std::remove_reference_t<T> t,
                                                   const std::remove_reference_t<U> u) {
    { t == u } -> neo::simple_boolean;
    { t != u } -> neo::simple_boolean;
    { u == t } -> neo::simple_boolean;
    { u != t } -> neo::simple_boolean;
};

} // namespace detail

template <typename T>
concept equality_comparable = neo::detail::weakly_equality_comparable_with<T, T>;

// XXX: This is not a full impl of equality_comparable_with: We do not have the common_reference work
template <typename T, typename U>
concept equality_comparable_with =
    equality_comparable<T> &&
    equality_comparable<U> &&
    neo::detail::weakly_equality_comparable_with<T, U>;

template <typename T>
concept totally_ordered =
    equality_comparable<T> &&
    requires(const T a, const T b) {
        { a < b } -> simple_boolean;
        { a > b } -> simple_boolean;
        { a <= b } -> simple_boolean;
        { a >= b } -> simple_boolean;
    };

template <typename T, typename U>
concept totally_ordered_with =
    totally_ordered<T> &&
    totally_ordered<U> &&
    equality_comparable_with<T, U> &&
    requires(const std::remove_reference_t<T>& t,
             const std::remove_reference_t<U>& u) {
        { t < u } -> simple_boolean;
        { t > u } -> simple_boolean;
        { t <= u } -> simple_boolean;
        { t >= u } -> simple_boolean;
        { u < t } -> simple_boolean;
        { u > t } -> simple_boolean;
        { u <= t } -> simple_boolean;
        { u >= t } -> simple_boolean;
    };

template <typename T>
concept semiregular = copyable<T> && default_constructible<T>;

template <typename T>
concept regular = semiregular<T> && equality_comparable<T>;

template <typename Func, typename... Args>
concept invocable =
    requires(Func&& fn, Args&&... args) {
        std::invoke(std::forward<Func>(fn), std::forward<Args>(args)...);
    };

template <typename Func, typename... Args>
concept regular_invocable = invocable<Func, Args...>;

template <typename Func, typename... Args>
concept predicate =
    regular_invocable<Func, Args...> &&
    simple_boolean<std::invoke_result_t<Func, Args...>>;

template <typename Rel, typename T, typename U>
concept relation =
    predicate<Rel, T, T> &&
    predicate<Rel, U, U> &&
    predicate<Rel, T, U> &&
    predicate<Rel, U, T>;

// clang-format on

}  // namespace neo
