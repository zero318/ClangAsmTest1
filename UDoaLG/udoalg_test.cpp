#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#pragma clang diagnostic ignored "-Winvalid-source-encoding"

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdint.h>
#include <direct.h>
#include <type_traits>

#include <string.h>

#define _USE_MATH_DEFINES 1
#include <math.h>

#if __INTELLISENSE__
#undef _HAS_CXX17
#define _HAS_CXX20 1
#endif
#include <bit>

#if __INTELLISENSE__
#undef _HAS_CXX17
#define _HAS_CXX17 1
#undef _HAS_CXX20
#define _HAS_CXX20 0
#endif
#include <mutex>

#include "../zero/FloatConstants.h"

#include "../zero/util.h"

template <typename T>
using ZUNList = ZUNListBase<T, true>;
template <typename T>
using ZUNListHead = ZUNListHeadBase<T, true>;

#define GAME_VERSION UDoALG_VER

#define ENGLISH_STRINGS

#ifndef ENGLISH_STRINGS
#define JpEnStr(jstring, estring) jstring
#else
#define JpEnStr(jstring, estring) estring
#endif

//#define USE_VOLATILES_AND_BARRIERS_FOR_ORIGINAL_CODEGEN 1

#if USE_VOLATILES_AND_BARRIERS_FOR_ORIGINAL_CODEGEN
#define codegen_volatile volatile
#define codegen_barrier() _ReadWriteBarrier()
#else
#define codegen_volatile
#define codegen_barrier() do {} while(false)
#endif


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


#include "../zero/zun.h"

//#pragma comment (lib, "wbemuuid.lib")


#ifndef SAFE_FREE
#define SAFE_FREE(p)       { if (p) { free ((void*)p);     (p)=NULL; } }
#endif

#ifndef SAFE_FREE_FAST
#define SAFE_FREE_FAST(p)       { if (auto ptr = (p)) { free ((void*)ptr);     (p)=NULL; } }
#endif


dllexport gnu_noinline void* cdecl memset_force(void* dst, int val, size_t size) {
    gnu_attr(musttail) return memset(dst, val, size);
}

#define zero_this() memset_force(this, 0, sizeof(*this));

#define UNUSED_DWORD (GARBAGE_ARG(int32_t))
#define UNUSED_FLOAT (GARBAGE_ARG(float))

typedef struct AnmManager AnmManager;

#define ANM_FULL_ID_BITS (32)
#define ANM_FAST_ID_BITS (15)
#define ANM_SLOW_ID_BITS (ANM_FULL_ID_BITS - ANM_FAST_ID_BITS)
#define INVALID_FAST_ID uint_width_max(ANM_FAST_ID_BITS)



// size: 0x4
union AnmID {
    uint32_t full;
    struct {
        uint32_t fast_id : ANM_FAST_ID_BITS;
        uint32_t slow_id : ANM_SLOW_ID_BITS;
    };

    inline AnmID() : full(0) {}

    inline AnmID(AnmID& id) : full(id.full) {}

    inline constexpr operator uint32_t() const {
        return this->full;
    }
    template<typename T> requires(std::is_integral_v<T>)
       inline constexpr AnmID& operator=(const T& raw) {
        this->full = raw;
        return *this;
    }
};

// size: 0x634
struct AnmVM {
    // size: 0x4D8
    struct AnmVMData {
        dummy_fields(0x18);
        uint32_t layer; // 0x18
        dummy_fields(0x4BC);
    };
    ValidateStructSize(0x4D8, AnmVMData);

    // size: 0x15C
    struct AnmVMController {
        AnmID id = {}; // 0x0, 0x4D8
        uint32_t fast_id; // 0x4, 0x4DC
        dummy_fields(0x2C); // 0x8, 0x4E0
        ZUNList<AnmVM> global_list_node; // 0x34
        ZUNList<AnmVM> __draw_list_node; // 0x44
        ZUNList<AnmVM> child_list_node; // 0x54
        ZUNList<AnmVM> child_list_head; // 0x64
        ZUNList<AnmVM> destroy_list_node; // 0x74
        AnmVM* next_in_layer; // 0x84
        AnmVM* prev_in_layer; // 0x88
        AnmVM* root_vm; // 0x8C
        AnmVM* parent; // 0x90
        float slowdown; // 0x94
        void* special_data; // 0x98
        uint32_t special_data_size; // 0x9C
        uint32_t __vm_state; // 0xA0
        dummy_fields(0xB8);
    };
    ValidateStructSize(0x15C, AnmVMController);

    AnmVMData data; // 0x0
    AnmVMController controller; // 0x4D8
    // 0x634
};
ValidateStructSize(0x634, AnmVM);

// size: 0x64C
struct AnmFastVM : AnmVM {
    ZUNList<AnmFastVM> fast_node; // 0x634
    bool alive; // 0x644
    padding_bytes(0x3); // 0x645
    uint32_t fast_id; // 0x648
    // 0x64C
};
ValidateStructSize(0x64C, AnmFastVM);

// Rx22AEC8
static std::mutex ANM_LIST_MUTEX;

struct AnmManager {
    // size: 0x20
    struct AnmListEnds {
        // size: 0x10
        struct AnmListEnd {
            ZUNList<AnmVM>* __ptr_0; // 0x0
            ZUNList<AnmVM>* __ptr_4; // 0x4
            ZUNList<AnmVM>* __ptr_8; // 0x8
            ZUNList<AnmVM>* __ptr_C; // 0xC
            // 0x10
        };
        ValidateStructSize(0x10, AnmListEnd);

        AnmListEnd head; // 0x0
        AnmListEnd tail; // 0x10
        // 0x20
    };
    ValidateStructSize(0x20, AnmListEnds);

    unknown_fields(0x1C); // 0x0
    int __dword_1C; // 0x1C
    int __dword_20; // 0x20
    dummy_fields(0xA0); // 0x24
    int __int_C4; // 0xC4
    int __dword_C8; // 0xC8
    int __int_CC; // 0xCC
    int __int_D0; // 0xD0
    Int2 __int2_D4; // 0xD4
    Float2 __float2_DC; // 0xDC
    uint32_t __counter_E4; // 0xE4
    AnmVM root_vm; // 0xE8
    int __int_71C; // 0x71C
    int32_t __current_side; // 0x720
    union {
        AnmListEnds lists[2]; // 0x724
        struct {
            AnmListEnds world_list; // 0x724
            AnmListEnds ui_list; // 0x744
        };
    };
    AnmFastVM fast_array[uint_width_max(ANM_FAST_ID_BITS)]; // 0x764
    AnmFastVM fast_array_end;
    int32_t next_snapshot_fast_id; // 0x3260764
    int32_t next_snapshot_discriminator; // 0x3260768
    ZUNList<AnmVM> snapshot_list_head; // 0x326076C
    ZUNList<AnmVM> free_list_head; // 0x326077C
    int __dword_326078C; // 0x326078C


    // RxBE0F0
    dllexport AnmVM* get_vm_with_id(AnmID id) {
        if (id) {
            if (id.fast_id == INVALID_FAST_ID) {
                for (size_t i = 0; i < countof(this->lists); ++i) {
                    for (
                        ZUNList<AnmVM>* node = this->lists[i].head.__ptr_0;
                        node;
                        node = node->next
                    ) {
                        AnmVM* vm = node->data;
                        if (vm->controller.id == id) {
                            return vm;
                        }
                    }
                    for (
                        ZUNList<AnmVM>* node = this->lists[i].head.__ptr_8;
                        node;
                        node = node->next
                    ) {
                        AnmVM* vm = node->data;
                        if (vm->controller.id == id) {
                            return vm;
                        }
                    }
                }
            }
            else if (this->fast_array[id].alive) {
                AnmFastVM* vm = &this->fast_array[id];
                if (vm->controller.id == id) {
                    return vm;
                }
            }
        }
        return NULL;
    }

    dllexport AnmVM* get_vm_with_id_new(const AnmID id) const {
        if (id) {
            if (expect(id.fast_id == INVALID_FAST_ID, false)) {
                for (size_t i = 0; i < countof(this->lists); ++i) {
                    for (
                        ZUNList<AnmVM>* node = this->lists[i].head.__ptr_0;
                        node;
                        node = node->next
                    ) {
                        AnmVM* vm = node->data;
                        if (vm->controller.id == id) {
                            return vm;
                        }
                    }
                    for (
                        ZUNList<AnmVM>* node = this->lists[i].head.__ptr_8;
                        node;
                        node = node->next
                    ) {
                        AnmVM* vm = node->data;
                        if (vm->controller.id == id) {
                            return vm;
                        }
                    }
                }
            }
            else if (this->fast_array[id].alive) {
                const AnmFastVM* vm = &this->fast_array[id];
                if (vm->controller.id == id) {
                    return (AnmVM*)vm;
                }
            }
        }
        return NULL;
    }
};

struct AnmManagerC {
    // size: 0x20
    struct AnmListEnds {
        // size: 0x10
        struct AnmListEnd {
            ZUNList<AnmVM>* __ptr_0; // 0x0
            ZUNList<AnmVM>* __ptr_4; // 0x4
            ZUNList<AnmVM>* __ptr_8; // 0x8
            ZUNList<AnmVM>* __ptr_C; // 0xC
            // 0x10
        };
        ValidateStructSize(0x10, AnmListEnd);

        AnmListEnd head; // 0x0
        AnmListEnd tail; // 0x10
        // 0x20
    };
    ValidateStructSize(0x20, AnmListEnds);

    unknown_fields(0xC); // 0x0
    int __dword_1C; // 0x1C
    int __dword_20; // 0x20
    dummy_fields(0xA0); // 0x24
    int __int_C4; // 0xC4
    int __dword_C8; // 0xC8
    int __int_CC; // 0xCC
    int __int_D0; // 0xD0
    Int2 __int2_D4; // 0xD4
    Float2 __float2_DC; // 0xDC
    uint32_t __counter_E4; // 0xE4
    AnmVM root_vm; // 0xE8
    int __int_71C; // 0x71C
    int32_t __current_side; // 0x720
    union {
        AnmListEnds lists[2]; // 0x724
        struct {
            AnmListEnds world_list; // 0x724
            AnmListEnds ui_list; // 0x744
        };
    };
    AnmFastVM fast_array[uint_width_max(ANM_FAST_ID_BITS)]; // 0x764
    AnmFastVM fast_array_end;
    int32_t next_snapshot_fast_id; // 0x3260764
    int32_t next_snapshot_discriminator; // 0x3260768
    ZUNList<AnmVM> snapshot_list_head; // 0x326076C
    ZUNList<AnmVM> free_list_head; // 0x326077C
    int __dword_326078C; // 0x326078C

    // This looks like it's just D3D config, hopefully it can be called less
    dllexport gnu_noinline void thiscall __sub_rCC030(int32_t layer_index, size_t list_index) {
        use_var(layer_index);
        use_var(list_index);
        volatile float dumb = tan(rand());
    }

    dllexport gnu_noinline void thiscall draw_vm(AnmVM* vm) {
        use_var(vm);
        volatile float dumb = tan(rand());
    }

