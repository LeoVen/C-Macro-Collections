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

#define HEAP_GENERATE(PFX, SNAME, FMOD, V)    \
    HEAP_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    HEAP_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

#define HEAP_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V) \
    HEAP_GENERATE_HEADER(PFX, SNAME, FMOD, V)

#define HEAP_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V) \
    HEAP_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* HEADER ********************************************************************/
#define HEAP_GENERATE_HEADER(PFX, SNAME, FMOD, V)                                    \
                                                                                     \
    /* Heap Structure */                                                             \
    typedef struct SNAME##_s                                                         \
    {                                                                                \
        /* Dynamic array of elements */                                              \
        V *buffer;                                                                   \
                                                                                     \
        /* Current array capacity */                                                 \
        size_t capacity;                                                             \
                                                                                     \
        /* Current amount of elements in the heap */                                 \
        size_t count;                                                                \
                                                                                     \
        /* Heap order (MaxHeap or MinHeap) */                                        \
        enum cmc_heap_order_e HO;                                                    \
                                                                                     \
        /* Element comparison function */                                            \
        int (*cmp)(V, V);                                                            \
                                                                                     \
        /* Function that returns an iterator to the start of the heap */             \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                       \
                                                                                     \
        /* Function that returns an iterator to the end of the heap */               \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                         \
                                                                                     \
    } SNAME, *SNAME##_ptr;                                                           \
                                                                                     \
    /* Heap Iterator */                                                              \
    typedef struct SNAME##_iter_s                                                    \
    {                                                                                \
        /* Target heap */                                                            \
        struct SNAME##_s *target;                                                    \
                                                                                     \
        /* Cursor's position (index) */                                              \
        size_t cursor;                                                               \
                                                                                     \
        /* If the iterator has reached the start of the iteration */                 \
        bool start;                                                                  \
                                                                                     \
        /* If the iterator has reached the end of the iteration */                   \
        bool end;                                                                    \
                                                                                     \
    } SNAME##_iter, *SNAME##_iter_ptr;                                               \
                                                                                     \
    /* Collection Functions */                                                       \
    /* Collection Allocation and Deallocation */                                     \
    FMOD SNAME *PFX##_new(size_t capacity, cmc_heap_order HO, int (*compare)(V, V)); \
    FMOD void PFX##_clear(SNAME *_heap_);                                            \
    FMOD void PFX##_free(SNAME *_heap_);                                             \
    /* Collection Input and Output */                                                \
    FMOD bool PFX##_insert(SNAME *_heap_, V element);                                \
    FMOD bool PFX##_remove(SNAME *_heap_, V *result);                                \
    /* Conditional Input and Output */                                               \
    FMOD bool PFX##_insert_if(SNAME *_heap_, V element, bool condition);             \
    FMOD bool PFX##_remove_if(SNAME *_heap_, V *result, bool condition);             \
    /* Element Access */                                                             \
    FMOD V PFX##_peek(SNAME *_heap_);                                                \
    FMOD V *PFX##_peek_ref(SNAME *_heap_);                                           \
    /* Collection State */                                                           \
    FMOD bool PFX##_contains(SNAME *_heap_, V element);                              \
    FMOD bool PFX##_empty(SNAME *_heap_);                                            \
    FMOD bool PFX##_full(SNAME *_heap_);                                             \
    FMOD size_t PFX##_count(SNAME *_heap_);                                          \
    FMOD size_t PFX##_capacity(SNAME *_heap_);                                       \
    /* Collection Utility */                                                         \
    FMOD cmc_string PFX##_to_string(SNAME *_heap_);                                  \
                                                                                     \
    /* Iterator Functions */                                                         \
    /* Iterator Allocation and Deallocation */                                       \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);                                \
    FMOD void PFX##_iter_free(SNAME##_iter *iter);                                   \
    /* Iterator Initialization */                                                    \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                    \
    /* Iterator State */                                                             \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                  \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                    \
    /* Iterator Movement */                                                          \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter);                               \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter);                                 \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter);                                   \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter);                                   \
    /* Iterator Access */                                                            \
    FMOD V PFX##_iter_value(SNAME##_iter *iter);                                     \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter);                                \
                                                                                     \
    /* Default Value */                                                              \
    static inline V PFX##_impl_default_value(void)                                   \
    {                                                                                \
        V _empty_value_;                                                             \
                                                                                     \
        memset(&_empty_value_, 0, sizeof(V));                                        \
                                                                                     \
        return _empty_value_;                                                        \
    }                                                                                \
                                                                                     \
