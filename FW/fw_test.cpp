#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

#define GAME_VERSION FW_VER

#include "../zero/util.h"

#undef _HAS_CXX17
#define _HAS_CXX17 1

#include <array>
#include <vector>
#include <mutex>
#include <thread>
#include <random>

#include <memory_resource>

#undef _HAS_CXX20
#define _HAS_CXX20 1
#include <memory>
#if !__INTELLISENSE__
#define __cpp_lib_format 1
#include <format>
#endif
#undef _HAS_CXX20
#define _HAS_CXX20 0

#undef WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>
#include <WinUser.h>
#include <wbemidl.h>
#include <winnls32.h>
#include <d3d9.h>
#include <dinput.h>
#include <dsound.h>
#define UNICODE
#include <Core/DXUT.h>
#include <Optional/SDKsound.h>
#undef UNICODE

#define OffsetTypePtr(ptr, type, base)      based_pointer<type>((ptr),-offsetof(type,base))
#define OffsetPtr(ptr, type, field, base)   based_pointer<typeof(type::field)>((ptr),offsetof(type,field)-offsetof(type,base))
#define OffsetField(ptr, type, field, base) (*OffsetPtr(ptr,type,field,base))

template <typename T>
using ZUNListOld = ZUNListBase<T, true>;

#define INCLUDE_PATCH_CODE 0
#define INCLUDE_REPLACEMENT_CODE 1

#if INCLUDE_REPLACEMENT_CODE
#define begin_fast begin
#define inc_fast operator++
#define neq_fast operator!=
#else
#define begin_slow begin
#define inc_slow operator++
#define new_slow operator!=
#endif

#include "../zero/FloatConstants.h"

#if INCLUDE_REPLACEMENT_CODE
#define Float1 Float1Slow
#define Float2 Float2Slow
#define Float3 Float3Slow
#define Int1 Int1Slow
#define Int2 Int2Slow
#define Int3 Int3Slow
#endif

#include "../zero/zun.h"

#if INCLUDE_REPLACEMENT_CODE
#undef Float1
#undef Float2
#undef Float3
#undef Int1
#undef Int2
#undef Int3

#define TH_VER FW

#define VEC_DEFINED 1
#define ZUN_RESULT_DEFINED 1
#define INTERP_MODES_DEFINED 1
#define ANGLE_CONSTS_DEFINED 1

#define PackUInt32 PackUInt32_
#define PackD3DCOLOR PackD3DCOLOR_
#define ZUNResult ZUNResult_
#define Timer TimerFast
#define ZUNInterp ZUNInterpFast
#define ZUNList ZUNList_
#define GAME_SPEED GAME_SPEED_FAST

#define VEC3_ALLOW_OVERREAD 0

#if __SSE3__
#undef __SSE3__
#define __SSE3__ 0
#define PREV__SSE3__ 1
#endif
#if __SSSE3__
#undef __SSSE3__
#define __SSSE3__ 0
#define PREV__SSSE3__ 1
#endif
#if __SSE4_1__
#undef __SSE4_1__
#define __SSE4_1__ 0
#define PREV__SSE4_1__ 1
#endif

#include "../zero/th_common.h"

#if PREV__SSE3__
#undef __SSE3__
#define __SSE3__ 1
#endif
#if PREV__SSSE3__
#undef __SSSE3__
#define __SSSE3__ 1
#endif
#if PREV__SSE4_1__
#undef __SSE4_1__
#define __SSE4_1__ 1
#endif


#undef PackUInt32
#undef PackD3DCOLOR
#undef ZUNResult
#undef Timer
#undef ZUNInterp
#undef ZUNList
#undef GAME_SPEED
#endif

dllexport gnu_noinline void* cdecl memset_force(void* dst, int val, size_t size) {
    gnu_attr(musttail) return memset(dst, val, size);
}

#define zero_pointer_contents(ptr) memset_force(ptr, 0, sizeof(*ptr))
#define zero_pointer_contents_inline(ptr) __builtin_memset(ptr, 0, sizeof(*ptr))
#define zero_array(arr) memset_force(arr, 0, sizeof(arr))
#define zero_array_inline(arr) __builtin_memset(arr, 0, sizeof(arr))
#define zero_this() memset_force(this, 0, sizeof(*this));
#define zero_this_inline() __builtin_memset(this, 0, sizeof(*this));

dllexport gnu_noinline void* cdecl memset_dumb(void* dest, int arg, size_t count) {
    void* ret = memset(dest, arg, count);
    __asm__ volatile ("":"+r"(ret));
    return ret;
}

dllexport uint64_t regparm(3) aullshr_test3(uint64_t value, uint8_t count) {
    uint32_t high = value >> 32;
    uint32_t low = value;

    low = count < 64 ? count < 32 ? low : high : 0;
    high = count < 32 ? high : 0;

    uint32_t highB = high >> count;
    uint32_t lowA = low >> count;
    count = ~count;
    high <<= 1;
    uint32_t lowB = high << count;
    low = lowA | lowB;
    high = highB;

    return (uint64_t)high << 32 | low;
}

dllexport uint64_t regparm(3) allshl_fast(uint64_t value, uint8_t count) {
    uint32_t high = value >> 32;
    uint32_t low = value;

    high = count < 64 ? count < 32 ? high : low : 0;
    low = count < 32 ? low : 0;
    __asm__ volatile ("":"+a"(low));

    uint32_t highA = high << count;
    uint32_t lowB = low << count;
    __asm__ volatile ("":"+S"(low));
    count = ~count;
    low >>= 1;
    uint32_t highB = low >> count;
    high = highA | highB;
    low = lowB;

    return (uint64_t)high << 32 | low;
}

dllexport uint64_t regparm(3) allshl_fast_old(uint64_t value, uint8_t count) {
    if (count < 64) {
        return value << count;
    }
    return 0;
}

/*
dllexport int64_t regparm(3) allshl_faster(int64_t value, uint8_t count) {
    assume(count < 64);
    return value << count;
}
dllexport int64_t regparm(3) allshr_faster(int64_t value, uint8_t count) {
    assume(count < 64);
    return value >> count;
}
dllexport uint64_t regparm(3) aullshr_faster(uint64_t value, uint8_t count) {
    assume(count < 64);
    return value >> count;
}
dllexport int64_t regparm(3) allshl_fast(int64_t value, uint8_t count) {
    if (expect(count < 64, true)) {
        return value << count;
    }
    return 0;
}
dllexport int64_t regparm(3) allshr_fast(int64_t value, uint8_t count) {
    if (expect(count < 64, true)) {
        return value >> count;
    }
    return value < 0 ? -1 : 0;
}
dllexport uint64_t regparm(3) aullshr_fast(uint64_t value, uint8_t count) {
    if (expect(count < 64, true)) {
        return value >> count;
    }
    return 0;
}
*/

struct debug_memory_resource : std::pmr::memory_resource {
    virtual void* do_allocate(std::size_t bytes, std::size_t alignment) {

    }
    virtual void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) {

    }
    virtual bool do_is_equal(const std::pmr::memory_resource& other) const noexcept {
        return true;
    }
};

struct MemAllocLog {
    dllexport gnu_noinline void* thiscall allocate(const char* str = "whatever") {
        use_var(str);
        return (void*)rand();
    }
    dllexport gnu_noinline void thiscall free_mem(void* ptr) {
        use_var(ptr);
    }
    dllexport gnu_noinline void thiscall delete_mem(void* ptr) {
        use_var(ptr);
    }
    dllexport static gnu_noinline void* stdcall allocate_static(const char* str = "whatever") {
        use_var(str);
        return (void*)rand();
    }
    dllexport static gnu_noinline void stdcall free_mem_static(void* ptr) asm_symbol_rel(Rx1F670) {
        use_var(ptr);
    }
    dllexport static gnu_noinline void stdcall delete_mem_static(void* ptr) asm_symbol_rel(Rx1F7C0) {
        use_var(ptr);
    }

    template <typename T>
    dllexport gnu_noinline T* thiscall allocate_new(const char* str = "whatever") {
        return new T;
    }
};

dllexport MemAllocLog* MEM_ALLOC_LOG;

// size: 0x8
struct GameSpeed {
    float value = 1.0f; // 0x0
    int __counter_4; // 0x4
    // 0x8

    
    void vectorcall set(float new_speed) {
        this->value = new_speed;
        // TODO
    }

    inline operator float() const {
        return this->value;
    }
};
#pragma region // GameSpeed Validation
ValidateFieldOffset32(0x0, GameSpeed, value);
ValidateFieldOffset32(0x4, GameSpeed, __counter_4);
ValidateStructSize32(0x8, GameSpeed);
#pragma endregion

dllexport GameSpeed GAME_SPEED;

#if INCLUDE_REPLACEMENT_CODE
dllexport float cdecl reduce_angle_fast(float angle) {
    float cmp = PI_f;
    __asm__ volatile("":"+x"(cmp));
    if (expect(angle > cmp, false)) {
        size_t counter = 0;
        float diff = -TWO_PI_f;
        __asm__ volatile("":"+x"(diff));
        nounroll do {
            angle += diff;
            if (counter == 33) break;
            ++counter;
        } while (expect(angle > cmp, true));
    }
    else {
        cmp = -PI_f;
        __asm__ volatile("":"+x"(cmp));
        if (expect(angle < cmp, false)) {
            size_t counter = 0;
            float diff = TWO_PI_f;
            __asm__ volatile("":"+x"(diff));
            nounroll do {
                angle += diff;
                if (counter == 33) break;
                ++counter;
            } while (expect(angle < cmp, true));
        }
    }
    return angle;
}
dllexport float cdecl reduce_angle_add_fast(float angle, float value) {
    return reduce_angle_fast(angle + value);
}
#else
#define reduce_angle_fast(...) reduce_angle(__VA_ARGS__)
#define reduce_angle_add_fast(...) reduce_angle_add(__VA_ARGS__)
#endif

// size: 0x10
struct Timer {
    int32_t previous; // 0x0
    int32_t current; // 0x4
    float current_f; // 0x8
    union {
        uint32_t flags; // 0xC
        struct {
            uint32_t initialized : 1;
            uint32_t scale_table_index : 2;
        };
    };
    // 0x10

    inline operator int32_t() {
        return this->current;
    }
    inline operator float() {
        return this->current_f;
    }

    inline void default_values() {
        this->current = 0;
        this->previous = -999999;
        this->current_f = 0.0f;
    }

    float cdecl add_raw(float value) {
        this->previous = this->current;
        return this->current = this->current_f += value;
    }

#if INCLUDE_REPLACEMENT_CODE

    void fast_dec() {

    }

    dllexport int32_t thiscall add_fast(float amount) {
        float time_scale = GAME_SPEED;
        //if (expect(this->initialized, true)) {
            this->previous = this->current;
            float current_f = this->current_f;
            if (!(expect(time_scale >= 0.99f, true) && expect(time_scale <= 1.01f, true))) {
                amount *= time_scale;
            }
            current_f += amount;
            this->current_f = current_f;
            return this->current = (int32_t)current_f;
            /*
        } else {
            this->previous = 0;
            this->flags = 1;
            if (!(expect(time_scale >= 0.99f, true) && expect(time_scale <= 1.01f, true))) {
                amount *= time_scale;
            }
            this->current_f = amount;
            return this->current = (int32_t)amount;
        }
        */
    }

    dllexport void thiscall set_new(int32_t time) {
        this->current = time;
        this->current_f = (float)time;
        this->previous = time - 1;
        this->flags = 1;
    }
    dllexport int32_t thiscall tick_new() {
        float time_scale = GAME_SPEED;
        //if (expect(this->initialized, true)) {
            int32_t current = this->current;
            this->previous = current;
            float current_f = this->current_f;
            if (expect(time_scale >= 0.99f, true) && expect(time_scale <= 1.01f, true)) {
                this->current = ++current;
                this->current_f = current_f + 1.0f;
            } else {
                current_f += time_scale;
                this->current_f = current_f;
                current = (int32_t)current_f;
                this->current = current;
            }
            return current;
            /*
        }
        else {
            if (expect(time_scale >= 0.99f, true) && expect(time_scale <= 1.01f, true)) {
                static constexpr Timer DEFAULT_TICKED = { .previous = 0, .current = 1, .current_f = 1.0f, .flags = 1 };
                *this = DEFAULT_TICKED;
                return 1;
            } else {
                this->previous = 0;
                this->flags = 1;
                this->current_f = time_scale;
                return this->current = (int32_t)time_scale;
            }
        }
        */
    }
#endif

    // Rx29280
    dllexport void thiscall set_from_timer(const Timer& rhs) {
        this->set_new(rhs.current);
    }

    // Rx23520
    dllexport void thiscall operator=(int32_t time) {
        this->set_new(time);
    }

    // Rx297A0
    dllexport int32_t thiscall add(int32_t value) {
        return this->add_fast(value);
    }

    // Rx29E70
    dllexport int32_t thiscall sub(int32_t value) {
        return this->add(-value);
    }

    // Rx135770
    dllexport int32_t thiscall sub(float value) {
        return this->add_fast(-value);
    }

    // Rx5C300
    dllexport int32_t thiscall operator+=(int32_t value) {
        return this->add(value);
    }

    // RxC9170
    dllexport int32_t thiscall operator-=(int32_t value) {
        return this->sub(value);
    }

    // RxC9100
    dllexport int32_t thiscall operator+=(float value) {
        return this->add_fast(value);
    }

    // Rx134FA0
    dllexport int32_t thiscall operator-=(float value) {
        return this->sub(value);
    }

    // Rx29420
    dllexport int32_t thiscall operator--(int) {
        return this->sub(1);
    }

    // Rx23540
    dllexport int32_t thiscall operator++(int) {
        return this->tick_new();
    }

    // Rx29400
    dllexport int32_t thiscall operator++() {
        return this->tick_new();
    }

    // Rx5C2E0
    dllexport int32_t thiscall operator*(int32_t value) {
        return this->current * value;
    }
    // Rx72040
    dllexport int32_t thiscall operator%(int32_t value) {
        return this->current % value;
    }

    // Rx23560
    dllexport bool thiscall operator<(int32_t time) {
        return this->current < time;
    }
    // Rx23590
    dllexport bool thiscall operator<=(int32_t time) {
        return this->current <= time;
    }
    // Rx235C0
    dllexport bool thiscall operator>=(int32_t time) {
        return this->current >= time;
    }
    // Rx61070
    dllexport bool thiscall operator>(int32_t time) {
        return this->current > time;
    }

    // Rx785E0
    dllexport bool thiscall operator==(int32_t time) {
        return this->current == time;
    }
    // Rx785E0
    dllexport bool thiscall operator!=(int32_t time) {
        return this->current != time;
    }
};

// size: 0x10
struct ZUNRect {
    uint32_t x; // 0x0
    uint32_t y; // 0x4
    uint32_t w; // 0x8
    uint32_t h; // 0xC
    // 0x10
};

// This is actually a struct but I'll deal with it later
using ZUNColor = D3DCOLOR;

static inline uint8_t& BLUE(D3DCOLOR& color) {
    return ((uint8_t*)&color)[0];
}
static inline uint8_t& GREEN(D3DCOLOR& color) {
    return ((uint8_t*)&color)[1];
}
static inline uint8_t& RED(D3DCOLOR& color) {
    return ((uint8_t*)&color)[2];
}
static inline uint8_t& ALPHA(D3DCOLOR& color) {
    return ((uint8_t*)&color)[3];
}
// Just make these macro colored
// to indicate there's BS going on
#define BLUE(...) BLUE(__VA_ARGS__)
#define GREEN(...) GREEN(__VA_ARGS__)
#define RED(...) RED(__VA_ARGS__)
#define ALPHA(...) ALPHA(__VA_ARGS__)

struct D3DMATRIXZ : D3DMATRIX {


    inline D3DXMATRIX& D3DX() {
        return *(D3DXMATRIX*)this;
    }

    // D3DXMatrixIdentity
    inline void set_identity() {
        this->m[0][0] = 1.0f;
        this->m[0][1] = 0.0f;
        this->m[0][2] = 0.0f;
        this->m[0][3] = 0.0f;
        this->m[1][0] = 0.0f;
        this->m[1][1] = 1.0f;
        this->m[1][2] = 0.0f;
        this->m[1][3] = 0.0f;
        this->m[2][0] = 0.0f;
        this->m[2][1] = 0.0f;
        this->m[2][2] = 1.0f;
        this->m[2][3] = 0.0f;
        this->m[3][0] = 0.0f;
        this->m[3][1] = 0.0f;
        this->m[3][2] = 0.0f;
        this->m[3][3] = 1.0f;
    }

    // D3DXMatrixScaling
    inline void set_scaled(float X, float Y, float Z = 1.0f) {
        this->m[0][0] = X;
        this->m[0][1] = 0.0f;
        this->m[0][2] = 0.0f;
        this->m[0][3] = 0.0f;
        this->m[1][0] = 0.0f;
        this->m[1][1] = Y;
        this->m[1][2] = 0.0f;
        this->m[1][3] = 0.0f;
        this->m[2][0] = 0.0f;
        this->m[2][1] = 0.0f;
        this->m[2][2] = Z;
        this->m[2][3] = 0.0f;
        this->m[3][0] = 0.0f;
        this->m[3][1] = 0.0f;
        this->m[3][2] = 0.0f;
        this->m[3][3] = 1.0f;
    }

    inline void rotate_x(float rotation) {
        if (rotation != 0.0f) {
            D3DXMATRIX temp;
            D3DXMatrixRotationX(&temp, rotation);
            D3DXMatrixMultiply(&this->D3DX(), &this->D3DX(), &temp);
        }
    }

    inline void rotate_y(float rotation) {
        if (rotation != 0.0f) {
            D3DXMATRIX temp;
            D3DXMatrixRotationY(&temp, rotation);
            D3DXMatrixMultiply(&this->D3DX(), &this->D3DX(), &temp);
        }
    }

    inline void rotate_z(float rotation) {
        if (rotation != 0.0f) {
            D3DXMATRIX temp;
            D3DXMatrixRotationZ(&temp, rotation);
            D3DXMatrixMultiply(&this->D3DX(), &this->D3DX(), &temp);
        }
    }
};
#pragma region // D3DMATRIXZ Validation
ValidateStructSize32(0x40, D3DMATRIXZ);
#pragma endregion

template <typename T>
struct ZUNListIter;

template <typename T>
struct ZUNListIterable;

template <typename T>
struct ZUNList {
    T* data; // 0x0
    union {
        struct {
            ZUNList<T>* next; // 0x4
            ZUNList<T>* prev; // 0x8
            ZUNListIterable<T>* list; // 0xC
            ZUNListIter<T>* current_iter; // 0x10
        };
        vec<float, 2, false> zero_vec_uppper packed_field;
        vec<float, 4, false> zero_vec packed_field;
    };
    // 0x14
    
    // Rx18A10
    ZUNList() {
        this->data = NULL;
        this->next = NULL;
        this->prev = NULL;
        this->list = NULL;
        this->current_iter = NULL;
    }

    // Rx4B9F0
    dllexport ZUNListIter<T> thiscall begin_impl() {
        ZUNListIter<T> ret;
        if (!this->get_data()) {
            ret.set(NULL, NULL);
        }
        else {
            ret.set(this, this->get_next());
        }
        return ret;
    }

    inline ZUNListIter<T> begin_slow() {
        ZUNListIter<T> iter;
        iter = this->begin_impl();
        return iter;
    }

#if INCLUDE_REPLACEMENT_CODE
    ZUNListIter<T> thiscall begin_fast() {
        return { this, this->next };
    }
#endif

    ZUNList<T>* end() {
        return NULL;
    }
    
    T*& get_data() {
        return this->data;
    }
    
    ZUNList<T>* get_next() {
        return this->next;
    }

    ZUNList<T>* get_prev() {
        return this->prev;
    }
    
    void set_list(ZUNListIterable<T>* list) {
        this->list = list;
    }
    
    void set_current_iter(ZUNListIter<T>* iter) {
        this->current_iter = iter;
    }
    
    // Rx4A670
    void initialize(T* data) {
        this->data = data;
        this->next = NULL;
        this->prev = NULL;
        this->list = NULL;
        this->current_iter = NULL;
    }
    
    // Rx11EE0
    void append(ZUNList<T>* node) {
        if (this->next) {
            node->next = this->next;
            this->next->prev = node;
        }
        this->next = node;
        node->list = this->list;
        node->prev = this;
    }
    
    // Rx11F30
    void prepend(ZUNList<T>* node) {
        if (this->prev) {
            node->prev = this->prev;
            this->prev->next = node;
        }
        node->list = this->list;
        node->next = this;
        this->prev = node;
    }

#if INCLUDE_REPLACEMENT_CODE
    dllexport void append_fast(ZUNList<T>* node) {
        ZUNList<T>* next_node = this->next;
        this->next = node;
        if (next_node) {
            next_node->prev = node;
            node->next = next_node;
        }
        node->prev = this;
        node->list = this->list;
    }
    dllexport void append_next_only(ZUNList<T>* node) {
        this->next = node;
    }
    dllexport void prepend_fast(ZUNList<T>* node) {
        ZUNList<T>* prev_node = this->prev;
        this->prev = node;
        if (prev_node) {
            prev_node->next = node;
            node->prev = prev_node;
        }
        node->next = this;
        node->list = this->list;
    }
    void append_fast_no_list() {
        ZUNList<T>* next_node = this->next;
        this->next = node;
        if (next_node) {
            next_node->prev = node;
            node->next = next_node;
        }
        node->prev = this;
    }
    void prepend_fast_no_list() {
        ZUNList<T>* prev_node = this->prev;
        this->prev = node;
        if (prev_node) {
            prev_node->next = node;
            node->prev = prev_node;
        }
        node->next = this;
    }
#endif
    
    // Rx11E30
    ZUNList<T>* find_node_with_data_impl(T* data) {
        ZUNList<T>* node = this;
        while (node) {
            if (node->get_data() == data) {
                return node;
            }
            node = node->get_next();
        }
        return NULL;
    }
    
    // Rx11E70
    ZUNList<T>* find_node_with_data(T* data) {
        ZUNList<T>* node = this;
        return node->find_node_with_data_impl(data);
    }
    
    // Rx10A0E0
    ZUNList<T>* find_end_node() {
        ZUNList<T>* node = this;
        while (node->get_next()) {
            node = node->get_next();
        }
        return node;
    }
    
    // Rx108B60
    void append_to_list_end(ZUNList<T>* node) {
        ZUNList<T>* end = this->find_end_node();
        end->append(node);
    }

    // Rx11CE0
    void unlink();
    
    // Rx11D50
    void unlink_impl();

#if INCLUDE_REPLACEMENT_CODE
    dllexport void unlink_fast();
    dllexport ZUNList<T>* unlink_fast_no_clear();
    dllexport ZUNList<T>* unlink_fast_always_tail_no_clear();
    dllexport ZUNList<T>* unlink_fast_no_tail();
    dllexport ZUNList<T>* unlink_fast_no_clear_no_tail();
    dllexport ZUNList<T>* unlinkR_fast_no_clear();
    dllexport ZUNList<T>* unlinkR_fast_no_tail();
    dllexport ZUNList<T>* unlinkR_fast_no_clear_no_tail();
#endif
};

template <typename T>
struct ZUNListIter {
    ZUNList<T>* current; // 0x0
    ZUNList<T>* next; // 0x4
    // 0x8
    
#if !INCLUDE_REPLACEMENT_CODE
    ZUNListIter() {
        this->current = NULL;
        this->next = NULL;
    }
#endif
    
    // Rx119B0
    ZUNList<T>* set(ZUNList<T>* node, ZUNList<T>* next) {
        this->current = node;
        this->next = next;
        if (this->current) {
            ZUNList<T>* new_current = this->current;
            new_current->set_current_iter(NULL);
        }
        ZUNList<T>* next_node;
        if (node) {
            next_node = node->get_next();
        } else {
            next_node = NULL;
        }
        this->next = next_node;
        if (this->next) {
            ZUNList<T>* new_next = this->next;
            new_next->set_current_iter(this);
        }
        return node;
    }

#if INCLUDE_REPLACEMENT_CODE
    dllexport ZUNListIter<T>& thiscall set_fast(ZUNList<T>* node, ZUNList<T>* next) {
        this->current = node;
        ZUNList<T>* temp = NULL;
        if (expect(node != NULL, true)) {
            temp = node->next;
        }
        this->next = temp;
        return *this;
    }

    dllexport ZUNListIter<T>& thiscall set_faster(ZUNList<T>* node, ZUNList<T>* next) {
        this->current = node;
        return *this;
    }
#endif
    
    ZUNList<T>* operator*() {
        return this->current;
    }
    
    // Rx11C30
    dllexport ZUNListIter<T>& thiscall inc_slow() {
        if (this->current) {
            ZUNList<T>* old_current = this->current;
            old_current->set_current_iter(NULL);
        }
        this->current = this->next;
        this->next = this->current;
        if (this->next) {
            this->next = this->current->get_next();
            if (this->next) {
                ZUNList<T>* new_next = this->next;
                new_next->set_current_iter(this);
            }
        }
        return *this;
    }

#if INCLUDE_REPLACEMENT_CODE
    dllexport void thiscall inc_fast() {
        ZUNList<T>* node = this->next;
        this->current = node;
        if (node) {
            node = node->next;
        }
        this->next = node;
    }

    dllexport void thiscall inc_faster() {
        ZUNList<T>* node = this->current;
        this->current = node->next;
    }
#endif
    
    // Rx11BB0
    dllexport uint8_t thiscall neq_slow(ZUNList<T>* node) {
        int ret;
        if (node) {
            // I may have this line wrong, it seems weird
            ret = (void*)this->current != (void*)node->data;
        } else {
            ret = this->current != NULL;
        }
        return ret;
    }

#if INCLUDE_REPLACEMENT_CODE
    /*
    dllexport uint8_t not_equal_fast(ZUNList<T>* node) {
        if (expect(node == NULL, true)) {
            return this->current != node;
        }
        else {
            return this->current != (ZUNList<T>*)node->data;
        }
    }
    */
    dllexport uint8_t thiscall neq_fast(ZUNList<T>* node) {
        return this->current != NULL;
    }
#endif
    
#if !INCLUDE_REPLACEMENT_CODE
    // Rx11B00
    ~ZUNListIter() {
        if (this->current) {
            ZUNList<T>* current = this->current;
            current->set_current_iter(NULL);
        }
        if (this->next) {
            ZUNList<T>* next = this->next;
            next->set_current_iter(NULL);
        }
    }
#endif
};

template <typename T>
struct ZUNListIterable : ZUNList<T> {
    // ZUNList<T> base; // 0x0
    ZUNList<T>* tail; // 0x14
    // 0x18
    
    ZUNListIterable() : tail(this) {
        this->tail = this;
    }

    ZUNList<T>* get_head() {
        return this->get_next();
    }
    
    // Rx4A640
    void initialize(T* data) {
        this->ZUNList<T>::initialize(data);
        this->tail = this;
    }
    
    // Rx12280
    ZUNListIter<T>& begin_impl(ZUNListIter<T>& iter) {
        iter.set(this->get_next(), NULL);
        return iter;
    }

    inline ZUNListIter<T> begin_slow() {
        ZUNListIter<T> iter;
        this->begin_impl(iter);
        return iter;
    }

#if INCLUDE_REPLACEMENT_CODE
    dllexport void thiscall begin_fast_old(ZUNListIter<T>& iter) {
        iter.set_faster(this->next, NULL);
    }
    inline ZUNListIter<T> begin_fast() {
        //return { this->next, NULL };
        ZUNListIter<T> iter;
        iter.set_fast(this->next, NULL);
        return iter;
    }
#endif
    
    // Rx11EA0
    void append(ZUNList<T>* node) {
        ZUNList<T>* tail = this->tail;
        tail->append(node);
        node->set_list(this);
        this->tail = node;
    }
    
    // Rx4A6B0
    void prepend(ZUNList<T>* node) {
        this->ZUNList<T>::prepend(node);
        node->set_list(this);
        if (this->tail == this) {
            this->tail = node;
        }
    }

    // Rx11D20
    void unlink_node(ZUNList<T>* node) {
        if (this->tail == node) {
            this->tail = node->get_prev();
        }
        node->unlink_impl();
    }
#if INCLUDE_REPLACEMENT_CODE
    dllexport void append_fast(ZUNList<T>* node) {
        ZUNList<T>* tail_node = this->tail;
        this->tail = node;
        node->list = this;
        ZUNList<T>* next_node = tail_node->next;
        tail_node->next = node;
        if (next_node) {
            next_node->prev = node;
            node->next = next_node;
        }
        node->prev = tail_node;
    }
    dllexport void prepend_fast(ZUNList<T>* node) {
        ZUNList<T>* next_node = this->next;
        this->next = node;
        if (next_node) {
            next_node->prev = node;
            node->next = next_node;
        }
        node->prev = this;
        node->list = this->list;
        if (this->tail == this) {
            this->tail = node;
        }
    }
#endif
};

#if INCLUDE_REPLACEMENT_CODE
dllexport void export_my_functions_please_thx(ZUNListIter<void>* ptr, ZUNListIterable<void>* list, ZUNList<void>* node) {
    ptr->set_fast(NULL, NULL);
    ptr->set_faster(NULL, NULL);
    ptr->inc_fast();
    ptr->inc_faster();
    //ptr->not_equal_fast(NULL);
    //ptr->not_equal_faster(NULL);
    //list->begin_fast(*ptr);
    //list->begin_faster(*ptr);
    //list->begin2_fast(*ptr);
    //list->begin2_faster(*ptr);
    list->append_fast(NULL);
    list->prepend_fast(NULL);
    node->append_fast(NULL);
    node->prepend_fast(NULL);
    node->unlink_fast();
    node->unlink_fast_no_clear();
    node->unlink_fast_no_clear_no_tail();
}
#endif

// Rx11CE0
template <typename T>
void ZUNList<T>::unlink() {
    if (this->list) {
        ZUNListIterable<T>* list = this->list;
        list->unlink_node(this);
    } else {
        this->unlink_impl();
    }
}

// Rx11D50
template <typename T>
void ZUNList<T>::unlink_impl() {
    if (this->current_iter) {
        if (this->current_iter->current == this) {
            this->current_iter->current = NULL;
        }
        if (
            this->current_iter->next == this &&
            this->current_iter->next != NULL
        ) {
            this->current_iter->next = this->next;
            if (this->current_iter->next) {
                ZUNList<T>* new_next = this->next;
                new_next->set_current_iter(this->current_iter);
            }
        }
        this->current_iter = NULL;
    }
    if (this->next) {
        this->next->prev = this->prev;
    }
    if (this->prev) {
        this->prev->next = this->next;
    }
    this->next = NULL;
    this->prev = NULL;
    this->list = NULL;
}

#if INCLUDE_REPLACEMENT_CODE
template <typename T>
dllexport void ZUNList<T>::unlink_fast() {
    ZUNList<T>* prev_node = this->prev;
    ZUNListIterable<T>* list = this->list;
    if (expect(list != NULL, true)) {
        if (list->tail == this) {
            list->tail = prev_node;
        }
    }
    ZUNList<T>* next_node = this->next;
    if (next_node) {
        next_node->prev = prev_node;
    }
    if (prev_node) {
        prev_node->next = next_node;
    }
    this->zero_vec = {};
}
template <typename T>
dllexport ZUNList<T>* ZUNList<T>::unlink_fast_no_clear() {
    ZUNList<T>* prev_node = this->prev;
    ZUNListIterable<T>* list = this->list;
    if (expect(list != NULL, true)) {
        if (list->tail == this) {
            list->tail = prev_node;
        }
    }
    ZUNList<T>* next_node = this->next;
    if (next_node) {
        next_node->prev = prev_node;
    }
    if (prev_node) {
        prev_node->next = next_node;
    }
    return next_node;
}
template <typename T>
dllexport ZUNList<T>* ZUNList<T>::unlink_fast_always_tail_no_clear() {
    ZUNList<T>* prev_node = this->prev;
    ZUNListIterable<T>* list = this->list;
    //if (expect(list != NULL, true)) {
        if (list->tail == this) {
            list->tail = prev_node;
        }
    //}
    ZUNList<T>* next_node = this->next;
    if (next_node) {
        next_node->prev = prev_node;
    }
    if (prev_node) {
        prev_node->next = next_node;
    }
    return next_node;
}
template <typename T>
dllexport ZUNList<T>* ZUNList<T>::unlink_fast_no_tail() {
    ZUNList<T>* next_node = this->next;
    ZUNList<T>* prev_node = this->prev;
    this->zero_vec_uppper = {};
    if (next_node) {
        next_node->prev = prev_node;
    }
    if (prev_node) {
        prev_node->next = next_node;
    }
    return next_node;
}
template <typename T>
dllexport ZUNList<T>* ZUNList<T>::unlink_fast_no_clear_no_tail() {
    ZUNList<T>* next_node = this->next;
    ZUNList<T>* prev_node = this->prev;
    if (next_node) {
        next_node->prev = prev_node;
    }
    if (prev_node) {
        prev_node->next = next_node;
    }
    return next_node;
}
template <typename T>
dllexport ZUNList<T>* ZUNList<T>::unlinkR_fast_no_clear() {
    ZUNList<T>* next_node = this->next;
    ZUNList<T>* prev_node = this->prev;
    if (prev_node) {
        prev_node->next = next_node;
    }
    if (next_node) {
        next_node->prev = prev_node;
    }
    return next_node;
}
template <typename T>
dllexport ZUNList<T>* ZUNList<T>::unlinkR_fast_no_tail() {
    ZUNList<T>* next_node = this->next;
    ZUNList<T>* prev_node = this->prev;
    this->zero_vec_uppper = {};
    if (prev_node) {
        prev_node->next = next_node;
    }
    if (next_node) {
        next_node->prev = prev_node;
    }
    return next_node;
}
template <typename T>
dllexport ZUNList<T>* ZUNList<T>::unlinkR_fast_no_clear_no_tail() {
    ZUNList<T>* next_node = this->next;
    ZUNList<T>* prev_node = this->prev;
    if (prev_node) {
        prev_node->next = next_node;
    }
    if (next_node) {
        next_node->prev = prev_node;
    }
    return next_node;
}
#endif

// size: 0xC
struct RGB {
    int32_t r; // 0x0
    int32_t g; // 0x4
    int32_t b; // 0x8
    // 0xC

private:
    inline RGB& add(RGB& out, const RGB& value) const {
        out.r = this->r + value.r;
        out.g = this->g + value.g;
        out.b = this->b + value.b;
        return out;
    }
public:
    inline RGB operator+(const RGB& value) const {
        RGB dummy;
        return this->add(dummy, value);
    }

private:
    inline RGB& sub(RGB& out, const RGB& value) const {
        out.r = this->r - value.r;
        out.g = this->g - value.g;
        out.b = this->b - value.b;
        return out;
    }
public:
    inline RGB operator-(const RGB& value) const {
        RGB dummy;
        return this->sub(dummy, value);
    }

private:
    inline RGB& mul(RGB& out, const float value) const {
        out.r = this->r * value;
        out.g = this->g * value;
        out.b = this->b * value;
        return out;
    }
public:
    inline RGB operator*(const float value) const {
        RGB dummy;
        return this->mul(dummy, value);
    }
    friend inline RGB operator*(const float value, const RGB& rhs) {
        RGB dummy;
        return rhs.mul(dummy, value);
    }
};

// size: 0x10
struct ZUN_COLORVALUE {
    float b; // 0x0
    float g; // 0x4
    float r; // 0x8
    float a; // 0xC
    // 0x10
};

// size: 0x1C
struct StageSky {
    float begin_distance; // 0x0
    float end_distance; // 0x4
    ZUN_COLORVALUE color_components; // 0x8
    D3DCOLOR color; // 0x18
    // 0x1C

    // The color values in this type are some sort of
    // terrible array thing, but I'm *not* doing that.

    // Rx73400
    void pack_color() {
        BLUE(this->color) = this->color_components.b;
        GREEN(this->color) = this->color_components.g;
        RED(this->color) = this->color_components.r;
        ALPHA(this->color) = this->color_components.a;
    }