    dllexport int thiscall render_layer(int32_t layer_index) {
        std::scoped_lock<std::mutex> lock(ANM_LIST_MUTEX);

        for (size_t i = 0; i < countof(this->lists); ++i) {

            this->lists[i].tail = {
                (ZUNList<AnmVM>*)&this->lists[i].head,
                NULL,
                NULL,
                NULL
            };

            ZUNList<AnmVM>* tail_node = (ZUNList<AnmVM>*)&this->lists[i].tail;

            ZUNList<AnmVM>* cur_node = this->lists[i].head.__ptr_0;

            bool found_data = false;

            while (cur_node) {
                const uint32_t* vm_state_ptr = based_pointer<uint32_t>(cur_node, 0x6C);
                if (!*vm_state_ptr) {
                    AnmVM* vm_ptr = based_pointer<AnmVM>(cur_node, -0x50C);
                    //uint32_t layer = *based_pointer<uint32_t>(cur_node, -0x4F4);
                    uint32_t layer = vm_ptr->data.layer;
                    switch (layer) {
                        case 0x2B:
                            layer = 0x1A;
                            break;
                        case 0x2C ... 0x33:
                            layer -= 15;
                            break;
                    }
                    if (expect_chance(layer == layer_index, false, 0.75)) {
                        found_data = true;

                        //vm_ptr->controller.__draw_list_node.data = vm_ptr;
                        //vm_ptr->controller.__draw_list_node.idk = NULL;

                        ZUNList<AnmVM>* tail_next = tail_node->next;
                        vm_ptr->controller.__draw_list_node.next = tail_next;
                        vm_ptr->controller.__draw_list_node.prev = tail_node;
                        ZUNList<AnmVM>* draw_node = cur_node + 1;
                        tail_node->next = draw_node;
                        if (tail_next) {
                            tail_next->prev = draw_node;
                        }
                        tail_node = draw_node;
                    }
                }

                cur_node = cur_node->next;
            }
        }
    }

    dllexport int render_layer_new(int32_t layer_index) {
        std::scoped_lock<std::mutex> lock(ANM_LIST_MUTEX);

        for (uint8_t i = 0; i < countof(this->lists); ++i) {

            ZUNList<AnmVM>* cur_node = this->lists[i].head.__ptr_0;

            uint8_t d3d_unconfigured = true;

            while (expect(cur_node != NULL, true)) {
                const uint32_t* vm_state_ptr = based_pointer<uint32_t>(cur_node, 0x6C);
                if (!*vm_state_ptr) {
                    uint32_t layer = *based_pointer<uint32_t>(cur_node, -0x4F4);
                    switch (layer) {
                        case 0x2B:
                            layer = 0x1A;
                            break;
                        case 0x2C ... 0x33:
                            layer -= 15;
                            break;
                    }
                    if (expect_chance(layer == layer_index, false, 0.75)) {
                        if (expect(!!d3d_unconfigured, false)) {
                            d3d_unconfigured = false;
                            this->__sub_rCC030(layer_index, i);
                        }
                        AnmVM* vm_ptr = based_pointer<AnmVM>(cur_node, -0x50C);
                        this->draw_vm(vm_ptr);
                        //++this->__counter_E4;
                    }
                }
                cur_node = cur_node->next;
            }

            cur_node = this->lists[i].head.__ptr_8;

            while (expect(cur_node != NULL, true)) {
                const uint32_t* vm_state_ptr = based_pointer<uint32_t>(cur_node, 0x6C);
                if (!*vm_state_ptr) {
                    uint32_t layer = *based_pointer<uint32_t>(cur_node, -0x4F4);
                    switch (layer) {
                        case 0x1A:
                            layer = 0x2B;
                            break;
                        case 0x1D ... 0x24:
                            layer += 15;
                            break;
                        default:
                            layer = 0x2D;
                            break;
                    }
                    if (expect_chance(layer == layer_index, false, 0.75)) {
                        if (expect(!!d3d_unconfigured, false)) {
                            d3d_unconfigured = false;
                            this->__sub_rCC030(layer_index, i);
                        }
                        AnmVM* vm_ptr = based_pointer<AnmVM>(cur_node, -0x50C);
                        this->draw_vm(vm_ptr);
                        //++this->__counter_E4;
                    }
                }
                cur_node = cur_node->next;
            }
        }

        return 1;
    }

    dllexport int render_layer_new2(int32_t layer_index) {
        std::scoped_lock<std::mutex> lock(ANM_LIST_MUTEX);

        for (uint8_t i = 0; i < countof(this->lists); ++i) {

            ZUNList<AnmVM>* cur_node = this->lists[i].head.__ptr_0;

            uint8_t d3d_unconfigured = true;

            while (expect(cur_node != NULL, true)) {
                if (!*based_pointer<uint32_t>(cur_node, 0x6C)) {
                    uint32_t layer = *based_pointer<uint32_t>(cur_node, -0x4F4);
                    switch (layer) {
                        case 0x2B:
                            layer = 0x1A;
                            break;
                        case 0x2C ... 0x33:
                            layer -= 15;
                            break;
                    }
                    if (expect_chance(layer == layer_index, false, 0.75)) {
                        if (expect(!!d3d_unconfigured, false)) {
                            d3d_unconfigured = false;
                            this->__sub_rCC030(layer_index, i);
                        }
                        this->draw_vm(based_pointer<AnmVM>(cur_node, -0x50C));
                    }
                }
                cur_node = cur_node->next;
            }

            cur_node = this->lists[i].head.__ptr_8;

            while (expect(cur_node != NULL, true)) {
                if (!*based_pointer<uint32_t>(cur_node, 0x6C)) {
                    uint32_t layer = *based_pointer<uint32_t>(cur_node, -0x4F4);
                    switch (layer) {
                        case 0x1A:
                            layer = 0x2B;
                            break;
                        case 0x1D ... 0x24:
                            layer += 15;
                            break;
                        default:
                            layer = 0x2D;
                            break;
                    }
                    if (expect_chance(layer == layer_index, false, 0.75)) {
                        if (expect(!!d3d_unconfigured, false)) {
                            d3d_unconfigured = false;
                            this->__sub_rCC030(layer_index, i);
                        }
                        this->draw_vm(based_pointer<AnmVM>(cur_node, -0x50C));
                    }
                }
                cur_node = cur_node->next;
            }
        }

        return 1;
    }
};

dllexport gnu_noinline long double vectorcall zsinB(double value) {
    use_var(value);
    return tan(rand());
}

dllexport float vectorcall zsinfB(float value) {
    return zsinB(value);
}

struct Player {
    char gap0[8420]; // 0x0
    int32_t unfocused_linear_speed; // 0x20E4
    int32_t focused_linear_speed; // 0x20E8
    int32_t unfocused_diagonal_speed; // 0x20EC
    int32_t focused_diagonal_speed; // 0x20F0
};

// size: 0x3C
struct CpuCollider {
    Float3 position; // 0x0
    Float3 __float3_C; // 0xC
    Float2 __hitbox_size; // 0x18
    float __hitbox_radius; // 0x20
    float __angle_24; // 0x24
    float __angle_28; // 0x28
    float __float_2C; // 0x2C
    int32_t __life; // 0x30
    void* __entity_ptr; // 0x34
    union {
        uint32_t flags; // 0x38
        struct {
            uint32_t round : 1;
        };
    };
    // 0x3C
};

struct GameSide;

// size: 0xEA64C
struct CpuHit {
    void* vftable; // 0x0
    CpuCollider __collider_array_4[8000]; // 0x4
    CpuCollider __collider_array_75304[8000]; // 0x75304
    int32_t collider_count; // 0xEA604
    Float3 position; // 0xEA608
    int __int_EA614; // 0xEA614
    int32_t __int_EA618; // 0xEA618
    int32_t __int_EA61C; // 0xEA61C
    int32_t __int_EA620; // 0xEA620
    int32_t __int_EA624; // 0xEA624
    int __int_EA628; // 0xEA628
    int __dword_EA62C; // 0xEA62C
    int __int_EA630; // 0xEA630
    int __int_EA634; // 0xEA634
    int __int_EA638; // 0xEA638
    int __int_EA63C; // 0xEA63C
    int __int_EA640; // 0xEA640
    uint32_t side_index; // 0xEA644
    GameSide* game_side_ptr; // 0xEA648
    // 0xEA64C
};

// size: 0x36EECC
struct CpuHitC {
    void* vftable; // 0x0
    CpuCollider __collider_array_4[30000]; // 0x4
    CpuCollider __collider_array_1B7744[30000]; // 0x1B7744
    int32_t collider_count; // 0x36EE84
    Float3 position; // 0x36EE88
    int __int_36EE94; // 0x36EE94
    int32_t __int_36EE98; // 0x36EE98
    int32_t __int_36EE9C; // 0x36EE9C
    int32_t __int_36EEA0; // 0x36EEA0
    int32_t __int_36EEA4; // 0x36EEA4
    int __int_36EEA8; // 0x36EEA8
    int __dword_36EEAC; // 0x36EEAC
    int __int_36EEB0; // 0x36EEB0
    int __int_36EEB4; // 0x36EEB4
    int __int_36EEB8; // 0x36EEB8
    int __int_36EEBC; // 0x36EEBC
    int __int_36EEC0; // 0x36EEC0
    uint32_t side_index; // 0x36EEC4
    GameSide* game_side_ptr; // 0x36EEC8
    // 0x36EECC

    dllexport uint32_t* vectorcall __check_colliders_new(int, uint32_t *volatile hit_flags_ptr, float, float, float radius);
};

// size: 0x3C
struct GameSide {
    void* bullet_manager_ptr; // 0x0
    Player* player_ptr; // 0x4
    void* enemy_manager_ptr; // 0x8
    void* item_manager_ptr; // 0xC
    void* spellcard_ptr; // 0x10
    void* laser_manager_ptr; // 0x14
    void* gauge_manager_ptr; // 0x18
    void* bomb_manager_ptr; // 0x1C
    void* ability_manager_ptr; // 0x20
    void* popup_manager_ptr; // 0x24
    void* effect_manager_ptr; // 0x28
    void* globals_side_ptr; // 0x2C
    void* shot_manager_ptr; // 0x30
    void* __cpu_hit_ptr; // 0x34
    void* ex_attack_manager_ptr; // 0x38
    // 0x3C
};
ValidateStructSize(0x3C, GameSide);

dllexport GameSide GAME_SIDES[2];

extern "C" {
    extern vec<float, 4> vectorcall __libm_sse2_sincosf(float angle) asm("___libm_sse2_sincosf_");
}

