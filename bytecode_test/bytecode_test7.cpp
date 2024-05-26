#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <tuple>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <array>
#include <bit>

#define USE_PRESERVE_NONE 0

#if USE_PRESERVE_NONE
#if __clang__
#define preserve_none [[clang::preserve_none]]
#else
#define preserve_none __attribute__((no_callee_saved_registers))
#endif
#else
#define preserve_none
#endif

#define countof(array_type) (sizeof(array_type) / sizeof(array_type[0]))

// Utility templates

template <typename T>
using SIForSize = std::conditional_t<sizeof(T) == sizeof(int8_t), int8_t,
                  std::conditional_t<sizeof(T) == sizeof(int16_t), int16_t,
                  std::conditional_t<sizeof(T) == sizeof(int32_t), int32_t,
                  std::conditional_t<sizeof(T) == sizeof(int64_t), int64_t,
                  void>>>>;

template <typename T>
using UIForSize = std::conditional_t<sizeof(T) == sizeof(uint8_t), uint8_t,
                  std::conditional_t<sizeof(T) == sizeof(uint16_t), uint16_t,
                  std::conditional_t<sizeof(T) == sizeof(uint32_t), uint32_t,
                  std::conditional_t<sizeof(T) == sizeof(uint64_t), uint64_t,
                  void>>>>;

template <typename T>
static constexpr UIForSize<T> bit_cast_from_size(T value) {
    return std::bit_cast<UIForSize<T>>(value);
}

template <typename T>
static constexpr T bit_cast_to_size(UIForSize<T> value) {
    return std::bit_cast<T>(value);
}

template <typename T1, typename T2 = T1>
static inline constexpr bool btr(T1& value, T2 index) {
    uint32_t temp = value;
    value &= ~(1 << index);
    return value != temp;
}

template <typename R, typename B, typename... Args>
struct func_traits_data {
    using class_type = B;

    using ret_type = R;
    using arg_types = ::std::tuple<Args...>;

    static inline constexpr size_t arg_count = sizeof...(Args);

    template<size_t N>
    using nth_arg_type = std::tuple_element_t<N, arg_types>;
};

template <typename F>
struct func_traits_impl;

template <typename R, typename... Args>
struct func_traits_impl<R(*)(Args...)> : func_traits_data<R, void, Args...> {};

template <typename R, typename B, typename... Args>
struct func_traits_impl<R(B::*)(Args...)> : func_traits_data<R, B, Args...> {};

template <typename R, typename B, typename... Args>
struct func_traits_impl<R(B::*)(Args...) const> : func_traits_data<R, B, Args...> {};

template <typename L, typename = void>
struct has_call_operator : std::false_type {};

template <typename L>
struct has_call_operator<L, std::void_t<decltype(&L::operator())>> : std::true_type {};

template <typename L>
inline constexpr bool has_call_operator_v = has_call_operator<L>::value;

template <typename F, typename = void>
struct func_traits;

template <typename F> requires(std::is_function_v<F>)
struct func_traits<F, void> : func_traits_impl<decltype(&std::declval<F>())> {};

template <typename L> requires(has_call_operator_v<L>)
struct func_traits<L, void> : func_traits_impl<decltype(&L::operator())> {};

// Stack type

template <typename U, typename F>
struct StackImpl {

    U* sp;

    using type = U;

private:
    template<typename T>
    static consteval inline size_t size_in_underlying_multiple() {
        size_t i = 0;
        while (sizeof(U) * ++i < sizeof(T));
        return i;
    }
public:

    template<typename T = U>
    constexpr inline T read(size_t index = 0) const {
        U temp = *(this->sp - index);
        return bit_cast_to_size<T>(temp);
    }
    template<typename T = U>
    constexpr inline void write(T value, size_t index = 0) {
        if consteval {
            // This is all just a constexpr way of
            // saying "only write to the low bytes
            // if the type being written is small"
            U temp = this->read(index);
            auto cast_value = bit_cast_from_size<T>(value);
            temp &= ~((U)~((decltype(cast_value))0));
            temp |= cast_value;
            *(this->sp - index) = temp;
        } else {
            // GCC actually emits code for the
            // complicated version rather than
            // understanding what it's doing like
            // clang does, so keep the simple
            // version around
            *(T*)(this->sp - index) = value;
        }
    }

