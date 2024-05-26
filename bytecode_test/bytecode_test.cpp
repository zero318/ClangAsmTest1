#include <stdlib.h>
#include <stdint.h>
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

template <typename U>
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

    template<typename F>
    constexpr inline void unary_op(const F& func) const {
        using A = typename func_traits<F>::template nth_arg_type<0>;
        using P = func_traits<F>::ret_type;
        this->top<P>() = func(this->top<A>());
    }
    template<typename F>
    constexpr inline void binary_op(const F& func) {
        using R = typename func_traits<F>::template nth_arg_type<1>;
        using L = typename func_traits<F>::template nth_arg_type<0>;
        using P = func_traits<F>::ret_type;
        R rhs = this->pop<R>();
        L lhs = this->top<L>();
        this->top<P>() = func(lhs, rhs);
    }
};

// Opcodes

enum Opcode : uint32_t {
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
    RETV, // Return and push stack top
    OPSIZE, // Push stack operand size
    INSSIZE, // Push instruction size
    I2F,
    U2F,
    F2I,
    F2U,
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

using Stack = StackImpl<UI>;

struct Instr;

typedef UI instr_impl(const Instr* ip, Stack stack);

struct Instr {
    union {
        Opcode opcode;
        instr_impl* next;
    };
    union {
        uintptr_t arg;
    };
};

// Instruction implementations

constexpr UI start_dispatching(const Instr* ip);
static constexpr UI dispatch_direct(const Instr* ip, Stack stack);
static constexpr UI dispatch(const Instr* ip, Stack stack);

alignas(64) static constexpr instr_impl *const dispatch_table[] = {
    [ENDL] = [](const Instr* ip, Stack stack) static constexpr {
        return ip->arg;
    },
    [ENDV] = [](const Instr* ip, Stack stack) static constexpr {
        return stack.pop();
    },
    [PUSHL] = [](const Instr* ip, Stack stack) static constexpr {
        stack.push(ip->arg);
        return dispatch(ip, stack);
    },
    [PUSHV] = [](const Instr* ip, Stack stack) static constexpr {
        stack.push(stack.ref(ip->arg));
        return dispatch(ip, stack);
    },
    [POP] = [](const Instr* ip, Stack stack) static constexpr {
        for (size_t i = 0; i < ip->arg; ++i) stack.pop();
        return dispatch(ip, stack);
    },
    [XCHG] = [](const Instr* ip, Stack stack) static constexpr {
        std::swap(stack.top(), stack.ref(ip->arg));
        return dispatch(ip, stack);
    },
    [ADD] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) {
            return lhs + rhs;
        });
        return dispatch(ip, stack);
    },
    [SUB] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) {
            return lhs - rhs;
        });
        return dispatch(ip, stack);
    },
    [MUL] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) {
            return lhs * rhs;
        });
        return dispatch(ip, stack);
    },
    [DIV] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) {
            [[assume(rhs != 0)]];
            return lhs / rhs;
        });
        return dispatch(ip, stack);
    },
    [IDIV] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](SI lhs, SI rhs) {
            [[assume(rhs != 0)]];
            return lhs / rhs;
        });
        return dispatch(ip, stack);
    },
    [MOD] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) {
            [[assume(rhs != 0)]];
            return lhs % rhs;
        });
        return dispatch(ip, stack);
    },
    [IMOD] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](SI lhs, SI rhs) {
            [[assume(rhs != 0)]];
            return lhs % rhs;
        });
        return dispatch(ip, stack);
    },
    [AND] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) {
            return lhs & rhs;
        });
        return dispatch(ip, stack);
    },
    [OR] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) {
            return lhs | rhs;
        });
        return dispatch(ip, stack);
    },
    [XOR] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) {
            return lhs ^ rhs;
        });
        return dispatch(ip, stack);
    },
    [SHL] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) {
            return lhs << rhs;
        });
        return dispatch(ip, stack);
    },
    [SHR] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) {
            return lhs >> rhs;
        });
        return dispatch(ip, stack);
    },
    [SAR] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](SI lhs, SI rhs) {
            return lhs >> rhs;
        });
        return dispatch(ip, stack);
    },
    [NEG] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([](SI value) {
            return -value;
        });
        return dispatch(ip, stack);
    },
    [NOT] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([](UI value) {
            return ~value;
        });
        return dispatch(ip, stack);
    },
    [EQU] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) -> UI {
            return lhs == rhs;
        });
        return dispatch(ip, stack);
    },
    [NEQ] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) -> UI {
            return lhs != rhs;
        });
        return dispatch(ip, stack);
    },
    [LES] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](SI lhs, SI rhs) -> UI {
            return lhs < rhs;
        });
        return dispatch(ip, stack);
    },
    [LEQ] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](SI lhs, SI rhs) -> UI {
            return lhs <= rhs;
        });
        return dispatch(ip, stack);
    },
    [GTR] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](SI lhs, SI rhs) -> UI {
            return lhs > rhs;
        });
        return dispatch(ip, stack);
    },
    [GEQ] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](SI lhs, SI rhs) -> UI {
            return lhs >= rhs;
        });
        return dispatch(ip, stack);
    },
    [BEL] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) -> UI {
            return lhs < rhs;
        });
        return dispatch(ip, stack);
    },
    [BEQ] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) -> UI {
            return lhs <= rhs;
        });
        return dispatch(ip, stack);
    },
    [ABV] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) -> UI {
            return lhs > rhs;
        });
        return dispatch(ip, stack);
    },
    [AEQ] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](UI lhs, UI rhs) -> UI {
            return lhs >= rhs;
        });
        return dispatch(ip, stack);
    },
    [JMP] = [](const Instr* ip, Stack stack) static constexpr {
        return dispatch_direct(ip + ip->arg, stack);
    },
    [JZ] = [](const Instr* ip, Stack stack) static constexpr {
        intptr_t offset = stack.pop() ? 1 : ip->arg;
        return dispatch_direct(ip + offset, stack);
    },
    [JNZ] = [](const Instr* ip, Stack stack) static constexpr {
        intptr_t offset = !stack.pop() ? 1 : ip->arg;
        return dispatch_direct(ip + offset, stack);
    },
    [CALL] = [](const Instr* ip, Stack stack) static constexpr {
        stack.push(ip + 1);
        return dispatch_direct(ip + ip->arg, stack);
    },
    [RET] = [](const Instr* ip, Stack stack) static constexpr {
        ip = stack.pop<const Instr*>();
        return dispatch_direct(ip, stack);
    },
    [RETL] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([&](const Instr* new_ip) {
            uintptr_t value = ip->arg;
            ip = new_ip;
            return value;
        });
        return dispatch_direct(ip, stack);
    },
    [RETV] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([&](const Instr* new_ip, UI value) {
            ip = new_ip;
            return value;
        });
        return dispatch_direct(ip, stack);
    },
    [OPSIZE] = [](const Instr* ip, Stack stack) static constexpr {
        stack.push(sizeof(Stack::type));
        return dispatch(ip, stack);
    },
    [INSSIZE] = [](const Instr* ip, Stack stack) static constexpr {
        stack.push(sizeof(Instr));
        return dispatch(ip, stack);
    },
    [I2F] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([](SI value) {
            return (FL)value;
        });
        return dispatch(ip, stack);
    },
    [U2F] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([](UI value) {
            return (FL)value;
        });
        return dispatch(ip, stack);
    },
    [F2I] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([](FL value) {
            return (SI)value;
        });
        return dispatch(ip, stack);
    },
    [F2U] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([](FL value) {
            return (UI)value;
        });
        return dispatch(ip, stack);
    },
    [FADD] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) {
            return lhs + rhs;
        });
        return dispatch(ip, stack);
    },
    [FSUB] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) {
            return lhs - rhs;
        });
        return dispatch(ip, stack);
    },
    [FMUL] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) {
            return lhs * rhs;
        });
        return dispatch(ip, stack);
    },
    [FDIV] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) {
            return lhs / rhs;
        });
        return dispatch(ip, stack);
    },
    [FNEG] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([](FL value) {
            return -value;
        });
        return dispatch(ip, stack);
    },
    [FEQU] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) -> UI {
            return lhs == rhs;
        });
        return dispatch(ip, stack);
    },
    [FNEQ] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) -> UI {
            return lhs != rhs;
        });
        return dispatch(ip, stack);
    },
    [FLES] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) -> UI {
            return lhs < rhs;
        });
        return dispatch(ip, stack);
    },
    [FLEQ] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) -> UI {
            return lhs <= rhs;
        });
        return dispatch(ip, stack);
    },
    [FGTR] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) -> UI {
            return lhs > rhs;
        });
        return dispatch(ip, stack);
    },
    [FGEQ] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) -> UI {
            return lhs >= rhs;
        });
        return dispatch(ip, stack);
    },
    [FSIN] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([](FL value) {
            return sinf(value);
        });
        return dispatch(ip, stack);
    },
    [FCOS] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([](FL value) {
            return cosf(value);
        });
        return dispatch(ip, stack);
    },
    [FATAN2] = [](const Instr* ip, Stack stack) static constexpr {
        stack.binary_op([](FL lhs, FL rhs) {
            return atan2f(rhs, lhs);
        });
        return dispatch(ip, stack);
    },
    [FSQRT] = [](const Instr* ip, Stack stack) static constexpr {
        stack.unary_op([](FL value) {
            [[assume(value >= 0.0)]];
            return sqrtf(value);
        });
        return dispatch(ip, stack);
    },
};

// State management

#define PREPARSE 1

static constexpr uintptr_t dispatch_direct(const Instr* ip, Stack stack) {
#if PREPARSE
    if !consteval {
        return ip->next(ip, stack);
    }
#endif
    return dispatch_table[ip->opcode](ip, stack);
}

static constexpr uintptr_t dispatch(const Instr* ip, Stack stack) {
    return dispatch_direct(++ip, stack);
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
        } while (op != ENDL && op != ENDV);
    }
#endif
    uintptr_t ret = dispatch_direct(ip, stack);

    delete[] stack_buffer;

    return ret;
}


int test() {
    Instr instructions[] = {
        {
            .opcode = PUSHL,
            .arg = 1
        },
        {
            .opcode = PUSHL,
            .arg = 1
        },
        {
            .opcode = ADD,
        },
        {
            .opcode = ENDV
        }
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
        }
    };

    return start_dispatching(instructions);
}

static_assert(test2() == 5);