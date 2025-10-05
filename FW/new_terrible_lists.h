#include <stdlib.h>
#include <stdint.h>

template <typename T>
struct ZUNListIter;

template <typename T>
struct ZUNListIterable;

template <typename T>
struct ZUNList {
    T* data; // 0x0
    ZUNList<T>* next; // 0x4
    ZUNList<T>* prev; // 0x8
    ZUNListIterable<T>* list; // 0xC
    ZUNListIter<T>* current_iter; // 0x10
    // 0x14
    
    T*& get_data() {
        return this->data;
    }
    
    ZUNList<T>* get_next() {
        return this->next;
    }

    ZUNList<T>* get_prev() {
        return this->prev;
    }
    
    void set_current_iter(ZUNListIter<T>* iter) {
        this->current_iter = iter;
    }

    // Rx11A90
    void unlink();
    
    // Rx11B00
    void unlink_impl();
};

template <typename T>
struct ZUNListIter {
    ZUNList<T>* current; // 0x0
    ZUNList<T>* next; // 0x4
    // 0x8
    
    // Rx118B0
    ZUNList<T>* set(ZUNList<T>* node) {
        if (this->next) {
            ZUNList<T>* old_next = this->next;
            old_next->set_current_iter(NULL);
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
        this->current = node;
        return node;
    }
    
    ZUNList<T>* operator*() {
        return this->current;
    }
    
    // Rx119B0
    ZUNListIter<T>& operator++() {
        this->current = this->next;
        if (this->next) {
            ZUNList<T>* old_next = this->next;
            old_next->set_current_iter(NULL);
        }
        ZUNList<T>* next_node;
        if (this->current) {
            next_node = this->current->get_next();
        } else {
            next_node = NULL;
        }
        this->next = next_node;
        if (this->next) {
            ZUNList<T>* new_next = this->next;
            new_next->set_current_iter(this);
        }
        return *this;
    }
    
    // Rx11930
    uint8_t operator!=(ZUNList<T>* node) {
        int ret;
        if (node) {
            // I may have this line wrong, it seems weird
            ret = (void*)this->current != (void*)node->data;
        } else {
            ret = this->current != NULL;
        }
        return ret;
    }
};

template <typename T>
struct ZUNListIterable : ZUNList<T> {
    // ZUNList<T> base; // 0x0
    ZUNList<T>* head; // 0x14
    ZUNListIter<T> iter; // 0x18
    // 0x20
    
    // Rx12060
    ZUNListIter<T>& begin() {
        this->iter.set(this->get_next());
        return this->iter;
    }
    ZUNList<T>* end() {
        return NULL;
    }

