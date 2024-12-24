#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <type_traits>
#include <utility>
#include <tuple>
#include <limits>

#if __INTELLISENSE__ && !_HAS_CXX20
#define TEMP_DEF_CXX20 1
#undef _HAS_CXX20
#define _HAS_CXX20 1
#endif
#include <bit>
#if __INTELLISENSE__ && TEMP_DEF_CXX20
#undef _HAS_CXX20
#define _HAS_CXX20 0
#endif

#include "8086_cpu.h"

#include "zero/util.h"

#define DS DS_SEG

#define USE_BITFIELDS 1

union GPR {
    uint16_t word;
    struct {
        uint8_t byte;
        uint8_t hbyte;
    };
};

enum REG_INDEX : uint8_t {
    AX = 0, AL = 0, ES = 0,
    CX = 1, CL = 1, CS = 1,
    DX = 2, DL = 2, SS = 2,
    BX = 3, BL = 3, DS = 3,
    SP = 4, AH = 4,
    BP = 5, CH = 5,
    SI = 6, DH = 6,
    DI = 7, BH = 7,
};

enum REP_STATE : int8_t {
    NO_REP = -1,
    REP_NE = 0,
    REP_E = 1
};

RAM mem;

struct x86Addr {
    union {
        uint32_t raw;
        struct {
            uint16_t offset;
            uint16_t segment;
        };
    };

    inline constexpr x86Addr() : raw(0) {};
    inline constexpr x86Addr(uint32_t raw) : raw(raw) {}
    inline constexpr x86Addr(uint16_t segment, uint16_t offset) : offset(offset), segment(segment) {}
    inline constexpr x86Addr(const x86Addr&) = default;


    inline constexpr size_t real_addr(size_t offset = 0) const {
        return ((size_t)this->segment << 4) + (uint16_t)(this->offset + offset);
    }

    inline constexpr operator size_t() const {
        return this->real_addr();
    }

    inline constexpr x86Addr& operator+=(intptr_t offset) {
        this->offset += offset;
        return *this;
    }

    inline constexpr x86Addr& operator-=(intptr_t offset) {
        this->offset -= offset;
        return *this;
    }

    inline constexpr x86Addr& operator++() {
        ++this->offset;
        return *this;
    }

    inline constexpr x86Addr operator++(int) {
        return x86Addr(this->segment, this->offset++);
    }

    inline constexpr x86Addr& operator--() {
        --this->offset;
        return *this;
    }

    inline constexpr x86Addr operator--(int) {
        return x86Addr(this->segment, this->offset--);
    }

    inline constexpr uint32_t operator+(intptr_t offset) const {
        x86Addr temp = this->raw;
        temp.offset += offset;
        return temp.raw;
    }

    inline constexpr uint32_t operator-(intptr_t offset) const {
        x86Addr temp = this->raw;
        temp.offset -= offset;
        return temp.raw;
    }

    template <typename T = uint8_t>
    inline void write(const T& value, intptr_t offset = 0) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return mem.write<T>(this->real_addr(offset), value);
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            if (!(this->offset + offset & 1)) {
                return mem.write<T>(this->real_addr(offset), value);
            }
            else {
                uint16_t raw = *(uint16_t*)&value;
                mem.write<uint8_t>(this->real_addr(offset), raw);
                mem.write<uint8_t>(this->real_addr(offset + 1), raw >> 8);
            }
        }
    }

    template <typename T = uint8_t>
    inline T read(intptr_t offset = 0) const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return mem.read<T>(this->real_addr(offset));
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            if (!(this->offset + offset & 1)) {
                return mem.read<T>(this->real_addr(offset));
            }
            else {
                union {
                    T ret;
                    uint16_t raw;
                };
                raw = mem.read<uint8_t>(this->real_addr(offset));
                raw |= mem.read<uint8_t>(this->real_addr(offset + 1)) << 8;
                return ret;
            }
        }
    }

    template <typename T = uint8_t>
    inline void write_advance(const T& value, intptr_t index = sizeof(T)) {
        this->write(value);
        this->offset += index;
    }

    template <typename T = uint8_t>
    inline T read_advance(intptr_t index = sizeof(T)) {
        T ret = this->read<T>();
        this->offset += index;
        return ret;
    }
};

struct x86Context {

    // Normal state
    union {
        GPR gpr[7];
        struct {
            union {
                uint16_t ax;
                struct {
                    uint8_t al;
                    uint8_t ah;
                };
            };
            union {
                uint16_t cx;
                struct {
                    uint8_t cl;
                    uint8_t ch;
                };
            };
            union {
                uint16_t dx;
                struct {
                    uint8_t dl;
                    uint8_t dh;
                };
            };
            union {
                uint16_t bx;
                struct {
                    uint8_t bl;
                    uint8_t bh;
                };
            };
            uint16_t sp;
            uint16_t bp;
            uint16_t si;
            uint16_t di;
        };
    };
    union {
        uint16_t seg[7];
        struct {
            uint16_t es;
            uint16_t cs;
            uint16_t ss;
            uint16_t ds;
        };
    };
    uint16_t ip;

    // Flags
    bool carry;
    bool parity;
    bool auxiliary;
    bool zero;
    bool sign;
    bool trap;
    bool interrupt;
    bool direction;
    bool overflow;

    inline constexpr bool cond_O() const { return this->overflow; }
    inline constexpr bool cond_NO() const { return !this->overflow; }
    inline constexpr bool cond_C() const { return this->carry; }
    inline constexpr bool cond_B() const { return this->cond_C(); }
    inline constexpr bool cond_NAE() const { return this->cond_C(); }
    inline constexpr bool cond_NC() const { return !this->carry; }
    inline constexpr bool cond_NB() const { return this->cond_NC(); }
    inline constexpr bool cond_AE() const { return this->cond_NC(); }
    inline constexpr bool cond_Z() const { return this->zero; }
    inline constexpr bool cond_E() const { return this->cond_Z(); }
    inline constexpr bool cond_NZ() const { return !this->zero; }
    inline constexpr bool cond_NE() const { return this->cond_NZ(); }
    inline constexpr bool cond_BE() const { return this->carry || this->zero; }
    inline constexpr bool cond_NA() const { return this->cond_BE(); }
    inline constexpr bool cond_A() const { return !this->carry && !this->zero; }
    inline constexpr bool cond_NBE() const { return this->cond_A(); }
    inline constexpr bool cond_S() const { return this->sign; }
    inline constexpr bool cond_NS() const { return !this->sign; }
    inline constexpr bool cond_P() const { return this->parity; }
    inline constexpr bool cond_PE() const { return this->cond_P(); }
    inline constexpr bool cond_NP() const { return !this->parity; }
    inline constexpr bool cond_PO() const { return this->cond_NP(); }
    inline constexpr bool cond_L() const { return this->sign != this->overflow; }
    inline constexpr bool cond_NGE() const { return this->cond_L(); }
    inline constexpr bool cond_GE() const { return this->sign == this->overflow; }
    inline constexpr bool cond_NL() const { return this->cond_GE(); }
    inline constexpr bool cond_LE() const { return this->zero || this->sign != this->overflow; }
    inline constexpr bool cond_NG() const { return this->cond_LE(); }
    inline constexpr bool cond_G() const { return !this->zero && this->sign == this->overflow; }
    inline constexpr bool cond_NLE() const { return this->cond_G(); }


