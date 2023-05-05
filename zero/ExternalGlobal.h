#pragma once

#pragma clang diagnostic ignored "-Wc++17-extensions"

#include <stdint.h>
#include <type_traits>

namespace ExternalGlobalVars {

	template <typename T, uintptr_t addr>
	struct ExternalGlobalBase {
		T make_sizeof_work;

		static inline constexpr uintptr_t static_addr = addr;

		inline T* operator&(void) const {
			return (T*)this->static_addr;
		}
	};

	template <typename T, uintptr_t addr>
	struct ExternalGlobalCastToBase :
		ExternalGlobalBase<T, addr>
	{
		/*inline operator T() const {
			return *(T* const)this->static_addr;
		}*/
		inline operator T&() const {
			return *(T* const)this->static_addr;
		}
	};

	template <typename T, uintptr_t addr>
	struct ExternalGlobalScalar :
		ExternalGlobalCastToBase<T, addr>
	{
		inline T& operator++(void) {
			return ++(*(T* const)this->static_addr);
		}
		inline T& operator--(void) {
			return --(*(T* const)this->static_addr);
		}
		inline T operator++(int) {
			return (*(T* const)this->static_addr)++;
		}
		inline T operator--(int) {
			return (*(T* const)this->static_addr)--;
		}
		template <typename T2>
		inline T& operator+=(const T2& value) {
			return *(T* const)this->static_addr += value;
		}
		template <typename T2>
		inline T& operator-=(const T2& value) {
			return *(T* const)this->static_addr -= value;
		}
		template <typename T2>
		inline T operator+(const T2& value) const {
			return *(T* const)this->static_addr + value;
		}
		template <typename T2>
		inline T operator-(const T2& value) const {
			return *(T* const)this->static_addr - value;
		}
	};

	template <typename T, uintptr_t addr>
	struct ExternalGlobalArithmetic :
		ExternalGlobalScalar<T, addr>
	{
		template <typename T2>
		inline T& operator*=(const T2& value) {
			return *(T* const)this->static_addr *= value;
		}
		template <typename T2>
		inline T& operator/=(const T2& value) {
			return *(T* const)this->static_addr /= value;
		}
	};

	template <typename T, uintptr_t addr, typename reeeeeeee = void>
	struct ExternalGlobal;

	// ====================
	// Integer Specialization
	// ====================
	template <typename T, uintptr_t addr>
	struct ExternalGlobal<T, addr, typename std::enable_if_t<std::is_integral_v<T>>> :
		ExternalGlobalArithmetic<T, addr>
	{
		template <typename T2>
		inline T& operator=(const T2& value) {
			return *(T* const)this->static_addr = value;
		};
		template <typename T2>
		inline T& operator%=(const T2& value) {
			return *(T* const)this->static_addr %= value;
		}
		template <typename T2>
		inline T& operator&=(const T2& value) {
			return *(T* const)this->static_addr &= value;
		}
		template <typename T2>
		inline T& operator|=(const T2& value) {
			return *(T* const)this->static_addr |= value;
		}
		template <typename T2>
		inline T& operator^=(const T2& value) {
			return *(T* const)this->static_addr ^= value;
		}
		template <typename T2>
		inline T& operator<<=(const T2& value) {
			return *(T* const)this->static_addr <<= value;
		}
		template <typename T2>
		inline T& operator>>=(const T2& value) {
			return *(T* const)this->static_addr >>= value;
		}
	};

	// ====================
	// Float Specialization
	// ====================
	template <typename T, uintptr_t addr>
	struct ExternalGlobal<T, addr, typename std::enable_if_t<std::is_floating_point_v<T>>> :
		ExternalGlobalArithmetic<T, addr>
	{
		template <typename T2>
		inline T& operator=(const T2& value) {
			return *(T* const)this->static_addr = value;
		};
	};

	// ====================
	// Enum Specialization
	// ====================
	template <typename T, uintptr_t addr>
	struct ExternalGlobal<T, addr, typename std::enable_if_t<std::is_enum_v<T>>> :
		ExternalGlobalCastToBase<T, addr>
	{
		inline T& operator=(const T& value) {
			return *(T* const)this->static_addr = value;
		};
	};

