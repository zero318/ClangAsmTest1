#pragma once

#ifndef Z86_CORE_INTERNAL_POST_H
#define Z86_CORE_INTERNAL_POST_H 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <limits>
#include <algorithm>
#include <utility>
#include <type_traits>

#include "../zero/util.h"

#if __INTELLISENSE__
// This just helps when intellisense derps out sometimes
#if !Z86_CORE_INTERNAL_PRE_H
#include "z86_core_internal_pre.h"
#endif
#endif

template <size_t max_bits>
struct z86AddrESImpl<max_bits, true> {
    using type = z86AddrFixedImpl<max_bits, ES>;
};
template <size_t max_bits>
struct z86AddrCSImpl<max_bits, true> {
    using type = z86AddrFixedImpl<max_bits, CS>;
};
template <size_t max_bits>
struct z86AddrSSImpl<max_bits, true> {
    using type = z86AddrFixedImpl<max_bits, SS>;
};

using z86Addr = z86AddrImpl<ctx.max_bits, ctx.PROTECTED_MODE>;

using z86AddrES = z86AddrESImpl<ctx.max_bits, ctx.PROTECTED_MODE>::type;
using z86AddrCS = z86AddrCSImpl<ctx.max_bits, ctx.PROTECTED_MODE>::type;
using z86AddrSS = z86AddrSSImpl<ctx.max_bits, ctx.PROTECTED_MODE>::type;

template <OP_FLAGS op_flags, typename P>
uint32_t ModRM::extra_length(const P& pc) const {
    uint8_t mod = this->Mod();
    if (mod == 3) {
        return 0;
    }
    if constexpr (ctx.max_bits > 16) {
        if (OP_CANNOT_ADDR16(op_flags) || !ctx.addr_size_16()) {
            uint32_t length = 0;
            switch (this->M()) {
                case 4:
                    ++length;
                    if (mod == 0) {
                        return 1 + (pc.read<SIB>(1).B() == 5) * 4;
                    }
                    break;
                case 5:
                    if (mod == 0) {
                        return 4;
                    }
            }
            switch (mod) {
                case 0:
                    return length;
                case 1:
                    return length + 1;
                case 2:
                    return length + 4;
                default:
                    unreachable;
            }
        }
    }
    switch (mod) {
        case 0:
            return (this->M() == 6) * 2;
        case 1:
            return 1;
        case 2:
            return 2;
        default:
            unreachable;
    }
}

template <OP_FLAGS op_flags, size_t imm_offset, typename P>
auto ModRM::parse_memM(P& pc) const {
    uint32_t segment_mask;
    size_t offset = 0;
    uint8_t m = this->M();
    assume(m < 8);
    uint8_t mod = this->Mod();
    if constexpr (ctx.max_bits > 16) {
        if (OP_CANNOT_ADDR16(op_flags) || !ctx.addr_size_16()) {
            if constexpr (!OP_CANNOT_ADDR64(op_flags) && ctx.max_bits == 64) {
                if (ctx.addr_size_64()) {
                    switch (m) {
                        default: unreachable;
                        case RSP: {
                            SIB sib = pc.read_advance<SIB>();
                            uint8_t i = sib.I();
                            if (i != RSP) {
                                offset = ctx.index_qword_regI(i) * (1 << sib.S());
                            }
                            m = sib.B();
                            if (m == RBP && mod == 0) {
                                // Not RIP relative
                                // Label overrides segment to DS
                                goto add_const32;
                            }
                            break;
                        }
                        case RBP:
                            if (mod == 0) {
                                // TODO: Properly offset for
                                // the end of the instruction
                                //offset = ctx.rip;
                                offset = pc.offset + imm_offset;
                                goto add_const32;
                            }
                        case RAX: case RCX: case RDX: case RBX: case RSI: case RDI:
                            break;
                    }
                    // Initialize the full M so that
                    // later code can bit test for 
                    // default segment
                    m = ctx.full_indexMB(m);
                    offset += ctx.index_qword_reg_raw(m);
                }
                goto add_const;
            }
            switch (m) {
                default: unreachable;
                case ESP: {
                    SIB sib = pc.read_advance<SIB>();
                    uint8_t i = sib.I();
                    if (i != ESP) {
                        offset = ctx.index_dword_regI<OP_CANNOT_ADDR64(op_flags)>(i) * (1 << sib.S());
                    }
                    m = sib.B();
                    if (m == EBP && mod == 0) {
                        // Not EIP relative
                        // Label overrides segment to DS
                        goto add_const32;
                    }
                    break;
                }
                case EBP:
                    if (mod == 0) {
                        if constexpr (!OP_CANNOT_ADDR64(op_flags) && ctx.max_bits == 64) {
                            if (expect(ctx.is_long_mode(), false)) {
                                // TODO: Properly offset for
                                // the end of the instruction
                                //offset = ctx.eip;
                                offset = (uint32_t)pc.offset + imm_offset;
                            }
                        }
                        goto add_const32;
                    }
                case EAX: case ECX: case EDX: case EBX: case ESI: case EDI:
                    break;
            }
            // Initialize the full M so that
            // later code can bit test for 
            // default segment
            if constexpr (!OP_CANNOT_ADDR64(op_flags)) {
                m = ctx.full_indexMB(m);
            }
            offset += ctx.index_dword_reg_raw(m);
        add_const:
            switch (mod) {
                default: unreachable;
                case 1:
                    offset += pc.read_advance<int8_t>();
                    break;
                add_const32:
                    // Override the segment to always be DS
                    m = 0;
                case 2:
                    offset += pc.read_advance<int32_t>();
                case 0:;
            }
            // Set bits are for DS
            if constexpr (!OP_CANNOT_ADDR64(op_flags) && ctx.max_bits == 64) {
                segment_mask = 0b11111111111111111111111111001111;
            }
            else {
                segment_mask = 0b11001111;
            }
            goto ret;
        }
    }
    
    uint32_t base16;
    if constexpr (sizeof(void*) == sizeof(uint64_t)) {
        static constexpr uint64_t first_reg16 = PackUInt64(BX, BX, BP, BP, SI, DI, BP, BX);
        base16 = first_reg16 >> m * 8 & 0xFF;
    }
    else {
        // This is using octets instead of nibbles
        // just because LEA reg [reg*2+reg] is
        // shorter than LEA reg [reg*4]
        static constexpr uint32_t first_reg16 = PackUIntOctets32(BX, BX, BP, BP, SI, DI, BP, BX);
        base16 = first_reg16 >> m * 3 & 0x7;
    }
    //static constexpr uint32_t first_reg16[] = { BX, BX, BP, BP, SI, DI, BP, BX };
    //offset = ctx.index_word_regMB<true>(first_reg16[m]);
    offset = ctx.index_word_regMB<true>(base16);
    if (m < 4) {
        offset += ctx.index_word_regI<true>(SI | m);
    }
    switch (mod) {
        default: unreachable;
        case 1:
            // TODO:
            // Merge this with 32 bit byte offset somehow?
            offset += pc.read_advance<int8_t>();
            break;
        case 0:
            if (m != 6) {
                break;
            }
            m = 0;
            offset = 0;
        case 2:
            offset += pc.read_advance<int16_t>();
    }
    // Set bits are for DS
    segment_mask = 0b10110011;
ret:
    //return std::make_pair(ctx.addr(SS + (bool)(segment_mask & 1 << m), offset), pc.raw);
    return ctx.addr(SS + (bool)(segment_mask & 1 << m), offset);
}