    template<typename T = U>
    constexpr inline void push(T value) {
        this->sp += size_in_underlying_multiple<T>();
        this->write(value);
    }
    template<typename T = U>
    constexpr inline T pop() {
        T temp = bit_cast_to_size<T>(*this->sp);
        this->sp -= size_in_underlying_multiple<T>();
        return temp;
    }
    template<typename R = U, typename T = U>
    constexpr inline R exchange(T value, size_t index = 0) {
        R temp = this->read<R>(index);
        this->write<T>(value, index);
        return temp;
    }
    template<typename T = U>
    constexpr inline void swap(T& value, size_t index = 0) {
        value = this->exchange<T>(value, index);
    }

    template<typename L>
    constexpr inline void unary_op(U& itop, F& ftop, const L& func) {
        using A = typename func_traits<L>::template nth_arg_type<0>;
        using R = func_traits<L>::ret_type;
        if constexpr (std::is_same_v<R, F>) {
            if constexpr (std::is_same_v<A, F>) {
                ftop = func(ftop);
            } else {
                U value = itop;
                itop = this->exchange(ftop);
                ftop = func((A)value);
            }
        } else {
            if constexpr (std::is_same_v<A, F>) {
                F value = ftop;
                ftop = this->exchange(itop);
                itop = func(value);
            } else {
                itop = func((A)itop);
            }
        }
    }
    template<typename L>
    constexpr inline void binary_op(U& itop, F& ftop, const L& func) {
        using T = typename func_traits<L>::template nth_arg_type<1>;
        using V = typename func_traits<L>::template nth_arg_type<0>;
        using R = func_traits<L>::ret_type;
        if constexpr (std::is_same_v<R, F>) {
            if constexpr (std::is_same_v<T, F>) {
                if constexpr (std::is_same_v<V, F>) {
                    // F = func(F, F)
                    ftop = func(this->pop<V>(), ftop);
                } else {
                    // F = func(I, F)
                    ftop = func((V)itop, ftop);
                    itop = this->pop();
                }
            } else {
                if constexpr (std::is_same_v<V, F>) {
                    // F = func(F, I)
                    ftop = func(ftop, (T)itop);
                    itop = this->pop();
                } else {
                    // F = func(I, I)
                    V lhs = this->pop<V>();
                    U rhs = itop;
                    itop = this->exchange(ftop);
                    ftop = func(lhs, (T)rhs);
                }
            }
        } else {
            if constexpr (std::is_same_v<T, F>) {
                if constexpr (std::is_same_v<V, F>) {
                    // I = func(F, F)
                    V lhs = this->pop<V>();
                    F rhs = ftop;
                    ftop = this->exchange<F>(itop);
                    itop = func(lhs, rhs);
                } else {
                    // I = func(I, F)
                    itop = func((V)itop, ftop);
                    ftop = this->pop<F>();
                }
            } else {
                if constexpr (std::is_same_v<V, F>) {
                    // I = func(F, I)
                    itop = func(ftop, (T)itop);
                    ftop = this->pop<F>();
                } else {
                    // I = func(I, I)
                    itop = func(this->pop<V>(), (T)itop);
                }
            }
        }
    }
};

// Opcodes

enum Opcode : uint32_t {
    DUMMY, // Null terminator style script end
    NOP,
    ENDL, // End parsing, return literal value
    ENDT, // End parsing, return stack top
    PUSHL, // Push literal
    PUSHV, // Push value from stack
    POP,
    XCHG,
    ADD,
    SUB,
    MUL,
    DIV,
    IDIV,
    MOD,
    IMOD,
    AND,
    OR,
    XOR,
    SHL,
    SHR,
    SAR,
    NEG,
    NOT,
    EQU,
    NEQ,
    LES,
    LEQ,
    GTR,
    GEQ,
    BEL,
    BEQ,
    ABV,
    AEQ,
    JMP,
    JZ,
    JNZ,
    CALL,
    RET, // Return
    RETL, // Return and push literal
    RETT, // Return and push stack top
    FRETL, // Return and push literal (float)
    FRETT, // Return and push stack top (float)
    OPSIZE, // Push stack operand size
    INSSIZE, // Push instruction size
    I2F,
    U2F,
    F2I,
    F2U,
    FPUSHL,
    FPUSHV,
    FPOP,
    FXCHG,
    FADD,
    FSUB,
    FMUL,
    FDIV,
    FNEG,
    FEQU,
    FNEQ,
    FLES,
    FLEQ,
    FGTR,
    FGEQ,
    FSIN,
    FCOS,
    FATAN2,
    FSQRT,
    PRINT, // Print int from stack
    FPRINT, // Print float from stack
    MAX_OPCODE
};