    // Internal state
    int8_t seg_override;
    int8_t rep_type;
    bool lock;
    size_t clock;

    inline constexpr void set_seg_override(uint8_t seg) {
        this->seg_override = seg & 3;
    }

    inline constexpr uint16_t segment(uint8_t default_seg) const {
        return this->seg[this->seg_override < 0 ? default_seg : this->seg_override];
    }

    inline constexpr uint32_t addr(uint8_t default_seg, uint16_t offset) const {
        return (uint32_t)this->segment(default_seg) << 16 | offset;
    }

    inline constexpr uint32_t addr_force(uint8_t seg, uint16_t offset) const {
        return (uint32_t)this->seg[seg] << 16 | offset;
    }

    inline constexpr uint32_t pc() const {
        return (uint32_t)this->cs << 16 | this->ip;
    }

    inline constexpr uint32_t stack() const {
        return (uint32_t)this->ss << 16 | this->sp;
    }

    inline constexpr uint32_t str_src() const {
        return this->addr(DS, this->si);
    }

    inline constexpr uint32_t str_dst() const {
        return this->addr_force(ES, this->di);
    }

    inline constexpr uint8_t& index_byte_reg(uint8_t index) {
        return *(&this->gpr[index & 3].byte + (index > 3));
    }

    inline constexpr uint16_t& index_word_reg(uint8_t index) {
        return this->gpr[index].word;
    }