template <size_t max_bits>
template <typename T>
inline constexpr T* z86DescriptorCache<max_bits>::get_ptr() const {
    return mem.ptr<T>(this->base);
}

template <size_t max_bits>
inline constexpr SEG_DESCRIPTOR<max_bits>* z86DescriptorCache<max_bits>::get_descriptor(uint16_t selector) const {
    BT offset = selector & 0xFFF8;
    if (expect(offset <= this->limit, true)) {
        return mem.ptr<SEG_DESCRIPTOR<max_bits>>(offset + this->base);
    }
    return NULL;
}

template <size_t bits, bool protected_mode>
inline constexpr const z86DescriptorCache<bits>& z86AddrImpl<bits, protected_mode>::descriptor() const {
    if constexpr (protected_mode) {
        return ctx.descriptors[this->segment];
    }
    else {
        // This shouldn't get executed
        return std::declval<z86DescriptorCache<bits>>();
    }
}

template <size_t bits, bool protected_mode>
inline constexpr size_t z86AddrImpl<bits, protected_mode>::limit() const {
    if constexpr (protected_mode) {
        return ctx.descriptors[this->segment].limit;
    }
    else {
        return (std::numeric_limits<OT>::max)();
    }
}

template <size_t bits, bool protected_mode>
inline constexpr size_t z86AddrImpl<bits, protected_mode>::seg() const {
    if constexpr (protected_mode) {
        return ctx.descriptors[this->segment].base;
    }
    else {
        return (size_t)this->segment << 4;
    }
}

template <size_t max_bits, uint8_t descriptor_index>
inline constexpr const z86DescriptorCache<max_bits>& z86AddrFixedImpl<max_bits, descriptor_index>::descriptor() const {
    return ctx.descriptors[descriptor_index];
}

template <size_t max_bits, uint8_t descriptor_index>
inline constexpr size_t z86AddrFixedImpl<max_bits, descriptor_index>::limit() const {
    return ctx.descriptors[descriptor_index].limit;
}

template <size_t max_bits, uint8_t descriptor_index>
inline constexpr size_t z86AddrFixedImpl<max_bits, descriptor_index>::seg() const {
    return ctx.descriptors[descriptor_index].base;
}

template <typename T>
inline constexpr bool z86AddrSharedFuncs::addr_fits_on_bus(size_t addr) {
    return (addr & align_mask<ctx.bus_bytes>) <= ctx.bus_bytes - z86DataProperites<T>::size;
}

template <typename T>
inline constexpr bool z86AddrSharedFuncs::addr_crosses_page(size_t addr) {
    if constexpr (ctx.PAGING) {
        // No paging simulated yet
    }
    return false;
}

inline constexpr size_t regcall z86AddrSharedFuncs::virt_to_phys(size_t addr) {
    // No paging yet
    return addr;
}

template <typename T, typename P>
inline EXCEPTION regcall z86AddrSharedFuncs::write(P* self, const T& value, ssize_t offset) {

    // Can this flag just directly check for protected mode instead?
    if constexpr (!ctx.SINGLE_MEM_WRAPS) {
        // TODO: Check segment limits
        //const auto& descriptor = self->descriptor();

        mem.write<T>(self->addr(offset), value);
    }
    else {
        // 8086 compatiblity
        size_t virt_seg_base = self->seg();
        size_t virt_addr_base = virt_seg_base + self->ptr(offset);
        size_t wrap = self->offset_wrap<T>(offset);
        if (expect(!wrap, true)) {
            mem.write<T>(virt_addr_base, value);
        }
        else {
            if constexpr (sizeof(T) != sizeof(uint16_t)) {
#if USE_NATIVE_INSTRS
                mem.write_movsb(
                    virt_addr_base - self->offset_wrap_sub<T>(wrap),
                    mem.write_movsb(virt_addr_base, &value, wrap),
                    z86DataProperites<T>::size - wrap
                );
#else
                mem.write(virt_addr_base, &value, wrap);
                mem.write(virt_addr_base - self->offset_wrap_sub<T>(wrap), (uint8_t*)&value) + wrap, z86DataProperites<T>::size - wrap);
#endif
            }
            else {
                uint16_t raw = std::bit_cast<uint16_t>(value);
                mem.write<uint8_t>(virt_addr_base, raw);
                mem.write<uint8_t>(virt_addr_base - self->offset_wrap_sub<T>(wrap), raw >> 8);
            }
        }
    }
    return NO_FAULT;
}

template <typename T, typename V, typename P>
inline V z86AddrSharedFuncs::read(const P* self, ssize_t offset) {
    if constexpr (!ctx.SINGLE_MEM_WRAPS) {
        // TODO: Check segment limits
        return mem.read<V>(self->addr(offset));
    }
    else {
        // 8086 compatibility
        size_t virt_seg_base = self->seg();
        size_t virt_addr_base = virt_seg_base + self->ptr(offset);
        size_t wrap = self->offset_wrap<V>(offset);
        if (expect(!wrap, true)) {
            return mem.read<V>(virt_addr_base);
        }
        else {
            if constexpr (sizeof(V) != sizeof(uint16_t)) {
                unsigned char raw[z86DataProperites<V>::size];
#if USE_NATIVE_INSTRS
                mem.read_movsb(
                    mem.read_movsb(raw, virt_addr_base, wrap),
                    virt_addr_base - self->offset_wrap_sub<V>(wrap),
                    z86DataProperites<V>::size - wrap
                );
#else
                mem.read(raw, virt_addr_base, wrap);
                mem.read(&raw[wrap], virt_addr_base - self->offset_wrap_sub<V>(wrap), z86DataProperites<V>::size - wrap);
#endif
                return *(V*)&raw;
            }
            else {
                uint16_t raw;
                raw = mem.read<uint8_t>(virt_addr_base);
                raw |= (uint16_t)mem.read<uint8_t>(virt_addr_base - self->offset_wrap_sub<V>(wrap)) << 8;
                return std::bit_cast<V>(raw);
            }
        }
    }
}

