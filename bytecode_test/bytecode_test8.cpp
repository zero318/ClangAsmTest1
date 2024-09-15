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

template <typename... Types>
struct largest_type {
private:
    static constexpr std::array sizes = {sizeof(Types)...};
    static constexpr auto index = std::max_element(sizes.begin(), sizes.end()) - sizes.begin();
public:
    using type = std::tuple_element_t<index, std::tuple<Types...>>;
};

template <typename... Types>
using largest_type_t = largest_type<Types...>::type;

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

// Opcodes

enum HydraOpcode : uint32_t {
    DUMMY, // Null terminator style script end
    NOP,
    ENDL, // End parsing, return literal value
    ENDT, // End parsing, return stack top
    PUSHL, // Push literal
    PUSHV, // Push value from stack
    //PUSHR, // Push reference to stack value
    //PUSHI, // Push value from stack (indirect)
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

//using SI = intptr_t;
//using UI = uintptr_t;
//using FL = float;

//using Stack = StackImpl<UI, FL>;



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

template <typename I, typename F>
struct HydraVM {
    using SI = std::make_signed_t<I>;
    using UI = std::make_unsigned_t<I>;
    using FL = F;

    using ST = UIForSize<largest_type_t<SI, UI, FL, void*>>;

    using PI = UIForSize<largest_type_t<SI, UI, void*>>;

    struct HydraInstruction {
        HydraOpcode opcode;
        union {
            UI arg;
            FL argf;
            const char* str;
        };
    };

    template<bool is_constexpr = false>
    struct Impl {

    private:
        struct StackA {
            ST* sp;

            constexpr ST& operator[](intptr_t index) const {
                return this->sp[index];
            }
        };
        struct StackB {
            ST *const stack;
            UI sp;

            constexpr ST& operator[](intptr_t index) const {
                return this->stack[this->sp + index];
            }
        };

        template<typename B>
        struct StackBase : B {

        private:
            template<typename T>
            static consteval inline size_t size_in_underlying_multiple() {
                size_t i = 0;
                while (sizeof(ST) * ++i < sizeof(T));
                return i;
            }
        public:

            template<typename T = PI>
            constexpr inline T read(size_t index = 0) const {
                return bit_cast_to_size<T>((*this)[-index]);
            }
            template<typename T = PI>
            constexpr inline void write(T value, size_t index = 0) {
                if consteval {
                    // This is all just a constexpr way of
                    // saying "only write to the low bytes
                    // if the type being written is small"
                    ST temp = this->read(index);
                    auto cast_value = bit_cast_from_size<T>(value);
                    temp &= ~((ST)~((decltype(cast_value))0));
                    temp |= cast_value;
                    (*this)[index] = temp;
                } else {
                    // GCC actually emits code for the
                    // complicated version rather than
                    // understanding what it's doing like
                    // clang does, so keep the simple
                    // version around
                    (*this)[index] = value;
                }
            }

            template<typename T = PI>
            constexpr inline void push(T value) {
                this->sp += size_in_underlying_multiple<T>();
                this->write(value);
            }
            template<typename T = PI>
            constexpr inline T pop() {
                T temp = bit_cast_to_size<T>((*this)[0]);
                this->sp -= size_in_underlying_multiple<T>();
                return temp;
            }
            template<typename R = PI, typename T = PI>
            constexpr inline R exchange(T value, size_t index = 0) {
                R temp = this->read<R>(index);
                this->write<T>(value, index);
                return temp;
            }
            template<typename T = PI>
            constexpr inline void swap(T& value, size_t index = 0) {
                value = this->exchange<T>(value, index);
            }