    template <typename T>
    inline constexpr auto& index_reg(uint8_t index) {
        if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->index_word_reg(index);
        } else {
            return this->index_byte_reg(index);
        }
    }

    inline constexpr uint16_t& index_seg(uint8_t index) {
        return this->seg[index & 3];
    }

    // Instruction implementations
    template <typename T>
    inline T get_flags() const {
        uint16_t base = 0b111100000000010;
        base |= this->carry;
        base |= (uint32_t)this->parity << 2;
        base |= (uint32_t)this->auxiliary << 4;
        base |= (uint32_t)this->zero << 6;
        base |= (uint32_t)this->sign << 7;
        if constexpr (sizeof(T) == sizeof(uint16_t)) {
            base |= (uint32_t)this->trap << 8;
            base |= (uint32_t)this->interrupt << 9;
            base |= (uint32_t)this->direction << 10;
            base |= (uint32_t)this->overflow << 11;
        }
        return base;
    }

    template <typename T>
    inline void set_flags(T src) {
        this->carry = src & 0x01;
        this->parity = src & 0x04;
        this->auxiliary = src & 0x10;
        this->zero = src & 0x40;
        this->sign = src & 0x80;
        if constexpr (sizeof(T) == sizeof(uint16_t)) {
            this->trap = src & 0x0100;
            this->interrupt = src & 0x0200;
            this->direction = src & 0x0400;
            this->overflow = src & 0x0800;
        }
    }

    inline void update_parity(uint8_t val) {
        this->parity = !__builtin_parity(val);
    }

    template <typename T>
    inline void add_impl(T& dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        T res = dst + src;
        this->carry = __builtin_add_overflow((U)dst, (U)src, (U*)NULL);
        this->overflow = __builtin_add_overflow((S)dst, (S)src, (S*)NULL);
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        dst = res;
        this->update_parity(dst);
        this->zero = !dst;
        this->sign = (S)dst < 0;
    }

    template <typename T>
    inline void or_impl(T& dst, T src) {
        using S = std::make_signed_t<T>;
        this->carry = false;
        this->overflow = false;
        dst |= src;
        this->update_parity(dst);
        this->zero = !dst;
        this->sign = (S)dst < 0;
    }

    template <typename T>
    inline void adc_impl(T& dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        T res = carry_add((U)dst, (U)src, this->carry);
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        this->overflow = (S)(~(dst ^ src) & (dst ^ res)) < 0;
        dst = res;
        this->update_parity(dst);
        this->zero = !dst;
        this->sign = (S)dst < 0;
    }

    template <typename T>
    inline void sbb_impl(T& dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        T res = carry_sub((U)dst, (U)src, this->carry);
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        this->overflow = (S)(~(dst ^ src) & (dst ^ res)) < 0;
        dst = res;
        this->update_parity(dst);
        this->zero = !dst;
        this->sign = (S)dst < 0;
    }

    template <typename T>
    inline void and_impl(T& dst, T src) {
        using S = std::make_signed_t<T>;
        this->carry = false;
        this->overflow = false;
        dst &= src;
        this->update_parity(dst);
        this->zero = !dst;
        this->sign = (S)dst < 0;
    }

    template <typename T>
    inline void sub_impl(T& dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        T res = dst + src;
        this->carry = __builtin_sub_overflow((U)dst, (U)src, (U*)NULL);
        this->overflow = __builtin_sub_overflow((S)dst, (S)src, (S*)NULL);
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        dst = res;
        this->update_parity(dst);
        this->zero = !dst;
        this->sign = (S)dst < 0;
    }

    template <typename T>
    inline void xor_impl(T& dst, T src) {
        using S = std::make_signed_t<T>;
        this->carry = false;
        this->overflow = false;
        dst ^= src;
        this->update_parity(dst);
        this->zero = !dst;
        this->sign = (S)dst < 0;
    }

    template <typename T>
    inline void cmp_impl(T dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        T res = dst + src;
        this->carry = __builtin_sub_overflow((U)dst, (U)src, (U*)NULL);
        this->overflow = __builtin_sub_overflow((S)dst, (S)src, (S*)NULL);
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        this->update_parity(res);
        this->zero = !res;
        this->sign = (S)res < 0;
    }

    template <typename T>
    inline void test_impl(T dst, T src) {
        using S = std::make_signed_t<T>;
        this->carry = false;
        this->overflow = false;
        T res = dst & src;
        this->update_parity(res);
        this->zero = !res;
        this->sign = (S)res < 0;
    }

    template <typename T>
    inline void inc_impl(T& dst) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        this->overflow = dst == (std::numeric_limits<S>::max)();
        this->auxiliary = (dst ^ 1 ^ dst + 1) & 0x10; // BLCMSK
        ++dst;
        this->update_parity(dst);
        this->zero = !dst;
        this->sign = (S)dst < 0;
    }

    template <typename T>
    inline void dec_impl(T& dst) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        this->overflow = dst == (std::numeric_limits<S>::max)();
        this->auxiliary = (dst ^ 1 ^ dst - 1) & 0x10; // BLSMSK
        --dst;
        this->update_parity(dst);
        this->zero = !dst;
        this->sign = (S)dst < 0;
    }

    template <typename T>
    inline void not_impl(T& dst) {
        dst = ~dst;
    }

    template <typename T>
    inline void neg_impl(T& dst) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        this->carry = dst;
        this->overflow = (S)dst == (std::numeric_limits<S>::min)();
        this->auxiliary = dst & 0xF;
        dst = -dst;
        this->update_parity(dst);
        this->zero = !dst;
        this->sign = (S)dst < 0;
    }

    template <typename T>
    inline void xchg_impl(T& dst, T& src) {
        std::swap(dst, src);
    }

    template <typename T = int16_t>
    inline void push_impl(T src) {
        this->sp -= 2;
        x86Addr stack = this->stack();
        stack.write(src);
    }

    template <typename T = uint16_t>
    inline uint16_t pop_impl() {
        x86Addr stack = this->stack();
        T ret = stack.read<T>();
        this->sp += 2;
        return ret;
    }

    // TODO: rep prefix negating inputs of imul/idiv
    template <typename T>
    inline void mul_impl(T src) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            this->ax = (uint16_t)this->al * src;
            this->carry = this->overflow = this->ah;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            uint32_t temp = (uint32_t)this->ax * src;
            this->ax = temp;
            this->dx = temp >> 16;
            this->carry = this->overflow = this->dx;
        }
    }

    template <typename T>
    inline void imul_impl(T src) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            int16_t temp = (int16_t)this->al * src;
            if (this->rep_type > NO_REP) {
                temp = -temp;
            }
            this->ax = temp;
            this->carry = this->overflow = this->ah != (int8_t)this->al >> 7;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            int32_t temp = (int32_t)this->ax * src;
            if (this->rep_type > NO_REP) {
                temp = -temp;
            }
            this->ax = temp;
            this->dx = temp >> 16;
            this->carry = this->overflow = this->dx != (int16_t)this->ax >> 15;
        }
    }

    template <typename T>
    inline void div_impl(T src) {
        if (!src) {
            // TODO: exception
        }
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            uint16_t temp = this->ax;
            uint16_t quot = temp / src;
            if (quot > UINT8_MAX) {
                // TODO: exception
            }
            this->al = quot;
            this->ah = temp % src;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            uint32_t temp = this->ax | (uint32_t)this->dx << 16;
            uint32_t quot = temp / src;
            if (quot > UINT16_MAX) {
                // TODO: exception
            }
            this->ax = quot;
            this->dx = temp % src;
        }
    }

    template <typename T>
    inline void idiv_impl(T src) {
        if (!src) {
            // TODO: exception
        }
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            int16_t temp = this->ax;
            int16_t quot = temp / src;
            if (this->rep_type > NO_REP) {
                quot = -quot;
            }
            if ((uint16_t)(quot - INT8_MIN) > UINT8_MAX) {
                // TODO: exception
            }
            this->al = quot;
            this->ah = temp % src;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            int32_t temp = this->ax | (uint32_t)this->dx << 16;
            int32_t quot = temp / src;
            if (this->rep_type > NO_REP) {
                quot = -quot;
            }
            if ((uint32_t)(quot - INT16_MIN) > UINT16_MAX) {
                // TODO: exception
            }
            this->ax = quot;
            this->dx = temp % src;
        }
    }

    template <typename T>
    inline void rol_impl(T& dst, uint8_t count) {
        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;

            T res = std::rotl<U>(dst, count);
            this->carry = res & 1;
            this->overflow = this->carry == (S)res < 0;
            dst = res;
        }
    }

    template <typename T>
    inline void ror_impl(T& dst, uint8_t count) {
        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;

            T res = std::rotr<U>(dst, count);
            this->carry = res & 1;
            this->overflow = this->carry == (S)res < 0;
            dst = res;
        }
    }

    template <typename T>
    inline void rcl_impl(T& dst, uint8_t count) {
        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;
            constexpr size_t bits = bitsof(T) + 1;

            UBitInt(bits) temp = dst;
            if (this->carry) {
                temp += (U)(std::numeric_limits<S>::min)();
                temp += (U)(std::numeric_limits<S>::min)();
            }
            count %= bits;
            temp = temp << count | temp >> bits - count;
            this->carry = temp > (std::numeric_limits<U>::max)();
            dst = (U)temp;
            this->overflow = this->carry == (S)dst < 0;
        }
    }

    template <typename T>
    inline void rcr_impl(T& dst, uint8_t count) {
        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;
            constexpr size_t bits = bitsof(T) + 1;

            UBitInt(bits) temp = dst;
            if (this->carry) {
                temp += (U)(std::numeric_limits<S>::min)();
                temp += (U)(std::numeric_limits<S>::min)();
            }
            count %= bits;
            temp = temp >> count | temp << bits - count;
            this->carry = temp > (std::numeric_limits<U>::max)();
            dst = (U)temp;
            this->overflow = __builtin_parity(dst & 3u << bitsof(T) - 2);
        }
    }

    template <typename T>
    inline void shl_impl(T& dst, uint8_t count) {
        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;

            this->carry = ((size_t)dst >> bitsof(T) - count) & 1;
            T res = (U)dst << count;
            this->overflow = this->carry == (S)res < 0;
            dst = res;
            this->update_parity(dst);
            this->zero = !dst;
            this->sign = (S)dst < 0;
        }
    }

    template <typename T>
    inline void shr_impl(T& dst, uint8_t count) {
        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;

            this->carry = ((size_t)dst >> 1 - count) & 1;
            this->overflow = (S)dst < 0;
            dst = (U)dst >> count;
            this->update_parity(dst);
            this->zero = !dst;
            this->sign = (S)dst < 0;
        }
    }

    // Yay, jank
    template <typename T>
    inline void setmo_impl(T& dst, uint8_t count) {
        if (count) {
            this->or_impl<T>(dst, (T)-1);
        }
    }

    template <typename T>
    inline void sar_impl(T& dst, uint8_t count) {
        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;

            this->carry = ((size_t)dst >> 1 - count) & 1;
            this->overflow = false;
            dst = (S)dst >> count;
            this->update_parity(dst);
            this->zero = !dst;
            this->sign = (S)dst < 0;
        }
    }

    // TODO: Check what happens if an interrupt toggles 
    // the direction flag during a repeating string instruction
    template <typename T>
    inline void lods_impl() {
        intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
        x86Addr src_addr = this->str_src();
        do {
            this->index_reg<T>(AX) = src_addr.read_advance<T>(offset);
        } while (this->rep_type > NO_REP && --this->cx);
        this->si = src_addr.offset;
    }

    template <typename T>
    inline void movs_impl() {
        intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
        x86Addr src_addr = this->str_src();
        x86Addr dst_addr = this->str_dst();
        do {
            dst_addr.write_advance<T>(src_addr.read_advance<T>(offset), offset);
        } while (this->rep_type > NO_REP && --this->cx);
        this->si = src_addr.offset;
        this->di = dst_addr.offset;
    }

    template <typename T>
    inline void stos_impl() {
        intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
        x86Addr dst_addr = this->str_dst();
        do {
            dst_addr.write_advance<T>(this->index_reg<T>(AX), offset);
        } while (this->rep_type > NO_REP && --this->cx);
        this->di = dst_addr.offset;
    }

    template <typename T>
    inline void scas_impl() {
        intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
        x86Addr dst_addr = this->str_dst();
        do {
            this->cmp_impl<T>(this->index_reg<T>(AX), dst_addr.read_advance<T>(offset));
        } while (this->rep_type > NO_REP && --this->cx && this->rep_type == this->zero);
        this->di = dst_addr.offset;
    }

    template <typename T>
    inline void cmps_impl() {
        intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
        x86Addr src_addr = this->str_src();
        x86Addr dst_addr = this->str_dst();
        do {
            this->cmp_impl<T>(src_addr.read_advance<T>(offset), dst_addr.read_advance<T>(offset));
        } while (this->rep_type > NO_REP && --this->cx && this->rep_type == this->zero);
        this->si = src_addr.offset;
        this->di = src_addr.offset;
    }

    // TODO: Read microcode dump to confirm accurate behavior of BCD, there's reason to doubt official docs here
    inline void aaa_impl() {
        if (this->auxiliary || (this->al & 0xF) > 9) {
            this->auxiliary = this->carry = true;
            this->ax += 0x106;
        } else {
            this->carry = false;
        }
        this->al &= 0xF;
    }

    inline void aas_impl() {
        if (this->auxiliary || (this->al & 0xF) > 9) {
            this->auxiliary = this->carry = true;
            this->ax -= 0x106;
        } else {
            this->carry = false;
        }
        this->al &= 0xF;
    }

    // Supposedly better implementation than official docs: https://www.righto.com/2023/01/understanding-x86s-decimal-adjust-after.html
    inline void daa_impl() {
        uint8_t temp = this->al;
        if (this->auxiliary || (temp & 0xF) > 9) {
            this->auxiliary = true;
            this->al -= 0x06;
        }
        if (this->carry || temp > 0x99) {
            this->carry = true;
            this->al -= 0x60;
        }
    }

    inline void das_impl() {
        uint8_t temp = this->al;
        if (this->auxiliary || (temp & 0xF) > 9) {
            this->auxiliary = true;
            this->al += 0x06;
        }
        if (this->carry || temp > 0x99) {
            this->carry = true;
            this->al += 0x60;
        }
    }

    inline void aam_impl(uint8_t imm) {
        this->ah = this->al / imm;
        this->al %= imm;
        this->update_parity(this->al);
        this->zero = !this->al;
        this->sign = (int8_t)this->al < 0;
    }

    inline void aad_impl(uint8_t imm) {
        this->al += this->ah * imm;
        this->ah = 0;
        this->update_parity(this->al);
        this->zero = !this->al;
        this->sign = (int8_t)this->al < 0;
    }
};

