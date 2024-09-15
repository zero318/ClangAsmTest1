#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <bit>

// Linked list node type
struct AllocNode {
    AllocNode* next;
    AllocNode* prev;
  
    void prepend(AllocNode* new_node) {
        new_node->next = this;
        this->prev = new_node;
    }

    void append(AllocNode* new_node) {
        this->next->prepend(new_node);
        new_node->prepend(this);
    }

    void unlink() {
        AllocNode* next_node = this->next;
        AllocNode* prev_node = this->prev;
        next_node->prev = prev_node;
        prev_node->next = next_node;
    }

    // Realloc can move the address of the
    // current allocation, so make sure that
    // the linked nodes get updated to reference
    // the new address
    void relink() {
        this->next->prev = this;
        this->prev->next = this;
    }
};

enum AllocTickResult {
    SaveData,
    FreeData
};

// Main type for replacing squirrel allocations
struct AllocData {
    AllocNode node;
    size_t size;
    int8_t unload_timer;
    alignas(8) unsigned char data[];

    void initialize(size_t size) {
        this->size = size;
        this->unload_timer = -1;
    }

    void reinitialize(size_t size) {
        this->size = size;
        this->node.relink();
    }

    void start_free(int8_t time) {
        this->unload_timer = time;
    }

    AllocTickResult tick() {
        if (
            !(this->unload_timer < 0) &&
            --this->unload_timer != 0
        ) {
            this->node.unlink();
            // Freeing "this" is a bad idea,
            // so use the return value to let
            // the caller handle it
            return FreeData;
        }
        return SaveData;
    }

    void rollback(size_t frames) {
        if (!(this->unload_timer < 0)) {
            this->unload_timer += frames;
        }
    }
};

// Packed type for storing saved
// data from previous frames
struct SavedAlloc {
    AllocData* ptr;
    size_t size;
    unsigned char data[];
};

// Constants used in various calculations
static constexpr size_t SAVED_FRAMES = 8;
static_assert(std::has_single_bit(SAVED_FRAMES));
static constexpr size_t FRAME_WRAP_MASK = SAVED_FRAMES - 1;

// Type for managing the linked list,
// saving data for previous frames, and
// performing rollback for allocations
struct AllocManager {
    AllocNode dummy_node; // next = head of list, prev = tail of list
    size_t saved_data_index = 0;
    uint8_t* saved_data[SAVED_FRAMES] = {};
    size_t buffer_sizes[SAVED_FRAMES] = {};

    constexpr AllocManager() {
        this->dummy_node.next = &this->dummy_node;
        this->dummy_node.prev = &this->dummy_node;
    }

    size_t increment_index() {
        size_t next_index = this->saved_data_index + 1 & FRAME_WRAP_MASK;
        return this->saved_data_index = next_index;
    }

    size_t rollback_index(size_t frames) {
        size_t next_index = this->saved_data_index - frames & FRAME_WRAP_MASK;
        return this->saved_data_index = next_index;
    }
  
    void append(AllocData* data) {
        this->dummy_node.prev->append(&data->node);
    }

    template<typename L>
    inline void for_each_alloc(const L& lambda) {
        AllocNode* node = this->dummy_node.next;
        for (
            AllocNode* next_node;
            node != &this->dummy_node;
            node = next_node
        ) {
            next_node = node->next;
            lambda((AllocData*)node);
        }
    }

    template<typename L>
    inline void for_each_saved_alloc(size_t index, const L& lambda) {
        uint8_t* buffer = this->saved_data[index];
        uint8_t* buffer_end = buffer + this->buffer_sizes[index];
        while (buffer < buffer_end) {
            SavedAlloc* saved_data = (SavedAlloc*)buffer;
            buffer += sizeof(SavedAlloc) + saved_data->size;
            lambda(saved_data);
        }
    }

    void tick() {
        size_t total_size = 0;
        for_each_alloc([&](AllocData* alloc) {
            switch (alloc->tick()) {
                case SaveData:
                    total_size += sizeof(SavedAlloc) + alloc->size;
                    break;
                case FreeData:
                    free(alloc);
                    break;
            }
        });
        size_t current_index = this->increment_index();
        uint8_t*& current_buffer = this->saved_data[current_index];
        if (total_size > this->buffer_sizes[current_index]) {
            this->buffer_sizes[current_index] = total_size;
            current_buffer = (uint8_t*)realloc(current_buffer, total_size);
        }
        uint8_t* buffer_write = current_buffer;
        for_each_alloc([&](AllocData* alloc) {
            SavedAlloc* saved_data = (SavedAlloc*)buffer_write;
            buffer_write += sizeof(SavedAlloc) + alloc->size;
            saved_data->ptr = alloc;
            saved_data->size = alloc->size;
            memcpy(saved_data->data, alloc->data, alloc->size);
        });
    }

    void rollback(size_t frames) {
        size_t index = this->rollback_index(frames);
        for_each_saved_alloc(index, [](SavedAlloc* saved_data) {
            memcpy(saved_data->ptr->data, saved_data->data, saved_data->size);
        });
    }
};

static AllocManager allocation_manager;

// Main functions for interacting
// with the allocation manager
void tick_allocs() {
    allocation_manager.tick();
}

void rollback_allocs(size_t frames) {
    allocation_manager.rollback(frames);
}

// Hook functions

static inline AllocData* get_alloc_data(void* ptr) {
    return (AllocData*)((uintptr_t)ptr - offsetof(AllocData, data));
}

void* __cdecl malloc_hook(size_t size) {
    AllocData* real_alloc = (AllocData*)malloc(sizeof(AllocData) + size);
    real_alloc->initialize(size);
    allocation_manager.append(real_alloc);
    return &real_alloc->data;
}

void* __cdecl calloc_hook(size_t num, size_t size) {
    size_t total_size = num * size;
    void* ret = malloc_hook(total_size);
    memset(ret, 0, total_size);
    return ret;
}

void __cdecl free_hook(void* ptr) {
    if (ptr) {
        AllocData* real_alloc = get_alloc_data(ptr);
        real_alloc->start_free(SAVED_FRAMES);
    }
}

void* __cdecl realloc_hook(void* ptr, size_t new_size) {
    if (ptr) {
        AllocData* real_alloc = get_alloc_data(ptr);
        if (new_size) {
            AllocData* new_alloc = (AllocData*)realloc(real_alloc, new_size + sizeof(AllocData));
            if (new_alloc) {
                new_alloc->reinitialize(new_size);
            }
        } else {
            real_alloc->start_free(SAVED_FRAMES);
        }
        return NULL;
    }
    return malloc_hook(new_size);
}