            template<typename L>
            constexpr inline void unary_op(PI& itop, FL& ftop, const L& func) {
                using A = typename func_traits<L>::template nth_arg_type<0>;
                using R = func_traits<L>::ret_type;
                if constexpr (std::is_same_v<R, FL>) {
                    if constexpr (std::is_same_v<A, FL>) {
                        ftop = func(ftop);
                    } else {
                        PI value = itop;
                        itop = this->exchange(ftop);
                        ftop = func((A)value);
                    }
                } else {
                    if constexpr (std::is_same_v<A, FL>) {
                        F value = ftop;
                        ftop = this->exchange(itop);
                        itop = func(value);
                    } else {
                        itop = func((A)itop);
                    }
                }
            }
            template<typename L>
            constexpr inline void binary_op(PI& itop, FL& ftop, const L& func) {
                using T = typename func_traits<L>::template nth_arg_type<1>;
                using V = typename func_traits<L>::template nth_arg_type<0>;
                using R = func_traits<L>::ret_type;
                if constexpr (std::is_same_v<R, FL>) {
                    if constexpr (std::is_same_v<T, FL>) {
                        if constexpr (std::is_same_v<V, FL>) {
                            // F = func(F, F)
                            ftop = func(this->pop<V>(), ftop);
                        } else {
                            // F = func(I, F)
                            ftop = func((V)itop, ftop);
                            itop = this->pop();
                        }
                    } else {
                        if constexpr (std::is_same_v<V, FL>) {
                            // F = func(F, I)
                            ftop = func(ftop, (T)itop);
                            itop = this->pop();
                        } else {
                            // F = func(I, I)
                            V lhs = this->pop<V>();
                            PI rhs = itop;
                            itop = this->exchange(ftop);
                            ftop = func(lhs, (T)rhs);
                        }
                    }
                } else {
                    if constexpr (std::is_same_v<T, FL>) {
                        if constexpr (std::is_same_v<V, FL>) {
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
                        if constexpr (std::is_same_v<V, FL>) {
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

    public:

        using Stack = std::conditional_t<is_constexpr, StackBase<StackB>, StackBase<StackA>>;

        struct HydraInstructionRuntime {

            preserve_none typedef UI instr_impl(const HydraInstructionRuntime* ip, Stack stack, PI itop, FL ftop);

            union {
                HydraOpcode opcode;
                instr_impl* next;
            };
            union {
                UI arg;
                FL argf;
                const char* str;
            };
        };

    private: 
        using InstrA = const HydraInstructionRuntime*;

        struct InstrB {
            const HydraInstruction *const instrs;
            mutable PI index;

            constexpr InstrB(const HydraInstruction* instrs) : instrs(instrs), index(0) {}
            constexpr InstrB(const HydraInstruction* instrs, PI index) : instrs(instrs), index(index) {}
            constexpr InstrB& operator=(size_t index) {
                this->index = index;
                return *this;
            }
            constexpr explicit operator PI() const {
                return this->index;
            }
            constexpr const HydraInstruction* operator->() const {
                return &this->instrs[this->index];
            }
            constexpr InstrB operator+(PI value) const {
                return { instrs, index + value };
            }
            constexpr InstrB& operator++() {
                ++this->index;
                return *this;
            }
        };

    public:

        using InstrPtr = std::conditional_t<is_constexpr, InstrB, InstrA>;
        using StackIP = std::conditional_t<is_constexpr, PI, InstrPtr>;

        preserve_none typedef UI dispatch_instr_impl (InstrPtr ip, Stack stack, PI itop, FL ftop);

        preserve_none static constexpr UI dispatch_direct(InstrPtr ip, Stack stack, PI itop, FL ftop);
        preserve_none static constexpr UI dispatch(InstrPtr ip, Stack stack, PI itop, FL ftop);

        alignas(64) static constexpr dispatch_instr_impl *const dispatch_table[MAX_OPCODE] = {
            [DUMMY] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                return dispatch_direct(ip, stack, itop, ftop);
            },
            [NOP] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                return dispatch(ip, stack, itop, ftop);
            },
            [ENDL] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                return ip->arg;
            },
            [ENDT] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                return (UI)itop;
            },
            [PUSHL] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.push(itop);
                itop = ip->arg;
                return dispatch(ip, stack, itop, ftop);
            },
            [PUSHV] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.push(itop);
                itop = stack.read(ip->arg);
                return dispatch(ip, stack, itop, ftop);
            },
            [POP] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                for (size_t i = 0; i < ip->arg; ++i) itop = stack.pop();
                return dispatch(ip, stack, itop, ftop);
            },
            [XCHG] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                // exchanging the stack top with itself would be
                // meaningless, so the index can just be offset by 1
                if (uintptr_t index = ip->arg) {
                    stack.swap(itop, index - 1);
                }
                return dispatch(ip, stack, itop, ftop);
            },
            [ADD] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                    return lhs + rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [SUB] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                    return lhs - rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [MUL] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                    return lhs * rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [DIV] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                    [[assume(rhs != 0)]];
                    return lhs / rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [IDIV] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](SI lhs, SI rhs) {
                    [[assume(rhs != 0)]];
                    return lhs / rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [MOD] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                    [[assume(rhs != 0)]];
                    return lhs % rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [IMOD] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](SI lhs, SI rhs) {
                    [[assume(rhs != 0)]];
                    return lhs % rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [AND] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                    return lhs & rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [OR] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                    return lhs | rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [XOR] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                    return lhs ^ rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [SHL] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                    return lhs << rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [SHR] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
                    return lhs >> rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [SAR] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](SI lhs, SI rhs) {
                    return lhs >> rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [NEG] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.unary_op(itop, ftop, [](SI value) {
                    return -value;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [NOT] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.unary_op(itop, ftop, [](UI value) {
                    return ~value;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [EQU] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                    return lhs == rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [NEQ] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                    return lhs != rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [LES] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
                    return lhs < rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [LEQ] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
                    return lhs <= rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [GTR] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
                    return lhs > rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [GEQ] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
                    return lhs >= rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [BEL] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                    return lhs < rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [BEQ] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                    return lhs <= rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [ABV] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                    return lhs > rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [AEQ] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
                    return lhs >= rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [JMP] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                return dispatch_direct(ip + ip->arg, stack, itop, ftop);
            },
            [JZ] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                intptr_t offset = stack.pop() ? 1 : ip->arg;
                return dispatch_direct(ip + offset, stack, itop, ftop);
            },
            [JNZ] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                intptr_t offset = !stack.pop() ? 1 : ip->arg;
                return dispatch_direct(ip + offset, stack, itop, ftop);
            },
            [CALL] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                // Stack state after call:
                // Old itop is pushed on top of the stack
                // Return address is in itop
                // ftop is unchanged
                stack.push(itop);
                itop = (PI)(ip + 1);
                return dispatch_direct(ip + ip->arg, stack, itop, ftop);
            },
            [RET] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                // No data is getting returned, so just pop
                // the pre-call itop value back into itop.
                ip = std::bit_cast<StackIP>(itop);
                itop = stack.pop();
                return dispatch_direct(ip, stack, itop, ftop);
            },
            [RETL] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                // When returning an int literal there won't be
                // any data from the called sub left on the stack
                // and the new value can be written directly to itop.
                StackIP new_ip = std::bit_cast<StackIP>(itop);
                itop = ip->arg;
                ip = new_ip;
                return dispatch_direct(ip, stack, itop, ftop);
            },
            [RETT] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                // If any ints have been pushed during a sub,
                // the return address will have been pushed onto
                // the stack.
                //
                // Since the value getting returned is just going to
                // stay in the itop register, the return address can
                // be popped directly into ip.
                ip = stack.template pop<StackIP>();
                return dispatch_direct(ip, stack, itop, ftop);
            },
            [FRETL] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
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
            [FRETT] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
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
                itop = stack.exchange(stack.template pop<FL>());
                return dispatch_direct(ip, stack, itop, ftop);
            },
            [OPSIZE] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.push(itop);
                itop = sizeof(ST);
                return dispatch(ip, stack, itop, ftop);
            },
            [INSSIZE] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.push(itop);
                itop = sizeof(HydraInstruction);
                return dispatch(ip, stack, itop, ftop);
            },
            [I2F] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.unary_op(itop, ftop, [](SI value) {
                    return (FL)value;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [U2F] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.unary_op(itop, ftop, [](UI value) {
                    return (FL)value;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [F2I] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.unary_op(itop, ftop, [](FL value) {
                    return (SI)value;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [F2U] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.unary_op(itop, ftop, [](FL value) {
                    return (UI)value;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FPUSHL] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.push(ftop);
                ftop = ip->argf;
                return dispatch(ip, stack, itop, ftop);
            },
            [FPUSHV] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.push(ftop);
                ftop = stack.template read<FL>(ip->arg);
                return dispatch(ip, stack, itop, ftop);
            },
            [FPOP] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                for (size_t i = 0; i < ip->arg; ++i) ftop = stack.template pop<FL>();
                return dispatch(ip, stack, itop, ftop);
            },
            [FXCHG] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                // exchanging the stack top with itself would be
                // meaningless, so the index can just be offset by 1
                if (uintptr_t index = ip->arg) {
                    stack.swap(ftop, index - 1);
                }
                return dispatch(ip, stack, itop, ftop);
            },
            [FADD] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
                    return lhs + rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FSUB] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
                    return lhs - rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FMUL] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
                    return lhs * rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FDIV] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
                    return lhs / rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FNEG] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.unary_op(itop, ftop, [](FL value) {
                    return -value;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FEQU] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                    return lhs == rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FNEQ] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                    return lhs != rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FLES] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                    return lhs < rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FLEQ] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                    return lhs <= rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FGTR] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                    return lhs > rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FGEQ] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
                    return lhs >= rhs;
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FSIN] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.unary_op(itop, ftop, [](FL value) {
                    return sinf(value);
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FCOS] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.unary_op(itop, ftop, [](FL value) {
                    return cosf(value);
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FATAN2] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
                    return atan2f(rhs, lhs);
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [FSQRT] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                stack.unary_op(itop, ftop, [](FL value) {
                    [[assume(value >= 0.0)]];
                    return sqrtf(value);
                });
                return dispatch(ip, stack, itop, ftop);
            },
            [PRINT] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
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
            [FPRINT] = [](InstrPtr ip, Stack stack, PI itop, FL ftop) static constexpr preserve_none {
                uint32_t format = ip->arg;
                bool is_capital = !btr(format, 5);
                switch (format) {
                    case 'F': case 'G': case 'E':
                        format -= first_float_format;
                        format += is_capital * 8;
                        printf(format_strings[format].data(), ftop);
                }
                ftop = stack.template pop<FL>();
                return dispatch(ip, stack, itop, ftop);
            }
        };
    };

    // Invoke

    static constexpr UI start_dispatching_with_values(HydraInstruction* ip_arg, UI initial_int, FL initial_float, size_t stack_size = 1024) {
        UI ret;
        if consteval {
            // Only new/delete are constexpr
            ST* stack_buffer = new ST[stack_size]{};

            typename Impl<true>::Stack stack{ stack_buffer };

            // If this is a constexpr call, run the version
            // that uses "index from initial instruction" calls
            // since that's a valid constant expression.
            ret = Impl<true>::dispatch_direct(ip_arg, stack, initial_int, initial_float);

            delete[] stack_buffer;
        }
        else {
            ST* stack_buffer = (ST*)malloc(sizeof(ST) * stack_size);

            typename Impl<false>::Stack stack{ stack_buffer };

            typename Impl<false>::HydraInstructionRuntime* ip = (typename Impl<false>::HydraInstructionRuntime*)ip_arg;

            HydraOpcode op;
            size_t i = 0;
            do {
                ip[i++].next = Impl<false>::dispatch_table[op = ip[i].opcode];
            } while (op != DUMMY);

            // If this isn't a constexpr call, run the version
            // that directly pushes pointers on the runtime stack.
            ret = Impl<false>::dispatch_direct(ip, stack, initial_int, initial_float);

            // Avoid array operator delete
            // since some compilers use extra
            // arguments when calling it
            free(stack_buffer);
        }
        return ret;
    }

    static constexpr UI start_dispatching(HydraInstruction* ip, size_t stack_size = 1024) {
        return start_dispatching_with_values(ip, 0, 0.0f, stack_size);
    }
};

