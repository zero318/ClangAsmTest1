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

    void unlink();
    
    void unlink_impl();
};

template <typename T>
struct ZUNListIter {
    ZUNList<T>* current; // 0x0
    ZUNList<T>* next; // 0x4
    // 0x8
    
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
    
    ZUNListIter<T>& begin() {
        this->iter.set(this->get_next());
        return this->iter;
    }
    ZUNList<T>* end() {
        return NULL;
    }

    void unlink_node(ZUNList<T>* node) {
        if (this->head == node) {
            this->head = node->get_prev();
        }
        node->unlink_impl();
    }
};

template <typename T>
void ZUNList<T>::unlink() {
    if (this->list) {
        ZUNListIterable<T>* list = this->list;
        list->unlink_node(this);
    } else {
        this->unlink_impl();
    }
}

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