dllexport uint32_t* vectorcall CpuHitC::__check_colliders_new(int, uint32_t *volatile hit_flags_ptr, float, float, float radius) {

    uint32_t hit_flags;
    __asm__(
        "xor %0, %0"
        : "=r"(hit_flags)
    );

    size_t collider_count = this->collider_count;
    if (collider_count) {

        CpuCollider* collider = &this->__collider_array_4[0];
        __asm__("":"+r"(collider));

        vec<int32_t, 4> speed_vec_int = *(vec<int32_t, 4>*)&this->game_side_ptr->player_ptr->unfocused_linear_speed; // { UL, FL, UD, FD }
        vec<float, 4> speed_vec = convertvec(speed_vec_int, vec<float, 4>);
        speed_vec *= 1.0f / 128.0f;

        vec<float, 4> neg_speed_vec = -speed_vec;

        //vec<float, 2> current_pos = *(vec<float, 2>*)&this->position;
        vec<float, 4> current_pos_wide = { this->position.x, this->position.y, 0.0f, 0.0f };

        constexpr vec<float, 4> min_clamp_wide = { -140.0f, 32.0f, -140.0f, 32.0f };
        constexpr vec<float, 4> max_clamp_wide = { 140.0f, 432.0f, 140.0f, 432.0f };
#define clamp(value) __builtin_elementwise_max(__builtin_elementwise_min(value, max_clamp_wide), min_clamp_wide)

        union {
            struct {
                vec<float, 2> check_pos_list[18];
                volatile vec<float, 4> radius_squared_vec_mem;
                volatile vec<float, 4> radius_vec_mem;
            };
        };

        check_pos_list[0] = *(vec<float, 2>*)&current_pos_wide;
        check_pos_list[9] = *(vec<float, 2>*)&current_pos_wide;

        //vec<float, 4> current_pos_wide = { current_pos[0], current_pos[1], current_pos[0], current_pos[1] };
        current_pos_wide = shufflevec(current_pos_wide, current_pos_wide, 0, 1, 0, 1);

        *(vec<float, 4>*)&check_pos_list[5] = clamp(current_pos_wide + shufflevec(neg_speed_vec, speed_vec, 3, 3, 6, 3)); // { -UD, -UD, +UD, -UD }
        *(vec<float, 4>*)&check_pos_list[7] = clamp(current_pos_wide + shufflevec(neg_speed_vec, speed_vec, 3, 6, 6, 6)); // { -UD, +UD, +UD, +UD }
        *(vec<float, 4>*)&check_pos_list[14] = clamp(current_pos_wide + shufflevec(neg_speed_vec, speed_vec, 4, 4, 7, 4)); // { -FD, -FD, +FD, -FD }
        *(vec<float, 4>*)&check_pos_list[16] = clamp(current_pos_wide + shufflevec(neg_speed_vec, speed_vec, 4, 7, 7, 7)); // { -FD, +FD, +FD, +FD }

        current_pos_wide = shufflevec(current_pos_wide, current_pos_wide, 0, 0, 1, 1); // { X, X, Y, Y }
        vec<float, 4> tempA = shufflevec(current_pos_wide, current_pos_wide, 2, 3, 0, 1); // { Y, Y, X, X }

        vec<float, 4> linear_speed = shufflevec(neg_speed_vec, speed_vec, 0, 4, 0, 4); // { -UL, +UL, -UL, +UL }
        vec<float, 4> tempB = clamp(tempA + linear_speed);
        *(vec<float, 4>*)&check_pos_list[1] = shufflevec(current_pos_wide, tempB, 0, 4, 1, 5);
        *(vec<float, 4>*)&check_pos_list[3] = shufflevec(tempB, current_pos_wide, 2, 6, 3, 7);

        linear_speed = shufflevec(neg_speed_vec, speed_vec, 1, 5, 1, 5); // { -FL, +FL, -FL, +FL }
        tempB = clamp(tempA + linear_speed);
        *(vec<float, 4>*)&check_pos_list[10] = shufflevec(current_pos_wide, tempB, 0, 4, 1, 5);
        *(vec<float, 4>*)&check_pos_list[12] = shufflevec(tempB, current_pos_wide, 2, 6, 3, 7);

        //vec<float, 4> zero_vec = {};
        //radius_vec = shufflevec(radius_vec, zero_vec, 0, 4, 4, 0);
        //vec<float, 4> radius_squared_vec = shufflevec(radius_vec, radius_vec, 0, 0, 0, 0);
        //radius_squared_vec *= radius_squared_vec;
        vec<float, 4> radius_vec = { radius, 0.0f, 0.0f, radius };
        //volatile vec<float, 4> radius_vec_mem = radius_vec;
        radius_vec_mem = radius_vec;
        float radius_squared = radius * radius;
        vec<float, 4> radius_squared_vec = { radius_squared, radius_squared, radius_squared, radius_squared };
        //volatile vec<float, 4> radius_squared_vec_mem = radius_squared_vec;
        radius_squared_vec_mem = radius_squared_vec;

        CpuCollider* collider_end = collider + collider_count;
        do {
            if (collider->round) {
                /*
                float total_dist = collider->__hitbox_radius;
                total_dist *= total_dist;
                total_dist += radius_squared_vec[0];
                vec<float, 2> position = *(vec<float, 2>*)&collider->position;
                nounroll for (size_t j = 0, mask = 1; j != 18; ++j, mask <<= 1) {
                    if (!(hit_flags & mask)) {
                        vec<float, 2> diff = check_pos_list[j] - position;
                        diff *= diff;
                        if (total_dist > diff[0] + diff[1]) {
                            hit_flags |= mask;
                        }
                    }
                }
                */
                
                float distance = collider->__hitbox_radius;
                distance *= distance;
                distance += radius_squared_vec[0];

                vec<float, 4> distance_vec = { distance, distance, distance, distance };

                vec<float, 2> position = *(vec<float, 2>*)&collider->position;
                vec<float, 4> position_wide = { position[0], position[1], position[0], position[1] };

                nounroll for (size_t i = 0; i != 16; i += 4) {
                    vec<float, 4> diffA = *(vec<float, 4>*)&check_pos_list[i] - position_wide;
                    diffA *= diffA;
                    diffA += shufflevec(diffA, diffA, 1, -1, 3, -1);
                    vec<float, 4> diffB = *(vec<float, 4>*)&check_pos_list[i + 2] - position_wide;
                    diffB *= diffB;
                    diffB += shufflevec(diffB, diffB, -1, 0, -1, 2);
                    hit_flags |= vec_movmsk(distance_vec > shufflevec(diffA, diffB, 0, 2, 5, 7)) << i;
                }
                vec<float, 4> diffC = *(volatile vec<float, 4>*)&check_pos_list[16] - position_wide;
                diffC *= diffC;
                diffC += shufflevec(diffC, diffC, -1, 0, -1, 2);
                hit_flags |= vec_movmsk((vec<double, 2>)(distance_vec > diffC)) << 16;
                
            }
            else {
                vec<float, 4> trig = __libm_sse2_sincosf(-collider->__angle_28);
                radius_squared_vec = radius_squared_vec_mem;
                trig = shufflevec(trig, trig, 1, 1, 0, 0);
                trig = vec_xor(trig, 0.0f, 0.0f, -0.0f, 0.0f);
                vec<float, 2> half_size = *(vec<float, 2>*)&collider->__hitbox_size * 0.5f;
                vec<float, 4> half_size_wideA = { half_size[0], half_size[1], half_size[0], half_size[1] };
                vec<float, 4> half_size_wideB = vec_xor(half_size_wideA, -0.0f, 0.0f, -0.0f, 0.0f);
                vec<float, 2> position = *(vec<float, 2>*)&collider->position;
                vec<float, 4> tempC = half_size_wideA;
                tempC += radius_vec_mem;
                nounroll for (size_t i = 0, mask = 1; i != 18; ++i, mask <<= 1) {
                    if (!(hit_flags & mask)) {
                        vec<float, 2> diff = check_pos_list[i] - position;
                        vec<float, 4> diff_wide = { diff[0], diff[1], diff[1], diff[0] };

                        vec<float, 4> rot = diff_wide * trig;
                        rot = shufflevec(rot, rot, 0, 1, 0, 1) + shufflevec(rot, rot, 2, 3, 2, 3);

                        vec<float, 4> abs_rot = __builtin_elementwise_abs(rot);

                        int vec_mask = ~vec_movmsk(tempC >= abs_rot);
                        __asm__("":"+a"(vec_mask));
                        if (!(vec_mask & 0x3) || !(vec_mask & 0xC)) {
                            hit_flags |= mask;
                            continue;
                        }

                        rot += half_size_wideB;
                        rot *= rot;
                        rot += shufflevec(rot, rot, 2, 3, 1, 0);
                        auto tempE = radius_squared_vec > rot;
                        if (tempE[0] | tempE[1] | tempE[2] | tempE[3]) {
                            hit_flags |= mask;
                        }

                        /*
                        float x_sq1 = rot_x - half_size_x;
                        x_sq1 *= x_sq1;
                        float x_sq2 = rot_x + half_size_x;
                        x_sq2 *= x_sq2;
                        float y_sq1 = rot_y - half_size_y;
                        y_sq1 *= y_sq1;
                        float y_sq2 = rot_y + half_size_y;
                        y_sq2 *= y_sq2;
                        if (
                            radius_squared_vec[0] > x_sq1 + y_sq1 || // 0 + 2   0 + 2
                            radius_squared_vec[0] > x_sq2 + y_sq1 || // 1 + 2   1 + 3
                            radius_squared_vec[0] > x_sq1 + y_sq2 || // 0 + 3   2 + 1
                            radius_squared_vec[0] > x_sq2 + y_sq2 || // 1 + 3   3 + 0
                            half_size_x + radius >= fabsf(rot_x) && half_size_y >= fabsf(rot_y) ||
                            half_size_x >= fabsf(rot_x) && half_size_y + radius >= fabsf(rot_y)
                        ) {
                            hit_flags |= 1 << j;
                        }
                        */
                    }
                }
            }
        } while (++collider != collider_end);
    }

    uint32_t* ret = hit_flags_ptr;
    *ret = hit_flags;
    return ret;
}

struct EnemyData {
    dummy_fields(0x5638);
    uint32_t side_index;

    dllexport void* get_own_ability_manager() {
        assume(this->side_index < 2);
        return GAME_SIDES[this->side_index].ability_manager_ptr;
    }
    dllexport void* get_opposite_ability_manager() {
        assume(this->side_index < 2);
        return GAME_SIDES[1 - this->side_index].ability_manager_ptr;
    }
};


static inline constexpr float half_pi = M_PI / 2.0;
static inline constexpr float three_half_pi = 3.0 * M_PI / 2.0;

/*
dllexport vec<float, 2> vectorcall sincos(float x) {
    float low_cos = x - three_half_pi;
    float high_cos = x + half_pi;

}

dllexport vec<float, 2> vectorcall cossin(float x) {
    float cos_x = __builtin_fabs(x);
    float sin_x = cos_x + three_half_pi;
    vec<float, 2> wide_x = { sin_x, cos_x };
    //vec<float, 2> wide_x = { }
}
*/

;

long double vectorcall slow_sinf(float x) {
    long double sin;
    __asm__("fsin" : "=t"(sin) : "0"(x));
    return sin;
}

gnu_noinline gnu_attr(target("no-sse,no-sse2")) long double vectorcall slow_sin_old(double x) {
//#pragma clang fp exceptions(strict)
//#pragma clang fp eval_method(extended)
    static const int64_t temp_load[2] = { 0xC90FDAA22168C235, 0x403E };
    long double cmp_val = *(long double*)&temp_load;
    if (__builtin_fabs(x) > cmp_val) {
        register uint8_t status_word asm("ah");
        do {
            __asm__ volatile (
                "fprem1 \n"
                "fstsw %%ax \n"
                : "+t"(x), "=a"(status_word)
                : "u"(cmp_val)
            );
        } while (status_word & 4);
    }
    long double ret;
    __asm__("fsin" : "=t"(ret) : "0"(x));
    return ret;
}