    // Rx11AD0
    void unlink_node(ZUNList<T>* node) {
        if (this->head == node) {
            this->head = node->get_prev();
        }
        node->unlink_impl();
    }
};

// Rx11A90
template <typename T>
void ZUNList<T>::unlink() {
    if (this->list) {
        ZUNListIterable<T>* list = this->list;
        list->unlink_node(this);
    } else {
        this->unlink_impl();
    }
}

// Rx11B00
template <typename T>
void ZUNList<T>::unlink_impl() {
    if (this->current_iter) {
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


struct LaserData {
    // void* vftable; // 0x0
    char paddingA[0x4]; // 0x4
    ZUNList<LaserData> list_node; // 0x8
    char paddingB[0x4]; // 0x1C
    int state; // 0x20

    virtual void __method_0() {}
    virtual void __method_4() {}
    virtual void __method_8() {}
    virtual void __method_C() {}
    virtual void __method_10() {}
    virtual void __method_14() {}
    virtual void __method_18() {}
    virtual void __method_1C() {}
    virtual void __method_20() {}
    __declspec(dllexport) virtual int cleanup() {
        this->list_node.unlink();
        return 0;
    }
};

struct LaserManager {
    char task_padding[0x10]; // 0x0
    ZUNListIterable<LaserData> laser_list; // 0x10
    int32_t laser_count; // 0x30
    int32_t prev_laser_id; // 0x34
    char float3_padding[0x18]; // 0x38
    void* bullet_anm; // 0x50
    int __dword_54; // 0x54
    uint32_t side_index; // 0x58
    void* game_side_ptr; // 0x5C
    // 0x60
    
    __declspec(dllexport) int on_tick() {
        //ANM_MANAGER_PTR->set_game_side(this->side_index);
        for (auto laser_node : this->laser_list) {
            if (laser_node->get_data()->state != 1) {
                LaserData* laser = laser_node->get_data();
                laser->__method_20();
            }
        }
        return 1;
    }

    __declspec(dllexport) int on_draw() {
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
};

// size: 0x30
struct GameSide {
    void* bullet_manager_ptr; // 0x0
    void* player_ptr; // 0x4
    void* enemy_manager_ptr; // 0x8
    void* item_manager_ptr; // 0xC
    void* spellcard_ptr; // 0x10
    LaserManager* laser_manager_ptr; // 0x14
    void* __ptr_18; // 0x18
    void* __ptr_1C; // 0x1C
    void* effect_manager_ptr; // 0x20
    void* __ptr_24; // 0x24
    void* __ptr_28; // 0x28
    void* __ptr_2C; // 0x2C
    // 0x30
};

// size: 0xF0
struct GlobalsSide {
    uint64_t score; // 0x0
    int __dword_8; // 0x8
    int __dword_C; // 0xC
    int __dword_10; // 0x10
    int __dword_14; // 0x14
    int __dword_18; // 0x18
    int __dword_1C; // 0x1C
    int __dword_20; // 0x20
    int __dword_24; // 0x24
    int __dword_28; // 0x28
    uint8_t __byte_2C; // 0x2C
    uint8_t __byte_2D; // 0x2D
    uint8_t __byte_2E; // 0x2E
    uint8_t __byte_2F; // 0x2F
    int32_t current_power; // 0x30
    int32_t max_power; // 0x34
    int32_t power_per_level; // 0x38
    int __dword_3C; // 0x3C
    int __int_40; // 0x40
    int __dword_44; // 0x44
    int __int_48; // 0x48
    int __dword_4C; // 0x4C
    int __int_50; // 0x50
    int __int_54; // 0x54
    int __dword_58; // 0x58
    int __dword_5C; // 0x5C
    int __dword_60; // 0x60
    int __dword_64; // 0x64
    int __dword_68; // 0x68
    int __dword_6C; // 0x6C
    int __dword_70; // 0x70
    int __dword_74; // 0x74
    int __dword_78; // 0x78
    int __dword_7C; // 0x7C
    int __dword_80; // 0x80
    int __dword_84; // 0x84
    int __dword_88; // 0x88
    int __dword_8C; // 0x8C
    int __dword_90; // 0x90
    int __dword_94; // 0x94
    int __int_98; // 0x98
    int __dword_9C; // 0x9C
    int __dword_A0; // 0xA0
    uint8_t __byte_A4; // 0xA4
    uint8_t __byte_A5; // 0xA5
    padding_bytes(0x2); // 0xA6
    int __dword_A8; // 0xA8
    int __dword_AC; // 0xAC
    uint8_t __byte_B0; // 0xB0
    padding_bytes(0x3); // 0xB1
    int __dword_B4; // 0xB4
    int __int_B8; // 0xB8
    int __dword_BC; // 0xBC
    int __dword_C0; // 0xC0
    int __dword_C4; // 0xC4
    int __dword_C8; // 0xC8
    int __dword_CC; // 0xCC
    int __dword_D0; // 0xD0
    int __int_D4; // 0xD4
    int __dword_D8; // 0xD8
    int __dword_DC; // 0xDC
    int __dword_E0; // 0xE0
    int __dword_E4; // 0xE4
    int __dword_E8; // 0xE8
    int __dword_EC; // 0xEC
    // 0xF0
};

// size: 0x228
struct Globals {
    GlobalsSide side[2]; // 0x0
    int __int_1E0; // 0x1E0
    int __dword_1E4; // 0x1E4
    int __dword_1E8; // 0x1E8
    int __dword_1EC; // 0x1EC
    int __dword_1F0; // 0x1F0
    int __dword_1F4; // 0x1F4
    int __dword_1F8; // 0x1F8
    int __dword_1FC; // 0x1FC
    int __int_200; // 0x200
    int __int_204; // 0x204
    int __dword_208; // 0x208
    int __dword_20C; // 0x20C
    int __dword_210; // 0x210
    int __dword_214; // 0x214
    int __dword_218; // 0x218
    int __dword_21C; // 0x21C
    int __int_220; // 0x220
    padding_bytes(0x4); // 0x224
    // 0x228
};

// size: 0x2C0
struct GameManager {
    GameSide sides[2]; // 0x0
    uint64_t high_score; // 0x60
    uint32_t high_score_continues; // 0x68
    int __dword_6C; // 0x6C
    int __dword_70; // 0x70
    int __dword_74; // 0x74
    int __dword_78; // 0x78
    int __dword_7C; // 0x7C
    int __dword_80; // 0x80
    padding_bytes(0x4); // 0x84
    Globals globals; // 0x88
    double __double_2B0; // 0x2B0
    int __dword_2B8; // 0x2B8
    int __int_2BC; // 0x2BC
    // 0x2C0
};