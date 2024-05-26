#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <tuple>
#include <type_traits>
#include <utility>
#include <bit>

// Utility templates

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

    constexpr inline void inc() {
        ++this->sp;
    }
    constexpr inline void dec() {
        --this->sp;
    }

    template<typename T = U>
    constexpr inline void push(T value) {
        *(T*)(this->sp += size_in_underlying_multiple<T>()) = value;
    }
    template<typename T = U>
    constexpr inline T pop() {
        T* temp = (T*)this->sp;
        this->sp -= size_in_underlying_multiple<T>();
        return *temp;
    }
    template<typename T = U>
    constexpr inline T& ref(size_t index) const {
        return *(T*)(this->sp - index);
    }
    template<typename T = U>
    constexpr inline T& top() const {
        return this->ref<T>(0);
    }

    template<typename L>
    constexpr inline void unary_op(U& itop, F& ftop, const L& func) {
        using A = typename func_traits<L>::template nth_arg_type<0>;
        using R = func_traits<L>::ret_type;
        if constexpr (std::is_same_v<R, F>) {
            if constexpr (std::is_same_v<A, F>) {
                ftop = func(ftop);
            } else {
                ftop = func((A)itop);
            }
        } else {
            if constexpr (std::is_same_v<A, F>) {
                itop = func(ftop);
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
        //T rhs = this->pop<T>();
        //V lhs = this->top<V>();
        //this->top<P>() = func(lhs, rhs);
        V lhs = this->pop<V>();
        if constexpr (std::is_same_v<R, F>) {
            if constexpr (std::is_same_v<T, F>) {
                if constexpr (!std::is_same_v<V, F>) static_assert(false);
                ftop = func(lhs, ftop);
            } else {
                if constexpr (std::is_same_v<V, F>) static_assert(false);
                U rhs = itop;
                itop = this->top();
                this->top<F>() = ftop;
                ftop = func(lhs, (T)itop);
            }
        } else {
            if constexpr (std::is_same_v<T, F>) {
                if constexpr (!std::is_same_v<V, F>) static_assert(false);
                F rhs = ftop;
                ftop = this->top<F>();
                this->top() = itop;
                itop = func(lhs, rhs);
            } else {
                if constexpr (std::is_same_v<V, F>) static_assert(false);
                itop = func(lhs, (T)itop);
            }
        }
    }
};

// Opcodes

enum Opcode : uint32_t {
    DUMMY,
    ENDL, // End parsing, return literal value
    ENDV, // End parsing, return stack top
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
    //FRETL,
    RETV, // Return and push stack top
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
    FSQRT
};

// Instruction format

using SI = intptr_t;
using UI = uintptr_t;
using FL = float;

using Stack = StackImpl<UI, FL>;

struct Instr;

typedef UI instr_impl(const Instr* ip, Stack stack, UI itop, FL ftop);

struct Instr {
    union {
        Opcode opcode;
        instr_impl* next;
    };
    uintptr_t arg;
};

// Instruction implementations

constexpr UI start_dispatching(const Instr* ip);
static constexpr UI dispatch_direct(const Instr* ip, Stack stack, UI itop, FL ftop);
static constexpr UI dispatch(const Instr* ip, Stack stack, UI itop, FL ftop);

alignas(64) static constexpr instr_impl *const dispatch_table[] = {
    [DUMMY] = NULL,
    [ENDL] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        return ip->arg;
    },
    [ENDV] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        return itop;
    },
    [PUSHL] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.push(itop);
        itop = ip->arg;
        return dispatch(ip, stack, itop, ftop);
    },
    [PUSHV] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.push(itop);
        itop = stack.ref(ip->arg);
        return dispatch(ip, stack, itop, ftop);
    },
    [POP] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        for (size_t i = 0; i < ip->arg; ++i) itop = stack.pop();
        return dispatch(ip, stack, itop, ftop);
    },
    [XCHG] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        if (uintptr_t index = ip->arg) {
            std::swap(itop, stack.ref(index - 1));
        }
        return dispatch(ip, stack, itop, ftop);
    },
    [ADD] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
            return lhs + rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [SUB] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
            return lhs - rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [MUL] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
            return lhs * rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [DIV] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
            [[assume(rhs != 0)]];
            return lhs / rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [IDIV] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](SI lhs, SI rhs) {
            [[assume(rhs != 0)]];
            return lhs / rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [MOD] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
            [[assume(rhs != 0)]];
            return lhs % rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [IMOD] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](SI lhs, SI rhs) {
            [[assume(rhs != 0)]];
            return lhs % rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [AND] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
            return lhs & rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [OR] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
            return lhs | rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [XOR] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
            return lhs ^ rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [SHL] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
            return lhs << rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [SHR] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) {
            return lhs >> rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [SAR] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](SI lhs, SI rhs) {
            return lhs >> rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [NEG] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.unary_op(itop, ftop, [](SI value) {
            return -value;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [NOT] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.unary_op(itop, ftop, [](UI value) {
            return ~value;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [EQU] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
            return lhs == rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [NEQ] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
            return lhs != rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [LES] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
            return lhs < rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [LEQ] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
            return lhs <= rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [GTR] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
            return lhs > rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [GEQ] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](SI lhs, SI rhs) -> UI {
            return lhs >= rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [BEL] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
            return lhs < rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [BEQ] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
            return lhs <= rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [ABV] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
            return lhs > rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [AEQ] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](UI lhs, UI rhs) -> UI {
            return lhs >= rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [JMP] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        return dispatch_direct(ip + ip->arg, stack, itop, ftop);
    },
    [JZ] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        intptr_t offset = stack.pop() ? 1 : ip->arg;
        return dispatch_direct(ip + offset, stack, itop, ftop);
    },
    [JNZ] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        intptr_t offset = !stack.pop() ? 1 : ip->arg;
        return dispatch_direct(ip + offset, stack, itop, ftop);
    },
    [CALL] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        //stack.push(ip + 1);
        stack.push(itop);
        itop = std::bit_cast<UI>(ip + 1);
        return dispatch_direct(ip + ip->arg, stack, itop, ftop);
    },
    [RET] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        //ip = stack.pop<const Instr*>();
        ip = std::bit_cast<const Instr*>(itop);
        itop = stack.pop();
        return dispatch_direct(ip, stack, itop, ftop);
    },
    [RETL] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        //stack.unary_op(itop, ftop, [&](const Instr* new_ip) {
            //UI value = ip->arg;
            //ip = new_ip;
            //return value;
        //});
        //UI new_itop = ip->arg;
        //ip = stack.top<const Instr*>();
        //stack.top() = itop;
        //itop = new_itop;
        ip = std::bit_cast<const Instr*>(itop);
        itop = ip->arg;
        return dispatch_direct(ip, stack, itop, ftop);
    },
    //[FRETL] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        //stack.unary_op(itop, ftop, [&](const Instr* new_ip) {
            //F value = *(F*)&ip->arg;
            //ip = new_ip;
            //return value;
        //});
        //FL new_ftop = *(FL*)&ip->arg;
        //ip = stack.top<const Instr*>();
        //stack.top<FL>() = ftop;
        //ftop = new_ftop;

        //return dispatch_direct(ip, stack, itop, ftop);
    //},
    [RETV] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        //stack.binary_op(itop, ftop, [&](const Instr* new_ip, UI value) {
            //ip = new_ip;
            //return value;
        //});
        ip = stack.pop<const Instr*>();
        return dispatch_direct(ip, stack, itop, ftop);
    },
    [OPSIZE] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.push(itop);
        itop = sizeof(Stack::type);
        return dispatch(ip, stack, itop, ftop);
    },
    [INSSIZE] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.push(itop);
        itop = sizeof(Instr);
        return dispatch(ip, stack, itop, ftop);
    },
    [I2F] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.unary_op(itop, ftop, [](SI value) {
            return (FL)value;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [U2F] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.unary_op(itop, ftop, [](UI value) {
            return (FL)value;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [F2I] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.unary_op(itop, ftop, [](FL value) {
            return (SI)value;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [F2U] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.unary_op(itop, ftop, [](FL value) {
            return (UI)value;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FPUSHL] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.push(ftop);
        ftop = *(FL*)&ip->arg;
        return dispatch(ip, stack, itop, ftop);
    },
    [FPUSHV] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.push(ftop);
        ftop = stack.ref<FL>(ip->arg);
        return dispatch(ip, stack, itop, ftop);
    },
    [FPOP] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        for (size_t i = 0; i < ip->arg; ++i) ftop = stack.pop<FL>();
        return dispatch(ip, stack, itop, ftop);
    },
    [FXCHG] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        if (uintptr_t index = ip->arg) {
            std::swap(ftop, stack.ref<FL>(index - 1));
        }
        return dispatch(ip, stack, itop, ftop);
    },
    [FADD] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
            return lhs + rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FSUB] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
            return lhs - rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FMUL] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
            return lhs * rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FDIV] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
            return lhs / rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FNEG] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.unary_op(itop, ftop, [](FL value) {
            return -value;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FEQU] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
            return lhs == rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FNEQ] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
            return lhs != rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FLES] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
            return lhs < rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FLEQ] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
            return lhs <= rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FGTR] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
            return lhs > rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FGEQ] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) -> UI {
            return lhs >= rhs;
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FSIN] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.unary_op(itop, ftop, [](FL value) {
            return sinf(value);
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FCOS] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.unary_op(itop, ftop, [](FL value) {
            return cosf(value);
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FATAN2] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.binary_op(itop, ftop, [](FL lhs, FL rhs) {
            return atan2f(rhs, lhs);
        });
        return dispatch(ip, stack, itop, ftop);
    },
    [FSQRT] = [](const Instr* ip, Stack stack, UI itop, FL ftop) static constexpr {
        stack.unary_op(itop, ftop, [](FL value) {
            [[assume(value >= 0.0)]];
            return sqrtf(value);
        });
        return dispatch(ip, stack, itop, ftop);
    },
};