// Value is 2**63.
// Running FSIN with a values >= 2**63
// or <= -2**63 causes it to give up
// and do nothing.
static const uint32_t trig_limit = 0x5F000000;

// Value is Pi * 2**62
static const int64_t trig_reduce[2] = { 0xC90FDAA22168C235, 0x403E };

#if __x86_64__
#define ASM_CLOBBER_AX_32
#define ASM_CLOBBER_AX_64 "ax"
#define ASM_STACK_ENTER4
#define ASM_STACK_ENTER8
#define ASM_STACK_ENTER16
#define ASM_STACK_LEAVE4
#define ASM_STACK_LEAVE8
#define ASM_STACK_LEAVE16
#if __AVX__
#define ASM_SSE_WRITE_FLT "VMOVSS %[x], 8(%%RSP) \n"
#define ASM_SSE_WRITE_DBL "VMOVSD %[x], 8(%%RSP) \n"
#define ASM_SSE_READ_FLT "VMOVSS 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_DBL "VMOVSD 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_2DBL "VMOVAPD 8(%%RSP), %[ret] \n"
#else
#define ASM_SSE_WRITE_FLT "MOVSS %[x], 8(%%RSP) \n"
#define ASM_SSE_WRITE_DBL "MOVSD %[x], 8(%%RSP) \n"
#define ASM_SSE_READ_FLT "MOVSS 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_DBL "MOVSD 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_2DBL "MOVAPD 8(%%RSP), %[ret] \n"
#endif
#define ASM_X87_READ_FLT "FLDS 8(%%RSP) \n"
#define ASM_X87_READ_DBL "FLDL 8(%%RSP) \n"
#define ASM_X87_WRITE_FLT "FSTPS 8(%%RSP) \n"
#define ASM_X87_WRITE_DBL "FSTPL 8(%%RSP) \n"
#define ASM_X87_WRTIE_FLT2 "FSTPS 12(%%RSP) \n"
#define ASM_X87_WRITE_DBL2 "FSTPL 16(%%RSP) \n"
#else
#define ASM_CLOBBER_AX_32 "ax"
#define ASM_CLOBBER_AX_64
#define ASM_STACK_ENTER4 "PUSH %%EAX \n"
#define ASM_STACK_ENTER8 "SUB $8, %%ESP \n"
#define ASM_STACK_ENTER16 "SUB $16, %%ESP \n"
#define ASM_STACK_LEAVE4 "POP %%EAX \n"
#define ASM_STACK_LEAVE8 "ADD $8, %%ESP \n"
#define ASM_STACK_LEAVE16 "ADD $16, %%ESP \n"
#if __AVX__
#define ASM_SSE_WRITE_FLT "VMOVSS %[x], (%%ESP) \n"
#define ASM_SSE_WRITE_DBL "VMOVSD %[x], (%%ESP) \n"
#define ASM_SSE_READ_FLT "VMOVSS (%%ESP), %[ret] \n"
#define ASM_SSE_READ_DBL "VMOVSD (%%ESP), %[ret] \n"
#define ASM_SSE_READ_2DBL "VMOVUPD (%%ESP), %[ret] \n"
#else
#define ASM_SSE_WRITE_FLT "MOVSS %[x], (%%ESP) \n"
#define ASM_SSE_WRITE_DBL "MOVSD %[x], (%%ESP) \n"
#define ASM_SSE_READ_FLT "MOVSS (%%ESP), %[ret] \n"
#define ASM_SSE_READ_DBL "MOVSD (%%ESP), %[ret] \n"
#define ASM_SSE_READ_2DBL "MOVUPD (%%ESP), %[ret] \n"
#endif
#define ASM_X87_READ_FLT "FLDS (%%ESP) \n"
#define ASM_X87_READ_DBL "FLDL (%%ESP) \n"
#define ASM_X87_WRITE_FLT "FSTPS (%%ESP) \n"
#define ASM_X87_WRITE_DBL "FSTPL (%%ESP) \n"
#define ASM_X87_WRTIE_FLT2 "FSTPS 4(%%ESP) \n"
#define ASM_X87_WRITE_DBL2 "FSTPL 8(%%ESP) \n"
#endif
#define ASM_SSE_READ_2FLT ASM_SSE_READ_DBL

gnu_noinline float vectorcall slow_sin(float x) {
    float ret;
    // .intel_syntax keeps emitting an extra
    // % for the x condition and killing itself
    __asm__(
        ASM_STACK_ENTER4
        ASM_SSE_WRITE_FLT
        ASM_X87_READ_FLT
        "FLDS %[trig_limit] \n"
        "FLD %%ST(1) \n"
        "FABS \n"
        "FUCOMIP %%ST(1), %%ST(0) \n"
        "FFREEP %%ST(0) \n"
        "JB 2f \n"
        "FLDT %[trig_reduce] \n"
        "FXCH %%ST(1) \n"
    "1: \n"
        "FPREM1 \n"
        "FNSTSW %%AX \n"
        "TEST $4, %%AH \n"
        "JNZ 1b \n"
        "FSTP %%ST(1) \n"
    "2: \n"
        "FSIN \n"
        ASM_X87_WRITE_FLT
        ASM_SSE_READ_FLT
        ASM_STACK_LEAVE4
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : ASM_CLOBBER_AX_64
    );
    return ret;
}

gnu_noinline float vectorcall slow_cos(float x) {
    float ret;
    // .intel_syntax keeps emitting an extra
    // % for the x condition and killing itself
    __asm__(
        ASM_STACK_ENTER4
        ASM_SSE_WRITE_FLT
        ASM_X87_READ_FLT
        "FLDS %[trig_limit] \n"
        "FLD %%ST(1) \n"
        "FABS \n"
        "FUCOMIP %%ST(1), %%ST(0) \n"
        "FFREEP %%ST(0) \n"
        "JB 2f \n"
        "FLDT %[trig_reduce] \n"
        "FXCH %%ST(1) \n"
    "1: \n"
        "FPREM1 \n"
        "FNSTSW %%AX \n"
        "TEST $4, %%AH \n"
        "JNZ 1b \n"
        "FSTP %%ST(1) \n"
    "2: \n"
        "FCOS \n"
        ASM_X87_WRITE_FLT
        ASM_SSE_READ_FLT
        ASM_STACK_LEAVE4
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : ASM_CLOBBER_AX_64
    );
    return ret;
}

gnu_noinline vec<float, 2> vectorcall slow_sincos(float x) {
    double ret;
    // .intel_syntax keeps emitting an extra
    // % for the x condition and killing itself
    __asm__(
        ASM_STACK_ENTER8
        ASM_SSE_WRITE_FLT
        ASM_X87_READ_FLT
        "FLDS %[trig_limit] \n"
        "FLD %%ST(1) \n"
        "FABS \n"
        "FUCOMIP %%ST(1), %%ST(0) \n"
        "FFREEP %%ST(0) \n"
        "JB 2f \n"
        "FLDT %[trig_reduce] \n"
        "FXCH %%ST(1) \n"
    "1: \n"
        "FPREM1 \n"
        "FNSTSW %%AX \n"
        "TEST $4, %%AH \n"
        "JNZ 1b \n"
        "FSTP %%ST(1) \n"
    "2: \n"
        "FSINCOS \n"
        ASM_X87_WRTIE_FLT2
        ASM_X87_WRITE_FLT
        ASM_SSE_READ_2FLT
        ASM_STACK_LEAVE8
        : [ret] "=x"(ret)
        : [x] "x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax"
    );
    return std::bit_cast<vec<float, 2>>(ret);
}

gnu_noinline double vectorcall slow_sin(double x) {
    double ret;
    // .intel_syntax keeps emitting an extra
    // % for the x condition and killing itself
    __asm__(
        ASM_STACK_ENTER8
        ASM_SSE_WRITE_DBL
        ASM_X87_READ_DBL
        "FLDT %[trig_limit] \n"
        "FLD %%ST(1) \n"
        "FABS \n"
        "FUCOMIP %%ST(1), %%ST(0) \n"
        "FFREEP %%ST(0) \n"
        "JB 2f \n"
        "FLDT %[trig_reduce] \n"
        "FXCH %%ST(1) \n"
    "1: \n"
        "FPREM1 \n"
        "FNSTSW %%AX \n"
        "TEST $4, %%AH \n"
        "JNZ 1b \n"
        "FSTP %%ST(1) \n"
    "2: \n"
        "FSIN \n"
        ASM_X87_WRITE_DBL
        ASM_SSE_READ_DBL
        ASM_STACK_LEAVE8
        : [ret] "=x"(ret)
        : [x] "x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax"
    );
    return ret;
}

gnu_noinline double vectorcall slow_cos(double x) {
    double ret;
    // .intel_syntax keeps emitting an extra
    // % for the x condition and killing itself
    __asm__(
        ASM_STACK_ENTER8
        ASM_SSE_WRITE_DBL
        ASM_X87_READ_DBL
        "FLDT %[trig_limit] \n"
        "FLD %%ST(1) \n"
        "FABS \n"
        "FUCOMIP %%ST(1), %%ST(0) \n"
        "FFREEP %%ST(0) \n"
        "JB 2f \n"
        "FLDT %[trig_reduce] \n"
        "FXCH %%ST(1) \n"
    "1: \n"
        "FPREM1 \n"
        "FNSTSW %%AX \n"
        "TEST $4, %%AH \n"
        "JNZ 1b \n"
        "FSTP %%ST(1) \n"
    "2: \n"
        "FCOS \n"
        ASM_X87_WRITE_DBL
        ASM_SSE_READ_DBL
        ASM_STACK_LEAVE8
        : [ret] "=x"(ret)
        : [x] "x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax"
    );
    return ret;
}

gnu_noinline vec<double, 2> vectorcall slow_sincos(double x) {
    vec<double, 2> ret;
    // .intel_syntax keeps emitting an extra
    // % for the x condition and killing itself
    __asm__(
        ASM_STACK_ENTER16
        ASM_SSE_WRITE_DBL
        ASM_X87_READ_DBL
        "FLDT %[trig_limit] \n"
        "FLD %%ST(1) \n"
        "FABS \n"
        "FUCOMIP %%ST(1), %%ST(0) \n"
        "FFREEP %%ST(0) \n"
        "JB 2f \n"
        "FLDT %[trig_reduce] \n"
        "FXCH %%ST(1) \n"
    "1: \n"
        "FPREM1 \n"
        "FNSTSW %%AX \n"
        "TEST $4, %%AH \n"
        "JNZ 1b \n"
        "FSTP %%ST(1) \n"
    "2: \n"
        "FSINCOS \n"
        ASM_X87_WRITE_DBL2
        ASM_X87_WRITE_DBL
        ASM_SSE_READ_2DBL
        ASM_STACK_LEAVE16
        : [ret] "=x"(ret)
        : [x] "x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax"
    );
    return ret;
}

long double vectorcall slow_cosf(float x) {
    long double sin;
    __asm__("fcos" : "=t"(sin) : "0"(x));
    return sin;
}
vec<long double, 2> regcall slow_sincosf(float x) {
    long double cos, sin;
    __asm__ volatile ("fsincos" : "=t"(cos), "=u"(sin) : "0"(x));
    return { cos, sin };
}

