
#include <cstddef>
#include <limits>
#include <type_traits>
#include <utility>
#include <memory>

#ifdef _MSVC_LANG
#define CPP_VER (__cplusplus > _MSVC_LANG ? __cplusplus : _MSVC_LANG)
#else
#define CPP_VER __cplusplus
#endif

#if CPP_VER >= 199711L
#define CPP98 1
#endif
#if CPP_VER >= 201103L
#define CPP11 1
#endif
#if CPP_VER >= 201402L
#define CPP14 1
#endif
#if CPP_VER >= 201703L
#define CPP17 1
#endif
#if CPP_VER >= 202002L
#define CPP20 1
#endif
#define CPP23 0

#define CPP_BEFORE(ver) (!(ver))
#define CPP_AFTER(ver) (!!(ver))

#if CPP_BEFORE(CPP11)
#define ZCONSTEXPR
#define CPP11_CONSTEXPR
#define ZNOTHROW throw()
#define ZNOEXCEPT
#define ZNOEXCEPT_COND(...)
#define ZSTATIC_ASSERT(cond) typedef char MACRO_CAT(static_assertion_, __COUNTER__)[(cond)?1:-1]
#else
#define ZCONSTEXPR constexpr
#define CPP11_CONSTEXPR ZCONSTEXPR
#define ZNOTHROW noexcept
#define ZNOEXCEPT noexcept
#define ZNOEXCEPT_COND(...) noexcept(__VA_ARGS__)
#define ZSTATIC_ASSERT(cond) static_assert(cond)
#endif
#if CPP_BEFORE(CPP17)
#define CPP17_CONSTEXPR
#define ZNODISCARD
#else
#define CPP17_CONSTEXPR ZCONSTEXPR
#define ZNODISCARD [[nodiscard]]
#endif
#if CPP_BEFORE(CPP20)
#define ZCONSTEVAL constexpr
#define CPP20_CONSTEVAL
#define CPP20_CONSTEXPR
#else
#define ZCONSTEVAL consteval
#define CPP20_CONSTEVAL ZCONSTEVAL
#define CPP20_CONSTEXPR ZCONSTEXPR
#endif

template <class T>
struct zallocator {

	ZSTATIC_ASSERT(
		!std::is_const_v<T> &&
#if CPP_BEFORE(CPP11)
		!std::is_reference_v<T>
#else
		!std::is_volatile_v<T>
#endif
	);

	typedef T value_type;
#if CPP_BEFORE(CPP20)
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	template <class U>
	struct rebind {
		typedef zallocator<U> other;
	};
#endif

	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
#if CPP_AFTER(CPP11)
	typedef std::true_type propagate_on_container_move_assignment;
	typedef std::true_type is_always_equal;
#endif
#if CPP_BEFORE(CPP17)
#define ZALLOC_PTR pointer
#define ZALLOC_CPTR const_pointer
#define ZALLOC_REF reference
#define ZALLOC_CREF const_reference
#define ZALLOC_SIZE size_type
#define ZALLOC_HINT_DEFAULT = 0
#else
#define ZALLOC_PTR T*
#define ZALLOC_CPTR const T*
#define ZALLOC_REF T&
#define ZALLOC_CREF const T&
#define ZALLOC_SIZE std::size_t
#define ZALLOC_HINT_DEFAULT
#endif

	CPP20_CONSTEXPR zallocator() ZNOTHROW {};
	CPP20_CONSTEXPR zallocator(const zallocator&) ZNOTHROW = default;
	template <typename U>
	CPP20_CONSTEXPR zallocator(const zallocator<U>&) ZNOTHROW = default;
	CPP20_CONSTEXPR ~zallocator() = default;

	template<class T1, class T2>
	CPP20_CONSTEXPR inline bool operator==(const zallocator<T1>& lhs, const zallocator<T2>& rhs) ZNOTHROW {
		return true;
	}
#if CPP_BEFORE(CPP20)
	template<class T1, class T2>
	inline bool operator!=(const zallocator<T1>& lhs, const zallocator<T2>& rhs) ZNOTHROW {
		return false;
	}
#endif

#if CPP_AFTER(CPP17)
	ZALLOC_NODISCARD CPP20_CONSTEXPR T* allocate(std::size_t n) {

	}
#endif
	CPP20_CONSTEXPR void deallocate(T* p, std::size_t n) {

	}

#if CPP_BEFORE(CPP20)
	inline pointer address(reference x) const ZNOEXCEPT {
		return std::addressof(x);
	}
	inline const_pointer address(const_reference x) const ZNOEXCEPT {
		return std::addressof(x);
	}
	inline ZALLOC_PTR allocate(ZALLOC_SIZE n, const void* hint ZALLOC_HINT_DEFAULT) {
		return allocate(n);
	}
	inline size_type max_size() const ZNOTHROW {
		return std::numeric_limits<size_type>::max() / sizeof(value_type);
	}
#if CPP_BEFORE(CPP11)
	inline void construct(pointer p, const_reference val) {
		new((void*)p) T(val);
	}
	inline void destroy(pointer p) {
		((T*)p)->~T();
	}
#else
	template<class U, class... Args>
	inline void construct(U* p, Args&&... args) {
		::new((void*)p) U(std::forward<Args>(args)...);
	}
	template<class U>
	inline void destroy(U* p) {
		p->~U();
	}
#endif
#endif
};


template <class CharT>
struct zstring {

};