    // Rx718D0
    StageSky& thiscall set(float begin_distance, float end_distance, float b, float g, float r, float a) {
        this->begin_distance = begin_distance;
        this->end_distance = end_distance;
        this->color_components.b = b;
        this->color_components.g = g;
        this->color_components.r = r;
        this->color_components.a = a;
        this->pack_color();
        return *this;
    }

private:
    // Rx71F50
    StageSky& thiscall add(StageSky& out, StageSky& value) {
        return out.set(
            this->begin_distance + value.begin_distance,
            this->begin_distance + value.end_distance,
            this->color_components.b + value.color_components.b,
            this->color_components.g + value.color_components.g,
            this->color_components.r + value.color_components.r,
            this->color_components.a + value.color_components.a
        );
    }
public:
    inline StageSky operator+(StageSky& rhs) {
        StageSky dummy;
        return this->add(dummy, rhs);
    }

private:
    // Rx71E60
    StageSky& thiscall sub(StageSky& out, StageSky& value) {
        return out.set(
            this->begin_distance - value.begin_distance,
            this->begin_distance - value.end_distance,
            this->color_components.b - value.color_components.b,
            this->color_components.g - value.color_components.g,
            this->color_components.r - value.color_components.r,
            this->color_components.a - value.color_components.a
        );
    }
public:
    inline StageSky operator-(StageSky& value) {
        StageSky dummy;
        return this->sub(dummy, value);
    }

private:
    // Rx71DB0
    StageSky& thiscall mul(StageSky& out, float value) {
        return out.set(
            this->begin_distance * value,
            this->begin_distance * value,
            this->color_components.b * value,
            this->color_components.g * value,
            this->color_components.r * value,
            this->color_components.a * value
        );
    }
public:
    inline StageSky operator*(float value) {
        StageSky dummy;
        return this->mul(dummy, value);
    }
};

// size: 0x4
struct ZUNAngle {
    float value; // 0x0
    // 0x4

    inline ZUNAngle() {}
};
// Macros were exactly like this in original code
// so keep it this way despite the crap
#define pow_2(x) (x)*(x)
#define pow_3(x) (x)*(x)*(x)
#define pow_4(x) (x)*(x)*(x)*(x)

// becomes asymptotic at b=0.5 and b=1
#define parabola(a, b) \
    (((pow_2(a - b)) / (pow_2(1.0f - b)) - (pow_2(b)) / (pow_2(1.0f - b))) / (1.0f - (pow_2(b)) / (pow_2(1.0f - b))))

dllexport gnu_noinline float cdecl __interp_inner_thing(int32_t mode, float current_time, float end_time) {
    if (!(end_time != 0.0f)) {
        return 1.0f;
    }
    float value = current_time / end_time;
    switch (mode) {
        case AccelerateSlow: // 1
            return pow_2(value);
        case DecelerateSlow: // 4
            return 1.0f - pow_2(1.0f - value);
        case Accelerate: // 2
            return pow_3(value);
        case Decelerate: // 5
            return 1.0f - pow_3(1.0f - value);
        case AccelerateFast: // 3
            return pow_4(value);
        case DecelerateFast: // 6
            return 1.0f - pow_4(1.0f - value);
        case AccelDecelSlow: { // 9
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = pow_2(value) / 2.0f;
            } else {
                ret = (2.0f - pow_2(2.0f - value)) / 2.0f;
            }
            return ret;
        }
        case DecelAccelSlow: { // 12
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = 0.5f - pow_2(1.0f - value) / 2.0f;
            } else {
                ret = 0.5f + pow_2(value - 1.0f) / 2.0f;
            }
            return ret;
        }
        case AccelDecel: { // 10
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = pow_3(value) / 2.0f;
            } else {
                ret = (2.0f - pow_3(2.0f - value)) / 2.0f;
            }
            return ret;
        }
        case DecelAccel: { // 13
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = 0.5f - pow_3(1.0f - value) / 2.0f;
            } else {
                ret = 0.5f + pow_3(value - 1.0f) / 2.0f;
            }
            return ret;
        }
        case AccelDecelFast: { // 11
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = pow_4(value) / 2.0f;
            } else {
                ret = (2.0f - pow_4(2.0f - value)) / 2.0f;
            }
            return ret;
        }
        case DecelAccelFast: { // 14
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = 0.5f - pow_4(1.0f - value) / 2.0f;
            } else {
                ret = 0.5f + pow_4(value - 1.0f) / 2.0f;
            }
        }
        case ForceInitial: // 15
            return 0.0f;
        case ForceFinal: // 16
            return 1.0f;
        case DecelerateSine: // 18
            return zsinf(value * PI_f / 2.0f);
        case AccelerateSine: // 19
            return 1.0f - zsinf(value * PI_f / 2.0f + PI_f / 2.0f);
        case DecelAccelSine: { // 20
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = zsinf(value * PI_f / 2.0f) / 2.0f;
            } else {
                ret = (1.0f - zsinf(value * PI_f / 2.0f)) / 2.0f + 0.5f;
            }
            return ret;
        }
        case AccelDecelSine: { // 21
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = (1.0f - zsinf(value * PI_f / 2.0f + PI_f / 2.0f)) / 2.0f;
            } else {
                ret = zsinf((value - 1.0f) * PI_f / 2.0f) / 2.0f + 0.5f;
            }
            return ret;
        }
        case AccelParabolaA: // 22
            return value = parabola(value, 0.25f);
        case AccelParabolaB: // 23
            return value = parabola(value, 0.30f);
        case AccelParabolaC: // 24
            return value = parabola(value, 0.35f);
        case AccelParabolaD: // 25
            return value = parabola(value, 0.38f);
        case AccelParabolaE: // 26
            return value = parabola(value, 0.40f);
        case DecelParabolaA: // 27
            value = 1.0f - value;
            return value = 1.0f - parabola(value, 0.25f);
        case DecelParabolaB: // 28
            value = 1.0f - value;
            return value = 1.0f - parabola(value, 0.30f);
        case DecelParabolaC: // 29
            value = 1.0f - value;
            return value = 1.0f - parabola(value, 0.35f);
        case DecelParabolaD: // 30
            value = 1.0f - value;
            return value = 1.0f - parabola(value, 0.38f);
        case DecelParabolaE: // 31
            value = 1.0f - value;
            return value = 1.0f - parabola(value, 0.40f);
        case Linear: // 0
        default:
            return value;
    }
}

#if INCLUDE_REPLACEMENT_CODE

/*
dllexport gnu_noinline float cdecl __interp_inner_thing_fast(int32_t mode, float current_time, float end_time) {
    if (end_time == 0.0f) {
        return 1.0f;
    }
    float value = current_time / end_time;
    float valueR = 1.0f - value;
    vec<float, 2> temp = { valueR, value };
    temp += temp;
    vec<float, 2> temp2 = temp;
    float ret = value;
    float retR = valueR;
    switch (expect_chance(mode, 0, 0.5)) {
        default:
            unreachable;
        case Linear: // 0
            return ret;
        case ForceInitial: // 15
            return 0.0f;
        case ForceFinal: // 16
            return 1.0f;
        case AccelerateFast: // 3
            ret *= value;
        case Accelerate: // 2
            ret *= value;
        case AccelerateSlow: // 1
            ret *= value;
            return ret;
        case DecelerateFast: // 6
            retR *= valueR;
        case Decelerate: // 5
            retR *= valueR;
        case DecelerateSlow: // 4
            retR *= valueR;
            return 1.0f - retR;
        case AccelDecelFast: { // 11
            temp2 *= temp;
        case AccelDecel: // 10
            temp2 *= temp;
        case AccelDecelSlow: // 9
            temp2 *= temp;
            float high = temp2[0];
            float low = temp2[1];
            high = 2.0f - high;
            float ret = value < 0.5f ? low : high;
            return ret * 0.5f;
        }
        case DecelAccelFast: // 14
            value += value;
            if (value < 1.0f) {
                ret = pow_4(1.0f - value) * -0.5f;
            } else {
                ret = pow_4(value - 1.0f) * 0.5f;
            }
            return ret + 0.5f;
        case DecelAccel: // 13
            value += value;
            if (value < 1.0f) {
                ret = pow_3(1.0f - value) * -0.5f;
            } else {
                ret = pow_3(value - 1.0f) * 0.5f;
            }
            return ret + 0.5f;
        case DecelAccelSlow: // 12
            value += value;
            if (value < 1.0f) {
                ret = pow_2(1.0f - value) * -0.5f;
            } else {
                ret = pow_2(value - 1.0f) * 0.5f;
            }
            return ret + 0.5f;
        case DecelerateSine: // 18
            return zsinf(value * HALF_PI_f);
        case AccelerateSine: // 19
            return 1.0f - zcosf(value * HALF_PI_f);
        case DecelAccelSine: // 20
            ret = zsinf(value * PI_f);
            if (value < 0.5f) {
                return ret * 0.5f;
            } else {
                return (1.0f - ret) * 0.5f + 0.5f;
            }
        case AccelDecelSine: // 21
            value += value;
            if (value < 1.0f) {
                ret = (1.0f - zcosf(value * HALF_PI_f)) * 0.5f;
            } else {
                ret = zsinf((value - 1.0f) * HALF_PI_f) * 0.5f + 0.5f;
            }
            return ret;
        case AccelParabolaA: // 22
            return parabola(value, 0.25f);
        case AccelParabolaB: // 23
            return parabola(value, 0.30f);
        case AccelParabolaC: // 24
            return parabola(value, 0.35f);
        case AccelParabolaD: // 25
            return parabola(value, 0.38f);
        case AccelParabolaE: // 26
            return parabola(value, 0.40f);
        case DecelParabolaA: // 27
            return 1.0f - parabola(valueR, 0.25f);
        case DecelParabolaB: // 28
            return 1.0f - parabola(valueR, 0.30f);
        case DecelParabolaC: // 29
            return 1.0f - parabola(valueR, 0.35f);
        case DecelParabolaD: // 30
            return 1.0f - parabola(valueR, 0.38f);
        case DecelParabolaE: // 31
            return 1.0f - parabola(valueR, 0.40f);
    }
}
*/

dllexport gnu_noinline float cdecl interp1D_fast2(int32_t mode, float current_time, float end_time) {
    if (expect(end_time == 0.0f, false)) {
        return 1.0f;
    }
    float value = current_time / end_time;

    float value_2x = value + value;
    vec<float, 4> values_2x = { value_2x, value_2x, value_2x, value_2x };
    values_2x = (vec<float, 4>){ 0.0f, 0.0f, 0.0f, 1.0f } - values_2x;
    values_2x += (vec<float, 4>) { 0.0f, -1.0f, 0.0f, 0.0f };

    float valueR = 1.0f - value;

    values_2x[0] = valueR + valueR;
    vec<float, 4> values_2xB = values_2x;

    /*
    vec<float, 4> values_2x = { valueR, valueR, value, value };
    values_2x += values_2x;
    vec<float, 4> values_2xB = values_2x;

    vec<float, 4> values_2xC = values_2xB;
    values_2xC = (vec<float, 4>){ 0.0f, 0.0f, 0.0f, 1.0f } - values_2xC;
    values_2xC += (vec<float, 4>) { 0.0f, 0.0f, -1.0f, 0.0f };
    vec<float, 4> values_2xD = values_2xC;
    */
    float ret = value;
    float retR = valueR;

    static constexpr float PARABOLA_LOOKUP_A[] = {
        -0.25f, -0.3f, -0.35f, -0.38f, -0.4f
    };
    static constexpr float PARABOLA_LOOKUP_B[] = {
        2.0f, 2.5f, bitcast<float>(0x40555557), bitcast<float>(0x40855554), 5.0f
    };
    static constexpr float PARABOLA_LOOKUP_C[] = {
        -0.125f, -0.225f, bitcast<float>(0xBED11111), bitcast<float>(0xBF1A06D3), -0.8f
    };
    switch (expect_chance(mode, 0, 0.5)) {
        default:
            unreachable;
        case Linear: // 0
            return ret;
        case ForceInitial: // 15
            return 0.0f;
        case ForceFinal: // 16
            return 1.0f;
        case AccelerateFast: // 3
            ret *= value;
        case Accelerate: // 2
            ret *= value;
        case AccelerateSlow: // 1
            ret *= value;
            return ret;
        case DecelerateFast: // 6
            retR *= valueR;
        case Decelerate: // 5
            retR *= valueR;
        case DecelerateSlow: // 4
            retR *= valueR;
            return 1.0f - retR;
        {
        case AccelDecelFast: // 11
            values_2x *= values_2xB;
        case AccelDecel: // 10
            values_2x *= values_2xB;
        case AccelDecelSlow: // 9
            values_2x *= values_2xB;
            float high = values_2x[0];
            float low = values_2x[2];
            high = 2.0f - high;
            float ret = value < 0.5f ? low : high;
            return ret * 0.5f;
        }
        {
        case DecelAccelFast: // 14
            values_2x *= values_2xB;
        case DecelAccel: // 13
            values_2x *= values_2xB;
        case DecelAccelSlow: // 12
            values_2x *= values_2xB;
            float high = values_2x[1];
            float low = -values_2x[3];
            float ret = value < 0.5f ? low : high;
            return ret * 0.5f + 0.5f;
        }
        case DecelerateSine: // 18
            return zsinf(value * HALF_PI_f);
        case AccelerateSine: // 19
            return 1.0f - zsinf(value * HALF_PI_f + HALF_PI_f);
        case DecelAccelSine: // 20
            ret = zsinf(value * PI_f);
            if (value < 0.5f) {
                return ret * 0.5f;
            } else {
                return (1.0f - ret) * 0.5f + 0.5f;
            }
        case AccelDecelSine: // 21
            if (value < 0.5f) {
                return (1.0f - zsinf(value_2x * HALF_PI_f + HALF_PI_f)) * 0.5f;
            } else {
                return zsinf((value_2x - 1.0f) * HALF_PI_f) * 0.5f + 0.5f;
            }
        case AccelParabolaA: // 22
        case AccelParabolaB: // 23
        case AccelParabolaC: // 24
        case AccelParabolaD: // 25
        case AccelParabolaE: // 26
            value += PARABOLA_LOOKUP_A[mode - AccelParabolaA];
            value *= value;
            value *= PARABOLA_LOOKUP_B[mode - AccelParabolaA];
            value += PARABOLA_LOOKUP_C[mode - AccelParabolaA];
            return value;
        case DecelParabolaA: // 27
        case DecelParabolaB: // 28
        case DecelParabolaC: // 29
        case DecelParabolaD: // 30
        case DecelParabolaE: // 31
            valueR += PARABOLA_LOOKUP_A[mode - DecelParabolaA];
            valueR *= valueR;
            valueR *= PARABOLA_LOOKUP_B[mode - DecelParabolaA];
            valueR += PARABOLA_LOOKUP_C[mode - DecelParabolaA];
            return 1.0f - valueR;
    }
}

dllexport gnu_noinline float cdecl __interp_inner_thing_fast(int32_t mode, float current_time, float end_time) {
    if (end_time == 0.0f) {
        return 1.0f;
    }
    float value = current_time / end_time;
    float valueR = 1.0f - value;
    vec<float, 2> temp = { valueR, value };
    temp += temp;
    vec<float, 2> temp2 = temp;
    float ret = value;
    float retR = valueR;
    switch (expect_chance(mode, 0, 0.5)) {
        default:
            unreachable;
        case Linear: // 0
            return ret;
        case ForceInitial: // 15
            return 0.0f;
        case ForceFinal: // 16
            return 1.0f;
        case AccelerateFast: // 3
            ret *= value;
        case Accelerate: // 2
            ret *= value;
        case AccelerateSlow: // 1
            ret *= value;
            return ret;
        case DecelerateFast: // 6
            retR *= valueR;
        case Decelerate: // 5
            retR *= valueR;
        case DecelerateSlow: // 4
            retR *= valueR;
            return 1.0f - retR;
        case AccelDecelFast: { // 11
            temp2 *= temp;
        case AccelDecel: // 10
            temp2 *= temp;
        case AccelDecelSlow: // 9
            temp2 *= temp;
            float high = temp2[0];
            float low = temp2[1];
            high = 2.0f - high;
            float ret = value < 0.5f ? low : high;
            return ret * 0.5f;
        }
        case DecelAccelFast: // 14
            value += value;
            if (value < 1.0f) {
                ret = pow_4(1.0f - value) * -0.5f;
            } else {
                ret = pow_4(value - 1.0f) * 0.5f;
            }
            return ret + 0.5f;
        case DecelAccel: // 13
            value += value;
            if (value < 1.0f) {
                ret = pow_3(1.0f - value) * -0.5f;
            } else {
                ret = pow_3(value - 1.0f) * 0.5f;
            }
            return ret + 0.5f;
        case DecelAccelSlow: // 12
            value += value;
            if (value < 1.0f) {
                ret = pow_2(1.0f - value) * -0.5f;
            } else {
                ret = pow_2(value - 1.0f) * 0.5f;
            }
            return ret + 0.5f;
        case DecelerateSine: // 18
            return zsinf(value * HALF_PI_f);
        case AccelerateSine: // 19
            return 1.0f - zcosf(value * HALF_PI_f);
        case DecelAccelSine: // 20
            ret = zsinf(value * PI_f);
            if (value < 0.5f) {
                return ret * 0.5f;
            } else {
                return (1.0f - ret) * 0.5f + 0.5f;
            }
        case AccelDecelSine: // 21
            value += value;
            if (value < 1.0f) {
                ret = (1.0f - zcosf(value * HALF_PI_f)) * 0.5f;
            } else {
                ret = zsinf((value - 1.0f) * HALF_PI_f) * 0.5f + 0.5f;
            }
            return ret;
        case AccelParabolaA: // 22
            return parabola(value, 0.25f);
        case AccelParabolaB: // 23
            return parabola(value, 0.30f);
        case AccelParabolaC: // 24
            return parabola(value, 0.35f);
        case AccelParabolaD: // 25
            return parabola(value, 0.38f);
        case AccelParabolaE: // 26
            return parabola(value, 0.40f);
        case DecelParabolaA: // 27
            return 1.0f - parabola(valueR, 0.25f);
        case DecelParabolaB: // 28
            return 1.0f - parabola(valueR, 0.30f);
        case DecelParabolaC: // 29
            return 1.0f - parabola(valueR, 0.35f);
        case DecelParabolaD: // 30
            return 1.0f - parabola(valueR, 0.38f);
        case DecelParabolaE: // 31
            return 1.0f - parabola(valueR, 0.40f);
    }
}
#endif

template <typename T>
struct ZUNInterp { //       0x54    0x40    0x2C
    //                      T3      T2      T1
    T initial_value; //     0x0     0x0     0x0
    T final_value; //       0xC     0x8     0x4
    T bezier1; //           0x18    0x10    0x8
    T bezier2; //           0x24    0x18    0xC
    T current; //           0x30    0x20    0x10
    Timer time; //          0x3C    0x28    0x14
    int32_t end_time; //    0x4C    0x38    0x24
    int32_t mode; //        0x50    0x3C    0x28

    dllexport gnu_noinline void thiscall initialize(int32_t end_time, int32_t mode, const T& initial_value, const T& final_value) {
        this->end_time = end_time;
        this->mode = mode;
        this->initial_value = initial_value;
        this->final_value = final_value;
        this->current = initial_value;
        this->time = 0;
    }

    dllexport gnu_noinline bool thiscall __running() {
        return this->time < this->end_time;
    }

    /*
    template <typename T>
    T Interp<T>::step() {
    
        if (this->end_time > 0) {
            ++this->time;

            Timer *time = &this->time;

            if (*time >= this->end_time) {
                this->time = this->end_time;
            }

            this->reset_end_time();

            if (this->mode != INTERP_CONSTANT_VELOCITY && this->mode != INTERP_CONSTANT_ACCELERATION) {
                return this->final_value;
            } else {
                return this->initial_value;
            }
        }

        if (this->end_time == 0) {
            if (this->mode != INTERP_CONSTANT_VELOCITY && this->mode != INTERP_CONSTANT_ACCELERATION) {
                return this->final_value;
            } else {
                return this->initial_value;
            }
        }

        if (this->mode == INTERP_CONSTANT_VELOCITY) {
            this->initial_value = (T)(this->initial_value) + this->final_value;
            this->current = this->initial_value;
        }
        else if (this->mode == INTERP_CONSTANT_ACCELERATION) {
            this->initial_value = (T)(this->initial_value) + this->extra2;
            this->extra2 = this->extra2 + this->final_value;
            this->current = this->initial_value;
        }
        else if (this->mode == INTERP_BEZIER) {
            float t = this->time.get_current_f() / this->end_time;
            float a = pow_2(t - 1.0f) * (2.0f * t + 1.0f);
            float b = pow_2(t) * (3.0f - 2.0f * t);
            float c = pow_2(1.0f - t) * t;
            float d = (t - 1.0f) * pow_2(t);
            this->current = this->initial_value * a + this->final_value * b + this->extra1 * c + this->extra2 * d;
        }
        else {
            float ebp24 = this->handle_other_interpolation();
            this->current = (this->final_value - this->initial_value) * ebp24 + this->initial_value;
        }

        return this->current;
    }
    */

    dllexport gnu_noinline T cdecl recalculate_current_instead_of_just_reading_the_current_field() {
        if (this->mode == ConstantVelocity) {
            this->initial_value = this->initial_value + this->final_value;
            this->current = this->initial_value;
        } else if (this->mode == ConstantAccel) {
            this->initial_value = this->initial_value + this->bezier2;
            this->bezier2 = this->bezier2 + this->final_value;
            this->current = this->initial_value;
        } else if (this->mode == Bezier) {
            float t = this->time.current_f / this->end_time;
            float a = pow_2(t - 1.0f) * (2.0f * t + 1.0f);
            float b = pow_2(t) * (3.0f - 2.0f * t);
            float c = pow_2(1.0f - t) * t;
            float d = (t - 1.0f) * pow_2(t);
            this->current = this->initial_value * a + this->final_value * b + this->bezier1 * c + this->bezier2 * d;
        } else {
            float interp_value = __interp_inner_thing(this->mode, this->time.current_f, (float)this->end_time);
            this->current = (this->final_value - this->initial_value) * interp_value + this->initial_value;
        }

        return this->current;
    }

    dllexport gnu_noinline T cdecl step() {
        if (this->end_time > 0) {
            ++this->time;
            if (this->time >= this->end_time) {
                this->time = this->end_time;

                this->end_time = 0;

                if (this->mode != ConstantVelocity && this->mode != ConstantAccel) {
                    return this->final_value;
                } else {
                    return this->initial_value;
                }
            }
        }
        else if (this->end_time == 0) {
            if (this->mode != ConstantVelocity && this->mode != ConstantAccel) {
                return this->final_value;
            } else {
                return this->initial_value;
            }
        }

        if (this->mode == ConstantVelocity) {
            this->initial_value = this->initial_value + this->final_value;
            this->current = this->initial_value;
        }
        else if (this->mode == ConstantAccel) {
            this->initial_value = this->initial_value + this->bezier2;
            this->bezier2 = this->bezier2 + this->final_value;
            this->current = this->initial_value;
        }
        else if (this->mode == Bezier) {
            float t = this->time.current_f / this->end_time;
            float a = pow_2(t - 1.0f) * (2.0f * t + 1.0f);
            float b = pow_2(t) * (3.0f - 2.0f * t);
            float c = pow_2(1.0f - t) * t;
            float d = (t - 1.0f) * pow_2(t);
            this->current = this->initial_value * a + this->final_value * b + this->bezier1 * c + this->bezier2 * d;
        }
        else {
            float interp_value = __interp_inner_thing(this->mode, this->time.current_f, (float)this->end_time);
            this->current = (this->final_value - this->initial_value) * interp_value + this->initial_value;
        }

        return this->current;
    }

#if INCLUDE_REPLACEMENT_CODE
    dllexport gnu_noinline T thiscall step_fast() {
        int32_t end_time = this->end_time;
        if (expect(end_time > 0, true)) {
            if (++this->time >= end_time) {
                this->time = end_time;
                goto TimeEnd;
            }
        } else if (expect(end_time == 0, false)) {
    TimeEnd:
            if constexpr (sizeof(T) <= 8) {
                return (&this->final_value)[0xFFFDFF7F & 1 << this->mode ? -1 : 0];
            } else {
                switch (this->mode) {
                    case ConstantVelocity: case ConstantAccel:
                        return this->final_value;
                    case Linear:
                    case AccelerateSlow: case Accelerate: case AccelerateFast:
                    case DecelerateSlow: case Decelerate: case DecelerateFast:
                    case Bezier:
                    case AccelDecelSlow: case AccelDecel: case AccelDecelFast:
                    case DecelAccelSlow: case DecelAccel: case DecelAccelFast:
                    case ForceInitial: case ForceFinal:
                    case DecelerateSine: case AccelerateSine:
                    case DecelAccelSine: case AccelDecelSine:
                    case AccelParabolaA: case AccelParabolaB: case AccelParabolaC: case AccelParabolaD: case AccelParabolaE:
                    case DecelParabolaA: case DecelParabolaB: case DecelParabolaC: case DecelParabolaD: case DecelParabolaE:
                        return this->initial_value;
                    default:
                        unreachable;
                }
            }
        }
        int32_t mode = this->mode;
        switch (expect(mode, 0)) {
            case ConstantVelocity:
                // Final value is per-tick velocity
                return this->current = (this->initial_value += this->final_value);
            case ConstantAccel: {
                // Bezier2 is per-tick velocity
                // Final value is per-tick acceleration
                T temp = this->bezier2;
                this->bezier2 = temp + this->final_value;
                return this->current = (this->initial_value += temp);
            }
            default: {
                float current_time = this->time.current_f;
                float end_time_f = end_time;
                if (expect_chance(mode == Bezier, false, 0.75)) { // 8
                    float t = current_time / end_time_f;

                    float t_plus_one = t + 1.0f;
                    float two_t = t + t;
                    float t_minus_one = t - 1.0f;
                    float one_minus_t = 1.0f - t;
                    float two_t_plus_one = two_t + 1.0f;

                    float initial_factor = t_minus_one * t_minus_one * two_t_plus_one;
                    float final_factor = (3.0f - two_t) * t * t;
                    float bezier1_factor = one_minus_t * one_minus_t * t;
                    float bezier2_factor = t_minus_one * t * t;

                    T value = this->initial_value * initial_factor;
                    value += this->final_value * final_factor;
                    value += this->bezier1 * bezier1_factor;
                    return this->current = value + this->bezier2 * bezier2_factor;
                } else {
                    float interp_value = __interp_inner_thing(mode, current_time, end_time_f);
                    return this->current = lerp(this->initial_value, this->final_value, interp_value);
                }
            }
        }
    }
#endif
};

dllexport ZUNInterp<uint8_t> byte_interp;
dllexport ZUNInterp<int32_t> int_interp;
dllexport ZUNInterp<float> float_interp;
dllexport ZUNInterp<Float2> float2_interp;
dllexport ZUNInterp<Float3> float3_interp;
//dllexport ZUNInterp<Int3> int3_interp;

dllexport void export_my_step_funcs_plz() {
    byte_interp.step_fast();
    int_interp.step_fast();
    float_interp.step_fast();
    float2_interp.step_fast();
    float3_interp.step_fast();
    //int3_interp.step_fast();
}

template <typename T, typename E>
struct ZUNInterpExImpl { //             0x64    0x4C
    static inline constexpr size_t AXIS_COUNT = sizeof(T) / sizeof(E);
    T current; //                       0x0     0x0
    T initial_value; //                 0xC     0x8
    T final_value; //                   0x18    0x10
    T bezier1; //                       0x24    0x18
    T bezier2; //                       0x30    0x20
    Timer time; //                      0x3C    0x28
    int32_t end_time; //                0x4C    0x38
    int32_t axis_modes[AXIS_COUNT]; //  0x50    0x3C
    int32_t combined_mode; //           0x5C    0x44
    union {
        uint32_t flags; //              0x60    0x48
        struct {
            uint32_t interp_per_axis : 1; // 1
        };
    };
};
template <typename T> struct ZUNInterpEx;
template <> struct ZUNInterpEx<Float2> : ZUNInterpExImpl<Float2, float> {};
template <> struct ZUNInterpEx<Float3> : ZUNInterpExImpl<Float3, float> {};
template <> struct ZUNInterpEx<Int2> : ZUNInterpExImpl<Int2, int32_t> {};
template <> struct ZUNInterpEx<Int3> : ZUNInterpExImpl<Int3, int32_t> {};

// size: 0x16C
struct StageCamera {
    Float3 position; // 0x0
    Float3 facing; // 0xC
    Float3 rotation; // 0x18
    Float3 facing_normalized; // 0x24
    Float3 __float3_30; // 0x30
    Float3 __shaking_float3_A; // 0x3C
    Float3 __shaking_float3_B; // 0x48
    float fov; // 0x54
    Int2 window_resolution; // 0x58
    D3DMATRIXZ view_matrix; // 0x60
    D3DMATRIXZ projection_matrix; // 0xA0
    D3DVIEWPORT9 viewport; // 0xE0
    int32_t camera_index; // 0xF8
    Int2 __int2_FC; // 0xFC
    D3DVIEWPORT9 __viewport_104; // 0x104
    Float2 __float2_11C; // 0x11C
    Float2 __float2_124; // 0x124
    Float2 __float2_array_12C[3]; // 0x12C
    Float3 __float3_144; // 0x144
    StageSky sky; // 0x150
    // 0x16C
};

#define SetInstr(value) \
current_instruction = (decltype(current_instruction))(value)

#define IndexInstr(offset) \
SetInstr((intptr_t)current_instruction + (offset))

#define ByteArg(number) \
(((uint8_t*)current_instruction->args)[(number)])

#define OneBitArg(number) \
(ByteArg(number) & 0b1)

#define ShortArg(number) \
(((int16_t*)current_instruction->args)[(number)])

#define UShortArg(number) \
(((uint16_t*)current_instruction->args)[(number)])

#define RawArg(number) \
(((EclArg*)current_instruction->args)[(number)])

#define IntArg(number) \
(((int32_t*)current_instruction->args)[(number)])

#define UIntArg(number) \
(((uint32_t*)current_instruction->args)[(number)])

#define FloatArg(number) \
(((float*)current_instruction->args)[(number)])

#define Float2Arg(number) \
(((Float2*)current_instruction->args)[(number)])

#define Float3Arg(number) \
(((Float3*)current_instruction->args)[(number)])

#define StringArg(offset) \
((const char*)(current_instruction->args + (offset)))

#define TypeArg(type, number) \
(((type*)current_instruction->args)[(number)])

enum CriticalSectionIndex {
    UpdateFuncRegistry_CS = 0,
    __unused_cs_1 = 1,
    FileIO_CS = 2,
    Logging_CS = 3,
    __unused_cs_2 = 4,
    Menu_CS = 5,
    __supervisor_thread_cs_1 = 6,
    __unused_cs_3 = 7,
    __unused_cs_4 = 8,
    AnmList_CS = 9,
    RNG_CS = 10,
    SoundManagerA_CS = 11,
    SoundManagerB_CS = 12,
    __unused_cs_5 = 13,
};

// size: 0x438
struct CriticalSectionManager {
    std::array<std::mutex, 22> sections; // 0x0
    uint8_t section_depths[22]; // 0x420
    bool enable_sections; // 0x436
    // 0x438

    std::mutex& get_mutex(size_t index) {
        return this->sections[index];
    }
};

static CriticalSectionManager CRITICAL_SECTION_MANAGER;

dllexport std::mutex ANM_LIST_MUTEX asm_symbol_abs(Rx1C03F0);

enum UpdateFuncRet : int32_t {
    UpdateFuncDeleteCurrentThenNext = 0,
    UpdateFuncNext = 1,
    UpdateFuncRepeatCurrent = 2,
    UpdateFuncEnd1 = 3,
    UpdateFuncEnd0 = 4,
    UpdateFuncEndN1 = 5,
    UpdateFuncRestartTick = 6,
    UpdateFuncCleanupThenNext = 7,
    UpdateFuncEnd0Dupe = 8,
};

#define UpdateFuncCC cdecl

typedef UpdateFuncRet(UpdateFuncCC UpdateFunction)(void*);
typedef ZUNResult(UpdateFuncCC UpdateFunc)(void*);
typedef ZUNResult(UpdateFuncCC UpdateFuncInit)(void*);
typedef ZUNResult(UpdateFuncCC UpdateFuncCleanup)(void*);

// size: 0x10
struct ZUNTask {
    //void* vftable; // 0x0
    union {
        uint32_t task_flags; // 0x4
        struct {
            uint32_t : 1;
            uint32_t __unknown_task_flag_A : 1;
        };
    };
    UpdateFunction* on_tick_func; // 0x8
    UpdateFunction* on_draw_func; // 0xC
    // 0x10

    dllexport virtual ~ZUNTask() {}
};

// size: 0x4
struct dllexport RngImpl {
    uint32_t rng_value; // 0x0
    // 0x4

    // Rx22C30
    RngImpl() {
        this->rng_value = validate_seed(1);
    }

    // Rx22C00
    static uint32_t validate_seed(uint32_t value) {
        value %= INT32_MAX;
        if (!value) {
            value = 1;
        }
        return value;
    }

    // Rx22CB0
    static uint32_t generate_random(uint32_t seed) {
        uint64_t temp = 0xBC8Full * seed;
        temp = (temp >> 31) + (temp & INT32_MAX);
        uint64_t value;
        if (temp < INT32_MAX) {
            value = temp;
        } else {
            value = temp - INT32_MAX;
        }
        temp = value;
        return temp;
    }

    // RxDDA10
    void initialize(uint32_t seed) {
        this->rng_value = validate_seed(seed);
    }

    // Rx22CB0
    uint32_t rand_uint() {
        this->rng_value = generate_random(this->rng_value);
        return this->rng_value;
    }

#if INCLUDE_REPLACEMENT_CODE
    static uint32_t generate_random_fast(uint32_t seed) {
        uint64_t temp = 0x1791Eull * seed;
        uint32_t high = temp >> 32;
        uint32_t low = (uint32_t)temp >> 1;
        //low += high;
        //return (int32_t)low >= 0 ? low : low - INT32_MAX;
        __asm__ (
            "ADD %[high], %[low] \n"
            "LEA 0x80000001(%[low]), %[high] \n"
            "CMOVS %[high], %[low]"
            : asm_arg("+a", low), asm_arg("+d", high)
            :
            : "cc"
        );
        return low;
    }

    int32_t rand_uint_fast() {
        this->rng_value = generate_random_fast(this->rng_value);
        return this->rng_value;
    }
#endif
};

// size: 0x18
struct dllexport Rng {
    uint32_t index = 0; // 0x0
    RngImpl impl; // 0x4
    uint32_t minimum = 0; // 0x8
    uint32_t maximum = 0x00FFFF00; // 0xC
    uint32_t range = 0; // 0x10
    uint32_t prev_value = 0; // 0x14
    uint32_t __generator_index; // 0x18
    // 0x1C

    // Rx22C50
    Rng(uint32_t index) {
        this->__generator_index = index;
    }

    // RxD9940
    void initialize(uint32_t seed) {
        //std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(RNG_MUTEX));
        this->minimum = 0;
        this->maximum = std::numeric_limits<uint32_t>::max() / 2;
        this->range = this->maximum - this->minimum;
        RngImpl* impl = &this->impl;
        impl->initialize(seed);
    }

    // Rx49D20
    uint8_t rand_byte() {
        //std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(RNG_MUTEX));
        this->prev_value = this->impl.rand_uint();
        return this->prev_value;
    }

    // Rx97470
    uint16_t rand_ushort() {
        //std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(RNG_MUTEX));
        this->prev_value = this->impl.rand_uint();
        return this->prev_value;
    }

    // Rx23EE0
    uint32_t rand_uint() {
        //std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(RNG_MUTEX));
        this->prev_value = this->impl.rand_uint();
        return this->prev_value % this->range;
    }

    // Rx23EA0
    uint32_t rand_uint_range(uint32_t range) {
        if (range) {
            return this->rand_uint() % range;
        } else {
            return 0;
        }
    }