alignas(16) static const vec<double, 2, true> cosf_sinf_ctable[64] = {
    { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0x0000000000000000ll) },
    { std::bit_cast<double>(0x3FEFD88DA3D12526), std::bit_cast<double>(0x3FB917A6BC29B42C) },
    { std::bit_cast<double>(0x3FEF6297CFF75CB0), std::bit_cast<double>(0x3FC8F8B83C69A60B) },
    { std::bit_cast<double>(0x3FEE9F4156C62DDA), std::bit_cast<double>(0x3FD294062ED59F06) },
    { std::bit_cast<double>(0x3FED906BCF328D46), std::bit_cast<double>(0x3FD87DE2A6AEA963) },
    { std::bit_cast<double>(0x3FEC38B2F180BDB1), std::bit_cast<double>(0x3FDE2B5D3806F63B) },
    { std::bit_cast<double>(0x3FEA9B66290EA1A3), std::bit_cast<double>(0x3FE1C73B39AE68C8) },
    { std::bit_cast<double>(0x3FE8BC806B151741), std::bit_cast<double>(0x3FE44CF325091DD6) },
    { std::bit_cast<double>(0x3FE6A09E667F3BCD), std::bit_cast<double>(0x3FE6A09E667F3BCD) },
    { std::bit_cast<double>(0x3FE44CF325091DD6), std::bit_cast<double>(0x3FE8BC806B151741) },
    { std::bit_cast<double>(0x3FE1C73B39AE68C8), std::bit_cast<double>(0x3FEA9B66290EA1A3) },
    { std::bit_cast<double>(0x3FDE2B5D3806F63B), std::bit_cast<double>(0x3FEC38B2F180BDB1) },
    { std::bit_cast<double>(0x3FD87DE2A6AEA963), std::bit_cast<double>(0x3FED906BCF328D46) },
    { std::bit_cast<double>(0x3FD294062ED59F06), std::bit_cast<double>(0x3FEE9F4156C62DDA) },
    { std::bit_cast<double>(0x3FC8F8B83C69A60B), std::bit_cast<double>(0x3FEF6297CFF75CB0) },
    { std::bit_cast<double>(0x3FB917A6BC29B42C), std::bit_cast<double>(0x3FEFD88DA3D12526) },
    { std::bit_cast<double>(0x0000000000000000ll), std::bit_cast<double>(0x3FF0000000000000) },
    { std::bit_cast<double>(0xBFB917A6BC29B42C), std::bit_cast<double>(0x3FEFD88DA3D12526) },
    { std::bit_cast<double>(0xBFC8F8B83C69A60B), std::bit_cast<double>(0x3FEF6297CFF75CB0) },
    { std::bit_cast<double>(0xBFD294062ED59F06), std::bit_cast<double>(0x3FEE9F4156C62DDA) },
    { std::bit_cast<double>(0xBFD87DE2A6AEA963), std::bit_cast<double>(0x3FED906BCF328D46) },
    { std::bit_cast<double>(0xBFDE2B5D3806F63B), std::bit_cast<double>(0x3FEC38B2F180BDB1) },
    { std::bit_cast<double>(0xBFE1C73B39AE68C8), std::bit_cast<double>(0x3FEA9B66290EA1A3) },
    { std::bit_cast<double>(0xBFE44CF325091DD6), std::bit_cast<double>(0x3FE8BC806B151741) },
    { std::bit_cast<double>(0xBFE6A09E667F3BCD), std::bit_cast<double>(0x3FE6A09E667F3BCD) },
    { std::bit_cast<double>(0xBFE8BC806B151741), std::bit_cast<double>(0x3FE44CF325091DD6) },
    { std::bit_cast<double>(0xBFEA9B66290EA1A3), std::bit_cast<double>(0x3FE1C73B39AE68C8) },
    { std::bit_cast<double>(0xBFEC38B2F180BDB1), std::bit_cast<double>(0x3FDE2B5D3806F63B) },
    { std::bit_cast<double>(0xBFED906BCF328D46), std::bit_cast<double>(0x3FD87DE2A6AEA963) },
    { std::bit_cast<double>(0xBFEE9F4156C62DDA), std::bit_cast<double>(0x3FD294062ED59F06) },
    { std::bit_cast<double>(0xBFEF6297CFF75CB0), std::bit_cast<double>(0x3FC8F8B83C69A60B) },
    { std::bit_cast<double>(0xBFEFD88DA3D12526), std::bit_cast<double>(0x3FB917A6BC29B42C) },
    { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0x0000000000000000ll) },
    { std::bit_cast<double>(0xBFEFD88DA3D12526), std::bit_cast<double>(0xBFB917A6BC29B42C) },
    { std::bit_cast<double>(0xBFEF6297CFF75CB0), std::bit_cast<double>(0xBFC8F8B83C69A60B) },
    { std::bit_cast<double>(0xBFEE9F4156C62DDA), std::bit_cast<double>(0xBFD294062ED59F06) },
    { std::bit_cast<double>(0xBFED906BCF328D46), std::bit_cast<double>(0xBFD87DE2A6AEA963) },
    { std::bit_cast<double>(0xBFEC38B2F180BDB1), std::bit_cast<double>(0xBFDE2B5D3806F63B) },
    { std::bit_cast<double>(0xBFEA9B66290EA1A3), std::bit_cast<double>(0xBFE1C73B39AE68C8) },
    { std::bit_cast<double>(0xBFE8BC806B151741), std::bit_cast<double>(0xBFE44CF325091DD6) },
    { std::bit_cast<double>(0xBFE6A09E667F3BCD), std::bit_cast<double>(0xBFE6A09E667F3BCD) },
    { std::bit_cast<double>(0xBFE44CF325091DD6), std::bit_cast<double>(0xBFE8BC806B151741) },
    { std::bit_cast<double>(0xBFE1C73B39AE68C8), std::bit_cast<double>(0xBFEA9B66290EA1A3) },
    { std::bit_cast<double>(0xBFDE2B5D3806F63B), std::bit_cast<double>(0xBFEC38B2F180BDB1) },
    { std::bit_cast<double>(0xBFD87DE2A6AEA963), std::bit_cast<double>(0xBFED906BCF328D46) },
    { std::bit_cast<double>(0xBFD294062ED59F06), std::bit_cast<double>(0xBFEE9F4156C62DDA) },
    { std::bit_cast<double>(0xBFC8F8B83C69A60B), std::bit_cast<double>(0xBFEF6297CFF75CB0) },
    { std::bit_cast<double>(0xBFB917A6BC29B42C), std::bit_cast<double>(0xBFEFD88DA3D12526) },
    { std::bit_cast<double>(0x0000000000000000ll), std::bit_cast<double>(0xBFF0000000000000) },
    { std::bit_cast<double>(0x3FB917A6BC29B42C), std::bit_cast<double>(0xBFEFD88DA3D12526) },
    { std::bit_cast<double>(0x3FC8F8B83C69A60B), std::bit_cast<double>(0xBFEF6297CFF75CB0) },
    { std::bit_cast<double>(0x3FD294062ED59F06), std::bit_cast<double>(0xBFEE9F4156C62DDA) },
    { std::bit_cast<double>(0x3FD87DE2A6AEA963), std::bit_cast<double>(0xBFED906BCF328D46) },
    { std::bit_cast<double>(0x3FDE2B5D3806F63B), std::bit_cast<double>(0xBFEC38B2F180BDB1) },
    { std::bit_cast<double>(0x3FE1C73B39AE68C8), std::bit_cast<double>(0xBFEA9B66290EA1A3) },
    { std::bit_cast<double>(0x3FE44CF325091DD6), std::bit_cast<double>(0xBFE8BC806B151741) },
    { std::bit_cast<double>(0x3FE6A09E667F3BCD), std::bit_cast<double>(0xBFE6A09E667F3BCD) },
    { std::bit_cast<double>(0x3FE8BC806B151741), std::bit_cast<double>(0xBFE44CF325091DD6) },
    { std::bit_cast<double>(0x3FEA9B66290EA1A3), std::bit_cast<double>(0xBFE1C73B39AE68C8) },
    { std::bit_cast<double>(0x3FEC38B2F180BDB1), std::bit_cast<double>(0xBFDE2B5D3806F63B) },
    { std::bit_cast<double>(0x3FED906BCF328D46), std::bit_cast<double>(0xBFD87DE2A6AEA963) },
    { std::bit_cast<double>(0x3FEE9F4156C62DDA), std::bit_cast<double>(0xBFD294062ED59F06) },
    { std::bit_cast<double>(0x3FEF6297CFF75CB0), std::bit_cast<double>(0xBFC8F8B83C69A60B) },
    { std::bit_cast<double>(0x3FEFD88DA3D12526), std::bit_cast<double>(0xBFB917A6BC29B42C) },
};

//static float initial_sin_const = std::bit_cast<float>(0x4122F983);

#if __AVX__
#define CVTSS2SI(ret, flt) __asm__("vcvtss2si %[val], %[out]" : [out]"=r"(ret) : [val]"x"(flt))
#define CVTSD2SI(ret, dbl) __asm__("vcvtsd2si %[val], %[out]" : [out]"=r"(ret) : [val]"x"(dbl))
#else
#define CVTSS2SI(ret, flt) __asm__("cvtss2si %[val], %[out]" : [out]"=r"(ret) : [val]"x"(flt))
#define CVTSD2SI(ret, dbl) __asm__("cvtsd2si %[val], %[out]" : [out]"=r"(ret) : [val]"x"(dbl))
#endif

dllexport gnu_noinline double vectorcall libm_sse2_sin(double x) {
#pragma clang fp contract(off)

    vec<uint16_t, 4> tempA = std::bit_cast<vec<uint16_t, 4>>(x);
    uint32_t exp = tempA[3];

    exp = (exp & 0x7FFF) - 0x3030;

    if ((uint16_t)exp > 0x10C5) {
        if ((int16_t)exp <= 0x10C5) {
            if ((uint16_t)(exp >> 4) == 0xCFD) {
                // Value named ALL_ONES
                // Value is ~0.9999999999999999
                return x * std::bit_cast<double>(0x3FEFFFFFFFFFFFFF);
            }
            // Value named TWO_POW_55.
            // Value is 2**55
            double ret = std::bit_cast<double>(0x4360000000000000);
            ret *= x;
            ret -= x;
            // Value named TWO_POW_M55
            // Value is 1/2**55
            ret *= std::bit_cast<double>(0x3C80000000000000);
            return ret;
        }
        return slow_sin(x);
    }

    // XMM1
    // Value named PI32INV.
    // ~10.18591635788130
    double A = std::bit_cast<double>(0x40245F306DC9C883);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSD2SI(lookup_index, A);

    // 0x4338000000000000
    double SHIFTER = 6755399441055744.0;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }

    // XMM3, XMM2
    // P_1, P_2
    // ~0.09817477042088285, ~3.798187816439979e-12
    vec<double, 2> B = { std::bit_cast<double>(0x3FB921FB54400000), std::bit_cast<double>(0x3D90B4611A600000) };

    B *= A;

    x -= B[0];

    // XMM2
    vec<double, 2> C = __builtin_shufflevector(B, B, 1, 1);

    // Why TF is this in the original here?
    // lookup_index += 0x1C7600;
    lookup_index &= 0x3F;

    vec<double, 2> D = { x, x };

    // P_3
    // ~1.263916405497469e-22
    A *= std::bit_cast<double>(0x3B63198A2E037073);


}