template <typename P>
inline uint32_t z86AddrSharedFuncs::read_Iz(const P* self, ssize_t index) {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            return self->read<uint32_t>(index);
        }
    }
    return self->read<uint16_t>(index);
}

template <typename P>
inline uint32_t z86AddrSharedFuncs::read_advance_Iz(P* self) {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            return self->read_advance<uint32_t>();
        }
    }
    return self->read_advance<uint16_t>();
}

template <typename P>
inline int32_t z86AddrSharedFuncs::read_Is(const P* self, ssize_t index) {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            return self->read<int32_t>(index);
        }
    }
    return self->read<int16_t>(index);
}

template <typename P>
inline int32_t z86AddrSharedFuncs::read_advance_Is(P* self) {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            return self->read_advance<int32_t>();
        }
    }
    return self->read_advance<int16_t>();
}

template <typename P>
inline uint64_t z86AddrSharedFuncs::read_Iv(const P* self, ssize_t index) {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.data_size_64()) {
                    return self->read<uint64_t>(index);
                }
            }
            return self->read<uint32_t>(index);
        }
    }
    return self->read<uint16_t>(index);
}

template <typename P>
inline uint64_t z86AddrSharedFuncs::read_advance_Iv(P* self) {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.data_size_64()) {
                    return self->read_advance<uint64_t>();
                }
            }
            return self->read_advance<uint32_t>();
        }
    }
    return self->read_advance<uint16_t>();
}


template <typename P>
inline uint64_t z86AddrSharedFuncs::read_O(const P* self, ssize_t index) {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.addr_size_16()) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.addr_size_64()) {
                    return self->read<uint64_t>(index);
                }
            }
            return self->read<uint32_t>(index);
        }
    }
    return self->read<uint16_t>(index);
}

template <typename P>
inline uint64_t z86AddrSharedFuncs::read_advance_O(P* self) {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.addr_size_16()) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.addr_size_64()) {
                    return self->read_advance<uint64_t>();
                }
            }
            return self->read_advance<uint32_t>();
        }
    }
    return self->read_advance<uint16_t>();
}

template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::LEA_impl(P& pc) {
    ModRM modrm = pc.read_advance<ModRM>();
    if (modrm.is_mem()) {
        z86Addr addr = modrm.parse_memM(pc);
        this->index_regR<T>(modrm.R()) = addr.offset;
    }
    else {
        if constexpr (NO_UD) {
            // TODO: jank
        }
        else {
            this->set_fault(IntUD);
            return FAULT;
        }
    }
    return NO_FAULT;
}

template <z86BaseTemplate>
template <uint8_t op_flags, typename T1, typename T2, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopMR_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T2& rval = this->index_regR<T2, OP_IGNORES_REX(op_flags)>(modrm.R());
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        T1 mval;
        if constexpr (!OP_SKIPS_READ(op_flags)) {
            mval = data_addr.read<T1>();
        }
        if (lambda(mval, rval)) {
            data_addr.write<T1>(mval);
        }
    }
    else {
        lambda(this->index_regMB<T1, OP_IGNORES_REX(op_flags)>(modrm.M()), rval);
    }
    return NO_FAULT;
}

template <z86BaseTemplate>
template <uint8_t op_flags, typename T1, typename T2, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopRM_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T2 mval;
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        mval = data_addr.read<T2>();
    }
    else {
        mval = this->index_regMB<T2, OP_IGNORES_REX(op_flags)>(modrm.M());
    }
    lambda(this->index_regR<T1, OP_IGNORES_REX(op_flags)>(modrm.R()), mval);
    return NO_FAULT;
}

template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopRMW_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint16_t mval;
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        mval = data_addr.read<uint16_t>();
    }
    else {
        mval = this->index_word_regMB<OP_IGNORES_REX(op_flags)>(modrm.M());
    }
    return lambda(this->index_regR<T, OP_IGNORES_REX(op_flags)>(modrm.R()), mval);
}

// Bit test memory operand
template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopMRB_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T rval = this->index_regR<T, OP_IGNORES_REX(op_flags)>(modrm.R());
    T mask = rval & bitsof(T) - 1;
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        data_addr.offset += sizeof(T) * (rval >> std::bit_width(bitsof(T) - 1));
        T mval;
        if constexpr (!OP_SKIPS_READ(op_flags)) {
            mval = data_addr.read<T>();
        }
        if (lambda(mval, mask)) {
            data_addr.write<T>(mval);
        }
    }
    else {
        lambda(this->index_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M()), mask);
    }
    return NO_FAULT;
}

// Bit string memory operand
template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::IBTS_impl(P& pc) {
    using DT = dbl_int_t<T>;

    ModRM modrm = pc.read_advance<ModRM>();
    T rval = this->index_regR<T, OP_IGNORES_REX(op_flags)>(modrm.R());
    T aval = this->A<T>();
    uint8_t offset = aval & bitsof(T) - 1;
    uint8_t count = this->cl;
    if constexpr (sizeof(T) < sizeof(uint64_t)) {
        count &= 0x1F;
    }
    else {
        count &= 0x3F;
    }
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        data_addr.offset += sizeof(T) * (aval >> std::bit_width(bitsof(T) - 1));
        DT mval = data_addr.read<T>();
        uint8_t overflow_bits = offset + count;
        if (overflow_bits > bitsof(T)) {
            mval |= (DT)data_addr.read<T>(sizeof(T)) << bitsof(T);
        }
        this->IBTS(mval, rval, offset, count);
        data_addr.write<T>(mval);
        if (overflow_bits > bitsof(T)) {
            data_addr.write<T>(mval >> bitsof(T), sizeof(T));
        }
    }
    else {
        this->IBTS(this->index_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M()), rval, offset, count);
    }
    return NO_FAULT;
}