    // Rx29830
    float rand_float() {
        float value = this->rand_uint();
        float range = (float)this->range - 1.0f;
        return value / range;
    }

    // Rx29800
    float rand_float_range(float range) {
        return this->rand_float() * range;
    }

    // Rx298E0
    float rand_float_signed() {
        float value = this->rand_uint();
        float range = (float)this->range / 2.0f - 1.0f;
        return (value / range) - 1.0f;
    }

    // Rx298B0
    float rand_float_signed_range(float range) {
        return this->rand_float_signed() * range;
    }

    // Rx298D0
    float rand_angle() {
        return this->rand_float_signed() * PI_f;
    }

    // Rx104A60
    float rand_angle_2() {
        float value = this->rand_uint();
        float range = (float)this->range / (PI_f * 2.0f) - PI_f;
        return (value / range) - PI_f;
    }

#if INCLUDE_REPLACEMENT_CODE
    
    int32_t rand_uint_fast() {
        //std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(RNG_MUTEX));
        this->prev_value = this->impl.rand_uint_fast();
        return this->prev_value;
    }

    // Rx23EA0
    uint32_t rand_uint_range_fast(uint32_t range) {
        if (expect(range != 0, true)) {
            return this->rand_uint_fast() % range;
        } else {
            return 0;
        }
    }

    float rand_float_fast() {
        float value = this->rand_uint_fast();
        const float range = (float)INT32_MAX - 1.0f;
        return value / range;
    }

    float rand_float_range_fast(float range) {
        return this->rand_float_fast() * range;
    }

    float rand_float_signed_fast() {
        float value = this->rand_uint_fast();
        const float range = (float)INT32_MAX / 2.0f - 1.0f;
        return (value / range) - 1.0f;
    }

    float rand_float_signed_range_fast(float range) {
        return this->rand_float_signed_fast() * range;
    }

    float rand_angle_fast() {
        return this->rand_float_signed_fast() * PI_f;
    }

    float rand_angle_2_fast() {
        float value = this->rand_uint_fast();
        const float range = (float)INT32_MAX / (PI_f * 2.0f) - PI_f;
        return (value / range) - PI_f;
    }
#endif
};

// Rx1BA4A8
dllexport Rng REPLAY_RNG = Rng(1);
// Rx1BA4C4
dllexport Rng RNG = Rng(1);

typedef struct GameThread GameThread;
typedef struct Gui Gui;

dllexport GameThread* GAME_THREAD_PTR;
dllexport Gui* GUI_PTR;

// size: 0xB0
struct Config {
    unknown_fields(0xB0);
};

// size: 0x110
struct GameThread : ZUNTask {
    //ZUNTask base; // 0x0
    Timer __timer_10; // 0x10
    Timer __timer_20; // 0x20
    int32_t stage_number; // 0x30
    int32_t chapter; // 0x34
    Config config; // 0x38
    union {
        uint32_t flags; // 0xE8
        struct {
            uint8_t __unknown_flag_A : 1; // 1
            uint8_t __unknown_flag_B : 1; // 2
            uint8_t skip_flag : 1; // 3
            uint8_t __unknown_flag_H : 1; // 4
            uint8_t __unknown_flag_I : 1; // 5
            uint8_t __unknown_flag_L : 1; // 6
            uint8_t __unknown_flag_M : 1; // 7
            uint8_t __unknown_flag_J : 1; // 8
            uint8_t __unknown_flag_D : 1; // 9
            uint8_t : 1; // 10
            uint8_t __unknown_flag_C : 1; // 11
            uint8_t __unknown_flag_K : 1; // 12
            uint8_t : 2; // 13-14
            uint8_t __unknown_flag_E : 1; // 15
            uint8_t : 1; // 16
            uint8_t __unknown_flag_F : 1; // 17
            uint8_t __unknown_flag_G : 1; // 18
        };
    };
    // 0xEC

    inline bool __is_paused() {
        return this->__unknown_flag_A | this->skip_flag;
    }

    bool __get_unknown_flag_B() {
        return this->__unknown_flag_B;
    }
};

inline bool __game_thread_not_paused_and_flag_B_is_not_true() {
    GameThread* game_thread = GAME_THREAD_PTR;
    return !(game_thread && game_thread->__is_paused() && game_thread->__get_unknown_flag_B());
}

inline bool __game_thread_flag_B_is_not_true() {
    GameThread* game_thread = GAME_THREAD_PTR;
    return !(game_thread && game_thread->__get_unknown_flag_B());
}

// size: 0x140
struct MsgVM {

};


struct Gui {
    unknown_fields(0x1BC); // 0x0
    MsgVM* msg_vm; // 0x1BC
    // 0x1C0

    // Rx78160
    dllexport gnu_noinline BOOL thiscall msg_vm_active() {
        return this->msg_vm != NULL;
    }
};

typedef struct BulletManager BulletManager;
typedef struct Player Player;
typedef struct EnemyManager EnemyManager;
typedef struct ItemManager ItemManager;
typedef struct Spellcard Spellcard;
typedef struct LaserManager LaserManager;
typedef struct BombManager BombManager;
typedef struct PopupManager PopupManager;
typedef struct EffectManager EffectManager;
typedef struct HitboxManager HitboxManager;
typedef struct PlayerStoneManager PlayerStoneManager;
typedef struct StoneManager StoneManager;

enum StoneColor {
    RedStone = 0,
    BlueStone = 1,
    YellowStone = 2,
    GreenStone = 3
};

/*
struct CPlayerStats // sizeof=0xF0
00000000 {                                       // XREF: CGameGlobals/r
00000000     __int64 score;
00000008     int character;
0000000C     int hyper_stone_raw;
00000010     int focus_stone_raw;
00000014     int unfocus_stone_raw;
00000018     int assist_stone_raw;
0000001C     int hyper_stone;
00000020     int focus_stone;
00000024     int unfocus_stone;
00000028     int assist_stone;
0000002C     bool hyper_copy;
0000002D     bool focus_copy;
0000002E     bool unfocus_copy;
0000002F     bool assist_copy;
00000030     int power;
00000034     int max_power;
00000038     int power_divisor;
0000003C     int field_3C;
00000040     int piv_base;
00000044     int piv;
00000048     int piv_divisor;
0000004C     int hyper_meter;
00000050     int hyper_meter_max;
00000054     int hyper_duration;
00000058     int field_58;
0000005C     int summon_meter;
00000060     int summon_meter_max;
00000064     int red_priority;
00000068     int blue_priority;
0000006C     int yellow_priority;
00000070     int green_priority;
00000074     int red_level;
00000078     int blue_level;
0000007C     int yellow_level;
00000080     int green_level;
00000084     int red_spawns;
00000088     int blue_spawns;
0000008C     int yellow_spawns;
00000090     int green_spawns;
00000094     int total_spawns;
00000098     int next_stone_enemy;
0000009C     int focus_damage_boost;
000000A0     int unfocus_damage_boost;
000000A4     char hyper_meter_fast_fill;
000000A5     char field_A5;
000000A6     // padding byte
000000A7     // padding byte
000000A8     int hyper_duration_boost;
000000AC     int stone_damage_boost;
000000B0     bool can_delete_on_graze;
000000B1     // padding byte
000000B2     // padding byte
000000B3     // padding byte
000000B4     int graze_delete_meter;
000000B8     int lives;
000000BC     int max_lives;
000000C0     int life_fragments;
000000C4     int lives_added;
000000C8     int field_C8;
000000CC     int bombs;
000000D0     int bomb_fragments;
000000D4     int bombs_on_new_life;
000000D8     int max_bombs;
000000DC     int chapter_total;
000000E0     int chapter_destroy;
000000E4     int graze;
000000E8     int hitbox_scale;
000000EC     int field_EC;
000000F0 };
*/

/// GLOBALS

// size: 0xF0
struct GlobalsSide {
    uint64_t score; // 0x0
    int32_t character; // 0x8
    
    int32_t story_stone; // 0xC
    int32_t narrow_shot_stone; // 0x10
    int32_t wide_shot_stone; // 0x14
    int32_t assist_stone; // 0x18
    
    int32_t story_stone_raw; // 0x1C
    int32_t narrow_shot_stone_raw; // 0x20
    int32_t wide_shot_stone_raw; // 0x24
    int32_t assist_stone_raw; // 0x28
    
    uint8_t story_stone_copy; // 0x2C
    uint8_t narrow_shot_stone_copy; // 0x2D
    uint8_t wide_shot_stone_copy; // 0x2E
    uint8_t assist_stone_copy; // 0x2F
    
    int32_t current_power; // 0x30 (0..400)
    int32_t max_power; // 0x34 (400..400)
    int32_t power_per_level; // 0x38 (100..400)
    int32_t __int_3C; // 0x3C (0..1,000,000)
    int32_t piv_base; // 0x40 (10,000..1,000,000)
    int32_t piv; // 0x44 (0..1,000,000)
    int32_t piv_divisor; // 0x48 (5,000..10,000)
    int32_t hyper_meter; // 0x4C (0..500)
    int32_t hyper_meter_max; // 0x50 (100..500)
    int32_t hyper_duration; // 0x54 (1..100)
    int __dword_58; // 0x58
    int32_t summon_meter; // 0x5C (0..10,000)
    int32_t summon_meter_max; // 0x60 (0..5000)
    int32_t stone_priority_red; // 0x64 (0..1,000)
    int32_t stone_priority_blue; // 0x68 (0..1,000)
    int32_t stone_priority_yellow; // 0x6C (0..1,000)
    int32_t stone_priority_green; // 0x70 (0..1,000)
    int32_t stone_level_red; // 0x74 (0..4)
    int32_t stone_level_blue; // 0x78 (0..4)
    int32_t stone_level_yellow; // 0x7C (0..4)
    int32_t stone_level_green; // 0x80 (0..4)
    int32_t stone_enemy_count_red; // 0x84 (0..999,999)
    int32_t stone_enemy_count_blue; // 0x88 (0..999,999)
    int32_t stone_enemy_count_yellow; // 0x8C (0..999,999)
    int32_t stone_enemy_count_green; // 0x90 (0..999,999)
    int32_t stone_enemy_count_total; // 0x94 (0..999,999)
    int32_t next_stone_enemy_type; // 0x98 (-1..4)
    int32_t __int_9C; // 0x9C (0..100)
    int32_t __int_A0; // 0xA0 (0..100)
    uint8_t hyper_meter_fast_fill; // 0xA4
    uint8_t __byte_A5; // 0xA5
    padding_bytes(0x2); // 0xA6
    int32_t __int_A8; // 0xA8 (0..100)
    int32_t __int_AC; // 0xAC (0..100)
    uint8_t __byte_B0; // 0xB0
    padding_bytes(0x3); // 0xB1
    int32_t __int_B4; // 0xB4 (0..100)
    int32_t life_stocks; // 0xB8 (-1..7)
    int32_t life_stock_max; // 0xBC (0..7)
    int32_t life_fragments; // 0xC0 (0..10)
    int32_t lives_added; // 0xC4 (0..100)
    int32_t __int_C8; // 0xC8 (0..999)
    int32_t bomb_stocks; // 0xCC (0..10)
    int32_t bomb_fragments; // 0xD0 (0..10)
    int32_t bomb_stocks_for_new_life; // 0xD4 (2..10)
    int32_t bomb_stock_max; // 0xD8 (0..7)
    int32_t __chapter_total; // 0xDC (0..1,000,000)
    int32_t __chapter_destroy; // 0xE0 (0..1,000,000)
    int32_t graze; // 0xE4 (0..99,999,999)
    int32_t hitbox_scale; // 0xE8
    uint32_t side_index; // 0xEC
    // 0xF0
};

// size: 0x30
struct GameSide {
    BulletManager* bullet_manager_ptr; // 0x0
    Player* player_ptr; // 0x4
    EnemyManager* enemy_manager_ptr; // 0x8
    ItemManager* item_manager_ptr; // 0xC
    Spellcard* spellcard_ptr; // 0x10
    LaserManager* laser_manager_ptr; // 0x14
    BombManager* bomb_manager_ptr; // 0x18
    PopupManager* popup_manager_ptr; // 0x1C
    EffectManager* effect_manager_ptr; // 0x20
    GlobalsSide* globals_side_ptr; // 0x24
    HitboxManager* hitbox_manager_ptr; // 0x28
    PlayerStoneManager* player_stone_manager_ptr; // 0x2C
    // 0x30
};

// yes it's still an array of 2 in most structs
static inline constexpr size_t GAME_SIDE_COUNT = 1;

// size: 0x228
struct Globals {
    GlobalsSide side[2]; // 0x0
    int32_t difficulty; // 0x1E0
    int32_t __int_1E4; // 0x1E4 (0..1)
    int32_t continues; // 0x1E8 (0..9)
    int32_t __int_1EC; // 0x1EC (0..999,999,999)
    int32_t __int_1F0; // 0x1F0 (0..999,999,999)
    int32_t stage_number; // 0x1F4 (0..999)
    int32_t __stage_number_related; // 0x1F8 (0..999)
    int32_t __chapter; // 0x1FC (0..999)
    int __int_200; // 0x200
    int32_t __int_204; // 0x204 (-1..9999)
    int32_t __int_208; // 0x208 (0..99,999)
    int __dword_20C; // 0x20C
    int __dword_210; // 0x210
    int32_t __int_214; // 0x214
    int32_t __int_218; // 0x218
    int32_t __int_21C; // 0x21C
    int __int_220; // 0x220
    padding_bytes(0x4); // 0x224
    // 0x228
};

// size: 0x2C0
struct GameManager {
    GameSide game_side[2]; // 0x0
    uint64_t high_score; // 0x60
    uint32_t high_score_continues; // 0x68
    union {
        uint32_t flags; // 0x6C
        struct {
            uint32_t __unknown_flag_A : 1; // 1
            uint32_t __unknown_flag_E : 1; // 2
            uint32_t __unknown_flag_F : 1; // 3
            uint32_t __unknown_flag_B : 1; // 4
            uint32_t __unknown_flag_D : 1; // 5
            uint32_t __unknown_flag_C : 1; // 6
        };
    };
    int __int_70; // 0x70
    int32_t __int_74; // 0x74
    int32_t __int_78; // 0x78
    int __dword_7C; // 0x7C
    int __dword_80; // 0x80
    padding_bytes(0x4); // 0x84
    Globals globals; // 0x88
    double __double_2B0; // 0x2B0
    int __dword_2B8; // 0x2B8
    int __int_2BC; // 0x2BC
    // 0x2C0

    // RxBBC0
    dllexport gnu_noinline GameSide* thiscall get_game_side(int32_t side) {
        return &this->game_side[side];
    }
};

dllexport GameManager GAME_MANAGER;

// Rx78040
dllexport gnu_noinline BombManager* cdecl get_bomb_manager(int32_t side) {
    return GAME_MANAGER.get_game_side(side)->bomb_manager_ptr;
}
// Rx78060
dllexport gnu_noinline EnemyManager* cdecl get_enemy_manager(int32_t side) {
    return GAME_MANAGER.get_game_side(side)->enemy_manager_ptr;
}
// Rx78E80
dllexport gnu_noinline BulletManager* cdecl get_bullet_manager(int32_t side) {
    return GAME_MANAGER.get_game_side(side)->bullet_manager_ptr;
}
// Rx78EA0
dllexport gnu_noinline Spellcard* cdecl get_spellcard(int32_t side) {
    return GAME_MANAGER.get_game_side(side)->spellcard_ptr;
}
// Rx78EC0
dllexport gnu_noinline HitboxManager* cdecl get_hitbox_manager(int32_t side) {
    return GAME_MANAGER.get_game_side(side)->hitbox_manager_ptr;
}
// Rx78EE0
dllexport gnu_noinline LaserManager* cdecl get_laser_manager(int32_t side) {
    return GAME_MANAGER.get_game_side(side)->laser_manager_ptr;
}
// Rx64080
dllexport gnu_noinline GlobalsSide* cdecl get_globals_side(int32_t side) {
    return GAME_MANAGER.get_game_side(side)->globals_side_ptr;
}

// size: 0x14
// D3DFVF_XYZRHW | D3DFVF_DIFFUSE (0x44)
struct PrimitiveVertex {
    Float4 position; // 0x0
    D3DCOLOR diffuse; // 0x10
    // 0x14

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
    static inline constexpr size_t buffer_size(size_t count) {
        return count * sizeof(PrimitiveVertex);
    }
};
// size: 0x14
// D3DFVF_XYZ | D3DFVF_TEX1 (0x102)
struct UnknownVertexA {
    Float3 position; // 0x0
    Float2 texture_uv; // 0xC
    // 0x14

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZ | D3DFVF_TEX1;
    static inline constexpr size_t buffer_size(size_t count) {
        return count * sizeof(UnknownVertexA);
    }
};
// size: 0x18
// D3DFVF_XYZRHW | D3DFVF_TEX1 (0x104)
struct SpriteVertexB {
    Float4 position; // 0x0
    Float2 texture_uv; // 0x10
    // 0x18

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZRHW | D3DFVF_TEX1;
    static inline constexpr size_t buffer_size(size_t count) {
        return count * sizeof(SpriteVertexB);
    }
};
// size: 0x18
// D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 (0x142)
struct SpriteVertexC {
    Float3 position; // 0x0
    D3DCOLOR diffuse; // 0xC
    Float2 texture_uv; // 0x10
    // 0x18

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
    static inline constexpr size_t buffer_size(size_t count) {
        return count * sizeof(SpriteVertexC);
    }
};
// size: 0x1C
// D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 (0x144)
struct SpriteVertex {
    Float4 position; // 0x0
    D3DCOLOR diffuse; // 0x10
    Float2 texture_uv; // 0x14
    // 0x1C

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
    static inline constexpr size_t buffer_size(size_t count) {
        return count * sizeof(SpriteVertex);
    }
};
// size: 0x10
// D3DFVF_XYZ | D3DFVF_DIFFUSE (0x42)
struct UnknownVertexB {
    Float3 position; // 0x0
    D3DCOLOR diffuse; // 0xC
    // 0x10

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZ | D3DFVF_DIFFUSE;
    static inline constexpr size_t buffer_size(size_t count) {
        return count * sizeof(UnknownVertexB);
    }
};

// size: 0x5E4
typedef struct AnmVM AnmVM;
// size: 0x48
typedef struct AnmLists AnmLists;

typedef union AnmID AnmID;

// size: 0x8
struct AnmFuncsBase {
    // void* vftable; // 0x0
    AnmVM* vm; // 0x4
    // 0x8
    
    // yup, this is how functions are being handled now
    // *more virtual BS*

    // Method 0
    dllexport gnu_noinline virtual void thiscall destructor() {}
    // Method 4
    dllexport gnu_noinline virtual int thiscall on_tick() {
        return 0;
    }
    // Method 8
    dllexport gnu_noinline virtual int thiscall on_draw() {
        return 0;
    }
    // Method C
    dllexport gnu_noinline virtual int thiscall on_destroy() {
        return 0;
    }
    // Method 10
    dllexport gnu_noinline virtual int thiscall on_interrupt(int32_t interrupt) {
        return 0;
    }
    // Method 14
    dllexport gnu_noinline virtual int thiscall __method_14(int) {
        return 0;
    }
};

// EffectChargePointInf
// size: 0x1940
struct AnmFuncsA : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    uint32_t __anm_id_array_8[200]; // 0x8
    unknown_fields(0x1608); // 0x328
    Timer __timer_1930; // 0x1930
    // 0x1940

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // on_tick_special_dataB
        // TODO
        return 0;
    }
    // Method C
    gnu_noinline virtual int thiscall on_destroy() {
        // on_destroy_special_dataB
        // TODO
        return 0;
    }
    // Method 10
    gnu_noinline virtual int thiscall on_interrupt(int32_t interrupt) {
        // on_interrupt_special_dataB
        // TODO
        return 0;
    }
};

// EffectFanSparkInf
// size: 0x3E0
struct AnmFuncsB : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
};

// EffectLineEffectInf
// size: 0x320
struct AnmFuncsC : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
};

// EffectLineSparkInf
// size: 0xB60
struct AnmFuncsD : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
};

// EffectLineSpark2Inf
// size: 0xBAC
struct AnmFuncsE : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
};

// EffectLineStoneInf
// size: 0x3E4
struct AnmFuncsF : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
};

// EffectLineStone2Inf
// size: 0x3F4
struct AnmFuncsG : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
};

// EffectMenuWindowInf
// size: 0x48
struct AnmFuncsH : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
    // Method 10
    gnu_noinline virtual int thiscall on_interrupt(int32_t interrupt) {
        // TODO
        return 0;
    }
};

// EffectPyramidInf
// size: 0x1B4
struct AnmFuncsI : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
    // Method C
    gnu_noinline virtual int thiscall on_destroy() {
        // TODO
        return 0;
    }
    // Method 10
    gnu_noinline virtual int thiscall on_interrupt(int32_t interrupt) {
        // TODO
        return 0;
    }
};

// EffectScreenSwitchInf
// size: 0x1D88
struct AnmFuncsJ : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
    // Method 10
    gnu_noinline virtual int thiscall on_interrupt(int32_t interrupt) {
        // TODO
        return 0;
    }
};

// EffectShortLineEffectInf
// size: 0x10C
struct AnmFuncsK : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
};

// EffectShortLineEffect2Inf
// size: 0x184
struct AnmFuncsL : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
};

// EffectShortLineEffect3Inf
// size: 0x2D18
struct AnmFuncsM : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
};

// EffectStoneGaugeLineInf
// size: 0x19C
struct AnmFuncsN : AnmFuncsBase {
    //AnmFuncsBase base; // 0x0
    // 0x8

    // Method 4
    gnu_noinline virtual int thiscall on_tick() {
        // TODO
        return 0;
    }
    // Method 8
    gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
};

enum AnmVMState : uint32_t {
    Normal = 0, // 0
    MarkedForDelete, // 1
    Deleted // 2
};

// These are copied from th19 and need to be checked
enum AnmAnchorMode : uint32_t {
};
enum AnmRenderMode : uint8_t {
    RenderMode0 = 0,
    RenderMode1 = 1,
    RenderMode2 = 2,
    RenderMode3 = 3,
    RenderMode4 = 4,
    RenderMode5 = 5,
    RenderMode6 = 6,
    RenderMode7 = 7,
    RenderMode8 = 8,
    RenderMode_TexturedRing = 9,
    RenderMode10 = 10,
    RenderMode11 = 11,
    RenderMode12 = 12,
    RenderMode_TexturedArcA = 13,
    RenderMode_TexturedArcB = 14,
    RenderMode15 = 15,
    RenderMode_PolygonRectangle = 16,
    RenderMode_Polygon = 17,
    RenderMode_PolygonHollow = 18,
    RenderMode_PolygonRing = 19,
    RenderMode_PolygonRectangleGradient = 20,
    RenderMode_PolygonRectangleAntialias = 21,
    RenderMode_PolygonRectangleGradientAntialias = 22,
    RenderMode23 = 23,
    RenderMode_TexturedCylinder = 24,
    RenderMode_TexturedRing3D = 25,
    RenderMode_PolygonLine = 26,
    RenderMode_PolygonRectangleHollow = 27,
    RenderMode_PolygonUnknownA = 28,
    RenderMode_PolygonRingUnknownA = 29,
    RenderMode_PolygonRingUnknownB = 30,
    RenderMode_PolygonUnknownC = 31,
    RenderMode_PolygonUnknownD = 32,
    RenderMode_PolygonUnknownE = 33,
    RenderMode_PolygonUnknownF1 = 34,
    RenderMode_PolygonUnknownF2 = 35,
    RenderMode_PolygonUnknownF3 = 36,
    RenderMode_PolygonUnknownG = 37,
    RenderMode_PolygonUnknownH = 38,
    RenderMode_PolygonUnknownI1 = 39,
    RenderMode_PolygonUnknownI2 = 40,
    RenderMode_PolygonUnknownI3 = 41,
    RenderMode_PolygonUnknownJ = 42,
    RenderMode_PolygonUnknownK = 43,
    RenderMode_PolygonUnknownL1 = 44,
    RenderMode_PolygonUnknownL2 = 45,
    RenderMode_PolygonUnknownL3 = 46,
};
enum AnmResampleMode : uint8_t {
    ResampleModeN1 = 0xFF,
    ResampleMode0 = 0,
    ResampleMode1 = 1
};
enum AnmBlendMode : uint8_t {
    BlendModeDefault = 0,
    BlendModeAdd = 1,
    BlendMode2 = 2,
    BlendMode3 = 3,
    BlendMode4 = 4,
    BlendMode5 = 5,
    BlendMode6 = 6,
    BlendMode7 = 7,
    BlendMode8 = 8,
    BlendMode9 = 9,
    BlendMode11 = 11
};
enum AnmColorMode : uint8_t {
    ColorModeUseColor1 = 0,
    ColorModeUseColor2 = 1,
    ColorModeHorizontalGradient = 2,
    ColorModeVerticalGradient = 3,
    ColorMode4 = 4
};
enum AnmOriginMode : uint8_t {
    OriginMode0 = 0,
    OriginMode1 = 1,
    OriginMode2 = 2
};
enum AnmResolutionMode : uint8_t {
    ResolutionMode0 = 0,
    ResolutionMode1 = 1,
    ResolutionMode2 = 2,
    ResolutionMode3 = 3,
    ResolutionMode4 = 4,
    ResolutionMode5 = 5,
    ResolutionMode6 = 6
};
enum AnmRotationMode : uint8_t {
    RotationXYZ = 0,
    RotationXZY = 1,
    RotationYXZ = 2,
    RotationYZX = 3,
    RotationZXY = 4,
    RotationZYX = 5
};
enum AnmUVMode : uint8_t {
    Wrap = 0,
    Clamp = 1,
    Mirror = 2
};
enum AnmTextureMode : int8_t {
    TextureModeN1 = -1,
    TextureMode0 = 0,
    TextureMode1 = 1,
    TextureMode2 = 2,
    TextureMode3 = 3
};
enum AnmRNGMode : uint8_t {
    ReplayRNG = 0,
    NormalRNG = 1
};

typedef struct FastAnmVM FastAnmVM;
typedef struct AnmLoaded AnmLoaded;
typedef struct AnmSprite AnmSprite;
typedef struct AnmManager AnmManager;

static AnmManager* ANM_MANAGER_PTR = (AnmManager*)rand();

#define ANM_FULL_ID_BITS (32)
#define ANM_FAST_ID_BITS (16)
#define ANM_SLOW_ID_BITS (ANM_FULL_ID_BITS - ANM_FAST_ID_BITS)
#define INVALID_FAST_ID uint_width_max(ANM_FAST_ID_BITS)

static inline constexpr size_t FAST_VM_COUNT = uint_width_max(ANM_FAST_ID_BITS);
static inline constexpr size_t FAST_VM_ARRAY_COUNT = FAST_VM_COUNT + 1;

// size: 0x4
union AnmID {
    uint32_t full;
    struct {
        uint32_t fast_id : ANM_FAST_ID_BITS;
        uint32_t slow_id : ANM_SLOW_ID_BITS;
    };

    inline constexpr AnmID() : full(0) {}

    inline constexpr operator uint32_t() const {
        return this->full;
    }
};

typedef int32_t cdecl AnmOnFunc(AnmVM*);
typedef int32_t cdecl AnmOnFuncArg(AnmVM*, int32_t);

namespace Anm {
enum Var : int32_t {
    I0 = 10000, // 10000
    I1, // 10001
    I2, // 10002
    I3, // 10003
    F0, // 10004
    F1, // 10005
    F2, // 10006
    F3, // 10007
    IC0, // 10008
    IC1, // 10009
    RAND_ANGLE, // 10010
    RAND_FLOAT, // 10011
    RAND_FLOAT_SIGNED, // 10012
    SELF_X, // 10013
    SELF_Y, // 10014
    SELF_Z, // 10015
    CAMERA_POS_X, // 10016
    CAMERA_POS_Y, // 10017
    CAMERA_POS_Z, // 10018
    CAMERA_FACING_X, // 10019
    CAMERA_FACING_Y, // 10020
    CAMERA_FACING_Z, // 10021
    RAND_UINT, // 10022
    SELF_X_ROT, // 10023
    SELF_Y_ROT, // 10024
    SELF_Z_ROT, // 10025
    __CONTROLLER_Z_ROT, // 10026
    RAND_SCALE, // 10027
    RAND_ANGLE_SCALE, // 10028
    RAND_UINT_RANGE, // 10029
    RAND_ANGLE_REPLAY, // 10030
    RAND_FLOAT_REPLAY, // 10031
    RAND_FLOAT_SIGNED_REPLAY, // 10032
    __UNKNOWN_X, // 10033
    __UNKNOWN_Y, // 10034
    __UNKNOWN_Z, // 10035
};
enum Opcode : int16_t {
    // Section A
    nop = 0, // 0
    anm_delete, // 1
    anm_static, // 2
    anm_halt, // 3
    anm_halt_invisible, // 4
    interrupt_label, // 5
    wait, // 6
    interrupt_return, // 7

    // Section B
    set_int = 100, // 100
    set_float, // 101
    math_int_add_assign, // 102
    math_float_add_assign, // 103
    math_int_sub_assign, // 104
    math_float_sub_assign, // 105
    math_int_mul_assign, // 106
    math_float_mul_assign, // 107
    math_int_div_assign, // 108
    math_float_div_assign, // 109
    math_int_mod_assign, // 110
    math_float_mod_assign, // 111
    math_int_add, // 112
    math_float_add, // 113
    math_int_sub, // 114
    math_float_sub, // 115
    math_int_mul, // 116
    math_float_mul, // 117
    math_int_div, // 118
    math_float_div, // 119
    math_int_mod, // 120
    math_float_mod, // 121
    set_int_rand_bound, // 122
    set_float_rand_bound, // 123
    math_sin, // 124
    math_cos, // 125
    math_tan, // 126
    math_acos, // 127
    math_atan, // 128
    math_reduce_angle, // 129
    math_circle_pos_fast, // 130
    math_circle_pos_rand, // 131

    // Section C
    jump = 200, // 200
    loop, // 201
    jump_int_equ, // 202
    jump_float_equ, // 203
    jump_int_neq, // 204
    jump_float_neq, // 205
    jump_int_les, // 206
    jump_float_les, // 207
    jump_int_leq, // 208
    jump_float_leq, // 209
    jump_int_gre, // 210
    jump_float_gre, // 211
    jump_int_geq, // 212
    jump_float_geq, // 213

    // Section D
    sprite_set = 300, // 300
    sprite_set_rand_bound, // 301
    render_mode, // 302
    blend_mode, // 303
    layer, // 304
    anm_flag_disable_z_write, // 305
    __anm_flag_unknown_std_A, // 306
    rand_mode, // 307
    scale_flip_x, // 308
    scale_flip_y, // 309
    anm_flag_visible, // 310
    resample_mode, // 311
    scroll_mode, // 312
    resolution_mode, // 313
    anm_flag_inherit_rotation, // 314
    anm_flag_color_children, // 315
    __anm_flag_set_visible2, // 316
    __anm_flag_clear_visible2, // 317
    __anm_flag_unknown_V, // 318
    __sprite_set_unknown, // 319

    // Section E
    move_position = 400, // 400
    rotation, // 401
    scale, // 402
    alpha, // 403
    color, // 404
    alpha_gradient, // 405
    color_gradient, // 406
    move_position_interp, // 407
    color_interp, // 408
    alpha_interp, // 409
    rotation_interp, // 410
    spin_interp, // 411
    scale_interp, // 412
    color_gradient_interp, // 413
    alpha_gradient_interp, // 414
    rotation_speed, // 415
    scale_speed, // 416
    alpha_interp_linear, // 417
    sprite_window, // 418
    __anm_flag_continual_sprite_window, // 419
    move_bezier, // 420
    anchor_mode, // 421
    position_inherit, // 422
    color_mode, // 423
    anm_flag_auto_rotate, // 424
    scroll_speed_x, // 425
    scroll_speed_y, // 426
    scroll_speed_x_interp, // 427
    scroll_speed_y_interp, // 428
    uv_scale, // 429
    uv_scale_interp, // 430
    __anm_flag_unknown_T, // 431
    anm_flag_slowdown_immune, // 432
    move_velocity_interp, // 433
    scale2, // 434
    scale2_interp, // 435
    anchor_offset, // 436
    rotation_mode, // 437
    origin_mode, // 438
    camera_fade, // 439
    scale_unflip, // 440
    move_position2, // 441

    // Section F
    anm_create_child_back = 500, // 500
    anm_create_child_ui_back, // 501
    anm_create_child_front, // 502
    anm_create_child_ui_front, // 503
    anm_create_back, // 504
    anm_create_child_back_rel, // 505
    anm_create_back_rel, // 506
    __anm_flag_treat_as_root, // 507
    effect_create, // 508
    copy_parent_context, // 509
    anm_create_child_front_rel, // 510

    // Section G
    textured_ring = 600, // 600
    textured_arc_A, // 601
    textured_arc_B, // 602
    polygon_rectangle, // 603
    polygon, // 604
    polygon_hollow, // 605
    polygon_rectangle_gradient, // 606
    polygon_rectangle_antialias, // 607
    polygon_rectangle_gradient_antialias, // 608
    textured_cylinder, // 609
    textured_ring_3D, // 610
    polygon_ring, // 611
    polygon_rectangle_hollow, // 612
    polygon_line, // 613
    __polygon_unknown_A, // 614
    __polygon_ring_unknown_A, // 615
    __polygon_ring_unknown_B, // 616
    __polygon_unknown_C, // 617
    __polygon_unknown_D, // 618
    __polygon_unknown_E, // 619
    __polygon_unknown_F1, // 620
    __polygon_unknown_F2, // 621
    __polygon_unknown_F3, // 622
    __polygon_unknown_G, // 623
    __polygon_unknown_H, // 624
    __polygon_unknown_I1, // 625
    __polygon_unknown_I2, // 626
    __polygon_unknown_I3, // 627
    __polygon_unknown_J, // 628
    __polygon_unknown_K, // 629
    __polygon_unknown_L1, // 630
    __polygon_unknown_L2, // 631
    __polygon_unknown_L3, // 632
    __polygon_unknown_M, // 633
    __polygon_unknown_N, // 634
};
}

// size: 0x8
struct AnmInstruction {
    int16_t opcode; // 0x0
    uint16_t offset_to_next; // 0x2
    int16_t time; // 0x4
    uint16_t param_mask; // 0x6
    unsigned char args[]; // 0x8
};

enum AnmVMCreationListType {
    WorldListBack = 0,
    WorldListFront = 1,
    UiListBack = 2,
    UiListFront = 3,
};
union AnmVMCreationFlags {
    uint32_t raw;
    struct {
        uint32_t __unknown_flag_A : 1; // 1
        uint32_t list_type : 2; // 2-3
        uint32_t __unknown_flag_B : 1; // 4
    };
};

// size: 0x70
struct AnmLoaded {
    unknown_fields(0x54); // 0x0
    AnmInstruction* (*scripts)[]; // 0x54

    AnmInstruction* thiscall get_script(int32_t index) {
        return (*this->scripts)[index];
    }
};

static inline constexpr size_t CHILD_LIST_NODE = 0;
static inline constexpr size_t CHILD_LIST_HEAD = 1;