/*
                                        // X0L
MOVSD XMM1, QWORD PTR [PI32INV]         // X0L, X1L
MULSD XMM1, XMM0                        // X0L, X1L
MOVSD XMM2, QWORD PTR [SHIFTER]         // X0L, X1L, X2L
CVTSD2SI EDX, XMM1
AND EDX, 0x3F
SHL EDX, 5
LEA EAX, [EDX+__acrt_cos_sin_ctable]
ADDSD XMM1, XMM2                        // X0L, X1L, X2L
SUBSD XMM1, XMM2                        // X0L, X1L, X2L
                                        // X0L, X1L
MOVSD XMM3, QWORD PTR [P_1]             // X0L, X1L, X3L
MOVAPD XMM2, XMMWORD PTR [P_2]          // X0L, X1L, (X2L, X2H), X3L
UNPCKLPD XMM1, XMM1                     // X0L, (X1L, X1H), (X2L, X2H), X3L
MULPD XMM2, XMM1                        // X0L, (X1L, X1H), (X2L, X2H), X3L
                                        // X0L, X1L, (X2L, X2H), X3L
MULSD XMM3, XMM1                        // X0L, X1L, X2L, X2H), X3L
MULSD XMM1, QWORD PTR [P_3]             // X0L, X1L, (X2L, X2H), X3L
SUBSD XMM0, XMM3                        // X0L, X1L, (X2L, X2H), X3L
                                        // X0L, X1L, (X2L, X2H)
UNPCKLPD XMM0, XMM0                     // (X0L, X0H), X1L, (X2L, X2H)
MOVAPD XMM5, XMMWORD PTR [SC_4]         // (X0L, X0H), X1L, (X2L, X2H), X5L, X5H
MULPD XMM5, XMM0                        // (X0L, X0H), X1L, (X2L, X2H), X5L, X5H
SUBPD XMM0, XMM2                        // (X0L, X0H), X1L, (X2L, X2H), X5L, X5H
                                        // (X0L, X0H), X1L, X2L, X5L, X5H
MOVSD XMM4, XMM0                        // (X0L, X0H), X1L, X2L, X4L, X5L, X5H
MOVSD XMM3, XMM4                        // (X0L, X0H), X1L, X2L, X3L, X4L, X5L, X5H
SUBSD XMM4, XMM2                        // (X0L, X0H), X1L, X2L, X3L, X4L, X5L, X5H
SUBSD XMM3, XMM4                        // (X0L, X0H), X1L, X2L, X3L, X4L, X5L, X5H
SUBSD XMM3, XMM2                        // (X0L, X0H), X1L, X2L, X3L, X4L, X5L, X5H
                                        // (X0L, X0H), X1L, X3L, X4L, X5L, X5H
SUBSD XMM1, XMM3                        // (X0L, X0H), X1L, X3L, X4L, X5L, X5H
                                        // (X0L, X0H), X1L, X4L, X5L, X5H
MULPD XMM5, XMM0                        // (X0L, X0H), X1L, X4L, X5L, X5H
MULPD XMM0, XMM0                        // (X0L, X0H), X1L, X4L, X5L, X5H
MOVAPD XMM2, XMMWORD PTR [EAX]          // (X0L, X0H), X1L, X2L, X2H, X4L, X5L, X5H
MOVAPD XMM7, XMM2                       // (X0L, X0H), X1L, X2L, X2H, X4L, X5L, X5H, X7L, X7H
MOVSD XMM3, QWORD PTR [EAX+0x18]        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X7L, X7H
ADDSD XMM2, XMM3                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X7L, X7H
MOVHLPS XMM6, XMM7                      // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X7L, X7H
MULSD XMM6, XMM4                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X7L, X7H
SUBSD XMM6, XMM2                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X7L, X7H
MULSD XMM1, XMM6                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X7L, X7H
ADDSD XMM1, QWORD PTR [EAX+0x10]        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X7L, X7H
                                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X7L, X7H
ADDSD XMM1, XMM6                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X7L, X7H
                                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X7L, X7H
MULSD XMM2, XMM4                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X7L, X7H
MULSD XMM3, XMM4                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X7L, X7H
MULSD XMM4, XMM7                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X7L, X7H
                                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
MOVAPD XMM6, XMMWORD PTR [SC_2]         // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
MULPD XMM2, XMM0                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
MULPD XMM6, XMM0                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
ADDPD XMM6, XMMWORD PTR [SC_1]          // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
MULPD XMM0, XMM0                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
ADDPD XMM5, XMMWORD PTR [SC_3]          // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
MULPD XMM5, XMM0                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
                                        // X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
ADDPD XMM6, XMM5                        // X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
                                        // X1L, X2L, X2H, X3L, X4L, X6L, X6H
MULPD XMM6, XMM2                        // X1L, X2L, X2H, X3L, X4L, X6L, X6H
                                        // X1L, X3L, X4L, X6L, X6H

MOVSD XMM0, XMM3



ADDSD XMM1, XMM5                        // X1L, X3L, X4L, X5L, X6L, X6H
                                        // X1L, X3L, X4L, X6L, X6H
ADDSD XMM1, XMM3                        // X1L, X3L, X4L, X6L, X6H
                                        // X1L, X4L, X6L, X6H
ADDSD XMM1, XMM6                        // X1L, X4L, X6L, X6H
UNPCKHPD XMM6, XMM6                     // X1L, X4L, X6L, X6H
                                        // X1L, X4L, X6L
ADDSD XMM1, XMM6                        // X1L, X4L, X6L
                                        // X1L, X4L
ADDSD XMM4, XMM1                        // X1L, X4L
                                        // X4L
MOVSD QWORD PTR [ESP+4], XMM4           // X4L
*/

/*
                                        // X0L
MOVSD XMM1, QWORD PTR [PI32INV]         // X0L, X1L
MULSD XMM1, XMM0                        // X0L, X1L
MOVSD XMM2, QWORD PTR [SHIFTER]         // X0L, X1L, X2L
CVTSD2SI EDX, XMM1
AND EDX, 0x3F
SHL EDX, 5
LEA EAX, [EDX+__acrt_cos_sin_ctable]
ADDSD XMM1, XMM2                        // X0L, X1L, X2L
SUBSD XMM1, XMM2                        // X0L, X1L, X2L
                                        // X0L, X1L
MOVSD XMM3, QWORD PTR [P_1]             // X0L, X1L, X3L
MOVAPD XMM2, XMMWORD PTR [P_2]          // X0L, X1L, (X2L, X2H), X3L
UNPCKLPD XMM1, XMM1                     // X0L, (X1L, X1H), (X2L, X2H), X3L
MULPD XMM2, XMM1                        // X0L, (X1L, X1H), (X2L, X2H), X3L
                                        // X0L, X1L, (X2L, X2H), X3L
MULSD XMM3, XMM1                        // X0L, X1L, X2L, X2H), X3L
MULSD XMM1, QWORD PTR [P_3]             // X0L, X1L, (X2L, X2H), X3L
SUBSD XMM0, XMM3                        // X0L, X1L, (X2L, X2H), X3L
                                        // X0L, X1L, (X2L, X2H)
UNPCKLPD XMM0, XMM0                     // (X0L, X0H), X1L, (X2L, X2H)
MOVAPD XMM5, XMMWORD PTR [SC_4]         // (X0L, X0H), X1L, (X2L, X2H), X5L, X5H
MULPD XMM5, XMM0                        // (X0L, X0H), X1L, (X2L, X2H), X5L, X5H
SUBPD XMM0, XMM2                        // (X0L, X0H), X1L, (X2L, X2H), X5L, X5H
                                        // (X0L, X0H), X1L, X2L, X5L, X5H
MOVSD XMM4, XMM0                        // (X0L, X0H), X1L, X2L, X4L, X5L, X5H
MOVSD XMM3, XMM4                        // (X0L, X0H), X1L, X2L, X3L, X4L, X5L, X5H
SUBSD XMM4, XMM2                        // (X0L, X0H), X1L, X2L, X3L, X4L, X5L, X5H
SUBSD XMM3, XMM4                        // (X0L, X0H), X1L, X2L, X3L, X4L, X5L, X5H
SUBSD XMM3, XMM2                        // (X0L, X0H), X1L, X2L, X3L, X4L, X5L, X5H
                                        // (X0L, X0H), X1L, X3L, X4L, X5L, X5H
SUBSD XMM1, XMM3                        // (X0L, X0H), X1L, X3L, X4L, X5L, X5H
                                        // (X0L, X0H), X1L, X4L, X5L, X5H
MULPD XMM5, XMM0                        // (X0L, X0H), X1L, X4L, X5L, X5H
MULPD XMM0, XMM0                        // (X0L, X0H), X1L, X4L, X5L, X5H
MOVAPD XMM2, XMMWORD PTR [EAX]          // (X0L, X0H), X1L, X2L, X2H, X4L, X5L, X5H
MOVSD XMM3, QWORD PTR [EAX+0x18]        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
ADDSD XMM2, XMM3                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
MOVSD XMM7, QWORD PTR [EAX+8]           // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X7L
MULSD XMM7, XMM4                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X7L
SUBSD XMM7, XMM2                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X7L
MULSD XMM1, XMM7                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X7L
                                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
ADDSD XMM1, QWORD PTR [EAX+0x10]        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
                                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
ADDSD XMM1, XMM6                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
                                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
MULSD XMM2, XMM4                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
MULSD XMM3, XMM4                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
MULSD XMM4, QWORD PTR [EAX]             // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
                                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H
MOVAPD XMM6, XMMWORD PTR [SC_2]         // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
MULPD XMM2, XMM0                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
MULPD XMM6, XMM0                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
ADDPD XMM6, XMMWORD PTR [SC_1]          // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
MULPD XMM0, XMM0                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
ADDPD XMM5, XMMWORD PTR [SC_3]          // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
MULPD XMM5, XMM0                        // (X0L, X0H), X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
                                        // X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
ADDPD XMM6, XMM5                        // X1L, X2L, X2H, X3L, X4L, X5L, X5H, X6L, X6H
                                        // X1L, X2L, X2H, X3L, X4L, X6L, X6H
MULPD XMM6, XMM2                        // X1L, X2L, X2H, X3L, X4L, X6L, X6H
                                        // X1L, X3L, X4L, X6L, X6H
MOVSD XMM0, XMM3                        // X0L, X1L, X3L, X4L, X6L, X6H
ADDSD XMM3, QWORD PTR [EAX+8]           // X0L, X1L, X3L, X4L, X6L, X6H
MOVSD XMM7, XMM4                        // X0L, X1L, X3L, X4L, X6L, X6H, X7L
ADDSD XMM4, XMM3                        // X0L, X1L, X3L, X4L, X6L, X6H, X7L
MOVSD XMM5, QWORD PTR [EAX+8]           // X0L, X1L, X3L, X4L, X5L, X6L, X6H, X7L
SUBSD XMM5, XMM3                        // X0L, X1L, X3L, X4L, X5L, X6L, X6H, X7L
SUBSD XMM3, XMM4                        // X0L, X1L, X3L, X4L, X5L, X6L, X6H, X7L
                                        // X0L, X1L, X3L, X5L, X6L, X6H, X7L
ADDSD XMM5, XMM0                        // X0L, X1L, X3L, X5L, X6L, X6H, X7L
                                        // X1L, X3L, X5L, X6L, X6H, X7L
ADDSD XMM3, XMM7                        // X1L, X3L, X5L, X6L, X6H, X7L
                                        // X1L, X3L, X5L, X6L, X6H
ADDSD XMM1, XMM5                        // X1L, X3L, X5L, X6L, X6H
                                        // X1L, X3L, X6L, X6H
ADDSD XMM1, XMM3                        // X1L, X3L, X6L, X6H
                                        // X1L, X4L, X6L, X6H
ADDSD XMM1, XMM6                        // X1L, X4L, X6L, X6H
UNPCKHPD XMM6, XMM6                     // X1L, X4L, X6L, X6H
                                        // X1L, X4L, X6L
ADDSD XMM1, XMM6                        // X1L, X4L, X6L
                                        // X1L, X4L
ADDSD XMM4, XMM1                        // X1L, X4L
                                        // X4L
MOVSD QWORD PTR [ESP+4], XMM4           // X4L
*/

