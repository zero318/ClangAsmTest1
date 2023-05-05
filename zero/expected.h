
#include <exception>
#include <type_traits>

#define __or_v disjunction_v
#define __and_v conjunction_v
#define __not_ negation
#define __is_constant_evaluated is_constant_evaluated
#define __invoke invoke

namespace std {

template<typename _Tp, typename _Er>
    class expected;

  template<typename _Er>
    class unexpected;

  template<typename _Er>
    class bad_expected_access;

  template<>
    class bad_expected_access<void> : public exception
    {
    protected:
      bad_expected_access() noexcept { }
      bad_expected_access(const bad_expected_access&) = default;
      bad_expected_access(bad_expected_access&&) = default;
      bad_expected_access& operator=(const bad_expected_access&) = default;
      bad_expected_access& operator=(bad_expected_access&&) = default;
      ~bad_expected_access() = default;

    public:

      [[nodiscard]]
      const char*
      what() const noexcept override
      { return "bad access to std::expected without expected value"; }
    };

  template<typename _Er>
    class bad_expected_access : public bad_expected_access<void> {
    public:
      explicit
      bad_expected_access(_Er __e) : _M_unex(std::move(__e)) { }

      [[nodiscard]]
      _Er&
      error() & noexcept
      { return _M_unex; }

      [[nodiscard]]
      const _Er&
      error() const & noexcept
      { return _M_unex; }

      [[nodiscard]]
      _Er&&
      error() && noexcept
      { return std::move(_M_unex); }

      [[nodiscard]]
      const _Er&&
      error() const && noexcept
      { return std::move(_M_unex); }

    private:
      _Er _M_unex;
    };

  struct unexpect_t
  {
    explicit unexpect_t() = default;
  };

  inline constexpr unexpect_t unexpect{};

namespace __expected
{
  template<typename _Tp>
    constexpr bool __is_expected = false;
  template<typename _Tp, typename _Er>
    constexpr bool __is_expected<expected<_Tp, _Er>> = true;

  template<typename _Tp>
    constexpr bool __is_unexpected = false;
  template<typename _Tp>
    constexpr bool __is_unexpected<unexpected<_Tp>> = true;

  template<typename _Fn, typename _Tp>
    using __result = remove_cvref_t<invoke_result_t<_Fn&&, _Tp&&>>;
  template<typename _Fn>
    using __result0 = remove_cvref_t<invoke_result_t<_Fn&&>>;

  template<typename _Er>
    concept __can_be_unexpected
      = is_object_v<_Er> && (!is_array_v<_Er>)
   && (!__expected::__is_unexpected<_Er>)
   && (!is_const_v<_Er>) && (!is_volatile_v<_Er>);

  struct __in_place_inv { };
  struct __unexpect_inv { };
}

  template<typename _Er>
    class unexpected
    {
      static_assert( __expected::__can_be_unexpected<_Er> );

    public:
      constexpr unexpected(const unexpected&) = default;
      constexpr unexpected(unexpected&&) = default;

      template<typename _Err = _Er>
 requires (!is_same_v<remove_cvref_t<_Err>, unexpected>)
   && (!is_same_v<remove_cvref_t<_Err>, in_place_t>)
   && is_constructible_v<_Er, _Err>
 constexpr explicit
 unexpected(_Err&& __e)
 noexcept(is_nothrow_constructible_v<_Er, _Err>)
 : _M_unex(std::forward<_Err>(__e))
 { }

      template<typename... _Args>
 requires is_constructible_v<_Er, _Args...>
 constexpr explicit
 unexpected(in_place_t, _Args&&... __args)
 noexcept(is_nothrow_constructible_v<_Er, _Args...>)
 : _M_unex(std::forward<_Args>(__args)...)
 { }

      template<typename _Up, typename... _Args>
 requires is_constructible_v<_Er, initializer_list<_Up>&, _Args...>
 constexpr explicit
 unexpected(in_place_t, initializer_list<_Up> __il, _Args&&... __args)
 noexcept(is_nothrow_constructible_v<_Er, initializer_list<_Up>&,
         _Args...>)
 : _M_unex(__il, std::forward<_Args>(__args)...)
 { }

      constexpr unexpected& operator=(const unexpected&) = default;
      constexpr unexpected& operator=(unexpected&&) = default;

      [[nodiscard]]
      constexpr const _Er&
      error() const & noexcept { return _M_unex; }

      [[nodiscard]]
      constexpr _Er&
      error() & noexcept { return _M_unex; }

      [[nodiscard]]
      constexpr const _Er&&
      error() const && noexcept { return std::move(_M_unex); }

      [[nodiscard]]
      constexpr _Er&&
      error() && noexcept { return std::move(_M_unex); }

      constexpr void
      swap(unexpected& __other) noexcept(is_nothrow_swappable_v<_Er>)
      requires is_swappable_v<_Er>
      {
 using std::swap;
 swap(_M_unex, __other._M_unex);
      }

      template<typename _Err>
 [[nodiscard]]
 friend constexpr bool
 operator==(const unexpected& __x, const unexpected<_Err>& __y)
 { return __x._M_unex == __y.error(); }

      friend constexpr void
      swap(unexpected& __x, unexpected& __y) noexcept(noexcept(__x.swap(__y)))
      requires is_swappable_v<_Er>
      { __x.swap(__y); }

    private:
      _Er _M_unex;
    };

  template<typename _Er> unexpected(_Er) -> unexpected<_Er>;

namespace __expected
{
  template<typename _Tp>
    struct _Guard
    {
      static_assert( is_nothrow_move_constructible_v<_Tp> );

      constexpr explicit
      _Guard(_Tp& __x)
      : _M_guarded(__builtin_addressof(__x)), _M_tmp(std::move(__x))
      { std::destroy_at(_M_guarded); }

      constexpr
      ~_Guard()
      {
 if (_M_guarded) [[unlikely]]
   std::construct_at(_M_guarded, std::move(_M_tmp));
      }

      _Guard(const _Guard&) = delete;
      _Guard& operator=(const _Guard&) = delete;

      constexpr _Tp&&
      release() noexcept
      {
 _M_guarded = nullptr;
 return std::move(_M_tmp);
      }

    private:
      _Tp* _M_guarded;
      _Tp _M_tmp;
    };

