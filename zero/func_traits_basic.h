#pragma once
#ifndef FUNC_TRAITS_BASIC_H
#define FUNC_TRAITS_BASIC_H 1

// Yup, "basic"

#include <tuple>
#include <utility>

template<
    typename R, typename B,
    unsigned char _member_func_value_type,
    bool _is_volatile_member_func, bool _is_const_member_func,
    bool _is_member_func,
    bool _is_variadic, bool _is_noexcept,
    typename ... Args
>
struct FunctionTraitsBase {
    using class_type = B;
    
    using ret_type = R;
    using arg_types = std::tuple<Args...>;
    static inline constexpr auto arg_count = sizeof...(Args);

    template<size_t N>
    using nth_arg_type = std::tuple_element_t<N, arg_types>;
    
    static inline constexpr bool is_noexcept = _is_noexcept;
    static inline constexpr bool is_variadic = _is_variadic;
    static inline constexpr bool is_member_func = _is_member_func;
    static inline constexpr bool is_const_member_func = _is_const_member_func;
    static inline constexpr bool is_volatile_member_func = _is_volatile_member_func;
    static inline constexpr bool is_lvalue_ref_member_func = _member_func_value_type == 1;
    static inline constexpr bool is_rvalue_ref_member_func = _member_func_value_type == 2;
};

template<typename R,typename...Args> constexpr FunctionTraitsBase<R,void,0,false,false,false,false,false,Args...> get_func_types_of3(R(*)(Args...) noexcept(false));
template<typename R,typename...Args> constexpr FunctionTraitsBase<R,void,0,false,false,false,false,true,Args...> get_func_types_of3(R(*)(Args...) noexcept(true));
template<typename R,typename...Args> constexpr FunctionTraitsBase<R,void,0,false,false,false,true,false,Args...> get_func_types_of3(R(*)(Args...,...) noexcept(false));
template<typename R,typename...Args> constexpr FunctionTraitsBase<R,void,0,false,false,false,true,true,Args...> get_func_types_of3(R(*)(Args...,...) noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,false,false,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,false,false,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,false,false,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,false,false,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,false,true,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) const noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,false,true,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) const noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,false,true,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) const noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,false,true,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) const noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,true,false,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) volatile noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,true,false,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) volatile noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,true,false,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) volatile noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,true,false,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) volatile noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,true,true,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) const volatile noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,true,true,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) const volatile noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,true,true,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) const volatile noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,0,true,true,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) const volatile noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,false,false,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) & noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,false,false,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) & noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,false,false,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) & noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,false,false,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) & noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,false,true,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) const & noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,false,true,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) const & noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,false,true,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) const & noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,false,true,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) const & noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,true,false,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) volatile & noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,true,false,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) volatile & noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,true,false,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) volatile & noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,true,false,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) volatile & noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,true,true,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) const volatile & noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,true,true,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) const volatile & noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,true,true,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) const volatile & noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,1,true,true,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) const volatile & noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,false,false,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,false,false,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,false,false,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,false,false,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,false,true,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) const noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,false,true,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) const noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,false,true,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) const noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,false,true,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) const noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,true,false,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) volatile noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,true,false,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) volatile noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,true,false,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) volatile noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,true,false,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) volatile noexcept(true));

template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,true,true,true,false,false,Args...> get_func_types_of3(R(B::*)(Args...) const volatile noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,true,true,true,false,true,Args...> get_func_types_of3(R(B::*)(Args...) const volatile noexcept(true));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,true,true,true,true,false,Args...> get_func_types_of3(R(B::*)(Args...,...) const volatile noexcept(false));
template<typename R,typename B,typename...Args> constexpr FunctionTraitsBase<R,B,2,true,true,true,true,true,Args...> get_func_types_of3(R(B::*)(Args...,...) const volatile noexcept(true));


template<auto func>
struct FunctionTraits : decltype(get_func_types_of3(func)) {};

template<typename F>
struct FunctionTraitsType : decltype(get_func_types_of3(&F::operator())) {};

#endif