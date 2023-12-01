#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#pragma clang diagnostic ignored "-Winvalid-source-encoding"

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdint.h>
#include <direct.h>

#include <string.h>

#define _USE_MATH_DEFINES 1
#include <math.h>

#include <bit>

#include "../zero/FloatConstants.h"

#include "../zero/util.h"

template <typename T>
using ZUNList = ZUNLinkedListBase<T, true>;
template <typename T>
using ZUNListHeadDummy = ZUNLinkedListHeadDummyBase<T, true>;

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
        dummy_fields(0x4D8);
    };
    ValidateStructSize(0x4D8, AnmVMData);

    // size: 0x15C
    struct AnmVMController {
        AnmID id = {}; // 0x0, 0x4D8
        uint32_t fast_id; // 0x4, 0x4DC
        dummy_fields(0x2C); // 0x8, 0x4E0
        ZUNList<AnmVM> global_list_node; // 0x34
        dummy_fields(0x118);
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

// size: 0x3C
struct GameSide {
    void* bullet_manager_ptr; // 0x0
    void* player_ptr; // 0x4
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

int main() {
    return 0;
}