/**
 * intervalheap.h
 *
 * Creation Date: 06/07/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * An interval heap is a specialized heap with:
 *
 * - O(1) - Find Min
 * - O(1) - Find Max
 * - O(log n) - Insert
 * - O(log n) - Remove Min
 * - O(log n) - Remove Max
 */

#ifndef CMC_INTERVALHEAP_H
#define CMC_INTERVALHEAP_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_intervalheap = "%s at %p { buffer:%p, capacity:%" PRIuMAX ", size:%" PRIuMAX ", count:%" PRIuMAX ", cmp:%p }";

#define CMC_GENERATE_INTERVALHEAP(PFX, SNAME, V)    \
    CMC_GENERATE_INTERVALHEAP_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_INTERVALHEAP_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_INTERVALHEAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_INTERVALHEAP_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_INTERVALHEAP_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_INTERVALHEAP_SOURCE(PFX, SNAME, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_INTERVALHEAP_HEADER(PFX, SNAME, V)                  \
                                                                         \
    /* Heap Structure */                                                 \
    typedef struct SNAME##_s                                             \
    {                                                                    \
        /* Dynamic array of nodes */                                     \
        struct SNAME##_node_s *buffer;                                   \
                                                                         \
        /* Current array capacity (how many nodes can be stored) */      \
        size_t capacity;                                                 \
                                                                         \
        /* Current amount of nodes in the dynamic array */               \
        size_t size;                                                     \
                                                                         \
        /* Current amount of elements in the heap */                     \
        size_t count;                                                    \
                                                                         \
        /* Element comparison function */                                \
        int (*cmp)(V, V);                                                \
                                                                         \
        /* Function that returns an iterator to the start of the heap */ \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);           \
                                                                         \
        /* Function that returns an iterator to the end of the heap */   \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);             \
                                                                         \
    } SNAME, *SNAME##_ptr;                                               \
                                                                         \
    /* Heap Node */                                                      \
    typedef struct SNAME##_node_s                                        \
    {                                                                    \
        /* 0 - Value belonging to the MinHeap */                         \
        /* 1 - Value belonging to the MaxHeap */                         \
        V data[2];                                                       \
                                                                         \
    } SNAME##_node, *SNAME##_node_ptr;                                   \
                                                                         \
    /* Heap Iterator */                                                  \
    typedef struct SNAME##_iter_s                                        \
    {                                                                    \
        /* Target heap */                                                \
        struct SNAME##_s *target;                                        \
                                                                         \
        /* Cursor's position (index) */                                  \
        size_t cursor;                                                   \
                                                                         \
        /* If the iterator has reached the start of the iteration */     \
        bool start;                                                      \
                                                                         \
        /* If the iterator has reached the end of the iteration */       \
        bool end;                                                        \
                                                                         \
    } SNAME##_iter, *SNAME##_iter_ptr;                                   \
                                                                         \
    /* Collection Functions */                                           \
    /* Collection Allocation and Deallocation */                         \
    SNAME *PFX##_new(size_t capacity, int (*compare)(V, V));             \
    void PFX##_clear(SNAME *_heap_, void (*deallocator)(V));             \
    void PFX##_free(SNAME *_heap_, void (*deallocator)(V));              \
    /* Collection Input and Output */                                    \
    bool PFX##_insert(SNAME *_heap_, V element);                         \
    bool PFX##_remove_max(SNAME *_heap_, V *result);                     \
    bool PFX##_remove_min(SNAME *_heap_, V *result);                     \
    /* Collection Update */                                              \
    bool PFX##_update_max(SNAME *_heap_, V element);                     \
    bool PFX##_update_min(SNAME *_heap_, V element);                     \
    /* Element Access */                                                 \
    bool PFX##_max(SNAME *_heap_, V *value);                             \
    bool PFX##_min(SNAME *_heap_, V *value);                             \
    /* Collection State */                                               \
    bool PFX##_contains(SNAME *_heap_, V element);                       \
    bool PFX##_empty(SNAME *_heap_);                                     \
    bool PFX##_full(SNAME *_heap_);                                      \
    size_t PFX##_count(SNAME *_heap_);                                   \
    size_t PFX##_capacity(SNAME *_heap_);                                \
    /* Collection Utility */                                             \
    bool PFX##_resize(SNAME *_heap_, size_t capacity);                   \
    SNAME *PFX##_copy_of(SNAME *_set_, V (*copy_func)(V));               \
    bool PFX##_equals(SNAME *_heap1_, SNAME *_heap2_);                   \
    cmc_string PFX##_to_string(SNAME *_heap_);                           \
                                                                         \
    /* Iterator Functions */                                             \
    /* Iterator Allocation and Deallocation */                           \
    SNAME##_iter *PFX##_iter_new(SNAME *target);                         \
    void PFX##_iter_free(SNAME##_iter *iter);                            \
    /* Iterator Initialization */                                        \
    void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);             \
    /* Iterator State */                                                 \
    bool PFX##_iter_start(SNAME##_iter *iter);                           \
    bool PFX##_iter_end(SNAME##_iter *iter);                             \
    /* Iterator Movement */                                              \
    void PFX##_iter_to_start(SNAME##_iter *iter);                        \
    void PFX##_iter_to_end(SNAME##_iter *iter);                          \
    bool PFX##_iter_next(SNAME##_iter *iter);                            \
    bool PFX##_iter_prev(SNAME##_iter *iter);                            \
    bool PFX##_iter_advance(SNAME##_iter *iter, size_t steps);           \
    bool PFX##_iter_rewind(SNAME##_iter *iter, size_t steps);            \
    bool PFX##_iter_go_to(SNAME##_iter *iter, size_t index);             \
    /* Iterator Access */                                                \
    V PFX##_iter_value(SNAME##_iter *iter);                              \
    size_t PFX##_iter_index(SNAME##_iter *iter);                         \
                                                                         \
    /* Default Value */                                                  \
    static inline V PFX##_impl_default_value(void)                       \
    {                                                                    \
        V _empty_value_;                                                 \
                                                                         \
        memset(&_empty_value_, 0, sizeof(V));                            \
                                                                         \
        return _empty_value_;                                            \
    }                                                                    \
                                                                         \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_INTERVALHEAP_SOURCE(PFX, SNAME, V)                                           \
                                                                                                  \
    /* Implementation Detail Functions */                                                         \
    static void PFX##_impl_float_up_max(SNAME *_heap_);                                           \
    static void PFX##_impl_float_up_min(SNAME *_heap_);                                           \
    static void PFX##_impl_float_down_max(SNAME *_heap_);                                         \
    static void PFX##_impl_float_down_min(SNAME *_heap_);                                         \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_heap_);                                       \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_heap_);                                         \
                                                                                                  \
    SNAME *PFX##_new(size_t capacity, int (*compare)(V, V))                                       \
    {                                                                                             \
        if (capacity == 0 || capacity == UINT64_MAX)                                              \
            return NULL;                                                                          \
                                                                                                  \
        SNAME *_heap_ = malloc(sizeof(SNAME));                                                    \
                                                                                                  \
        if (!_heap_)                                                                              \
            return NULL;                                                                          \
                                                                                                  \
        /* Since each node can store two elements, divide the actual capacity by 2 */             \
        /* Round the capacity of nodes up */                                                      \
        capacity = capacity % 2 == 0 ? capacity / 2 : (capacity + 1) / 2;                         \
                                                                                                  \
        _heap_->buffer = malloc(sizeof(SNAME##_node) * capacity);                                 \
                                                                                                  \
        if (!_heap_->buffer)                                                                      \
        {                                                                                         \
            free(_heap_);                                                                         \
            return NULL;                                                                          \
        }                                                                                         \
                                                                                                  \
        memset(_heap_->buffer, 0, sizeof(SNAME##_node) * capacity);                               \
                                                                                                  \
        _heap_->capacity = capacity;                                                              \
        _heap_->size = 0;                                                                         \
        _heap_->count = 0;                                                                        \
        _heap_->cmp = compare;                                                                    \
                                                                                                  \
        _heap_->it_start = PFX##_impl_it_start;                                                   \
        _heap_->it_end = PFX##_impl_it_end;                                                       \
                                                                                                  \
        return _heap_;                                                                            \
    }                                                                                             \
                                                                                                  \
    void PFX##_clear(SNAME *_heap_, void (*deallocator)(V))                                       \
    {                                                                                             \
        if (deallocator)                                                                          \
        {                                                                                         \
            for (size_t i = 0; i < _heap_->count; i++)                                            \
            {                                                                                     \
                deallocator(_heap_->buffer[i / 2].data[i % 2]);                                   \
            }                                                                                     \
        }                                                                                         \
                                                                                                  \
        memset(_heap_->buffer, 0, sizeof(V) * _heap_->capacity);                                  \
                                                                                                  \
        _heap_->size = 0;                                                                         \
        _heap_->count = 0;                                                                        \
    }                                                                                             \
                                                                                                  \
    void PFX##_free(SNAME *_heap_, void (*deallocator)(V))                                        \
    {                                                                                             \
        if (deallocator)                                                                          \
        {                                                                                         \
            for (size_t i = 0; i < _heap_->count; i++)                                            \
            {                                                                                     \
                deallocator(_heap_->buffer[i / 2].data[i % 2]);                                   \
            }                                                                                     \
        }                                                                                         \
                                                                                                  \
        free(_heap_->buffer);                                                                     \
                                                                                                  \
        free(_heap_);                                                                             \
    }                                                                                             \
                                                                                                  \
    bool PFX##_insert(SNAME *_heap_, V element)                                                   \
    {                                                                                             \
        if (PFX##_full(_heap_))                                                                   \
        {                                                                                         \
            if (!PFX##_resize(_heap_, PFX##_capacity(_heap_) * 4))                                \
                return false;                                                                     \
        }                                                                                         \
                                                                                                  \
        if (PFX##_count(_heap_) % 2 == 0)                                                         \
        {                                                                                         \
            /* Occupying a new node */                                                            \
            _heap_->buffer[_heap_->size].data[0] = element;                                       \
            _heap_->buffer[_heap_->size].data[1] = PFX##_impl_default_value();                    \
                                                                                                  \
            _heap_->size++;                                                                       \
        }                                                                                         \
        else                                                                                      \
        {                                                                                         \
            SNAME##_node *curr_node = &(_heap_->buffer[_heap_->size - 1]);                        \
                                                                                                  \
            /* Decide if the new element goes into the MinHeap or MaxHeap */                      \
            if (_heap_->cmp(curr_node->data[0], element) > 0)                                     \
            {                                                                                     \
                /* Swap current value and add new element to the MinHeap */                       \
                curr_node->data[1] = curr_node->data[0];                                          \
                curr_node->data[0] = element;                                                     \
            }                                                                                     \
            else                                                                                  \
            {                                                                                     \
                /* No need to swap and add the new element to the MaxHeap */                      \
                curr_node->data[1] = element;                                                     \
            }                                                                                     \
        }                                                                                         \
                                                                                                  \
        _heap_->count++;                                                                          \
                                                                                                  \
        if (PFX##_count(_heap_) <= 2)                                                             \
            return true;                                                                          \
                                                                                                  \
        /* Determine wheather to do a MaxHeap insert or a MinHeap insert */                       \
        SNAME##_node *parent = &(_heap_->buffer[(_heap_->size - 1) / 2]);                         \
                                                                                                  \
        if (_heap_->cmp(parent->data[0], element) > 0)                                            \
            PFX##_impl_float_up_min(_heap_);                                                      \
        else if (_heap_->cmp(parent->data[1], element) < 0)                                       \
            PFX##_impl_float_up_max(_heap_);                                                      \
        /* else no float up required */                                                           \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_remove_max(SNAME *_heap_, V *result)                                               \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return false;                                                                         \
                                                                                                  \
        if (PFX##_count(_heap_) == 1)                                                             \
        {                                                                                         \
            *result = _heap_->buffer[0].data[0];                                                  \
                                                                                                  \
            _heap_->buffer[0].data[0] = PFX##_impl_default_value();                               \
                                                                                                  \
            _heap_->count--;                                                                      \
                                                                                                  \
            return true;                                                                          \
        }                                                                                         \
        else                                                                                      \
            *result = _heap_->buffer[0].data[1];                                                  \
                                                                                                  \
        /* Swap the result with the last element in the MaxHeap and float it down */              \
        SNAME##_node *last_node = &(_heap_->buffer[_heap_->size - 1]);                            \
                                                                                                  \
        if (PFX##_count(_heap_) % 2 == 1)                                                         \
        {                                                                                         \
            /* Grab from MinHeap and discard last node */                                         \
            _heap_->buffer[0].data[1] = last_node->data[0];                                       \
                                                                                                  \
            last_node->data[0] = PFX##_impl_default_value();                                      \
                                                                                                  \
            _heap_->size--;                                                                       \
        }                                                                                         \
        else                                                                                      \
        {                                                                                         \
            _heap_->buffer[0].data[1] = last_node->data[1];                                       \
                                                                                                  \
            last_node->data[1] = PFX##_impl_default_value();                                      \
        }                                                                                         \
                                                                                                  \
        _heap_->count--;                                                                          \
                                                                                                  \
        /* FLoat Down on the MaxHeap */                                                           \
        PFX##_impl_float_down_max(_heap_);                                                        \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_remove_min(SNAME *_heap_, V *result)                                               \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return false;                                                                         \
                                                                                                  \
        *result = _heap_->buffer[0].data[0];                                                      \
                                                                                                  \
        if (PFX##_count(_heap_) == 1)                                                             \
        {                                                                                         \
            _heap_->buffer[0].data[0] = PFX##_impl_default_value();                               \
                                                                                                  \
            _heap_->count--;                                                                      \
                                                                                                  \
            return true;                                                                          \
        }                                                                                         \
                                                                                                  \
        /* Swap the result with the last element in the MinHeap and float it down */              \
        SNAME##_node *last_node = &(_heap_->buffer[_heap_->size - 1]);                            \
                                                                                                  \
        _heap_->buffer[0].data[0] = last_node->data[0];                                           \
                                                                                                  \
        if (PFX##_count(_heap_) % 2 == 1)                                                         \
        {                                                                                         \
            /* Discard last node */                                                               \
            last_node->data[0] = PFX##_impl_default_value();                                      \
                                                                                                  \
            _heap_->size--;                                                                       \
        }                                                                                         \
        else                                                                                      \
        {                                                                                         \
            /* Place the MaxHeap value in the MinHeap value spot */                               \
            last_node->data[0] = last_node->data[1];                                              \
            last_node->data[1] = PFX##_impl_default_value();                                      \
        }                                                                                         \
                                                                                                  \
        _heap_->count--;                                                                          \
                                                                                                  \
        /* FLoat Down on the MinHeap */                                                           \
        PFX##_impl_float_down_min(_heap_);                                                        \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_update_max(SNAME *_heap_, V element)                                               \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return false;                                                                         \
                                                                                                  \
        if (PFX##_count(_heap_) == 1)                                                             \
        {                                                                                         \
            _heap_->buffer[0].data[0] = element;                                                  \
        }                                                                                         \
        else if (_heap_->cmp(element, _heap_->buffer[0].data[0]) < 0)                             \
        {                                                                                         \
            /* Corner case: we are updating the Max value but it is less than */                  \
            /* the Min value */                                                                   \
            _heap_->buffer[0].data[1] = _heap_->buffer[0].data[0];                                \
            _heap_->buffer[0].data[0] = element;                                                  \
                                                                                                  \
            PFX##_impl_float_down_max(_heap_);                                                    \
        }                                                                                         \
        else                                                                                      \
        {                                                                                         \
            /* Update Max element and float it down */                                            \
            _heap_->buffer[0].data[1] = element;                                                  \
                                                                                                  \
            PFX##_impl_float_down_max(_heap_);                                                    \
        }                                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_update_min(SNAME *_heap_, V element)                                               \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return false;                                                                         \
                                                                                                  \
        if (PFX##_count(_heap_) == 1)                                                             \
        {                                                                                         \
            _heap_->buffer[0].data[0] = element;                                                  \
        }                                                                                         \
        else if (_heap_->cmp(element, _heap_->buffer[0].data[1]) > 0)                             \
        {                                                                                         \
            /* Corner case: we are updating the Min value but it is greater */                    \
            /* than the Max value. */                                                             \
            _heap_->buffer[0].data[0] = _heap_->buffer[0].data[1];                                \
            _heap_->buffer[0].data[1] = element;                                                  \
                                                                                                  \
            PFX##_impl_float_down_min(_heap_);                                                    \
        }                                                                                         \
        else                                                                                      \
        {                                                                                         \
            /* Update Min element and float it down */                                            \
            _heap_->buffer[0].data[0] = element;                                                  \
                                                                                                  \
            PFX##_impl_float_down_min(_heap_);                                                    \
        }                                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_max(SNAME *_heap_, V *value)                                                       \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return false;                                                                         \
                                                                                                  \
        /* If there is only one element, then the maximum element is the same */                  \
        /* as the one in the MinHeap */                                                           \
        if (PFX##_count(_heap_) == 1)                                                             \
            *value = _heap_->buffer[0].data[0];                                                   \
        else                                                                                      \
            *value = _heap_->buffer[0].data[1];                                                   \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_min(SNAME *_heap_, V *value)                                                       \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return false;                                                                         \
                                                                                                  \
        *value = _heap_->buffer[0].data[0];                                                       \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_contains(SNAME *_heap_, V element)                                                 \
    {                                                                                             \
        for (size_t i = 0; i < _heap_->count; i++)                                                \
        {                                                                                         \
            if (_heap_->cmp(_heap_->buffer[i / 2].data[i % 2], element) == 0)                     \
                return true;                                                                      \
        }                                                                                         \
                                                                                                  \
        return false;                                                                             \
    }                                                                                             \
                                                                                                  \
    bool PFX##_empty(SNAME *_heap_)                                                               \
    {                                                                                             \
        return _heap_->count == 0;                                                                \
    }                                                                                             \
                                                                                                  \
    bool PFX##_full(SNAME *_heap_)                                                                \
    {                                                                                             \
        /* The heap is full if all nodes are completely filled */                                 \
        return _heap_->size >= _heap_->capacity && _heap_->count % 2 == 0;                        \
    }                                                                                             \
                                                                                                  \
    size_t PFX##_count(SNAME *_heap_)                                                             \
    {                                                                                             \
        return _heap_->count;                                                                     \
    }                                                                                             \
                                                                                                  \
    size_t PFX##_capacity(SNAME *_heap_)                                                          \
    {                                                                                             \
        return _heap_->capacity;                                                                  \
    }                                                                                             \
                                                                                                  \
    bool PFX##_resize(SNAME *_heap_, size_t capacity)                                             \
    {                                                                                             \
        if (PFX##_capacity(_heap_) == capacity)                                                   \
            return true;                                                                          \
                                                                                                  \
        if (capacity < PFX##_count(_heap_))                                                       \
            return false;                                                                         \
                                                                                                  \
        capacity = capacity % 2 == 0 ? capacity / 2 : (capacity + 1) / 2;                         \
                                                                                                  \
        SNAME##_node *new_buffer = realloc(_heap_->buffer, sizeof(SNAME##_node) * capacity);      \
                                                                                                  \
        if (!new_buffer)                                                                          \
            return false;                                                                         \
                                                                                                  \
        memset(new_buffer + _heap_->capacity, 0, sizeof(SNAME##_node) * _heap_->capacity);        \
                                                                                                  \
        _heap_->buffer = new_buffer;                                                              \
        _heap_->capacity = capacity;                                                              \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    SNAME *PFX##_copy_of(SNAME *_heap_, V (*copy_func)(V))                                        \
    {                                                                                             \
        SNAME *result = malloc(sizeof(SNAME));                                                    \
                                                                                                  \
        if (!result)                                                                              \
            return NULL;                                                                          \
                                                                                                  \
        memcpy(result, _heap_, sizeof(SNAME));                                                    \
                                                                                                  \
        result->buffer = malloc(sizeof(SNAME##_node) * _heap_->capacity);                         \
                                                                                                  \
        if (!result->buffer)                                                                      \
        {                                                                                         \
            free(result);                                                                         \
            return NULL;                                                                          \
        }                                                                                         \
                                                                                                  \
        if (copy_func)                                                                            \
        {                                                                                         \
            for (size_t i = 0; i < _heap_->count; i++)                                            \
                result->buffer[i / 2].data[i % 2] = copy_func(_heap_->buffer[i / 2].data[i % 2]); \
        }                                                                                         \
        else                                                                                      \
            memcpy(result->buffer, _heap_->buffer, sizeof(SNAME##_node) * _heap_->capacity);      \
                                                                                                  \
        return result;                                                                            \
    }                                                                                             \
                                                                                                  \
    bool PFX##_equals(SNAME *_heap1_, SNAME *_heap2_)                                             \
    {                                                                                             \
        if (PFX##_count(_heap1_) != PFX##_count(_heap2_))                                         \
            return false;                                                                         \
                                                                                                  \
        for (size_t i = 0; i < _heap1_->count; i++)                                               \
        {                                                                                         \
            V element1 = _heap1_->buffer[i / 2].data[i % 2];                                      \
            V element2 = _heap2_->buffer[i / 2].data[i % 2];                                      \
                                                                                                  \
            if (_heap1_->cmp(element1, element2) == 0)                                            \
                return true;                                                                      \
        }                                                                                         \
                                                                                                  \
        return false;                                                                             \
    }                                                                                             \
                                                                                                  \
    cmc_string PFX##_to_string(SNAME *_heap_)                                                     \
    {                                                                                             \
        cmc_string str;                                                                           \
        SNAME *h_ = _heap_;                                                                       \
        const char *name = #SNAME;                                                                \
                                                                                                  \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_intervalheap,                              \
                 name, h_, h_->buffer, h_->capacity, h_->size, h_->count, h_->cmp);               \
                                                                                                  \
        return str;                                                                               \
    }                                                                                             \
                                                                                                  \
    SNAME##_iter *PFX##_iter_new(SNAME *target)                                                   \
    {                                                                                             \
        SNAME##_iter *iter = malloc(sizeof(SNAME##_iter));                                        \
                                                                                                  \
        if (!iter)                                                                                \
            return NULL;                                                                          \
                                                                                                  \
        PFX##_iter_init(iter, target);                                                            \
                                                                                                  \
        return iter;                                                                              \
    }                                                                                             \
                                                                                                  \
    void PFX##_iter_free(SNAME##_iter *iter)                                                      \
    {                                                                                             \
        free(iter);                                                                               \
    }                                                                                             \
                                                                                                  \
    void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                                       \
    {                                                                                             \
        iter->target = target;                                                                    \
        iter->cursor = 0;                                                                         \
        iter->start = true;                                                                       \
        iter->end = PFX##_empty(target);                                                          \
    }                                                                                             \
                                                                                                  \
    bool PFX##_iter_start(SNAME##_iter *iter)                                                     \
    {                                                                                             \
        return PFX##_empty(iter->target) || iter->start;                                          \
    }                                                                                             \
                                                                                                  \
    bool PFX##_iter_end(SNAME##_iter *iter)                                                       \
    {                                                                                             \
        return PFX##_empty(iter->target) || iter->end;                                            \
    }                                                                                             \
                                                                                                  \
    void PFX##_iter_to_start(SNAME##_iter *iter)                                                  \
    {                                                                                             \
        if (!PFX##_empty(iter->target))                                                           \
        {                                                                                         \
            iter->cursor = 0;                                                                     \
            iter->start = true;                                                                   \
            iter->end = PFX##_empty(iter->target);                                                \
        }                                                                                         \
    }                                                                                             \
                                                                                                  \
    void PFX##_iter_to_end(SNAME##_iter *iter)                                                    \
    {                                                                                             \
        if (!PFX##_empty(iter->target))                                                           \
        {                                                                                         \
            iter->cursor = iter->target->count - 1;                                               \
            iter->start = PFX##_empty(iter->target);                                              \
            iter->end = true;                                                                     \
        }                                                                                         \
    }                                                                                             \
                                                                                                  \
    bool PFX##_iter_next(SNAME##_iter *iter)                                                      \
    {                                                                                             \
        if (iter->end)                                                                            \
            return false;                                                                         \
                                                                                                  \
        if (iter->cursor + 1 == PFX##_count(iter->target))                                        \
        {                                                                                         \
            iter->end = true;                                                                     \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        iter->start = PFX##_empty(iter->target);                                                  \
                                                                                                  \
        iter->cursor++;                                                                           \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_iter_prev(SNAME##_iter *iter)                                                      \
    {                                                                                             \
        if (iter->start)                                                                          \
            return false;                                                                         \
                                                                                                  \
        if (iter->cursor == 0)                                                                    \
        {                                                                                         \
            iter->start = true;                                                                   \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        iter->end = PFX##_empty(iter->target);                                                    \
                                                                                                  \
        iter->cursor--;                                                                           \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    /* Returns true only if the iterator moved */                                                 \
    bool PFX##_iter_advance(SNAME##_iter *iter, size_t steps)                                     \
    {                                                                                             \
        if (iter->start)                                                                          \
            return false;                                                                         \
                                                                                                  \
        if (iter->cursor == 0)                                                                    \
        {                                                                                         \
            iter->start = true;                                                                   \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        if (steps == 0 || iter->cursor + steps >= PFX##_count(iter->target))                      \
            return false;                                                                         \
                                                                                                  \
        iter->start = PFX##_empty(iter->target);                                                  \
                                                                                                  \
        iter->cursor += steps;                                                                    \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    /* Returns true only if the iterator moved */                                                 \
    bool PFX##_iter_rewind(SNAME##_iter *iter, size_t steps)                                      \
    {                                                                                             \
        if (iter->start)                                                                          \
            return false;                                                                         \
                                                                                                  \
        if (iter->cursor == 0)                                                                    \
        {                                                                                         \
            iter->start = true;                                                                   \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        if (steps == 0 || iter->cursor < steps)                                                   \
            return false;                                                                         \
                                                                                                  \
        iter->end = PFX##_empty(iter->target);                                                    \
                                                                                                  \
        iter->cursor -= steps;                                                                    \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    /* Returns true only if the iterator was able to be positioned at the given index */          \
    bool PFX##_iter_go_to(SNAME##_iter *iter, size_t index)                                       \
    {                                                                                             \
        if (index >= PFX##_count(iter->target))                                                   \
            return false;                                                                         \
                                                                                                  \
        if (iter->cursor > index)                                                                 \
            return PFX##_iter_rewind(iter, iter->cursor - index);                                 \
        else if (iter->cursor < index)                                                            \
            return PFX##_iter_advance(iter, index - iter->cursor);                                \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    V PFX##_iter_value(SNAME##_iter *iter)                                                        \
    {                                                                                             \
        if (PFX##_empty(iter->target))                                                            \
            return PFX##_impl_default_value();                                                    \
                                                                                                  \
        return iter->target->buffer[iter->cursor / 2].data[iter->cursor % 2];                     \
    }                                                                                             \
                                                                                                  \
    size_t PFX##_iter_index(SNAME##_iter *iter)                                                   \
    {                                                                                             \
        return iter->cursor;                                                                      \
    }                                                                                             \
                                                                                                  \
    static void PFX##_impl_float_up_max(SNAME *_heap_)                                            \
    {                                                                                             \
        size_t index = _heap_->size - 1;                                                          \
                                                                                                  \
        SNAME##_node *curr_node = &(_heap_->buffer[index]);                                       \
                                                                                                  \
        /* FLoat Up on the MaxHeap */                                                             \
        while (index > 0)                                                                         \
        {                                                                                         \
            /* Parent index */                                                                    \
            size_t P_index = (index - 1) / 2;                                                     \
                                                                                                  \
            SNAME##_node *parent = &(_heap_->buffer[P_index]);                                    \
                                                                                                  \
            if (index == _heap_->size - 1 && PFX##_count(_heap_) % 2 != 0)                        \
            {                                                                                     \
                /* In this case, the current node has no MaxHeap value so we */                   \
                /* instead compare with the MinHeap value */                                      \
                if (_heap_->cmp(curr_node->data[0], parent->data[1]) < 0)                         \
                    break;                                                                        \
                                                                                                  \
                /* Since the comparison above passed now we need to swap the */                   \
                /* current node with the parent but it will have to be done */                    \
                /* with the node's MinHeap value since the MaxHeap value */                       \
                /* doesn't exist */                                                               \
                V tmp = curr_node->data[0];                                                       \
                curr_node->data[0] = parent->data[1];                                             \
                parent->data[1] = tmp;                                                            \
            }                                                                                     \
            else                                                                                  \
            {                                                                                     \
                /* Usual case, just compare both MaxHeap values */                                \
                if (_heap_->cmp(curr_node->data[1], parent->data[1]) < 0)                         \
                    break;                                                                        \
                                                                                                  \
                /* Swap with parent and repeat */                                                 \
                V tmp = curr_node->data[1];                                                       \
                curr_node->data[1] = parent->data[1];                                             \
                parent->data[1] = tmp;                                                            \
            }                                                                                     \
                                                                                                  \
            /* Update indexe and node */                                                          \
            index = P_index;                                                                      \
            curr_node = parent;                                                                   \
        }                                                                                         \
    }                                                                                             \
                                                                                                  \
    static void PFX##_impl_float_up_min(SNAME *_heap_)                                            \
    {                                                                                             \
        size_t index = _heap_->size - 1;                                                          \
                                                                                                  \
        SNAME##_node *curr_node = &(_heap_->buffer[index]);                                       \
                                                                                                  \
        /* FLoat Up on the MinHeap */                                                             \
        while (index > 0)                                                                         \
        {                                                                                         \
            /* Parent index */                                                                    \
            size_t P_index = (index - 1) / 2;                                                     \
                                                                                                  \
            SNAME##_node *parent = &(_heap_->buffer[P_index]);                                    \
                                                                                                  \
            if (_heap_->cmp(curr_node->data[0], parent->data[0]) >= 0)                            \
                break;                                                                            \
                                                                                                  \
            /* Swap with parent and repeat */                                                     \
            V tmp = curr_node->data[0];                                                           \
            curr_node->data[0] = parent->data[0];                                                 \
            parent->data[0] = tmp;                                                                \
                                                                                                  \
            /* Update indexe and node */                                                          \
            index = P_index;                                                                      \
            curr_node = parent;                                                                   \
        }                                                                                         \
    }                                                                                             \
                                                                                                  \
    static void PFX##_impl_float_down_max(SNAME *_heap_)                                          \
    {                                                                                             \
        /* Floats Down from the MinHeap */                                                        \
        size_t index = 0;                                                                         \
                                                                                                  \
        SNAME##_node *curr_node = &(_heap_->buffer[index]);                                       \
                                                                                                  \
        while (true)                                                                              \
        {                                                                                         \
            /* If there are no children it is done */                                             \
            if (2 * index + 1 >= _heap_->size)                                                    \
                break;                                                                            \
                                                                                                  \
            /* Child index to be compared with current value */                                   \
            size_t child;                                                                         \
            size_t L_index = index * 2 + 1;                                                       \
            size_t R_index = index * 2 + 2;                                                       \
                                                                                                  \
            /* If there are two children, pick the smallest one */                                \
            if (R_index < _heap_->size)                                                           \
            {                                                                                     \
                SNAME##_node *L = &(_heap_->buffer[L_index]);                                     \
                SNAME##_node *R = &(_heap_->buffer[R_index]);                                     \
                                                                                                  \
                /* If the right child is the last node and there is no MaxHeap value */           \
                /* then do the comparison with the MinHeap value */                               \
                if (R_index == _heap_->size - 1 && PFX##_count(_heap_) % 2 != 0)                  \
                    child = _heap_->cmp(L->data[1], R->data[0]) > 0 ? L_index : R_index;          \
                else                                                                              \
                    child = _heap_->cmp(L->data[1], R->data[1]) > 0 ? L_index : R_index;          \
            }                                                                                     \
            /* Pick the only one available */                                                     \
            else                                                                                  \
                child = L_index;                                                                  \
                                                                                                  \
            SNAME##_node *child_node = &(_heap_->buffer[child]);                                  \
                                                                                                  \
            /* Again, check if the child node is the last one and has no MaxHeap value */         \
            if (child == _heap_->size - 1 && PFX##_count(_heap_) % 2 != 0)                        \
            {                                                                                     \
                /* Odd case, compare with MinHeap value */                                        \
                /* If current value is not less than the child node's value, it is done */        \
                if (_heap_->cmp(curr_node->data[1], child_node->data[0]) >= 0)                    \
                    break;                                                                        \
                                                                                                  \
                /* Otherwise swap and continue */                                                 \
                V tmp = child_node->data[0];                                                      \
                child_node->data[0] = curr_node->data[1];                                         \
                curr_node->data[1] = tmp;                                                         \
            }                                                                                     \
            else                                                                                  \
            {                                                                                     \
                /* If current value is not less than the child node's value, it is done */        \
                if (_heap_->cmp(curr_node->data[1], child_node->data[1]) >= 0)                    \
                    break;                                                                        \
                                                                                                  \
                /* Otherwise swap and continue */                                                 \
                V tmp = child_node->data[1];                                                      \
                child_node->data[1] = curr_node->data[1];                                         \
                curr_node->data[1] = tmp;                                                         \
                                                                                                  \
                /* Check if the MinHeap and MaxHeap values need to be swapped */                  \
                if (_heap_->cmp(child_node->data[0], child_node->data[1]) > 0)                    \
                {                                                                                 \
                    /* Swap because the MinHeap value is greater than the MaxHeap value */        \
                    tmp = child_node->data[0];                                                    \
                    child_node->data[0] = child_node->data[1];                                    \
                    child_node->data[1] = tmp;                                                    \
                }                                                                                 \
            }                                                                                     \
                                                                                                  \
            /* Update index and node */                                                           \
            index = child;                                                                        \
            curr_node = child_node;                                                               \
        }                                                                                         \
    }                                                                                             \
                                                                                                  \
    static void PFX##_impl_float_down_min(SNAME *_heap_)                                          \
    {                                                                                             \
        /* Floats Down from the MinHeap */                                                        \
        size_t index = 0;                                                                         \
                                                                                                  \
        SNAME##_node *curr_node = &(_heap_->buffer[index]);                                       \
                                                                                                  \
        while (true)                                                                              \
        {                                                                                         \
            /* If there are no children it is done */                                             \
            if (2 * index + 1 >= _heap_->size)                                                    \
                break;                                                                            \
                                                                                                  \
            /* Child index to be compared with current value */                                   \
            size_t child;                                                                         \
            size_t L_index = index * 2 + 1;                                                       \
            size_t R_index = index * 2 + 2;                                                       \
                                                                                                  \
            /* If there are two children, pick the smallest one */                                \
            if (R_index < _heap_->size)                                                           \
            {                                                                                     \
                SNAME##_node *L = &(_heap_->buffer[L_index]);                                     \
                SNAME##_node *R = &(_heap_->buffer[R_index]);                                     \
                                                                                                  \
                child = _heap_->cmp(L->data[0], R->data[0]) < 0 ? L_index : R_index;              \
            }                                                                                     \
            /* Pick the only one available */                                                     \
            else                                                                                  \
                child = L_index;                                                                  \
                                                                                                  \
            SNAME##_node *child_node = &(_heap_->buffer[child]);                                  \
                                                                                                  \
            /* If current value is smaller than the child node's value, it is done */             \
            if (_heap_->cmp(curr_node->data[0], child_node->data[0]) < 0)                         \
                break;                                                                            \
                                                                                                  \
            /* Otherwise swap and continue */                                                     \
            V tmp = child_node->data[0];                                                          \
            child_node->data[0] = curr_node->data[0];                                             \
            curr_node->data[0] = tmp;                                                             \
                                                                                                  \
            /* If the child node is the last node check if the MinHeap and */                     \
            /* MaxHeap values need to be swapped */                                               \
            if (child != _heap_->size - 1 || PFX##_count(_heap_) % 2 == 0)                        \
            {                                                                                     \
                if (_heap_->cmp(child_node->data[0], child_node->data[1]) > 0)                    \
                {                                                                                 \
                    /* Swap because the MinHeap value is greater than the MaxHeap value */        \
                    tmp = child_node->data[0];                                                    \
                    child_node->data[0] = child_node->data[1];                                    \
                    child_node->data[1] = tmp;                                                    \
                }                                                                                 \
            }                                                                                     \
                                                                                                  \
            /* Update index and node */                                                           \
            index = child;                                                                        \
            curr_node = child_node;                                                               \
        }                                                                                         \
    }                                                                                             \
                                                                                                  \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_heap_)                                        \
    {                                                                                             \
        SNAME##_iter iter;                                                                        \
                                                                                                  \
        PFX##_iter_init(&iter, _heap_);                                                           \
        PFX##_iter_to_start(&iter);                                                               \
                                                                                                  \
        return iter;                                                                              \
    }                                                                                             \
                                                                                                  \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_heap_)                                          \
    {                                                                                             \
        SNAME##_iter iter;                                                                        \
                                                                                                  \
        PFX##_iter_init(&iter, _heap_);                                                           \
        PFX##_iter_to_end(&iter);                                                                 \
                                                                                                  \
        return iter;                                                                              \
    }

#endif /* CMC_INTERVALHEAP_H */