dllexport gnu_noinline double vectorcall libm_sse2_sin_no_vec(double x) {
#pragma clang fp contract(off)
    vec<double, 2> tempA;
    tempA[0] = x;

    vec<uint16_t, 8> tempB = (vec<uint16_t, 8>)tempA;

    uint32_t exp = tempB[3];

    exp = (exp & 0x7FFF) - 0x3030;

    if ((uint16_t)exp > 0x10C5) {
        if ((int16_t)exp <= 0x10C5) {
            if ((uint16_t)(exp >> 4) == 0xCFD) {
                // Value named ALL_ONES
                // Value is ~0.9999999999999999
                return x * std::bit_cast<double>(0x3FEFFFFFFFFFFFFF);
            }
            // Value named TWO_POW_55.
            // Value is 2**55
            double ret = std::bit_cast<double>(0x4360000000000000);
            ret *= x;
            ret -= x;
            // Value named TWO_POW_M55
            // Value is 1/2**55
            ret *= std::bit_cast<double>(0x3C80000000000000);
            return ret;
        }
        return slow_sin(x);
    }

    // XMM1
    // Value named PI32INV.
    // ~10.18591635788130
    double X1 = std::bit_cast<double>(0x40245F306DC9C883);
    X1 *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSD2SI(lookup_index, X1);

    // 0x4338000000000000
    double SHIFTER = 6755399441055744.0;
    {
#pragma clang fp exceptions(strict)
        X1 += SHIFTER;
        X1 -= SHIFTER;
    }

    // XMM3, XMM2
    // P_1, P_2
    // ~0.09817477042088285, ~3.798187816439979e-12
    double X3 = std::bit_cast<double>(0x3FB921FB54400000);
    double X2 = std::bit_cast<double>(0x3D90B4611A600000);

    X3 *= X1;
    X2 *= X1;

    // Why TF is this in the original here?
    // lookup_index += 0x1C7600;
    lookup_index &= 0x3F;

    const double* lookup_table = (const double*)&cosf_sinf_ctable[lookup_index * 2];

    x -= X3;
    X3 = x;

    // P_3
    // ~1.263916405497469e-22
    X1 *= std::bit_cast<double>(0x3B63198A2E037073);

    //X3 = x;
    //double X4 = x;

    //X4 -= X2;

    // XMM5
    // SC_4
    // 2.755731922398589e-06, 2.480158730158730e-05
    double X5 = std::bit_cast<double>(0x3EC71DE3A556C734);
    double X8 = std::bit_cast<double>(0x3EFA01A01A01A01A);

    X5 *= x;
    X8 *= x;

    x -= X2;
    double X4 = x;

    // XMM6
    // SC_2
    // 0.008333333333333333, 0.04166666666666666
    double X6 = std::bit_cast<double>(0x3F81111111111111);
    double X9 = std::bit_cast<double>(0x3FA5555555555555);


}


/*
                                        // 0
MOVSD XMM1, QWORD PTR [PI32INV]         // 0, 1
MULSD XMM1, XMM0                        // 0, 1
MOVSD XMM2, QWORD PTR [SHIFTER]         // 0, 1, 2
CVTSD2SI EDX, XMM1
ADDSD XMM1, XMM2                        // 0, 1, 2
MOVSD XMM3, QWORD PTR [P_1]             // 0, 1, 2, 3
SUBSD XMM1, XMM2                        // 0, 1, 2, 3
                                        // 0, 1, 3
MOVSD XMM2, QWORD PTR [P_2]             // 0, 1, 2, 3
MULSD XMM3, XMM1                        // 0, 1, 2, 3
AND EDX, 0x3F
MOVSD XMM5, QWORD PTR [SC_4]            // 0, 1, 2, 3, 5
MOVSD XMM8, QWORD PTR [SC_4+8]          // 0, 1, 2, 3, 5, 8
SHL EDX, 5
LEA EAX, [EDX+__acrt_cos_sin_ctable]
MULSD XMM2, XMM1                        // 0, 1, 2, 3, 5, 8
SUBSD XMM0, XMM3                        // 0, 1, 2, 3, 5, 8
SUBSD XMM0, XMM3                        // 0, 1, 2, 5, 8
MULSD XMM1, QWORD PTR [P_3]             // 0, 1, 2, 5, 8
MOVSD XMM3, XMM0                        // 0, 1, 2, 3, 5, 8
MOVSD XMM13, QWORD PTR [EAX+8]          
MOVSD XMM7, XMM13                       // 0, 1, 2, 3, 4, 5, 7, 8
MOVSD XMM4, XMM0                        // 0, 1, 2, 3, 4, 5, 7, 8
SUBSD XMM4, XMM2                        // 0, 1, 2, 3, 4, 5, 7, 8
MULSD XMM5, XMM0                        // 0, 1, 2, 3, 4, 5, 7, 8
MULSD XMM8, XMM0                        // 0, 1, 2, 3, 4, 5, 7, 8
SUBSD XMM0, XMM2                        // 0, 1, 2, 3, 4, 5, 7, 8
MOVSD XMM6, QWORD PTR [SC_2]            // 0, 1, 2, 3, 4, 5, 6, 7, 8
MOVSD XMM9, QWORD PTR [SC_2+8]          // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
MULSD XMM7, XMM4                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
SUBSD XMM3, XMM4                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
MULSD XMM5, XMM0                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
MULSD XMM8, XMM0                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
MULSD XMM0, XMM0                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
SUBSD XMM3, XMM2                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
                                        // 0, 1, 3, 4, 5, 6, 7, 8, 9
MOVSD XMM12, QWORD PTR [EAX]
MOVSD XMM2, XMM12                       // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
MOVSD XMM10, XMM13                      // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
SUBSD XMM1, XMM3                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
                                        // 0, 1, 2, 4, 5, 6, 7, 8, 9, 10
MOVSD XMM15, QWORD PTR [EAX+0x18]       
MOVSD XMM3, XMM15                       // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
ADDSD XMM2, XMM3                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
SUBSD XMM7, XMM2                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM2, XMM4                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM6, XMM0                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM9, XMM0                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM3, XMM4                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM2, XMM0                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM10, XMM0                       // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM0, XMM0                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
ADDSD XMM5, QWORD PTR [SC_3]            // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
ADDSD XMM8, QWORD PTR [SC_3+8]          // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM4, XMM12                       // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
ADDSD XMM6, QWORD PTR [SC_1]            // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
ADDSD XMM9, QWORD PTR [SC_1+8]          // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM5, XMM0                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM8, XMM0                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
                                        // 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MOVSD XMM0, XMM3                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
ADDSD XMM3, XMM13                       // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
MULSD XMM1, XMM7                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
                                        // 0, 1, 2, 3, 4, 5, 6, 8, 9, 10
MOVSD XMM7, XMM4                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
ADDSD XMM4, XMM3                        // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
ADDSD XMM6, XMM5
ADDSD XMM9, XMM8
MOVSD XMM5, XMM13
SUBSD XMM5, XMM3
SUBSD XMM3, XMM4
ADDSD XMM1, QWORD PTR [EAX+0x10]
MULSD XMM6, XMM2
MULSD XMM9, XMM10
ADDSD XMM5, XMM0
ADDSD XMM3, XMM7
ADDSD XMM1, XMM5
ADDSD XMM1, XMM3
ADDSD XMM1, XMM6
ADDSD XMM1, XMM9
ADDSD XMM4, XMM1
*/

dllexport gnu_noinline float vectorcall libm_sse2_sinf(float x) {
#pragma clang fp contract(off)

    uint32_t int_x = std::bit_cast<uint32_t>(x);

    // Magic bit manipulation :D
    uint32_t exp = (int_x & 0x7FFF0000) - 0x00800000;
    if ((uint16_t)(exp >> 16) > 0x457F) {
        if ((int16_t)(exp >> 16) <= 0x457F) {
            // Value named ONE_M230
            // Value is ~0.9999999990686774
            return x * std::bit_cast<double>(0x3FEFFFFFFF800000);
        }
        if ((int_x & 0x7F800000) != 0x7F800000) {
            // This actually calls libm_sse2_sin
            // first, which doesn't always fall through.
            // I haven't implemented that yet.
            return slow_sin(x);
        }
        // Original code always gives -NaN here?
        // But it uses SUBSS?
        return std::bit_cast<float>(0xFFC00000);
    }

    // Value named PI_32_RECIP.
    // ~10.18591
    float A = std::bit_cast<float>(0x4122F983);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSS2SI(lookup_index, A);
    double B = x;

    // 0x4B400000
    float SHIFTER = 12582912.0f;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }

    // XMM3, XMM2
    // P_2,  P_1
    // Magic BS numbers.
    // The only difference between the two
    // sets is ^0x9000000000000000
    vec<double, 2> posP = { std::bit_cast<double>(0x7532E7B9676733AF), std::bit_cast<double>(0x77B921FB54444000) };
    vec<double, 2> negP = { std::bit_cast<double>(0xE532E7B9676733AF), std::bit_cast<double>(0xE7B921FB54444000) };

    // Why TF is this in the original here?
    // lookup_index += 0x1C7600;
    lookup_index &= 0x3F;

    vec<double, 2> C = x >= 0.0f ? posP : negP;

    // Magic bit shift in the middle???
    vec<float, 4> D = { A, 0.0f };
    vec<uint64_t, 2> E = (vec<uint64_t, 2>)D << 29;
    vec<double, 2> F = (vec<double, 2>)E;

    F[1] = F[0];
    C *= F;

    B -= C[1];
    C[0] += B;

    B *= B;

    vec<double, 2> G = { B, B };

    // XMM5, XMM4
    // S_1,  C_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> H = { -1.0/6.0, -0.5 };

    H *= G;

    G *= G;

    // XMM7, XMM6
    // S_2,  C_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> I = { 1.0/120.0, 1.0/24.0 };

    I *= G;

    H += 1.0;

    H += I;
    
    vec<double, 2> J = cosf_sinf_ctable[lookup_index];
    J[0] *= C[0];

    J *= H;

    J[0] += J[1];

    return J[0];
}