// State management

template<typename I, typename F>
template<bool is_constexpr>
preserve_none constexpr HydraVM<I, F>::UI HydraVM<I, F>::Impl<is_constexpr>::dispatch_direct(
    HydraVM<I, F>::Impl<is_constexpr>::InstrPtr ip,
    HydraVM<I, F>::Impl<is_constexpr>::Stack stack,
    HydraVM<I, F>::PI itop, HydraVM<I, F>::FL ftop
) {
    if constexpr (!is_constexpr) {
        return ip->next(ip, stack, itop, ftop);
    }
    else {
        return dispatch_table[ip->opcode](ip, stack, itop, ftop);
    }
}

template<typename I, typename F>
template<bool is_constexpr>
preserve_none constexpr HydraVM<I, F>::UI HydraVM<I, F>::Impl<is_constexpr>::dispatch(
    HydraVM<I, F>::Impl<is_constexpr>::InstrPtr ip,
    HydraVM<I, F>::Impl<is_constexpr>::Stack stack,
    HydraVM<I, F>::PI itop, HydraVM<I, F>::FL ftop
) {
    return dispatch_direct(++ip, stack, itop, ftop);
}

#define HydraVM HydraVM<uintptr_t, float>
#define HydraInstruction HydraVM::HydraInstruction

int test() {
    static HydraInstruction instructions[] = {
        {
            .opcode = PUSHL,
            .arg = 2
        },
        {
            .opcode = CALL,
            .arg = 5
        },
        {
            .opcode = ADD
        },
        {
            .opcode = PUSHV,
            .arg = 0
        },
        {
            .opcode = PRINT,
            .arg = 'i'
        },
        {
            .opcode = ENDT
        },
        {
            .opcode = RETL,
            .arg = 4
        },
        {}
    };

    return HydraVM::start_dispatching(instructions);
}

constexpr int test2() {
    HydraInstruction instructions[] = {
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
            .opcode = FPUSHL,
            .argf = 3.0f
        },
        {
            .opcode = FRETT
        },
        {}
    };

    return HydraVM::start_dispatching(instructions);
}

static_assert(test2() == 6);

int main() {
    //printf("%u", test());
    test();
    return 0;
}