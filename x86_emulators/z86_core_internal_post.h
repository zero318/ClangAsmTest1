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
        if (!ctx.addr_size_16()) {
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
        if (!ctx.addr_size_16()) {
            if constexpr (ctx.max_bits == 64) {
                if (ctx.addr_size_64()) {
                    switch (m) {
                        case 4: {
                            SIB sib = pc.read_advance<SIB>();
                            uint8_t i = sib.I();
                            if (i != 4) {
                                offset = ctx.index_qword_regI(i) * (1 << sib.S());
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
                                    offset += ctx.index_qword_regMB(b);
                                    break;
                            }
                            break;
                        }
                        case 5:
                            default_segment = SS;
                        default:
                            offset = ctx.index_qword_regMB(m);
                    }
                }
                goto add_const;
            }
            switch (m) {
                case 4: {
                    SIB sib = pc.read_advance<SIB>();
                    uint8_t i = sib.I();
                    if (i != 4) {
                        offset = ctx.index_dword_regI(i) * (1 << sib.S());
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
                            offset += ctx.index_dword_regMB(b);
                            break;
                    }
                    break;
                }
                case 5:
                    default_segment = SS;
                default:
                    offset = ctx.index_dword_regMB(m);
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
inline void regcall z86AddrImpl<bits>::write(const T& value, ssize_t offset) {
    if constexpr (sizeof(T) == sizeof(uint8_t)) {
        return mem.write<T>(this->real_addr(offset), value);
    }
    else if constexpr (sizeof(T) == sizeof(uint16_t)) {
        if (is_aligned<uint16_t>(this->offset + offset)) {
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
template <typename T, typename V>
inline V z86AddrImpl<bits>::read(ssize_t offset) const {
    if constexpr (sizeof(V) == sizeof(uint8_t)) {
        return mem.read<V>(this->real_addr(offset));
    }
    else if constexpr (sizeof(V) == sizeof(uint16_t)) {
        if (is_aligned<uint16_t>(this->offset + offset)) {
            return mem.read<V>(this->real_addr(offset));
        }
        else {
            union {
                V ret;
                uint16_t raw;
            };
            raw = mem.read<uint8_t>(this->real_addr(offset));
            raw |= mem.read<uint8_t>(this->real_addr(offset + 1)) << 8;
            return ret;
        }
    }
    else if constexpr (sizeof(V) == sizeof(uint32_t)) {
        return mem.read<V>(this->real_addr(offset));
    }
    else if constexpr (sizeof(V) == sizeof(uint64_t)) {
        return mem.read<V>(this->real_addr(offset));
    }
}

template <size_t bits>
inline uint32_t z86AddrImpl<bits>::read_Iz(ssize_t index) const {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            return this->read<uint32_t>();
        }
    }
    return this->read<uint16_t>();
}

template <size_t bits>
inline uint32_t z86AddrImpl<bits>::read_advance_Iz() {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            return this->read_advance<uint32_t>();
        }
    }
    return this->read_advance<uint16_t>();
}

template <size_t bits>
inline int32_t z86AddrImpl<bits>::read_Is(ssize_t index) const {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            return this->read<int32_t>();
        }
    }
    return this->read<int16_t>();
}

template <size_t bits>
inline int32_t z86AddrImpl<bits>::read_advance_Is() {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            return this->read_advance<int32_t>();
        }
    }
    return this->read_advance<int16_t>();
}

template <size_t bits>
inline uint64_t z86AddrImpl<bits>::read_Iv(ssize_t index) const {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.data_size_16()) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.data_size_64()) {
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
        if (!ctx.data_size_16()) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.data_size_64()) {
                    return this->read_advance<uint64_t>();
                }
            }
            return this->read_advance<uint32_t>();
        }
    }
    return this->read_advance<uint16_t>();
}

template <size_t bits>
inline uint64_t z86AddrImpl<bits>::read_O(ssize_t index) const {
    if constexpr (ctx.max_bits > 16) {
        if (!ctx.addr_size_16()) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.addr_size_64()) {
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
        if (!ctx.addr_size_16()) {
            if constexpr (ctx.max_bits > 32) {
                if (ctx.addr_size_64()) {
                    return this->read_advance<uint64_t>();
                }
            }
            return this->read_advance<uint32_t>();
        }
    }
    return this->read_advance<uint16_t>();
}

template <z86BaseTemplate>
template <typename T, typename P, typename L>
inline bool regcall z86BaseDefault::binopMR_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = this->index_regR<T>(modrm.R());
    if (modrm.is_mem()) {
        P data_addr = modrm.parse_memM(pc);
        T mval = data_addr.read<T>();
        if (lambda(mval, rval)) {
            data_addr.write<T>(mval);
        }
    }
    else {
        lambda(this->index_regMB<T>(modrm.M()), rval);
    }
    return false;
}

template <z86BaseTemplate>
template <typename T, typename P, typename L>
inline bool regcall z86BaseDefault::binopRM_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T mval;
    if (modrm.is_mem()) {
        P data_addr = modrm.parse_memM(pc);
        mval = data_addr.read<T>();
    }
    else {
        mval = this->index_regMB<T>(modrm.M());
    }
    lambda(this->index_regR<T>(modrm.R()), mval);
    return false;
}