static x86Context ctx;

#if USE_BITFIELDS
union ModRM {
    uint8_t raw;
private:
    struct {
        uint8_t m : 3;
        uint8_t r : 3;
        uint8_t mod : 2;
    };
public:

    inline constexpr uint8_t Mod() const {
        return this->mod;
    }

    inline constexpr uint8_t R() const {
        return this->r;
    }

    inline constexpr uint8_t M() const {
        return this->m;
    }
#else
struct ModRM {
    uint8_t raw;

    inline constexpr uint8_t Mod() const {
        return this->raw >> 6;
    }

    inline constexpr uint8_t R() const {
        return (this->raw & 0070) >> 3;
    }

    inline constexpr uint8_t M() const {
        return this->raw & 0007;
    }
#endif

    inline constexpr bool is_mem() const {
        return this->Mod() != 3;
    }

    uint32_t extra_length() const {
        switch (this->Mod()) {
            case 0:
                return (this->M() == 6) * 2;
            case 1:
                return 1;
            case 2:
                return 2;
            case 3:
                return 0;
            default:
                unreachable;
        }
    }

    uint32_t parse_memM(x86Addr& pc) const {
        uint8_t default_segment = DS;
        size_t offset;
        uint8_t m = this->M();
        switch (m) {
            case 0:
                offset = ctx.bx + ctx.si;
                break;
            case 1:
                offset = ctx.bx + ctx.di;
                break;
            case 2:
                offset = ctx.bp + ctx.si;
                default_segment = SS;
                break;
            case 3:
                offset = ctx.bp + ctx.di;
                default_segment = SS;
                break;
            case 4:
                offset = ctx.si;
                break;
            case 5:
                offset = ctx.di;
                break;
            case 6:
                offset = ctx.bp;
                default_segment = SS;
                break;
            case 7:
                offset = ctx.bx;
                break;
            default:
                unreachable;
        }
        switch (this->Mod()) {
            case 0:
                if (m == 6) {
                    default_segment = DS;
                    offset = pc.read_advance<int16_t>();
                }
                break;
            case 1:
                offset += pc.read_advance<int8_t>();
                break;
            case 2:
                offset += pc.read_advance<int16_t>();
                break;
            default:
                unreachable;
        }
        return ctx.addr(default_segment, offset);
    }
};

template <typename T, typename L>
static inline void binopMR(x86Addr& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = ctx.index_reg<T>(modrm.R());
    if (modrm.is_mem()) {
        x86Addr data_addr = modrm.parse_memM(pc);
        T mval = data_addr.read<T>();
        if (lambda(mval, rval)) {
            data_addr.write<T>(mval);
        }
    }
    else {
        lambda(ctx.index_reg<T>(modrm.M()), rval);
    }
}

template <typename T, typename L>
static inline void binopRM(x86Addr& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = ctx.index_reg<T>(modrm.R());
    if (modrm.is_mem()) {
        x86Addr data_addr = modrm.parse_memM(pc);
        lambda(rval, data_addr.read<T>());
    }
    else {
        lambda(rval, ctx.index_reg<T>(modrm.M()));
    }
}

// Double width memory operand, special for LDS/LES
template <typename T, typename L>
static inline void binopRM2(x86Addr& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = ctx.index_reg<T>(modrm.R());
    if (modrm.is_mem()) {
        x86Addr data_addr = modrm.parse_memM(pc);
        uint32_t temp = data_addr.read<uint16_t>();
        temp |= (uint32_t)data_addr.read<uint16_t>(2) << 16;
        lambda(rval, temp);
    }
    else {
        // TODO: jank
    }
}

template <typename T, typename L>
static inline void unopM(x86Addr& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint8_t r = modrm.R();
    if (modrm.is_mem()) {
        x86Addr data_addr = modrm.parse_memM(pc);
        T mval = data_addr.read<T>();
        if (lambda(mval, r)) {
            data_addr.write<T>(mval);
        }
    }
    else {
        lambda(ctx.index_reg<T>(modrm.M()), r);
    }
}

template <typename T, typename L>
static inline void binopMS(x86Addr& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = ctx.index_seg(modrm.R());
    if (modrm.is_mem()) {
        x86Addr data_addr = modrm.parse_memM(pc);
        T mval = data_addr.read<T>();
        if (lambda(mval, rval)) {
            data_addr.write<T>(mval);
        }
    }
    else {
        lambda(ctx.index_seg(modrm.M()), rval);
    }
}

template <typename T, typename L>
static inline void binopSM(x86Addr& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = ctx.index_seg(modrm.R());
    if (modrm.is_mem()) {
        x86Addr data_addr = modrm.parse_memM(pc);
        lambda(rval, data_addr.read<T>());
    }
    else {
        lambda(rval, ctx.index_seg(modrm.M()));
    }
}

dllexport void execute_z86() {
    ctx = {
        .cs = 0xF000,
        .ip = 0xFFF0,
    };

    for (;;) {
        // Reset per-instruction states
        ctx.seg_override = -1;
        ctx.rep_type = NO_REP;
        ctx.lock = false;


        x86Addr pc = ctx.pc();
        // TODO: The 6 byte prefetch cache
        // TODO: Clock cycles
    next_byte:
        uint8_t opcode = pc.read_advance();
        switch (opcode) {
            case 0x00: // ADD Mb, Rb
                binopMR<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.add_impl(dst, src);
                    return true;
                });
                break;
            case 0x01: // ADD Mv, Rv
                binopMR<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.add_impl(dst, src);
                    return true;
                });
                break;
            case 0x02: // ADD Rb, Mb
                binopRM<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.add_impl(dst, src);
                    return true;
                });
                break;
            case 0x03: // ADD Rv, Mv
                binopRM<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.add_impl(dst, src);
                    return true;
                });
                break;
            case 0x04: // ADD AL, Ib
                ctx.add_impl(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x05: // ADD AX, Iz
                ctx.add_impl(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x06: case 0x0E: case 0x16: case 0x1E: // PUSH seg
                ctx.push_impl(ctx.index_seg(opcode >> 3));
                break;
            case 0x07: case 0x0F: case 0x17: case 0x1F: // POP seg
                ctx.index_seg(opcode >> 3) = ctx.pop_impl();
                break;
            case 0x08: // OR Mb, Rb
                binopMR<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.or_impl(dst, src);
                    return true;
                });
                break;
            case 0x09: // OR Mv, Rv
                binopMR<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.or_impl(dst, src);
                    return true;
                });
                break;
            case 0x0A: // OR Rb, Mb
                binopRM<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.or_impl(dst, src);
                    return true;
                });
                break;
            case 0x0B: // OR Rv, Mv
                binopRM<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.or_impl(dst, src);
                    return true;
                });
                break;
            case 0x0C: // OR AL, Ib
                ctx.or_impl(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x0D: // OR AX, Iz
                ctx.or_impl(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x10: // ADC Mb, Rb
                binopMR<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.adc_impl(dst, src);
                    return true;
                });
                break;
            case 0x11: // ADC Mv, Rv
                binopMR<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.adc_impl(dst, src);
                    return true;
                });
                break;
            case 0x12: // ADC Rb, Mb
                binopRM<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.adc_impl(dst, src);
                    return true;
                });
                break;
            case 0x13: // ADC Rv, Mv
                binopRM<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.adc_impl(dst, src);
                    return true;
                });
                break;
            case 0x14: // ADC AL, Ib
                ctx.adc_impl(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x15: // ADC AX, Iz
                ctx.adc_impl(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x18: // SBB Mb, Rb
                binopMR<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.sbb_impl(dst, src);
                    return true;
                });
                break;
            case 0x19: // SBB Mv, Rv
                binopMR<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.sbb_impl(dst, src);
                    return true;
                });
                break;
            case 0x1A: // SBB Rb, Mb
                binopRM<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.sbb_impl(dst, src);
                    return true;
                });
                break;
            case 0x1B: // SBB Rv, Mv
                binopRM<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.sbb_impl(dst, src);
                    return true;
                });
                break;
            case 0x1C: // SBB AL, Ib
                ctx.sbb_impl(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x1D: // SBB AX, Iz
                ctx.sbb_impl(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x20: // AND Mb, Rb
                binopMR<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.and_impl(dst, src);
                    return true;
                });
                break;
            case 0x21: // AND Mv, Rv
                binopMR<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.and_impl(dst, src);
                    return true;
                });
                break;
            case 0x22: // AND Rb, Mb
                binopRM<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.and_impl(dst, src);
                    return true;
                });
                break;
            case 0x23: // AND Rv, Mv
                binopRM<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.and_impl(dst, src);
                    return true;
                });
                break;
            case 0x24: // AND AL, Ib
                ctx.and_impl(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x25: // AND AX, Iz
                ctx.and_impl(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x26: case 0x2E: case 0x36: case 0x3E: // SEG:
                ctx.set_seg_override(opcode >> 3);
                goto next_byte;
            case 0x27: // DAA
                ctx.daa_impl();
                break;
            case 0x28: // SUB Mb, Rb
                binopMR<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.sub_impl(dst, src);
                    return true;
                });
                break;
            case 0x29: // SUB Mv, Rv
                binopMR<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.sub_impl(dst, src);
                    return true;
                });
                break;
            case 0x2A: // SUB Rb, Mb
                binopRM<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.sub_impl(dst, src);
                    return true;
                });
                break;
            case 0x2B: // SUB Rv, Mv
                binopRM<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.sub_impl(dst, src);
                    return true;
                });
                break;
            case 0x2C: // SUB AL, Ib
                ctx.sub_impl(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x2D: // SUB AX, Iz
                ctx.sub_impl(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x2F: // DAS
                ctx.das_impl();
                break;
            case 0x30: // XOR Mb, Rb
                binopMR<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.xor_impl(dst, src);
                    return true;
                });
                break;
            case 0x31: // XOR Mv, Rv
                binopMR<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.xor_impl(dst, src);
                    return true;
                });
                break;
            case 0x32: // XOR Rb, Mb
                binopRM<uint8_t>(pc, [](auto& dst, auto src) {
                    ctx.xor_impl(dst, src);
                    return true;
                });
                break;
            case 0x33: // XOR Rv, Mv
                binopRM<uint16_t>(pc, [](auto& dst, auto src) {
                    ctx.xor_impl(dst, src);
                    return true;
                });
                break;
            case 0x34: // XOR AL, Ib
                ctx.xor_impl(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x35: // XOR AX, Iz
                ctx.xor_impl(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x37: // AAA
                ctx.aaa_impl();
                break;
            case 0x38: // CMP Mb, Rb
                binopMR<uint8_t>(pc, [](auto dst, auto src) {
                    ctx.cmp_impl(dst, src);
                    return false;
                });
                break;
            case 0x39: // CMP Mv, Rv
                binopMR<uint16_t>(pc, [](auto dst, auto src) {
                    ctx.cmp_impl(dst, src);
                    return false;
                });
                break;
            case 0x3A: // CMP Rb, Mb
                binopRM<uint8_t>(pc, [](auto dst, auto src) {
                    ctx.cmp_impl(dst, src);
                    return false;
                });
                break;
            case 0x3B: // CMP Rv, Mv
                binopRM<uint16_t>(pc, [](auto dst, auto src) {
                    ctx.cmp_impl(dst, src);
                    return false;
                });
                break;
            case 0x3C: // CMP AL, Ib
                ctx.cmp_impl(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x3D: // CMP AX, Iz
                ctx.cmp_impl(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x3F: // AAS
                ctx.aas_impl();
                break;
            case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: // INC reg
                ctx.inc_impl(ctx.index_reg<uint16_t>(opcode & 7));
                break;
            case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F: // DEC reg
                ctx.dec_impl(ctx.index_reg<uint16_t>(opcode & 7));
                break;
            case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: // PUSH reg
                ctx.push_impl(ctx.index_reg<uint16_t>(opcode & 7));
                break;
            case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F: // POP reg
                ctx.index_reg<uint16_t>(opcode & 7) = ctx.pop_impl();
                break;
            case 0x60: case 0x70: // JO Jb
            case 0x61: case 0x71: // JNO Jb
                ctx.ip = pc.offset + 1;
                if (ctx.overflow != (opcode & 1)) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x62: case 0x72: // JC Jb
            case 0x63: case 0x73: // JNC Jb
                ctx.ip = pc.offset + 1;
                if (ctx.carry != (opcode & 1)) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x64: case 0x74: // JZ Jb
            case 0x65: case 0x75: // JNZ Jb
                ctx.ip = pc.offset + 1;
                if (ctx.zero != (opcode & 1)) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x66: case 0x76: // JBE Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_BE()) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x67: case 0x77: // JA Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_A()) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x68: case 0x78: // JS Jb
            case 0x69: case 0x79: // JNS Jb
                ctx.ip = pc.offset + 1;
                if (ctx.sign != (opcode & 1)) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x6A: case 0x7A: // JP Jb
            case 0x6B: case 0x7B: // JNP Jb
                ctx.ip = pc.offset + 1;
                if (ctx.parity != (opcode & 1)) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x6C: case 0x7C: // JL Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_L()) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x6D: case 0x7D: // JGE Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_GE()) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x6E: case 0x7E: // JLE Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_LE()) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x6F: case 0x7F: // JG Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_G()) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0x80: case 0x82: // GRP1 Mb, Ib
                unopM<uint8_t>(pc, [&](auto& dst, uint8_t r) {
                    uint8_t val = pc.read<int8_t>();
                    switch (r) {
                        case 0: ctx.add_impl(dst, val); return true;
                        case 1: ctx.or_impl(dst, val); return true;
                        case 2: ctx.adc_impl(dst, val); return true;
                        case 3: ctx.sbb_impl(dst, val); return true;
                        case 4: ctx.and_impl(dst, val); return true;
                        case 5: ctx.sub_impl(dst, val); return true;
                        case 6: ctx.xor_impl(dst, val); return true;
                        case 7: ctx.cmp_impl(dst, val); return false;
                        default: unreachable;
                    }
                });
                ++pc;
                break;
            case 0x81: // GRP1 Ev, Iz
                unopM<uint16_t>(pc, [&](auto& dst, uint8_t r) {
                    uint16_t val = pc.read<uint16_t>();
                    switch (r) {
                        case 0: ctx.add_impl(dst, val); return true;
                        case 1: ctx.or_impl(dst, val); return true;
                        case 2: ctx.adc_impl(dst, val); return true;
                        case 3: ctx.sbb_impl(dst, val); return true;
                        case 4: ctx.and_impl(dst, val); return true;
                        case 5: ctx.sub_impl(dst, val); return true;
                        case 6: ctx.xor_impl(dst, val); return true;
                        case 7: ctx.cmp_impl(dst, val); return false;
                        default: unreachable;
                    }
                });
                pc += 2;
                break;
            case 0x83: // GRP1 Ev, Ib
                unopM<uint16_t>(pc, [&](auto& dst, uint8_t r) {
                    uint16_t val = (int16_t)pc.read<int8_t>();
                    switch (r) {
                        case 0: ctx.add_impl(dst, val); return true;
                        case 1: ctx.or_impl(dst, val); return true;
                        case 2: ctx.adc_impl(dst, val); return true;
                        case 3: ctx.sbb_impl(dst, val); return true;
                        case 4: ctx.and_impl(dst, val); return true;
                        case 5: ctx.sub_impl(dst, val); return true;
                        case 6: ctx.xor_impl(dst, val); return true;
                        case 7: ctx.cmp_impl(dst, val); return false;
                        default: unreachable;
                    }
                });
                ++pc;
                break;
            case 0x84: // TEST Mb, Rb
                binopMR<uint8_t>(pc, [](auto dst, auto src) {
                    ctx.test_impl(dst, src);
                    return false;
                });
                break;
            case 0x85: // TEST Mv, Rv
                binopMR<uint16_t>(pc, [](auto dst, auto src) {
                    ctx.test_impl(dst, src);
                    return false;
                });
                break;
            case 0x86: // XCHG Mb, Rb
                binopMR<uint8_t>(pc, [](auto& dst, auto& src) {
                    ctx.xchg_impl(dst, src);
                    return true;
                });
                break;
            case 0x87: // XCHG Mv, Rv
                binopMR<uint16_t>(pc, [](auto& dst, auto& src) {
                    ctx.xchg_impl(dst, src);
                    return true;
                });
                break;
            case 0x88: // MOV Mb, Rb
                binopMR<uint8_t>(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x89: // MOV Mv, Rv
                binopMR<uint16_t>(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x8A: // MOV Rb, Mb
                binopRM<uint8_t>(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x8B: // MOV Rv, Mv
                binopRM<uint16_t>(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x8C: // MOV M, seg
                binopMS<uint16_t>(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x8D: { // LEA
                ModRM modrm = pc.read_advance<ModRM>();
                if (modrm.is_mem()) {
                    x86Addr addr = modrm.parse_memM(pc);
                    ctx.index_reg<uint16_t>(modrm.R()) = addr.offset;
                }
                else {
                    // TODO: jank
                }
                break;
            }
            case 0x8E: // MOV seg, M
                binopSM<uint16_t>(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x8F: // GRP1A (Supposedly this does mystery jank if R != 0)
                unopM<uint16_t>(pc, [](auto src, uint8_t r) {
                    switch (r) {
                        case 0:
                            ctx.push_impl(src);
                            return false;
                        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                            // TODO: ???
                            return false;
                        default:
                            unreachable;
                    }
                });
                break;
            case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: // XCHG AX, reg
                ctx.xchg_impl(ctx.ax, ctx.index_reg<uint16_t>(opcode & 7));
                break;
            case 0x98: // CBW
                ctx.ax = (int16_t)(int8_t)ctx.al;
                break;
            case 0x99: // CWD
                ctx.dx = (int16_t)ctx.ax >> 15;
                break;
            case 0x9A: // CALL far abs
                ctx.push_impl(ctx.cs);
                ctx.push_impl(pc.offset + 4);
                ctx.ip = pc.read<uint16_t>();
                ctx.cs = pc.read<uint16_t>(2);
                continue;
            case 0x9B: // WAIT
                // NOP :D
                break;
            case 0x9C: // PUSHF
                ctx.push_impl(ctx.get_flags<uint16_t>());
                break;
            case 0x9D: // POPF
                ctx.set_flags<uint16_t>(ctx.pop_impl());
                break;
            case 0x9E: // SAHF
                ctx.set_flags<uint8_t>(ctx.ah);
                break;
            case 0x9F: // LAHF
                ctx.ah = ctx.get_flags<uint8_t>();
                break;
            case 0xA0: { // MOV AL, mem
                x86Addr addr = ctx.addr(DS, pc.read_advance<uint16_t>());
                ctx.al = addr.read<uint8_t>();
                break;
            }
            case 0xA1: { // MOV AX, mem
                x86Addr addr = ctx.addr(DS, pc.read_advance<uint16_t>());
                ctx.ax = addr.read<uint16_t>();
                break;
            }
            case 0xA2: { // MOV mem, AL
                x86Addr addr = ctx.addr(DS, pc.read_advance<uint16_t>());
                addr.write(ctx.al);
                break;
            }
            case 0xA3: { // MOV mem, AX
                x86Addr addr = ctx.addr(DS, pc.read_advance<uint16_t>());
                addr.write(ctx.ax);
                break;
            }
            case 0xA4: // MOVSB
                ctx.movs_impl<uint8_t>();
                break;
            case 0xA5: // MOVSW
                ctx.movs_impl<uint16_t>();
                break;
            case 0xA6: // CMPSB
                ctx.cmps_impl<uint8_t>();
                break;
            case 0xA7: // CMPSW
                ctx.cmps_impl<uint16_t>();
                break;
            case 0xA8: // TEST AL, Ib
                ctx.test_impl(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0xA9: // TEST AX, Iz
                ctx.test_impl(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0xAA: // STOSB
                ctx.stos_impl<uint8_t>();
                break;
            case 0xAB: // STOSW
                ctx.stos_impl<uint16_t>();
                break;
            case 0xAC: // LODSB
                ctx.lods_impl<uint8_t>();
                break;
            case 0xAD: // LODSW
                ctx.lods_impl<uint16_t>();
                break;
            case 0xAE: // SCASB
                ctx.scas_impl<uint8_t>();
                break;
            case 0xAF: // SCASW
                ctx.scas_impl<uint16_t>();
                break;
            case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: // MOV reg8, Ib
                ctx.index_reg<uint8_t>(opcode & 7) = pc.read_advance<int8_t>();
                break;
            case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF: // MOV reg, Iv
                ctx.index_reg<uint16_t>(opcode & 7) = pc.read_advance<uint16_t>();
                break;
            case 0xC0: case 0xC2: // RET imm
                ctx.ip = ctx.pop_impl();
                ctx.sp += pc.read<int16_t>();
                continue;
            case 0xC1: case 0xC3: // RET
                ctx.ip = ctx.pop_impl();
                continue;
            case 0xC4: // LES
                binopRM2<uint16_t>(pc, [](auto& dst, uint32_t src) {
                    dst = src;
                    ctx.es = src >> 16;
                    return true;
                });
                break;
            case 0xC5: // LDS
                binopRM2<uint16_t>(pc, [](auto& dst, uint32_t src) {
                    dst = src;
                    ctx.ds = src >> 16;
                    return true;
                });
                break;
            case 0xC6: // GRP11 Ib (Supposedly this just ignores R bits)
                unopM<uint8_t>(pc, [&](auto& dst, uint8_t r) {
                    dst = pc.read<int8_t>();
                    return true;
                });
                ++pc;
                break;
            case 0xC7: // GRP11 Iz (Supposedly this just ignores R bits)
                unopM<uint16_t>(pc, [&](auto& dst, uint8_t r) {
                    dst = pc.read<uint16_t>();
                    return true;
                });
                pc += 2;
                break;
            case 0xC8: case 0xCA: // RETF imm
                ctx.ip = ctx.pop_impl();
                ctx.cs = ctx.pop_impl();
                ctx.sp += pc.read<int16_t>();
                continue;
            case 0xC9: case 0xCB: // RETF
                ctx.ip = ctx.pop_impl();
                ctx.cs = ctx.pop_impl();
                continue;
            case 0xCC: // INT3
            case 0xCD: // INT Ib
            case 0xCE: // INTO
                // TODO
                break;
            case 0xCF: // IRET
                ctx.ip = ctx.pop_impl();
                ctx.cs = ctx.pop_impl();
                ctx.set_flags(ctx.pop_impl());
                continue;
            case 0xD0: // GRP2 Mb, 1
                unopM<uint8_t>(pc, [](auto& dst, uint8_t r) {
                    switch (r) {
                        case 0: ctx.rol_impl(dst, 1); return true;
                        case 1: ctx.ror_impl(dst, 1); return true;
                        case 2: ctx.rcl_impl(dst, 1); return true;
                        case 3: ctx.rcr_impl(dst, 1); return true;
                        case 4: ctx.shl_impl(dst, 1); return true;
                        case 5: ctx.shr_impl(dst, 1); return true;
                        case 6: ctx.setmo_impl(dst, 1); return true;
                        case 7: ctx.sar_impl(dst, 1); return true;
                        default: unreachable;
                    }
                });
                break;
            case 0xD1: // GRP2 Mv, 1
                unopM<uint16_t>(pc, [](auto& dst, uint8_t r) {
                    switch (r) {
                        case 0: ctx.rol_impl(dst, 1); return true;
                        case 1: ctx.ror_impl(dst, 1); return true;
                        case 2: ctx.rcl_impl(dst, 1); return true;
                        case 3: ctx.rcr_impl(dst, 1); return true;
                        case 4: ctx.shl_impl(dst, 1); return true;
                        case 5: ctx.shr_impl(dst, 1); return true;
                        case 6: ctx.setmo_impl(dst, 1); return true;
                        case 7: ctx.sar_impl(dst, 1); return true;
                        default: unreachable;
                    }
                });
                break;
            case 0xD2: // GRP2 Mb, CL
                unopM<uint8_t>(pc, [](auto& dst, uint8_t r) {
                    switch (r) {
                        case 0: ctx.rol_impl(dst, ctx.cl); return true;
                        case 1: ctx.ror_impl(dst, ctx.cl); return true;
                        case 2: ctx.rcl_impl(dst, ctx.cl); return true;
                        case 3: ctx.rcr_impl(dst, ctx.cl); return true;
                        case 4: ctx.shl_impl(dst, ctx.cl); return true;
                        case 5: ctx.shr_impl(dst, ctx.cl); return true;
                        case 6: ctx.setmo_impl(dst, ctx.cl); return true;
                        case 7: ctx.sar_impl(dst, ctx.cl); return true;
                        default: unreachable;
                    }
                });
                break;
            case 0xD3: // GRP2 Mv, CL
                unopM<uint16_t>(pc, [](auto& dst, uint8_t r) {
                    switch (r) {
                        case 0: ctx.rol_impl(dst, ctx.cl); return true;
                        case 1: ctx.ror_impl(dst, ctx.cl); return true;
                        case 2: ctx.rcl_impl(dst, ctx.cl); return true;
                        case 3: ctx.rcr_impl(dst, ctx.cl); return true;
                        case 4: ctx.shl_impl(dst, ctx.cl); return true;
                        case 5: ctx.shr_impl(dst, ctx.cl); return true;
                        case 6: ctx.setmo_impl(dst, ctx.cl); return true;
                        case 7: ctx.sar_impl(dst, ctx.cl); return true;
                        default: unreachable;
                    }
                });
                break;
                break;
            case 0xD4: // AAM Ib
                ctx.aam_impl(pc.read_advance<uint8_t>());
                break;
            case 0xD5: // AAD Ib
                ctx.aad_impl(pc.read_advance<uint8_t>());
                break;
            case 0xD6: // SALC
                ctx.al = ctx.carry ? -1 : 0;
                break;
            case 0xD7: { // XLAT
                x86Addr addr = ctx.addr(DS, ctx.bx + ctx.al);
                ctx.al = addr.read<uint8_t>();
                break;
            }
            case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDE: case 0xDF: { // ESC x87
                // NOP :D
                pc += 1 + pc.read<ModRM>().extra_length();
                break;
            }
            case 0xE0: // LOOPNZ Jb
                ctx.ip = pc.offset + 1;
                if (--ctx.cx || ctx.cond_NZ()) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0xE1: // LOOPZ Jb
                ctx.ip = pc.offset + 1;
                if (--ctx.cx || ctx.cond_Z()) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0xE2: // LOOP Jb
                ctx.ip = pc.offset + 1;
                if (--ctx.cx) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0xE3: // JCXZ Jb
                ctx.ip = pc.offset + 1;
                if (!ctx.cx) {
                    ctx.ip += pc.read<int8_t>();
                }
                continue;
            case 0xE4: // IN AL, Ib
            case 0xE5: // IN AX, Ib
            case 0xE6: // OUT Ib, AL
            case 0xE7: // OUT Ib, AX
                // TODO
                break;
            case 0xE8: // CALL Jz
                ctx.push_impl(pc.offset + 2);
                ctx.ip = pc.offset + 2 + pc.read<int16_t>();
                continue;
            case 0xE9: // JMP Jz
                ctx.ip = pc.offset + 2 + pc.read<int16_t>();
                continue;
            case 0xEA: // JMP far abs
                ctx.ip = pc.read<uint16_t>();
                ctx.cs = pc.read<uint16_t>(2);
                continue;
            case 0xEB: // JMP Jb
                ctx.ip = pc.offset + 1 + pc.read<int8_t>();
                continue;
            case 0xEC: // IN AL, DX
            case 0xED: // IN AX, DX
            case 0xEE: // OUT DX, AL
            case 0xEF: // OUT DX, AX
                // TODO
                break;
            case 0xF0: case 0xF1: // LOCK
                ctx.lock = true;
                goto next_byte;
            case 0xF2: case 0xF3: // REPNE, REP
                ctx.rep_type = opcode & 1;
                goto next_byte;
            case 0xF4: // HLT
                // TODO
                break;
            case 0xF5: // CMC
                ctx.carry ^= 1;
                break;
            case 0xF6: // GRP3 Mb
                unopM<uint8_t>(pc, [&](auto& val, uint8_t r) {
                    switch (r) {
                        case 0: case 1: // TEST Mb, Ib
                            ctx.test_impl(val, pc.read_advance<uint8_t>());
                            return false;
                        case 2: // NOT Mb
                            ctx.not_impl(val);
                            return true;
                        case 3: // NEG Mb
                            ctx.neg_impl(val);
                            return true;
                        case 4: // MUL Mb
                            ctx.mul_impl(val);
                            return false;
                        case 5: // IMUL Mb
                            ctx.imul_impl(val);
                            return false;
                        case 6: // DIV Mb
                            ctx.div_impl(val);
                            return false;
                        case 7: // IDIV Mb
                            ctx.idiv_impl(val);
                            return false;
                        default:
                            unreachable;
                    }
                });
                break;
            case 0xF7: // GRP3 Mv
                unopM<uint16_t>(pc, [&](auto& val, uint8_t r) {
                    switch (r) {
                        case 0: case 1: // TEST Mv, Iz
                            ctx.test_impl(val, pc.read_advance<uint16_t>());
                            return false;
                        case 2: // NOT Mv
                            ctx.not_impl(val);
                            return true;
                        case 3: // NEG Mv
                            ctx.neg_impl(val);
                            return true;
                        case 4: // MUL Mv
                            ctx.mul_impl(val);
                            return false;
                        case 5: // IMUL Mv
                            ctx.imul_impl(val);
                            return false;
                        case 6: // DIV Mv
                            ctx.div_impl(val);
                            return false;
                        case 7: // IDIV Mv
                            ctx.idiv_impl(val);
                            return false;
                        default:
                            unreachable;
                    }
                });
                break;
            case 0xF8: case 0xF9: // CLC, STC
                ctx.carry = opcode & 1;
                break;
            case 0xFA: case 0xFB: // CLI, STI
                ctx.interrupt = opcode & 1;
                break;
            case 0xFC: case 0xFD: // CLD, STD
                ctx.direction = opcode & 1;
                break;
            case 0xFE: // GRP4 Mb
            case 0xFF: // GRP5 Mv
                // TODO: deal with the extra BS of getting segment refs on the far ops
                break;
        }
        ctx.ip = pc.offset;
    }
}