// size: 0x5E4
struct AnmVM {
    // size: 0x40
    struct AnmContext {
        int32_t I0; // 0x0 (0x444)
        int32_t I1; // 0x4 (0x448)
        int32_t I2; // 0x8 (0x44C)
        int32_t I3; // 0xC (0x450)
        float F0; // 0x10 (0x454)
        float F1; // 0x14 (0x458)
        float F2; // 0x18 (0x45C)
        float F3; // 0x1C (0x460)
        Float3 __float3_20; // 0x20 (0x464)
        int32_t IC0; // 0x2C (0x470)
        int32_t IC1; // 0x30 (0x474)
        float rand_scale; // 0x34 (0x478)
        float rand_angle_scale; // 0x38 (0x47C)
        int32_t rand_int_range; // 0x3C (0x480)
        // 0x40
    };
    struct AnmVMData {
        Timer interrupt_return_time; // 0x0
        int32_t interrupt_return_offset; // 0x10
        uint32_t layer; // 0x14
        int32_t slot; // 0x18
        int32_t slot2; // 0x1C
        int32_t sprite_id; // 0x20
        int32_t script_id; // 0x24
        int32_t current_instruction_offset; // 0x28
        Float3 position; // 0x2C
        Float3 rotation; // 0x38
        Float3 angular_velocity; // 0x44
        Float2 scale; // 0x50
        Float2 scale2; // 0x58
        Float2 scale_delta; // 0x60
        Float2 uv_scale; // 0x68
        Float2 sprite_size; // 0x70
        Float2 uv_scroll; // 0x78
        Float3 anchor_offset; // 0x80
        ZUNInterp<Float3> position_interp; // 0x8C
        ZUNInterp<RGB> color_interp; // 0xE0
        ZUNInterp<int32_t> alpha_interp; // 0x134
        ZUNInterp<Float3> rotation_interp; // 0x160
        ZUNInterp<ZUNAngle> spin_interp; // 0x1B4
        ZUNInterp<Float2> scale_interp; // 0x1E0
        ZUNInterp<Float2> scale2_interp; // 0x220
        ZUNInterp<Float2> uv_scale_interp; // 0x260
        ZUNInterp<RGB> color2_interp; // 0x2A0
        ZUNInterp<int32_t> alpha2_interp; // 0x2F4
        ZUNInterp<float> u_scroll_speed_interp; // 0x320
        ZUNInterp<float> v_scroll_speed_interp; // 0x34C
        Float2 sprite_uv_quad[4]; // 0x378
        Float2 __sprite_uv; // 0x398
        Float2 uv_scroll_speed; // 0x3A0
        int __dword_3A8; // 0x3A8
        int __dword_3AC; // 0x3AC
        int __dword_3B0; // 0x3B0
        int __dword_3B4; // 0x3B4
        D3DMATRIXZ __matrix_3B8; // 0x3B8
        D3DMATRIXZ __matrix_3F8; // 0x3F8
        int32_t run_interrupt; // 0x438
        int32_t __last_sprite_set_time; // 0x43C
        uint16_t script_id2; // 0x440
        probably_padding_bytes(0x2); // 0x442
        AnmContext current_context; // 0x444
        Float3 __position_2; // 0x484
        D3DCOLOR color1; // 0x490
        D3DCOLOR color2; // 0x494
        AnmRenderMode render_mode; // 0x498
        AnmBlendMode blend_mode; // 0x499
        bool visible; // 0x49A
        probably_padding_bytes(0x1); // 0x49B
        union {
            uint32_t flags_low; // 0x49C
            struct {
                uint32_t visible2 : 1; // 1
                uint32_t rotation_enabled : 1; // 2
                uint32_t scale_enabled : 1; // 3
                uint32_t uv_scale_enabled : 1; // 4
                uint32_t disable_z_write : 1; // 5
                uint32_t inherit_rotation : 1; // 6
                uint32_t position_mode : 1; // 7
                uint32_t __unknown_flag_T : 1; // 8
                uint32_t slowdown_immune : 1; // 9
                uint32_t rand_mode : 1; // 10
                uint32_t : 1; // 11
                uint32_t __continual_sprite_window : 1; // 12
                uint32_t __treat_as_root : 1; // 13
                uint32_t : 6; // 14-19
                uint32_t __deltas_enabled : 1; // 20
                uint32_t enable_camera_fade : 1; // 21
                uint32_t : 1;
                uint32_t __unknown_flag_V : 1; // 23
                uint32_t resolution_mode_set : 1; // 24 (prevents layer changes overriding an explicit mode)
                uint32_t colorize_children : 1; // 25
                uint32_t __unknown_flag_O : 1; // 26
                // ???
            };
        };
        union {
            uint8_t flags_mid0; // 0x4A0
            struct {
                uint8_t __unknown_field_B : 2; // 1-2
                uint8_t resample_mode : 2; // 3-4
                uint8_t __unknown_field_C : 2; // 5-6
                uint8_t mirror_modes : 2; // 7-8
            };
            struct {
                uint8_t : 6; // I refuse to get rid of these just because ZUN actually uses one field
                uint8_t mirror_x : 1; // 7
                uint8_t mirror_y : 1; // 8
            };
        };
        union {
            uint8_t flags_mid1; // 0x4A1
            struct {
                uint8_t __visible3 : 1; // 1
                uint8_t __unknown_std_flag_A : 1; // 2
                uint8_t color_mode : 3; // 3-5
                uint8_t v_scroll_mode : 2; // 6-7
                
            };
        };
        union {
            uint8_t flags_mid2; // 0x4A2
            struct {
                uint8_t u_scroll_mode : 2; // 1-2
                uint8_t rotation_mode : 3; // 3-5
                uint8_t __auto_rotate_mode : 3; // 6-8
            };
        };
        union {
            uint8_t flags_mid3; // 0x4A3
            struct {
                uint8_t origin_mode : 2; // 1-2
                uint8_t : 6; // 3-8 (completely unused bits)
            };
        };
        union {
            uint8_t flags_mid4; // 0x4A4
            struct {
                uint8_t resolution_mode : 8; // 1-8 why
            };
        };
        probably_padding_bytes(0x3); // 0x4A5
        AnmAnchorMode x_anchor_mode; // 0x4A8
        AnmAnchorMode y_anchor_mode; // 0x4AC
        union {
            uint8_t flags_high0; // 0x4B0
            struct {
                
            };
        };
        union {
            uint32_t flags_idk_anymore; // 0x4B4
            struct {
                uint32_t __unknown_flag_X : 1; // 1
            };
        };
        float camera_near_fade_end; // 0x4B8
        float camera_near_clip; // 0x4BC
        // 0x4C0
    };
    struct AnmVMController {
        AnmID id; // 0x0 (0x4C0)
        uint32_t fast_id; // 0x4 (0x4C4)
        Timer script_time; // 0x8 (0x4C8)
        Timer __timer_18; // 0x18 (0x4D8)
        AnmLists* global_list_base; // 0x28 (0x4E8)
        ZUNList<AnmVM> global_list_node; // 0x2C (0x4EC)
        ZUNList<AnmVM> __draw_list_node; // 0x40 (0x500)
        union {
            std::array<ZUNList<AnmVM>, 2> child_list_array; // WTF zun why this is horrible
            struct {
                ZUNList<AnmVM> child_list_node; // 0x54 (0x514)
                ZUNList<AnmVM> child_list_head; // 0x68 (0x528)
            };
        };
        ZUNList<AnmVM> destroy_list_node; // 0x7C (0x53C)
        AnmVM* next_in_layer; // 0x90 (0x550)
        AnmVM* prev_in_layer; // 0x94 (0x554)
        AnmVM* __root_vm; // 0x98 (0x558)
        AnmVM* parent_vm; // 0x9C (0x55C)
        float slowdown; // 0xA0 (0x560)
        void* special_data; // 0xA4 (0x564)
        AnmFuncsBase* on_funcs; // 0xA8 (0x568)
        uint32_t special_data_size; // 0xAC (0x56C)
        union {
            AnmVMState __vm_state; // 0xB0 (0x570)
            uint8_t __vm_state_fast;
        };
        AnmVMCreationFlags creation_flags; // 0xB4 (0x574)
        uint8_t __byte_B8; // 0xB8 (0x578)
        uint8_t __byte_B9; // 0xB9 (0x579)
        probably_padding_bytes(0x2); // 0xBA (0x57A)
        D3DMATRIX __matrix_BC; // 0xBC (0x57C)
        Float3 position; // 0xFC (0x5BC)
        void* associated_entity; // 0x108 (0x5C8)
        D3DCOLOR __color_104; // 0x10C (0x5CC)
        Float3 rotation; // 0x110 (0x5D0)
        AnmOnFunc* on_tick2_func; // 0x11C (0x5DC)
        AnmOnFuncArg* on_sprite_lookup_func; // 0x120 (0x5E0)
        // 0x124 (0x5E4)
    };
    
    AnmVMData data; // 0x0
    AnmVMController controller; // 0x4C0
    // 0x5E4

    // Rx299D0
    dllexport gnu_noinline void reset() {
        this->data.scale = { 1.0f, 1.0f };
        this->data.scale2 = { 1.0f, 1.0f };
        this->data.uv_scale = { 1.0f, 1.0f };
        this->data.color1 = PackD3DCOLOR(255, 255, 255, 255);
        this->data.__matrix_3B8.set_identity();
        //__builtin_memset(&this->data.render_mode, 0, 0x20);
        *(vec<float, 8>*)&this->data.render_mode = {};
        this->data.visible = true;
        this->data.visible2 = true;
        this->data.__unknown_field_B = 1;
        this->controller.script_time.default_values();
        this->controller.__timer_18.default_values();
        this->data.current_context.rand_scale = 1.0f;
        this->data.current_context.rand_angle_scale = PI_f;
        this->data.current_context.rand_int_range = UINT16_MAX + 1;
        this->controller.__root_vm = NULL;
        this->controller.parent_vm = NULL;
        this->data.position = {};
        this->data.__position_2 = {};
        this->data.rotation = {};
        this->data.angular_velocity = {};
        this->data.scale = { 1.0f, 1.0f };
        this->data.scale2 = { 1.0f, 1.0f };
        this->data.scale_delta = {};
        this->data.uv_scale = { 1.0f, 1.0f };
        this->data.sprite_size = {};
        this->data.uv_scroll.y = 0.0f;
        this->data.uv_scroll.x = 0.0f;
        this->data.anchor_offset = {};
        this->data.position_interp.end_time = 0;
        this->data.color_interp.end_time = 0;
        this->data.alpha_interp.end_time = 0;
        this->data.rotation_interp.end_time = 0;
        this->data.spin_interp.end_time = 0;
        this->data.scale_interp.end_time = 0;
        this->data.scale2_interp.end_time = 0;
        this->data.uv_scale_interp.end_time = 0;
        this->data.color2_interp.end_time = 0;
        this->data.alpha2_interp.end_time = 0;
        this->data.u_scroll_speed_interp.end_time = 0;
        this->data.v_scroll_speed_interp.end_time = 0;
    }

#if INCLUDE_REPLACEMENT_CODE
    dllexport AnmVM* constructor_fast() {
        __builtin_memset(this, 0, sizeof(*this));
        this->controller.global_list_node.data = this;
        this->controller.__draw_list_node.data = this;
        this->controller.child_list_node.data = this;
        this->controller.child_list_head.data = this;
        this->controller.child_list_head.list = (ZUNListIterable<AnmVM>*)&this->controller.child_list_head;
        this->controller.destroy_list_node.data = this;
    }
    dllexport void reset_fast() {
        uint32_t fast_id = this->controller.fast_id;
        __builtin_memset(this, 0, sizeof(*this));
        this->data.scale = { 1.0f, 1.0f };
        this->data.scale2 = { 1.0f, 1.0f };
        this->data.uv_scale = { 1.0f, 1.0f };
        this->data.__matrix_3B8.m[0][0] = 1.0f;
        this->data.__matrix_3B8.m[1][1] = 1.0f;
        this->data.__matrix_3B8.m[2][2] = 1.0f;
        this->data.__matrix_3B8.m[3][3] = 1.0f;
        this->data.current_context.rand_scale = 1.0f;
        this->data.current_context.rand_angle_scale = PI_f;
        this->data.current_context.rand_int_range = UINT16_MAX + 1;
        this->data.color1 = PackD3DCOLOR(255, 255, 255, 255);
        this->data.visible = true;
        this->data.visible2 = true;
        this->data.__unknown_field_B = 1;
        this->controller.fast_id = fast_id;
        this->controller.script_time.previous = -999999;
        this->controller.__timer_18.previous = -999999;
    }
    dllexport void reset_fast_thprac_compat() {
        //uint32_t fast_id = this->controller.fast_id;
        //__builtin_memset(this, 0, sizeof(*this));
        this->data.scale = { 1.0f, 1.0f };
        this->data.scale2 = { 1.0f, 1.0f };
        this->data.uv_scale = { 1.0f, 1.0f };
        this->data.__matrix_3B8.m[0][0] = 1.0f;
        this->data.__matrix_3B8.m[1][1] = 1.0f;
        this->data.__matrix_3B8.m[2][2] = 1.0f;
        this->data.__matrix_3B8.m[3][3] = 1.0f;
        this->data.current_context.rand_scale = 1.0f;
        this->data.current_context.rand_angle_scale = PI_f;
        this->data.current_context.rand_int_range = UINT16_MAX + 1;
        this->data.color1 = PackD3DCOLOR(255, 255, 255, 255);
        this->data.visible = true;
        this->data.visible2 = true;
        this->data.__unknown_field_B = 1;
        //this->controller.fast_id = fast_id;
        this->controller.script_time.previous = -999999;
        this->controller.__timer_18.previous = -999999;
    }
    dllexport gnu_noinline void reset_fast_allocate() {
        uint32_t fast_id = this->controller.fast_id;
        __builtin_memset(this, 0, sizeof(*this));
        this->data.scale = { 1.0f, 1.0f };
        this->data.scale2 = { 1.0f, 1.0f };
        this->data.uv_scale = { 1.0f, 1.0f };
        this->data.color1 = PackD3DCOLOR(255, 255, 255, 255);
        this->data.__matrix_3B8.m[0][0] = 1.0f;
        this->data.__matrix_3B8.m[1][1] = 1.0f;
        this->data.__matrix_3B8.m[2][2] = 1.0f;
        this->data.__matrix_3B8.m[3][3] = 1.0f;
        this->data.current_context.rand_scale = 1.0f;
        this->data.current_context.rand_angle_scale = PI_f;
        this->data.current_context.rand_int_range = UINT16_MAX + 1;
        this->data.visible = true;
        this->data.visible2 = true;
        this->data.__unknown_field_B = 1;
        this->controller.fast_id = INVALID_FAST_ID;
        this->controller.script_time.previous = -999999;
        this->controller.__timer_18.previous = -999999;
    }
#endif

    inline void cleanup_on_funcs() {
        MEM_ALLOC_LOG->delete_mem(this->controller.on_funcs);
        this->controller.on_funcs = NULL;
    }

    inline void cleanup_special_data() {
        if (this->controller.special_data) {
            MEM_ALLOC_LOG->free_mem(this->controller.special_data);
        }
        this->controller.special_data = NULL;
        this->controller.special_data_size = 0;
    }

#if INCLUDE_REPLACEMENT_CODE
    inline void cleanup_on_funcs_fast() {
        AnmFuncsBase* funcs = this->controller.on_funcs;
        if (expect(funcs != NULL, false)) {
            this->controller.on_funcs = NULL;
            MEM_ALLOC_LOG->delete_mem_static(funcs);
        }
    }

    inline void cleanup_special_data_fast() {
        void* special_data = this->controller.special_data;
        if (expect(special_data != NULL, false)) {
            this->controller.special_data = NULL;
            this->controller.special_data_size = 0;
            MEM_ALLOC_LOG->free_mem_static(special_data);
        }
    }

    inline void cleanup_special_data_faster() {
        void* special_data = this->controller.special_data;
        if (expect(special_data != NULL, false)) {
            //this->controller.special_data = NULL;
            //this->controller.special_data_size = 0;
            MEM_ALLOC_LOG->free_mem_static(special_data);
        }
    }
#endif

    // Rx4C240
    dllexport gnu_noinline void thiscall cleanup() {
        this->cleanup_special_data();
        this->cleanup_on_funcs();
        this->controller.id.full = 0;
        this->data.current_instruction_offset = -1;
        for (
            AnmVM* vm = this;
            vm->controller.next_in_layer;
            // vm = vm->controller.next_in_layer
        ) {
            vm->controller.id.full = 0;
        }
    }

#if INCLUDE_REPLACEMENT_CODE
    dllexport void cleanup_fast() {
        this->cleanup_special_data_fast();
        this->cleanup_on_funcs_fast();
        this->controller.id.full = 0;
        this->data.current_instruction_offset = -1;
    }

    dllexport void cleanup_fast_with_destroy() {
        AnmFuncsBase* funcs = this->controller.on_funcs;
        if (expect(funcs != NULL, false)) {
            this->controller.on_funcs = NULL;
            funcs->on_destroy();
            MEM_ALLOC_LOG->delete_mem_static(funcs);
        }
        this->cleanup_special_data_fast();
        this->controller.id.full = 0;
        this->data.current_instruction_offset = -1;
    }

    dllexport void cleanup_faster_with_destroy() {
        AnmFuncsBase* funcs = this->controller.on_funcs;
        if (expect(funcs != NULL, false)) {
            //this->controller.on_funcs = NULL;
            funcs->on_destroy();
            MEM_ALLOC_LOG->delete_mem_static(funcs);
        }
        this->cleanup_special_data_faster();
        //this->controller.id.full = 0;
        //this->data.current_instruction_offset = -1;
    }
#endif

    /*
    inline int32_t run_on_wait() {
        if (uint32_t index = this->controller.on_wait_index) {
            return ANM_ON_WAIT_FUNCS[index](this);
        }
        return 0;
    }
    */
    inline int32_t run_on_tick() {
        AnmFuncsBase* funcs = this->controller.on_funcs;
        if (funcs) {
            return funcs->on_tick();
        }
        return 0;
    }
    inline int32_t run_on_tick2() {
        AnmOnFunc* func = this->controller.on_tick2_func;
        if (func) {
            return func(this);
        }
        return 0;
    }
    inline int32_t run_on_draw() {
        AnmFuncsBase* funcs = this->controller.on_funcs;
        if (funcs) {
            return funcs->on_draw();
        }
        return 0;
    }
    inline int32_t run_on_destroy() {
        AnmFuncsBase* funcs = this->controller.on_funcs;
        if (funcs) {
            return funcs->on_destroy();
        }
        return 0;
    }
    inline int32_t run_on_interrupt(int32_t interrupt) {
        AnmFuncsBase* funcs = this->controller.on_funcs;
        if (funcs) {
            return funcs->on_interrupt(interrupt);
        }
        return 0;
    }
    /*
    inline int32_t run_on_sprite_lookup(int32_t sprite) {
        if (uint32_t index = this->controller.on_sprite_lookup_index) {
            return ANM_ON_SPRITE_LOOKUP_FUNCS[index](this, sprite);
        }
        return 0;
    }
    */

    inline AnmInstruction* get_current_instruction(int32_t offset) {
        AnmInstruction* current_instruction = this->get_anm_loaded()->get_script(this->data.script_id);
        return IndexInstr(offset);
    }
    inline AnmInstruction* get_current_instruction() {
        AnmInstruction* current_instruction = this->get_anm_loaded()->get_script(this->data.script_id);
        return IndexInstr(this->data.current_instruction_offset);
    }

#if INCLUDE_REPLACEMENT_CODE
    inline AnmVM* find_root_vm() {
        AnmVM* search = this;
        AnmVM* root;
        while (
            (root = search->controller.__root_vm) != NULL &&
            !search->data.__treat_as_root
        ) search = root;
        return search;
    }

    dllexport float thiscall get_custom_slowdown_fast() {
        return this->find_root_vm()->controller.slowdown;
    }

    dllexport gnu_noinline Float3* thiscall get_controller_rotation_fast() {
        Float3* controller_rotation = &this->controller.rotation;
        *controller_rotation = this->data.rotation;

        if (!this->data.__treat_as_root) {
            AnmVM* root_vm = this->controller.__root_vm;
            if (root_vm) {
                *controller_rotation += *root_vm->get_controller_rotation_fast();
            }
        }
        return controller_rotation;
    }
#endif

    float thiscall get_custom_slowdown() {
        if (
            this->controller.__root_vm &&
            !this->data.__treat_as_root
        ) {
            return this->controller.__root_vm->get_custom_slowdown();
        }
        return this->controller.slowdown;
    }

    // Rx37770
    dllexport gnu_noinline Float3* thiscall get_controller_rotation() {
        this->controller.rotation = this->data.rotation;
        
        if (
            this->controller.__root_vm &&
            !this->data.__treat_as_root
        ) {
            this->controller.rotation += *this->controller.__root_vm->get_controller_rotation();

            // Is this supposed to be applied to controller rotation instead?
            this->data.rotation.x = reduce_angle(this->data.rotation.x);
            this->data.rotation.y = reduce_angle(this->data.rotation.y);
            this->data.rotation.z = reduce_angle(this->data.rotation.z);
        }

        return &this->controller.rotation;
    }

    AnmLoaded* thiscall AnmVM::get_anm_loaded();

    // Rx37EB0
    dllexport gnu_noinline float thiscall get_float_var(float index) {
        using namespace Anm;
        switch ((int32_t)index) {
            case I0: // 10000
                return this->data.current_context.I0;
            case I1: // 10001
                return this->data.current_context.I1;
            case I2: // 10002
                return this->data.current_context.I2;
            case I3: // 10003
                return this->data.current_context.I3;
            case F0: // 10004
                return this->data.current_context.F0;
            case F1: // 10005
                return this->data.current_context.F1;
            case F2: // 10006
                return this->data.current_context.F2;
            case F3: // 10007
                return this->data.current_context.F3;
            case __UNKNOWN_X: // 10033
                return this->data.current_context.__float3_20.x;
            case __UNKNOWN_Y: // 10034
                return this->data.current_context.__float3_20.y;
            case __UNKNOWN_Z: // 10035
                return this->data.current_context.__float3_20.z;
            case IC0: // 10008
                return this->data.current_context.IC0;
            case IC1: // 10009
                return this->data.current_context.IC1;
            case RAND_FLOAT: // 10011
                return RNG.rand_float_range(this->data.current_context.rand_scale);
            case RAND_FLOAT_SIGNED: // 10012
                return RNG.rand_float_signed_range(this->data.current_context.rand_scale);
            case RAND_ANGLE: // 10010
                return RNG.rand_float_signed_range(this->data.current_context.rand_angle_scale);
            case SELF_X: // 10013
                return this->data.position.x;
            case SELF_Y: // 10014
                return this->data.position.x;
            case SELF_Z: // 10015
                return this->data.position.x;
            case SELF_X_ROT: // 10023
                return this->data.rotation.x;
            case SELF_Y_ROT: // 10024
                return this->data.rotation.y;
            case SELF_Z_ROT: // 10025
                return this->data.rotation.z;
            case __CONTROLLER_Z_ROT: // 10026
                return this->get_controller_rotation()->z;
            case CAMERA_POS_X: // 10016
                //return SUPERVISOR.cameras[3].position.x + SUPERVISOR.cameras[3].__shaking_float3_A.x;
            case CAMERA_POS_Y: // 10017
                //return SUPERVISOR.cameras[3].position.y + SUPERVISOR.cameras[3].__shaking_float3_A.y;
            case CAMERA_POS_Z: // 10018
                //return SUPERVISOR.cameras[3].position.z + SUPERVISOR.cameras[3].__shaking_float3_A.z;
            case CAMERA_FACING_X: // 10019
                //return SUPERVISOR.cameras[3].facing_normalized.x;
            case CAMERA_FACING_Y: // 10020
                //return SUPERVISOR.cameras[3].facing_normalized.x;
            case CAMERA_FACING_Z: // 10021
                //return SUPERVISOR.cameras[3].facing_normalized.x;
            case RAND_SCALE: // 10027
                return this->data.current_context.rand_scale;
            case RAND_ANGLE_SCALE: // 10028
                return this->data.current_context.rand_angle_scale;
            case RAND_UINT_RANGE: // 10029
                return this->data.current_context.rand_int_range;
            case RAND_UINT: // 10022
                // BUG: Range variable is not applied here
                return RNG.rand_uint();
            case RAND_FLOAT_REPLAY: // 10031
                return REPLAY_RNG.rand_float_range(this->data.current_context.rand_scale);
            case RAND_FLOAT_SIGNED_REPLAY: // 10032
                return REPLAY_RNG.rand_float_signed_range(this->data.current_context.rand_scale);
            case RAND_ANGLE_REPLAY: // 10030
                return REPLAY_RNG.rand_float_signed_range(this->data.current_context.rand_angle_scale);
            default:
                return index;
        }
    }

    // Rx37D00
    dllexport gnu_noinline int32_t thiscall get_int_var(int32_t index) {
        using namespace Anm;
        switch (index) {
            case I0: // 10000
                return this->data.current_context.I0;
            case I1: // 10001
                return this->data.current_context.I1;
            case I2: // 10002
                return this->data.current_context.I2;
            case I3: // 10003
                return this->data.current_context.I3;
            case F0: // 10004
                return this->data.current_context.F0;
            case F1: // 10005
                return this->data.current_context.F1;
            case F2: // 10006
                return this->data.current_context.F2;
            case F3: // 10007
                return this->data.current_context.F3;
            case __UNKNOWN_X: // 10033
                return this->data.current_context.__float3_20.x;
            case __UNKNOWN_Y: // 10034
                return this->data.current_context.__float3_20.y;
            case __UNKNOWN_Z: // 10035
                return this->data.current_context.__float3_20.z;
            case IC0: // 10008
                return this->data.current_context.IC0;
            case IC1: // 10009
                return this->data.current_context.IC1;
            case RAND_SCALE: // 10027
                return this->data.current_context.rand_scale;
            case RAND_ANGLE_SCALE: // 10028
                return this->data.current_context.rand_angle_scale;
            case RAND_UINT_RANGE: // 10029
                return this->data.current_context.rand_int_range;
            case RAND_UINT: // 10022
                return RNG.rand_uint_range(this->data.current_context.rand_int_range);
            default:
                return index;
        }
    }

    inline float* get_float_ptr(int32_t index) {
        using namespace Anm;
        switch ((int32_t)index) {
            case F0: // 10004
                return &this->data.current_context.F0;
            case F1: // 10005
                return &this->data.current_context.F1;
            case F2: // 10006
                return &this->data.current_context.F2;
            case F3: // 10007
                return &this->data.current_context.F3;
            case SELF_X: // 10013
                return &this->data.position.x;
            case SELF_Y: // 10014
                return &this->data.position.x;
            case SELF_Z: // 10015
                return &this->data.position.x;
            case SELF_X_ROT: // 10023
                return &this->data.rotation.x;
            case SELF_Y_ROT: // 10024
                return &this->data.rotation.y;
            case SELF_Z_ROT: // 10025
                return &this->data.rotation.z;
            case __UNKNOWN_X: // 10033
                return &this->data.current_context.__float3_20.x;
            case __UNKNOWN_Y: // 10034
                return &this->data.current_context.__float3_20.y;
            case __UNKNOWN_Z: // 10035
                return &this->data.current_context.__float3_20.z;
            case RAND_SCALE: // 10027
                return &this->data.current_context.rand_scale;
            case RAND_ANGLE_SCALE: // 10028
                return &this->data.current_context.rand_angle_scale;
            default:
                return NULL;
        }
    }

    inline int32_t* get_int_ptr(int32_t index) {
        using namespace Anm;
        switch (index) {
            case I0: // 10000
                return &this->data.current_context.I0;
            case I1: // 10001
                return &this->data.current_context.I1;
            case I2: // 10002
                return &this->data.current_context.I2;
            case I3: // 10003
                return &this->data.current_context.I3;
            case IC0: // 10008
                return &this->data.current_context.IC0;
            case IC1: // 10009
                return &this->data.current_context.IC1;
            case RAND_UINT_RANGE: // 10029
                return &this->data.current_context.rand_int_range;
            default:
                return NULL;
        }
    }

    inline float get_float_arg(float& value, uint16_t param_mask, int32_t index) {
        if (param_mask & 1 << index) {
            return this->get_float_var(value);
        } else {
            return value;
        }
    }

    inline int32_t get_int_arg(int32_t& value, uint16_t param_mask, int32_t index) {
        if (param_mask & 1 << index) {
            return this->get_int_var(value);
        } else {
            return value;
        }
    }

    // Rx37310
    dllexport gnu_noinline float* thiscall get_float_ptr_arg(float* value, uint16_t param_mask, int32_t index) {
        float* ret;
        if (
            !(param_mask & 1 << index) ||
            !(ret = this->get_float_ptr(*value))
        ) {
            ret = value;
        }
        return ret;
    }

    // Rx37220
    dllexport gnu_noinline int32_t* thiscall get_int_ptr_arg(int32_t* value, uint16_t param_mask, int32_t index = 0) {
        int32_t* ret;
        if (
            !(param_mask & 1) ||
            !(ret = this->get_int_ptr(*value))
        ) {
            ret = value;
        }
        return ret;
    }

    // Rx2B5D0
    dllexport gnu_noinline ZUNResult thiscall run_anm();

#if INCLUDE_REPLACEMENT_CODE
    dllexport gnu_noinline ZUNResult thiscall run_anm_fast() asm_symbol_rel(Rx2B5D0);
#endif
};
ValidateStructSize(0x5E4, AnmVM);

// size: 0x600
struct FastAnmVM : AnmVM {
    //AnmVM base; // 0x0
    ZUNList<FastAnmVM> fast_node; // 0x5E4
    bool alive; // 0x5F8
    padding_bytes(0x3); // 0x5F9
    uint32_t fast_id; // 0x5FC
    // 0x600

#if INCLUDE_REPLACEMENT_CODE
    dllexport void thiscall destructor_fast() {
        if (this->alive) {
            this->cleanup_faster_with_destroy();
        }
    }
#endif
};
ValidateStructSize(0x600, FastAnmVM);

#if INCLUDE_REPLACEMENT_CODE
struct DummyFastArray {
    FastAnmVM array[FAST_VM_ARRAY_COUNT];

    dllexport void thiscall destructor_fast() {
        for (auto& vm : this->array) {
            vm.destructor_fast();
        }
    }
};

dllexport void export_more_stuff_plz(FastAnmVM& vm, DummyFastArray& fast) {
    vm.destructor_fast();
    fast.destructor_fast();
}
#endif

// size: 0x28
struct BackbufferTexture {
    int anm_loaded_index; // 0x0
    int anm_loaded_d3d_index; // 0x4
    ZUNRect src; // 0x8
    ZUNRect dst; // 0x18
    // 0x28
};

// size: 0x48
struct AnmLists {
    ZUNListIterable<AnmVM> world_list; // 0x0
    ZUNListIterable<AnmVM> ui_list; // 0x18
    union {
        ZUNListIterable<AnmVM> current_layer_list; // 0x30
        struct {
            padding_bytes(0x8);
            uint32_t vm_count;
            uint32_t array_size;
            void* array_ptr;
            AnmVM** vm_array_ptr;
        };
    };
    // 0x48
    
    /*
    // Rx49FA0
    void draw_layer_list() {
        for (ZUNList<AnmVM>& vm_node : this->current_layer_list) {
            ANM_MANAGER_PTR->draw_vm(vm_node.get_data());
            ANM_MANAGER_PTR->__counter_D8++;
        }
    }
    
    // Rx4A090
    uint32_t build_layer_list_world(uint32_t layer) {
        this->current_layer_list.initialize(NULL);
        uint32_t count = 0;
        for (ZUNList<AnmVM>& vm_node : this->world_list) {
            if (vm_node.get_data()->controller.__vm_state == Normal) {
                int32_t vm_layer = vm_node.get_data()->data.layer;
                if (vm_layer >= 46 && vm_layer <= 53) {
                    vm_layer -= 17;
                }
                else if (vm_layer == 45) {
                    vm_layer = 26;
                }
                if (vm_layer == layer) {
                    vm_node.get_data()->controller.__draw_list_node.initialize(vm_node.get_data());
                    this->current_layer_list.append(vm_node.get_data());
                    ++count;
                }
            }
        }
        return count;
    }
    
    // Rx4A210
    uint32_t build_layer_list_ui(uint32_t layer) {
        this->current_layer_list.initialize(NULL);
        uint32_t count = 0;
        for (ZUNList<AnmVM>& vm_node : this->world_list) {
            if (vm_node.get_data()->controller.__vm_state == Normal) {
                int32_t vm_layer = vm_node.get_data()->data.layer;
                if (vm_layer >= 29 && vm_layer <= 36) {
                    vm_layer += 17;
                }
                else if (vm_layer == 26) {
                    vm_layer = 45;
                }
                else {
                    vm_layer = 47;
                }
                if (vm_layer == layer) {
                    vm_node.get_data()->controller.__draw_list_node.initialize(vm_node.get_data());
                    this->current_layer_list.append(vm_node.get_data());
                    ++count;
                }
            }
        }
        return count;
    }
    */

    // Rx49870
    dllexport void thiscall on_tick_world();
    // Rx49B20
    dllexport void thiscall on_tick_ui();

#if INCLUDE_REPLACEMENT_CODE
    // Rx4B7F0
    dllexport AnmID* thiscall add_vm_to_world_list_back_fast(AnmID* out, AnmVM* vm);
    // Rx4B840
    dllexport AnmID* thiscall add_vm_to_world_list_front_fast(AnmID* out, AnmVM* vm);
    // Rx4B890
    dllexport AnmID* thiscall add_vm_to_ui_list_back_fast(AnmID* out, AnmVM* vm);
    // Rx4B8E0
    dllexport AnmID* thiscall add_vm_to_ui_list_front_fast(AnmID* out, AnmVM* vm);
#endif
};
ValidateStructSize(0x48, AnmLists);

dllexport gnu_noinline void fastcall lock_mutex(std::mutex& mutex) asm_symbol_rel(RxC470);
dllexport gnu_noinline void fastcall lock_mutex(std::mutex& mutex) {
    _Mtx_lock((_Mtx_internal_imp_t*)&mutex);
}
dllexport gnu_noinline void fastcall unlock_mutex(std::mutex& mutex) asm_symbol_rel(RxC660);
dllexport gnu_noinline void fastcall unlock_mutex(std::mutex& mutex) {
    _Mtx_unlock((_Mtx_internal_imp_t*)&mutex);
}

// size: 0x7D40E94
struct AnmManager {
    AnmTextureMode current_texture_mode; // 0x0 (UM current_texture_op)
    padding_bytes(0x3); // 0x1
    char __thread_4[0x10]; // 0x4
    int __dword_14; // 0x14
    BackbufferTexture backbuffer_textures[4]; // 0x18
    int __int_B8; // 0xB8
    int __dword_BC; // 0xBC
    int __int_C0; // 0xC0
    int __int_C4; // 0xC4
    Float2 __float2_C8; // 0xC8 Was Int2 in th19?
    Float2 __float2_D0; // 0xD0
    uint32_t __counter_D8; // 0xD8
    AnmVM root_vm; // 0xDC
    int __int_6C0; // 0x6C0
    int32_t current_side; // 0x6C4
    union {
        std::array<AnmLists, GAME_SIDE_COUNT> lists; // 0x6C8 (yes this is an array of 1)
        AnmLists lists_raw;
    };
    std::array<FastAnmVM, FAST_VM_ARRAY_COUNT> fast_array; // 0x710
    int32_t next_snapshot_fast_id; // 0x6000710
    int32_t next_snapshot_discriminator; // 0x6000714
    ZUNList<FastAnmVM> free_list_head; // 0x6000718
    int __dword_600072C; // 0x600072C
    std::array<AnmLoaded*, 42> loaded_anm_files; // 0x6000730
    D3DMATRIX __matrix_60007D8; // 0x60007D8
    AnmVM __vm_6000818; // 0x6000818
    unknown_fields(0x4); // 0x6000DFC
    int __dword_6000E00; // 0x6000E00
    D3DCOLOR current_texture_blend_color; // 0x6000E04
    int32_t __current_texture_index; // 0x6000E08
    AnmBlendMode current_blend_mode; // 0x6000E0C
    char __byte_6000E0D; // 0x6000E0D
    uint8_t __byte_6000E0E; // 0x6000E0E
    char __byte_6000E0F; // 0x6000E0F
    char __byte_6000E10; // 0x6000E10
    char __byte_6000E11; // 0x6000E11
    AnmResampleMode current_resample_mode; // 0x6000E12
    AnmUVMode current_u_sample_mode; // 0x6000E13
    AnmUVMode current_v_sample_mode; // 0x6000E14
    padding_bytes(0x3); // 0x6000E15
    AnmSprite* __current_sprite; // 0x6000E18
    LPDIRECT3DVERTEXBUFFER9 __d3d_vertex_buffer_6000E1C; // 0x6000E1C
    UnknownVertexA __vertex_array_6000E20[4]; // 0x6000E20
    uint32_t unrendered_sprite_count; // 0x6000E70
    SpriteVertex sprite_vertex_data[0x100000]; // 0x6000E74
    SpriteVertex* sprite_write_cursor; // 0x7C00E74
    SpriteVertex* sprite_render_cursor; // 0x7C00E78
    uint32_t unrendered_primitive_count; // 0x7C00E7C
    PrimitiveVertex primitive_vertex_data[0x10000]; // 0x7C00E80
    PrimitiveVertex* primitive_write_cursor; // 0x7D40E80
    PrimitiveVertex* primitive_render_cursor; // 0x7D40E84
    union {
        AnmID prev_discriminator; // 0x7D40E88
        uint16_t prev_slow_id;
    };
    ZUNColor __color_7D40E8C; // 0x7D40E8C
    BOOL __bool_7D40E90; // 0x7D40E90
    // 0x7D40E94
    
