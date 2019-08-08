/**
 * queue.h
 *
 * Creation Date: 15/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Queue
 *
 * A Queue is a First-In First-out (or Last-in Last-out) data structure. It is
 * a Dynamic Circular Array where elements are added from one end of the array
 * and removed from the other end. The circular array here (also known as
 * circular buffer or ring buffer) is very important so that both adding and
 * removing elements from the Queue are done instantly. The array is linear but
 * with the modulo operator it is treated as a circular sequence of elements.
 *
 * If the Queue was implemented as a regular Dynamic Array, when adding or
 * removing an element at the front, it would be necessary to shift all elements
 * currently present in the Queue and this would add up a lot of computing time.
 * Shifting `100000` elements in memory by one position every time an element is
 * added to the Queue is simply not efficient.
 *
 * The Queue has two ends. The `front` and `back`. In this implementation all
 * elements are added to the back of the Queue and removed from the front, which
 * is more or less how queues work in real life. Unlike a Stack that only has
 * operations at one end of the buffer, the Queue needs to be implemented as a
 * circular array in order to quickly add or remove elements.
 *
 * The Queue has three main functions: `enqueue` which adds an element to the
 * Queue; `dequeue` which removes an element from the Queue; and `peek` which
 * return the element at the front of the Queue, that is, the next element to
 * be removed from it.
 *
 * The Queue is used in many applications where a resource is shared among
 * multiple consumers and the Queue is responsible for scheduling the access to
 * the resource.
 */

#ifndef CMC_QUEUE_H
#define CMC_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

#define QUEUE_GENERATE(PFX, SNAME, FMOD, V)    \
    QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

#define QUEUE_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V) \
    QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)

#define QUEUE_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V) \
    QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* HEADER ********************************************************************/