// State management

#define PREPARSE 1

static constexpr uintptr_t dispatch_direct(const Instr* ip, Stack stack, UI itop, FL ftop) {
#if PREPARSE
    if !consteval {
        return ip->next(ip, stack, itop, ftop);
    }
#endif
    return dispatch_table[ip->opcode](ip, stack, itop, ftop);
}

static constexpr uintptr_t dispatch(const Instr* ip, Stack stack, UI itop, FL ftop) {
    return dispatch_direct(++ip, stack, itop, ftop);
}

constexpr uintptr_t start_dispatching(Instr* ip, size_t stack_size = 1024) {
    uintptr_t* stack_buffer = new uintptr_t[stack_size];
    
    Stack stack;
    stack.sp = (uintptr_t*)stack_buffer;
#if PREPARSE
    if !consteval {
        Opcode op;
        size_t i = 0;
        do {
            ip[i++].next = dispatch_table[op = ip[i].opcode];
        } while (op != DUMMY);
    }
#endif
    uintptr_t ret = dispatch_direct(ip, stack, 0, 0.0f);

    delete[] stack_buffer;

    return ret;
}


int test() {
    static Instr instructions[] = {
        {
            .opcode = PUSHL,
            .arg = 5
        },
        {
            .opcode = CALL,
            .arg = 3
        },
        {
            .opcode = ADD
        },
        {
            .opcode = ENDV
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
            .arg = 5
        },
        {
            .opcode = ENDV
        },
        {}
    };

    return start_dispatching(instructions);
}

static_assert(test2() == 5);

int main() {
    printf("%u", test());
    return 0;
}