template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::XBTS_impl(P& pc) {
    ModRM modrm = pc.read_advance<ModRM>();
    T aval = this->A<T>();
    int8_t offset = aval & bitsof(T) - 1;
    uint8_t count = this->cl;
    if constexpr (sizeof(T) < sizeof(uint64_t)) {
        count &= 0x1F;
    }
    else {
        count &= 0x3F;
    }
    T mval;
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        data_addr.offset += sizeof(T) * (aval >> std::bit_width(bitsof(T) - 1));
        mval = data_addr.read<T>() >> offset;
        offset += count;
        offset -= bitsof(T);
        if (offset > 0) {
            mval |= data_addr.read<T>(sizeof(T)) << offset;
        }
    }
    else {
        mval = this->index_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M());
        mval >>= offset;
    }
    this->XBTS(this->index_regR<T, OP_IGNORES_REX(op_flags)>(modrm.R()), mval, count);
    return NO_FAULT;
}

// Far width memory operand, special for LDS/LES
template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopRMF_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = this->index_regR<T, OP_IGNORES_REX(op_flags)>(modrm.R());
    uint8_t ret = 0;
    if (modrm.is_mem()) {
        using DT = dbl_int_t<T>;
        z86Addr data_addr = modrm.parse_memM(pc);
        DT temp = data_addr.read<T>();
        temp |= (DT)data_addr.read<uint16_t>(sizeof(T)) << bitsof(T);
        ret = lambda(rval, temp);
    }
    else {
        if constexpr (NO_UD) {
            // TODO: jank
        }
        else {
            this->set_fault(IntUD);
            return FAULT;
        }
    }
    return OP_HAD_FAULT(ret);
}

// Double width memory operand, special for BOUND
template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopRM2_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = this->index_regR<T, OP_IGNORES_REX(op_flags)>(modrm.R());
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        return lambda(rval, data_addr.read<T>(), data_addr.read<T>(sizeof(T)));
    }
    else {
        if constexpr (NO_UD) {
            // TODO: jank
        }
        else {
            this->set_fault(IntUD);
            return FAULT;
        }
    }
    return NO_FAULT;
}

template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopMS_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint16_t rval = this->get_seg(modrm.R());
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        T mval;
        if constexpr (!OP_SKIPS_READ(op_flags)) {
            mval = data_addr.read<T>();
        }
        if (lambda(mval, rval)) {
            data_addr.write<T>(mval);
        }
    }
    else {
        lambda(this->index_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M()), rval);
    }
    return NO_FAULT;
}

template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopSM_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T mval;
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        mval = data_addr.read<T>();
    }
    else {
        mval = this->index_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M());
    }
    uint8_t seg_index = modrm.R();
    uint16_t rval;
    if constexpr (!OP_SKIPS_READ(op_flags)) {
        rval = this->get_seg(seg_index);
    }
    lambda(rval, mval);
    return this->write_seg(seg_index, rval);
}

template <z86BaseTemplate>
template <typename T, uint8_t op_flags, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopMR_MM(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    MMXT<T>& rval = this->index_mmx_reg<T>(modrm.R());
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        MMXT<T> mval;
        if constexpr (!OP_SKIPS_READ(op_flags)) {
            mval = data_addr.read<MMXT<T>>();
        }
        if (lambda(mval, rval)) {
            data_addr.write<MMXT<T>>(mval);
        }
    }
    else {
        lambda(this->index_mmx_reg<T>(modrm.M()), rval);
    }
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T, uint8_t op_flags, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopRM_MM(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    MMXT<T> mval;
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        mval = data_addr.read<MMXT<T>>();
    }
    else {
        mval = this->index_mmx_reg<T>(modrm.M());
    }
    lambda(this->index_mmx_reg<T>(modrm.R()), mval);
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T, uint8_t op_flags, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopMR_XX(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    SSET<T>& rval = this->index_xmm_regR<T, OP_IGNORES_REX(op_flags)>(modrm.R());
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        SSET<T> mval;
        if constexpr (!OP_SKIPS_READ(op_flags)) {
            mval = data_addr.read<SSET<T>>();
        }
        if (lambda(mval, rval)) {
            data_addr.write<SSET<T>>(mval);
        }
    }
    else {
        lambda(this->index_xmm_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M()), rval);
    }
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T, uint8_t op_flags, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopRM_XX(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    SSET<T> mval;
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        mval = data_addr.read<SSET<T>>();
    }
    else {
        mval = this->index_xmm_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M());
    }
    lambda(this->index_xmm_regR<T, OP_IGNORES_REX(op_flags)>(modrm.R()), mval);
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T, uint8_t op_flags, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::binopRM_MX(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    SSET<T> mval;
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        mval = { data_addr.read<T>() };
    }
    else {
        mval = this->index_xmm_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M());
    }
    lambda(this->index_mmx_reg<T>(modrm.R()), mval);
    return NO_FAULT;
}

template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::unopR_impl(P& pc, uint8_t reg, const L& lambda) {
    return OP_HAD_FAULT(lambda(this->index_regMB<T, OP_IGNORES_REX(op_flags)>(reg)));
}

template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::unopM_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint8_t r = modrm.R();
    uint8_t ret;
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        T mval;
        if constexpr (!OP_SKIPS_READ(op_flags)) {
            mval = data_addr.read<T>();
        }
        ret = lambda(mval, r);
        if (OP_NEEDS_WRITE(ret)) {
            data_addr.write<T>(mval);
        }
    }
    else {
        ret = lambda(this->index_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M()), r);
    }
    if constexpr (FAULTS_ARE_TRAPS) {
        return NO_FAULT;
    }
    return OP_HAD_FAULT(ret);
}

template <z86RegCommonTemplate>
template <typename P, typename T>
inline EXCEPTION regcall z86RegCommonDefault::PUSH_impl(const T& src) {
    this->SP<P>() -= (std::max)(sizeof(T), (size_t)2);
    z86AddrSS stack = this->stack<P>();
    stack.write(src);
    return NO_FAULT;
}

template <z86RegCommonTemplate>
template <typename P, typename R, typename T>
inline EXCEPTION z86RegCommonDefault::POP_impl(T& dst) {
    using SMALLER_T = std::conditional_t<sizeof(T) <= sizeof(R), T, R>;
    
    z86AddrSS stack = this->stack<P>();
    dst = stack.read<SMALLER_T>();
    this->SP<P>() += (std::max)(sizeof(R), (size_t)2);
    return NO_FAULT;
}

