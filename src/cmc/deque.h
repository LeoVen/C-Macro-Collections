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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

#define DEQUE_GENERATE(PFX, SNAME, FMOD, V)    \
    DEQUE_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    DEQUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

#define DEQUE_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V) \
    DEQUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)

#define DEQUE_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V) \
    DEQUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* HEADER ********************************************************************/
#define DEQUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)                                \
                                                                                  \
    /* Deque Structure */                                                         \
    typedef struct SNAME##_s                                                      \
    {                                                                             \
        /* Dynamic circular array of elements */                                  \
        V *buffer;                                                                \
                                                                                  \
        /* Current circular array capacity */                                     \
        size_t capacity;                                                          \
                                                                                  \
        /* Current amount of elements */                                          \
        size_t count;                                                             \
                                                                                  \
        /* Index representing the front of the deque */                           \
        size_t front;                                                             \
                                                                                  \
        /* Index representing the back of the deque */                            \
        size_t back;                                                              \
                                                                                  \
        /* Function that returns an iterator to the start of the deque */         \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                    \
                                                                                  \
        /* Function that returns an iterator to the end of the deque */           \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                      \
                                                                                  \
    } SNAME, *SNAME##_ptr;                                                        \
                                                                                  \
    /* Deque Iterator */                                                          \
    typedef struct SNAME##_iter_s                                                 \
    {                                                                             \
        /* Target deque */                                                        \
        struct SNAME##_s *target;                                                 \
                                                                                  \
        /* Cursor's position (index) */                                           \
        size_t cursor;                                                            \
                                                                                  \
        /* Keeps track of relative index to the iteration of elements */          \
        size_t index;                                                             \
                                                                                  \
        /* If the iterator has reached the start of the iteration */              \
        bool start;                                                               \
                                                                                  \
        /* If the iterator has reached the end of the iteration */                \
        bool end;                                                                 \
                                                                                  \
    } SNAME##_iter, *SNAME##_iter_ptr;                                            \
                                                                                  \
    /* Collection Functions */                                                    \
    /* Collection Allocation and Deallocation */                                  \
    FMOD SNAME *PFX##_new(size_t capacity);                                       \
    FMOD void PFX##_clear(SNAME *_deque_);                                        \
    FMOD void PFX##_free(SNAME *_deque_);                                         \
    /* Collection Input and Output */                                             \
    FMOD bool PFX##_push_front(SNAME *_deque_, V element);                        \
    FMOD bool PFX##_push_back(SNAME *_deque_, V element);                         \
    FMOD bool PFX##_pop_front(SNAME *_deque_);                                    \
    FMOD bool PFX##_pop_back(SNAME *_deque_);                                     \
    /* Conditional Input and Output */                                            \
    FMOD bool PFX##_push_front_if(SNAME *_deque_, V element, bool condition);     \
    FMOD bool PFX##_push_back_if(SNAME *_deque_, V element, bool condition);      \
    FMOD bool PFX##_pop_front_if(SNAME *_deque_, bool condition);                 \
    FMOD bool PFX##_pop_back_if(SNAME *_deque_, bool condition);                  \
    /* Element Access */                                                          \
    FMOD V PFX##_front(SNAME *_deque_);                                           \
    FMOD V PFX##_back(SNAME *_deque_);                                            \
    /* Collection State */                                                        \
    FMOD bool PFX##_contains(SNAME *_deque_, V element, int (*comparator)(V, V)); \
    FMOD bool PFX##_empty(SNAME *_deque_);                                        \
    FMOD bool PFX##_full(SNAME *_deque_);                                         \
    FMOD size_t PFX##_count(SNAME *_deque_);                                      \
    FMOD size_t PFX##_capacity(SNAME *_deque_);                                   \
    /* Collection Utility */                                                      \
    FMOD cmc_string PFX##_to_string(SNAME *_deque_);                              \
                                                                                  \
    /* Iterator Functions */                                                      \
    /* Iterator Allocation and Deallocation */                                    \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);                             \
    FMOD void PFX##_iter_free(SNAME##_iter *iter);                                \
    /* Iterator Initialization */                                                 \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                 \
    /* Iterator State */                                                          \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                               \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                 \
    /* Iterator Movement */                                                       \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter);                            \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter);                              \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter);                                \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter);                                \
    /* Iterator Access */                                                         \
    FMOD V PFX##_iter_value(SNAME##_iter *iter);                                  \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter);                                \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter);                             \
                                                                                  \
    /* Default Value */                                                           \
    static inline V PFX##_impl_default_value(void)                                \
    {                                                                             \
        V _empty_value_;                                                          \
                                                                                  \
        memset(&_empty_value_, 0, sizeof(V));                                     \
                                                                                  \
        return _empty_value_;                                                     \
    }                                                                             \
                                                                                  \
