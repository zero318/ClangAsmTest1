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

template <typename P>
uint32_t ModRM::extra_length(const P& pc) const {
    uint8_t mod = this->Mod();
    if (mod == 3) {
        return 0;
    }
    if constexpr (ctx.max_bits > 16) {
        if (ctx.addr_size <= 0) {
            uint32_t length = 0;
            switch (this->M()) {
                case 4:
                    ++length;
                    if (mod == 0) {
                        return length + (pc.read<SIB>(1).B() == 5) * 4;
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

template <typename P>
auto ModRM::parse_memM(P& pc) const {
    uint8_t default_segment = DS;
    size_t offset = 0;
    uint8_t m = this->M();
    uint8_t mod = this->Mod();
    if constexpr (ctx.max_bits > 16) {
        if (ctx.addr_size <= 0) {
            if constexpr (ctx.max_bits == 64) {
                if (ctx.addr_size < 0) {
                    switch (m) {
                        case 4: {
                            SIB sib = pc.read_advance<SIB>();
                            uint8_t i = sib.I();
                            if (i != 4) {
                                offset = ctx.index_qword_reg(i) * (1 << sib.S());
                            }
                            switch (uint8_t b = sib.B()) {
                                case 5:
                                    if (mod != 0) {
                                        offset += ctx.rbp;
                                        default_segment = SS;
                                    }
                                    break;
                                case 4:
                                    default_segment = SS;
                                default:
                                    offset += ctx.index_qword_reg(b);
                                    break;
                            }
                            break;
                        }
                        case 5:
                            default_segment = SS;
                        default:
                            offset = ctx.index_qword_reg(m);
                    }
                }
                goto add_const;
            }
            switch (m) {
                case 4: {
                    SIB sib = pc.read_advance<SIB>();
                    uint8_t i = sib.I();
                    if (i != 4) {
                        offset = ctx.index_qword_reg(i) * (1 << sib.S());
                    }
                    switch (uint8_t b = sib.B()) {
                        case 5:
                            if (mod != 0) {
                                offset += ctx.rbp;
                                default_segment = SS;
                            }
                            break;
                        case 4:
                            default_segment = SS;
                        default:
                            offset += ctx.index_qword_reg(b);
                            break;
                    }
                    break;
                }
                case 5:
                    default_segment = SS;
                default:
                    offset = ctx.index_dword_reg(m);
            }
        add_const:
            switch (mod) {
                case 0:
                    if (m == 5) {
                        offset += (ssize_t)pc.read_advance<int32_t>();
                        default_segment = DS;
                    }
                    break;
                case 1:
                    offset += pc.read_advance<int8_t>();
                    break;
                case 2:
                    offset += pc.read_advance<int32_t>();
                    break;
                default:
                    unreachable;
            }
            return ctx.addr(default_segment, offset);
        }
    }
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
    switch (mod) {
        case 0:
            if (m == 6) {
                offset = pc.read_advance<int16_t>();
                default_segment = DS;
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

template <size_t bits>
template <typename T>
inline void z86AddrImpl<bits>::write(const T& value, ssize_t offset) {
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
    else if constexpr (sizeof(T) == sizeof(uint32_t)) {
        return mem.write<T>(this->real_addr(offset), value);
    }
    else if constexpr (sizeof(T) == sizeof(uint64_t)) {
        return mem.write<T>(this->real_addr(offset), value);
    }
}

template <size_t bits>
template <typename T>
inline T z86AddrImpl<bits>::read(ssize_t offset) const {
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
    else if constexpr (sizeof(T) == sizeof(uint32_t)) {
        return mem.read<T>(this->real_addr(offset));
    }
    else if constexpr (sizeof(T) == sizeof(uint64_t)) {
        return mem.read<T>(this->real_addr(offset));
    }
}

template <size_t bits>
inline uint32_t z86AddrImpl<bits>::read_Iz(ssize_t index = 0) const {
    if constexpr (ctx.max_bits > 16) {
        if (ctx.data_size <= 0) {
            return this->read<uint32_t>();
        }
    }
    return this->read<uint16_t>();
}

template <size_t bits>
inline uint32_t z86AddrImpl<bits>::read_advance_Iz() {
    if constexpr (ctx.max_bits > 16) {
        if (ctx.data_size <= 0) {
            return this->read_advance<uint32_t>();
        }
    }
    return this->read_advance<uint16_t>();
}

template <size_t bits>
inline int32_t z86AddrImpl<bits>::read_Is(ssize_t index = 0) const {
    if constexpr (ctx.max_bits > 16) {
        if (ctx.data_size <= 0) {
            return this->read<int32_t>();
        }
    }
    return this->read<int16_t>();
}

template <size_t bits>
inline int32_t z86AddrImpl<bits>::read_advance_Is() {
    if constexpr (ctx.max_bits > 16) {
        if (ctx.data_size <= 0) {
            return this->read_advance<int32_t>();
        }
    }
    return this->read_advance<int16_t>();
}

template <size_t bits>
inline uint64_t z86AddrImpl<bits>::read_Iv(ssize_t index = 0) const {
    if constexpr (ctx.max_bits > 16) {
        if (ctx.data_size <= 0) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.data_size < 0) {
                    return this->read<uint64_t>();
                }
            }
            return this->read<uint32_t>();
        }
    }
    return this->read<uint16_t>();
}

template <size_t bits>
inline uint64_t z86AddrImpl<bits>::read_advance_Iv() {
    if constexpr (ctx.max_bits > 16) {
        if (ctx.data_size <= 0) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.data_size < 0) {
                    return this->read_advance<uint64_t>();
                }
            }
            return this->read_advance<uint32_t>();
        }
    }
    return this->read_advance<uint16_t>();
}

template <size_t bits>
inline uint64_t z86AddrImpl<bits>::read_O(ssize_t index = 0) const {
    if constexpr (ctx.max_bits > 16) {
        if (ctx.addr_size <= 0) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.addr_size < 0) {
                    return this->read<uint64_t>();
                }
            }
            return this->read<uint32_t>();
        }
    }
    return this->read<uint16_t>();
}