// No wonder ENTER sucks
template <z86BaseTemplate>
template <typename T>
gnu_attr(minsize) inline EXCEPTION regcall z86BaseDefault::ENTER_impl(uint16_t alloc, uint8_t nesting) {
    if constexpr (sizeof(T) == sizeof(uint64_t)) {
        uint64_t cur_bp = this->rbp;
        uint64_t new_bp = this->rsp - sizeof(T);
        this->PUSH_impl<uint64_t>(cur_bp);
        switch (nesting) {
            default: {
                z86Addr stack_bp(this->ss, cur_bp);
                do {
                    stack_bp.offset -= sizeof(T); // sizeof(T) = 8
                    this->PUSH_impl<uint64_t>(stack_bp.read<T>());
                } while (--nesting != 1);
            }
            case 1:
                this->PUSH(new_bp);
            case 0:
                this->rbp = new_bp;
                this->rsp -= alloc;
        }
    }
    else if constexpr (sizeof(T) == sizeof(uint32_t)) {
        uint32_t cur_bp = this->ebp;
        uint32_t new_bp = this->esp - sizeof(T);
        if (expect(this->stack_size_32(), true)) {
            this->PUSH_impl<uint32_t>(cur_bp);
            switch (nesting) {
                default: {
                    z86Addr stack_bp(this->ss, cur_bp);
                    do {
                        stack_bp.offset -= sizeof(T); // sizeof(T) = 4
                        this->PUSH_impl<uint32_t>(stack_bp.read<T>());
                    } while (--nesting != 1);
                }
                case 1:
                    this->PUSH_impl<uint32_t>(new_bp);
                case 0:;
            }
        }
        else {
            this->PUSH_impl<uint16_t>(cur_bp);
            switch (nesting) {
                default: {
                    z86Addr stack_bp(this->ss, (uint16_t)cur_bp);
                    do {
                        stack_bp.offset -= sizeof(T); // sizeof(T) = 4
                        this->PUSH_impl<uint16_t>(stack_bp.read<T>());
                    } while (--nesting != 1);
                }
                case 1:
                    this->PUSH_impl<uint16_t>(new_bp);
                case 0:
                    break;
            }
        }
        this->ebp = new_bp;
        this->esp -= alloc;
    }
    else {
        uint16_t cur_bp = this->bp;
        uint16_t new_bp = this->sp - sizeof(T);
        if constexpr (bits > 16) {
            if constexpr (bits == 64) {
                if (expect(this->stack_size_64(), false)) {
                    this->PUSH_impl<uint64_t>(cur_bp);
                    switch (nesting) {
                        default: {
                            z86Addr stack_bp(this->ss, this->rbp);
                            do {
                                stack_bp.offset -= sizeof(T); // sizeof(T) = 2
                                this->PUSH_impl<uint64_t>(stack_bp.read<T>());
                            } while (--nesting != 1);
                        }
                        case 1:
                            this->PUSH_impl<uint64_t>(new_bp);
                        case 0:
                            goto end_enter16;
                    }
                }
            }
            if (expect(this->stack_size_32(), false)) {
                this->PUSH_impl<uint32_t>(cur_bp);
                switch (nesting) {
                    default: {
                        z86Addr stack_bp(this->ss, this->ebp);
                        do {
                            stack_bp.offset -= sizeof(T); // sizeof(T) = 2
                            this->PUSH_impl<uint32_t>(stack_bp.read<T>());
                        } while (--nesting != 1);
                    }
                    case 1:
                        this->PUSH_impl<uint32_t>(new_bp);
                    case 0:
                        goto end_enter16;
                }
            }
        }
        this->PUSH_impl<uint16_t>(cur_bp);
        switch (nesting) {
            default: {
                z86Addr stack_bp(this->ss, cur_bp);
                do {
                    stack_bp.offset -= sizeof(T); // sizeof(T) = 2
                    this->PUSH_impl<uint16_t>(stack_bp.read<T>());
                } while (--nesting != 1);
            }
            case 1:
                this->PUSH_impl<uint16_t>(new_bp);
            case 0:
                break;
        }
    end_enter16:
        this->bp = new_bp;
        this->sp -= alloc;
    }
    return NO_FAULT;
}