    /*
    // Rx4F3D0
    void __render_layer_prep(uint32_t layer, uint32_t side) {
        if (layer == 41) {
            SUPERVISOR.__set_camera_by_index(3, side);
            SUPERVISOR.d3d_disable_fog();
            this->__int_6C0 = side + 1;
        }
        else if (layer == 43 || layer == 44) {
            SUPERVISOR.__set_camera_by_index(3, side);
            SUPERVISOR.d3d_disable_fog();
            this->__int_6C0 = 13;
        }
        else if (layer <= 2 || (layer >= 39 && layer <= 42)) {
            this->__int_6C0 = 0;
        }
        else if (layer <= 5) {
            if (this->__int_6C0 != side + 1) {
                SUPERVISOR.__set_camera_by_index(0, side);
                SUPERVISOR.d3d_disable_fog();
                this->__int_6C0 = side + 1;
            }
        }
        else if (layer <= 11) {
            if (this->__int_6C0 != side + 3) {
                SUPERVISOR.__set_camera_by_index(0, side);
                SUPERVISOR.d3d_disable_fog();
                this->__int_6C0 = side + 3;
            }
        }
        else if (layer <= 19) {
            if (this->__int_6C0 != side + 5) {
                SUPERVISOR.__set_camera_by_index(0, side);
                SUPERVISOR.d3d_disable_fog();
                this->__int_6C0 = side + 5;
            }
        }
        else if (layer <= 23) {
            if (this->__int_6C0 != side + 7) {
                SUPERVISOR.__set_camera_by_indexB(1, side);
                SUPERVISOR.d3d_disable_zwrite();
                SUPERVISOR.set_render_state(D3DRS_ZFUNC, D3DCMP_ALWAYS);
                // Missing assignment to __int_6C0?
            }
        }
        else if (layer <= 25) {
            if (this->__int_6C0 != 9) {
                SUPERVISOR.__set_camera_by_indexC(4);
                SUPERVISOR.d3d_disable_zwrite();
                SUPERVISOR.set_render_state(D3DRS_ZFUNC, D3DCMP_ALWAYS);
                ANM_MANAGER_PTR->__float2_C8.x = 0.0f;
                ANM_MANAGER_PTR->__float2_C8.y = 0.0f;
                this->__int_6C0 = 9;
            }
        }
        else if (
            layer <= 31 ||
            (layer >= 34 && layer <= 36) ||
            (layer >= 45 && layer <= 48) ||
            (layer >= 51 && layer <= 53)
        ) {
            if (this->__int_6C0 != 10) {
                SUPERVISOR.__set_camera_by_indexC(2);
                SUPERVISOR.d3d_disable_zwrite();
                SUPERVISOR.set_render_state(D3DRS_ZFUNC, D3DCMP_ALWAYS);
                ANM_MANAGER_PTR->__float2_C8.x = 0.0f;
                ANM_MANAGER_PTR->__float2_C8.y = 0.0f;
                this->__int_6C0 = 10;
            }
        }
        else if (
            (layer >= 32 && layer <= 33) ||
            (layer >= 49 && layer <= 50)
        ) {
            if (this->__int_6C0 != side + 11) {
                SUPERVISOR.__set_camera_by_indexB(5, side);
                SUPERVISOR.d3d_disable_zwrite();
                SUPERVISOR.set_render_state(D3DRS_ZFUNC, D3DCMP_ALWAYS);
                ANM_MANAGER_PTR->__float2_C8.x = 0.0f;
                ANM_MANAGER_PTR->__float2_C8.y = 0.0f;
                this->__int_6C0 = side + 11;
            }
        }
    }
    
    // Rx49E40
    UpdateFuncRet render_layer(uint32_t layer) {
        std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(AnmList_CS));
        
        uint32_t size;
        for (int32_t i = 0; i < GAME_SIDE_COUNT; ++i) {
            size = this->vm_lists[i].build_layer_list_world(layer);
            if (size) {
                this->__render_layer_prep(layer, i);
                this->vm_lists[i].draw_layer_list();
            }
            size = this->vm_lists[i].build_layer_list_ui(layer);
            if (size) {
                this->__render_layer_prep(layer, i);
                this->vm_lists[i].draw_layer_list();
            }
        }
        return UpdateFuncNext;
    }
    */

    // Rx4C9B0
    dllexport gnu_noinline AnmVM* thiscall allocate_new_vm() {
        
    }
    
    // Rx4F840
    dllexport gnu_noinline void thiscall __recursive_remove(AnmVM* vm, ZUNListIterable<AnmVM>* destroy_list) {
        if (vm->controller.child_list_array[CHILD_LIST_HEAD].get_next()) {
            for (ZUNList<AnmVM>* node : *vm->controller.child_list_array[CHILD_LIST_HEAD].get_next()) {
                this->__recursive_remove(node->get_data(), destroy_list);
            }
        }
        if (vm->controller.__vm_state != AnmVMState::Deleted) {
            ZUNList<AnmVM>* current_node = &vm->controller.destroy_list_node;
            current_node->initialize(vm);
            destroy_list->append(current_node);
        }
        vm->controller.__vm_state = AnmVMState::Deleted;
        vm->controller.__root_vm = NULL;
        vm->controller.parent_vm = NULL;
        vm->controller.prev_in_layer = NULL;
        vm->controller.next_in_layer = NULL;
        vm->controller.child_list_array[CHILD_LIST_NODE].unlink();
    }

    // 0x4C2F0
    dllexport gnu_noinline int32_t thiscall destroy_possibly_managed_vm(AnmVM* vm) {
        vm->run_on_destroy();
        vm->cleanup_on_funcs();
        vm->cleanup_special_data(); // not inlined
        vm->controller.global_list_node.unlink();
        vm->controller.destroy_list_node.unlink();
        vm->controller.__root_vm = NULL;
        vm->controller.parent_vm = NULL;
        if (vm >= &this->fast_array[0] && vm < array_end_addr(this->fast_array)) {
            FastAnmVM* fast_vm = &this->fast_array[vm->controller.fast_id];
            fast_vm->alive = false;
            this->free_list_head.append(&fast_vm->fast_node);
            vm->cleanup();
            vm->controller.id.full = 0;
            return 0;
        } else {
            vm->cleanup();
            delete vm;
            return 0;
        }
    }

#if INCLUDE_REPLACEMENT_CODE

    dllexport AnmID* thiscall assign_next_id_to_vm_fast(AnmID* out, AnmVM* vm) {
        uint16_t slow_id = this->prev_slow_id;
        slow_id += !++slow_id;
        this->prev_slow_id = slow_id;
        uint32_t full_id = (uint16_t)vm->controller.fast_id;
        full_id |= slow_id << 16;
        out->full = vm->controller.id.full = full_id;
        return out;
    }

    dllexport AnmID* thiscall add_vm_to_world_list_back_fast(AnmID* out, AnmVM* vm) {

    }

    dllexport AnmVM* allocate_new_vm_fast() {
        ZUNList<FastAnmVM>* free_node = this->free_list_head.next;
        if (expect(free_node != NULL, true)) {
            free_node->unlink_fast_no_clear();
            FastAnmVM* vm = OffsetTypePtr(free_node, FastAnmVM, fast_node);
            vm->reset_fast();

            vm->controller.global_list_node.data = vm;
            vm->controller.child_list_node.data = vm;
            vm->controller.child_list_head.data = vm;
            vm->controller.child_list_head.list = (ZUNListIterable<AnmVM>*)&vm->controller.child_list_head;
            vm->alive = true;
            return vm;
        }
        else {
            AnmVM* vm = (AnmVM*)MEM_ALLOC_LOG->allocate_static();
            vm->reset_fast_allocate();
            return vm;
        }
    }

    dllexport static ZUNList<AnmVM>* stdcall __recursive_remove_fast(AnmVM* vm, ZUNListIterable<AnmVM>* destroy_list) {
        ZUNList<AnmVM>* child_node = vm->controller.child_list_head.next;
        while (child_node != NULL) {
            AnmVM* vm = OffsetTypePtr(child_node, AnmVM, controller.child_list_node);
            child_node = __recursive_remove_fast(vm, destroy_list);
            //child_node->next;
        }
        //if (vm->controller.__vm_state_fast != AnmVMState::Deleted) {
            vm->controller.__vm_state_fast = AnmVMState::Deleted;
            ZUNList<AnmVM>* current_node = &vm->controller.destroy_list_node;
            current_node->data = vm;
            destroy_list->append_fast(current_node);
        //}
        //*(vec<float, 2>*)&vm->controller.__root_vm = {};
        //vm->controller.next_in_layer = NULL;
        //vm->controller.prev_in_layer = NULL;
        return vm->controller.child_list_node.unlinkR_fast_no_clear();
    }

    dllexport void thiscall destroy_possibly_managed_vm_fast(AnmVM* vm) {
        --this->lists[0].vm_count;
        vm->controller.global_list_node.unlink_fast_no_clear();
        //vm->controller.destroy_list_node.unlink_fast();
        if (
            expect(vm->controller.fast_id != INVALID_FAST_ID, true)
        ) {
            vm->cleanup_faster_with_destroy();
            //vm->controller.__root_vm = NULL;
            //vm->controller.parent_vm = NULL;
            //*(vec<float, 2>*)&vm->controller.__root_vm = {};
            FastAnmVM* fast_vm = (FastAnmVM*)vm;
            fast_vm->alive = false;
            this->free_list_head.append_fast(&fast_vm->fast_node);
        } else {
            MEM_ALLOC_LOG->delete_mem_static(vm);
        }
    }
#endif

    // Rx497D0
    dllexport gnu_noinline UpdateFuncRet thiscall on_tick_world() {
        std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(AnmList_CS));
        
        return UpdateFuncNext;
    }

    dllexport gnu_noinline ZUNResult thiscall draw_vm(AnmVM* vm) {
        use_var(vm);
        return (ZUNResult)rand();
    }

    dllexport gnu_noinline void thiscall __render_layer_prep(int32_t layer_index, int32_t game_side) {
        use_var(layer_index);
        use_var(game_side);
    }

#if INCLUDE_REPLACEMENT_CODE

    dllexport gnu_noinline AnmVM* thiscall get_vm_with_id_fast(AnmID vm_id) {
        if (expect(vm_id != 0, true)) {
            uint32_t fast_id = vm_id.fast_id;
            if (expect(fast_id != INVALID_FAST_ID, true)) {
                FastAnmVM& fast_vm = this->fast_array[fast_id];
                if (expect(fast_vm.alive && fast_vm.controller.id == vm_id, true)) {
                    return &fast_vm;
                }
            }
            else {
                ZUNList<AnmVM>* node = this->lists[0].world_list.next;
                __asm__ volatile ("":"+a"(node));
                while (expect(node != NULL, true)) {
                    uint32_t vm_id = OffsetField(node, AnmVM, controller.id, controller.global_list_node);
                    if (expect(vm_id == vm_id, false)) {
                        return OffsetTypePtr(node, AnmVM, controller.global_list_node);
                    }
                    node = node->next;
                }
                node = this->lists[0].ui_list.next;
                __asm__ volatile ("":"+a"(node));
                while (expect(node != NULL, true)) {
                    __asm__ volatile ("":"+a"(node));
                    uint32_t vm_id = OffsetField(node, AnmVM, controller.id, controller.global_list_node);
                    if (expect(vm_id == vm_id, false)) {
                        return OffsetTypePtr(node, AnmVM, controller.global_list_node);
                    }
                    __asm__ volatile ("":"+a"(node));
                    node = node->next;
                }
                __asm__ volatile ("":"+a"(node));
                return (AnmVM*)node;
            }
        }
        return NULL;
    }

    static constexpr uint8_t world_layer_lookup[] = {
        26,
        29, 30, 31, 32, 33, 34, 35, 36
    };
    static constexpr uint8_t ui_layer_lookup[] = {
        45,
        47, 47,
        46, 47, 48, 49, 50, 51, 52, 53
    };

    dllexport UpdateFuncRet fastcall render_layer_new(int32_t layer_index) {
        lock_mutex(CRITICAL_SECTION_MANAGER.get_mutex(AnmList_CS));

        ZUNList<AnmVM>* cur_node = this->lists[0].world_list.next;
        uint8_t d3d_unconfigured = true;

        while (expect(cur_node != NULL, true)) {
            uint32_t vm_state = OffsetField(cur_node, AnmVM, controller.__vm_state_fast, controller.global_list_node);
            if (!vm_state) {
                uint32_t layer = OffsetField(cur_node, AnmVM, data.layer, controller.global_list_node);
                
                uint32_t temp = layer - 45;
                if (expect(temp <= 8, false)) {
                    layer = world_layer_lookup[temp];
                }
                
                if (expect_chance(layer == layer_index, false, 0.75)) {
                    if (expect(!!d3d_unconfigured, false)) {
                        d3d_unconfigured = false;
                        this->__render_layer_prep(layer_index, 0);
                    }
                    AnmVM* vm_ptr = OffsetTypePtr(cur_node, AnmVM, controller.global_list_node);
                    this->draw_vm(vm_ptr);
                }
            }
            cur_node = cur_node->next;
        }

        cur_node = this->lists[0].ui_list.next;

        while (expect(cur_node != NULL, true)) {
            uint32_t vm_state = OffsetField(cur_node, AnmVM, controller.__vm_state_fast, controller.global_list_node);
            if (!vm_state) {
                uint32_t layer = OffsetField(cur_node, AnmVM, data.layer, controller.global_list_node);
                
                uint32_t temp = layer - 26;
                if (expect_chance(temp <= 10, true, 0.99)) {
                    layer = ui_layer_lookup[temp];
                } else {
                    layer = 47;
                }
                if (expect_chance(layer == layer_index, false, 0.75)) {
                    if (expect(!!d3d_unconfigured, false)) {
                        d3d_unconfigured = false;
                        this->__render_layer_prep(layer_index, 0);
                    }
                    AnmVM* vm_ptr = OffsetTypePtr(cur_node, AnmVM, controller.global_list_node);
                    this->draw_vm(vm_ptr);
                }
            }
            cur_node = cur_node->next;
        }

        unlock_mutex(CRITICAL_SECTION_MANAGER.get_mutex(AnmList_CS));

        return UpdateFuncNext;
    }

    dllexport UpdateFuncRet fastcall render_layer_turbo(int32_t layer_index) {
        // if any VMs get invalidated after prepping the turbo list
        // then we're screwed anyway, so to heck with the lock

        size_t vm_count = this->lists[0].vm_count;
        if (expect(vm_count != 0, true)) {
            this->__render_layer_prep(layer_index, 0);

            size_t aligned_vm_count = AlignUpToMultipleOf2(vm_count, 16) + 16;
            void* turbo_array = this->lists[0].array_ptr;

            const vec<uint8_t, 16>* layers = (vec<uint8_t, 16>*)turbo_array;
            AnmVM *const * vms = based_pointer<AnmVM*>(turbo_array, aligned_vm_count);
            const vec<uint8_t, 16>* layers_end = (vec<uint8_t, 16>*)vms;

            layer_index *= 0x01010101;
            //layer_index |= layer_index << 8;
            //layer_index |= layer_index << 16;
            //register uint32_t layer_test asm("ebp") = layer_index;
            vec<uint8_t, 16> layer_test = (vec<uint8_t, 16>)vec_broadcast<uint32_t, 4>(layer_index);
            do {
                auto mask = vec_movmsk(*layers++ == layer_test);
                if (expect(mask != 0, false)) {
                    if (mask & 0x0001) this->draw_vm(vms[0]);
                    if (mask & 0x0002) this->draw_vm(vms[1]);
                    if (mask & 0x0004) this->draw_vm(vms[2]);
                    if (mask & 0x0008) this->draw_vm(vms[3]);
                    if (mask & 0x0010) this->draw_vm(vms[4]);
                    if (mask & 0x0020) this->draw_vm(vms[5]);
                    if (mask & 0x0040) this->draw_vm(vms[6]);
                    if (mask & 0x0080) this->draw_vm(vms[7]);
                    if (mask & 0x0100) this->draw_vm(vms[8]);
                    if (mask & 0x0200) this->draw_vm(vms[9]);
                    if (mask & 0x0400) this->draw_vm(vms[10]);
                    if (mask & 0x0800) this->draw_vm(vms[11]);
                    if (mask & 0x1000) this->draw_vm(vms[12]);
                    if (mask & 0x2000) this->draw_vm(vms[13]);
                    if (mask & 0x4000) this->draw_vm(vms[14]);
                    if (mask & 0x8000) this->draw_vm(vms[15]);
                    layer_test = (vec<uint8_t, 16>)vec_broadcast<uint32_t, 4>(layer_index);
                }
                vms += 16;
            } while (expect(layers != layers_end, true));
        }

        return UpdateFuncNext;
    }

    dllexport UpdateFuncRet fastcall render_layer_turbo2(int32_t layer_index) {
        uint8_t* layer_array = (uint8_t*)this->lists[0].array_ptr;
        AnmVM *const * vms = this->lists[0].vm_array_ptr;

        if (expect((void*)layer_array != (void*)vms, true)) {
            this->__render_layer_prep(layer_index, 0);

            const vec<uint8_t, 16, true>* layers = (vec<uint8_t, 16, true>*)layer_array;
            const vec<uint8_t, 16, true>* layers_end = (vec<uint8_t, 16, true>*)(vms--);

            layer_index *= 0x01010101;
            __asm__("":"+d"(layer_index), "+a"(layers_end));
            vec<uint8_t, 16> layer_test = (vec<uint8_t, 16>)vec_broadcast<uint32_t, 4>(layer_index);
            for(;;) {
                auto mask = vec_movmsk(*layers++ == layer_test);
                vms += 16;
                __asm__("":"+S"(layers), "+D"(vms), "+b"(mask));
                if (expect(mask != 0, false)) {
                    if (expect((mask & 0x0001) != 0, true)) this->draw_vm(vms[-15]);
                    if (expect((mask & 0x0002) != 0, true)) this->draw_vm(vms[-14]);
                    if (expect((mask & 0x0004) != 0, true)) this->draw_vm(vms[-13]);
                    if (expect((mask & 0x0008) != 0, true)) this->draw_vm(vms[-12]);
                    if (expect((mask & 0x0010) != 0, true)) this->draw_vm(vms[-11]);
                    if (expect((mask & 0x0020) != 0, true)) this->draw_vm(vms[-10]);
                    if (expect((mask & 0x0040) != 0, true)) this->draw_vm(vms[-9]);
                    if (expect((mask & 0x0080) != 0, true)) this->draw_vm(vms[-8]);
                    if (expect((mask & 0x0100) != 0, true)) this->draw_vm(vms[-7]);
                    if (expect((mask & 0x0200) != 0, true)) this->draw_vm(vms[-6]);
                    if (expect((mask & 0x0400) != 0, true)) this->draw_vm(vms[-5]);
                    if (expect((mask & 0x0800) != 0, true)) this->draw_vm(vms[-4]);
                    if (expect((mask & 0x1000) != 0, true)) this->draw_vm(vms[-3]);
                    if (expect((mask & 0x2000) != 0, true)) this->draw_vm(vms[-2]);
                    if (expect((mask & 0x4000) != 0, true)) this->draw_vm(vms[-1]);
                    if (expect((mask & 0x8000) != 0, true)) this->draw_vm(vms[0]);
                    __asm__("":"+m"(layer_index), "+m"(layers_end));
                    if (expect(layers == layers_end, false)) {
                        break;
                    }
                    layer_test = (vec<uint8_t, 16>)vec_broadcast<uint32_t, 4>(layer_index);
                }
                else if (expect(layers == layers_end, false)) {
                    break;
                }
            }
        }

        return UpdateFuncNext;
    }

    dllexport gnu_attr(target("sse3,ssse3,sse4.1,avx,avx2")) UpdateFuncRet fastcall render_layer_turbo_avx2(int32_t layer_index) {
        uint8_t* layer_array = (uint8_t*)this->lists[0].array_ptr;
        AnmVM *const * vms = this->lists[0].vm_array_ptr;

        if (expect((void*)layer_array != (void*)vms, true)) {
            this->__render_layer_prep(layer_index, 0);

            const vec<uint8_t, 32, true>* layers = (vec<uint8_t, 32, true>*)layer_array;
            const vec<uint8_t, 32, true>* layers_end = (vec<uint8_t, 32, true>*)(vms--);

            __asm__("":"+a"(layers_end));
            vec<uint8_t, 32> layer_test = vec_broadcast<uint8_t, 32>((uint8_t)layer_index);
            for(;;) {
                auto mask = _mm256_movemask_epi8((__m256i)(*layers++ == layer_test));
                vms += 32;
                __asm__("":"+S"(layers), "+D"(vms), "+b"(mask));
                if (expect(mask != 0, false)) {
                    if (expect((mask & 0x00000001) != 0, true)) this->draw_vm(vms[-31]);
                    if (expect((mask & 0x00000002) != 0, true)) this->draw_vm(vms[-30]);
                    if (expect((mask & 0x00000004) != 0, true)) this->draw_vm(vms[-29]);
                    if (expect((mask & 0x00000008) != 0, true)) this->draw_vm(vms[-28]);
                    if (expect((mask & 0x00000010) != 0, true)) this->draw_vm(vms[-27]);
                    if (expect((mask & 0x00000020) != 0, true)) this->draw_vm(vms[-26]);
                    if (expect((mask & 0x00000040) != 0, true)) this->draw_vm(vms[-25]);
                    if (expect((mask & 0x00000080) != 0, true)) this->draw_vm(vms[-24]);
                    if (expect((mask & 0x00000100) != 0, true)) this->draw_vm(vms[-23]);
                    if (expect((mask & 0x00000200) != 0, true)) this->draw_vm(vms[-22]);
                    if (expect((mask & 0x00000400) != 0, true)) this->draw_vm(vms[-21]);
                    if (expect((mask & 0x00000800) != 0, true)) this->draw_vm(vms[-20]);
                    if (expect((mask & 0x00001000) != 0, true)) this->draw_vm(vms[-19]);
                    if (expect((mask & 0x00002000) != 0, true)) this->draw_vm(vms[-18]);
                    if (expect((mask & 0x00004000) != 0, true)) this->draw_vm(vms[-17]);
                    if (expect((mask & 0x00008000) != 0, true)) this->draw_vm(vms[-16]);
                    if (expect((mask & 0x00010000) != 0, true)) this->draw_vm(vms[-15]);
                    if (expect((mask & 0x00020000) != 0, true)) this->draw_vm(vms[-14]);
                    if (expect((mask & 0x00040000) != 0, true)) this->draw_vm(vms[-13]);
                    if (expect((mask & 0x00080000) != 0, true)) this->draw_vm(vms[-12]);
                    if (expect((mask & 0x00100000) != 0, true)) this->draw_vm(vms[-11]);
                    if (expect((mask & 0x00200000) != 0, true)) this->draw_vm(vms[-10]);
                    if (expect((mask & 0x00400000) != 0, true)) this->draw_vm(vms[-9]);
                    if (expect((mask & 0x00800000) != 0, true)) this->draw_vm(vms[-8]);
                    if (expect((mask & 0x01000000) != 0, true)) this->draw_vm(vms[-7]);
                    if (expect((mask & 0x02000000) != 0, true)) this->draw_vm(vms[-6]);
                    if (expect((mask & 0x04000000) != 0, true)) this->draw_vm(vms[-5]);
                    if (expect((mask & 0x08000000) != 0, true)) this->draw_vm(vms[-4]);
                    if (expect((mask & 0x10000000) != 0, true)) this->draw_vm(vms[-3]);
                    if (expect((mask & 0x20000000) != 0, true)) this->draw_vm(vms[-2]);
                    if (expect((mask & 0x40000000) != 0, true)) this->draw_vm(vms[-1]);
                    if (expect((mask & 0x80000000) != 0, true)) this->draw_vm(vms[0]);
                    __asm__("":"+m"(layer_index), "+m"(layers_end));
                    if (expect(layers == layers_end, false)) {
                        break;
                    }
                    layer_test = vec_broadcast<uint8_t, 32>((uint8_t)layer_index);
                }
                else if (expect(layers == layers_end, false)) {
                    break;
                }
            }
        }

        return UpdateFuncNext;
    }

    dllexport void allocate_tubro_array() {
        this->lists[0].vm_count = 0;
        this->lists[0].array_size = 16;
        this->lists[0].array_ptr = _aligned_malloc(16, 16);
    }

    dllexport static UpdateFuncRet cdecl preprocess_vm_list_to_make_rendering_suck_less(AnmManager* self) {

        //size_t vm_count = AlignUpToMultipleOf2(self->lists[0].vm_count, 16) + 16; // Align up to 16 while making sure to round up 0 to a non-zero value
        size_t vm_count = (self->lists[0].vm_count & -16) + 16; // Align up to 16 while making sure to round up 0 to a non-zero value

        size_t min_alloc_size = vm_count * (sizeof(AnmVM*) + sizeof(uint8_t));

        size_t cur_array_size = self->lists[0].array_size;
        void* turbo_array = self->lists[0].array_ptr;
        if (cur_array_size < min_alloc_size) {
            self->lists[0].array_size = min_alloc_size;
            turbo_array = self->lists[0].array_ptr = _aligned_realloc(turbo_array, min_alloc_size, 16);
        }

        turbo_array = memset_dumb(turbo_array, -1, min_alloc_size);

        uint8_t* layer_array = (uint8_t*)turbo_array;
        AnmVM** vm_array = based_pointer<AnmVM*>(turbo_array, vm_count);

        ZUNList<AnmVM>* cur_node = self->lists[0].world_list.next;
        while (expect(cur_node != NULL, true)) {
            uint32_t vm_state = OffsetField(cur_node, AnmVM, controller.__vm_state, controller.global_list_node);
            if (!vm_state) {
                uint32_t layer = OffsetField(cur_node, AnmVM, data.layer, controller.global_list_node);
                uint32_t temp = layer - 45;
                if (
                    expect((expect(temp <= 8, false) ? (layer = world_layer_lookup[temp]), true : layer < UINT8_MAX), true)
                ) {
                    *layer_array++ = layer;
                    *vm_array++ = OffsetTypePtr(cur_node, AnmVM, controller.global_list_node);
                }
            }
            cur_node = cur_node->next;
        }

        cur_node = self->lists[0].ui_list.next;
        while (expect(cur_node != NULL, true)) {
            uint32_t vm_state = OffsetField(cur_node, AnmVM, controller.__vm_state, controller.global_list_node);
            if (!vm_state) {
                uint32_t layer = OffsetField(cur_node, AnmVM, data.layer, controller.global_list_node);
                uint32_t temp = layer - 26;
                if (expect_chance(temp <= 10, true, 0.99)) {
                    layer = ui_layer_lookup[temp];
                } else {
                    layer = 47;
                }
                *layer_array++ = layer;
                *vm_array++ = OffsetTypePtr(cur_node, AnmVM, controller.global_list_node);
            }
            cur_node = cur_node->next;
        }

        return UpdateFuncNext;
    }

    dllexport static UpdateFuncRet cdecl preprocess_vm_list_to_make_rendering_suck_less2(AnmManager* self) {

        lock_mutex(ANM_LIST_MUTEX);

        size_t vm_count = AlignUpToMultipleOf2(self->lists[0].vm_count, 32);

        size_t min_alloc_size = vm_count * (sizeof(AnmVM*) + sizeof(uint8_t));

        size_t cur_array_size = self->lists[0].array_size;
        void* turbo_array = self->lists[0].array_ptr;
        if (cur_array_size < min_alloc_size) {
            self->lists[0].array_size = min_alloc_size;
            turbo_array = self->lists[0].array_ptr = _aligned_realloc(turbo_array, min_alloc_size, 32);
        }

        turbo_array = memset_dumb(turbo_array, -1, min_alloc_size);

        uint8_t* layer_array = (uint8_t*)turbo_array;
        AnmVM** vm_array = based_pointer<AnmVM*>(turbo_array, vm_count);
        self->lists[0].vm_array_ptr = vm_array;

        ZUNList<AnmVM>* cur_node = self->lists[0].world_list.next;
        while (expect(cur_node != NULL, true)) {
            uint32_t vm_state = OffsetField(cur_node, AnmVM, controller.__vm_state, controller.global_list_node);
            if (!vm_state) {
                uint32_t layer = OffsetField(cur_node, AnmVM, data.layer, controller.global_list_node);
                uint32_t temp = layer - 45;
                if (
                    expect((expect(temp <= 8, false) ? (layer = world_layer_lookup[temp]), true : layer < UINT8_MAX), true)
                ) {
                    *layer_array++ = layer;
                    *vm_array++ = OffsetTypePtr(cur_node, AnmVM, controller.global_list_node);
                }
            }
            cur_node = cur_node->next;
        }

        cur_node = self->lists[0].ui_list.next;
        while (expect(cur_node != NULL, true)) {
            uint32_t vm_state = OffsetField(cur_node, AnmVM, controller.__vm_state, controller.global_list_node);
            if (!vm_state) {
                uint32_t layer = OffsetField(cur_node, AnmVM, data.layer, controller.global_list_node);
                uint32_t temp = layer - 26;
                if (expect_chance(temp <= 10, true, 0.99)) {
                    layer = ui_layer_lookup[temp];
                } else {
                    layer = 47;
                }
                *layer_array++ = layer;
                *vm_array++ = OffsetTypePtr(cur_node, AnmVM, controller.global_list_node);
            }
            cur_node = cur_node->next;
        }

        unlock_mutex(ANM_LIST_MUTEX);

        return UpdateFuncNext;
    }

    //dllexport static std::pair<ZUNList<AnmVM>*,ZUNList<AnmVM>*> regcall __recursive_remove_faster(ZUNList<AnmVM>* child_list_node, ZUNList<AnmVM>* destroy_list_end) asm_symbol_rel(codecave:recursive_remove_fast) {
    dllexport static uint128_t regcall __recursive_remove_faster(ZUNList<AnmVM>* child_list_node, int dummyA, int dummyB, ZUNList<AnmVM>* destroy_list_end) asm_symbol_rel(codecave:recursive_remove_fast) {
        ZUNList<AnmVM>* child_node = OffsetField(child_list_node, AnmVM, controller.child_list_head.next, controller.child_list_node);
        while (child_node != NULL) {
            //auto [next_child, temp_destroy] = __recursive_remove_faster(child_node, destroy_list_end);
            uint128_t temp = __recursive_remove_faster(child_node, UNUSED_DWORD, UNUSED_DWORD, destroy_list_end);
            child_node = (ZUNList<AnmVM>*)temp;
            dummyA = (int32_t)(temp >> 32);
            dummyB = (int32_t)(temp >> 64);
            destroy_list_end = (ZUNList<AnmVM>*)(temp >> 96);
        }
        OffsetField(child_list_node, AnmVM, controller.__vm_state_fast, controller.child_list_node) = AnmVMState::Deleted;

        OffsetField(child_list_node, AnmVM, controller.destroy_list_node.next, controller.child_list_node) = destroy_list_end;
        destroy_list_end = OffsetPtr(child_list_node, AnmVM, controller.destroy_list_node, controller.child_list_node);

        ZUNList<AnmVM>* next_node = child_list_node->next;
        ZUNList<AnmVM>* prev_node = child_list_node->prev;
        if (prev_node) {
            prev_node->next = next_node;
        }
        if (next_node) {
            next_node->prev = prev_node;
        }

        auto ret = std::tuple(destroy_list_end, dummyB, prev_node, next_node);
        return *(uint128_t*)&ret;

        /*
        ZUNList<AnmVM>* destroy_node = OffsetPtr(child_list_node, AnmVM, controller.destroy_list_node, controller.child_list_node);
        destroy_node->append_next_only(destroy_list_end);


        auto ret = std::tuple<ZUNList<AnmVM>*, int, int, ZUNList<AnmVM>*>(destroy_node, dummyB, dummyA, child_list_node->unlinkR_fast_no_clear_no_tail());
        return *(uint128_t*)&ret;
        */
        //return { child_list_node->unlinkR_fast_no_clear_no_tail(), destroy_node };
    }

    dllexport gnu_noinline UpdateFuncRet fastcall on_tick_list_fast(uintptr_t offset) {
        lock_mutex(ANM_LIST_MUTEX);

        ZUNList<AnmVM>* node = *based_pointer<ZUNList<AnmVM>*>(this, offset);

        if (expect(node != NULL, true)) {
            ZUNList<AnmVM>* destroy_list_end = NULL;
            do {
                AnmVM* vm;
                uint32_t vm_state = OffsetField(node, AnmVM, controller.__vm_state_fast, controller.global_list_node);

                if (expect(vm_state == 0, true)) {
                    AnmVM* vm = OffsetTypePtr(node, AnmVM, controller.global_list_node);
                    vm_state = vm->run_anm();
                }
                if (vm_state == 1) {
                    uint128_t temp = __recursive_remove_faster(OffsetPtr(node, AnmVM, controller.child_list_node, controller.global_list_node), UNUSED_DWORD, UNUSED_DWORD, destroy_list_end);
                    destroy_list_end = (ZUNList<AnmVM>*)(temp >> 96);
                }

                /*
                switch (expect(vm_state, AnmVMState::Normal)) {
                    default:
                        unreachable;
                    case AnmVMState::Normal:
                        vm = OffsetTypePtr(node, AnmVM, controller.global_list_node);
                        if (expect(ZUN_FAILED(vm->run_anm_fast()), false)) {
                    case AnmVMState::MarkedForDelete:

                            //auto [_, new_destroy] = __recursive_remove_faster(OffsetPtr(node, AnmVM, controller.child_list_node, controller.global_list_node), destroy_list_end);
                            auto temp = __recursive_remove_faster(OffsetPtr(node, AnmVM, controller.child_list_node, controller.global_list_node), UNUSED_DWORD, UNUSED_DWORD, destroy_list_end);
                            destroy_list_end = (ZUNList<AnmVM>*)(temp >> 96);
                            */
                            /*
                            auto add_to_destroy_list = [&](ZUNList<AnmVM>* node) {
                                node->append_next_only(destroy_list_end);
                                destroy_list_end = node;
                            };

                            add_to_destroy_list(OffsetPtr(node, AnmVM, controller.destroy_list_node, controller.global_list_node));
                            OffsetPtr(node, AnmVM, controller.child_list_node, controller.global_list_node)->unlink_fast_no_tail();
                            *(vec<float, 2>*)OffsetPtr(node, AnmVM, controller.__root_vm, controller.global_list_node) = {};
                            OffsetField(node, AnmVM, controller.__vm_state_fast, controller.global_list_node) = AnmVMState::Deleted;

                            ZUNList<AnmVM>* child_node;
                            ZUNList<AnmVM>* next_child_node;
                            //= OffsetField(node, AnmVM, controller.child_list_head.next, controller.global_list_node);
                            //OffsetField(node, AnmVM, controller.child_list_head.next, controller.global_list_node) = NULL;
                            next_child_node = OffsetPtr(node, AnmVM, controller.child_list_head, controller.global_list_node)->unlinkR_fast_no_tail();
                            while (next_child_node != NULL) {
                                do {
                                    child_node = next_child_node;
                                    if (OffsetField(child_node, AnmVM, controller.__vm_state_fast, controller.child_list_node) != AnmVMState::Deleted) {
                                        add_to_destroy_list(OffsetPtr(child_node, AnmVM, controller.destroy_list_node, controller.child_list_node));
                                        *(vec<float, 2>*)OffsetPtr(child_node, AnmVM, controller.__root_vm, controller.child_list_node) = {};
                                        OffsetField(child_node, AnmVM, controller.__vm_state_fast, controller.child_list_node) = AnmVMState::Deleted;
                                    }
                                    //next_child_node = OffsetField(child_node, AnmVM, controller.child_list_head.next, controller.child_list_node);
                                    //OffsetField(child_node, AnmVM, controller.child_list_head.next, controller.child_list_node) = NULL;
                                    next_child_node = OffsetPtr(child_node, AnmVM, controller.child_list_head, controller.child_list_node)->unlinkR_fast_no_tail();
                                } while (next_child_node != NULL);

                                next_child_node = ((ZUNList<AnmVM>*)child_node->list)[-1].unlinkR_fast_no_tail();
                            }
                            */
                        //}
                    //case AnmVMState::Deleted:;
                //}
                node = node->next;
            } while (expect(node != NULL, true));
            if (expect(destroy_list_end != NULL, false)) {
                ZUNList<FastAnmVM>* free_list = &this->free_list_head;
                __asm__ volatile ("":"+S"(free_list));
                do {
                    --OffsetField(free_list, AnmManager, lists_raw.vm_count, free_list_head);
                    AnmFuncsBase* funcs = OffsetField(destroy_list_end, AnmVM, controller.on_funcs, controller.destroy_list_node);
                    if (expect(funcs != NULL, false)) {
                        OffsetField(destroy_list_end, AnmVM, controller.on_funcs, controller.destroy_list_node) = NULL;
                        funcs->on_destroy();
                        MEM_ALLOC_LOG->delete_mem_static(funcs);
                    }
                    void* special_data = OffsetField(destroy_list_end, AnmVM, controller.special_data, controller.destroy_list_node);
                    if (expect(special_data != NULL, false)) {
                        OffsetField(destroy_list_end, AnmVM, controller.special_data, controller.destroy_list_node) = NULL;
                        OffsetField(destroy_list_end, AnmVM, controller.special_data_size, controller.destroy_list_node) = 0;
                        MEM_ALLOC_LOG->free_mem_static(special_data);
                    }
                    OffsetPtr(destroy_list_end, AnmVM, controller.global_list_node, controller.destroy_list_node)->unlink_fast_no_clear();
                    //OffsetPtr(destroy_list_end, AnmVM, controller.child_list_node, controller.destroy_list_node)->unlink_fast_no_clear_no_tail();
                    uint32_t fast_id = OffsetField(destroy_list_end, AnmVM, controller.fast_id, controller.destroy_list_node);
                    ZUNList<AnmVM>* destroy_list_next = destroy_list_end->next;
                    if (
                        expect(fast_id != INVALID_FAST_ID, true)
                    ) {
                        OffsetField(destroy_list_end, AnmVM, controller.id.full, controller.destroy_list_node) = 0;
                        OffsetField(destroy_list_end, AnmVM, data.current_instruction_offset, controller.destroy_list_node) = -1;
                        OffsetField(destroy_list_end, FastAnmVM, alive, controller.destroy_list_node) = false;
                        free_list->append_fast(OffsetPtr(destroy_list_end, FastAnmVM, fast_node, controller.destroy_list_node));
                    } else {
                        MEM_ALLOC_LOG->delete_mem_static(OffsetTypePtr(destroy_list_end, AnmVM, controller.destroy_list_node));
                    }
                    destroy_list_end = destroy_list_next;
                } while (expect(destroy_list_end != NULL, true));
            }
        }

        unlock_mutex(ANM_LIST_MUTEX);
        return UpdateFuncNext;
    }

    /*
    dllexport UpdateFuncRet thiscall on_tick_ui_fast() {
        lock_mutex(CRITICAL_SECTION_MANAGER.get_mutex(AnmList_CS));

        this->on_tick_list_fast(this->lists[0].ui_list.next);

        unlock_mutex(CRITICAL_SECTION_MANAGER.get_mutex(AnmList_CS));
        return UpdateFuncNext;
    }
    */

    dllexport static UpdateFuncRet cdecl on_tick_world_fast(AnmManager* self) {
        if (expect(__game_thread_not_paused_and_flag_B_is_not_true(), true)) {
            // offsetof(AnmManager, lists[0].world_list.next) = 0x6CC
            self->on_tick_list_fast(0x6CC);
        }
        return UpdateFuncNext;
    }
    dllexport static UpdateFuncRet cdecl on_tick_ui_fast(AnmManager* self) {
        // offsetof(AnmManager, lists[0].ui_list.next) = 0x6E4
        self->on_tick_list_fast(0x6E4);
        return UpdateFuncNext;
    }