// Instruction format

using SI = intptr_t;
using UI = uintptr_t;
using FL = float;

using Stack = StackImpl<UI, FL>;

struct Instr;

preserve_none typedef UI instr_impl(const Instr* ip, Stack stack, UI itop, FL ftop);

struct Instr {
    union {
        Opcode opcode;
        instr_impl * next;
    };
    union {
        UI arg;
        FL argf;
        const char* str;
    };
};

// Dumb lookup tables

// This is all here just to wrap the printf format specifiers
// for exact size integer types into a fixed size character array
// without needing to know what their underlying types are
// at compile time. It is extremely dumb.

template<typename T, typename... IntsT>
static consteval size_t max_in_format_count(size_t max = 0) {
    size_t current_type_size = 0;

    if constexpr (std::is_same_v<T, char>) current_type_size = countof("%c");
    else if constexpr (std::is_same_v<T, wchar_t>) current_type_size = countof("%lc");
    else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, long double>) current_type_size = countof("%lf");
    else if constexpr (std::is_same_v<T, float>) current_type_size = countof("%f");
    else {
        using U = std::make_unsigned_t<T>;
        if constexpr (std::is_same_v<U, unsigned char> || std::is_same_v<U, unsigned long long>) current_type_size = countof("%hhu");
        else if constexpr (std::is_same_v<U, unsigned short> || std::is_same_v<U, unsigned long>) current_type_size = countof("%hu");
        else if constexpr (std::is_same_v<U, unsigned int>) current_type_size = countof("%u");
        else static_assert(false);
    }

    max = std::max(current_type_size, max);

    if constexpr (sizeof...(IntsT)) {
        return max_in_format_count<IntsT...>(max);
    } else {
        return max;
    }
}

static inline constexpr size_t longest_format_count = max_in_format_count<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, long double>();

using fmt_str = std::array<char, std::bit_ceil(longest_format_count)>;

template<size_t N>
static consteval fmt_str make_fmt_str(const char(&str)[N]) {
    char temp[std::bit_ceil(longest_format_count)] = {};
    for (size_t i = 0; i < N; ++i) {
        temp[i] = str[i];
    }
    return std::to_array(temp);
}

template<typename T>
static consteval fmt_str format_for_int_type(bool is_hex) {
         if constexpr (std::is_same_v<T, signed char>)        return make_fmt_str(is_hex ? "%hhx" : "%hhi");
    else if constexpr (std::is_same_v<T, unsigned char>)      return make_fmt_str(is_hex ? "%hhX" : "%hhu");
    else if constexpr (std::is_same_v<T, short>)              return make_fmt_str(is_hex ? "%hx" : "%hi");
    else if constexpr (std::is_same_v<T, unsigned short>)     return make_fmt_str(is_hex ? "%hX" : "%hu");
    else if constexpr (std::is_same_v<T, int>)                return make_fmt_str(is_hex ? "%x" : "%i");
    else if constexpr (std::is_same_v<T, unsigned int>)       return make_fmt_str(is_hex ? "%X" : "%u");
    else if constexpr (std::is_same_v<T, long>)               return make_fmt_str(is_hex ? "%lx" : "%li");
    else if constexpr (std::is_same_v<T, unsigned long>)      return make_fmt_str(is_hex ? "%lX" : "%lu");
    else if constexpr (std::is_same_v<T, long long>)          return make_fmt_str(is_hex ? "%llx" : "%lli");
    else if constexpr (std::is_same_v<T, unsigned long long>) return make_fmt_str(is_hex ? "%llX" : "%llu");
    else static_assert(false);
}
template<typename T, size_t variant>
static consteval fmt_str format_for_float_type(bool is_capital) {
    if constexpr (std::is_same_v<T, long double>) {
             if constexpr (variant == 0) return make_fmt_str(is_capital ? "%LF" : "%Lf");
        else if constexpr (variant == 1) return make_fmt_str(is_capital ? "%LG" : "%Lg");
        else if constexpr (variant == 2) return make_fmt_str(is_capital ? "%LE" : "%Le");
        else static_assert(false);
    }
         if constexpr (variant == 0) return make_fmt_str(is_capital ? "%F" : "%f");
    else if constexpr (variant == 1) return make_fmt_str(is_capital ? "%G" : "%g");
    else if constexpr (variant == 2) return make_fmt_str(is_capital ? "%E" : "%e");
    else static_assert(false);
}
template<typename T>
static consteval fmt_str format_for_text_type() {
         if constexpr (std::is_same_v<T, char>)    return make_fmt_str("%c");
    else if constexpr (std::is_same_v<T, wchar_t>) return make_fmt_str("%lc");
    else static_assert(false);
}