  template<typename _Tp, typename _Up, typename _Vp>
    constexpr void
    __reinit(_Tp* __newval, _Up* __oldval, _Vp&& __arg)
    noexcept(is_nothrow_constructible_v<_Tp, _Vp>)
    {
      if constexpr (is_nothrow_constructible_v<_Tp, _Vp>)
 {
   std::destroy_at(__oldval);
   std::construct_at(__newval, std::forward<_Vp>(__arg));
 }
      else if constexpr (is_nothrow_move_constructible_v<_Tp>)
 {
   _Tp __tmp(std::forward<_Vp>(__arg));
   std::destroy_at(__oldval);
   std::construct_at(__newval, std::move(__tmp));
 }
      else
 {
   _Guard<_Up> __guard(*__oldval);
   std::construct_at(__newval, std::forward<_Vp>(__arg));
   __guard.release();
 }
    }
}

  template<typename _Tp, typename _Er>
    class expected
    {
      static_assert( ! is_reference_v<_Tp> );
      static_assert( ! is_function_v<_Tp> );
      static_assert( ! is_same_v<remove_cv_t<_Tp>, in_place_t> );
      static_assert( ! is_same_v<remove_cv_t<_Tp>, unexpect_t> );
      static_assert( ! __expected::__is_unexpected<remove_cv_t<_Tp>> );
      static_assert( __expected::__can_be_unexpected<_Er> );

      template<typename _Up, typename _Err, typename _Unex = unexpected<_Er>>
 static constexpr bool __cons_from_expected
   = __or_v<is_constructible<_Tp, expected<_Up, _Err>&>,
     is_constructible<_Tp, expected<_Up, _Err>>,
     is_constructible<_Tp, const expected<_Up, _Err>&>,
     is_constructible<_Tp, const expected<_Up, _Err>>,
     is_convertible<expected<_Up, _Err>&, _Tp>,
     is_convertible<expected<_Up, _Err>, _Tp>,
     is_convertible<const expected<_Up, _Err>&, _Tp>,
     is_convertible<const expected<_Up, _Err>, _Tp>,
     is_constructible<_Unex, expected<_Up, _Err>&>,
     is_constructible<_Unex, expected<_Up, _Err>>,
     is_constructible<_Unex, const expected<_Up, _Err>&>,
     is_constructible<_Unex, const expected<_Up, _Err>>
    >;

      template<typename _Up, typename _Err>
 constexpr static bool __explicit_conv
   = __or_v<__not_<is_convertible<_Up, _Tp>>,
     __not_<is_convertible<_Err, _Er>>
    >;

      template<typename _Up>
 static constexpr bool __same_val
   = is_same_v<typename _Up::value_type, _Tp>;

      template<typename _Up>
 static constexpr bool __same_err
   = is_same_v<typename _Up::error_type, _Er>;

    public:
      using value_type = _Tp;
      using error_type = _Er;
      using unexpected_type = unexpected<_Er>;

      template<typename _Up>
 using rebind = expected<_Up, error_type>;

      constexpr
      expected()
      noexcept(is_nothrow_default_constructible_v<_Tp>)
      requires is_default_constructible_v<_Tp>
      : _M_val(), _M_has_value(true)
      { }

      expected(const expected&) = default;

      constexpr
      expected(const expected& __x)
      noexcept(__and_v<is_nothrow_copy_constructible<_Tp>,
         is_nothrow_copy_constructible<_Er>>)
      requires is_copy_constructible_v<_Tp> && is_copy_constructible_v<_Er>
      && (!is_trivially_copy_constructible_v<_Tp>
   || !is_trivially_copy_constructible_v<_Er>)
      : _M_has_value(__x._M_has_value)
      {
 if (_M_has_value)
   std::construct_at(__builtin_addressof(_M_val), __x._M_val);
 else
   std::construct_at(__builtin_addressof(_M_unex), __x._M_unex);
      }

      expected(expected&&) = default;

      constexpr
      expected(expected&& __x)
      noexcept(__and_v<is_nothrow_move_constructible<_Tp>,
         is_nothrow_move_constructible<_Er>>)
      requires is_move_constructible_v<_Tp> && is_move_constructible_v<_Er>
      && (!is_trivially_move_constructible_v<_Tp>
   || !is_trivially_move_constructible_v<_Er>)
      : _M_has_value(__x._M_has_value)
      {
 if (_M_has_value)
   std::construct_at(__builtin_addressof(_M_val),
       std::move(__x)._M_val);
 else
   std::construct_at(__builtin_addressof(_M_unex),
       std::move(__x)._M_unex);
      }

      template<typename _Up, typename _Gr>
 requires is_constructible_v<_Tp, const _Up&>
       && is_constructible_v<_Er, const _Gr&>
       && (!__cons_from_expected<_Up, _Gr>)
 constexpr explicit(__explicit_conv<const _Up&, const _Gr&>)
 expected(const expected<_Up, _Gr>& __x)
 noexcept(__and_v<is_nothrow_constructible<_Tp, const _Up&>,
    is_nothrow_constructible<_Er, const _Gr&>>)
 : _M_has_value(__x._M_has_value)
 {
   if (_M_has_value)
     std::construct_at(__builtin_addressof(_M_val), __x._M_val);
   else
     std::construct_at(__builtin_addressof(_M_unex), __x._M_unex);
 }

      template<typename _Up, typename _Gr>
 requires is_constructible_v<_Tp, _Up>
       && is_constructible_v<_Er, _Gr>
       && (!__cons_from_expected<_Up, _Gr>)
 constexpr explicit(__explicit_conv<_Up, _Gr>)
 expected(expected<_Up, _Gr>&& __x)
 noexcept(__and_v<is_nothrow_constructible<_Tp, _Up>,
    is_nothrow_constructible<_Er, _Gr>>)
 : _M_has_value(__x._M_has_value)
 {
   if (_M_has_value)
     std::construct_at(__builtin_addressof(_M_val),
         std::move(__x)._M_val);
   else
     std::construct_at(__builtin_addressof(_M_unex),
         std::move(__x)._M_unex);
 }

      template<typename _Up = _Tp>
 requires (!is_same_v<remove_cvref_t<_Up>, expected>)
   && (!is_same_v<remove_cvref_t<_Up>, in_place_t>)
   && (!__expected::__is_unexpected<remove_cvref_t<_Up>>)
   && is_constructible_v<_Tp, _Up>
 constexpr explicit(!is_convertible_v<_Up, _Tp>)
 expected(_Up&& __v)
 noexcept(is_nothrow_constructible_v<_Tp, _Up>)
 : _M_val(std::forward<_Up>(__v)), _M_has_value(true)
 { }

      template<typename _Gr = _Er>
 requires is_constructible_v<_Er, const _Gr&>
 constexpr explicit(!is_convertible_v<const _Gr&, _Er>)
 expected(const unexpected<_Gr>& __u)
 noexcept(is_nothrow_constructible_v<_Er, const _Gr&>)
 : _M_unex(__u.error()), _M_has_value(false)
 { }

