/**
 * heap.h
 *
 * Creation Date: 25/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Heap
 *
 * The Heap is a specialized tree-based data structure which is essentially a
 * complete tree that satisfies the heap property:
 *     - MaxHeap:
 *         - The root node is the greatest element of the unique set of keys.
 *         - A node key is always greater than or equal to its children keys.
 *     - Minheap:
 *         - The root node is the smallest element of the unique set of keys.
 *         - A node Key is always greater than or equal to its children keys.
 *
 * The heap is mostly used to implement priority queues. Sometimes it can also
 * be used to sort elements. There are three main functions:
 *     - insert : Adds an element to the heap
 *     - remove_(min/max): Removes the min/max element from the heap
 *     - min/max : Accesses the min/max element from the heap
 */

#ifndef CMC_HEAP_H
#define CMC_HEAP_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

typedef enum cmc_heap_order_e
{
    cmc_max_heap = 1,
    cmc_min_heap = -1
} cmc_heap_order;

#define CMC_GENERATE_HEAP(PFX, SNAME, V)    \
    CMC_GENERATE_HEAP_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_HEAP_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_HEAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_HEAP_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_HEAP_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_HEAP_SOURCE(PFX, SNAME, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_HEAP_HEADER(PFX, SNAME, V)                                 \
                                                                                \
    /* Heap Structure */                                                        \
    typedef struct SNAME##_s                                                    \
    {                                                                           \
        /* Dynamic array of elements */                                         \
        V *buffer;                                                              \
                                                                                \
        /* Current array capacity */                                            \
        size_t capacity;                                                        \
                                                                                \
        /* Current amount of elements in the heap */                            \
        size_t count;                                                           \
                                                                                \
        /* Heap order (MaxHeap or MinHeap) */                                   \
        enum cmc_heap_order_e HO;                                               \
                                                                                \
        /* Element comparison function */                                       \
        int (*cmp)(V, V);                                                       \
                                                                                \
        /* Function that returns an iterator to the start of the heap */        \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                  \
                                                                                \
        /* Function that returns an iterator to the end of the heap */          \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                    \
                                                                                \
    } SNAME, *SNAME##_ptr;                                                      \
                                                                                \
    /* Heap Iterator */                                                         \
    typedef struct SNAME##_iter_s                                               \
    {                                                                           \
        /* Target heap */                                                       \
        struct SNAME##_s *target;                                               \
                                                                                \
        /* Cursor's position (index) */                                         \
        size_t cursor;                                                          \
                                                                                \
        /* If the iterator has reached the start of the iteration */            \
        bool start;                                                             \
                                                                                \
        /* If the iterator has reached the end of the iteration */              \
        bool end;                                                               \
                                                                                \
    } SNAME##_iter, *SNAME##_iter_ptr;                                          \
                                                                                \
    /* Collection Functions */                                                  \
    /* Collection Allocation and Deallocation */                                \
    SNAME *PFX##_new(size_t capacity, cmc_heap_order HO, int (*compare)(V, V)); \
    void PFX##_clear(SNAME *_heap_, void (*deallocator)(V));                    \
    void PFX##_free(SNAME *_heap_, void (*deallocator)(V));                     \
    /* Collection Input and Output */                                           \
    bool PFX##_insert(SNAME *_heap_, V element);                                \
    bool PFX##_remove(SNAME *_heap_, V *result);                                \
    /* Conditional Input and Output */                                          \
    bool PFX##_insert_if(SNAME *_heap_, V element, bool condition);             \
    bool PFX##_remove_if(SNAME *_heap_, V *result, bool condition);             \
    /* Element Access */                                                        \
    V PFX##_peek(SNAME *_heap_);                                                \
    V *PFX##_peek_ref(SNAME *_heap_);                                           \
    /* Collection State */                                                      \
    bool PFX##_contains(SNAME *_heap_, V element);                              \
    bool PFX##_empty(SNAME *_heap_);                                            \
    bool PFX##_full(SNAME *_heap_);                                             \
    size_t PFX##_count(SNAME *_heap_);                                          \
    size_t PFX##_capacity(SNAME *_heap_);                                       \
    /* Collection Utility */                                                    \
    SNAME *PFX##_copy_of(SNAME *_heap_, V (*copy_func)(V));                     \
    bool PFX##_equals(SNAME *_heap1_, SNAME *_heap2_);                          \
    cmc_string PFX##_to_string(SNAME *_heap_);                                  \
                                                                                \
    /* Iterator Functions */                                                    \
    /* Iterator Allocation and Deallocation */                                  \
    SNAME##_iter *PFX##_iter_new(SNAME *target);                                \
    void PFX##_iter_free(SNAME##_iter *iter);                                   \
    /* Iterator Initialization */                                               \
    void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                    \
    /* Iterator State */                                                        \
    bool PFX##_iter_start(SNAME##_iter *iter);                                  \
    bool PFX##_iter_end(SNAME##_iter *iter);                                    \
    /* Iterator Movement */                                                     \
    void PFX##_iter_to_start(SNAME##_iter *iter);                               \
    void PFX##_iter_to_end(SNAME##_iter *iter);                                 \
    bool PFX##_iter_next(SNAME##_iter *iter);                                   \
    bool PFX##_iter_prev(SNAME##_iter *iter);                                   \
    bool PFX##_iter_advance(SNAME##_iter *iter, size_t steps);                  \
    bool PFX##_iter_rewind(SNAME##_iter *iter, size_t steps);                   \
    bool PFX##_iter_go_to(SNAME##_iter *iter, size_t index);                    \
    /* Iterator Access */                                                       \
    V PFX##_iter_value(SNAME##_iter *iter);                                     \
    size_t PFX##_iter_index(SNAME##_iter *iter);                                \
                                                                                \
    /* Default Value */                                                         \
    static inline V PFX##_impl_default_value(void)                              \
    {                                                                           \
        V _empty_value_;                                                        \
                                                                                \
        memset(&_empty_value_, 0, sizeof(V));                                   \
                                                                                \
        return _empty_value_;                                                   \
    }                                                                           \
                                                                                \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_HEAP_SOURCE(PFX, SNAME, V)                                                   \
                                                                                                  \
    /* Implementation Detail Functions */                                                         \
    static bool PFX##_impl_grow(SNAME *_heap_);                                                   \
    static bool PFX##_impl_float_up(SNAME *_heap_, size_t index);                                 \
    static bool PFX##_impl_float_down(SNAME *_heap_, size_t index);                               \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_heap_);                                       \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_heap_);                                         \
                                                                                                  \
    SNAME *PFX##_new(size_t capacity, cmc_heap_order HO, int (*compare)(V, V))                    \
    {                                                                                             \
        if (capacity < 1)                                                                         \
            return NULL;                                                                          \
                                                                                                  \
        if (HO != cmc_min_heap && HO != cmc_max_heap)                                             \
            return NULL;                                                                          \
                                                                                                  \
        SNAME *_heap_ = malloc(sizeof(SNAME));                                                    \
                                                                                                  \
        if (!_heap_)                                                                              \
            return NULL;                                                                          \
                                                                                                  \
        _heap_->buffer = malloc(sizeof(V) * capacity);                                            \
                                                                                                  \
        if (!_heap_->buffer)                                                                      \
        {                                                                                         \
            free(_heap_);                                                                         \
            return NULL;                                                                          \
        }                                                                                         \
                                                                                                  \
        memset(_heap_->buffer, 0, sizeof(V) * capacity);                                          \
                                                                                                  \
        _heap_->capacity = capacity;                                                              \
        _heap_->count = 0;                                                                        \
        _heap_->HO = HO;                                                                          \
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
                deallocator(_heap_->buffer[i]);                                                   \
            }                                                                                     \
        }                                                                                         \
                                                                                                  \
        memset(_heap_->buffer, 0, sizeof(V) * _heap_->capacity);                                  \
                                                                                                  \
        _heap_->count = 0;                                                                        \
    }                                                                                             \
                                                                                                  \
    void PFX##_free(SNAME *_heap_, void (*deallocator)(V))                                        \
    {                                                                                             \
        if (deallocator)                                                                          \
        {                                                                                         \
            for (size_t i = 0; i < _heap_->count; i++)                                            \
            {                                                                                     \
                deallocator(_heap_->buffer[i]);                                                   \
            }                                                                                     \
        }                                                                                         \
                                                                                                  \
        free(_heap_->buffer);                                                                     \
        free(_heap_);                                                                             \
    }                                                                                             \
                                                                                                  \
    bool PFX##_insert(SNAME *_heap_, V element)                                                   \
    {                                                                                             \
        if (PFX##_full(_heap_))                                                                   \
        {                                                                                         \
            if (!PFX##_impl_grow(_heap_))                                                         \
                return false;                                                                     \
        }                                                                                         \
                                                                                                  \
        if (PFX##_empty(_heap_))                                                                  \
        {                                                                                         \
            _heap_->buffer[_heap_->count++] = element;                                            \
            return true;                                                                          \
        }                                                                                         \
                                                                                                  \
        _heap_->buffer[_heap_->count++] = element;                                                \
                                                                                                  \
        if (!PFX##_impl_float_up(_heap_, _heap_->count - 1))                                      \
            return false;                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_remove(SNAME *_heap_, V *result)                                                   \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return false;                                                                         \
                                                                                                  \
        *result = _heap_->buffer[0];                                                              \
        _heap_->buffer[0] = _heap_->buffer[_heap_->count - 1];                                    \
        _heap_->buffer[_heap_->count - 1] = PFX##_impl_default_value();                           \
                                                                                                  \
        _heap_->count--;                                                                          \
                                                                                                  \
        if (!PFX##_impl_float_down(_heap_, 0))                                                    \
            return false;                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_insert_if(SNAME *_heap_, V element, bool condition)                                \
    {                                                                                             \
        if (condition)                                                                            \
            return PFX##_insert(_heap_, element);                                                 \
                                                                                                  \
        return false;                                                                             \
    }                                                                                             \
                                                                                                  \
    bool PFX##_remove_if(SNAME *_heap_, V *result, bool condition)                                \
    {                                                                                             \
        if (condition)                                                                            \
            return PFX##_remove(_heap_, result);                                                  \
                                                                                                  \
        return false;                                                                             \
    }                                                                                             \
                                                                                                  \
    V PFX##_peek(SNAME *_heap_)                                                                   \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return PFX##_impl_default_value();                                                    \
                                                                                                  \
        return _heap_->buffer[0];                                                                 \
    }                                                                                             \
                                                                                                  \
    V *PFX##_peek_ref(SNAME *_heap_)                                                              \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return NULL;                                                                          \
                                                                                                  \
        return &(_heap_->buffer[0]);                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_contains(SNAME *_heap_, V element)                                                 \
    {                                                                                             \
        for (size_t i = 0; i < _heap_->count; i++)                                                \
        {                                                                                         \
            if (_heap_->cmp(_heap_->buffer[i], element) == 0)                                     \
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
        return _heap_->count >= _heap_->capacity;                                                 \
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
    SNAME *PFX##_copy_of(SNAME *_heap_, V (*copy_func)(V))                                        \
    {                                                                                             \
        SNAME *result = PFX##_new(_heap_->capacity, _heap_->HO, _heap_->cmp);                     \
                                                                                                  \
        if (!result)                                                                              \
            return NULL;                                                                          \
                                                                                                  \
        if (copy_func)                                                                            \
        {                                                                                         \
            for (size_t i = 0; i < _heap_->count; i++)                                            \
                result->buffer[i] = copy_func(_heap_->buffer[i]);                                 \
        }                                                                                         \
        else                                                                                      \
            memcpy(result->buffer, _heap_->buffer, sizeof(V) * _heap_->count);                    \
                                                                                                  \
        result->count = _heap_->count;                                                            \
                                                                                                  \
        return result;                                                                            \
    }                                                                                             \
                                                                                                  \
    bool PFX##_equals(SNAME *_heap1_, SNAME *_heap2_)                                             \
    {                                                                                             \
        if (PFX##_count(_heap1_) != PFX##_count(_heap2_))                                         \
            return false;                                                                         \
                                                                                                  \
        for (size_t i = 0; i < PFX##_count(_heap1_); i++)                                         \
        {                                                                                         \
            if (_heap1_->cmp(_heap1_->buffer[i], _heap2_->buffer[i]) != 0)                        \
                return false;                                                                     \
        }                                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    cmc_string PFX##_to_string(SNAME *_heap_)                                                     \
    {                                                                                             \
        cmc_string str;                                                                           \
        SNAME *h_ = _heap_;                                                                       \
        const char *name = #SNAME;                                                                \
        const char *t = h_->HO == 1 ? "MaxHeap" : "MinHeap";                                      \
                                                                                                  \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_heap,                                      \
                 name, h_, h_->buffer, h_->capacity, h_->count, t, h_->cmp);                      \
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
            iter->cursor = PFX##_count(iter->target) - 1;                                         \
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
        if (iter->cursor + 1 == PFX##_count(iter->target))                                        \
        {                                                                                         \
            iter->end = true;                                                                     \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        if (steps == 0 || iter->cursor + steps >= PFX##_count(iter->target))                      \
            return false;                                                                         \
                                                                                                  \
        iter->start = PFX##_empty(iter->target);                                                  \
                                                                                                  \
        if (iter->end)                                                                            \
            return false;                                                                         \
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
        return iter->target->buffer[iter->cursor];                                                \
    }                                                                                             \
                                                                                                  \
    size_t PFX##_iter_index(SNAME##_iter *iter)                                                   \
    {                                                                                             \
        return iter->cursor;                                                                      \
    }                                                                                             \
                                                                                                  \
    static bool PFX##_impl_grow(SNAME *_heap_)                                                    \
    {                                                                                             \
        size_t new_capacity = _heap_->capacity * 2;                                               \
                                                                                                  \
        V *new_buffer = realloc(_heap_->buffer, sizeof(V) * new_capacity);                        \
                                                                                                  \
        if (!new_buffer)                                                                          \
            return false;                                                                         \
                                                                                                  \
        _heap_->buffer = new_buffer;                                                              \
        _heap_->capacity = new_capacity;                                                          \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    static bool PFX##_impl_float_up(SNAME *_heap_, size_t index)                                  \
    {                                                                                             \
        /* Current index */                                                                       \
        size_t C = index;                                                                         \
        V child = _heap_->buffer[C];                                                              \
        V parent = _heap_->buffer[(index - 1) / 2];                                               \
                                                                                                  \
        int mod = _heap_->HO;                                                                     \
                                                                                                  \
        while (C > 0 && _heap_->cmp(child, parent) * mod > 0)                                     \
        {                                                                                         \
            /* Swap between C (current element) and its parent */                                 \
            V tmp = _heap_->buffer[C];                                                            \
            _heap_->buffer[C] = _heap_->buffer[(C - 1) / 2];                                      \
            _heap_->buffer[(C - 1) / 2] = tmp;                                                    \
                                                                                                  \
            /* Go to parent */                                                                    \
            C = (C - 1) / 2;                                                                      \
                                                                                                  \
            child = _heap_->buffer[C];                                                            \
            parent = _heap_->buffer[(C - 1) / 2];                                                 \
        }                                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    static bool PFX##_impl_float_down(SNAME *_heap_, size_t index)                                \
    {                                                                                             \
        int mod = _heap_->HO;                                                                     \
                                                                                                  \
        while (index < _heap_->count)                                                             \
        {                                                                                         \
            size_t L = 2 * index + 1;                                                             \
            size_t R = 2 * index + 2;                                                             \
            size_t C = index;                                                                     \
                                                                                                  \
            /* Determine if we swap with the left or right element */                             \
            if (L < _heap_->count && _heap_->cmp(_heap_->buffer[L], _heap_->buffer[C]) * mod > 0) \
            {                                                                                     \
                C = L;                                                                            \
            }                                                                                     \
                                                                                                  \
            if (R < _heap_->count && _heap_->cmp(_heap_->buffer[R], _heap_->buffer[C]) * mod > 0) \
            {                                                                                     \
                C = R;                                                                            \
            }                                                                                     \
                                                                                                  \
            /* Swap only if either left or right was picked, otherwise done */                    \
            if (C != index)                                                                       \
            {                                                                                     \
                V tmp = _heap_->buffer[index];                                                    \
                _heap_->buffer[index] = _heap_->buffer[C];                                        \
                _heap_->buffer[C] = tmp;                                                          \
                                                                                                  \
                index = C;                                                                        \
            }                                                                                     \
            else                                                                                  \
                break;                                                                            \
        }                                                                                         \
                                                                                                  \
        return true;                                                                              \
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

#endif /* CMC_HEAP_H */