static inline constexpr auto first_format_char = 'A';

static inline constexpr auto first_int_format = 'A';
static inline constexpr auto first_float_format = 'E';

#define F(letter) ((letter)-first_format_char)

// This lookup table is used for all format strings

// GCC only allows array designators if
// all array elements are specified.
static inline constexpr fmt_str format_strings[] = {
    /*  0 */ [F('A')] = format_for_int_type<uint8_t>(true),
    /*  1 */ [F('B')] = format_for_int_type<uint8_t>(false),
    /*  2 */ [F('C')] = format_for_int_type<int8_t>(false),
    /*  3 */ [F('D')] = format_for_int_type<int8_t>(true),
    /*  4 */ [F('E')] = format_for_float_type<float,2>(false),
    /*  5 */ [F('F')] = format_for_float_type<float,0>(false),
    /*  6 */ [F('G')] = format_for_float_type<float,1>(false),
    /*  7 */ [F('H')] = format_for_int_type<uintptr_t>(true),
    /*  8 */ [F('I')] = format_for_int_type<int32_t>(false),
    /*  9 */ [F('J')] = format_for_int_type<int32_t>(true),
    /* 10 */ [F('K')] = format_for_int_type<int64_t>(true),
    /* 11 */ [F('L')] = format_for_int_type<int64_t>(false),
    /* 12 */ [F('M')] = format_for_float_type<float,2>(true),
    /* 13 */ [F('N')] = format_for_float_type<float,0>(true),
    /* 14 */ [F('O')] = format_for_float_type<float,1>(true),
    /* 15 */ [F('P')] = format_for_int_type<uintptr_t>(false),
    /* 16 */ [F('Q')] = format_for_int_type<uint64_t>(false),
    /* 17 */ [F('R')] = format_for_int_type<uint64_t>(true),
    /* 18 */ [F('S')] = format_for_int_type<int16_t>(false),
    /* 19 */ [F('T')] = format_for_int_type<int16_t>(true),
    /* 20 */ [F('U')] = format_for_int_type<uint32_t>(false),
    /* 21 */ [F('V')] = format_for_int_type<uint32_t>(true),
    /* 22 */ [F('W')] = format_for_int_type<uint16_t>(false),
    /* 23 */ [F('X')] = format_for_int_type<uint16_t>(true),
    /* 24 */ [F('Y')] = format_for_text_type<char>(),
    /* 25 */ [F('Z')] = format_for_text_type<wchar_t>()
};

#undef F

// Instruction implementations

template<bool is_constexpr = false>
struct Dispatch {

private:
    using InstrA = const Instr*;

    struct InstrB {
        const Instr* instrs;
        mutable UI index;

        constexpr InstrB(const Instr* instrs) : instrs(instrs), index(0) {}
        constexpr InstrB(const Instr* instrs, UI index) : instrs(instrs), index(index) {}
        constexpr InstrB& operator=(size_t index) {
            this->index = index;
            return *this;
        }
        constexpr explicit operator UI() const {
            return this->index;
        }
        constexpr const Instr* operator->() const {
            return &this->instrs[this->index];
        }
        constexpr InstrB operator+(size_t value) const {
            return { instrs, index + value };
        }
        constexpr InstrB& operator++() {
            ++this->index;
            return *this;
        }
    };
public:

    using InstrPtr = std::conditional_t<is_constexpr, InstrB, InstrA>;
    using StackIP = std::conditional_t<is_constexpr, UI, InstrPtr>;

    preserve_none typedef UI dispatch_instr_impl (InstrPtr ip, Stack stack, UI itop, FL ftop);

    preserve_none static constexpr UI dispatch_direct(InstrPtr ip, Stack stack, UI itop, FL ftop);
    preserve_none static constexpr UI dispatch(InstrPtr ip, Stack stack, UI itop, FL ftop);

    alignas(64) static constexpr dispatch_instr_impl *const dispatch_table[MAX_OPCODE] = {
        [DUMMY] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            return dispatch_direct(ip, stack, itop, ftop);
        },
        [NOP] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            return dispatch(ip, stack, itop, ftop);
        },
        [ENDL] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            return ip->arg;
        },
        [ENDT] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            return itop;
        },
        [PUSHL] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.push(itop);
            itop = ip->arg;
            return dispatch(ip, stack, itop, ftop);
        },
        [PUSHV] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.push(itop);
            itop = stack.read(ip->arg);
            return dispatch(ip, stack, itop, ftop);
        },
        [POP] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            for (size_t i = 0; i < ip->arg; ++i) itop = stack.pop();
            return dispatch(ip, stack, itop, ftop);
        },
        [XCHG] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            // exchanging the stack top with itself would be
            // meaningless, so the index can just be offset by 1
            if (uintptr_t index = ip->arg) {
                stack.swap(itop, index - 1);
            }
            return dispatch(ip, stack, itop, ftop);
        },
        [ADD] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                return lhs + rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [SUB] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                return lhs - rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [MUL] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                return lhs * rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [DIV] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                [[assume(rhs != 0)]];
                return lhs / rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [IDIV] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](SI lhs, SI rhs) {
                [[assume(rhs != 0)]];
                return lhs / rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [MOD] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                [[assume(rhs != 0)]];
                return lhs % rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [IMOD] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](SI lhs, SI rhs) {
                [[assume(rhs != 0)]];
                return lhs % rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [AND] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                return lhs & rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [OR] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                return lhs | rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [XOR] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                return lhs ^ rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [SHL] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                return lhs << rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [SHR] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                return lhs >> rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [SAR] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](SI lhs, SI rhs) {
                return lhs >> rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [NEG] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.unary_op(itop, ftop, [](SI value) {
                return -value;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [NOT] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.unary_op(itop, ftop, [](UI value) {
                return ~value;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [EQU] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                return lhs == rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [NEQ] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                return lhs != rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [LES] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
                return lhs < rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [LEQ] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
                return lhs <= rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [GTR] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
                return lhs > rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [GEQ] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
                return lhs >= rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [BEL] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                return lhs < rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [BEQ] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                return lhs <= rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [ABV] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                return lhs > rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [AEQ] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                return lhs >= rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [JMP] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            return dispatch_direct(ip + ip->arg, stack, itop, ftop);
        },
        [JZ] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            intptr_t offset = stack.pop() ? 1 : ip->arg;
            return dispatch_direct(ip + offset, stack, itop, ftop);
        },
        [JNZ] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            intptr_t offset = !stack.pop() ? 1 : ip->arg;
            return dispatch_direct(ip + offset, stack, itop, ftop);
        },
        [CALL] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            // Stack state after call:
            // Old itop is pushed on top of the stack
            // Return address is in itop
            // ftop is unchanged
            stack.push(itop);
            itop = (UI)(ip + 1);
            return dispatch_direct(ip + ip->arg, stack, itop, ftop);
        },
        [RET] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            // No data is getting returned, so just pop
            // the pre-call itop value back into itop.
            ip = std::bit_cast<StackIP>(itop);
            itop = stack.pop();
            return dispatch_direct(ip, stack, itop, ftop);
        },
        [RETL] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            // When returning an int literal there won't be
            // any data from the called sub left on the stack
            // and the new value can be written directly to itop.
            StackIP new_ip = std::bit_cast<StackIP>(itop);
            itop = ip->arg;
            ip = new_ip;
            return dispatch_direct(ip, stack, itop, ftop);
        },
        [RETT] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            // If any ints have been pushed during a sub,
            // the return address will have been pushed onto
            // the stack.
            //
            // Since the value getting returned is just going to
            // stay in the itop register, the return address can
            // be popped directly into ip.
            ip = stack.pop<StackIP>();
            return dispatch_direct(ip, stack, itop, ftop);
        },
        [FRETL] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            // When returning a float literal there won't be
            // any data from the called sub left on the stack
            // and ftop will still contain its value from
            // before the call.
            //
            // This can be handled by just writing ftop into
            // the slot that gets popped into itop and then
            // writing the new value into ftop.
            StackIP new_ip = std::bit_cast<StackIP>(itop);
            itop = stack.exchange(ftop);
            ftop = ip->argf;
            ip = new_ip;
            return dispatch_direct(ip, stack, itop, ftop);
        },
        [FRETT] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            // If any floats have been pushed during a sub,
            // the ftop from before the call will be on top
            // of the stack, but this should be handled invisibly
            // as far as itop is concerned for managing its pop.
            //
            // Since the value getting returned is just going to
            // stay in the ftop register, the old ftop needs to be
            // slid down into the stack slot occupied by the value
            // popped into itop.
            ip = std::bit_cast<StackIP>(itop);
            itop = stack.exchange(stack.pop<FL>());
            return dispatch_direct(ip, stack, itop, ftop);
        },
        [OPSIZE] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.push(itop);
            itop = sizeof(Stack::type);
            return dispatch(ip, stack, itop, ftop);
        },
        [INSSIZE] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.push(itop);
            itop = sizeof(Instr);
            return dispatch(ip, stack, itop, ftop);
        },
        [I2F] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.unary_op(itop, ftop, [](SI value) {
                return (FL)value;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [U2F] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.unary_op(itop, ftop, [](UI value) {
                return (FL)value;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [F2I] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.unary_op(itop, ftop, [](FL value) {
                return (SI)value;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [F2U] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.unary_op(itop, ftop, [](FL value) {
                return (UI)value;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FPUSHL] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.push(ftop);
            ftop = ip->argf;
            return dispatch(ip, stack, itop, ftop);
        },
        [FPUSHV] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.push(ftop);
            ftop = stack.read<FL>(ip->arg);
            return dispatch(ip, stack, itop, ftop);
        },
        [FPOP] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            for (size_t i = 0; i < ip->arg; ++i) ftop = stack.pop<FL>();
            return dispatch(ip, stack, itop, ftop);
        },
        [FXCHG] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            // exchanging the stack top with itself would be
            // meaningless, so the index can just be offset by 1
            if (uintptr_t index = ip->arg) {
                stack.swap(ftop, index - 1);
            }
            return dispatch(ip, stack, itop, ftop);
        },
        [FADD] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
                return lhs + rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FSUB] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
                return lhs - rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FMUL] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
                return lhs * rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FDIV] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
                return lhs / rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FNEG] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.unary_op(itop, ftop, [](FL value) {
                return -value;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FEQU] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                return lhs == rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FNEQ] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                return lhs != rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FLES] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                return lhs < rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FLEQ] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                return lhs <= rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FGTR] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                return lhs > rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FGEQ] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                return lhs >= rhs;
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FSIN] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.unary_op(itop, ftop, [](FL value) {
                return sinf(value);
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FCOS] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.unary_op(itop, ftop, [](FL value) {
                return cosf(value);
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FATAN2] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
                return atan2f(rhs, lhs);
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [FSQRT] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            stack.unary_op(itop, ftop, [](FL value) {
                [[assume(value >= 0.0)]];
                return sqrtf(value);
            });
            return dispatch(ip, stack, itop, ftop);
        },
        [PRINT] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            uint32_t format = ip->arg;
            bool is_capital = !btr(format, 5);
            // This switch doesn't get compiled to a memory lookup table,
            // instead searching for a bit in an immediate
            switch (format) {
                case 'B': case 'C': // uint8_t / int8_t
                case 'W': case 'S': // uint16_t / int16_t
                case 'U': case 'I': // uint32_t / int32_t
                case 'Q': case 'L': // uint64_t / int64_t
                case 'Y':           // char
                    format -= first_int_format;
                    format += is_capital ? -(format & 1) | 1 : 0;
                    printf(format_strings[format].data(), itop);
            }
            itop = stack.pop();
            return dispatch(ip, stack, itop, ftop);
        },
        [FPRINT] = [](InstrPtr ip, Stack stack, UI itop, FL ftop) static constexpr preserve_none {
            uint32_t format = ip->arg;
            bool is_capital = !btr(format, 5);
            switch (format) {
                case 'F': case 'G': case 'E':
                    format -= first_float_format;
                    format += is_capital * 8;
                    printf(format_strings[format].data(), ftop);
            }
            ftop = stack.pop<FL>();
            return dispatch(ip, stack, itop, ftop);
        }
    };
};