// TODO: Check what happens if an interrupt toggles 
// the direction flag during a repeating string instruction
template <z86BaseTemplate>
template <typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::LODS_impl() {
    z86Addr src_addr = this->str_src<P>();
    intptr_t offset = this->direction ? -sizeof(T) : sizeof(T);
    if (expect(this->has_rep(), false)) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
                this->A<T>() = src_addr.read_advance<T>(offset);
            } while (--this->C<P>());
        }
        else {
            return NO_FAULT;
        }
    }
    else {
        this->A<T>() = src_addr.read_advance<T>(offset);
    }
    this->SI<P>() = src_addr.offset;
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::MOVS_impl() {
    z86Addr src_addr = this->str_src<P>();
    z86AddrES dst_addr = this->str_dst<P>();
    intptr_t offset = this->direction ? -sizeof(T) : sizeof(T);
    if (this->has_rep()) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
                dst_addr.write_advance<T>(src_addr.read_advance<T>(offset), offset);
            } while (--this->C<P>());
        }
        else {
            return NO_FAULT;
        }
    }
    else {
        dst_addr.write_advance<T>(src_addr.read_advance<T>(offset), offset);
    }
    this->SI<P>() = src_addr.offset;
    this->DI<P>() = dst_addr.offset;
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::STOS_impl() {
    z86AddrES dst_addr = this->str_dst<P>();
    intptr_t offset = this->direction ? -sizeof(T) : sizeof(T);
    if (this->has_rep()) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
                dst_addr.write_advance<T>(this->A<T>(), offset);
            } while (--this->C<P>());
        }
        else {
            return NO_FAULT;
        }
    }
    else {
        dst_addr.write_advance<T>(this->A<T>(), offset);
    }
    this->DI<P>() = dst_addr.offset;
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::SCAS_impl() {
    z86AddrES dst_addr = this->str_dst<P>();
    intptr_t offset = this->direction ? -sizeof(T) : sizeof(T);
    if (this->has_rep()) {
        if (this->C<P>()) {
            if constexpr (OPCODES_V20 && !OPCODES_80386) {
                if (this->is_repc()) {
                    do {
                        // TODO: Interrupt check here
                        this->CMP<T>(this->A<T>(), dst_addr.read_advance<T>(offset));
                    } while (--this->C<P>() && this->rep_type == this->carry + 2);
                    goto finish;
                }
            }
            do {
                // TODO: Interrupt check here
                this->CMP<T>(this->A<T>(), dst_addr.read_advance<T>(offset));
            } while (--this->C<P>() && this->rep_type == this->zero);
        }
        else {
            return NO_FAULT;
        }
    }
    else {
        this->CMP<T>(this->A<T>(), dst_addr.read_advance<T>(offset));
    }
finish:
    this->DI<P>() = dst_addr.offset;
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::CMPS_impl() {
    z86Addr src_addr = this->str_src<P>();
    z86AddrES dst_addr = this->str_dst<P>();
    intptr_t offset = this->direction ? -sizeof(T) : sizeof(T);
    if (this->has_rep()) {
        if (this->C<P>()) {
            if constexpr (OPCODES_V20 && !OPCODES_80386) {
                if (this->is_repc()) {
                    do {
                        // TODO: Interrupt check here
                        this->CMP<T>(src_addr.read_advance<T>(offset), dst_addr.read_advance<T>(offset));
                    } while (--this->C<P>() && this->rep_type == this->carry + 2);
                    goto finish;
                }
            }
            do {
                // TODO: Interrupt check here
                this->CMP<T>(src_addr.read_advance<T>(offset), dst_addr.read_advance<T>(offset));
            } while (--this->C<P>() && this->rep_type == this->zero);
        }
        else {
            return NO_FAULT;
        }
    }
    else {
        this->CMP<T>(src_addr.read_advance<T>(offset), dst_addr.read_advance<T>(offset));
    }
finish:
    this->SI<P>() = src_addr.offset;
    this->DI<P>() = dst_addr.offset;
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename P>
inline EXCEPTION regcall z86BaseDefault::ADD4S_impl() {
    // How does this actually behave on real hardware?
    z86Addr src_addr = this->str_src<P>();
    z86AddrES dst_addr = this->str_dst<P>();
    // Yes, this overflows ridiculously for 0 and 255
    uint16_t count = (uint8_t)(this->cl + 1) >> 1;
    while (--count) {

    }
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::OUTS_impl() {
    z86Addr src_addr = this->str_src<P>();
    intptr_t offset = this->direction ? -sizeof(T) : sizeof(T);
    uint16_t port = this->dx;
    if (this->has_rep()) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
                this->port_out_impl<T>(port, src_addr.read_advance<T>(offset));
            } while (--this->C<P>());
        }
        else {
            return NO_FAULT;
        }
    }
    else {
        this->port_out_impl<T>(port, src_addr.read_advance<T>(offset));
    }
    this->SI<P>() = src_addr.offset;
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::INS_impl() {
    z86AddrES dst_addr = this->str_dst<P>();
    intptr_t offset = this->direction ? -sizeof(T) : sizeof(T);
    uint16_t port = this->dx;
    if (this->has_rep()) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
                dst_addr.write_advance<T>(this->port_in_impl<T>(port), offset);
            } while (--this->C<P>());
        }
        else {
            return NO_FAULT;
        }
    }
    else {
        dst_addr.write_advance<T>(this->port_in_impl<T>(port), offset);
    }
    this->DI<P>() = dst_addr.offset;
    return NO_FAULT;
}

template <z86BaseTemplate>
template <typename T>
inline void regcall z86BaseDefault::port_out_impl(uint16_t port, T value) const {
    uint32_t full_port = port;

    if constexpr (sizeof(T) == sizeof(uint8_t)) {
        const std::vector<PortByteDevice*>& devices = io_byte_devices;
        for (auto device : devices) {
            if (device->out_byte(full_port, value)) {
                return;
            }
        }
        printf("Unhandled: OUT 0x%X, 0x%02X\n", port, value);
    }
    else if constexpr (sizeof(T) == sizeof(uint16_t)) {
        const std::vector<PortWordDevice*>& devices = io_word_devices;
        for (auto device : devices) {
            if constexpr (bus >= 16) {
                if (
                    is_aligned<uint16_t>(full_port) &&
                    device->out_word(full_port, value)
                ) {
                    return;
                }
            }
            if (
                device->out_byte(full_port, value) &&
                device->out_byte(full_port + 1, value >> 8)
            ) {
                return;
            }
        }
        printf("Unhandled: OUT 0x%X, 0x%04X\n", port, value);
    }
    else if constexpr (sizeof(T) == sizeof(uint32_t)) {
        const std::vector<PortDwordDevice*>& devices = io_dword_devices;
        for (auto device : devices) {
            if constexpr (bus >= 32) {
                if (
                    is_aligned<uint32_t>(full_port) &&
                    device->out_dword(full_port, value)
                ) {
                    return;
                }
            }
            if constexpr (bus >= 16) {
                if (
                    is_aligned<uint16_t>(full_port) &&
                    device->out_word(full_port, value) &&
                    device->out_word(full_port + 2, value >> 16)
                ) {
                    return;
                }
            }
            if (
                device->out_byte(full_port, value) &&
                device->out_byte(full_port + 1, value >> 8) &&
                device->out_byte(full_port + 2, value >> 16) &&
                device->out_byte(full_port + 3, value >> 24)
            ) {
                return;
            }
        }
        printf("Unhandled: OUT 0x%X, 0x%08X\n", port, value);
    }
}