#endif

    AnmLoaded* thiscall get_anm_loaded(int32_t slot) {
        return this->loaded_anm_files[slot];
    }
};
ValidateFieldOffset(0x6C8, AnmManager, lists);
ValidateFieldOffset(0x6000710, AnmManager, next_snapshot_fast_id);
ValidateFieldOffset(0x6000E0C, AnmManager, current_blend_mode);
ValidateFieldOffset(0x6000E1C, AnmManager, __d3d_vertex_buffer_6000E1C);

AnmLoaded* thiscall AnmVM::get_anm_loaded() {
    return ANM_MANAGER_PTR->get_anm_loaded(this->data.slot);
}

// Rx49870
dllexport void thiscall AnmLists::on_tick_world() {
    ZUNListIterable<AnmVM> destroy_list;
    destroy_list.initialize(NULL);

    
    for (ZUNList<AnmVM>* vm_node : this->world_list) {
        AnmVM*& vm = vm_node->get_data();
        vm->controller.prev_in_layer = NULL;
        vm->controller.next_in_layer = NULL;
        if (vm->controller.__vm_state == AnmVMState::MarkedForDelete) {
            AnmVM* vm_ptr = vm;
            ANM_MANAGER_PTR->__recursive_remove(vm_ptr, &destroy_list);
        }
        else if (vm->controller.__vm_state == AnmVMState::Normal) {
            if (ZUN_FAILED(vm->run_anm())) {
                AnmVM* vm_ptr = vm;
                ANM_MANAGER_PTR->__recursive_remove(vm_ptr, &destroy_list);
            }
        }
    }

    for (ZUNList<AnmVM>* vm_node : destroy_list) {
        AnmVM*& vm = vm_node->get_data();
        ANM_MANAGER_PTR->destroy_possibly_managed_vm(vm);
    }
}

dllexport void thiscall AnmLists::on_tick_ui() {
    ZUNListIterable<AnmVM> destroy_list;
    destroy_list.initialize(NULL);

    for (ZUNList<AnmVM>* vm_node : this->ui_list) {
        AnmVM*& vm = vm_node->get_data();
        vm->controller.prev_in_layer = NULL;
        vm->controller.next_in_layer = NULL;
        if (vm->controller.__vm_state == AnmVMState::MarkedForDelete) {
            AnmVM* vm_ptr = vm;
            ANM_MANAGER_PTR->__recursive_remove(vm_ptr, &destroy_list);
        }
        else if (vm->controller.__vm_state == AnmVMState::Normal) {
            if (ZUN_FAILED(vm->run_anm())) {
                AnmVM* vm_ptr = vm;
                ANM_MANAGER_PTR->__recursive_remove(vm_ptr, &destroy_list);
            }
        }
    }

    for (ZUNList<AnmVM>* vm_node : destroy_list) {
        AnmVM*& vm = vm_node->get_data();
        ANM_MANAGER_PTR->destroy_possibly_managed_vm(vm);
    }
}

#if INCLUDE_REPLACEMENT_CODE
// Rx4B7F0
dllexport AnmID* thiscall AnmLists::add_vm_to_world_list_back_fast(AnmID* out, AnmVM* vm) {
    vm->controller.global_list_base = this;
    ++this->vm_count;
    this->world_list.append(&vm->controller.global_list_node);
    return ANM_MANAGER_PTR->assign_next_id_to_vm_fast(out, vm);
}
// Rx4B840
dllexport AnmID* thiscall AnmLists::add_vm_to_world_list_front_fast(AnmID* out, AnmVM* vm) {
    vm->controller.global_list_base = this;
    ++this->vm_count;
    this->world_list.prepend(&vm->controller.global_list_node);
    return ANM_MANAGER_PTR->assign_next_id_to_vm_fast(out, vm);
}
// Rx4B890
dllexport AnmID* thiscall AnmLists::add_vm_to_ui_list_back_fast(AnmID* out, AnmVM* vm) {
    vm->controller.global_list_base = this;
    ++this->vm_count;
    this->ui_list.append(&vm->controller.global_list_node);
    return ANM_MANAGER_PTR->assign_next_id_to_vm_fast(out, vm);
}
// Rx4B8E0
dllexport AnmID* thiscall AnmLists::add_vm_to_ui_list_front_fast(AnmID* out, AnmVM* vm) {
    vm->controller.global_list_base = this;
    ++this->vm_count;
    this->ui_list.prepend(&vm->controller.global_list_node);
    return ANM_MANAGER_PTR->assign_next_id_to_vm_fast(out, vm);
}
#endif

#define ParseIntArg(number) \
vm->get_int_arg(IntArg(number), current_instruction->param_mask, number)

#define ParseIntPtrArg(number) \
vm->get_int_ptr_arg(&IntArg(number), current_instruction->param_mask, number)

#define ParseFloatArg(number) \
vm->get_float_arg(FloatArg(number), current_instruction->param_mask, number)

#define ParseFloatPtrArg(number) \
vm->get_float_ptr_arg(&FloatArg(number), current_instruction->param_mask, number)

// Rx2B5D0
dllexport gnu_noinline ZUNResult thiscall AnmVM::run_anm() {
    use_var(this);
    return (ZUNResult)rand();
}

#if INCLUDE_REPLACEMENT_CODE

#define UNUSED_PTR ((void*)UNUSED_DWORD)

extern "C" {
#undef regparm
#undef sysv_abi
//#undef regparm
#define anmcallA fastcall
#define anmcallB

typedef uint64_t anmcallA DispatchAnmInstr(AnmVM* vm, const AnmInstruction* current_instruction) anmcallB;
using DispatchPtr = DispatchAnmInstr *const;

//anmcallB static inline int anmcallA dispatch_direct(AnmVM* vm, const AnmInstruction* offset_to_next);
anmcallB static inline uint64_t anmcallA dispatch(AnmVM* vm, const AnmInstruction* current_instruction);

// 3
anmcallB gnu_noinline uint64_t anmcallA ANM_HALT(AnmVM* vm, const AnmInstruction* current_instruction);
// 4
anmcallB gnu_noinline uint64_t anmcallA ANM_HALT_INVISIBLE(AnmVM* vm, const AnmInstruction* current_instruction);
// 6
anmcallB gnu_noinline uint64_t anmcallA ANM_WAIT(AnmVM* vm, const AnmInstruction* current_instruction);
// 7
anmcallB gnu_noinline uint64_t anmcallA ANM_INTERRUPT_RETURN(AnmVM* vm, const AnmInstruction* current_instruction);
// 100
anmcallB gnu_noinline uint64_t anmcallA ANM_SET_INT(AnmVM* vm, const AnmInstruction* current_instruction);
// 200
anmcallB gnu_noinline uint64_t anmcallA ANM_JUMP(AnmVM* vm, const AnmInstruction* current_instruction);
// 201
anmcallB gnu_noinline uint64_t anmcallA ANM_LOOP(AnmVM* vm, const AnmInstruction* current_instruction);
alignas(64) static constexpr DispatchAnmInstr *const dispatch_table[] = {
    [Anm::anm_halt] = ANM_HALT,
    [Anm::anm_halt_invisible] = ANM_HALT_INVISIBLE,
    [Anm::wait] = ANM_WAIT,
    [Anm::interrupt_return] = ANM_INTERRUPT_RETURN,
    [Anm::set_int] = ANM_SET_INT,
    [Anm::jump] = ANM_JUMP,
    [Anm::loop] = ANM_LOOP,
};

#define dispatch_return(value) \
    pack_udreg((uintptr_t)(value), (uintptr_t)current_instruction)

//#define dispatch_direct(vm, offset_to_next) \
{ \
    uintptr_t offset_temp = (uintptr_t)(offset_to_next); \
    offset_temp += vm->data.current_instruction_offset; \
    current_instruction = (const AnmInstruction*)offset_temp; \
    if (expect(current_instruction->time <= vm->controller.script_time.current, true)) { \
        [[clang::musttail]] return dispatch_table[current_instruction->opcode + 1](vm, current_instruction); \
    } \
    return dispatch_return(0); \
}

#define dispatch_direct(vm, offset_to_next) \
{ \
    __asm__ volatile (\
        "ADDL %[base_instr], %%EDX" \
        : "=d"(current_instruction) \
        : "d"((int32_t)(offset_to_next)), [base_instr]"m"(vm->data.current_instruction_offset) \
    ); \
    if (expect(current_instruction->time <= vm->controller.script_time.current, true)) { \
        [[clang::musttail]] return dispatch_table[current_instruction->opcode + 1](vm, current_instruction); \
        /*__asm__ volatile ( \
            "JMPL *%c[dispatch_table](,%%EAX,4) \n" \
            "INT3" \
            : \
            : "c"(vm), "d"(current_instruction), "a"(current_instruction->opcode), [dispatch_table]"i"(&dispatch_table[1]) \
        );*/\
    } \
    return dispatch_return(0); \
}
/*
anmcallB inline int anmcallA dispatch_direct(AnmVM* vm, const AnmInstruction* offset_to_next) {
    //const AnmInstruction* current_instruction = base_instr;
    current_instruction = IndexInstr((int32_t)offset_to_next);
    if (expect(current_instruction->time <= vm->controller.script_time.current, true)) {
        [[clang::musttail]] return dispatch_table[current_instruction->opcode + 1](vm, current_instruction);
    }
    return 0;
}
anmcallB inline int anmcallA dispatch_directB(AnmVM* vm, const AnmInstruction* current_instruction) {
    //return dispatch_directB(vm, current_instruction);
}
*/
anmcallB inline uint64_t anmcallA dispatch(AnmVM* vm, const AnmInstruction* current_instruction) {
    dispatch_direct(vm, current_instruction->offset_to_next);
}
// 3
anmcallB gnu_noinline uint64_t anmcallA ANM_HALT(AnmVM* vm, const AnmInstruction* current_instruction) {
    vm->data.__visible3 = true;
    vm->controller.script_time--;
    return dispatch_return(0);
}
// 4
anmcallB gnu_noinline uint64_t anmcallA ANM_HALT_INVISIBLE(AnmVM* vm, const AnmInstruction* current_instruction) {
    vm->data.visible = false;
    vm->data.__visible3 = true;
    vm->controller.script_time--;
    return dispatch_return(0);
}
// 6
anmcallB gnu_noinline uint64_t anmcallA ANM_WAIT(AnmVM* vm, const AnmInstruction* current_instruction) {
    vm->controller.script_time.add_raw(-ParseIntArg(0));
    return dispatch(vm, current_instruction);
}
// 7
anmcallB gnu_noinline uint64_t anmcallA ANM_INTERRUPT_RETURN(AnmVM* vm, const AnmInstruction* current_instruction) {
    vm->controller.script_time.set_from_timer(vm->data.interrupt_return_time);
    dispatch_direct(vm, vm->data.interrupt_return_offset);
}
// 100
anmcallB gnu_noinline uint64_t anmcallA ANM_SET_INT(AnmVM* vm, const AnmInstruction* current_instruction) {
    int32_t value = ParseIntArg(1);
    int32_t* write = ParseIntPtrArg(0);
    *write = value;
    return dispatch(vm, current_instruction);
}
// 200
anmcallB gnu_noinline uint64_t anmcallA ANM_JUMP(AnmVM* vm, const AnmInstruction* current_instruction) {
    vm->controller.script_time.set_new(IntArg(1));
    dispatch_direct(vm, IntArg(0));
}
// 201
anmcallB gnu_noinline uint64_t anmcallA ANM_LOOP(AnmVM* vm, const AnmInstruction* current_instruction) {
    int32_t* write = ParseIntPtrArg(0);
    int32_t offset;
    if (expect((*write)-- > 0, true)) {
        vm->controller.script_time.set_new(IntArg(2));
        offset = IntArg(1);
    } else {
        offset = current_instruction->offset_to_next;
    }
    dispatch_direct(vm, offset);
}
}

dllexport gnu_noinline ZUNResult thiscall AnmVM::run_anm_fast() {
    use_var(this);
    return (ZUNResult)rand();

    using namespace Anm;

    if (expect(this->data.__unknown_flag_X, false)) {
        return ZUN_SUCCESS;
    }

    float previous_gamespeed = GAME_SPEED; // ESP+1C
    if (this->data.slowdown_immune) {
        GAME_SPEED.value = 1.0f;
    }
    float slowdown = this->get_custom_slowdown_fast();
    if (slowdown > 0.0f) {
        float new_gamespeed = previous_gamespeed - (slowdown * previous_gamespeed);
        GAME_SPEED.value = new_gamespeed >= 0.0f ? new_gamespeed : 0.0f;
    }
    if (
        ZUN_SUCCEEDED(this->run_on_tick()) &&
        ZUN_SUCCEEDED(this->run_on_tick2())
    ) {
        uint32_t instruction_offset = this->data.current_instruction_offset;
        if (
            instruction_offset >= 0 &&
            !this->data.__unknown_flag_O
        ) {
            this->controller.__timer_18.tick_new();

            const AnmInstruction* current_instruction;
            const AnmInstruction* base_instruction;
            int dispatch_ret;
            if (expect_chance(this->data.run_interrupt, true, 0.33)) {
                current_instruction = base_instruction = this->get_anm_loaded()->get_script(this->data.script_id);
                this->data.current_instruction_offset = (int32_t)base_instruction;
                current_instruction = IndexInstr(instruction_offset);
                dispatch_ret = dispatch(this, current_instruction);
                goto dispatch_handle;
            }
            else if (
                this->data.__unknown_field_B == 1 &&
                __game_thread_flag_B_is_not_true()
            ) {
                current_instruction = base_instruction = this->get_anm_loaded()->get_script(this->data.script_id);
                current_instruction = IndexInstr(instruction_offset);
                if (expect(current_instruction->time <= this->controller.script_time.current, false)) {
                    this->data.current_instruction_offset = (int32_t)base_instruction;
                    dispatch_ret = dispatch(this, current_instruction);
dispatch_handle:
                }
            }
        }
return_static:
        GAME_SPEED.set(previous_gamespeed);
        return ZUN_SUCCESS;
    }
    else {
return_delete:
        GAME_SPEED.set(previous_gamespeed);
        return ZUN_SUCCESS2;
    }
}
#endif

// size: 0x38
struct EffectInitData {
    Float3 position; // 0x0
    Float3 __float3_C; // 0xC
    float __float_18; // 0x18
    float __float_1C; // 0x1C
    D3DCOLOR color; // 0x20
    float __float_24; // 0x24
    uint8_t __byte_28; // 0x28
    padding_bytes(0x3); // 0x29
    Float3 __float3_2C; // 0x2C
    // 0x38
};

// size: 0x48
struct MotionData {
    Float3 position; // 0x0
    Float3 position2; // 0xC
    float speed; // 0x18
    ZUNAngle angle; // 0x1C
    float orbit_radius; // 0x20
    float orbit_velocity; // 0x24
    ZUNAngle ellipse_angle; // 0x28
    float ellipse_ratio; // 0x2C
    ZUNAngle __angle_30; // 0x30
    float speed_factor; // 0x34
    union {
        Float3 misc_float3 = {}; // 0x38
        Float3 axis_velocity; // 0x38
        Float3 orbit_origin; // 0x38
    };
    union {
        uint32_t flags; // 0x44
        struct {
            uint32_t mode : 4; // 1-4
        };
    };
    // 0x48

    // RxA73D0
    dllexport gnu_noinline void thiscall reset() {
        zero_this();
    }
};

typedef struct Enemy Enemy;

// size: 0x4
struct EnemyID {
    uint32_t full; // 0x0
    // 0x4

    // RxAAAC0
    dllexport gnu_noinline Enemy* thiscall __get_enemy_from_sides();

    // RxAADB0
    dllexport gnu_noinline Float3* thiscall get_position();
};

typedef struct PlayerDamageArea PlayerDamageArea;
typedef struct PlayerBullet PlayerBullet;
typedef struct PlayerShotManager PlayerShotManager;

typedef struct ShtFile ShtFile;

using BulletInitFunc = int32_t thiscall(PlayerBullet* self, PlayerDamageArea* damage_source);
using BulletDamageFunc = int32_t cdecl(PlayerBullet* self, Float3* position, Float2* size, float rotation, float radius);

// size: 0x78
struct ShtFileUnknownA {
    unknown_fields(0x2); // 0x0
    int16_t __short_2; // 0x2
    Float2 position; // 0x4
    Float2 size; // 0xC
    // 0x14

    int8_t __sbyte_28; // 0x28
    int8_t __sbyte_29; // 0x29
    // 0x2A

    uint32_t __init_func_index; // 0x48
    uint32_t __unknown_func_B_index; // 0x4C
    uint32_t __unknown_func_C_index; // 0x50
    uint32_t __damage_func_index; // 0x54
    // 0x58
};

struct ShtFile {
    unknown_fields(0x2); // 0x0
    uint16_t __unknownA_count; // 0x2
    unknown_fields(0x5D0); // 0x4
    union {
        ShtFileUnknownA* __unknownA_ptr_array_5D4[1]; // 0x5D4
        int32_t __unknownA_offset_array_5D4[1];
    };
    //ShtFileUnknownA __unknownA_array_180[]; // 0x180
};

// Rx1C60F0
dllexport ShtFile* CACHED_SHT_FILE_PTR;

static inline constexpr size_t PLAYER_OPTION_COUNT = 10;
static inline constexpr size_t PLAYER_EQUIPMENT_OPTION_COUNT = 12;

static inline constexpr size_t PLAYER_TOTAL_OPTION_COUNT = PLAYER_OPTION_COUNT + PLAYER_EQUIPMENT_OPTION_COUNT;
static inline constexpr size_t PLAYER_TOTAL_OPTION_COUNT_BIT_WIDTH = std::bit_width(PLAYER_TOTAL_OPTION_COUNT - 1);

static inline constexpr size_t PLAYER_BULLET_COUNT = 256;
static inline constexpr size_t PLAYER_DAMAGE_SOURCE_COUNT = 256;

// size: 0x4
union DamageID {
    uint32_t full; // 0x0
    struct {
        uint32_t discriminator : 16;
        uint32_t game_side : 1;
        uint32_t : 7;
        uint32_t is_heap_allocated : 1;
    };
    // 0x4

    // RxC0E10
    dllexport gnu_noinline PlayerDamageArea* thiscall get_damage_area_ptr();

    // RxC1F00
    dllexport gnu_noinline void thiscall delete_area();

    // RxC1FF0
    dllexport gnu_noinline void thiscall set_position(Float3* position);
};

enum HitboxType {
    HitboxRotatedRectangle = 0,
    HitboxCircle = 1
};

// size: 0xC4
struct PlayerDamageArea {
    ZUNList<PlayerDamageArea> list_node;  // 0x0
    union {
        uint32_t flags; // 0x14
        struct {
            uint32_t active : 1; // 1
            uint32_t hitbox_type : 3; // 2-4
            uint32_t __unknown_flag_A : 1; // 5
            uint32_t : 1; // 6
            uint32_t __unknown_flag_B : 1; // 7
        };
    };
    float radius; // 0x18
    float __float_1C; // 0x1C
    float radius_delta; // 0x20
    ZUNAngle angle; // 0x24
    float angular_velocity; // 0x28
    Float2 size; // 0x2C
    MotionData motion; // 0x34
    Timer duration; // 0x7C
    DamageID id; // 0x8C
    uint32_t player_bullet_id; // 0x90
    int32_t damage; // 0x94
    int32_t damage_dealt; // 0x98
    int32_t damage_cap; // 0x9C
    int32_t __hit_frequency; // 0xA0
    EnemyID __enemy_id_A4; // 0xA4
    int __int_A8; // 0xA8
    int __int_AC; // 0xAC
    EnemyID __enemy_id_B0; // 0xB0
    uint32_t __unknown_func_index; // 0xB4
    int32_t __int_B8; // 0xB8
    uint32_t side_index; // 0xBC
    GameSide* game_side; // 0xC0
    // 0xC4

    // RxC1000
    bool thiscall is_heap_allocated() {
        return this->id.is_heap_allocated;
    }

    // Rx7A4A0
    void thiscall __set_flag_A() {
        this->__unknown_flag_A = true;
    }

    // Rx106CF0
    void thiscall __set_flag_B(int32_t value) {
        this->__unknown_flag_B = value;
    }

    // RxC2030
    void thiscall set_position(Float3* position) {
        this->motion.position = *position;
    }

    // Rx7A4E0
    void thiscall __set_hit_frequency(int32_t value) {
        this->__hit_frequency = value;
    }

    // Rx88A20
    void thiscall set_game_side(int32_t side) {
        this->side_index = side;
        this->game_side = GAME_MANAGER.get_game_side(side);
    }

    // RxC1F30
    dllexport gnu_noinline void thiscall delete_area();

    // RxC1C50
    dllexport gnu_noinline uint32_t thiscall initialize_as_rotated_rectangle(Float3* position, float width, float height, float rotation, int32_t duration, int32_t damage) {
        this->active = true;
        this->hitbox_type = HitboxRotatedRectangle;
        this->__unknown_flag_A = false;
        this->__unknown_flag_B = false;
        this->motion.reset();
        this->set_position(position);
        this->size.x = width;
        this->size.y = height;
        this->angle.value = rotation;
        this->angular_velocity = 0.0f;
        this->duration = duration;
        this->damage = damage;
        this->damage_dealt = 0;
        this->damage_cap = 9999999;
        this->__hit_frequency = 1;
        this->__unknown_func_index = 0;
        this->__enemy_id_A4.full = 0;
        this->__int_A8 = 0;
        this->__int_AC = 0;
        return this->id.full;
    }

    // RxC1D80
    dllexport gnu_noinline uint32_t thiscall initialize_as_circle(Float3* position, float radius, float radius_delta, int32_t duration, int32_t damage) {
        this->active = true;
        this->hitbox_type = HitboxCircle;
        this->__unknown_flag_A = false;
        this->__unknown_flag_B = false;
        this->motion.reset();
        this->set_position(position);
        this->radius = radius;
        this->radius_delta = radius_delta;
        this->duration = duration;
        this->damage = damage;
        this->damage_dealt = 0;
        this->damage_cap = 9999999;
        this->__hit_frequency = 1;
        this->__unknown_func_index = 0;
        this->__enemy_id_A4.full = 0;
        this->__int_A8 = 0;
        this->__int_AC = 0;
        return this->id.full;
    }

    // RxC03D0
    void thiscall update();
};

// size: 0xC460
struct HitboxManager : ZUNTask {
    //ZUNTask base; // 0x0
    std::array<PlayerDamageArea, PLAYER_DAMAGE_SOURCE_COUNT> damage_areas; // 0x10
    uint32_t __next_damage_id; // 0xC410
    ZUNListIterable<PlayerDamageArea> tick_list; // 0xC414
    ZUNListIterable<PlayerDamageArea> free_list; // 0xC42C
    int32_t damage_area_count; // 0xC444
    Timer __timer_C448; // 0xC448
    uint32_t side_index; // 0xC458
    GameSide* game_side; // 0xC45C
    // 0xC460

    // RxC02D0
    UpdateFuncRet thiscall on_tick();
    
    // RxC0C60
    dllexport gnu_noinline void thiscall __update_next_damage_id() {
        uint32_t next_id = this->__next_damage_id + 1 & 0xFFFF;
        if (!(next_id & 0xFFFF)) {
            next_id = 1;
        }
        this->__next_damage_id = next_id;
    }

    // RxC0D00
    dllexport gnu_noinline PlayerDamageArea* thiscall get_damage_area_with_id(DamageID id) {
        if (!id.full) {
            return NULL;
        }
        for (ZUNList<PlayerDamageArea>* node : this->tick_list) {
            if (node->get_data()->id.full == id.full) {
                return node->get_data();
            }
        }
        return NULL;
    }

    // RxC1EB0
    dllexport gnu_noinline void thiscall cleanup_damage_area(PlayerDamageArea* damage_area) {
        if (damage_area) {
            damage_area->list_node.unlink();
            if (!damage_area->is_heap_allocated()) {
                this->free_list.prepend(&damage_area->list_node);
            }
        }
    }

    // RxC0E60
    dllexport gnu_noinline PlayerDamageArea* thiscall allocate_new_damage_area() {
        ZUNList<PlayerDamageArea>* damage_area_node = this->free_list.get_next();
        if (damage_area_node) {
            damage_area_node->unlink();
            this->tick_list.append(damage_area_node);
            PlayerDamageArea* damage_area = damage_area_node->get_data();
            damage_area->id.full = this->__next_damage_id;
            this->__update_next_damage_id();
            return damage_area;
        }
        else {
            PlayerDamageArea* damage_area = MEM_ALLOC_LOG->allocate_new<PlayerDamageArea>();
            ZUNList<PlayerDamageArea>* damage_area_node = &damage_area->list_node;
            damage_area_node->initialize(damage_area);
            this->tick_list.append(damage_area_node);
            damage_area->id.full = this->__next_damage_id | 0x01000000;
            damage_area->set_game_side(0);
            this->__update_next_damage_id();
            return damage_area;
        }
    }

    // RxC1AE0
    dllexport gnu_noinline DamageID& thiscall create_damage_area_rotated_rectangle(DamageID& out, Float3* position, float width, float height, float rotation, int32_t duration, int32_t damage) {
        out.full = this->__next_damage_id;
        PlayerDamageArea* damage_area = this->allocate_new_damage_area();
        if (damage_area) {
            damage_area->set_game_side(this->side_index);
            damage_area->initialize_as_rotated_rectangle(position, width, height, rotation, duration, damage);
            out.full = damage_area->id.full;
        } else {
            out.full = 0;
        }
        return out;
    }

    // RxC1B80
    dllexport gnu_noinline DamageID& thiscall create_damage_area_circle(DamageID& out, Float3* position, float radius, float radius_delta, int32_t duration, int32_t damage) {
        out.full = this->__next_damage_id;
        PlayerDamageArea* damage_area = this->allocate_new_damage_area();
        if (damage_area) {
            damage_area->set_game_side(this->side_index);
            damage_area->initialize_as_circle(position, radius, radius_delta, duration, damage);
            out.full = damage_area->id.full;
        }
        else {
            out.full = 0;
        }
        return out;
    }
};

// RxC1F30
dllexport gnu_noinline void thiscall PlayerDamageArea::delete_area() {
    if (!this->id.full) {
        return;
    }

    DamageID damage_id = this->id;
    this->game_side->hitbox_manager_ptr->cleanup_damage_area(this);
    this->active = false;
    this->id.full = 0;

    if (damage_id.is_heap_allocated) {
        MEM_ALLOC_LOG->delete_mem(this);
    }
}

// RxC0E10
dllexport gnu_noinline PlayerDamageArea* thiscall DamageID::get_damage_area_ptr() {
    HitboxManager* hitbox_manager = get_hitbox_manager(0);
    PlayerDamageArea* damage_area_ptr = hitbox_manager->get_damage_area_with_id(*this);
    if (!damage_area_ptr) {
        this->full = 0;
    }
    return damage_area_ptr;
}

// RxC1F00
dllexport gnu_noinline void thiscall DamageID::delete_area() {
    if (this->get_damage_area_ptr()) {
        this->get_damage_area_ptr()->delete_area();
    }
}

// RxC1FF0
dllexport gnu_noinline void thiscall DamageID::set_position(Float3* position) {
    if (this->get_damage_area_ptr()) {
        this->get_damage_area_ptr()->set_position(position);
    }
}

// size: 0xB8
struct BombBase {
    //void* vftable; // 0x0
    // 0x4
    Timer __timer_8; // 0x8
    Timer __timer_18; // 0x18
    MotionData motion; // 0x28
    AnmID __anm_id_70; // 0x70
    AnmID __anm_id_74; // 0x74
    int __dword_78; // 0x78
    int __dword_7C; // 0x7C
    ZUNInterp<int32_t> __int_interp_80; // 0x80
    int __dword_AC; // 0xAC
    uint32_t side_index; // 0xB0
    GameSide* game_side; // 0xB4
    // 0xB8
};

// size: 0xD8
struct BombReimuAInner {
    AnmID __anm_id_0; // 0x0
    MotionData motion; // 0x4
    ZUNInterp<Float3> __float3_interp_4C; // 0x4C
    int __int_A0; // 0xA0
    Timer __timer_A4; // 0xA4
    Float3 __float3_B4; // 0xB4
    int __dword_C0; // 0xC0
    int __dword_C4; // 0xC4
    int __int_C8; // 0xC8
    DamageID damage_id; // 0xCC
    int __dword_D0; // 0xD0
    float __float_D4; // 0xD4
    // 0xD8
};

// size: 0x14F8
struct BombReimuA : BombBase {
    //BombBase base; // 0x0
    std::array<BombReimuAInner, 24> data; // 0xB8
    // 0x14F8
};

// size: 0xCC
struct BombMarisaA : BombBase {
    //BombBase base; // 0x0
    Float3 __float3_B8; // 0xB8
    ZUNAngle __angle_C4; // 0xC4
    AnmID __anm_id_C8; // 0xC8
    // 0xCC
};

// size: 0x3C
struct BombManager : ZUNTask {
    //ZUNTask base; // 0x0
    int __dword_10; // 0x10
    BombBase* bomb_ptr; // 0x14
    BOOL bomb_active; // 0x18
    Timer __timer_1C; // 0x1C
    int __dword_2C; // 0x2C
    int __dword_30; // 0x30
    uint32_t side_index; // 0x34
    GameSide* game_side; // 0x38
    // 0x3C

    // Rx780A0
    bool bomb_allowed();
};

// Copied from UM, update values!
enum ItemID : int32_t {
    InvalidItem = 0,
    PowerItem = 1,
    PointItem = 2,
    BigPowerItem = 3,
    LifeFragmentItem = 4,
    LifeItem = 5,
    BombFragmentItem = 6,
    BombItem = 7,
    FItem = 8,
    Piv5Item = 9,
    Piv10Item = 10,
    Piv20Item = 11,
    Piv30Item = 12,
    Piv40Item = 13,
    Piv50Item = 14,
    Item15 = 15,
    Item16 = 16,
    Item17 = 17,
    Item18 = 18,
    Item19 = 19
};

// size: 0x12C
struct PlayerOption {
    int state; // 0x0
    Float3 __float3_4; // 0x4
    Float3 __float3_10; // 0x10
    Float3 __float3_array_1C[7]; // 0x1C
    Int2 position; // 0x70
    Int2 internal_position; // 0x78
    Int2 __int2_array_80[7]; // 0x80
    Int2 __unfocused_offset; // 0xB8
    Int2 __focused_offset; // 0xC0
    Float3 __float3_C8; // 0xC8
    ZUNAngle __angle_D4; // 0xD4
    float __float_D8; // 0xD8
    AnmID __anm_id_DC; // 0xDC
    AnmID __anm_id_E0; // 0xE0
    Timer __timer_E4; // 0xE4
    int __dword_F4; // 0xF4
    int __dword_F8; // 0xF8
    int __int_FC; // 0xFC
    int __dword_100; // 0x100
    int __dword_104; // 0x104
    int __dword_108; // 0x108
    int __dword_10C; // 0x10C
    float __float_110; // 0x110
    int __dword_114; // 0x114
    int __dword_118; // 0x118
    int __dword_11C; // 0x11C
    int __dword_120; // 0x120
    uint32_t side_index; // 0x124
    GameSide* game_side; // 0x128
    // 0x12C
};