// Far width memory operand, special for LDS/LES
template <z86BaseTemplate>
template <typename T, typename P, typename L>
inline bool regcall z86BaseDefault::binopRMF_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = this->index_regR<T>(modrm.R());
    if (modrm.is_mem()) {
        using DT = dbl_int_t<T>;
        P data_addr = modrm.parse_memM(pc);
        DT temp = data_addr.read<T>();
        temp |= (DT)data_addr.read<uint16_t>(sizeof(T)) << bitsof(T);
        lambda(rval, temp);
    }
    else {
        if constexpr (!this->NO_UD) {
            // TODO: jank
        }
        else {
            this->set_fault(IntUD);
            return true;
        }
    }
    return false;
}

// Double width memory operand, special for BOUND
template <z86BaseTemplate>
template <typename T, typename P, typename L>
inline bool regcall z86BaseDefault::binopRM2_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T& rval = this->index_regR<T>(modrm.R());
    if (modrm.is_mem()) {
        P data_addr = modrm.parse_memM(pc);
        return lambda(rval, data_addr.read<T>(), data_addr.read<T>(sizeof(T)));
    }
    else {
        if constexpr (!this->NO_UD) {
            // TODO: jank
        }
        else {
            this->set_fault(IntUD);
            return true;
        }
    }
    return false;
}

template <z86BaseTemplate>
template <typename T, typename P, typename L>
inline bool regcall z86BaseDefault::binopMS_impl(P& pc, const L& lambda) {
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
        lambda(this->index_regMB<T>(modrm.M()), rval);
    }
    return false;
}

template <z86BaseTemplate>
template <typename T, typename P, typename L>
inline bool regcall z86BaseDefault::binopSM_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    T mval;
    if (modrm.is_mem()) {
        P data_addr = modrm.parse_memM(pc);
        mval = data_addr.read<T>();
    }
    else {
        mval = this->index_regMB<T>(modrm.M());
    }
    lambda(this->index_seg(modrm.R()), mval);
    return false;
}

template <z86BaseTemplate>
template <typename T, typename P, typename L>
inline bool regcall z86BaseDefault::unopM_impl(P& pc, const L& lambda) {
    ModRM modrm = pc.read_advance<ModRM>();
    uint8_t r = modrm.R();
    uint8_t ret;
    if (modrm.is_mem()) {
        P data_addr = modrm.parse_memM(pc);
        T mval = data_addr.read<T>();
        ret = lambda(mval, r);
        if (ret & 1) {
            data_addr.write<T>(mval);
        }
    }
    else {
        ret = lambda(this->index_regMB<T>(modrm.M()), r);
    }
    if constexpr (this->FAULTS_ARE_TRAPS) {
        return false;
    }
    return ret & 2;
}

template <z86BaseTemplate>
template <typename P, typename T>
inline void regcall z86BaseDefault::PUSH_impl(const T& src) {
    this->SP<P>() -= (std::max)(sizeof(T), (size_t)2);
    z86Addr stack = this->stack<P>();
    stack.write(src);
}

template <z86BaseTemplate>
template <typename P, typename T>
inline T z86BaseDefault::POP_impl() {
    z86Addr stack = this->stack<P>();
    T ret = stack.read<T>();
    this->SP<P>() += (std::max)(sizeof(T), (size_t)2);
    return ret;
}

// No wonder ENTER sucks
template <z86BaseTemplate>
template <typename T>
gnu_attr(minsize) inline void regcall z86BaseDefault::ENTER_impl(uint16_t alloc, uint8_t nesting) {
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
}

// TODO: Check what happens if an interrupt toggles 
// the direction flag during a repeating string instruction
template <z86BaseTemplate>
template <typename T, typename P>
inline bool regcall z86BaseDefault::LODS_impl() {
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
    return false;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline bool regcall z86BaseDefault::MOVS_impl() {
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
    return false;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline bool regcall z86BaseDefault::STOS_impl() {
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
    return false;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline bool regcall z86BaseDefault::SCAS_impl() {
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
    return false;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline bool regcall z86BaseDefault::CMPS_impl() {
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
    return false;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline bool regcall z86BaseDefault::OUTS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr src_addr = this->str_src<P>();
    uint16_t port = this->dx;
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
                this->port_out_impl<T>(port, src_addr.read_advance<T>(offset));
            } while (--this->C<P>());
        }
    }
    else {
        this->port_out_impl<T>(port, src_addr.read_advance<T>(offset));
    }
    this->SI<P>() = src_addr.offset;
    return false;
}

template <z86BaseTemplate>
template <typename T, typename P>
inline bool regcall z86BaseDefault::INS_impl() {
    intptr_t offset = this->direction ? sizeof(T) : -sizeof(T);
    z86Addr dst_addr = this->str_dst<P>();
    uint16_t port = this->dx;
    if (this->rep_type > NO_REP) {
        if (this->C<P>()) {
            do {
                // TODO: Interrupt check here
                dst_addr.write_advance<T>(this->port_in_impl<T>(port), offset);
            } while (--this->C<P>());
        }
    }
    else {
        dst_addr.write_advance<T>(this->port_in_impl<T>(port), offset);
    }
    this->DI<P>() = dst_addr.offset;
    return false;
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
        printf("Unhandled: OUT %X, %02X\n", port, value);
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
        printf("Unhandled: OUT %X, %04X\n", port, value);
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
        printf("Unhandled: OUT %X, %08X\n", port, value);
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
        printf("Unhandled: IN AL, %X\n", full_port);
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
        printf("Unhandled: IN AX, %X\n", full_port);
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
        printf("Unhandled: IN EAX, %X\n", full_port);
    }
    return 0;
}

// Special unop for groups 4/5
template <z86BaseTemplate>
template <typename T, typename P>
inline bool regcall z86BaseDefault::unopMS_impl(P& pc) {
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
        T& mval_ref = ctx.index_regMB<T>(modrm.M());
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