#define QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)                                \
                                                                                  \
    /* Queue Structure */                                                         \
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
        /* Index representing the front of the queue */                           \
        size_t front;                                                             \
                                                                                  \
        /* Index representing the back of the queue */                            \
        size_t back;                                                              \
                                                                                  \
        /* Function that returns an iterator to the start of the queue */         \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                    \
                                                                                  \
        /* Function that returns an iterator to the end of the queue */           \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                      \
                                                                                  \
    } SNAME, *SNAME##_ptr;                                                        \
                                                                                  \
    /* Queue Iterator */                                                          \
    typedef struct SNAME##_iter_s                                                 \
    {                                                                             \
        /* Target queue */                                                        \
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
    FMOD void PFX##_clear(SNAME *_queue_);                                        \
    FMOD void PFX##_free(SNAME *_queue_);                                         \
    /* Collection Input and Output */                                             \
    FMOD bool PFX##_enqueue(SNAME *_queue_, V element);                           \
    FMOD bool PFX##_dequeue(SNAME *_queue_);                                      \
    /* Conditional Input and Output */                                            \
    FMOD bool PFX##_enqueue_if(SNAME *_queue_, V element, bool condition);        \
    FMOD bool PFX##_dequeue_if(SNAME *_queue_, bool condition);                   \
    /* Element Access */                                                          \
    FMOD V PFX##_peek(SNAME *_queue_);                                            \
    /* Collection State */                                                        \
    FMOD bool PFX##_contains(SNAME *_queue_, V element, int (*comparator)(V, V)); \
    FMOD bool PFX##_empty(SNAME *_queue_);                                        \
    FMOD bool PFX##_full(SNAME *_queue_);                                         \
    FMOD size_t PFX##_count(SNAME *_queue_);                                      \
    FMOD size_t PFX##_capacity(SNAME *_queue_);                                   \
    /* Collection Utility */                                                      \
    FMOD cmc_string PFX##_to_string(SNAME *_queue_);                              \
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
#define QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                                                          \
                                                                                                            \
    /* Implementation Detail Functions */                                                                   \
    static bool PFX##_impl_grow(SNAME *_queue_);                                                            \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_queue_);                                                \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_queue_);                                                  \
                                                                                                            \
    FMOD SNAME *PFX##_new(size_t capacity)                                                                  \
    {                                                                                                       \
        if (capacity < 1)                                                                                   \
            return NULL;                                                                                    \
                                                                                                            \
        SNAME *_queue_ = malloc(sizeof(SNAME));                                                             \
                                                                                                            \
        if (!_queue_)                                                                                       \
            return NULL;                                                                                    \
                                                                                                            \
        _queue_->buffer = malloc(sizeof(V) * capacity);                                                     \
                                                                                                            \
        if (!_queue_->buffer)                                                                               \
        {                                                                                                   \
            free(_queue_);                                                                                  \
            return NULL;                                                                                    \
        }                                                                                                   \
                                                                                                            \
        memset(_queue_->buffer, 0, sizeof(V) * capacity);                                                   \
                                                                                                            \
        _queue_->capacity = capacity;                                                                       \
        _queue_->count = 0;                                                                                 \
        _queue_->front = 0;                                                                                 \
        _queue_->back = 0;                                                                                  \
                                                                                                            \
        _queue_->it_start = PFX##_impl_it_start;                                                            \
        _queue_->it_end = PFX##_impl_it_end;                                                                \
                                                                                                            \
        return _queue_;                                                                                     \
    }                                                                                                       \
                                                                                                            \
    FMOD void PFX##_clear(SNAME *_queue_)                                                                   \
    {                                                                                                       \
        memset(_queue_->buffer, 0, sizeof(V) * _queue_->capacity);                                          \
                                                                                                            \
        _queue_->count = 0;                                                                                 \
        _queue_->front = 0;                                                                                 \
        _queue_->back = 0;                                                                                  \
    }                                                                                                       \
                                                                                                            \
    FMOD void PFX##_free(SNAME *_queue_)                                                                    \
    {                                                                                                       \
        free(_queue_->buffer);                                                                              \
        free(_queue_);                                                                                      \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_enqueue(SNAME *_queue_, V element)                                                      \
    {                                                                                                       \
        if (PFX##_full(_queue_))                                                                            \
        {                                                                                                   \
            if (!PFX##_impl_grow(_queue_))                                                                  \
                return false;                                                                               \
        }                                                                                                   \
                                                                                                            \
        _queue_->buffer[_queue_->back] = element;                                                           \
                                                                                                            \
        _queue_->back = (_queue_->back == _queue_->capacity - 1) ? 0 : _queue_->back + 1;                   \
        _queue_->count++;                                                                                   \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_dequeue(SNAME *_queue_)                                                                 \
    {                                                                                                       \
        if (PFX##_empty(_queue_))                                                                           \
            return false;                                                                                   \
                                                                                                            \
        _queue_->buffer[_queue_->front] = PFX##_impl_default_value();                                       \
                                                                                                            \
        _queue_->front = (_queue_->front == _queue_->capacity - 1) ? 0 : _queue_->front + 1;                \
        _queue_->count--;                                                                                   \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_enqueue_if(SNAME *_queue_, V element, bool condition)                                   \
    {                                                                                                       \
        if (condition)                                                                                      \
            return PFX##_enqueue(_queue_, element);                                                         \
                                                                                                            \
        return false;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_dequeue_if(SNAME *_queue_, bool condition)                                              \
    {                                                                                                       \
        if (condition)                                                                                      \
            return PFX##_dequeue(_queue_);                                                                  \
                                                                                                            \
        return false;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    FMOD V PFX##_peek(SNAME *_queue_)                                                                       \
    {                                                                                                       \
        if (PFX##_empty(_queue_))                                                                           \
            return PFX##_impl_default_value();                                                              \
                                                                                                            \
        return _queue_->buffer[_queue_->front];                                                             \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_contains(SNAME *_queue_, V element, int (*comparator)(V, V))                            \
    {                                                                                                       \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; i = (i + 1) % _queue_->count, j++)       \
        {                                                                                                   \
            if (comparator(_queue_->buffer[i], element) == 0)                                               \
                return true;                                                                                \
        }                                                                                                   \
                                                                                                            \
        return false;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_empty(SNAME *_queue_)                                                                   \
    {                                                                                                       \
        return _queue_->count == 0;                                                                         \
    }                                                                                                       \
                                                                                                            \
    FMOD bool PFX##_full(SNAME *_queue_)                                                                    \
    {                                                                                                       \
        return _queue_->count >= _queue_->capacity;                                                         \
    }                                                                                                       \
                                                                                                            \
    FMOD size_t PFX##_count(SNAME *_queue_)                                                                 \
    {                                                                                                       \
        return _queue_->count;                                                                              \
    }                                                                                                       \
                                                                                                            \
    FMOD size_t PFX##_capacity(SNAME *_queue_)                                                              \
    {                                                                                                       \
        return _queue_->capacity;                                                                           \
    }                                                                                                       \
                                                                                                            \
    FMOD cmc_string PFX##_to_string(SNAME *_queue_)                                                         \
    {                                                                                                       \
        cmc_string str;                                                                                     \
        SNAME *q_ = _queue_;                                                                                \
        const char *name = #SNAME;                                                                          \
                                                                                                            \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_queue,                                               \
                 name, q_, q_->buffer, q_->capacity, q_->count, q_->front, q_->back);                       \
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
    static bool PFX##_impl_grow(SNAME *_queue_)                                                             \
    {                                                                                                       \
                                                                                                            \
        size_t new_capacity = _queue_->capacity * 2;                                                        \
                                                                                                            \
        V *new_buffer = malloc(sizeof(V) * new_capacity);                                                   \
                                                                                                            \
        if (!new_buffer)                                                                                    \
            return false;                                                                                   \
                                                                                                            \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; i = (i + 1) % _queue_->capacity, j++)    \
        {                                                                                                   \
            new_buffer[j] = _queue_->buffer[i];                                                             \
        }                                                                                                   \
                                                                                                            \
        free(_queue_->buffer);                                                                              \
                                                                                                            \
        _queue_->buffer = new_buffer;                                                                       \
        _queue_->capacity = new_capacity;                                                                   \
        _queue_->front = 0;                                                                                 \
        _queue_->back = _queue_->count;                                                                     \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_queue_)                                                 \
    {                                                                                                       \
        SNAME##_iter iter;                                                                                  \
                                                                                                            \
        PFX##_iter_init(&iter, _queue_);                                                                    \
        PFX##_iter_to_start(&iter);                                                                         \
                                                                                                            \
        return iter;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_queue_)                                                   \
    {                                                                                                       \
        SNAME##_iter iter;                                                                                  \
                                                                                                            \
        PFX##_iter_init(&iter, _queue_);                                                                    \
        PFX##_iter_to_end(&iter);                                                                           \
                                                                                                            \
        return iter;                                                                                        \
    }

#endif /* CMC_QUEUE_H */
