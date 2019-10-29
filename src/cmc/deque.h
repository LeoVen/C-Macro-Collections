/**
 * deque.h
 *
 * Creation Date: 20/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Deque
 *
 * A Deque (double-ended queue) is a linear data structure that is able to add
 * or remove elements from both ends. It can also be thought of a double-ended
 * stack since you can push and pop elements from two ends. The Deque can also
 * be used as a Queue.
 *
 * There is no random access for a Deque. The only elements accessible are the
 * front element and the back.
 *
 * Implementation
 *
 * This implementation uses a circular buffer (ring buffer or cyclic buffer) in
 * order to operate on O(1) for push and pop on either ends (only case where it
 * takes longer than O(1) is when the buffer is reallocated). If it was
 * implemented as a regular array, adding or removing elements from the front
 * would take O(N) due to the need to shift all elements in the deque.
 */

#ifndef CMC_DEQUE_H
#define CMC_DEQUE_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_deque = "%s at %p { buffer:%p, capacity:%" PRIuMAX ", count:%" PRIuMAX ", front:%" PRIuMAX ", back:%" PRIuMAX " }";

#define CMC_GENERATE_DEQUE(PFX, SNAME, V)    \
    CMC_GENERATE_DEQUE_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_DEQUE_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_DEQUE_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_DEQUE_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_DEQUE_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_DEQUE_SOURCE(PFX, SNAME, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_DEQUE_HEADER(PFX, SNAME, V)                                                \
                                                                                                \
    /* Deque Structure */                                                                       \
    struct SNAME                                                                                \
    {                                                                                           \
        /* Dynamic circular array of elements */                                                \
        V *buffer;                                                                              \
                                                                                                \
        /* Current circular array capacity */                                                   \
        size_t capacity;                                                                        \
                                                                                                \
        /* Current amount of elements */                                                        \
        size_t count;                                                                           \
                                                                                                \
        /* Index representing the front of the deque */                                         \
        size_t front;                                                                           \
                                                                                                \
        /* Index representing the back of the deque */                                          \
        size_t back;                                                                            \
                                                                                                \
        /* Function that returns an iterator to the start of the deque */                       \
        struct SNAME##_iter (*it_start)(struct SNAME *);                                        \
                                                                                                \
        /* Function that returns an iterator to the end of the deque */                         \
        struct SNAME##_iter (*it_end)(struct SNAME *);                                          \
    };                                                                                          \
                                                                                                \
    /* Deque Iterator */                                                                        \
    struct SNAME##_iter                                                                         \
    {                                                                                           \
        /* Target deque */                                                                      \
        struct SNAME *target;                                                                   \
                                                                                                \
        /* Cursor's position (index) */                                                         \
        size_t cursor;                                                                          \
                                                                                                \
        /* Keeps track of relative index to the iteration of elements */                        \
        size_t index;                                                                           \
                                                                                                \
        /* If the iterator has reached the start of the iteration */                            \
        bool start;                                                                             \
                                                                                                \
        /* If the iterator has reached the end of the iteration */                              \
        bool end;                                                                               \
    };                                                                                          \
                                                                                                \
    /* Collection Functions */                                                                  \
    /* Collection Allocation and Deallocation */                                                \
    struct SNAME *PFX##_new(size_t capacity);                                                   \
    void PFX##_clear(struct SNAME *_deque_, void (*deallocator)(V));                            \
    void PFX##_free(struct SNAME *_deque_, void (*deallocator)(V));                             \
    /* Collection Input and Output */                                                           \
    bool PFX##_push_front(struct SNAME *_deque_, V element);                                    \
    bool PFX##_push_back(struct SNAME *_deque_, V element);                                     \
    bool PFX##_pop_front(struct SNAME *_deque_);                                                \
    bool PFX##_pop_back(struct SNAME *_deque_);                                                 \
    /* Element Access */                                                                        \
    V PFX##_front(struct SNAME *_deque_);                                                       \
    V PFX##_back(struct SNAME *_deque_);                                                        \
    /* Collection State */                                                                      \
    bool PFX##_contains(struct SNAME *_deque_, V element, int (*comparator)(V, V));             \
    bool PFX##_empty(struct SNAME *_deque_);                                                    \
    bool PFX##_full(struct SNAME *_deque_);                                                     \
    size_t PFX##_count(struct SNAME *_deque_);                                                  \
    size_t PFX##_capacity(struct SNAME *_deque_);                                               \
    /* Collection Utility */                                                                    \
    bool PFX##_resize(struct SNAME *_deque_, size_t capacity);                                  \
    struct SNAME *PFX##_copy_of(struct SNAME *_deque_, V (*copy_func)(V));                      \
    bool PFX##_equals(struct SNAME *_deque1_, struct SNAME *_deque2_, int (*comparator)(V, V)); \
    struct cmc_string PFX##_to_string(struct SNAME *_deque_);                                   \
                                                                                                \
    /* Iterator Functions */                                                                    \
    /* Iterator Allocation and Deallocation */                                                  \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);                                  \
    void PFX##_iter_free(struct SNAME##_iter *iter);                                            \
    /* Iterator Initialization */                                                               \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);                      \
    /* Iterator State */                                                                        \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                                           \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                                             \
    /* Iterator Movement */                                                                     \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                                        \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                                          \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                                            \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                                            \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);                           \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);                            \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);                             \
    /* Iterator Access */                                                                       \
    V PFX##_iter_value(struct SNAME##_iter *iter);                                              \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter);                                            \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);                                         \
                                                                                                \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_DEQUE_SOURCE(PFX, SNAME, V)                                                  \
                                                                                                  \
    /* Implementation Detail Functions */                                                         \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_deque_);                        \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_deque_);                          \
                                                                                                  \
    struct SNAME *PFX##_new(size_t capacity)                                                      \
    {                                                                                             \
        if (capacity < 1)                                                                         \
            return NULL;                                                                          \
                                                                                                  \
        struct SNAME *_deque_ = malloc(sizeof(struct SNAME));                                     \
                                                                                                  \
        if (!_deque_)                                                                             \
            return NULL;                                                                          \
                                                                                                  \
        _deque_->buffer = calloc(capacity, sizeof(V));                                            \
                                                                                                  \
        if (!_deque_->buffer)                                                                     \
        {                                                                                         \
            free(_deque_);                                                                        \
            return NULL;                                                                          \
        }                                                                                         \
                                                                                                  \
        _deque_->capacity = capacity;                                                             \
        _deque_->count = 0;                                                                       \
        _deque_->front = 0;                                                                       \
        _deque_->back = 0;                                                                        \
                                                                                                  \
        _deque_->it_start = PFX##_impl_it_start;                                                  \
        _deque_->it_end = PFX##_impl_it_end;                                                      \
                                                                                                  \
        return _deque_;                                                                           \
    }                                                                                             \
                                                                                                  \
    void PFX##_clear(struct SNAME *_deque_, void (*deallocator)(V))                               \
    {                                                                                             \
        if (deallocator)                                                                          \
        {                                                                                         \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)                       \
            {                                                                                     \
                deallocator(_deque_->buffer[i]);                                                  \
                                                                                                  \
                i = (i + 1) % _deque_->capacity;                                                  \
            }                                                                                     \
        }                                                                                         \
                                                                                                  \
        memset(_deque_->buffer, 0, sizeof(V) * _deque_->capacity);                                \
                                                                                                  \
        _deque_->count = 0;                                                                       \
        _deque_->front = 0;                                                                       \
        _deque_->back = 0;                                                                        \
    }                                                                                             \
                                                                                                  \
    void PFX##_free(struct SNAME *_deque_, void (*deallocator)(V))                                \
    {                                                                                             \
        if (deallocator)                                                                          \
        {                                                                                         \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)                       \
            {                                                                                     \
                deallocator(_deque_->buffer[i]);                                                  \
                                                                                                  \
                i = (i + 1) % _deque_->capacity;                                                  \
            }                                                                                     \
        }                                                                                         \
                                                                                                  \
        free(_deque_->buffer);                                                                    \
        free(_deque_);                                                                            \
    }                                                                                             \
                                                                                                  \
    bool PFX##_push_front(struct SNAME *_deque_, V element)                                       \
    {                                                                                             \
        if (PFX##_full(_deque_))                                                                  \
        {                                                                                         \
            if (!PFX##_resize(_deque_, PFX##_capacity(_deque_) * 2))                              \
                return false;                                                                     \
        }                                                                                         \
                                                                                                  \
        _deque_->front = (_deque_->front == 0) ? _deque_->capacity - 1 : _deque_->front - 1;      \
                                                                                                  \
        _deque_->buffer[_deque_->front] = element;                                                \
                                                                                                  \
        _deque_->count++;                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_push_back(struct SNAME *_deque_, V element)                                        \
    {                                                                                             \
        if (PFX##_full(_deque_))                                                                  \
        {                                                                                         \
            if (!PFX##_resize(_deque_, PFX##_capacity(_deque_) * 2))                              \
                return false;                                                                     \
        }                                                                                         \
                                                                                                  \
        _deque_->buffer[_deque_->back] = element;                                                 \
                                                                                                  \
        _deque_->back = (_deque_->back == _deque_->capacity - 1) ? 0 : _deque_->back + 1;         \
                                                                                                  \
        _deque_->count++;                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_pop_front(struct SNAME *_deque_)                                                   \
    {                                                                                             \
        if (PFX##_empty(_deque_))                                                                 \
            return false;                                                                         \
                                                                                                  \
        _deque_->buffer[_deque_->front] = (V){0};                                                 \
                                                                                                  \
        _deque_->front = (_deque_->front == _deque_->capacity - 1) ? 0 : _deque_->front + 1;      \
                                                                                                  \
        _deque_->count--;                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_pop_back(struct SNAME *_deque_)                                                    \
    {                                                                                             \
        if (PFX##_empty(_deque_))                                                                 \
            return false;                                                                         \
                                                                                                  \
        _deque_->back = (_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1;         \
                                                                                                  \
        _deque_->buffer[_deque_->back] = (V){0};                                                  \
                                                                                                  \
        _deque_->count--;                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    V PFX##_front(struct SNAME *_deque_)                                                          \
    {                                                                                             \
        if (PFX##_empty(_deque_))                                                                 \
            return (V){0};                                                                        \
                                                                                                  \
        return _deque_->buffer[_deque_->front];                                                   \
    }                                                                                             \
                                                                                                  \
    V PFX##_back(struct SNAME *_deque_)                                                           \
    {                                                                                             \
        if (PFX##_empty(_deque_))                                                                 \
            return (V){0};                                                                        \
                                                                                                  \
        return _deque_->buffer[(_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1]; \
    }                                                                                             \
                                                                                                  \
    bool PFX##_contains(struct SNAME *_deque_, V element, int (*comparator)(V, V))                \
    {                                                                                             \
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)                           \
        {                                                                                         \
            if (comparator(_deque_->buffer[i], element) == 0)                                     \
                return true;                                                                      \
                                                                                                  \
            i = (i + 1) % _deque_->capacity;                                                      \
        }                                                                                         \
                                                                                                  \
        return false;                                                                             \
    }                                                                                             \
                                                                                                  \
    bool PFX##_empty(struct SNAME *_deque_)                                                       \
    {                                                                                             \
        return _deque_->count == 0;                                                               \
    }                                                                                             \
                                                                                                  \
    bool PFX##_full(struct SNAME *_deque_)                                                        \
    {                                                                                             \
        return _deque_->count >= _deque_->capacity;                                               \
    }                                                                                             \
                                                                                                  \
    size_t PFX##_count(struct SNAME *_deque_)                                                     \
    {                                                                                             \
        return _deque_->count;                                                                    \
    }                                                                                             \
                                                                                                  \
    size_t PFX##_capacity(struct SNAME *_deque_)                                                  \
    {                                                                                             \
        return _deque_->capacity;                                                                 \
    }                                                                                             \
                                                                                                  \
    bool PFX##_resize(struct SNAME *_deque_, size_t capacity)                                     \
    {                                                                                             \
        if (PFX##_capacity(_deque_) == capacity)                                                  \
            return true;                                                                          \
                                                                                                  \
        if (capacity < PFX##_count(_deque_))                                                      \
            return false;                                                                         \
                                                                                                  \
        V *new_buffer = malloc(sizeof(V) * capacity);                                             \
                                                                                                  \
        if (!new_buffer)                                                                          \
            return false;                                                                         \
                                                                                                  \
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)                           \
        {                                                                                         \
            new_buffer[j] = _deque_->buffer[i];                                                   \
                                                                                                  \
            i = (i + 1) % PFX##_capacity(_deque_);                                                \
        }                                                                                         \
                                                                                                  \
        free(_deque_->buffer);                                                                    \
                                                                                                  \
        _deque_->buffer = new_buffer;                                                             \
        _deque_->capacity = capacity;                                                             \
        _deque_->front = 0;                                                                       \
        _deque_->back = _deque_->count;                                                           \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    struct SNAME *PFX##_copy_of(struct SNAME *_deque_, V (*copy_func)(V))                         \
    {                                                                                             \
        struct SNAME *result = PFX##_new(_deque_->capacity);                                      \
                                                                                                  \
        if (!result)                                                                              \
            return NULL;                                                                          \
                                                                                                  \
        if (copy_func)                                                                            \
        {                                                                                         \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)                       \
            {                                                                                     \
                result->buffer[j] = copy_func(_deque_->buffer[i]);                                \
                                                                                                  \
                i = (i + 1) % _deque_->capacity;                                                  \
            }                                                                                     \
        }                                                                                         \
        else                                                                                      \
        {                                                                                         \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)                       \
            {                                                                                     \
                result->buffer[j] = _deque_->buffer[i];                                           \
                                                                                                  \
                i = (i + 1) % _deque_->capacity;                                                  \
            }                                                                                     \
        }                                                                                         \
                                                                                                  \
        result->count = _deque_->count;                                                           \
                                                                                                  \
        return result;                                                                            \
    }                                                                                             \
                                                                                                  \
    bool PFX##_equals(struct SNAME *_deque1_, struct SNAME *_deque2_, int (*comparator)(V, V))    \
    {                                                                                             \
        if (PFX##_count(_deque1_) != PFX##_count(_deque2_))                                       \
            return false;                                                                         \
                                                                                                  \
        size_t i, j, k;                                                                           \
        for (i = _deque1_->front, j = _deque2_->front, k = 0; k < PFX##_count(_deque1_); k++)     \
        {                                                                                         \
            if (comparator(_deque1_->buffer[i], _deque2_->buffer[j]) != 0)                        \
                return false;                                                                     \
                                                                                                  \
            i = (i + 1) % _deque1_->capacity;                                                     \
            j = (j + 1) % _deque2_->capacity;                                                     \
        }                                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    struct cmc_string PFX##_to_string(struct SNAME *_deque_)                                      \
    {                                                                                             \
        struct cmc_string str;                                                                    \
        struct SNAME *d_ = _deque_;                                                               \
        const char *name = #SNAME;                                                                \
                                                                                                  \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_deque,                                     \
                 name, d_, d_->buffer, d_->capacity, d_->count, d_->front, d_->back);             \
                                                                                                  \
        return str;                                                                               \
    }                                                                                             \
                                                                                                  \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                                     \
    {                                                                                             \
        struct SNAME##_iter *iter = malloc(sizeof(struct SNAME##_iter));                          \
                                                                                                  \
        if (!iter)                                                                                \
            return NULL;                                                                          \
                                                                                                  \
        PFX##_iter_init(iter, target);                                                            \
                                                                                                  \
        return iter;                                                                              \
    }                                                                                             \
                                                                                                  \
    void PFX##_iter_free(struct SNAME##_iter *iter)                                               \
    {                                                                                             \
        free(iter);                                                                               \
    }                                                                                             \
                                                                                                  \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)                         \
    {                                                                                             \
        iter->target = target;                                                                    \
        iter->cursor = target->front;                                                             \
        iter->index = 0;                                                                          \
        iter->start = true;                                                                       \
        iter->end = PFX##_empty(target);                                                          \
    }                                                                                             \
                                                                                                  \
    bool PFX##_iter_start(struct SNAME##_iter *iter)                                              \
    {                                                                                             \
        return PFX##_empty(iter->target) || iter->start;                                          \
    }                                                                                             \
                                                                                                  \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                                                \
    {                                                                                             \
        return PFX##_empty(iter->target) || iter->end;                                            \
    }                                                                                             \
                                                                                                  \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                                           \
    {                                                                                             \
        iter->cursor = iter->target->front;                                                       \
        iter->index = 0;                                                                          \
        iter->start = true;                                                                       \
        iter->end = PFX##_empty(iter->target);                                                    \
    }                                                                                             \
                                                                                                  \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                                             \
    {                                                                                             \
        if (PFX##_empty(iter->target))                                                            \
        {                                                                                         \
            iter->cursor = 0;                                                                     \
            iter->index = 0;                                                                      \
        }                                                                                         \
        else                                                                                      \
        {                                                                                         \
            if (iter->target->back == 0)                                                          \
                iter->cursor = iter->target->capacity - 1;                                        \
            else                                                                                  \
                iter->cursor = iter->target->back - 1;                                            \
                                                                                                  \
            iter->index = PFX##_count(iter->target) - 1;                                          \
        }                                                                                         \
                                                                                                  \
        iter->start = PFX##_empty(iter->target);                                                  \
        iter->end = true;                                                                         \
    }                                                                                             \
                                                                                                  \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                                               \
    {                                                                                             \
        if (iter->end)                                                                            \
            return false;                                                                         \
                                                                                                  \
        if (iter->index + 1 == PFX##_count(iter->target))                                         \
        {                                                                                         \
            iter->end = true;                                                                     \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        iter->start = PFX##_empty(iter->target);                                                  \
                                                                                                  \
        iter->cursor = (iter->cursor + 1) % (iter->target->capacity);                             \
        iter->index++;                                                                            \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                                               \
    {                                                                                             \
        if (iter->start)                                                                          \
            return false;                                                                         \
                                                                                                  \
        if (iter->index == 0)                                                                     \
        {                                                                                         \
            iter->start = true;                                                                   \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        iter->end = PFX##_empty(iter->target);                                                    \
                                                                                                  \
        iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;       \
        iter->index--;                                                                            \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    /* Returns true only if the iterator moved */                                                 \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)                              \
    {                                                                                             \
        if (iter->end)                                                                            \
            return false;                                                                         \
                                                                                                  \
        if (iter->index + 1 == PFX##_count(iter->target))                                         \
        {                                                                                         \
            iter->end = true;                                                                     \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        if (steps == 0 || iter->index + steps >= PFX##_count(iter->target))                       \
            return false;                                                                         \
                                                                                                  \
        iter->start = PFX##_empty(iter->target);                                                  \
                                                                                                  \
        iter->index += steps;                                                                     \
        iter->cursor = (iter->cursor + steps) % iter->target->capacity;                           \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    /* Returns true only if the iterator moved */                                                 \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps)                               \
    {                                                                                             \
        if (iter->start)                                                                          \
            return false;                                                                         \
                                                                                                  \
        if (iter->index == 0)                                                                     \
        {                                                                                         \
            iter->start = true;                                                                   \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        if (steps == 0 || iter->index < steps)                                                    \
            return false;                                                                         \
                                                                                                  \
        iter->end = PFX##_empty(iter->target);                                                    \
                                                                                                  \
        iter->index -= steps;                                                                     \
                                                                                                  \
        /* Prevent underflow */                                                                   \
        if (iter->cursor < steps)                                                                 \
            iter->cursor += PFX##_capacity(iter->target);                                         \
                                                                                                  \
        iter->cursor -= steps;                                                                    \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    /* Returns true only if the iterator was able to be positioned at the given index */          \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)                                \
    {                                                                                             \
        if (index >= PFX##_count(iter->target))                                                   \
            return false;                                                                         \
                                                                                                  \
        if (iter->index > index)                                                                  \
            return PFX##_iter_rewind(iter, iter->index - index);                                  \
        else if (iter->index < index)                                                             \
            return PFX##_iter_advance(iter, index - iter->index);                                 \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    V PFX##_iter_value(struct SNAME##_iter *iter)                                                 \
    {                                                                                             \
        if (PFX##_empty(iter->target))                                                            \
            return (V){0};                                                                        \
                                                                                                  \
        return iter->target->buffer[iter->cursor];                                                \
    }                                                                                             \
                                                                                                  \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter)                                               \
    {                                                                                             \
        if (PFX##_empty(iter->target))                                                            \
            return NULL;                                                                          \
                                                                                                  \
        return &(iter->target->buffer[iter->cursor]);                                             \
    }                                                                                             \
                                                                                                  \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                                            \
    {                                                                                             \
        return iter->index;                                                                       \
    }                                                                                             \
                                                                                                  \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_deque_)                         \
    {                                                                                             \
        struct SNAME##_iter iter;                                                                 \
                                                                                                  \
        PFX##_iter_init(&iter, _deque_);                                                          \
                                                                                                  \
        return iter;                                                                              \
    }                                                                                             \
                                                                                                  \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_deque_)                           \
    {                                                                                             \
        struct SNAME##_iter iter;                                                                 \
                                                                                                  \
        PFX##_iter_init(&iter, _deque_);                                                          \
        PFX##_iter_to_end(&iter);                                                                 \
                                                                                                  \
        return iter;                                                                              \
    }

#endif /* CMC_DEQUE_H */
