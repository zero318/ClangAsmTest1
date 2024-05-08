#pragma once
#ifndef ZVARIANT
#define ZVARIANT

#include <utility>
#include <variant>
#include <type_traits>
#include <algorithm>
#include <stdlib.h>
#include <stdint.h>

template <template<typename> typename Trait, typename... Types>
struct count_trait_in_pack {
private:
    template <typename T, typename... TArgs>
    static constexpr size_t count_trait() {
        size_t ret = Trait<T>::value;
        if constexpr (sizeof...(TArgs)) ret += count_trait<TArgs...>();
        return ret;
    }
public:
    static constexpr size_t value = count_trait<Types...>();
};

template <template<typename> typename Trait, typename... Types>
inline constexpr size_t count_trait_in_pack_v = count_trait_in_pack<Trait, Types...>::value;

template <typename Count, typename... Types>
struct count_type_in_pack {
private:
    template <typename T, typename... TArgs>
    static constexpr size_t count_type() {
        size_t ret = std::is_same_v<T, Count>;
        if constexpr (sizeof...(TArgs)) ret += count_type<TArgs...>();
        return ret;
    }
public:
    static constexpr size_t value = count_type<Types...>();
};

template <typename Count, typename... Types>
inline constexpr size_t count_type_in_pack_v = count_type_in_pack<Count, Types...>::value;

template <template<typename> typename Trait, typename... Types>
struct find_trait_in_pack {
private:
    template <typename>
    struct impl;
    template <size_t... idx>
    struct impl<std::integer_sequence<size_t, idx...>> : std::integral_constant<size_t, std::min({ (Trait<Types>::value ? idx : SIZE_MAX)... })>{};
public:
    static constexpr size_t value = impl<std::index_sequence_for<Types...>>::value;
};

template <template<typename> typename Trait, typename... Types>
inline constexpr size_t find_trait_in_pack_v = find_trait_in_pack<Trait, Types...>::value;

template <typename Find, typename... Types>
struct find_type_in_pack {
private:
    template <typename>
    struct impl;
    template <size_t... idx>
    struct impl<std::integer_sequence<size_t, idx...>> : std::integral_constant<size_t, std::min({ (std::is_same_v<Find, Types> ? idx : SIZE_MAX)... })>{};
public:
    static constexpr size_t value = impl<std::index_sequence_for<Types...>>::value;
};

template <typename Find, typename... Types>
inline constexpr size_t find_type_in_pack_v = find_type_in_pack<Find, Types...>::value;

template <template<typename> typename Trait, typename... Types>
struct trait_in_pack {
    static constexpr bool value = find_trait_in_pack_v<Trait, Types...> != SIZE_MAX;
};

template <template<typename> typename Trait, typename... Types>
inline constexpr bool trait_in_pack_v = trait_in_pack<Trait, Types...>::value;

template <typename Find, typename... Types>
struct type_in_pack {
    static constexpr bool value = find_type_in_pack_v<Find, Types...> != SIZE_MAX;
};
template <typename Find, typename... Types>
inline constexpr bool type_in_pack_v = type_in_pack<Find, Types...>::value;

template <size_t index, typename T, typename... Types>
struct indexed_pack_type : indexed_pack_type<index - 1, Types...> {
};

template <typename T, typename... Types>
struct indexed_pack_type<0, T, Types...> {
    using type = T;
};

template <size_t index, typename... Types>
using indexed_pack_type_t = indexed_pack_type<index, Types...>::type;

template <typename T, typename... VTypes> requires(type_in_pack_v<T, VTypes...>)
static constexpr T* get_variant_ptr(std::variant<VTypes...>& var) {
    if (T* ptr = std::get_if<T>(&var)) {
        return ptr;
    } else {
        return nullptr;
    }
}

template <typename T, typename... VTypes> requires(type_in_pack_v<T, VTypes...>)
static constexpr T& get_variant_ref(std::variant<VTypes...>& var, T& default_value) {
    if (T* ptr = std::get_if<T>(&var)) {
        return *ptr;
    } else {
        return default_value;
    }
}

template <typename T, typename... VTypes> requires(type_in_pack_v<T, VTypes...>)
static constexpr T get_variant(const std::variant<VTypes...>& var, const T& default_value) {
    if (const T* ptr = std::get_if<T>(&var)) {
        return *ptr;
    } else {
        return default_value;
    }
}

template <typename T, typename... VTypes> requires(type_in_pack_v<T, VTypes...>)
static constexpr T get_variant(const std::variant<VTypes...>& var) {
    if (const T* ptr = std::get_if<T>(&var)) {
        return *ptr;
    } else {
        return {};
    }
}

template <typename... VTypes>
struct ZVariant : std::variant<VTypes...> {

    using std::variant<VTypes...>::variant;
    using std::variant<VTypes...>::operator=;

    template <size_t index> requires(index < sizeof...(VTypes))
    using type_at_index = indexed_pack_type_t<index, VTypes...>;
    template <typename T> requires(type_in_pack_v<T, VTypes...>)
    static constexpr size_t index_of_type = find_type_in_pack_v<T, VTypes...>;

    template <typename T>
    constexpr T* get_ptr() {
        return get_variant_ptr<T>(*this);
    }

    template <typename T>
    constexpr T& get_ref(T& default_value) {
        return get_variant_ref<T>(*this, default_value);
    }

    template <typename T>
    constexpr T get(T& default_value) const {
        return get_variant<T>(*this, default_value);
    }

    template <typename T>
    constexpr T get() const {
        return get_variant<T, VTypes...>(*this);
    }

#if __cpp_lib_variant < 202306L
    template <typename L>
    constexpr decltype(auto) visit(const L& func) {
        return std::visit(func, *this);
    }

    template <typename R, typename L>
    constexpr R visit(const L& func) {
        return std::visit(func, *this);
    }
#endif

    template <typename T> requires(type_in_pack_v<T, VTypes...>)
    constexpr bool holds_alternative() const {
        return std::holds_alternative<T>(*this);
    }
    
    template <typename T> requires(type_in_pack_v<T, VTypes...>)
    explicit constexpr operator T() const {
        return this->get<T>();
    }

    template <typename T> requires(std::is_integral_v<T> && !type_in_pack_v<T, VTypes...> && count_trait_in_pack_v<std::is_integral, VTypes...> == 1)
    constexpr operator T() const {
        return this->get<indexed_pack_type_t<find_trait_in_pack_v<std::is_integral, VTypes...>, VTypes...>>();
    }

    template <typename T> requires(std::is_floating_point_v<T> && !type_in_pack_v<T, VTypes...> && count_trait_in_pack_v<std::is_floating_point, VTypes...> == 1)
    constexpr operator T() const {
        return this->get<indexed_pack_type_t<find_trait_in_pack_v<std::is_floating_point, VTypes...>, VTypes...>>();
    }

    template <typename T> requires(std::is_pointer_v<T> && !type_in_pack_v<T, VTypes...> && count_trait_in_pack_v<std::is_pointer, VTypes...> == 1)
    constexpr operator T() const {
        return this->get<indexed_pack_type_t<find_trait_in_pack_v<std::is_pointer, VTypes...>, VTypes...>>();
    }
};

#endif