template <z86BaseTemplate>
template <typename T>
inline T regcall z86BaseDefault::port_in_impl(uint16_t port) {
    uint32_t full_port = port;

    T value;

    if constexpr (sizeof(T) == sizeof(uint8_t)) {
        const std::vector<PortByteDevice*>& devices = io_byte_devices;
        for (auto device : devices) {
            if (device->in_byte(value, full_port)) {
                return value;
            }
        }
        printf("Unhandled: IN AL, 0x%X\n", full_port);
    }
    else if constexpr (sizeof(T) == sizeof(uint16_t)) {
        const std::vector<PortWordDevice*>& devices = io_word_devices;
        for (auto device : devices) {
            if constexpr (bus >= 16) {
                if (
                    is_aligned<uint16_t>(full_port) &&
                    device->in_word(value, full_port)
                ) {
                    return value;
                }
            }
            if (
                device->in_byte(((uint8_t*)&value)[0], full_port) &&
                device->in_byte(((uint8_t*)&value)[1], full_port + 1)
            ) {
                return value;
            }
        }
        printf("Unhandled: IN AX, 0x%X\n", full_port);
    }
    else if constexpr (sizeof(T) == sizeof(uint32_t)) {
        const std::vector<PortDwordDevice*>& devices = io_dword_devices;
        for (auto device : devices) {
            if constexpr (bus >= 32) {
                if (
                    is_aligned<uint32_t>(full_port) &&
                    device->in_dword(value, full_port)
                ) {
                    return value;
                }
            }
            if constexpr (bus >= 16) {
                if (
                    is_aligned<uint16_t>(full_port) &&
                    device->in_word(((uint16_t*)&value)[0], full_port) &&
                    device->in_word(((uint16_t*)&value)[1], full_port + 2)
                ) {
                    return value;
                }
            }
            if (
                device->in_byte(((uint8_t*)&value)[0], full_port) &&
                device->in_byte(((uint8_t*)&value)[1], full_port + 1) &&
                device->in_byte(((uint8_t*)&value)[2], full_port + 2) &&
                device->in_byte(((uint8_t*)&value)[3], full_port + 3)
            ) {
                return value;
            }
        }
        printf("Unhandled: IN EAX, 0x%X\n", full_port);
    }
    return 0;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline EXCEPTION regcall z86BaseDefault::MASKMOV_impl(T& src, T mask) {
    z86Addr data_addr = this->addr(DS, this->DI<P>());

    uint8_t* data_vec = (uint8_t*)&src;

    // TODO: Get the compiler to generate PMOVMSKB for this
    for (size_t i = 0; i < sizeof(T); ++i) {
        if (mask[i] & 0x80) {
            data_addr.write<uint8_t>(data_vec[i], i);
        }
    }
    return NO_FAULT;
}

// Special unop for groups 4/5
template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P>
inline bool regcall z86BaseDefault::unopMS_impl(P& pc) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint8_t r = modrm.R();
    T mval;
    uint16_t sval = 0; // TODO: jank
    z86Addr data_addr;
    if (modrm.is_mem()) {
        data_addr = modrm.parse_memM(pc);
        mval = data_addr.read<T>();
        switch (r) {
            case 0:
                ctx.INC(mval);
                break;
            case 1:
                ctx.DEC(mval);
                break;
            case 2:
            
            call:
                ctx.CALLABS<T>(pc.offset, mval);
                return true;
            case 3:
                sval = data_addr.read<uint16_t>(sizeof(T)); // TODO: Byte offset?
            callf:
                ctx.CALLFABS<T>(pc.offset, mval, sval);
                return true;
            case 4: jmp:
                ctx.JMPABS(mval);
                return true;
            case 5:
                sval = data_addr.read<uint16_t>(sizeof(T)); // TODO: Byte offset?
            jmpf:
                ctx.JMPFABS(mval, sval);
                return true;
            case 6: case 7: push:
                ctx.PUSH(mval);
                return false;
            default:
                unreachable;
        }
        data_addr.write<T>(mval);
    }
    else {
        T& mval_ref = ctx.index_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M());
        mval = mval_ref;
        switch (r) {
            case 0:
                ctx.INC(mval_ref);
                break;
            case 1:
                ctx.DEC(mval_ref);
                break;
            case 2:
                goto call;
            case 3:
                // TODO: jank
                goto callf;
            case 4:
                goto jmp;
            case 5:
                // TODO: Jank
                goto jmpf;
            case 6: case 7: 
                goto push;
            default:
                unreachable;
        }
    }
    return false;
}

template <z86BaseTemplate>
void z86BaseDefault::LOADALL2() {
    const z86LoadallFrame80286& loadall_data = mem.ref<z86LoadallFrame80286>(0x800);
    this->set_machine_status_word(loadall_data.msw);
    this->seg[TSS] = loadall_data.tr;
    this->set_flags<uint16_t, true>(loadall_data.flags);
    this->ip = loadall_data.ip;
    this->seg[LDT] = loadall_data.ldtr;
    this->ds = loadall_data.ds;
    this->ss = loadall_data.ss;
    this->cs = loadall_data.cs;
    this->es = loadall_data.es;
    this->di = loadall_data.di;
    this->si = loadall_data.si;
    this->bp = loadall_data.bp;
    this->sp = loadall_data.sp;
    this->bx = loadall_data.bx;
    this->dx = loadall_data.dx;
    this->cx = loadall_data.cx;
    this->ax = loadall_data.ax;
    this->load_descriptor(ES, loadall_data.es_descriptor, loadall_data.es_descriptor.attributes<false>());
    this->load_descriptor(CS, loadall_data.cs_descriptor, loadall_data.cs_descriptor.attributes<true>());
    this->set_privilege_level((loadall_data.ss_descriptor.attributes_field & 0b1100000) >> 5);
    this->load_descriptor(SS, loadall_data.ss_descriptor, loadall_data.ss_descriptor.attributes<false>());
    this->load_descriptor(DS, loadall_data.ds_descriptor, loadall_data.ds_descriptor.attributes<false>());
    this->load_descriptor(GDT, loadall_data.gdt_descriptor, loadall_data.gdt_descriptor.attributes<false>());
    this->load_descriptor(LDT, loadall_data.ldt_descriptor, loadall_data.ldt_descriptor.attributes<false>());
    this->load_descriptor(IDT, loadall_data.idt_descriptor, loadall_data.idt_descriptor.attributes<false>());
    this->load_descriptor(TSS, loadall_data.tss_descriptor, loadall_data.tss_descriptor.attributes<false>());
}

template <z86BaseTemplate>
void z86BaseDefault::STOREALL() {
    z86LoadallFrame80286& loadall_data = mem.ref<z86LoadallFrame80286>(0x800);
    loadall_data.msw = ctx.get_machine_status_word();
    loadall_data.tr = this->seg[TSS];
    loadall_data.flags = this->get_flags<uint16_t>();
    loadall_data.ip = this->ip; // Is this supposed to be after or before the instruction?
    loadall_data.ldtr = this->seg[LDT];
    loadall_data.ds = this->ds;
    loadall_data.ss = this->ss;
    loadall_data.cs = this->cs;
    loadall_data.es = this->es;
    loadall_data.di = this->di;
    loadall_data.si = this->si;
    loadall_data.bp = this->bp;
    loadall_data.sp = this->sp;
    loadall_data.bx = this->bx;
    loadall_data.dx = this->dx;
    loadall_data.cx = this->cx;
    loadall_data.ax = this->ax;
    this->fill_descriptor(ES, loadall_data.es_descriptor);
    this->fill_descriptor(CS, loadall_data.cs_descriptor);
    this->fill_descriptor(SS, loadall_data.ss_descriptor);
    this->fill_descriptor(DS, loadall_data.ds_descriptor);
    this->fill_descriptor(GDT, loadall_data.gdt_descriptor);
    this->fill_descriptor(LDT, loadall_data.ldt_descriptor);
    this->fill_descriptor(IDT, loadall_data.idt_descriptor);
    this->fill_descriptor(TSS, loadall_data.tss_descriptor);
}