dllexport gnu_noinline float vectorcall libm_sse2_cosf(float x) {
#pragma clang fp contract(off)

    uint32_t int_x = std::bit_cast<uint32_t>(x);

    // Magic bit manipulation :D
    uint32_t exp = (int_x & 0x7FFF0000) - 0x00800000;
    if ((uint16_t)(exp >> 16) > 0x457F) {
        if ((int16_t)(exp >> 16) <= 0x457F) {
            double dbl_x = x;
            return 1.0 - dbl_x * dbl_x;
        }
        if ((int_x & 0x7F800000) != 0x7F800000) {
            // This actually calls libm_sse2_cos
            // first, which doesn't always fall through.
            // I haven't implemented that yet.
            return slow_cos(x);
        }
        // Original code always gives -NaN here?
        // But it uses SUBSS?
        return std::bit_cast<float>(0xFFC00000);
    }

    // Value named PI_32_RECIP.
    // ~10.18591
    float A = std::bit_cast<float>(0x4122F983);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSS2SI(lookup_index, A);
    double B = x;

    // 0x4B400000
    float SHIFTER = 12582912.0f;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }

    // XMM3, XMM2
    // P_2,  P_1
    // Magic BS numbers.
    // The only difference between the two
    // sets is ^0x9000000000000000
    vec<double, 2> posP = { std::bit_cast<double>(0x7532E7B9676733AF), std::bit_cast<double>(0x77B921FB54444000) };
    vec<double, 2> negP = { std::bit_cast<double>(0xE532E7B9676733AF), std::bit_cast<double>(0xE7B921FB54444000) };

    // Why TF is this in the original here?
    // lookup_index += 0x1C7600;
    lookup_index += 0x10;
    lookup_index &= 0x3F;

    vec<double, 2> C = x >= 0.0f ? posP : negP;

    // Magic bit shift in the middle???
    vec<float, 4> D = { A, 0.0f };
    vec<uint64_t, 2> E = (vec<uint64_t, 2>)D << 29;
    vec<double, 2> F = (vec<double, 2>)E;

    F[1] = F[0];
    C *= F;

    B -= C[1];
    C[0] += B;

    B *= B;

    vec<double, 2> G = { B, B };

    // XMM5, XMM4
    // S_1,  C_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> H = { -1.0/6.0, -0.5 };

    H *= G;

    G *= G;

    // XMM7, XMM6
    // S_2,  C_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> I = { 1.0/120.0, 1.0/24.0 };

    I *= G;

    H += 1.0;

    H += I;
    
    vec<double, 2> J = cosf_sinf_ctable[lookup_index];
    J[0] *= C[0];

    J *= H;

    J[0] += J[1];

    return J[0];
}

dllexport gnu_noinline vec<float, 2> vectorcall libm_sse2_sincosf(float x) {
#pragma clang fp contract(off)

    uint32_t int_x = std::bit_cast<uint32_t>(x);

    // Magic bit manipulation :D
    uint32_t exp = (int_x & 0x7FFF0000) - 0x00800000;
    if ((uint16_t)(exp >> 16) > 0x457F) {
        if ((int16_t)(exp >> 16) <= 0x457F) {
            // Value named ONE_M230
            // Value is ~0.9999999990686774
            double dbl_x = x;
            double sine_x = dbl_x * std::bit_cast<double>(0x3FEFFFFFFF800000);
            dbl_x *= dbl_x;
            dbl_x = 1.0 - dbl_x;
            vec<double, 2> dbl_vec = { sine_x, dbl_x };
            return convertvec(dbl_vec, vec<float, 2>);
        }
        if ((int_x & 0x7F800000) != 0x7F800000) {
            // This actually calls libm_sse2_cos
            // first, which doesn't always fall through.
            // I haven't implemented that yet.
            return slow_sincos(x);
        }
        // Original code always gives -NaN here?
        // But it uses SUBSS?
        return { std::bit_cast<float>(0xFFC00000), std::bit_cast<float>(0xFFC00000) };
    }

    // Value named PI_32_RECIP.
    // ~10.18591
    float A = std::bit_cast<float>(0x4122F983);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSS2SI(lookup_index, A);
    double B = x;

    // 0x4B400000
    float SHIFTER = 12582912.0f;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }

    // XMM3, XMM2
    // P_2,  P_1
    // Magic BS numbers.
    // The only difference between the two
    // sets is ^0x9000000000000000
    vec<double, 2> posP = { std::bit_cast<double>(0x7532E7B9676733AF), std::bit_cast<double>(0x77B921FB54444000) };
    vec<double, 2> negP = { std::bit_cast<double>(0xE532E7B9676733AF), std::bit_cast<double>(0xE7B921FB54444000) };

    // Why TF is this in the original here?
    // lookup_index += 0x1C7600;
    int32_t sine_lookup_index = lookup_index;
    int32_t cosine_lookup_index = lookup_index + 0x10;
    sine_lookup_index &= 0x3F;
    cosine_lookup_index &= 0x3F;

    vec<double, 2> C = x >= 0.0f ? posP : negP;

    // Magic bit shift in the middle???
    vec<float, 4> D = { A, 0.0f };
    vec<uint64_t, 2> E = (vec<uint64_t, 2>)D << 29;
    vec<double, 2> F = (vec<double, 2>)E;

    F[1] = F[0];
    C *= F;

    B -= C[1];
    C[0] += B;

    B *= B;

    vec<double, 2> G = { B, B };

    // XMM5, XMM4
    // S_1,  C_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> H = { -1.0/6.0, -0.5 };

    H *= G;

    G *= G;

    // XMM7, XMM6
    // S_2,  C_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> I = { 1.0/120.0, 1.0/24.0 };

    I *= G;

    H += 1.0;

    H += I;
    
    vec<double, 2> J_sin = cosf_sinf_ctable[sine_lookup_index];
    vec<double, 2> J_cos = cosf_sinf_ctable[cosine_lookup_index];

    J_sin[0] *= C[0];
    J_cos[0] *= C[0];

    J_sin *= H;
    J_cos *= H;

    vec<double, 2> ret = shufflevec(J_sin, J_cos, 0, 2);
    vec<double, 2> add = shufflevec(J_sin, J_cos, 1, 3);

    ret += add;

    /*
    vec<double, 2> J_sin = cosf_sinf_ctable[sine_lookup_index];
    vec<double, 2> J_cos = cosf_sinf_ctable[cosine_lookup_index];
    
    vec<double, 2> low = shufflevec(J_sin, J_cos, 0, 2);
    vec<double, 2> high = shufflevec(J_sin, J_cos, 1, 3);

    low *= C[0];

    low *= H;
    high *= H;

    low += high;
    */

    return convertvec(ret, vec<float, 2>);
}

dllexport float vectorcall fast_sinf(float x) {

}

dllexport float vectorcall fast_cosf(float x) {

}

dllexport float vectorcall fast_sincosf(float x) {

}

template <typename T = uint8_t>
[[nodiscard]] inline auto read_file_to_buffer(const char* path) {
    long file_size = 0;
    T* buffer = NULL;
    if (FILE* file = fopen(path, "rb")) {
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        if ((buffer = (T*)malloc(file_size))) {
            rewind(file);
            fread(buffer, file_size, 1, file);
        }
        fclose(file);
    }
    return std::make_pair(buffer, file_size);
}

static inline constexpr float minimum_negative_float_value = -0x1.FFFFFEp127f;
static inline constexpr float maximum_negative_float_value = -0x0.0p0f;
static inline constexpr float minimum_positive_float_value = 0x0.0p0f;
static inline constexpr float maximum_positive_float_value = 0x1.FFFFFEp127f;

extern "C" {
    extern float vectorcall libm_sinf(float x) asm("___libm_sse2_sinf");
}

int main() {

    //Sleep(15000);

    /*
    uint64_t slowA = rdtsc();
    for (int32_t i = 0; i < 100000000; ++i) {
        volatile float A = slow_sinA(i);
    }
    slowA = rdtsc() - slowA;

    uint64_t slowB = rdtsc();
    for (int32_t i = 0; i < 100000000; ++i) {
        volatile float B = slow_sinB(i);
    }
    slowB = rdtsc() - slowB;

    printf(
        "SlowA: %llu\n"
        "SlowB: %llu\n"
        , slowA
        , slowB
    );
    */

    /*
    size_t total = 0;
    size_t correct_count = 0;

    for (
        uint32_t value = std::bit_cast<uint32_t>(minimum_negative_float_value);
        value >= std::bit_cast<uint32_t>(maximum_negative_float_value);
        --value
    ) {
        ++total;
        float x = std::bit_cast<float>(value);
        uint32_t correct = std::bit_cast<uint32_t>(libm_sinf(x));
        uint32_t test = std::bit_cast<uint32_t>(libm_sse2_sinf(x));
        //if (correct != test) __asm int3
        correct_count += correct == test;
    }
    for (
        uint32_t value = std::bit_cast<uint32_t>(minimum_positive_float_value);
        value <= std::bit_cast<uint32_t>(maximum_positive_float_value);
        ++value
    ) {
        ++total;
        float x = std::bit_cast<float>(value);
        uint32_t correct = std::bit_cast<uint32_t>(libm_sinf(x));
        uint32_t test = std::bit_cast<uint32_t>(libm_sse2_sinf(x));
        //if (correct != test) __asm int3
        correct_count += correct == test;
    }
    
    printf(
        "Total: %zu\n"
        "Match: %zu\n"
        "Fails: %zu\n"
        , total
        , correct_count
        , total - correct_count
    );
    */

    
    if (HMODULE handle = LoadLibraryExW(L"F:\\Touhou_Stuff_2\\disassembly_stuff\\19\\th19\\th19.exe", NULL, LOAD_LIBRARY_AS_IMAGE_RESOURCE)) {

        //vec<uint64_t, 2>* sinf_data_ptr = based_pointer<vec<uint64_t, 2>>(handle, 0x1897C0 - 2);
        vec<uint64_t, 4>* sin_data_ptr = based_pointer<vec<uint64_t, 4>>(handle, 0x189DE0 - 2);

        FILE* temp = fopen("F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\temp.txt", "wb");

        for (size_t i = 0; i < 64; ++i) {
            /*
            fprintf(temp,
                "{ std::bit_cast<double>(0x%llX), std::bit_cast<double>(0x%llX) },\n"
                , sinf_data_ptr[i][1], sinf_data_ptr[i][0]
            );
            */

            fprintf(temp,
                "{ std::bit_cast<double>(0x%llX), std::bit_cast<double>(0x%llX) },\n"
                "{ std::bit_cast<double>(0x%llX), std::bit_cast<double>(0x%llX) },\n"
                , sin_data_ptr[i][0], sin_data_ptr[i][1]
                , sin_data_ptr[i][3], sin_data_ptr[i][2]
            );
        }

        fclose(temp);
        FreeLibrary(handle);
    }
    

    //sizeof(std::mutex)
    return 0;
}