// State management

template<bool is_constexpr>
preserve_none constexpr uintptr_t Dispatch<is_constexpr>::dispatch_direct(InstrPtr ip, Stack stack, UI itop, FL ftop) {
    if constexpr (!is_constexpr) {
        return ip->next(ip, stack, itop, ftop);
    }
    else {
        return dispatch_table[ip->opcode](ip, stack, itop, ftop);
    }
}

template<bool is_constexpr>
preserve_none constexpr uintptr_t Dispatch<is_constexpr>::dispatch(InstrPtr ip, Stack stack, UI itop, FL ftop) {
    return dispatch_direct(++ip, stack, itop, ftop);
}

// Invoke

constexpr uintptr_t start_dispatching_with_values(Instr* ip, UI initial_int, FL initial_float, size_t stack_size = 1024) {
    
    uintptr_t ret;
    if consteval {
        // Only new/delete are constexpr
        uintptr_t* stack_buffer = new uintptr_t[stack_size]{};

        Stack stack{ stack_buffer };

        // If this is a constexpr call, run the version
        // that uses "index from initial instruction" calls
        // since that's a valid constant expression.
        ret = Dispatch<true>::dispatch_direct(ip, stack, initial_int, initial_float);

        delete[] stack_buffer;
    }
    else {
        uintptr_t* stack_buffer = (uintptr_t*)malloc(sizeof(uintptr_t) * stack_size);

        Stack stack{ stack_buffer };

        Opcode op;
        size_t i = 0;
        do {
            ip[i++].next = Dispatch<false>::dispatch_table[op = ip[i].opcode];
        } while (op != DUMMY);

        // If this isn't a constexpr call, run the version
        // that directly pushes pointers on the runtime stack.
        ret = Dispatch<false>::dispatch_direct(ip, stack, initial_int, initial_float);

        // Avoid array operator delete
        // since some compilers use extra
        // arguments when calling it
        free(stack_buffer);
    }
    return ret;
}