/* SOURCE ********************************************************************/
#define HEAP_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                                                 \
                                                                                                  \
    /* Implementation Detail Functions */                                                         \
    static bool PFX##_impl_grow(SNAME *_heap_);                                                   \
    static bool PFX##_impl_float_up(SNAME *_heap_, size_t index);                                 \
    static bool PFX##_impl_float_down(SNAME *_heap_, size_t index);                               \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_heap_);                                       \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_heap_);                                         \
                                                                                                  \
    FMOD SNAME *PFX##_new(size_t capacity, cmc_heap_order HO, int (*compare)(V, V))               \
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
    FMOD void PFX##_clear(SNAME *_heap_)                                                          \
    {                                                                                             \
        memset(_heap_->buffer, 0, sizeof(V) * _heap_->capacity);                                  \
                                                                                                  \
        _heap_->count = 0;                                                                        \
    }                                                                                             \
                                                                                                  \
    FMOD void PFX##_free(SNAME *_heap_)                                                           \
    {                                                                                             \
        free(_heap_->buffer);                                                                     \
        free(_heap_);                                                                             \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_insert(SNAME *_heap_, V element)                                              \
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
    FMOD bool PFX##_remove(SNAME *_heap_, V *result)                                              \
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
    FMOD bool PFX##_insert_if(SNAME *_heap_, V element, bool condition)                           \
    {                                                                                             \
        if (condition)                                                                            \
            return PFX##_insert(_heap_, element);                                                 \
                                                                                                  \
        return false;                                                                             \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_remove_if(SNAME *_heap_, V *result, bool condition)                           \
    {                                                                                             \
        if (condition)                                                                            \
            return PFX##_remove(_heap_, result);                                                  \
                                                                                                  \
        return false;                                                                             \
    }                                                                                             \
                                                                                                  \
    FMOD V PFX##_peek(SNAME *_heap_)                                                              \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return PFX##_impl_default_value();                                                    \
                                                                                                  \
        return _heap_->buffer[0];                                                                 \
    }                                                                                             \
                                                                                                  \
    FMOD V *PFX##_peek_ref(SNAME *_heap_)                                                         \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return NULL;                                                                          \
                                                                                                  \
        return &(_heap_->buffer[0]);                                                              \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_contains(SNAME *_heap_, V element)                                            \
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
    FMOD bool PFX##_empty(SNAME *_heap_)                                                          \
    {                                                                                             \
        return _heap_->count == 0;                                                                \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_full(SNAME *_heap_)                                                           \
    {                                                                                             \
        return _heap_->count >= _heap_->capacity;                                                 \
    }                                                                                             \
                                                                                                  \
    FMOD size_t PFX##_count(SNAME *_heap_)                                                        \
    {                                                                                             \
        return _heap_->count;                                                                     \
    }                                                                                             \
                                                                                                  \
    FMOD size_t PFX##_capacity(SNAME *_heap_)                                                     \
    {                                                                                             \
        return _heap_->capacity;                                                                  \
    }                                                                                             \
                                                                                                  \
    FMOD cmc_string PFX##_to_string(SNAME *_heap_)                                                \
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
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target)                                              \
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
    FMOD void PFX##_iter_free(SNAME##_iter *iter)                                                 \
    {                                                                                             \
        free(iter);                                                                               \
    }                                                                                             \
                                                                                                  \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                                  \
    {                                                                                             \
        iter->target = target;                                                                    \
        iter->cursor = 0;                                                                         \
        iter->start = true;                                                                       \
        iter->end = PFX##_empty(target);                                                          \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                                \
    {                                                                                             \
        return PFX##_empty(iter->target) || iter->start;                                          \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                                  \
    {                                                                                             \
        return PFX##_empty(iter->target) || iter->end;                                            \
    }                                                                                             \
                                                                                                  \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter)                                             \
    {                                                                                             \
        iter->cursor = 0;                                                                         \
        iter->start = true;                                                                       \
        iter->end = PFX##_empty(iter->target);                                                    \
    }                                                                                             \
                                                                                                  \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter)                                               \
    {                                                                                             \
        iter->cursor = iter->target->count - 1;                                                   \
        iter->start = PFX##_empty(iter->target);                                                  \
        iter->end = true;                                                                         \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter)                                                 \
    {                                                                                             \
        if (iter->end)                                                                            \
            return false;                                                                         \
                                                                                                  \
        iter->start = false;                                                                      \
                                                                                                  \
        if (iter->cursor == iter->target->count - 1)                                              \
            iter->end = true;                                                                     \
        else                                                                                      \
            iter->cursor++;                                                                       \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter)                                                 \
    {                                                                                             \
        if (iter->start)                                                                          \
            return false;                                                                         \
                                                                                                  \
        iter->end = false;                                                                        \
                                                                                                  \
        if (iter->cursor == 0)                                                                    \
            iter->start = true;                                                                   \
        else                                                                                      \
            iter->cursor--;                                                                       \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    FMOD V PFX##_iter_value(SNAME##_iter *iter)                                                   \
    {                                                                                             \
        if (PFX##_empty(iter->target))                                                            \
            return PFX##_impl_default_value();                                                    \
                                                                                                  \
        return iter->target->buffer[iter->cursor];                                                \
    }                                                                                             \
                                                                                                  \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter)                                              \
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