template <z86BaseTemplate>
EXCEPTION z86BaseDefault::LOADALL3() {
    z86AddrES load_addr = this->str_dst();
    this->set_control_reg(CR0, load_addr.read<uint32_t>());
    this->set_flags<uint32_t, true>(load_addr.read<uint32_t>(4));
    this->eip = load_addr.read<uint32_t>(8);
    this->edi = load_addr.read<uint32_t>(0xC);
    this->esi = load_addr.read<uint32_t>(0x10);
    this->ebp = load_addr.read<uint32_t>(0x14);
    this->esp = load_addr.read<uint32_t>(0x18);
    this->ebx = load_addr.read<uint32_t>(0x1C);
    this->edx = load_addr.read<uint32_t>(0x20);
    this->ecx = load_addr.read<uint32_t>(0x24);
    this->eax = load_addr.read<uint32_t>(0x28);
    //this->dr[DR6] = load_addr.read<uint32_t>(0x2C);
    //this->dr[DR7] = load_addr.read<uint32_t>(0x30);
    this->seg[TSS] = load_addr.read<uint16_t>(0x34);
    this->seg[LDT] = load_addr.read<uint16_t>(0x38);
    this->seg[GS] = load_addr.read<uint16_t>(0x3C);
    this->seg[FS] = load_addr.read<uint16_t>(0x40);
    this->ds = load_addr.read<uint16_t>(0x44);
    this->ss = load_addr.read<uint16_t>(0x48);
    this->cs = load_addr.read<uint16_t>(0x4C);
    this->es = load_addr.read<uint16_t>(0x50);
    
    uint16_t temp = z86DescriptorCache80386::make_full_attributes<false>(__builtin_bswap16(load_addr.read<uint16_t>(0x55)));
    this->load_descriptor(TSS, load_addr.read<uint32_t>(0x58), load_addr.read<uint32_t>(0x5C), temp);
    temp = z86DescriptorCache80386::make_full_attributes<false>(__builtin_bswap16(load_addr.read<uint16_t>(0x61)));
    this->load_descriptor(IDT, load_addr.read<uint32_t>(0x64), load_addr.read<uint32_t>(0x68), temp);
    temp = z86DescriptorCache80386::make_full_attributes<false>(__builtin_bswap16(load_addr.read<uint16_t>(0x6D)));
    this->load_descriptor(GDT, load_addr.read<uint32_t>(0x70), load_addr.read<uint32_t>(0x74), temp);
    temp = z86DescriptorCache80386::make_full_attributes<false>(__builtin_bswap16(load_addr.read<uint16_t>(0x79)));
    this->load_descriptor(LDT, load_addr.read<uint32_t>(0x7C), load_addr.read<uint32_t>(0x80), temp);
    temp = z86DescriptorCache80386::make_full_attributes<false>(__builtin_bswap16(load_addr.read<uint16_t>(0x85)));
    this->load_descriptor(GS, load_addr.read<uint32_t>(0x88), load_addr.read<uint32_t>(0x8C), temp);
    temp = z86DescriptorCache80386::make_full_attributes<false>(__builtin_bswap16(load_addr.read<uint16_t>(0x91)));
    this->load_descriptor(FS, load_addr.read<uint32_t>(0x94), load_addr.read<uint32_t>(0x98), temp);
    temp = z86DescriptorCache80386::make_full_attributes<false>(__builtin_bswap16(load_addr.read<uint16_t>(0x9D)));
    this->load_descriptor(DS, load_addr.read<uint32_t>(0xA0), load_addr.read<uint32_t>(0xA4), temp);
    temp = z86DescriptorCache80386::make_full_attributes<false>(__builtin_bswap16(load_addr.read<uint16_t>(0xA9)));
    this->set_privilege_level((temp & 0b1100000) >> 5);
    this->change_stack_size(temp & 0x4000);
    this->load_descriptor(SS, load_addr.read<uint32_t>(0xAC), load_addr.read<uint32_t>(0xB0), temp);
    temp = z86DescriptorCache80386::make_full_attributes<true>(__builtin_bswap16(load_addr.read<uint16_t>(0xB5)));
    this->change_data_size(temp & 0x4000);
    this->change_addr_size(temp & 0x4000);
    this->load_descriptor(CS, load_addr.read<uint32_t>(0xB8), load_addr.read<uint32_t>(0xBC), temp);
    temp = z86DescriptorCache80386::make_full_attributes<false>(__builtin_bswap16(load_addr.read<uint16_t>(0xC1)));
    this->load_descriptor(ES, load_addr.read<uint32_t>(0xC4), load_addr.read<uint32_t>(0xC8), temp);
    return NO_FAULT;
}

// Special unop for group 6
template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P, typename L>
inline EXCEPTION regcall z86BaseDefault::unopMW_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint8_t r = modrm.R();
    uint8_t ret;
    if (modrm.is_mem()) {
        z86Addr data_addr = modrm.parse_memM(pc);
        T mval;
        if constexpr (!OP_SKIPS_READ(op_flags)) {
            mval = data_addr.read<uint16_t>();
        }
        ret = lambda(mval, r);
        if (OP_NEEDS_WRITE(ret)) {
            data_addr.write<uint16_t>(mval);
        }
    }
    else {
        ret = lambda(this->index_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M()), r);
    }
    if constexpr (FAULTS_ARE_TRAPS) {
        return NO_FAULT;
    }
    return OP_HAD_FAULT(ret);
}

template <z86BaseTemplate>
template <uint8_t op_flags, typename T, typename P, typename LM, typename LR>
inline EXCEPTION regcall z86BaseDefault::unopMM_impl(P& pc, const LM& lambdaM, const LR& lambdaR) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint8_t r = modrm.R();
    uint8_t ret;
    if (modrm.is_mem()) {
        ret = lambdaM(modrm.parse_memM(pc), r);
    }
    else {
        ret = lambdaR(this->index_regMB<T, OP_IGNORES_REX(op_flags)>(modrm.M()), r);
    }
    if constexpr (FAULTS_ARE_TRAPS) {
        return NO_FAULT;
    }
    return OP_HAD_FAULT(ret);
}

#endif