constexpr uintptr_t start_dispatching(Instr* ip, size_t stack_size = 1024) {
    return start_dispatching_with_values(ip, 0, 0.0f, stack_size);
}


int test() {
    static Instr instructions[] = {
        {
            .opcode = PUSHL,
            .arg = 2
        },
        {
            .opcode = CALL,
            .arg = 3
        },
        {
            .opcode = ADD
        },
        {
            .opcode = ENDT
        },
        {
            .opcode = RETL,
            .arg = 3
        },
        {}
    };

    return start_dispatching(instructions);
}

constexpr int test2() {
    Instr instructions[] = {
        {
            .opcode = PUSHL,
            .arg = 1
        },
        {
            .opcode = FPUSHL,
            .argf = 2.0f
        },
        {
            .opcode = CALL,
            .arg = 8,
        },
        {
            .opcode = FADD,
        },
        {
            .opcode = F2I
        },
        {
            .opcode = ADD
        },
        {
            .opcode = NOP
        },
        {
            .opcode = ENDT,
            .argf = 0.0f
        },
        {
            .opcode = FNEQ
        },
        {
            .opcode = ENDT
        },
        {
            .opcode = FRETL,
            .argf = 3.0f
        },
        {
            .opcode = FRETT
        },
        {}
    };

    return start_dispatching(instructions);
}

static_assert(test2() == 6);

int main() {
    printf("%u", test());
    return 0;
}