template <size_t bits>
inline uint64_t z86AddrImpl<bits>::read_advance_O() {
    if constexpr (ctx.max_bits > 16) {
        if (ctx.addr_size <= 0) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.addr_size < 0) {
                    return this->read_advance<uint64_t>();
                }
            }
            return this->read_advance<uint32_t>();
        }
    }
    return this->read_advance<uint16_t>();
}

template <size_t bits, size_t bus>
template <typename T, typename P, typename L>
inline void z86Base<bits, bus>::binopMR_impl(P& pc, const L& lambda) {
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

template <size_t bits, size_t bus>
template <typename T, typename P, typename L>
inline void z86Base<bits, bus>::binopRM_impl(P& pc, const L& lambda) {
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
template <size_t bits, size_t bus>
template <typename T, typename P, typename L>
inline void z86Base<bits, bus>::binopRM2_impl(P& pc, const L& lambda) {
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

template <size_t bits, size_t bus>
template <typename T, typename P, typename L>
inline void z86Base<bits, bus>::binopMS_impl(P& pc, const L& lambda) {
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

template <size_t bits, size_t bus>
template <typename T, typename P, typename L>
inline void z86Base<bits, bus>::binopSM_impl(P& pc, const L& lambda) {
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

template <size_t bits, size_t bus>
template <typename T, typename P, typename L>
inline void z86Base<bits, bus>::unopM_impl(P& pc, const L& lambda) {
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

template <size_t bits, size_t bus>
template <typename T>
inline void z86Base<bits, bus>::PUSH(T src) {
    this->SP<T>() -= (std::max)(sizeof(T), (size_t)2);
    z86Addr stack = this->stack();
    stack.write(src);
}

template <size_t bits, size_t bus>
template <typename T>
inline T z86Base<bits, bus>::POP() {
    z86Addr stack = this->stack();
    T ret = stack.read<T>();
    this->SP<T>() += (std::max)(sizeof(T), (size_t)2);
    return ret;
}

// TODO: Check what happens if an interrupt toggles 
// the direction flag during a repeating string instruction
template <size_t bits, size_t bus>
template <typename T, typename P>
inline void z86Base<bits, bus>::LODS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr src_addr = this->str_src<P>();
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
                this->A<T>() = src_addr.read_advance<T>(offset);
            } while (--this->C<P>());
        }
    }
    else {
        this->A<T>() = src_addr.read_advance<T>(offset);
    }
    this->SI<P>() = src_addr.offset;
}

template <size_t bits, size_t bus>
template <typename T, typename P>
inline void z86Base<bits, bus>::MOVS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr src_addr = this->str_src<P>();
    z86Addr dst_addr = this->str_dst<P>();
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
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

template <size_t bits, size_t bus>
template <typename T, typename P>
inline void z86Base<bits, bus>::STOS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr dst_addr = this->str_dst<P>();
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
                dst_addr.write_advance<T>(this->A<T>(), offset);
            } while (--this->C<P>());
        }
    }
    else {
        dst_addr.write_advance<T>(this->A<T>(), offset);
    }
    this->DI<P>() = dst_addr.offset;
}