// size: 0x124
struct PlayerBullet {
    ZUNList<PlayerBullet> list_node; // 0x0
    uint32_t id; // 0x14
    AnmID __anm_id_18; // 0x18
    Timer __timer_1C; // 0x1C
    Timer __timer_2C; // 0x2C
    union {
        uint32_t flags; // 0x3C
        struct {
            uint32_t __unknown_flag_A : 1; // 1
            uint32_t __focused : 1; // 2
            uint32_t __unknown_field_A : 4; // 3-6
        };
    };
    BulletInitFunc* __init_func; // 0x40
    void* __unknown_func_B_ptr; // 0x44
    void* __unknown_func_C_ptr; // 0x48
    BulletDamageFunc* __damage_func; // 0x4C
    MotionData motion; // 0x50
    ZUNAngle __angle_98; // 0x98
    int state; // 0x9C
    EnemyID __enemy_id_A0; // 0xA0
    int __int_A4; // 0xA4
    int __dword_A8; // 0xA8
    int32_t __int_AC; // 0xAC
    Float2 size; // 0xB0
    float __float_array_B8[8]; // 0xB8
    int32_t __sht_unknownA_index; // 0xD8
    DamageID __damage_id_DC; // 0xDC
    int __dword_array_E0[8]; // 0xE0
    Float3 __float3_100; // 0x100
    int __dword_10C; // 0x10C
    PlayerOption* option; // 0x110
    unsigned char __byte_114; // 0x114
    padding_bytes(0x3); // 0x115
    uint32_t side_index; // 0x118
    PlayerShotManager* shot_manager; // 0x11C
    GameSide* game_side; // 0x120
    // 0x124

    // Rx106D80
    dllexport gnu_noinline void thiscall set_source_pointers(int32_t side);

    // Rx104D20
    dllexport gnu_noinline void thiscall set_game_side(int32_t side) {
        this->set_source_pointers(side);
    }

    // Rx104E90
    dllexport gnu_noinline ZUNResult thiscall shoot(int32_t unknownA_index, int32_t time, Float3* position, PlayerOption* option, int32_t side);
};

// size: 0x12598
struct PlayerShotManager {
    std::array<PlayerBullet, PLAYER_BULLET_COUNT> bullets; // 0x0
    Timer __timer_12400; // 0x12400
    Timer __timer_12410; // 0x12410
    Timer __timer_12420; // 0x12420
    ZUNListIterable<PlayerBullet> tick_list; // 0x12430
    ZUNListIterable<PlayerBullet> free_list; // 0x12448
    int __int_12460; // 0x12460
    int __dword_12464; // 0x12464
    int __dword_array_12468[30]; // 0x12468
    void* __pointer_array_124E0[30]; // 0x124E0
    ShtFile* sht_file; // 0x12558
    union {
        uint32_t flags; // 0x1255C
        struct {
            uint32_t __unknown_flag_C : 1; // 1
            uint32_t __unknown_flag_A : 1; // 2
            uint32_t : 1; // 3
            uint32_t __unknown_flag_B : 1; // 4
        };
    };
    ZUNAngle __angle_12560; // 0x12560
    float __float_12564; // 0x12564
    Timer __timer_12568; // 0x12568
    int __dword_12578; // 0x12578
    unsigned char __byte_1257C; // 0x1257C
    padding_bytes(0x3); // 0x1257D
    Timer __timer_12580; // 0x12580
    uint32_t side_index; // 0x12590
    GameSide* game_side; // 0x12594
    // 0x12598

    // Rx106D40
    void thiscall set_game_side(int32_t side) {
        this->side_index = side;
        this->game_side = GAME_MANAGER.get_game_side(side);
    }

    // Rx106CD0
    void thiscall __set_timer_12400(int32_t time) {
        this->__timer_12400 = time;
    }

    // Rx104BD0
    dllexport gnu_noinline void thiscall initialize(int32_t side);
};

enum PlayerState : int32_t {

};

// size: 0x14
struct PlayerItemStats {
    float __item_attract_speed; // 0x0
    float item_collect_radius; // 0x4
    float item_attract_radius_focused; // 0x8
    float item_attract_radius_unfocused; // 0xC
    float poc_height; // 0x10
    // 0x14
};

// size: 0x20
struct PlayerHitbox {
    float radius_unfocused; // 0x0
    float radius_focused; // 0x4
    Float3 size_unfocused; // 0x8
    Float3 size_focused; // 0x14
    // 0x20
};

// size: 0x5C
struct PlayerInnerB {
    Timer __timer_0; // 0x0
    Timer __timer_10; // 0x10
    Timer __timer_20; // 0x20
    int __dword_30; // 0x30
    int __dword_34; // 0x34
    int __dword_38; // 0x38
    int __dword_3C; // 0x3C
    Float3 __float3_40; // 0x40
    int __dword_4C; // 0x4C
    unsigned char __byte_50; // 0x50
    padding_bytes(0x3); // 0x51
    uint32_t side_index; // 0x54
    GameSide* game_side; // 0x58
    // 0x5C
};

// size: 0x1485C
struct Player : ZUNTask {
    //ZUNTask base; // 0x0
    int32_t state; // 0x10
    union {
        uint32_t flags; // 0x14
        // Only scale flag has been verified
        struct {
            //uint32_t __unknown_flag_A : 1; // 1
            //uint32_t __unknown_flag_C : 1; // 2
            //uint32_t __unknown_flag_H : 1; // 3
            //uint32_t __unknown_flag_B : 1; // 4
            uint32_t scale_enabled : 1; // 5
            //uint32_t __unknown_flag_E : 1; // 6
            //uint32_t __unknown_flag_G : 1; // 7
            //uint32_t __unknown_field_A : 2; // 8-9
            //uint32_t __unknown_flag_D : 1; // 10
        };
    };
    float __angle_to_player_from_enemy_on_collide; // 0x18
    AnmLoaded* player_anm; // 0x1C
    AnmLoaded* __player_anmB; // 0x20
    AnmLoaded* __player_anmC; // 0x24
    AnmVM __anm_vm_28; // 0x28
    AnmID __anm_id_60C; // 0x60C
    AnmID __anm_id_610; // 0x610
    Float3 position; // 0x614
    Int2 internal_position; // 0x620
    Int2 __int2_628; // 0x628
    Int2 __int2_630; // 0x630
    Float3 __float3_638; // 0x638
    Timer __timer_644; // 0x644
    Timer __timer_654; // 0x654
    Timer __timer_664; // 0x664
    int __dword_674; // 0x674
    int __dword_678; // 0x678
    int __dword_67C; // 0x67C
    int __dword_680; // 0x680
    std::array<PlayerOption, PLAYER_OPTION_COUNT> options; // 0x684
    std::array<PlayerOption, PLAYER_EQUIPMENT_OPTION_COUNT> equipment; // 0x123C
    bool focusing; // 0x204C
    padding_bytes(0x3); // 0x204D
    Timer __timer_2050; // 0x2050
    Timer __timer_2060; // 0x2060
    Timer __timer_2070; // 0x2070
    PlayerItemStats item_stats; // 0x2080
    PlayerHitbox hitbox; // 0x2094
    int32_t unfocused_linear_speed; // 0x20B4
    int32_t focused_linear_speed; // 0x20B8
    int32_t unfocused_diagonal_speed; // 0x20BC
    int32_t focused_diagonal_speed; // 0x20C0
    Float3 velocity; // 0x20C4
    Float3 __last_movement_velocity; // 0x20D0
    Int2 internal_velocity; // 0x20DC
    int32_t __int_20E4; // 0x20E4
    int32_t __int_20E8; // 0x20E8
    float __speed_modifier; // 0x20EC
    Float3 __base_axis_speed; // 0x20F0
    std::array<Int2, 33> __previous_positions; // 0x20FC
    int __int_2204; // 0x2204
    ShtFile* sht_file; // 0x2208
    ZUNInterp<float> scale_interp; // 0x220C
    float scale; // 0x2238
    float damage_multiplier; // 0x223C
    int __dword_from_sht_2240; // 0x2240
    PlayerInnerB __innerB_2244; // 0x2244
    int __int_array_22A0[5]; // 0x22A0
    PlayerShotManager shot_manager; // 0x22B4
    ZUNAngle __angle_1484C; // 0x1484C
    unknown_fields(0x4); // 0x14850
    uint32_t side_index; // 0x14854
    GameSide* game_side; // 0x14858
    // 0x1485C

    // RxFFE40
    void thiscall set_game_side(int32_t side) {
        this->side_index = side;
        this->game_side = GAME_MANAGER.get_game_side(side);
    }

    // RxFF5E0
    bool thiscall focused() {
        return this->focusing;
    }

    // Rx106340
    ShtFile* thiscall get_sht_file() {
        return this->sht_file;
    }

    // RxFF6B0
    PlayerShotManager* thiscall get_shot_manager() {
        return &this->shot_manager;
    }

    // Rx1062F0
    Int2* thiscall get_option_internal_position(int32_t index) {
        return &this->get_option(index)->internal_position;
    }

    // Rx106310
    float thiscall __get_option_angle_D4(int32_t index) {
        return this->get_option(index)->__angle_D4.value;
    }

    // Rx1060A0
    PlayerOption* thiscall get_option(int32_t index) {
        if (index >= 64) {
            return &this->equipment[index - 64];
        } else {
            return &this->options[index];
        }
    }

    // Rx1357C0
    PlayerOption* thiscall get_normal_option(int32_t index) {
        return &this->options[index];
    }

    // RxF9B80
    ZUNResult thiscall __load_sht_file(ShtFile*& sht_file, const char* filename) {
        // sht_file = read_file_to_buffer(filename, NULL, false);
        if (!sht_file) {
            return ZUN_ERROR;
        }
        for (int32_t i = 0; i < sht_file->__unknownA_count; ++i) {
            if (sht_file->__unknownA_offset_array_5D4[i] >= 0) {
                sht_file->__unknownA_ptr_array_5D4[i] = (ShtFileUnknownA*)((uintptr_t)sht_file + sht_file->__unknownA_offset_array_5D4[i]);
            }
        }
    }

    // RxF9520
    ZUNResult thiscall initialize(int32_t side) {
        GAME_MANAGER.get_game_side(side)->player_ptr = this;
        this->set_game_side(side);
        //ANM_MANAGER_PTR->set_current_side(side);

        //std::string filename;
        //filename = format("pl{:02}.anm", this->game_side->globals_side_ptr->character);

        AnmLoaded* player_anm;
        // player_anm = ANM_MANAGER_PTR->preload_anm(filename.data());
        this->player_anm = player_anm;

        if (!player_anm) {
            goto error;
        }
        
        //filename = format("pl{:02}.sht", this->game_side->globals_side_ptr->character);
        //if (ZUN_FAILED(this->__load_sht_file(this->sht_file, filename.data())))
        {
        error:
            // log error
            return ZUN_ERROR;
        }

        this->__player_anmC = this->player_anm;

        // The cache pointer is no longer written to,
        // so this code is just unused
        if (ShtFile* cached_sht_file = CACHED_SHT_FILE_PTR) {
            this->sht_file = cached_sht_file;
            CACHED_SHT_FILE_PTR = NULL;
        }
        else {
            this->shot_manager.initialize(this->side_index);
        }

        // register on_tick
        // register on_draw

        // 
        this->__int_array_22A0[0] = 0;
        this->__int_array_22A0[1] = 1;
        this->__int_array_22A0[2] = 2;
        this->__int_array_22A0[3] = 3;
        this->__int_array_22A0[4] = 4;
    }
};

// Rx106D80
dllexport gnu_noinline void thiscall PlayerBullet::set_source_pointers(int32_t side) {
    this->side_index = side;
    this->game_side = GAME_MANAGER.get_game_side(side);
    this->shot_manager = this->game_side->player_ptr->get_shot_manager();
}

// Rx104E90
dllexport gnu_noinline ZUNResult thiscall PlayerBullet::shoot(int32_t unknownA_index, int32_t time, Float3* position, PlayerOption* option, int32_t side) {
    this->set_source_pointers(side);
    ShtFileUnknownA* unknownA_ptr;
    // unknownA_ptr = this->game_side->player_ptr->__get_sht_unknownA_from_full_index(unknownA_index);

    this->state = 1;
    this->__sht_unknownA_index = unknownA_index;
    this->__timer_1C = 0;
    this->__int_AC = unknownA_ptr->__short_2;
    this->size = unknownA_ptr->size;
    this->option = option;
    this->__focused = this->game_side->player_ptr->focused();

    if (unknownA_ptr->__sbyte_28 & 0xF) {
        // both of these globals are clamped 0..100, so why
        if (this->__focused) {
            this->__int_AC += get_globals_side(0)->__int_9C / 100;
        } else {
            this->__int_AC += get_globals_side(0)->__int_A0 / 100;
        }
    }

    // TODO
}

// Rx104BD0
dllexport gnu_noinline void thiscall PlayerShotManager::initialize(int32_t side) {
    this->set_game_side(side);
    this->sht_file = this->game_side->player_ptr->get_sht_file();
    this->tick_list.initialize(NULL);
    this->free_list.initialize(NULL);

    for (PlayerBullet& bullet : this->bullets) {
        bullet.set_game_side(side);
        bullet.list_node.initialize(&bullet);
        this->free_list.append(&bullet.list_node);
    }

    this->__set_timer_12400(-1);
    this->__timer_12410 = -1;
    this->__timer_12420 = 0;
    this->__angle_12560.value = -PI_f / 2.0f;
    this->__float_12564 = 0.0f;
    this->__timer_12568 = 0;
    this->__int_12460 = 1;
}

// size: 0xC4C
struct Item {
    ZUNList<Item> list_node; // 0x0
    int __dword_14; // 0x14
    AnmVM __vm_18; // 0x18
    AnmVM __vm_5FC; // 0x5FC
    AnmID __vm_id_BE0; // 0xBE0
    Float3 position; // 0xBE4
    Float3 velocity; // 0xBF0
    float speed; // 0xBFC
    float angle; // 0xC00 (*Not* a ZUNAngle? No constructor)
    Timer __timer_C04; // 0xC04
    Timer __timer_C14; // 0xC14
    uint32_t state; // 0xC24
    ItemID id; // 0xC28
    int __dword_C2C; // 0xC2C
    float __float_C30; // 0xC30
    int __int_C34; // 0xC34
    int __dword_C38; // 0xC38
    int __dword_C3C; // 0xC3C
    int32_t sound_id; // 0xC40
    uint32_t side_index; // 0xC44
    GameSide* game_side; // 0xC48
    // 0xC4C
};

// size: 0x49C89C
struct ItemManager : ZUNTask {
    //ZUNTask base; // 0x0
    int __dword_10; // 0x10
    UpdateFunc* on_draw_func_B; // 0x14
    std::array<Item, 0x600> items; // 0x18
    ZUNListIterable<Item> tick_list; // 0x49C818
    ZUNListIterable<Item> free_list; // 0x49C830
    ZUNListIterable<Item> __list_49C848; // 0x49C848 (Cancel items free list? Is that still a thing?)
    float slowdown_factor; // 0x49C860
    int32_t items_onscreen; // 0x49C864
    int32_t item_count; // 0x49C868
    int32_t __int_49C86C; // 0x49C86C
    int32_t __int_49C870; // 0x49C870
    int __dword_49C874; // 0x49C874
    int32_t __int_49C878; // 0x49C878
    int __int_49C87C; // 0x49C87C
    int __dword_49C880; // 0x49C880
    int __dword_49C884; // 0x49C884
    Float3 __float3_49C888; // 0x49C888
    uint32_t side_index; // 0x49C894
    GameSide* game_side; // 0x49C898
    // 0x49C89C
};

typedef struct Stage Stage;

struct StdQuad {
};

struct StdEntry {
};

struct StdHeader {
};

struct StdFace {
};

// size: 0x8+
struct StdInstruction {
    int32_t time; // 0x0
    int16_t opcode; // 0x4
    int16_t size; // 0x6
    unsigned char args[]; // 0x8
};

// size: 0x3310
struct StdVM {
    Timer script_time; // 0x0, 0x10
    int32_t current_instruction_offset; // 0x10, 0x20
    int32_t shaking_mode; // 0x14, 0x24
    Timer shaking_timer; // 0x18, 0x28
    Timer __shaking_6_timer; // 0x28, 0x38
    ZUNInterp<Float3> camera_facing_interp; // 0x38, 0x48
    ZUNInterp<Float3> camera_position_interp; // 0x8C, 0x9C
    ZUNInterp<Float3> camera_rotation_interp; // 0xE0, 0xF0
    ZUNInterp<StageSky> sky_data_interp; // 0x134, 0x144
    ZUNInterp<float> camera_fov_interp; // 0x1D8, 0x1E8
    StageCamera camera; // 0x204, 0x214
    Stage* full_stage; // 0x370, 0x380
    AnmVM slot_vms[8]; // 0x374, 0x384
    int32_t slot_layers[8]; // 0x3294, 0x32A4
    float draw_distance_squared; // 0x32B4, 0x32C4
    void* __ptr_array_32B8[2]; // 0x32B8, 0x32C8
    float __float_array_32C0[2]; // 0x32C0, 0x32D0
    float __float_array_32C8[2]; // 0x32C8, 0x32D8
    int32_t __int_array_32D0[2]; // 0x32D0, 0x32E0
    Timer __timer_array_32D8[2]; // 0x32D8, 0x32E8
    float __float_array_32F8[2]; // 0x32F8, 0x3308
    float __float_array_3308[2]; // 0x3300, 0x3310
    int __screen_effect_type; // 0x3308, 0x3318
    D3DCOLOR __color_330C; // 0x330C, 0x331C
    // 0x3310, 0x3320
};

// size: 0x336C
struct Stage : ZUNTask {
    //ZUNTask base; // 0x0
    StdVM std_vm; // 0x10
    AnmVM* face_vms; // 0x3320
    StdHeader* std_file; // 0x3324
    StdEntry* entries; // 0x3328
    StdFace* faces; // 0x332C
    StdInstruction* script; // 0x3330
    AnmLoaded* anm_file; // 0x3334
    int __int_3338; // 0x3338
    int __int_333C; // 0x333C
    int __int_3340; // 0x3340
    union {
        uint32_t flags; // 0x3344
        struct {

        };
    };
    Timer __timer_3348; // 0x3348
    int32_t stage_number; // 0x3358
    int __int_335C; // 0x335C
    void* std_file_buffer; // 0x3360
    int32_t std_file_size; // 0x3364
    UpdateFunc* on_draw_func_B; // 0x3368
    // 0x336C
};

typedef struct Enemy Enemy;

// size: 0x14
struct BulletSpriteColorData {
    int32_t sprite_id; // 0x0
    int32_t spawn_effect_id; // 0x4
    int32_t cancel_effect_id; // 0x8
    int32_t cancel_script; // 0xC
    D3DCOLOR __color_10; // 0x10
    // 0x14
};
// size: 0x158
struct BulletSpriteData {
    int32_t anm_script; // 0x0
    BulletSpriteColorData color_data[16]; // 0x4
    float hitbox_size; // 0x144
    int32_t layer; // 0x148
    int __int_10C; // 0x14C
    int __int_110; // 0x150
    int32_t __anm_script_114; // 0x154
    // 0x158
};

enum BulletEffectType : uint32_t {
    EX_DIST = 0, // (broken)
    EX_ANIM = 1,
    EX_ACCEL = 2,
    EX_ANGLE_ACCEL = 3,
    EX_ANGLE = 4,
    EX_SPAWNSOUND = 5, // not a real ex anymore
    EX_BOUNCE = 6,
    EX_INVULN = 7,
    EX_OFFSCREEN = 8,
    EX_SETSPRITE = 9,
    EX_DELETE = 10,
    EX_PLAYSOUND = 11,
    EX_WRAP = 12,
    EX_SHOOT = 13,
    EX_SHOOT_DATA = 14, // Does nothing
    EX_REACT = 15,
    EX_LOOP = 16,
    EX_MOVE = 17,
    EX_VEL = 18,
    EX_VELADD = 19,
    EX_BRIGHT = 20,
    EX_VELTIME = 21,
    EX_SIZE = 22,
    EX_SAVEANGLE = 23,
    EX_ENEMY = 24,
    EX_LAYER = 25,
    EX_DELAY = 26,
    EX_LASER = 27,
    EX_LASER_DATA = 28, // Does nothing for bullets. Changes ID of some lasers
    EX_HITBOX = 29,
    EX_WAIT = 30,
    EX_HOMING = 31,
    EX_UNKNOWN_A = 32
};

// size: 0x2C
struct BulletEffectArgs {
    float float_values[4]; // 0x0
    int32_t int_values[4]; // 0x10
    BulletEffectType type; // 0x20
    int32_t async; // 0x24
    const char* string; // 0x28
    // 0x2C
};

// size: 0x48
struct BulletEffectData {
    Timer timer; // 0x0
    union { // 0x10
        float speed;
        float acceleration;
    };
    union { // 0x14
        float angle;
        float angular_velocity;
    };
    union { // 0x18
        Float3 position;
        Float3 size;
    };
    union { // 0x24
        Float3 velocity;
        Float3 acceleration_vec;
        Float3 target;
    };
    union { // 0x30
        int32_t duration;
        int32_t __offscreen_unknown;
        int32_t wrap_count;
    };
    union { // 0x34
        int32_t max_count;
        int32_t mode;
    };
    union { // 0x38
        int32_t count;
        int32_t flags;
    };
    int32_t type; // 0x3C
    // 0x40

    inline BulletEffectData() {}
};

struct ShooterData {
    int32_t type; // 0x0
    int32_t color; // 0x4
    Float3 position; // 0x8
    float angle1; // 0x14
    float angle2; // 0x18
    float speed1; // 0x1C
    float speed2; // 0x20
    int16_t count1; // 0x24
    int16_t count2; // 0x26
    // 0x28
};

struct ShooterEffectData {
    
    
    uint16_t aim_mode; // 0x38
    // 0x3A
    
    int32_t shoot_sound; // 0x3C
    // 0x40
    int32_t start_transform; // 0x44
    // 0x48
};

// size: 0x528
struct Bullet {
    ZUNList<Bullet> __list_node_0; // 0x0
    union {
        uint32_t flags; // 0x14
        struct {
            // All commented out flags copied from UM and
            // not checked yet, but they're probably the same.
            //uint32_t __unknown_flag_E : 1; // 1
            //uint32_t __unknown_flag_F : 1; // 2
            //uint32_t grazed : 1; // 3
            uint32_t __unknown_flag_D : 1; // 4
            uint32_t __circular_hitbox : 1; // 5
            //uint32_t : 1; // 6
            uint32_t __scale_enabled : 1; // 7
            //uint32_t : 1; // 8
            //uint32_t __unknown_flag_C : 1; // 9
            uint32_t __delay_flag : 1; // 10
            uint32_t __unknown_flag_G : 1; // 11
        };
    };
    int32_t invulnerable_time; // 0x18
    AnmVM* __anm_vm_1C; // 0x1C
    float speed; // 0x20
    unknown_fields(0x8); // 0x24
    int32_t __ex_func_a; // 0x2C
    int32_t offscreen_time; // 0x30
    int32_t cancel_script_id; // 0x34
    int32_t effect_index; // 0x38
    int32_t effect_loop_index; // 0x3C
    int32_t transform_sound; // 0x40
    int32_t layer; // 0x44
    float scale; // 0x48
    int16_t sprite; // 0x4C
    int16_t color; // 0x4E
    uint32_t state; // 0x50
    unknown_fields(0x8); // 0x54
    AnmID __anm_tree_id; // 0x5C
    Bullet* next_in_layer; // 0x60
    Float3 position; // 0x64
    Float3 velocity; // 0x70
    union {
        float hitbox_radius; // 0x7C
        Float2 hitbox_size; // 0x7C
    };
    ZUNAngle angle; // 0x84
    unknown_fields(0x8); // 0x88
    uint64_t active_effects; // 0x90
    std::unique_ptr<std::vector<BulletEffectArgs>> effects; // 0x98
    union {
        std::array<BulletEffectData, 14> effect_data; // 0xA0
        struct {
            BulletEffectData effect_speedup; // 0xA0
            BulletEffectData effect_accel; // 0xE0
            BulletEffectData effect_angle_accel; // 0x120
            BulletEffectData effect_angle; // 0x160
            BulletEffectData effect_bounce; // 0x1A0
            BulletEffectData effect_wait; // 0x1E0
            BulletEffectData effect_wrap; // 0x220
            BulletEffectData effect_homing; // 0x260
            BulletEffectData effect_move; // 0x2A0
            BulletEffectData effect_veladd; // 0x2E0
            BulletEffectData effect_veltime; // 0x320
            BulletEffectData effect_offscreen; // 0x360
            BulletEffectData effect_saveangle; // 0x3A0
            BulletEffectData effect_delay; // 0x3E0
            // 0x420
        };
    };
    ZUNInterp<Float3> effect_move_interp; // 0x420
    ZUNInterp<float> scale_interp; // 0x474
    Timer __timer_4A0; // 0x4A0
    Timer __timer_4B0; // 0x4B0
    Timer __timer_4C0; // 0x4C0
    int __dword_4D0; // 0x4D0
    int __dword_4D4; // 0x4D4
    Timer __timer_4D8; // 0x4D8
    Timer __timer_4E8; // 0x4E8
    Timer __timer_4F8; // 0x4F8
    Timer __timer_508; // 0x508
    int __dword_518; // 0x518
    uint32_t side_index; // 0x51C
    GameSide* game_side; // 0x520
    padding_bytes(0x4); // 0x524
    // 0x528
};
ValidateStructSize(0x528, Bullet);

static inline constexpr int32_t MAX_BULLETS = 2000;

// size: 0x286DA8
struct BulletManager : ZUNTask {
    //ZUNTask base; // 0x0
    Bullet* __bullet_ptr_10; // 0x10
    /*
    Bullet* __bullet_ptr_14; // 0x14
    Bullet* __bullet_cache_A[5]; // 0x18
    Bullet* __bullet_ptr_2C; // 0x2C
    Bullet* __bullet_cache_B[5]; // 0x30
    */
    Bullet* __bullet_cache_A[6]; // 0x14
    Bullet* __bullet_cache_B[6]; // 0x2C
    int32_t __int_44; // 0x44 (ECL variable -9898)
    float player_protect_radius_squared; // 0x48
    Float2 __bounce_bounds; // 0x4C
    Float2 __float2_54; // 0x54
    padding_bytes(0x4); // 0x5C
    std::array<Bullet, MAX_BULLETS + 1> bullets; // 0x60
    std::array<AnmID, MAX_BULLETS + 1> anm_ids; // 0x284E08
    ZUNListIterable<Bullet> bullet_free_list; // 0x286D4C
    ZUNListIterable<Bullet> bullet_tick_list; // 0x286D64
    uint32_t __counter_286D7C; // 0x286D7C
    int32_t __cancel_counter; // 0x286D80
    int __dword_286D84; // 0x286D84
    int32_t __cancel_counter2; // 0x286D88
    AnmLoaded* bullet_anm; // 0x286D8C
    int __dword_286D90; // 0x286D90
    int __dword_286D94; // 0x286D94
    int __dword_286D98; // 0x286D98
    uint32_t side_index; // 0x286D9C
    GameSide* game_side; // 0x286DA0
    padding_bytes(0x4); // 0x286DA4
    // 0x286DA8

    dllexport UpdateFuncRet thiscall on_draw_new() {
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        anm_manager->__render_layer_prep(12, 0);

        Bullet** cache_list = this->__bullet_cache_A;
        nounroll for (
            size_t i = 0;
            i < countof(this->__bullet_cache_A);
            ++i
        ) {
            for (
                Bullet* bullet = cache_list[i];
                expect(bullet != NULL, true);
                bullet = bullet->next_in_layer
            ) {
                AnmVM* vm = bullet->__anm_vm_1C;
                vm->controller.position = bullet->position;
                if (expect(vm->data.__auto_rotate_mode != 0, true)) {
                    vm->data.rotation.z = reduce_angle_add_fast(bullet->angle.value, HALF_PI_f);
                    vm->data.rotation_enabled = true;
                }
                if (expect(bullet->__scale_enabled != 0, false)) {
                    float scale = bullet->scale;
                    vec<float, 2> scale_vec = { scale, scale };
                    *(vec<float, 2>*)&vm->data.scale2 = scale_vec;
                    //vm->data.scale2.x = scale;
                    //vm->data.scale2.y = scale;
                    vm->data.scale_enabled = true;
                }
                anm_manager->draw_vm(vm);
            }
        }
        return UpdateFuncNext;
    }
};

typedef struct EclVM EclVM;

// size: 0x8
struct EclLocation {
    int32_t sub_index; // 0x0
    int32_t instruction_offset; // 0x4
    // 0x8
};

// size: 0x18
struct EclStack {
    std::vector<int> data; // 0x0
    int32_t pointer; // 0x10
    int32_t base; // 0x14
    // 0x18
};

struct EclFloatInterp {
    
};

// size: 0x48
struct EclContext {
    float time; // 0x0
    EclLocation location; // 0x4
    EclStack stack; // 0xC
    int32_t async_id; // 0x24
    EclVM* vm; // 0x28
    int32_t __int_2C; // 0x2C
    uint8_t difficulty_mask; // 0x30
    padding_bytes(3); // 0x31
    std::vector<EclFloatInterp> float_interps; // 0x34
    union {
        uint32_t flags; // 0x1204
        struct {
            uint32_t __unknown_flag_A : 1;
        };
    };// 0x44
    // 0x48
};

static inline constexpr size_t MAX_ECL_FILE_COUNT = 64;
static inline constexpr size_t MAX_ECL_ANM_FILES = 8;

// size: 0x234
struct SptResource {
    // void* vftable; // 0x0
    int32_t file_count; // 0x4
    int32_t sub_count; // 0x8
    void* files[MAX_ECL_FILE_COUNT]; // 0xC
    int __array_10C[MAX_ECL_FILE_COUNT]; // 0x10C
    std::vector<void*> __subs; // 0x20C
    EclStack __wtf_stack; // 0x21C
    // 0x234
};

// size: 0x23C
struct EclManager : SptResource {
    // SptResource base; // 0x0
    uint32_t side_index; // 0x234
    GameSide* game_side; // 0x238
    // 0x23C
};

// size: 0x70
struct EclVM {
    // void* vftable;
    EclContext* next_context; // 0x4
    EclContext* prev_context; // 0x8
    EclContext* current_context; // 0xC
    EclContext context; // 0x10
    EclManager* controller; // 0x58
    ZUNList<EclContext> context_list; // 0x5C
    // 0x70
};

// size: 0x14
struct EnemyAnm {
    AnmID id; // 0x0
    // 0x4
    
};

// size: 0x184
struct EnemyMotion {
    MotionData motion; // 0x0
    ZUNInterpEx<Float3> position_interp; // 0x48
    ZUNInterp<ZUNAngle> angle_interp; // 0xAC
    ZUNInterp<float> speed_interp; // 0xD8
    ZUNInterp<Float2> orbit_radius_interp; // 0x104
    ZUNInterp<Float2> ellipse_interp; // 0x144
    // 0x184
};

// size: 0x30
struct EnemyEclVariables {
    int32_t EI0; // 0x0
    int32_t EI1; // 0x4
    int32_t EI2; // 0x8
    int32_t EI3; // 0xC
    float EF0; // 0x10
    float EF1; // 0x14
    float EF2; // 0x18
    float EF3; // 0x1C
    float EF4; // 0x20
    float EF5; // 0x24
    float EF6; // 0x28
    float EF7; // 0x2C
    // 0x30
};

// size: 0x54
struct EnemyInitData {
    Float3 position; // 0x0
    int32_t score; // 0xC
    int32_t item_drop; // 0x10
    int32_t life; // 0x14
    int32_t mirrored; // 0x18
    int32_t __basic_anm_update; // 0x1C
    EnemyEclVariables variables; // 0x20
    EnemyID parent_id; // 0x50
    // 0x54
};

// wow
// size: 0x4
struct EnemyScore {
    int32_t score; // 0x0
    // 0x4
};

// size: 0xA8
struct EnemyDrops {
    int32_t game_side_index; // 0x0
    int32_t main_id; // 0x4
    int32_t override_id; // 0x8
    int32_t extra_ids[16]; // 0xC
    int32_t limited_extra_ids[16]; // 0x4C
    int32_t limited_end_time; // 0x8C
    Timer limited_timer; // 0x90
    float width; // 0xA0
    float height; // 0xA4
    // 0xA8
};

// size: 0x1C
struct EnemyLife {
    int32_t current; // 0x0
    int32_t maximum; // 0x4
    int32_t remaining_current_attack; // 0x8
    int32_t current_scaled_by_seven; // 0xC
    int32_t starting_value_for_next_attack; // 0x10
    int32_t total_damage_including_ignored; // 0x14
    union {
        uint32_t flags; // 0x18
        struct {
            uint32_t is_spell : 1; // 1
            uint32_t __unknown_flag_A : 1; // 2
        };
    };
    // 0x1C
};

typedef char EclSubName[64];

// size: 0x88
struct EnemyCallback {
    int32_t life; // 0x0
    int32_t time; // 0x4
    EclSubName life_sub; // 0x8
    EclSubName time_sub; // 0x48
    // 0x88
};

using ExtraDamageFunc = void;
using FuncSetFunc = void;
using ExtraHitboxFunc = void;

// size: 0x2F0
struct EnemyData {
    EnemyID id; // 0x0
    EnemyID parent_id; // 0x4
    EclVM* ecl_vm; // 0x8
    std::vector<EnemyAnm> anms; // 0xC
    int32_t anm_source_index; // 0x1C
    int32_t anm_slot_0_source_index; // 0x20
    int32_t anm_slot_0_script; // 0x24
    int32_t current_anm_script; // 0x28
    int32_t current_anm_pose; // 0x2C
    int32_t kill_id; // 0x30
    int32_t anm_base_layer; // 0x34
    float hitbox_rotation; // 0x38
    float __float_3C; // 0x3C
    float __float_40; // 0x40
    int __dword_44; // 0x44
    int32_t __int_48; // 0x48
    float bomb_damage_multiplier; // 0x4C
    int __dword_50; // 0x50
    int32_t bombshield_on_anm; // 0x54
    int32_t bombshield_off_anm; // 0x58
    Float2 hitbox_size; // 0x5C
    Float2 collision_size; // 0x64
    Float3 __float3_6C; // 0x6C
    EnemyEclVariables variables; // 0x78
    Timer ecl_time; // 0xA8
    Timer boss_timer; // 0xB8
    MotionData previous_motion; // 0xC8
    MotionData current_motion; // 0x110
    std::vector<EnemyMotion> motion; // 0x158
    //std::vector<ShooterData> shooters; // 0x168
    Float2 final_sprite_size; // 0x170
    Float2 move_bounds_center; // 0x178
    Float2 move_bounds_size; // 0x180
    EnemyScore score; // 0x188
    EnemyLife life; // 0x18C
    EnemyDrops drops; // 0x1A8
    int32_t death_sound; // 0x250
    int32_t death_anm_script; // 0x254
    int32_t death_anm_index; // 0x258
    int32_t __int_25C; // 0x25C
    int __dword_260; // 0x260
    int32_t hit_sound; // 0x264
    int32_t __int_268; // 0x268
    int __dword_26C; // 0x26C
    int32_t boss_id; // 0x270
    float player_protect_radius_squared; // 0x274
    int32_t __stone_value; // 0x278
    int32_t death_callback_sub; // 0x27C
    int32_t chapter; // 0x280
    int32_t chapter_spawn_weight; // 0x284
    Timer invulnerable_timer; // 0x288
    Timer no_collision_timer; // 0x298
    Timer __timer_2A8; // 0x2A8
    std::vector<EnemyCallback> callbacks; // 0x2B8
    union {
        uint32_t flags_low; // 0x2C8
        struct {
            uint32_t disable_hitbox : 1; // 1
            uint32_t disable_collision : 1; // 2
            uint32_t offscreen_immune_horizontal : 1; // 3
            uint32_t offscreen_immune_vertical : 1; // 4
            uint32_t invincible : 1; // 5
            uint32_t intangible : 1; // 6
            uint32_t __is_midboss : 1; // 7
            uint32_t no_delete : 1; // 8
            uint32_t always_delete : 1; // 9
            uint32_t grazable : 1; // 10
            uint32_t only_dialogue_delete : 1; // 11
            uint32_t delete_as_bullet : 1; // 12
            uint32_t rectangular_hitbox : 1; // 13
            uint32_t slowdown_immune : 1; // 14
        };
    };
    union {
        uint32_t flags_mid; // 0x2CC
        struct {
            uint32_t __unknown_flag_G : 1; // 1
            uint32_t move_bounds_enable : 1; // 2
            uint32_t __unknown_flag_A : 1; // 3
            uint32_t mirrored : 1; // 4
            uint32_t __unknown_flag_I : 1; // 5
            uint32_t __damaged_this_frame : 1; // 6
            uint32_t __delete_related : 1; // 7 like is_boss, but skips something in the GUI code
            uint32_t is_boss : 1; // 8
            uint32_t __unknown_flag_L : 1; // 9
            uint32_t __unknown_flag_B : 1; // 10
            uint32_t __basic_anm_update : 1; // 11
            uint32_t homing_disable : 1; // 12
            uint32_t bomb_shield : 1; // 13
            uint32_t bomb_shield_active : 1; // 14
            uint32_t __unknown_flag_O : 1; // 15
            uint32_t __anm_related_flag_A : 1; // 16
            uint32_t __anm_slowdown_immune : 1; // 17
            uint32_t : 1; // 18
            uint32_t __unknown_flag_P : 1; // 19
            uint32_t : 12; // 20-30
            uint32_t __is_stone_enemy : 1; // 31
            uint32_t : 1; // 32
        };
    };
    union {
        uint32_t flags_high; // 0x2D0
        struct {
            uint32_t stone_color : 2; // 1-2
            uint32_t __can_have_children : 1; // 3
            uint32_t __parent_movement_related : 1; // 4
            uint32_t : 2; // 5-6
            uint32_t __unknown_flag_Q : 1; // 7
        };
    };
    uint32_t __is_func_set_2; // 0x2D8
    ExtraDamageFunc* extra_damage_func; // 0x2DC
    FuncSetFunc* func_set_func; // 0x2E0
    ExtraHitboxFunc* extra_hitbox_func; // 0x2E4
    uint32_t side_index; // 0x2E8
    GameSide* game_side; // 0x2EC
    // 0x2F0
};