      template<typename _Gr = _Er>
 requires is_constructible_v<_Er, _Gr>
 constexpr explicit(!is_convertible_v<_Gr, _Er>)
 expected(unexpected<_Gr>&& __u)
 noexcept(is_nothrow_constructible_v<_Er, _Gr>)
 : _M_unex(std::move(__u).error()), _M_has_value(false)
 { }

      template<typename... _Args>
 requires is_constructible_v<_Tp, _Args...>
 constexpr explicit
 expected(in_place_t, _Args&&... __args)
 noexcept(is_nothrow_constructible_v<_Tp, _Args...>)
 : _M_val(std::forward<_Args>(__args)...), _M_has_value(true)
 { }

      template<typename _Up, typename... _Args>
 requires is_constructible_v<_Tp, initializer_list<_Up>&, _Args...>
 constexpr explicit
 expected(in_place_t, initializer_list<_Up> __il, _Args&&... __args)
 noexcept(is_nothrow_constructible_v<_Tp, initializer_list<_Up>&,
         _Args...>)
 : _M_val(__il, std::forward<_Args>(__args)...), _M_has_value(true)
 { }

      template<typename... _Args>
 requires is_constructible_v<_Er, _Args...>
 constexpr explicit
 expected(unexpect_t, _Args&&... __args)
 noexcept(is_nothrow_constructible_v<_Er, _Args...>)
 : _M_unex(std::forward<_Args>(__args)...), _M_has_value(false)
 { }

      template<typename _Up, typename... _Args>
 requires is_constructible_v<_Er, initializer_list<_Up>&, _Args...>
 constexpr explicit
 expected(unexpect_t, initializer_list<_Up> __il, _Args&&... __args)
 noexcept(is_nothrow_constructible_v<_Er, initializer_list<_Up>&,
         _Args...>)
 : _M_unex(__il, std::forward<_Args>(__args)...), _M_has_value(false)
 { }

      constexpr ~expected() = default;

      constexpr ~expected()
      requires (!is_trivially_destructible_v<_Tp>)
     || (!is_trivially_destructible_v<_Er>)
      {
 if (_M_has_value)
   std::destroy_at(__builtin_addressof(_M_val));
 else
   std::destroy_at(__builtin_addressof(_M_unex));
      }

      expected& operator=(const expected&) = delete;

      constexpr expected&
      operator=(const expected& __x)
      noexcept(__and_v<is_nothrow_copy_constructible<_Tp>,
         is_nothrow_copy_constructible<_Er>,
         is_nothrow_copy_assignable<_Tp>,
         is_nothrow_copy_assignable<_Er>>)
      requires is_copy_assignable_v<_Tp> && is_copy_constructible_v<_Tp>
     && is_copy_assignable_v<_Er> && is_copy_constructible_v<_Er>
     && (is_nothrow_move_constructible_v<_Tp>
  || is_nothrow_move_constructible_v<_Er>)
      {
 if (__x._M_has_value)
   this->_M_assign_val(__x._M_val);
 else
   this->_M_assign_unex(__x._M_unex);
 return *this;
      }

      constexpr expected&
      operator=(expected&& __x)
      noexcept(__and_v<is_nothrow_move_constructible<_Tp>,
         is_nothrow_move_constructible<_Er>,
         is_nothrow_move_assignable<_Tp>,
         is_nothrow_move_assignable<_Er>>)
      requires is_move_assignable_v<_Tp> && is_move_constructible_v<_Tp>
     && is_move_assignable_v<_Er> && is_move_constructible_v<_Er>
     && (is_nothrow_move_constructible_v<_Tp>
  || is_nothrow_move_constructible_v<_Er>)
      {
 if (__x._M_has_value)
   _M_assign_val(std::move(__x._M_val));
 else
   _M_assign_unex(std::move(__x._M_unex));
 return *this;
      }

      template<typename _Up = _Tp>
 requires (!is_same_v<expected, remove_cvref_t<_Up>>)
       && (!__expected::__is_unexpected<remove_cvref_t<_Up>>)
       && is_constructible_v<_Tp, _Up> && is_assignable_v<_Tp&, _Up>
       && (is_nothrow_constructible_v<_Tp, _Up>
    || is_nothrow_move_constructible_v<_Tp>
    || is_nothrow_move_constructible_v<_Er>)
 constexpr expected&
 operator=(_Up&& __v)
 {
   _M_assign_val(std::forward<_Up>(__v));
   return *this;
 }

      template<typename _Gr>
 requires is_constructible_v<_Er, const _Gr&>
       && is_assignable_v<_Er&, const _Gr&>
       && (is_nothrow_constructible_v<_Er, const _Gr&>
    || is_nothrow_move_constructible_v<_Tp>
    || is_nothrow_move_constructible_v<_Er>)
 constexpr expected&
 operator=(const unexpected<_Gr>& __e)
 {
   _M_assign_unex(__e.error());
   return *this;
 }

      template<typename _Gr>
 requires is_constructible_v<_Er, _Gr>
       && is_assignable_v<_Er&, _Gr>
       && (is_nothrow_constructible_v<_Er, _Gr>
    || is_nothrow_move_constructible_v<_Tp>
    || is_nothrow_move_constructible_v<_Er>)
 constexpr expected&
 operator=(unexpected<_Gr>&& __e)
 {
   _M_assign_unex(std::move(__e).error());
   return *this;
 }

      template<typename... _Args>
 requires is_nothrow_constructible_v<_Tp, _Args...>
 constexpr _Tp&
 emplace(_Args&&... __args) noexcept
 {
   if (_M_has_value)
     std::destroy_at(__builtin_addressof(_M_val));
   else
     {
       std::destroy_at(__builtin_addressof(_M_unex));
       _M_has_value = true;
     }
   std::construct_at(__builtin_addressof(_M_val),
       std::forward<_Args>(__args)...);
   return _M_val;
 }

      template<typename _Up, typename... _Args>
 requires is_nothrow_constructible_v<_Tp, initializer_list<_Up>&,
         _Args...>
 constexpr _Tp&
 emplace(initializer_list<_Up> __il, _Args&&... __args) noexcept
 {
   if (_M_has_value)
     std::destroy_at(__builtin_addressof(_M_val));
   else
     {
       std::destroy_at(__builtin_addressof(_M_unex));
       _M_has_value = true;
     }
   std::construct_at(__builtin_addressof(_M_val),
       __il, std::forward<_Args>(__args)...);
   return _M_val;
 }