template <size_t bits, size_t bus>
template <typename T, typename P>
inline void z86Base<bits, bus>::SCAS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr dst_addr = this->str_dst<P>();
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
                this->CMP<T>(this->A<T>(), dst_addr.read_advance<T>(offset));
            } while (--this->C<P>() && this->rep_type == this->zero);
        }
    }
    else {
        this->CMP<T>(this->A<T>(), dst_addr.read_advance<T>(offset));
    }
    this->DI<P>() = dst_addr.offset;
}

template <size_t bits, size_t bus>
template <typename T, typename P>
inline void z86Base<bits, bus>::CMPS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr src_addr = this->str_src<P>();
    z86Addr dst_addr = this->str_dst<P>();
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
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

template <size_t bits, size_t bus>
template <typename T>
inline void z86Base<bits, bus>::port_out_impl(uint16_t port) const {
    uint32_t full_port = port;

    T value = this->A<T>();

    if constexpr (sizeof(T) == sizeof(uint8_t)) {
        const std::vector<PortByteDevice*>& devices = io_byte_devices;
        for (auto device : devices) {
            if (device->out_byte(full_port, value)) {
                return;
            }
        }
        printf("Unhandled: OUT %X, %02X\n", port, value);
    }
    else if constexpr (sizeof(T) == sizeof(uint16_t)) {
        const std::vector<PortWordDevice*>& devices = io_word_devices;
        for (auto device : devices) {
            if constexpr (bus >= 16) {
                if (
                    !(full_port & 1) &&
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
        printf("Unhandled: OUT %X, %04X\n", port, value);
    }
    else if constexpr (sizeof(T) == sizeof(uint32_t)) {
        const std::vector<PortDwordDevice*>& devices = io_dword_devices;
        for (auto device : devices) {
            if constexpr (bus >= 32) {
                if (
                    !(full_port & 3) &&
                    device->out_dword(full_port, value)
                ) {
                    return;
                }
            }
            if constexpr (bus >= 16) {
                if (
                    !(full_port & 1) &&
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
        printf("Unhandled: OUT %X, %08X\n", port, value);
    }
}

template <size_t bits, size_t bus>
template <typename T>
inline void z86Base<bits, bus>::port_in_impl(uint16_t port) {
    uint32_t full_port = port;

    T value = 0;

    if constexpr (sizeof(T) == sizeof(uint8_t)) {
        const std::vector<PortByteDevice*>& devices = io_byte_devices;
        for (auto device : devices) {
            if (device->in_byte(value, full_port)) {
                this->A<T>() = value;
                return;
            }
        }
        printf("Unhandled: IN AL, %X\n", full_port);
    }
    else if constexpr (sizeof(T) == sizeof(uint16_t)) {
        const std::vector<PortWordDevice*>& devices = io_word_devices;
        for (auto device : devices) {
            if constexpr (bus >= 16) {
                if (
                    !(full_port & 1) &&
                    device->in_word(value, full_port)
                ) {
                    this->A<T>() = value;
                    return;
                }
            }
            if (
                device->in_byte(((uint8_t*)&value)[0], full_port) &&
                device->in_byte(((uint8_t*)&value)[1], full_port + 1)
            ) {
                this->A<T>() = value;
                return;
            }
        }
        printf("Unhandled: IN AX, %X\n", full_port);
    }
    else if constexpr (sizeof(T) == sizeof(uint32_t)) {
        const std::vector<PortDwordDevice*>& devices = io_dword_devices;
        for (auto device : devices) {
            if constexpr (bus >= 32) {
                if (
                    !(full_port & 3) &&
                    device->in_dword(value, full_port)
                ) {
                    this->A<T>() = value;
                    return;
                }
            }
            if constexpr (bus >= 16) {
                if (
                    !(full_port & 1) &&
                    device->in_word(((uint16_t*)&value)[0], full_port) &&
                    device->in_word(((uint16_t*)&value)[1], full_port + 2)
                ) {
                    this->A<T>() = value;
                    return;
                }
            }
            if (
                device->out_byte(full_port, value) &&
                device->out_byte(full_port + 1, value >> 8) &&
                device->out_byte(full_port + 2, value >> 16) &&
                device->out_byte(full_port + 3, value >> 24)
            ) {
                this->A<T>() = value;
                return;
            }
        }
        printf("Unhandled: IN EAX, %X\n", full_port);
    }
}

// Special unop for groups 4/5
template <size_t bits, size_t bus>
template <typename T, typename P>
inline bool z86Base<bits, bus>::unopMS_impl(P& pc) {
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