	// ====================
	// Struct/Union Specialization
	// ====================
	template <typename T, uintptr_t addr>
	struct ExternalGlobal<T, addr, typename std::enable_if_t<std::is_class_v<T> || std::is_union_v<T>>> :
		ExternalGlobalCastToBase<T, addr>
	{
		template <typename T2>
		inline T& operator=(const T2& value) {
			return *(T* const)this->static_addr = value;
		};

		inline T* const operator->(void) const {
			return (T* const)this->static_addr;
		}
	};

	// ====================
	// Array Specialization
	// ====================
	template <typename T, uintptr_t addr>
	struct ExternalGlobal<T, addr, typename std::enable_if_t<std::is_array_v<T>>> :
		ExternalGlobalBase<T, addr>
	{
		using DT = std::decay_t<T>;
		inline constexpr operator DT() const {
			return (DT)this->static_addr;
		}
	};

	// ====================
	// Function Specialization
	// ====================
	template <typename T, uintptr_t addr>
	struct ExternalGlobal<T, addr, typename std::enable_if_t<std::is_function_v<T>>> :
		ExternalGlobalCastToBase<T, addr>
	{
		using TR = std::add_rvalue_reference_t<std::remove_reference_t<T>>;

		template<class... Args>
		inline std::invoke_result_t<TR, Args&&...> operator()(Args&&... args) const {
			return ((TR)*(T*const)this->static_addr)(args...);
		}
	};

	// ====================
	// Void Pointer Specialization
	// ====================
	template <typename T, uintptr_t addr>
	struct ExternalGlobal<T, addr, typename std::enable_if_t<std::is_pointer_v<T> && std::is_void_v<std::remove_pointer_t<T>>>> :
		ExternalGlobalCastToBase<T, addr>
	{
		inline T& operator=(const T& value) {
			return *(T* const)this->static_addr = value;
		};
	};

	// ====================
	// Value/Array Pointer Specialization
	// ====================
	template <typename T, uintptr_t addr>
	struct ExternalGlobal<T, addr, typename std::enable_if_t<std::is_pointer_v<T> && (std::is_arithmetic_v<std::remove_pointer_t<T>> || std::is_array_v<std::remove_pointer_t<T>> || std::is_enum_v<std::remove_pointer_t<T>>)>> :
		ExternalGlobalScalar<T, addr>
	{
		inline T& operator=(const T& value) {
			return *(T* const)this->static_addr = value;
		};
		inline auto operator-(const std::remove_cv_t<T>& value) const {
			return (T)this->static_addr - value;
		}
		template <uintptr_t second_addr, typename T2>
		inline constexpr ptrdiff_t operator-(const ExternalGlobal<T, second_addr, T2>& value) const {
			return this->static_addr - value.static_addr;
		}
	};

	// ====================
	// Struct/Union Pointer Specialization
	// ====================
	template <typename T, uintptr_t addr>
	struct ExternalGlobal<T, addr, typename std::enable_if_t<std::is_pointer_v<T> && (std::is_class_v<std::remove_pointer_t<T>> || std::is_union_v<std::remove_pointer_t<T>>)>> :
		ExternalGlobalScalar<T, addr>
	{
		using DT = std::remove_pointer_t<T>;

		inline T& operator=(const T& value) {
			return *(T* const)this->static_addr = value;
		};
		inline auto operator-(const std::remove_cv_t<T>& value) const {
			return (T)this->static_addr - value;
		}
		template <uintptr_t second_addr, typename T2>
		inline constexpr ptrdiff_t operator-(const ExternalGlobal<T, second_addr, T2>& value) const {
			return this->static_addr - value.static_addr;
		}
		inline T operator->(void) const {
			return *(T* const)this->static_addr;
		}
		inline DT& operator*(void) const {
			return **(T* const)this->static_addr;
		}
	};

	// ====================
	// Function Pointer Specialization
	// ====================
	template <typename T, uintptr_t addr>
	struct ExternalGlobal<T, addr, typename std::enable_if_t<std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>>> :
		ExternalGlobalCastToBase<T, addr>
	{
		using TR = std::add_rvalue_reference_t<std::remove_reference_t<std::remove_pointer_t<T>>>;

		inline T& operator=(const T& value) {
			return *(T* const)this->static_addr = value;
		};

		template<class... Args>
		inline std::invoke_result_t<TR, Args&&...> operator()(Args&&... args) const {
			return ((TR)**(T* const)this->static_addr)(args...);
		}
	};
}