// size: 0x428
struct Enemy : EclVM {
    //EclVM base; // 0x0
    int __dword_70; // 0x70
    ZUNList<Enemy> global_list_node; // 0x74
    EnemyData data; // 0x88
    EnemyInitData init_data; // 0x378
    ZUNListIterable<void> child_list; // 0x3CC
    ZUNList<void> child_list_node; // 0x3E4
    // 0x3F8
    
    uint32_t side_index; // 0x420
    GameSide* game_side; // 0x424
    // 0x428
};

static inline constexpr size_t MAX_BOSS_COUNT = 16;

// size: 0xA4
struct EnemyManagerData {
    EnemyEclVariables vars; // 0x0
    int32_t story_boss_difficulty; // 0x30
    BOOL story_boss_final_phase; // 0x34
    int32_t player_death_count; // 0x38
    int32_t player_bombs_used; // 0x3C
    int32_t can_capture_spell; // 0x40
    std::array<int32_t, MAX_BOSS_COUNT> boss_ids; // 0x44
    union {
        uint32_t flags; // 0x84
        struct {
            uint32_t __unknown_flag_A : 1; // 1
        };
    };
    int32_t enemy_limit; // 0x88
    Timer __timer_8C; // 0x8C
    int __dword_9C; // 0x9C
    int __dword_A0; // 0xA0
    // 0xA4
};

// size: 0x134
struct EnemyManager : ZUNTask {
    //ZUNTask base; // 0x0
    EnemyManagerData data; // 0x10
    std::vector<unsigned char> __vector_B4; // 0xB4
    int __dword_C4; // 0xC4
    int __dword_C8; // 0xC8
    int __dword_CC; // 0xCC
    Timer __timer_D0; // 0xD0
    int32_t __int_E0; // 0xE0
    std::array<AnmLoaded*, MAX_ECL_ANM_FILES> enemy_anms; // 0xE4
    int __dword_104; // 0x104
    ZUNListIterable<Enemy> enemy_list; // 0x108
    int __dword_120; // 0x120
    int __dword_124; // 0x124
    ZUNAngle __angle_128; // 0x128
    uint32_t side_index; // 0x12C
    GameSide* game_side_ptr; // 0x130
    // 0x134
    
    // RxA4A20
    void kill_all() {
        
        //this->data.__timer_8C++;
    }

    // Rx98A80
    dllexport gnu_noinline Enemy* thiscall get_enemy_with_id(uint32_t id) {
        // TODO
        use_var(this);
        use_var(id);
        return (Enemy*)rand();
    }

    // Rx9BE20
    dllexport gnu_noinline bool thiscall __boss_is_active() {
        // TODO
        use_var(this);
        return (bool)rand();
    }

    // RxA8920
    dllexport gnu_noinline Enemy* thiscall allocate_new_enemy(const char* sub_name, EnemyInitData* data, Enemy* parent) {
        use_var(this);
        use_var(sub_name);
        use_var(data);
        use_var(parent);
        return (Enemy*)rand();
    }

    // RxAAE10
    dllexport gnu_noinline AnmLoaded* thiscall anm_file_lookup(int32_t index) {
        use_var(this);
        use_var(index);
        return (AnmLoaded*)rand();
    }
};

// RxAAAC0
dllexport gnu_noinline Enemy* thiscall EnemyID::__get_enemy_from_sides() {
    Enemy* ret = NULL;
    if (get_enemy_manager(0)) {
        ret = get_enemy_manager(0)->get_enemy_with_id(this->full);
    }
    return ret;
}

// RxAADB0
dllexport gnu_noinline Float3* thiscall EnemyID::get_position() {
    if (this->__get_enemy_from_sides()) {
        return &this->__get_enemy_from_sides()->data.current_motion.position;
    }
    static Float3 DUMMY_FLOAT3 = {};
    return &DUMMY_FLOAT3;
}

// Rx173FF0
int32_t STONE_LIFE_LOOKUP[4][5] = {
    { 3200, 3400, 3700, 3900, 4000 },
    { 3200, 3400, 3700, 3900, 4000 },
    { 3200, 3400, 3700, 3900, 4000 },
    { 3200, 3400, 3700, 3900, 4000 }
};
// Rx1B09D8
char* STONE_SUB_NAMES[4] = {
    "StoneR",
    "StoneB",
    "StoneY",
    "StoneG"
};
// Rx1B09E8
char* STONE_ATTACK_SUB_NAMES[4][5] = {
    { "StoneAttackLevel_R1", "StoneAttackLevel_R2", "StoneAttackLevel_R3", "StoneAttackLevel_R4", "StoneAttackLevel_R5" },
    { "StoneAttackLevel_B1", "StoneAttackLevel_B2", "StoneAttackLevel_B3", "StoneAttackLevel_B4", "StoneAttackLevel_B5" },
    { "StoneAttackLevel_Y1", "StoneAttackLevel_Y2", "StoneAttackLevel_Y3", "StoneAttackLevel_Y4", "StoneAttackLevel_Y5" },
    { "StoneAttackLevel_G1", "StoneAttackLevel_G2", "StoneAttackLevel_G3", "StoneAttackLevel_G4", "StoneAttackLevel_G5" }
};
// Rx1B0A38
D3DCOLOR STONE_COLOR_LOOKUP[4] = {
    PackD3DCOLOR(0xFF, 0xFF, 0x40, 0x40),
    PackD3DCOLOR(0xFF, 0x40, 0x40, 0xFF),
    PackD3DCOLOR(0xFF, 0xF0, 0xF0, 0x40),
    PackD3DCOLOR(0xFF, 0x40, 0xFF, 0x40)
};

// size: 0x34
struct StoneEnemy {
    ZUNList<StoneEnemy> list_node; // 0x0
    EnemyID parent_id; // 0x14
    AnmID __anm_id_18; // 0x18
    Timer ticks_alive; // 0x1C
    int32_t color; // 0x2C
    int32_t level; // 0x30
    // 0x34

    // Rx112F30
    dllexport gnu_noinline void thiscall cleanup() {
        this->list_node.unlink();
        //this->__anm_id_18.mark_tree_for_delete();
        MEM_ALLOC_LOG->delete_mem(this);
    }

    // Rx112FB0
    dllexport gnu_noinline void thiscall initialize(EnemyID parent_id, int32_t stone_type) {
        this->parent_id = parent_id;
        this->ticks_alive = 0;
        this->color = stone_type;

        AnmLoaded* first_ecl_anm = get_enemy_manager(0)->anm_file_lookup(2);

        Float3 position = { 0.0f, 0.0f, 0.0f };
        // This script ID corresponds to some random fairy...?
        //this->__anm_id_18 = first_ecl_anm->instantiate_vm_to_world_list_back2("enemy", 319 + stone_type, &position, 0.0f, -1, NULL);

        EffectInitData effect_init_data = {};
        effect_init_data.position = *this->parent_id.get_position();
        effect_init_data.__float_18 = 0.06f;
        effect_init_data.__float_1C = 128.0f;

        D3DCOLOR colors[4] = {
            PackD3DCOLOR(0xFF, 0xFF, 0x00, 0x00),
            PackD3DCOLOR(0xFF, 0x00, 0x00, 0xFF),
            PackD3DCOLOR(0xFF, 0xFF, 0xFF, 0x00),
            PackD3DCOLOR(0xFF, 0x00, 0xFF, 0x00)
        };

        effect_init_data.color = colors[stone_type];

        //get_effect_manager(0)->instantiate_effect_vm_to_world_list_back(11, &effect_init_data, NULL);

        //this->__anm_id_18.set_associated_entity(this);
        //this->__anm_id_18.set_on_tick2_func(__on_tick_for_stone_enemy);

        Enemy* enemy = this->parent_id.__get_enemy_from_sides();
        EnemyInitData enemy_init_data = {};
        enemy_init_data.position = { 0.0f, 0.0f, 0.0f };
        if (enemy) {
            enemy_init_data.variables = enemy->data.variables;
        }

        int32_t stone_levels[4] = {
            get_globals_side(0)->stone_level_red,
            get_globals_side(0)->stone_level_blue,
            get_globals_side(0)->stone_level_yellow,
            get_globals_side(0)->stone_level_green
        };

        this->level = stone_levels[stone_type];

        enemy = get_enemy_manager(0)->allocate_new_enemy(STONE_ATTACK_SUB_NAMES[stone_type][this->level], &enemy_init_data, enemy);

        switch (get_globals_side(0)->stone_enemy_count_total % 3) {
            case 0:
                enemy->data.drops.main_id = BombFragmentItem;
                break;
            case 1:
                enemy->data.drops.main_id = BombFragmentItem;
                break;
            case 2:
                enemy->data.drops.main_id = LifeFragmentItem;
                break;
        }

        switch (this->level) {
            case 0:
                enemy->data.drops.extra_ids[0] = 31;
                enemy->data.drops.extra_ids[1] = 31;
                enemy->data.drops.width = 64.0f;
                enemy->data.drops.height = 64.0f;
                break;
            case 1:
                enemy->data.drops.extra_ids[0] = 33;
                enemy->data.drops.extra_ids[1] = 33;
                enemy->data.drops.width = 80.0f;
                enemy->data.drops.height = 80.0f;
                break;
            case 2:
                enemy->data.drops.extra_ids[0] = 36;
                enemy->data.drops.extra_ids[1] = 36;
                enemy->data.drops.width = 88.0f;
                enemy->data.drops.height = 88.0f;
                break;
            case 3:
                enemy->data.drops.extra_ids[0] = 39;
                enemy->data.drops.extra_ids[1] = 39;
                enemy->data.drops.width = 96.0f;
                enemy->data.drops.height = 96.0f;
                break;
            case 4:
                enemy->data.drops.extra_ids[0] = 42;
                enemy->data.drops.extra_ids[1] = 42;
                enemy->data.drops.width = 112.0f;
                enemy->data.drops.height = 112.0f;
                break;
        }
        switch (stone_type) {
            case RedStone: // 0
                get_globals_side(0)->stone_enemy_count_red += 1;
                break;
            case BlueStone: // 1
                get_globals_side(0)->stone_enemy_count_blue += 1;
                break;
            case YellowStone: // 2
                get_globals_side(0)->stone_enemy_count_yellow += 1;
                break;
            case GreenStone: // 3
                get_globals_side(0)->stone_enemy_count_green += 1;
                break;
        }
        get_globals_side(0)->stone_enemy_count_total += 1;
    }

    // Rx112990
    dllexport gnu_noinline BOOL thiscall on_tick() {
        Enemy* parent = this->parent_id.__get_enemy_from_sides();
        if (!parent) {
            // The main enemy died
            return true;
        }

        if (this->ticks_alive > 60) {

            EffectInitData effect_init_data = {};

            effect_init_data.color = STONE_COLOR_LOOKUP[this->color];
            RED(effect_init_data.color) -= REPLAY_RNG.rand_uint() % 64;
            GREEN(effect_init_data.color) -= REPLAY_RNG.rand_uint() % 64;
            BLUE(effect_init_data.color) -= REPLAY_RNG.rand_uint() % 64;

            effect_init_data.position = *this->parent_id.get_position();

            //get_effect_manager(0)->instantiate_effect_vm_to_world_list_back(10, &effect_init_data, NULL);
        }
        this->ticks_alive++;

        return false;
    }
};

// size: 0xB4
struct StoneManager {
    ZUNListIterable<StoneEnemy> stone_list; // 0x0
    Timer summon_timer; // 0x18
    Timer passive_boss_meter_timer; // 0x28
    ZUNInterp<float> summon_text_scale_interp; // 0x38
    ZUNInterp<uint8_t> summon_text_alpha_interp; // 0x64
    ZUNInterp<float> summon_meter_drain_interp; // 0x84
    bool summon_active; // 0xB0
    padding_bytes(0x3); // 0xB1
    // 0xB4

    // Invoked from EnemyManager::on_tick
    // Rx1120D0
    dllexport gnu_noinline void thiscall on_tick() {
        if (GAME_MANAGER.__int_70 == 2) {
            get_globals_side(0)->summon_meter = 0;
        }
        if (
            get_enemy_manager(0) &&
            get_enemy_manager(0)->__boss_is_active() &&
            !this->summon_active &&
            !GUI_PTR->msg_vm_active()
        ) {
            if (this->passive_boss_meter_timer % 3 <= 1) {
                get_globals_side(0)->summon_meter += 1;
            }
            this->passive_boss_meter_timer++;
        }
        if (
            !this->summon_active &&
            get_globals_side(0)->summon_meter >= get_globals_side(0)->summon_meter_max &&
            !GUI_PTR->msg_vm_active() //&&
            //STAGE_CLEAR_PTR == NULL
        ) {
            EnemyInitData enemy_init_data = {}; // -0x74

            int32_t stone_type = 0; // -0x80
            int32_t priority = -1; // -0x84
            int32_t stone_level = 0; // -0x7C

            if (get_globals_side(0)->next_stone_enemy_type >= 0) {
                stone_type = get_globals_side(0)->next_stone_enemy_type;
                switch (stone_type) {
                    default:
                    case RedStone: // 0
                        stone_level = get_globals_side(0)->stone_level_red;
                        break;
                    case BlueStone: // 1
                        stone_level = get_globals_side(0)->stone_level_blue;
                        break;
                    case YellowStone: // 2
                        stone_level = get_globals_side(0)->stone_level_yellow;
                        break;
                    case GreenStone: // 3
                        stone_level = get_globals_side(0)->stone_level_green;
                        break;
                }
                get_globals_side(0)->next_stone_enemy_type = -1;
            }
            else {
                if (get_globals_side(0)->stone_priority_red >= priority) {
                    priority = get_globals_side(0)->stone_priority_red;
                    stone_level = get_globals_side(0)->stone_level_red;
                    stone_type = RedStone;
                }
                if (get_globals_side(0)->stone_priority_blue >= priority) {
                    priority = get_globals_side(0)->stone_priority_blue;
                    stone_level = get_globals_side(0)->stone_level_blue;
                    stone_type = BlueStone;
                }
                if (get_globals_side(0)->stone_priority_yellow >= priority) {
                    priority = get_globals_side(0)->stone_priority_yellow;
                    stone_level = get_globals_side(0)->stone_level_yellow;
                    stone_type = YellowStone;
                }
                if (get_globals_side(0)->stone_priority_green >= priority) {
                    priority = get_globals_side(0)->stone_priority_green;
                    stone_level = get_globals_side(0)->stone_level_green;
                    stone_type = GreenStone;
                }
            }

            Float3 position = { REPLAY_RNG.rand_float_signed() * 144.0f, 120.0f, 0.0f };
            enemy_init_data.position = position;
            enemy_init_data.life = STONE_LIFE_LOOKUP[stone_type][stone_level];
            this->summon_active = true;
            get_enemy_manager(0)->allocate_new_enemy(STONE_SUB_NAMES[stone_type], &enemy_init_data, NULL);

            switch (stone_type) {
                case RedStone: // 0
                    get_globals_side(0)->stone_priority_red = 0;
                    break;
                case BlueStone: // 1
                    get_globals_side(0)->stone_priority_blue = 0;
                    break;
                case YellowStone: // 2
                    get_globals_side(0)->stone_priority_yellow = 0;
                    break;
                case GreenStone: // 3
                    get_globals_side(0)->stone_priority_green = 0;
                    break;
            }

            get_globals_side(0)->summon_meter -= get_globals_side(0)->summon_meter_max / 3;
            this->summon_timer = 0;

            this->summon_text_scale_interp.initialize(60, DecelerateSlow, 1.0f, 0.0f);
            this->summon_text_alpha_interp.initialize(120, DecelerateSlow, 255, 0);
            this->summon_meter_drain_interp.initialize(1320, Decelerate, 1.0f, 0.0f);
        }

        size_t stone_count = 0;
        for (ZUNList<StoneEnemy>* node : this->stone_list) {
            ++stone_count;
            if (node->get_data()->on_tick()) {
                node->get_data()->cleanup();
                this->summon_active = false;
            }
        }

        if (this->summon_active) {
            if (
                this->summon_timer > 10 &&
                stone_count == 0
            ) {
                this->summon_active = false;
            }

            get_globals_side(0)->summon_meter = get_globals_side(0)->summon_meter_max * this->summon_meter_drain_interp.recalculate_current_instead_of_just_reading_the_current_field();
            this->summon_timer++;

            this->summon_text_scale_interp.step();
            this->summon_text_alpha_interp.step();
            this->summon_meter_drain_interp.step();

            __asm nop // why is there a nop here

            if (
                !this->summon_meter_drain_interp.__running() ||
                this->summon_timer >= 1800
            ) {
                this->summon_active = false;
                this->summon_timer = 0;
                for (ZUNList<StoneEnemy>* node : this->stone_list) {
                    node->get_data()->cleanup();
                }
            }
        }
    }

    // invoked from EnemyManager::on_draw
    // Rx112AA0
    dllexport gnu_noinline void thiscall on_draw() {
        if (
            this->summon_active &&
            this->stone_list.get_head() && // stone list not empty
            this->summon_timer < 200
        ) {
            StoneEnemy* stone = this->stone_list.get_head()->get_data();
            
            //ASCII_MANAGER_PTR->__sub_rB8620(0, 0);
            //ASCII_MANAGER_PTR->__sub_r88B00(12);

            D3DCOLOR colors[4] = {
                PackD3DCOLOR(0xFF, 0xFF, 0x40, 0x40),
                PackD3DCOLOR(0xFF, 0x40, 0x40, 0xFF),
                PackD3DCOLOR(0xFF, 0xFF, 0xFF, 0x00),
                PackD3DCOLOR(0xFF, 0x40, 0xFF, 0x40)
            };

            if (this->summon_timer % 8 <= 5) {
                //ASCII_MANAGER_PTR->set_color(colors[stone->color]);
            }
            else {
                //ASCII_MANAGER_PTR->set_color(PackD3DCOLOR(0xFF, 0xFF, 0xFF, 0xFF));
            }

            Float3 position = { 224.0f, 160.0f, 0.0f };

            Float3 parent_position;
            //parent_position = get_enemy_manager(0)->get_enemy_with_id(stone->parent_id.full)->get_current_motion().position;
            if (parent_position.x < -112.0f) {
                parent_position.x = -112.0f;
            }
            else if (parent_position.x > 112.0f) {
                parent_position.x = 112.0f;
            }
            position.x = parent_position.x;
            position.x += 224.0f;

            //ASCII_MANAGER_PTR->add_string(&position, std::string_view("Wonder Stone Appeared!"));
            position.y += 16.0f;
            //ASCII_MANAGER_PTR->add_string(&position, std::string_view("Level "), stone->level);
            position.y -= 16.0f;

            float scale = this->summon_text_scale_interp.recalculate_current_instead_of_just_reading_the_current_field();

            //ASCII_MANAGER_PTR->__sub_rE6920(1);
            //ASCII_MANAGER_PTR->set_scale(scale + 1.0f, scale + 1.0f);
            //ASCII_MANAGER_PTR->set_alpha(this->summon_text_alpha_interp.recalculate_current_instead_of_just_reading_the_current_field());
            
            //ASCII_MANAGER_PTR->add_string(&position, std::string_view("Wonder Stone Appeared!"));
            position.y += 16.0f;
            //ASCII_MANAGER_PTR->add_string(&position, std::string_view("Level "), stone->level);
            position.y -= 16.0f;

            //ASCII_MANAGER_PTR->set_scale(scale * 0.66f + 1.0f, scale * 0.66f + 1.0f);

            //ASCII_MANAGER_PTR->add_string(&position, std::string_view("Wonder Stone Appeared!"));
            position.y += 16.0f;
            //ASCII_MANAGER_PTR->add_string(&position, std::string_view("Level "), stone->level);
            position.y -= 16.0f;

            //ASCII_MANAGER_PTR->set_scale(scale * 0.33f + 1.0f, scale * 0.33f + 1.0f);

            //ASCII_MANAGER_PTR->add_string(&position, std::string_view("Wonder Stone Appeared!"));
            position.y += 16.0f;
            //ASCII_MANAGER_PTR->add_string(&position, std::string_view("Level "), stone->level);
            position.y -= 16.0f;

            //ASCII_MANAGER_PTR->__reset_properties();
        }
    }

    // Rx113F70
    dllexport gnu_noinline int32_t thiscall __get_summon_color() {
        if (
            this->summon_active &&
            this->stone_list.get_head() // stone list not empty
        ) {
            return this->stone_list.get_head()->get_data()->color;
        }

        int32_t priority = -1;
        int32_t stone_type = 0;
        if (get_globals_side(0)->stone_priority_red > priority) {
            priority = get_globals_side(0)->stone_priority_red;
            stone_type = RedStone;
        }
        if (get_globals_side(0)->stone_priority_blue > priority) {
            priority = get_globals_side(0)->stone_priority_blue;
            stone_type = BlueStone;
        }
        if (get_globals_side(0)->stone_priority_yellow > priority) {
            priority = get_globals_side(0)->stone_priority_yellow;
            stone_type = YellowStone;
        }
        if (get_globals_side(0)->stone_priority_green > priority) {
            priority = get_globals_side(0)->stone_priority_green;
            stone_type = GreenStone;
        }

        return stone_type;
    }

    // Invoked from the __stone_enemy_spawn ECL instruction
    // Rx112F60
    dllexport gnu_noinline void thiscall allocate_new_stone_enemy(EnemyID parent_id, int32_t stone_type) {
        StoneEnemy* stone_enemy = MEM_ALLOC_LOG->allocate_new<StoneEnemy>();
        this->stone_list.append(&stone_enemy->list_node);
        stone_enemy->initialize(parent_id, stone_type);
    }
};

struct LaserData {
    // void* vftable; // 0x0
    alignment_bytes(0x4); // 0x4
    ZUNList<LaserData> list_node; // 0x8
    union {
        uint32_t flags; // 0x1C
        struct {

        };
    };
    int32_t state; // 0x20
    int32_t type; // 0x24
    Timer __timer_28; // 0x28
    Timer graze_timer; // 0x38
    Timer __timer_48; // 0x48
    Float3 position; // 0x58
    Float3 velocity; // 0x64
    ZUNAngle angle; // 0x70
    float length; // 0x74
    float width; // 0x78
    float speed; // 0x7C
    float __float_80; // 0x80
    float __float_84; // 0x84
    float __float_88; // 0x88
    int32_t id; // 0x8C
    union {
        std::array<BulletEffectData, 24> effects; // 0x90
    };
    int32_t effect_index; // 0x690
    alignment_bytes(0x4); // 0x694


    virtual void __method_0() {}
    virtual void __method_4() {}
    virtual void __method_8() {}
    virtual void __method_C() {}
    virtual void __method_10() {}
    virtual void __method_14() {}
    virtual void __method_18() {}
    virtual void __method_1C() {}
    virtual void __method_20() {}
    dllexport virtual int cleanup() {
        this->list_node.unlink();
        return 0;
    }
};

// size: 0x58
struct LaserManager : ZUNTask {
    //ZUNTask base; 0x0
    ZUNListIterable<LaserData> laser_list; // 0x10
    int32_t laser_count; // 0x28
    int32_t prev_laser_id; // 0x2C
    char float3_padding[0x18]; // 0x30
    void* bullet_anm; // 0x48
    int __dword_54; // 0x4C
    uint32_t side_index; // 0x50
    GameSide* game_side_ptr; // 0x54
    // 0x58
    
    /*
    dllexport int on_tick() {
        //ANM_MANAGER_PTR->set_game_side(this->side_index);
        for (auto laser_node : this->laser_list) {
            if (laser_node->get_data()->state != 1) {
                LaserData* laser = laser_node->get_data();
                laser->__method_20();
            }
        }
        return 1;
    }

    dllexport int on_draw() {
        //ANM_MANAGER_PTR->set_game_side(this->side_index);
        //ANM_MANAGER_PTR->__prep_draw_r4EE00(12, this->side_index);
        for (auto laser_node : this->laser_list) {
            if (laser_node->get_data()->state != 1) {
                LaserData* laser = laser_node->get_data();
                laser->__method_20();
            }
        }
        return 1;
    }
    */
};

// size: 0x38
struct StoneBase {
    //void* vftable; // 0x0
    int32_t stone_type; // 0x4
    int __dword_8; // 0x8
    int __dword_C; // 0xC
    int __dword_10; // 0x10
    Timer __timer_14; // 0x14
    Timer __timer_24; // 0x24
    uint8_t __byte_34; // 0x34
    uint8_t __byte_35; // 0x35
    padding_bytes(0x2); // 0x36
    // 0x38
};

// size: 0x38
struct StoneRedReimu : StoneBase {
    //StoneBase base; // 0x0
    // 0x38
};
// size: 0x38
struct StoneRedReimu2 : StoneBase {
    //StoneBase base; // 0x0
    // 0x38
};
// size: 0x38
struct StoneRedMarisa : StoneBase {
    //StoneBase base; // 0x0
    // 0x38
};
// size: 0x38
struct StoneRedMarisa2 : StoneBase {
    //StoneBase base; // 0x0
    // 0x38
};

// size: 0x38
struct StoneBlueReimu : StoneBase {
    //StoneBase base; // 0x0
    // 0x38
};
// size: 0x40
struct StoneBlueReimu2 : StoneBase {
    //StoneBase base; // 0x0
    int __dword_38; // 0x38
    float __float_3C; // 0x3C
    // 0x40
};
// size: 0x38
struct StoneBlueMarisa : StoneBase {
    //StoneBase base; // 0x0
    // 0x38
};
// size: 0x40
struct StoneBlueMarisa2 : StoneBase {
    //StoneBase base; // 0x0
    int __dword_38; // 0x38
    float __float_3C; // 0x3C
    // 0x40
};

// size: 0x48
struct StoneYellowReimu : StoneBase {
    //StoneBase base; // 0x0
    Timer __timer_38; // 0x38
    // 0x48
};
// size: 0x5C
struct StoneYellowReimu2 : StoneBase {
    //StoneBase base; // 0x0
    Timer __timer_38; // 0x38
    Timer __timer_48; // 0x48
    int __dword_58; // 0x58
    // 0x5C
};
// size: 0x48
struct StoneYellowMarisa : StoneBase {
    //StoneBase base; // 0x0
    Timer __timer_38; // 0x38
    // 0x48
};
// size: 0x5C
struct StoneYellowMarisa2 : StoneBase {
    //StoneBase base; // 0x0
    Timer __timer_38; // 0x38
    Timer __timer_48; // 0x48
    int __dword_58; // 0x58
    // 0x5C
};

// size: 0x5C
struct StoneGreenReimu : StoneBase {
    //StoneBase base; // 0x0
    int __dword_38; // 0x38
    Timer __timer_3C; // 0x3C
    Timer __timer_4C; // 0x4C
    // 0x5C
};
// size: 0x98
struct StoneGreenReimu2 : StoneBase {
    //StoneBase base; // 0x0
    int __dword_38; // 0x38
    Timer __timer_3C; // 0x3C
    Timer __timer_4C; // 0x3C
    Float3 __float3_5C; // 0x5C
    float __float_68; // 0x68
    ZUNInterp<float> __float_interp_6C; // 0x6C
    // 0x98
};
// size: 0x5C
struct StoneGreenMarisa : StoneBase {
    //StoneBase base; // 0x0
    int __dword_38; // 0x38
    Timer __timer_3C; // 0x3C
    Timer __timer_4C; // 0x4C
    // 0x5C
};
// size: 0x98
struct StoneGreenMarisa2 : StoneBase {
    //StoneBase base; // 0x0
    int __dword_38; // 0x38
    Timer __timer_3C; // 0x3C
    Timer __timer_4C; // 0x4C
    float __float_5C; // 0x5C
    ZUNInterp<float> __float_interp_60; // 0x60
    Float3 __float3_8C; // 0x8C
    // 0x98
};

// size: 0x84
struct PlayerStoneManager : ZUNTask {
    //ZUNTask base; // 0x0
    Timer __timer_10; // 0x10
    AnmID __anm_id_20; // 0x20
    int __dword_24; // 0x24
    StoneBase* story_stone; // 0x28
    StoneBase* narrow_shot_stone; // 0x2C
    StoneBase* wide_shot_stone; // 0x30
    StoneBase* assist_stone; // 0x34
    int __dword_38; // 0x38
    int __dword_3C; // 0x3C
    void* __ptr_40; // 0x40
    Timer __timer_44; // 0x44
    int __hyper_state; // 0x54
    Float3 __float3_58; // 0x58
    Timer __timer_64; // 0x64
    float __float_74; // 0x74
    uint8_t __byte_78; // 0x78
    padding_bytes(3); // 0x79
    uint32_t side_index; // 0x7C
    GameSide* game_side_ptr; // 0x80
    // 0x84
};

// size: 0xC8
struct Spellcard : ZUNTask {
    //ZUNTask base; // 0x0
    AnmID __vm_id_10; // 0x10
    AnmID __vm_id_14; // 0x14
    AnmID __vm_id_18; // 0x18
    AnmID __vm_id_1C; // 0x1C
    AnmID __vm_id_20; // 0x20
    Timer __timer_24; // 0x24
    char name[64]; // 0x34
    int32_t id; // 0x74
    union {
        uint32_t flags; // 0x78
        struct {
            uint32_t spell_active : 1; // 1
            uint32_t __unknown_flag_B : 1; // 2
            uint32_t : 1; // 3
            uint32_t __timeout_spell : 1; // 4
            uint32_t __unknown_flag_C : 1; // 5
            uint32_t __unknown_flag_E : 1; // 6
            uint32_t __unknown_flag_F : 1; // 7
            uint32_t __unknown_flag_D : 1; // 8
            uint32_t __unknown_flag_H: 1; // 9
            uint32_t __unknown_flag_G : 1; // 10
        };
    };
    int32_t __bonus_A; // 0x7C
    int32_t __bonus_B; // 0x80
    int32_t time; // 0x84
    int __dword_88; // 0x88
    int __int_8C; // 0x8C
    int __dword_90; // 0x90
    padding_bytes(4); // 0x94
    double __double_98; // 0x98
    double __double_A0; // 0xA0
    int __dword_A8; // 0xA8
    Float3 __float3_AC; // 0xAC
    int __dword_B8; // 0xB8
    uint32_t side_index; // 0xBC
    GameSide* game_side_ptr; // 0xC0
    padding_bytes(4); // 0xC4
    // 0xC8
    
    // Rx87C30
    void thiscall start_spellstart_spell(int32_t id, const char* name, int32_t time, int32_t mode) {
        //this->__timer_24 = 0;
        this->id = id;
        strcpy_s(this->name, countof(this->name), name);
        this->spell_active = true;
        this->__unknown_flag_B = true;
        this->__timeout_spell = false;
        this->__unknown_flag_C = false;
        this->__unknown_flag_G = true;
        //this->__clear_flag_D();
        
        // Scorefile/gui stuff
        
        this->__unknown_flag_E = false;
        if (
            //get_bomb_manager(0)->__bomb_is_active()
            1
        ) {
            this->__unknown_flag_E = true;
        }
        this->__int_8C = 1;
        this->__unknown_flag_F = false;
        
        // TODO
        
        if (
            //this->game_side_ptr->enemy_manager_ptr->get_boss_by_index(0)
            1
        ) {
            //Enemy* boss = this->game_side_ptr->enemy_manager_ptr->get_boss_by_index(0);
            //this->__float3_AC = boss->get_current_motion()->position;
        }
        else {
            Float3 zero = { 0.0f, 0.0f, 0.0f };
            this->__float3_AC = zero;
        }
        //this->__vm_id_20.set_controller_position(&this->__float3_AC);
        
        //this->__vm_id_1C.__wtf_child_list_jank_A(4, 0)->data.current_context.int_vars[2] = time;
        //this->__vm_id_1C.__wtf_child_list_jank_A(5, 0)->data.current_context.int_vars[2] = time;
        
        this->time = time;

        // ZUN didn't mark this as const,
        // so it gets written onto the
        // stack for no good reason.
        int32_t bonus_table[] = {
            500000,
            1000000,
            1500000,
            2000000,
            1000000
        };
        
        //int32_t bonus = bonus_table[GAME_MANAGER.get_difficulty()] * GAME_MANAGER.get_current_stage();
        //this->__bonus_A = bonus;
        //this->__bonus_B = bonus;
        if (this->__bonus_B >= 1000000000) {
            this->__bonus_B = 1000000000;
        }
    }
};


gnu_noinline float vectorcall original_float(float value) {
#pragma clang fp exceptions(strict)
    //value *= 2.0f;
    value += value;
    if (value < 1.0f) {
        return pow_2(value) * 0.5f;
    } else {
        return (2.0f - pow_2(2.0f - value)) * 0.5f;
    }
}

gnu_noinline float vectorcall test_float(float value, float valueR) {
    if (value < 0.5f) {
#pragma clang fp exceptions(strict)
        return pow_2(value + value) * 0.5f;
    } else {
        value = pow_2(valueR);
        value += value;
        value = 1.0f - value;
    }
    return value;
}

gnu_noinline float vectorcall test_float2(float value, float valueR) {
    vec<float, 2> temp = { valueR, value };
    temp += temp;
    temp *= temp;
    float high = temp[0];
    float low = temp[1];
    high = 2.0f - high;
    float ret = value < 0.5f ? low : high;
    return ret * 0.5f;
}

int main() {

    const uint32_t min = bitcast<uint32_t>(0.0f);
    const uint32_t max = bitcast<uint32_t>(1.0f);

    size_t matching = 0;
    size_t total = 0;
    for (
        uint32_t x = min;
        x <= max;
        x
    ) {
        ++total;
        float y = bitcast<float>(x);
        float original_val = original_float(y);
        float test_val = test_float(y, 1.0f - y);
        bool match = original_val == test_val;
        matching += match;
        if (!match) {
            //__asm int3
        }
        ++x;
    }

    printf(
        "FAILS: %zu\n"
        "%zu/%zu matching (%f%%)"
        , total - matching
        , matching, total, ((float)matching / (float)total) * 100.0f
    );

    return 0;
}