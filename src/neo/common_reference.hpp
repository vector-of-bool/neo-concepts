#pragma once

#include <type_traits>

#warning "This common_reference implementation is not ready to be used."

namespace neo {

namespace detail {

template <typename T, typename U>
struct cv_union_ref {
    using base_T = std::remove_reference<T>;
    using base_U = std::remove_reference<U>;

    constexpr static bool want_const
        = std::disjunction_v<std::is_const<base_T>, std::is_const<base_U>>;
    constexpr static bool want_volatile
        = std::disjunction_v<std::is_volatile<base_T>, std::is_volatile<base_U>>;

    using const_T = std::conditional_t<want_const, const T, T>;
    using cv_T    = std::conditional_t<want_volatile, volatile const_T, const_T>;

    using const_U = std::conditional_t<want_const, const U, U>;
    using cv_U    = std::conditional_t<want_volatile, volatile const_U, const_U>;

    using t_type = cv_T&;
    using u_type = cv_U&;
};

template <typename cv_T_ref, typename cv_U_ref, typename = void>
struct simple_common_reference_type_base_1 {};

template <typename T, typename U>
struct simple_common_reference_type_base_1<T,
                                           U,
                                           std::enable_if_t<std::is_reference_v<decltype(
                                               false ? std::declval<T>() : std::declval<U>())>>> {
    using type = decltype(false ? std::declval<T>() : std::declval<U>());
};

template <typename T, typename U, typename = void>
struct simple_common_reference_type {};

template <typename T, typename U>
struct simple_common_reference_type<
    T,
    U,
    std::void_t<std::enable_if_t<std::is_lvalue_reference_v<T> && std::is_lvalue_reference_v<U>>,
                typename cv_union_ref<T, U>::t_type,
                typename cv_union_ref<T, U>::u_type>>
    : simple_common_reference_type_base_1<typename cv_union_ref<T, U>::t_type,
                                          typename cv_union_ref<T, U>::u_type> {};

template <typename T, typename U, typename CommonLV, typename = void>
struct simple_common_reference_type_base_2 {};

template <typename T, typename U, typename C>
struct simple_common_reference_type_base_2<
    T,
    U,
    C,
    std::enable_if_t<std::is_convertible_v<T, C> && std::is_convertible_v<U, C>>> {
    using type = C;
};

template <typename T, typename U>
struct simple_common_reference_type<
    T,
    U,
    std::void_t<std::enable_if_t<std::is_rvalue_reference_v<T> && std::is_rvalue_reference_v<U>>,
                typename simple_common_reference_type<T&, U&>::type>>
    : simple_common_reference_type_base_2<
          T,
          U,
          std::add_rvalue_reference_t<
              std::remove_reference_t<typename simple_common_reference_type<T&, U&>::type>>> {};

}  // namespace detail

template <typename T, typename U, template <class> class TQual, template <class> class UQual>
struct basic_common_reference {};

template <typename... Ts>
struct common_reference;

template <typename T>
struct common_reference<T> {
    using type = T;
};

template <typename T, typename U>
struct common_reference<T, U> {};

}  // namespace neo