/* SOURCE ********************************************************************/
#define DEQUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                                                          \
                                                                                                            \
    /* Implementation Detail Functions */                                                                   \
    static bool PFX##_impl_grow(SNAME *_deque_);                                                            \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_deque_);                                                \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_deque_);                                                  \
                                                                                                            \
    FMOD SNAME *PFX##_new(size_t capacity)                                                                  \
    {                                                                                                       \
        if (capacity < 1)                                                                                   \
            return NULL;                                                                                    \
                                                                                                            \
        SNAME *_deque_ = malloc(sizeof(SNAME));                                                             \
                                                                                                            \
        if (!_deque_)                                                                                       \
            return NULL;                                                                                    \
                                                                                                            \
        _deque_->buffer = malloc(sizeof(V) * capacity);                                                     \
                                                                                                            \
        if (!_deque_->buffer)                                                                               \
        {                                                                                                   \
            free(_deque_);                                                                                  \
            return NULL;                                                                                    \
        }                                                                                                   \
                                                                                                            \
        memset(_deque_->buffer, 0, sizeof(V) * capacity);                                                   \
                                                                                                            \
        _deque_->capacity = capacity;                                                                       \
        _deque_->count = 0;                                                                                 \
        _deque_->front = 0;                                                                                 \
        _deque_->back = 0;                                                                                  \
                                                                                                            \
        _deque_->it_start = PFX##_impl_it_start;                                                            \
        _deque_->it_end = PFX##_impl_it_end;                                                                \
                                                                                                            \
        return _deque_;                                                                                     \
    }                                                                                                       \
                                                                                                            \
    FMOD void PFX##_clear(SNAME *_deque_)                                                                   \
    {                                                                                                       \
        memset(_deque_->buffer, 0, sizeof(V) * _deque_->capacity);                                          \
                                                                                                            \
        _deque_->count = 0;                                                                                 \
        _deque_->front = 0;                                                                                 \
        _deque_->back = 0;                                                                                  \
    }                                                                                                       \
                                                                                                            \
    FMOD void PFX##_free(SNAME *_deque_)                                                                    \
    {                                                                                                       \
        free(_deque_->buffer);                                                                              \
        free(_deque_);                                                                                      \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_push_front(SNAME *_deque_, V element)                                                   \
    {                                                                                                       \
        if (PFX##_full(_deque_))                                                                            \
        {                                                                                                   \
            if (!PFX##_impl_grow(_deque_))                                                                  \
                return false;                                                                               \
        }                                                                                                   \
                                                                                                            \
        _deque_->front = (_deque_->front == 0) ? _deque_->capacity - 1 : _deque_->front - 1;                \
                                                                                                            \
        _deque_->buffer[_deque_->front] = element;                                                          \
                                                                                                            \
        _deque_->count++;                                                                                   \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_push_back(SNAME *_deque_, V element)                                                    \
    {                                                                                                       \
        if (PFX##_full(_deque_))                                                                            \
        {                                                                                                   \
            if (!PFX##_impl_grow(_deque_))                                                                  \
                return false;                                                                               \
        }                                                                                                   \
                                                                                                            \
        _deque_->buffer[_deque_->back] = element;                                                           \
                                                                                                            \
        _deque_->back = (_deque_->back == _deque_->capacity - 1) ? 0 : _deque_->back + 1;                   \
                                                                                                            \
        _deque_->count++;                                                                                   \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_pop_front(SNAME *_deque_)                                                               \
    {                                                                                                       \
        if (PFX##_empty(_deque_))                                                                           \
            return false;                                                                                   \
                                                                                                            \
        _deque_->buffer[_deque_->front] = PFX##_impl_default_value();                                       \
                                                                                                            \
        _deque_->front = (_deque_->front == _deque_->capacity - 1) ? 0 : _deque_->front + 1;                \
                                                                                                            \
        _deque_->count--;                                                                                   \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_pop_back(SNAME *_deque_)                                                                \
    {                                                                                                       \
        if (PFX##_empty(_deque_))                                                                           \
            return false;                                                                                   \
                                                                                                            \
        _deque_->back = (_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1;                   \
                                                                                                            \
        _deque_->buffer[_deque_->back] = PFX##_impl_default_value();                                        \
                                                                                                            \
        _deque_->count--;                                                                                   \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_push_front_if(SNAME *_deque_, V element, bool condition)                                \
    {                                                                                                       \
        if (condition)                                                                                      \
            return PFX##_push_front(_deque_, element);                                                      \
                                                                                                            \
        return false;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_push_back_if(SNAME *_deque_, V element, bool condition)                                 \
    {                                                                                                       \
        if (condition)                                                                                      \
            return PFX##_push_back(_deque_, element);                                                       \
                                                                                                            \
        return false;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_pop_front_if(SNAME *_deque_, bool condition)                                            \
    {                                                                                                       \
        if (condition)                                                                                      \
            return PFX##_pop_front(_deque_);                                                                \
                                                                                                            \
        return false;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_pop_back_if(SNAME *_deque_, bool condition)                                             \
    {                                                                                                       \
        if (condition)                                                                                      \
            return PFX##_pop_back(_deque_);                                                                 \
                                                                                                            \
        return false;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    FMOD V PFX##_front(SNAME *_deque_)                                                                      \
    {                                                                                                       \
        if (PFX##_empty(_deque_))                                                                           \
            return PFX##_impl_default_value();                                                              \
                                                                                                            \
        return _deque_->buffer[_deque_->front];                                                             \
    }                                                                                                       \
                                                                                                            \
    FMOD V PFX##_back(SNAME *_deque_)                                                                       \
    {                                                                                                       \
        if (PFX##_empty(_deque_))                                                                           \
            return PFX##_impl_default_value();                                                              \
                                                                                                            \
        return _deque_->buffer[(_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1];           \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_contains(SNAME *_deque_, V element, int (*comparator)(V, V))                            \
    {                                                                                                       \
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; i = (i + 1) % _deque_->count, j++)       \
        {                                                                                                   \
            if (comparator(_deque_->buffer[i], element) == 0)                                               \
                return true;                                                                                \
        }                                                                                                   \
                                                                                                            \
        return false;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_empty(SNAME *_deque_)                                                                   \
    {                                                                                                       \
        return _deque_->count == 0;                                                                         \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_full(SNAME *_deque_)                                                                    \
    {                                                                                                       \
        return _deque_->count >= _deque_->capacity;                                                         \
    }                                                                                                       \
                                                                                                            \
    FMOD size_t PFX##_count(SNAME *_deque_)                                                                 \
    {                                                                                                       \
        return _deque_->count;                                                                              \
    }                                                                                                       \
                                                                                                            \
    FMOD size_t PFX##_capacity(SNAME *_deque_)                                                              \
    {                                                                                                       \
        return _deque_->capacity;                                                                           \
    }                                                                                                       \
                                                                                                            \
    FMOD cmc_string PFX##_to_string(SNAME *_deque_)                                                         \
    {                                                                                                       \
        cmc_string str;                                                                                     \
        SNAME *d_ = _deque_;                                                                                \
        const char *name = #SNAME;                                                                          \
                                                                                                            \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_deque,                                               \
                 name, d_, d_->buffer, d_->capacity, d_->count, d_->front, d_->back);                       \
                                                                                                            \
        return str;                                                                                         \
    }                                                                                                       \
                                                                                                            \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target)                                                        \
    {                                                                                                       \
        SNAME##_iter *iter = malloc(sizeof(SNAME##_iter));                                                  \
                                                                                                            \
        if (!iter)                                                                                          \
            return NULL;                                                                                    \
                                                                                                            \
        PFX##_iter_init(iter, target);                                                                      \
                                                                                                            \
        return iter;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    FMOD void PFX##_iter_free(SNAME##_iter *iter)                                                           \
    {                                                                                                       \
        free(iter);                                                                                         \
    }                                                                                                       \
                                                                                                            \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                                            \
    {                                                                                                       \
        iter->target = target;                                                                              \
        iter->cursor = target->front;                                                                       \
        iter->index = 0;                                                                                    \
        iter->start = true;                                                                                 \
        iter->end = PFX##_empty(target);                                                                    \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                                          \
    {                                                                                                       \
        return PFX##_empty(iter->target) || iter->start;                                                    \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                                            \
    {                                                                                                       \
        return PFX##_empty(iter->target) || iter->end;                                                      \
    }                                                                                                       \
                                                                                                            \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter)                                                       \
    {                                                                                                       \
        iter->cursor = iter->target->front;                                                                 \
        iter->index = 0;                                                                                    \
        iter->start = true;                                                                                 \
        iter->end = PFX##_empty(iter->target);                                                              \
    }                                                                                                       \
                                                                                                            \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter)                                                         \
    {                                                                                                       \
        if (PFX##_empty(iter->target))                                                                      \
            iter->cursor = 0;                                                                               \
        else                                                                                                \
            iter->cursor = (iter->target->back == 0) ? iter->target->capacity - 1 : iter->target->back - 1; \
                                                                                                            \
        iter->index = iter->target->count - 1;                                                              \
        iter->start = PFX##_empty(iter->target);                                                            \
        iter->end = true;                                                                                   \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter)                                                           \
    {                                                                                                       \
        if (iter->end)                                                                                      \
            return false;                                                                                   \
                                                                                                            \
        iter->start = PFX##_empty(iter->target);                                                            \
                                                                                                            \
        if (iter->index == iter->target->count - 1)                                                         \
            iter->end = true;                                                                               \
        else                                                                                                \
        {                                                                                                   \
            iter->cursor = (iter->cursor + 1) % (iter->target->capacity);                                   \
            iter->index++;                                                                                  \
        }                                                                                                   \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter)                                                           \
    {                                                                                                       \
        if (iter->start)                                                                                    \
            return false;                                                                                   \
                                                                                                            \
        iter->end = PFX##_empty(iter->target);                                                              \
                                                                                                            \
        if (iter->index == 0)                                                                               \
            iter->start = true;                                                                             \
        else                                                                                                \
        {                                                                                                   \
            iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;             \
            iter->index--;                                                                                  \
        }                                                                                                   \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    FMOD V PFX##_iter_value(SNAME##_iter *iter)                                                             \
    {                                                                                                       \
        if (PFX##_empty(iter->target))                                                                      \
            return PFX##_impl_default_value();                                                              \
                                                                                                            \
        return iter->target->buffer[iter->cursor];                                                          \
    }                                                                                                       \
                                                                                                            \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter)                                                           \
    {                                                                                                       \
        if (PFX##_empty(iter->target))                                                                      \
            return NULL;                                                                                    \
                                                                                                            \
        return &(iter->target->buffer[iter->cursor]);                                                       \
    }                                                                                                       \
                                                                                                            \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter)                                                        \
    {                                                                                                       \
        return iter->index;                                                                                 \
    }                                                                                                       \
                                                                                                            \
    static bool PFX##_impl_grow(SNAME *_deque_)                                                             \
    {                                                                                                       \
        size_t new_capacity = _deque_->capacity * 2;                                                        \
                                                                                                            \
        V *new_buffer = malloc(sizeof(V) * new_capacity);                                                   \
                                                                                                            \
        if (!new_buffer)                                                                                    \
            return false;                                                                                   \
                                                                                                            \
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; i = (i + 1) % _deque_->capacity, j++)    \
        {                                                                                                   \
            new_buffer[j] = _deque_->buffer[i];                                                             \
        }                                                                                                   \
                                                                                                            \
        free(_deque_->buffer);                                                                              \
                                                                                                            \
        _deque_->buffer = new_buffer;                                                                       \
        _deque_->capacity = new_capacity;                                                                   \
        _deque_->front = 0;                                                                                 \
        _deque_->back = _deque_->count;                                                                     \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_deque_)                                                 \
    {                                                                                                       \
        SNAME##_iter iter;                                                                                  \
                                                                                                            \
        PFX##_iter_init(&iter, _deque_);                                                                    \
                                                                                                            \
        return iter;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_deque_)                                                   \
    {                                                                                                       \
        SNAME##_iter iter;                                                                                  \
                                                                                                            \
        PFX##_iter_init(&iter, _deque_);                                                                    \
        PFX##_iter_to_end(&iter);                                                                           \
                                                                                                            \
        return iter;                                                                                        \
    }

#endif /* CMC_DEQUE_H */
