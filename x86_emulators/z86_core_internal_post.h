#pragma once

#ifndef Z86_CORE_INTERNAL_POST_H
#define Z86_CORE_INTERNAL_POST_H 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <limits>
#include <algorithm>
#include <utility>
#include <type_traits>

#include "../zero/util.h"

template <size_t bits>
template <typename T>
inline void z86AddrImpl<bits>::write(const T& value, intptr_t offset) {
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

template <size_t bits>
template <typename T>
inline T z86AddrImpl<bits>::read(intptr_t offset) const {
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

template <size_t bits>
template <typename T, typename P, typename L>
inline void z86Base<bits>::binopMR_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = this->index_reg<T>(modrm.R());
    if (modrm.is_mem()) {
        P data_addr = modrm.parse_memM(pc);
        T mval = data_addr.read<T>();
        if (lambda(mval, rval)) {
            data_addr.write<T>(mval);
        }
    }
    else {
        lambda(this->index_reg<T>(modrm.M()), rval);
    }
}

template <size_t bits>
template <typename T, typename P, typename L>
inline void z86Base<bits>::binopRM_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T mval;
    if (modrm.is_mem()) {
        P data_addr = modrm.parse_memM(pc);
        mval = data_addr.read<T>();
    }
    else {
        mval = this->index_reg<T>(modrm.M());
    }
    lambda(this->index_reg<T>(modrm.R()), mval);
}

// Double width memory operand, special for LDS/LES
template <size_t bits>
template <typename T, typename P, typename L>
inline void z86Base<bits>::binopRM2_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = this->index_reg<T>(modrm.R());
    if (modrm.is_mem()) {
        using DT = dbl_int_t<T>;
        P data_addr = modrm.parse_memM(pc);
        DT temp = data_addr.read<T>();
        temp |= (DT)data_addr.read<T>(sizeof(T)) << bitsof(T);
        lambda(rval, temp);
    }
    else {
        // TODO: jank
    }
}

template <size_t bits>
template <typename T, typename P, typename L>
inline void z86Base<bits>::binopMS_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint16_t& rval = this->index_seg(modrm.R());
    if (modrm.is_mem()) {
        P data_addr = modrm.parse_memM(pc);
        T mval = data_addr.read<T>();
        if (lambda(mval, rval)) {
            data_addr.write<T>(mval);
        }
    }
    else {
        lambda(this->index_reg<T>(modrm.M()), rval);
    }
}

template <size_t bits>
template <typename T, typename P, typename L>
inline void z86Base<bits>::binopSM_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T mval;
    if (modrm.is_mem()) {
        P data_addr = modrm.parse_memM(pc);
        mval = data_addr.read<T>();
    }
    else {
        mval = this->index_reg<T>(modrm.M());
    }
    lambda(this->index_seg(modrm.R()), mval);
}

template <size_t bits>
template <typename T, typename P, typename L>
inline void z86Base<bits>::unopM_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint8_t r = modrm.R();
    if (modrm.is_mem()) {
        P data_addr = modrm.parse_memM(pc);
        T mval = data_addr.read<T>();
        if (lambda(mval, r)) {
            data_addr.write<T>(mval);
        }
    }
    else {
        lambda(this->index_reg<T>(modrm.M()), r);
    }
}

template <size_t bits>
template <typename T>
inline void z86Base<bits>::PUSH(T src) {
    this->SP<T>() -= (std::max)(sizeof(T), (size_t)2);
    z86Addr stack = this->stack();
    stack.write(src);
}

template <size_t bits>
template <typename T>
inline T z86Base<bits>::POP() {
    z86Addr stack = this->stack();
    T ret = stack.read<T>();
    this->SP<T>() += (std::max)(sizeof(T), (size_t)2);
    return ret;
}

// TODO: Check what happens if an interrupt toggles 
// the direction flag during a repeating string instruction
template <size_t bits>
template <typename T, typename P>
inline void z86Base<bits>::LODS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr src_addr = this->str_src<P>();
    if (this->rep_type > NO_REP) {
        while (this->C<P>()) {
            --this->C<P>();
            this->A<T>() = src_addr.read_advance<T>(offset);
        }
    }
    else {
        this->A<T>() = src_addr.read_advance<T>(offset);
    }
    this->SI<P>() = src_addr.offset;
}

template <size_t bits>
template <typename T, typename P>
inline void z86Base<bits>::MOVS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr src_addr = this->str_src<P>();
    z86Addr dst_addr = this->str_dst<P>();
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                dst_addr.write_advance<T>(src_addr.read_advance<T>(offset), offset);
            } while (--this->C<P>());
        }
    }
    else {
        dst_addr.write_advance<T>(src_addr.read_advance<T>(offset), offset);
    }
    this->SI<P>() = src_addr.offset;
    this->DI<P>() = dst_addr.offset;
}

template <size_t bits>
template <typename T, typename P>
inline void z86Base<bits>::STOS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr dst_addr = this->str_dst<P>();
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                dst_addr.write_advance<T>(this->A<T>(), offset);
            } while (--this->C<P>());
        }
    }
    else {
        dst_addr.write_advance<T>(this->A<T>(), offset);
    }
    this->DI<P>() = dst_addr.offset;
}

template <size_t bits>
template <typename T, typename P>
inline void z86Base<bits>::SCAS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr dst_addr = this->str_dst<P>();
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                this->CMP<T>(this->A<T>(), dst_addr.read_advance<T>(offset));
            } while (--this->C<P>() && this->rep_type == this->zero);
        }
    }
    else {
        this->CMP<T>(this->A<T>(), dst_addr.read_advance<T>(offset));
    }
    this->DI<P>() = dst_addr.offset;
}

template <size_t bits>
template <typename T, typename P>
inline void z86Base<bits>::CMPS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr src_addr = this->str_src<P>();
    z86Addr dst_addr = this->str_dst<P>();
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                this->CMP<T>(src_addr.read_advance<T>(offset), dst_addr.read_advance<T>(offset));
            } while (--this->C<P>() && this->rep_type == this->zero);
        }
    }
    else {
        this->CMP<T>(src_addr.read_advance<T>(offset), dst_addr.read_advance<T>(offset));
    }
    this->SI<P>() = src_addr.offset;
    this->DI<P>() = src_addr.offset;
}

// Special unop for groups 4/5
template <size_t bits>
template <typename T, typename P>
inline bool z86Base<bits>::unopMS_impl(P& pc) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint8_t r = modrm.R();
    T mval;
    uint16_t sval = 0; // TODO: jank
    P data_addr;
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
            case 2: call:
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
    } else {
        T& mval_ref = ctx.index_reg<T>(modrm.M());
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

#endif