      constexpr void
      swap(expected& __x)
      noexcept(__and_v<is_nothrow_move_constructible<_Tp>,
         is_nothrow_move_constructible<_Er>,
         is_nothrow_swappable<_Tp&>,
         is_nothrow_swappable<_Er&>>)
      requires is_swappable_v<_Tp> && is_swappable_v<_Er>
     && is_move_constructible_v<_Tp>
     && is_move_constructible_v<_Er>
     && (is_nothrow_move_constructible_v<_Tp>
  || is_nothrow_move_constructible_v<_Er>)
      {
 if (_M_has_value)
   {
     if (__x._M_has_value)
       {
  using std::swap;
  swap(_M_val, __x._M_val);
       }
     else
       this->_M_swap_val_unex(__x);
   }
 else
   {
     if (__x._M_has_value)
       __x._M_swap_val_unex(*this);
     else
       {
  using std::swap;
  swap(_M_unex, __x._M_unex);
       }
   }
      }

      [[nodiscard]]
      constexpr const _Tp*
      operator->() const noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(_M_has_value)) __builtin_unreachable(); } while (false);
 return __builtin_addressof(_M_val);
      }

      [[nodiscard]]
      constexpr _Tp*
      operator->() noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(_M_has_value)) __builtin_unreachable(); } while (false);
 return __builtin_addressof(_M_val);
      }

      [[nodiscard]]
      constexpr const _Tp&
      operator*() const & noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(_M_has_value)) __builtin_unreachable(); } while (false);
 return _M_val;
      }

      [[nodiscard]]
      constexpr _Tp&
      operator*() & noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(_M_has_value)) __builtin_unreachable(); } while (false);
 return _M_val;
      }

      [[nodiscard]]
      constexpr const _Tp&&
      operator*() const && noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(_M_has_value)) __builtin_unreachable(); } while (false);
 return std::move(_M_val);
      }

      [[nodiscard]]
      constexpr _Tp&&
      operator*() && noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(_M_has_value)) __builtin_unreachable(); } while (false);
 return std::move(_M_val);
      }

      [[nodiscard]]
      constexpr explicit
      operator bool() const noexcept { return _M_has_value; }

      [[nodiscard]]
      constexpr bool has_value() const noexcept { return _M_has_value; }

      constexpr const _Tp&
      value() const &
      {
 if (_M_has_value) [[likely]]
   return _M_val;
 (throw (bad_expected_access<_Er>(_M_unex)));
      }

      constexpr _Tp&
      value() &
      {
 if (_M_has_value) [[likely]]
   return _M_val;
 (throw (bad_expected_access<_Er>(_M_unex)));
      }

      constexpr const _Tp&&
      value() const &&
      {
 if (_M_has_value) [[likely]]
   return std::move(_M_val);
 (throw (bad_expected_access<_Er>( std::move(_M_unex))))
                          ;
      }

      constexpr _Tp&&
      value() &&
      {
 if (_M_has_value) [[likely]]
   return std::move(_M_val);
 (throw (bad_expected_access<_Er>( std::move(_M_unex))))
                          ;
      }

      constexpr const _Er&
      error() const & noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(!_M_has_value)) __builtin_unreachable(); } while (false);
 return _M_unex;
      }

      constexpr _Er&
      error() & noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(!_M_has_value)) __builtin_unreachable(); } while (false);
 return _M_unex;
      }

      constexpr const _Er&&
      error() const && noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(!_M_has_value)) __builtin_unreachable(); } while (false);
 return std::move(_M_unex);
      }

      constexpr _Er&&
      error() && noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(!_M_has_value)) __builtin_unreachable(); } while (false);
 return std::move(_M_unex);
      }

      template<typename _Up>
 constexpr _Tp
 value_or(_Up&& __v) const &
 noexcept(__and_v<is_nothrow_copy_constructible<_Tp>,
    is_nothrow_convertible<_Up, _Tp>>)
 {
   static_assert( is_copy_constructible_v<_Tp> );
   static_assert( is_convertible_v<_Up, _Tp> );

   if (_M_has_value)
     return _M_val;
   return static_cast<_Tp>(std::forward<_Up>(__v));
 }

      template<typename _Up>
 constexpr _Tp
 value_or(_Up&& __v) &&
 noexcept(__and_v<is_nothrow_move_constructible<_Tp>,
    is_nothrow_convertible<_Up, _Tp>>)
 {
   static_assert( is_move_constructible_v<_Tp> );
   static_assert( is_convertible_v<_Up, _Tp> );

   if (_M_has_value)
     return std::move(_M_val);
   return static_cast<_Tp>(std::forward<_Up>(__v));
 }

      template<typename _Gr = _Er>
 constexpr _Er
 error_or(_Gr&& __e) const&
 {
   static_assert( is_copy_constructible_v<_Er> );
   static_assert( is_convertible_v<_Gr, _Er> );

   if (_M_has_value)
     return std::forward<_Gr>(__e);
   return _M_unex;
 }

      template<typename _Gr = _Er>
 constexpr _Er
 error_or(_Gr&& __e) &&
 {
   static_assert( is_move_constructible_v<_Er> );
   static_assert( is_convertible_v<_Gr, _Er> );

   if (_M_has_value)
     return std::forward<_Gr>(__e);
   return std::move(_M_unex);
 }

      template<typename _Fn> requires is_copy_constructible_v<_Er>
 constexpr auto
 and_then(_Fn&& __f) &
 {
   using _Up = __expected::__result<_Fn, _Tp&>;
   static_assert(__expected::__is_expected<_Up>);
   static_assert(is_same_v<typename _Up::error_type, _Er>);

   if (has_value())
     return std::__invoke(std::forward<_Fn>(__f), value());
   else
     return _Up(unexpect, error());
 }

      template<typename _Fn> requires is_copy_constructible_v<_Er>
 constexpr auto
 and_then(_Fn&& __f) const &
 {
   using _Up = __expected::__result<_Fn, const _Tp&>;
   static_assert(__expected::__is_expected<_Up>);
   static_assert(is_same_v<typename _Up::error_type, _Er>);

   if (has_value())
     return std::__invoke(std::forward<_Fn>(__f), value());
   else
     return _Up(unexpect, error());
 }

      template<typename _Fn> requires is_move_constructible_v<_Er>
 constexpr auto
 and_then(_Fn&& __f) &&
 {
   using _Up = __expected::__result<_Fn, _Tp&&>;
   static_assert(__expected::__is_expected<_Up>);
   static_assert(is_same_v<typename _Up::error_type, _Er>);

   if (has_value())
     return std::__invoke(std::forward<_Fn>(__f), std::move(value()));
   else
     return _Up(unexpect, std::move(error()));
 }

      template<typename _Fn> requires is_move_constructible_v<_Er>
 constexpr auto
 and_then(_Fn&& __f) const &&
 {
   using _Up = __expected::__result<_Fn, const _Tp&&>;
   static_assert(__expected::__is_expected<_Up>);
   static_assert(is_same_v<typename _Up::error_type, _Er>);

   if (has_value())
     return std::__invoke(std::forward<_Fn>(__f), std::move(value()));
   else
     return _Up(unexpect, std::move(error()));
 }

      template<typename _Fn> requires is_copy_constructible_v<_Er>
 constexpr auto
 or_else(_Fn&& __f) &
 {
   using _Gr = __expected::__result<_Fn, _Er&>;
   static_assert(__expected::__is_expected<_Gr>);
   static_assert(is_same_v<typename _Gr::value_type, _Tp>);

   if (has_value())
     return _Gr(in_place, value());
   else
     return std::__invoke(std::forward<_Fn>(__f), error());
 }

      template<typename _Fn> requires is_copy_constructible_v<_Er>
 constexpr auto
 or_else(_Fn&& __f) const &
 {
   using _Gr = __expected::__result<_Fn, const _Er&>;
   static_assert(__expected::__is_expected<_Gr>);
   static_assert(is_same_v<typename _Gr::value_type, _Tp>);

   if (has_value())
     return _Gr(in_place, value());
   else
     return std::__invoke(std::forward<_Fn>(__f), error());
 }

      template<typename _Fn> requires is_move_constructible_v<_Er>
 constexpr auto
 or_else(_Fn&& __f) &&
 {
   using _Gr = __expected::__result<_Fn, _Er&&>;
   static_assert(__expected::__is_expected<_Gr>);
   static_assert(is_same_v<typename _Gr::value_type, _Tp>);

   if (has_value())
     return _Gr(in_place, std::move(value()));
   else
     return std::__invoke(std::forward<_Fn>(__f), std::move(error()));
 }

      template<typename _Fn> requires is_move_constructible_v<_Er>
 constexpr auto
 or_else(_Fn&& __f) const &&
 {
   using _Gr = __expected::__result<_Fn, const _Er&&>;
   static_assert(__expected::__is_expected<_Gr>);
   static_assert(is_same_v<typename _Gr::value_type, _Tp>);

   if (has_value())
     return _Gr(in_place, std::move(value()));
   else
     return std::__invoke(std::forward<_Fn>(__f), std::move(error()));
 }

      template<typename _Fn> requires is_copy_constructible_v<_Er>
 constexpr auto
 transform(_Fn&& __f) &
 {
   using _Up = __expected::__result<_Fn, _Tp&>;
   using _Res = expected<_Up, _Er>;

   if (has_value())
     return _Res(__in_place_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            _M_val);
   });
   else
     return _Res(unexpect, std::move(error()));
 }

      template<typename _Fn> requires is_copy_constructible_v<_Er>
 constexpr auto
 transform(_Fn&& __f) const &
 {
   using _Up = __expected::__result<_Fn, const _Tp&>;
   using _Res = expected<_Up, _Er>;

   if (has_value())
     return _Res(__in_place_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            _M_val);
   });
   else
     return _Res(unexpect, std::move(error()));
 }

      template<typename _Fn> requires is_move_constructible_v<_Er>
 constexpr auto
 transform(_Fn&& __f) &&
 {
   using _Up = __expected::__result<_Fn, _Tp>;
   using _Res = expected<_Up, _Er>;

   if (has_value())
     return _Res(__in_place_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            std::move(_M_val));
   });
   else
     return _Res(unexpect, std::move(error()));
 }

      template<typename _Fn> requires is_move_constructible_v<_Er>
 constexpr auto
 transform(_Fn&& __f) const &&
 {
   using _Up = __expected::__result<_Fn, const _Tp>;
   using _Res = expected<_Up, _Er>;

   if (has_value())
     return _Res(__in_place_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            std::move(_M_val));
   });
   else
     return _Res(unexpect, std::move(error()));
 }

      template<typename _Fn> requires is_copy_constructible_v<_Tp>
 constexpr auto
 transform_error(_Fn&& __f) &
 {
   using _Gr = __expected::__result<_Fn, _Er&>;
   using _Res = expected<_Tp, _Gr>;

   if (has_value())
     return _Res(in_place, value());
   else
     return _Res(__unexpect_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            _M_unex);
   });
 }

      template<typename _Fn> requires is_copy_constructible_v<_Tp>
 constexpr auto
 transform_error(_Fn&& __f) const &
 {
   using _Gr = __expected::__result<_Fn, const _Er&>;
   using _Res = expected<_Tp, _Gr>;

   if (has_value())
     return _Res(in_place, value());
   else
     return _Res(__unexpect_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            _M_unex);
   });
 }

      template<typename _Fn> requires is_move_constructible_v<_Tp>
 constexpr auto
 transform_error(_Fn&& __f) &&
 {
   using _Gr = __expected::__result<_Fn, _Er&&>;
   using _Res = expected<_Tp, _Gr>;

   if (has_value())
     return _Res(in_place, std::move(value()));
   else
     return _Res(__unexpect_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            std::move(_M_unex));
   });
 }

      template<typename _Fn> requires is_move_constructible_v<_Tp>
 constexpr auto
 transform_error(_Fn&& __f) const &&
 {
   using _Gr = __expected::__result<_Fn, const _Er&&>;
   using _Res = expected<_Tp, _Gr>;

   if (has_value())
     return _Res(in_place, std::move(value()));
   else
     return _Res(__unexpect_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            std::move(_M_unex));
   });
 }

      template<typename _Up, typename _Er2>
 requires (!is_void_v<_Up>)
 friend constexpr bool
 operator==(const expected& __x, const expected<_Up, _Er2>& __y)

 {
   if (__x.has_value())
     return __y.has_value() && bool(*__x == *__y);
   else
     return !__y.has_value() && bool(__x.error() == __y.error());
 }

      template<typename _Up>
 friend constexpr bool
 operator==(const expected& __x, const _Up& __v)

 { return __x.has_value() && bool(*__x == __v); }

      template<typename _Er2>
 friend constexpr bool
 operator==(const expected& __x, const unexpected<_Er2>& __e)

 { return !__x.has_value() && bool(__x.error() == __e.error()); }

      friend constexpr void
      swap(expected& __x, expected& __y)
      noexcept(noexcept(__x.swap(__y)))
      requires requires {__x.swap(__y);}
      { __x.swap(__y); }

    private:
      template<typename, typename> friend class expected;

      template<typename _Vp>
 constexpr void
 _M_assign_val(_Vp&& __v)
 {
   if (_M_has_value)
     _M_val = std::forward<_Vp>(__v);
   else
     {
       __expected::__reinit(__builtin_addressof(_M_val),
       __builtin_addressof(_M_unex),
       std::forward<_Vp>(__v));
       _M_has_value = true;
     }
 }

      template<typename _Vp>
 constexpr void
 _M_assign_unex(_Vp&& __v)
 {
   if (_M_has_value)
     {
       __expected::__reinit(__builtin_addressof(_M_unex),
       __builtin_addressof(_M_val),
       std::forward<_Vp>(__v));
       _M_has_value = false;
     }
   else
     _M_unex = std::forward<_Vp>(__v);
 }

      constexpr void
      _M_swap_val_unex(expected& __rhs)
      noexcept(__and_v<is_nothrow_move_constructible<_Er>,
         is_nothrow_move_constructible<_Tp>>)
      {
 if constexpr (is_nothrow_move_constructible_v<_Er>)
   {
     __expected::_Guard<_Er> __guard(__rhs._M_unex);
     std::construct_at(__builtin_addressof(__rhs._M_val),
         std::move(_M_val));
     __rhs._M_has_value = true;
     std::destroy_at(__builtin_addressof(_M_val));
     std::construct_at(__builtin_addressof(_M_unex),
         __guard.release());
     _M_has_value = false;
   }
 else
   {
     __expected::_Guard<_Tp> __guard(_M_val);
     std::construct_at(__builtin_addressof(_M_unex),
         std::move(__rhs._M_unex));
     _M_has_value = false;
     std::destroy_at(__builtin_addressof(__rhs._M_unex));
     std::construct_at(__builtin_addressof(__rhs._M_val),
         __guard.release());
     __rhs._M_has_value = true;
   }
      }

      using __in_place_inv = __expected::__in_place_inv;
      using __unexpect_inv = __expected::__unexpect_inv;

      template<typename _Fn>
 explicit constexpr
 expected(__in_place_inv, _Fn&& __fn)
 : _M_val(std::forward<_Fn>(__fn)()), _M_has_value(true)
 { }

      template<typename _Fn>
 explicit constexpr
 expected(__unexpect_inv, _Fn&& __fn)
 : _M_unex(std::forward<_Fn>(__fn)()), _M_has_value(false)
 { }

      union {
 _Tp _M_val;
 _Er _M_unex;
      };

      bool _M_has_value;
    };

  template<typename _Tp, typename _Er> requires is_void_v<_Tp>
    class expected<_Tp, _Er>
    {
      static_assert( __expected::__can_be_unexpected<_Er> );

      template<typename _Up, typename _Err, typename _Unex = unexpected<_Er>>
 static constexpr bool __cons_from_expected
   = __or_v<is_constructible<_Unex, expected<_Up, _Err>&>,
     is_constructible<_Unex, expected<_Up, _Err>>,
     is_constructible<_Unex, const expected<_Up, _Err>&>,
     is_constructible<_Unex, const expected<_Up, _Err>>
    >;

      template<typename _Up>
 static constexpr bool __same_val
   = is_same_v<typename _Up::value_type, _Tp>;

      template<typename _Up>
 static constexpr bool __same_err
   = is_same_v<typename _Up::error_type, _Er>;

    public:
      using value_type = _Tp;
      using error_type = _Er;
      using unexpected_type = unexpected<_Er>;

      template<typename _Up>
 using rebind = expected<_Up, error_type>;

      constexpr
      expected() noexcept
      : _M_void(), _M_has_value(true)
      { }

      expected(const expected&) = default;

      constexpr
      expected(const expected& __x)
      noexcept(is_nothrow_copy_constructible_v<_Er>)
      requires is_copy_constructible_v<_Er>
     && (!is_trivially_copy_constructible_v<_Er>)
      : _M_void(), _M_has_value(__x._M_has_value)
      {
 if (!_M_has_value)
   std::construct_at(__builtin_addressof(_M_unex), __x._M_unex);
      }

      expected(expected&&) = default;

      constexpr
      expected(expected&& __x)
      noexcept(is_nothrow_move_constructible_v<_Er>)
      requires is_move_constructible_v<_Er>
     && (!is_trivially_move_constructible_v<_Er>)
      : _M_void(), _M_has_value(__x._M_has_value)
      {
 if (!_M_has_value)
   std::construct_at(__builtin_addressof(_M_unex),
       std::move(__x)._M_unex);
      }

      template<typename _Up, typename _Gr>
 requires is_void_v<_Up>
       && is_constructible_v<_Er, const _Gr&>
       && (!__cons_from_expected<_Up, _Gr>)
 constexpr explicit(!is_convertible_v<const _Gr&, _Er>)
 expected(const expected<_Up, _Gr>& __x)
 noexcept(is_nothrow_constructible_v<_Er, const _Gr&>)
 : _M_void(), _M_has_value(__x._M_has_value)
 {
   if (!_M_has_value)
     std::construct_at(__builtin_addressof(_M_unex), __x._M_unex);
 }

      template<typename _Up, typename _Gr>
 requires is_void_v<_Up>
       && is_constructible_v<_Er, _Gr>
       && (!__cons_from_expected<_Up, _Gr>)
 constexpr explicit(!is_convertible_v<_Gr, _Er>)
 expected(expected<_Up, _Gr>&& __x)
 noexcept(is_nothrow_constructible_v<_Er, _Gr>)
 : _M_void(), _M_has_value(__x._M_has_value)
 {
   if (!_M_has_value)
     std::construct_at(__builtin_addressof(_M_unex),
         std::move(__x)._M_unex);
 }

      template<typename _Gr = _Er>
 requires is_constructible_v<_Er, const _Gr&>
 constexpr explicit(!is_convertible_v<const _Gr&, _Er>)
 expected(const unexpected<_Gr>& __u)
 noexcept(is_nothrow_constructible_v<_Er, const _Gr&>)
 : _M_unex(__u.error()), _M_has_value(false)
 { }

      template<typename _Gr = _Er>
 requires is_constructible_v<_Er, _Gr>
 constexpr explicit(!is_convertible_v<_Gr, _Er>)
 expected(unexpected<_Gr>&& __u)
 noexcept(is_nothrow_constructible_v<_Er, _Gr>)
 : _M_unex(std::move(__u).error()), _M_has_value(false)
 { }

      template<typename... _Args>
 constexpr explicit
 expected(in_place_t) noexcept
 : expected()
 { }

      template<typename... _Args>
 requires is_constructible_v<_Er, _Args...>
 constexpr explicit
 expected(unexpect_t, _Args&&... __args)
 noexcept(is_nothrow_constructible_v<_Er, _Args...>)
 : _M_unex(std::forward<_Args>(__args)...), _M_has_value(false)
 { }

      template<typename _Up, typename... _Args>
 requires is_constructible_v<_Er, initializer_list<_Up>&, _Args...>
 constexpr explicit
 expected(unexpect_t, initializer_list<_Up> __il, _Args&&... __args)
 noexcept(is_nothrow_constructible_v<_Er, initializer_list<_Up>&,
         _Args...>)
 : _M_unex(__il, std::forward<_Args>(__args)...), _M_has_value(false)
 { }

      constexpr ~expected() = default;

      constexpr ~expected() requires (!is_trivially_destructible_v<_Er>)
      {
 if (!_M_has_value)
   std::destroy_at(__builtin_addressof(_M_unex));
      }

      expected& operator=(const expected&) = delete;

      constexpr expected&
      operator=(const expected& __x)
      noexcept(__and_v<is_nothrow_copy_constructible<_Er>,
         is_nothrow_copy_assignable<_Er>>)
      requires is_copy_constructible_v<_Er>
     && is_copy_assignable_v<_Er>
      {
 if (__x._M_has_value)
   emplace();
 else
   _M_assign_unex(__x._M_unex);
 return *this;
      }

      constexpr expected&
      operator=(expected&& __x)
      noexcept(__and_v<is_nothrow_move_constructible<_Er>,
         is_nothrow_move_assignable<_Er>>)
      requires is_move_constructible_v<_Er>
     && is_move_assignable_v<_Er>
      {
 if (__x._M_has_value)
   emplace();
 else
   _M_assign_unex(std::move(__x._M_unex));
 return *this;
      }

      template<typename _Gr>
 requires is_constructible_v<_Er, const _Gr&>
       && is_assignable_v<_Er&, const _Gr&>
 constexpr expected&
 operator=(const unexpected<_Gr>& __e)
 {
   _M_assign_unex(__e.error());
   return *this;
 }

      template<typename _Gr>
 requires is_constructible_v<_Er, _Gr>
       && is_assignable_v<_Er&, _Gr>
 constexpr expected&
 operator=(unexpected<_Gr>&& __e)
 {
   _M_assign_unex(std::move(__e.error()));
   return *this;
 }

      constexpr void
      emplace() noexcept
      {
 if (!_M_has_value)
   {
     std::destroy_at(__builtin_addressof(_M_unex));
     _M_has_value = true;
   }
      }

      constexpr void
      swap(expected& __x)
      noexcept(__and_v<is_nothrow_swappable<_Er&>,
         is_nothrow_move_constructible<_Er>>)
      requires is_swappable_v<_Er> && is_move_constructible_v<_Er>
      {
 if (_M_has_value)
   {
     if (!__x._M_has_value)
       {
  std::construct_at(__builtin_addressof(_M_unex),
      std::move(__x._M_unex));
  std::destroy_at(__builtin_addressof(__x._M_unex));
  _M_has_value = false;
  __x._M_has_value = true;
       }
   }
 else
   {
     if (__x._M_has_value)
       {
  std::construct_at(__builtin_addressof(__x._M_unex),
      std::move(_M_unex));
  std::destroy_at(__builtin_addressof(_M_unex));
  _M_has_value = true;
  __x._M_has_value = false;
       }
     else
       {
  using std::swap;
  swap(_M_unex, __x._M_unex);
       }
   }
      }

      [[nodiscard]]
      constexpr explicit
      operator bool() const noexcept { return _M_has_value; }

      [[nodiscard]]
      constexpr bool has_value() const noexcept { return _M_has_value; }

      constexpr void
      operator*() const noexcept { do { if (std::__is_constant_evaluated() && !bool(_M_has_value)) __builtin_unreachable(); } while (false); }

      constexpr void
      value() const&
      {
 if (_M_has_value) [[likely]]
   return;
 (throw (bad_expected_access<_Er>(_M_unex)));
      }

      constexpr void
      value() &&
      {
 if (_M_has_value) [[likely]]
   return;
 (throw (bad_expected_access<_Er>(std::move(_M_unex))));
      }

      constexpr const _Er&
      error() const & noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(!_M_has_value)) __builtin_unreachable(); } while (false);
 return _M_unex;
      }

      constexpr _Er&
      error() & noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(!_M_has_value)) __builtin_unreachable(); } while (false);
 return _M_unex;
      }

      constexpr const _Er&&
      error() const && noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(!_M_has_value)) __builtin_unreachable(); } while (false);
 return std::move(_M_unex);
      }

      constexpr _Er&&
      error() && noexcept
      {
 do { if (std::__is_constant_evaluated() && !bool(!_M_has_value)) __builtin_unreachable(); } while (false);
 return std::move(_M_unex);
      }

      template<typename _Gr = _Er>
 constexpr _Er
 error_or(_Gr&& __e) const&
 {
   static_assert( is_copy_constructible_v<_Er> );
   static_assert( is_convertible_v<_Gr, _Er> );

   if (_M_has_value)
     return std::forward<_Gr>(__e);
   return _M_unex;
 }

      template<typename _Gr = _Er>
 constexpr _Er
 error_or(_Gr&& __e) &&
 {
   static_assert( is_move_constructible_v<_Er> );
   static_assert( is_convertible_v<_Gr, _Er> );

   if (_M_has_value)
     return std::forward<_Gr>(__e);
   return std::move(_M_unex);
 }

      template<typename _Fn> requires is_copy_constructible_v<_Er>
 constexpr auto
 and_then(_Fn&& __f) &
 {
   using _Up = __expected::__result0<_Fn>;
   static_assert(__expected::__is_expected<_Up>);
   static_assert(is_same_v<typename _Up::error_type, _Er>);

   if (has_value())
     return std::__invoke(std::forward<_Fn>(__f));
   else
     return _Up(unexpect, error());
 }

     template<typename _Fn> requires is_copy_constructible_v<_Er>
 constexpr auto
 and_then(_Fn&& __f) const &
 {
   using _Up = __expected::__result0<_Fn>;
   static_assert(__expected::__is_expected<_Up>);
   static_assert(is_same_v<typename _Up::error_type, _Er>);

   if (has_value())
     return std::__invoke(std::forward<_Fn>(__f));
   else
     return _Up(unexpect, error());
 }

      template<typename _Fn> requires is_move_constructible_v<_Er>
 constexpr auto
 and_then(_Fn&& __f) &&
 {
   using _Up = __expected::__result0<_Fn>;
   static_assert(__expected::__is_expected<_Up>);
   static_assert(is_same_v<typename _Up::error_type, _Er>);

   if (has_value())
     return std::__invoke(std::forward<_Fn>(__f));
   else
     return _Up(unexpect, std::move(error()));
 }

       template<typename _Fn> requires is_move_constructible_v<_Er>
 constexpr auto
 and_then(_Fn&& __f) const &&
 {
   using _Up = __expected::__result0<_Fn>;
   static_assert(__expected::__is_expected<_Up>);
   static_assert(is_same_v<typename _Up::error_type, _Er>);

   if (has_value())
     return std::__invoke(std::forward<_Fn>(__f));
   else
     return _Up(unexpect, std::move(error()));
 }

      template<typename _Fn>
 constexpr auto
 or_else(_Fn&& __f) &
 {
   using _Gr = __expected::__result<_Fn, _Er&>;
   static_assert(__expected::__is_expected<_Gr>);
   static_assert(is_same_v<typename _Gr::value_type, _Tp>);

   if (has_value())
     return _Gr();
   else
     return std::__invoke(std::forward<_Fn>(__f), error());
 }

      template<typename _Fn>
 constexpr auto
 or_else(_Fn&& __f) const &
 {
   using _Gr = __expected::__result<_Fn, const _Er&>;
   static_assert(__expected::__is_expected<_Gr>);
   static_assert(is_same_v<typename _Gr::value_type, _Tp>);

   if (has_value())
     return _Gr();
   else
     return std::__invoke(std::forward<_Fn>(__f), error());
 }

      template<typename _Fn>
 constexpr auto
 or_else(_Fn&& __f) &&
 {
   using _Gr = __expected::__result<_Fn, _Er&&>;
   static_assert(__expected::__is_expected<_Gr>);
   static_assert(is_same_v<typename _Gr::value_type, _Tp>);

   if (has_value())
     return _Gr();
   else
     return std::__invoke(std::forward<_Fn>(__f), std::move(error()));
 }

      template<typename _Fn>
 constexpr auto
 or_else(_Fn&& __f) const &&
 {
   using _Gr = __expected::__result<_Fn, const _Er&&>;
   static_assert(__expected::__is_expected<_Gr>);
   static_assert(is_same_v<typename _Gr::value_type, _Tp>);

   if (has_value())
     return _Gr();
   else
     return std::__invoke(std::forward<_Fn>(__f), std::move(error()));
 }

      template<typename _Fn> requires is_copy_constructible_v<_Er>
 constexpr auto
 transform(_Fn&& __f) &
 {
   using _Up = __expected::__result0<_Fn>;
   using _Res = expected<_Up, _Er>;

   if (has_value())
     return _Res(__in_place_inv{}, std::forward<_Fn>(__f));
   else
     return _Res(unexpect, error());
 }

      template<typename _Fn> requires is_copy_constructible_v<_Er>
 constexpr auto
 transform(_Fn&& __f) const &
 {
   using _Up = __expected::__result0<_Fn>;
   using _Res = expected<_Up, _Er>;

   if (has_value())
     return _Res(__in_place_inv{}, std::forward<_Fn>(__f));
   else
     return _Res(unexpect, error());
 }

      template<typename _Fn> requires is_move_constructible_v<_Er>
 constexpr auto
 transform(_Fn&& __f) &&
 {
   using _Up = __expected::__result0<_Fn>;
   using _Res = expected<_Up, _Er>;

   if (has_value())
     return _Res(__in_place_inv{}, std::forward<_Fn>(__f));
   else
     return _Res(unexpect, std::move(error()));
 }

      template<typename _Fn> requires is_move_constructible_v<_Er>
 constexpr auto
 transform(_Fn&& __f) const &&
 {
   using _Up = __expected::__result0<_Fn>;
   using _Res = expected<_Up, _Er>;

   if (has_value())
     return _Res(__in_place_inv{}, std::forward<_Fn>(__f));
   else
     return _Res(unexpect, std::move(error()));
 }

      template<typename _Fn>
 constexpr auto
 transform_error(_Fn&& __f) &
 {
   using _Gr = __expected::__result<_Fn, _Er&>;
   using _Res = expected<_Tp, _Gr>;

   if (has_value())
     return _Res();
   else
     return _Res(__unexpect_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            _M_unex);
   });
 }

      template<typename _Fn>
 constexpr auto
 transform_error(_Fn&& __f) const &
 {
   using _Gr = __expected::__result<_Fn, const _Er&>;
   using _Res = expected<_Tp, _Gr>;

   if (has_value())
     return _Res();
   else
     return _Res(__unexpect_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            _M_unex);
   });
 }

      template<typename _Fn>
 constexpr auto
 transform_error(_Fn&& __f) &&
 {
   using _Gr = __expected::__result<_Fn, _Er&&>;
   using _Res = expected<_Tp, _Gr>;

   if (has_value())
     return _Res();
   else
     return _Res(__unexpect_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            std::move(_M_unex));
   });
 }

      template<typename _Fn>
 constexpr auto
 transform_error(_Fn&& __f) const &&
 {
   using _Gr = __expected::__result<_Fn, const _Er&&>;
   using _Res = expected<_Tp, _Gr>;

   if (has_value())
     return _Res();
   else
     return _Res(__unexpect_inv{}, [&]() {
     return std::__invoke(std::forward<_Fn>(__f),
            std::move(_M_unex));
   });
 }

      template<typename _Up, typename _Er2>
 requires is_void_v<_Up>
 friend constexpr bool
 operator==(const expected& __x, const expected<_Up, _Er2>& __y)

 {
   if (__x.has_value())
     return __y.has_value();
   else
     return !__y.has_value() && bool(__x.error() == __y.error());
 }

      template<typename _Er2>
 friend constexpr bool
 operator==(const expected& __x, const unexpected<_Er2>& __e)

 { return !__x.has_value() && bool(__x.error() == __e.error()); }

      friend constexpr void
      swap(expected& __x, expected& __y)
      noexcept(noexcept(__x.swap(__y)))
      requires requires { __x.swap(__y); }
      { __x.swap(__y); }

    private:
      template<typename, typename> friend class expected;

      template<typename _Vp>
 constexpr void
 _M_assign_unex(_Vp&& __v)
 {
   if (_M_has_value)
     {
       std::construct_at(__builtin_addressof(_M_unex),
    std::forward<_Vp>(__v));
       _M_has_value = false;
     }
   else
     _M_unex = std::forward<_Vp>(__v);
 }

      using __in_place_inv = __expected::__in_place_inv;
      using __unexpect_inv = __expected::__unexpect_inv;

      template<typename _Fn>
 explicit constexpr
 expected(__in_place_inv, _Fn&& __fn)
 : _M_void(), _M_has_value(true)
 { std::forward<_Fn>(__fn)(); }

      template<typename _Fn>
 explicit constexpr
 expected(__unexpect_inv, _Fn&& __fn)
 : _M_unex(std::forward<_Fn>(__fn)()), _M_has_value(false)
 { }

      union {
 struct { } _M_void;
 _Er _M_unex;
      };

      bool _M_has_value;
    };
}