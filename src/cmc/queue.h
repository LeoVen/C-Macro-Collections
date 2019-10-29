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

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_queue = "%s at %p { buffer:%p, capacity:%" PRIuMAX ", count:%" PRIuMAX ", front:%" PRIuMAX ", back:%" PRIuMAX " }";

#define CMC_GENERATE_QUEUE(PFX, SNAME, V)    \
    CMC_GENERATE_QUEUE_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_QUEUE_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_QUEUE_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_QUEUE_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_QUEUE_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_QUEUE_SOURCE(PFX, SNAME, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_QUEUE_HEADER(PFX, SNAME, V)                                                \
                                                                                                \
    /* Queue Structure */                                                                       \
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
        /* Index representing the front of the queue */                                         \
        size_t front;                                                                           \
                                                                                                \
        /* Index representing the back of the queue */                                          \
        size_t back;                                                                            \
                                                                                                \
        /* Function that returns an iterator to the start of the queue */                       \
        struct SNAME##_iter (*it_start)(struct SNAME *);                                        \
                                                                                                \
        /* Function that returns an iterator to the end of the queue */                         \
        struct SNAME##_iter (*it_end)(struct SNAME *);                                          \
    };                                                                                          \
                                                                                                \
    /* Queue Iterator */                                                                        \
    struct SNAME##_iter                                                                         \
    {                                                                                           \
        /* Target queue */                                                                      \
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
    void PFX##_clear(struct SNAME *_queue_, void (*deallocator)(V));                            \
    void PFX##_free(struct SNAME *_queue_, void (*deallocator)(V));                             \
    /* Collection Input and Output */                                                           \
    bool PFX##_enqueue(struct SNAME *_queue_, V element);                                       \
    bool PFX##_dequeue(struct SNAME *_queue_);                                                  \
    /* Element Access */                                                                        \
    V PFX##_peek(struct SNAME *_queue_);                                                        \
    /* Collection State */                                                                      \
    bool PFX##_contains(struct SNAME *_queue_, V element, int (*comparator)(V, V));             \
    bool PFX##_empty(struct SNAME *_queue_);                                                    \
    bool PFX##_full(struct SNAME *_queue_);                                                     \
    size_t PFX##_count(struct SNAME *_queue_);                                                  \
    size_t PFX##_capacity(struct SNAME *_queue_);                                               \
    /* Collection Utility */                                                                    \
    bool PFX##_resize(struct SNAME *_queue_, size_t capacity);                                  \
    struct SNAME *PFX##_copy_of(struct SNAME *_queue_, V (*copy_func)(V));                      \
    bool PFX##_equals(struct SNAME *_queue1_, struct SNAME *_queue2_, int (*comparator)(V, V)); \
    struct cmc_string PFX##_to_string(struct SNAME *_queue_);                                   \
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
#define CMC_GENERATE_QUEUE_SOURCE(PFX, SNAME, V)                                               \
                                                                                               \
    /* Implementation Detail Functions */                                                      \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_queue_);                     \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_queue_);                       \
                                                                                               \
    struct SNAME *PFX##_new(size_t capacity)                                                   \
    {                                                                                          \
        if (capacity < 1)                                                                      \
            return NULL;                                                                       \
                                                                                               \
        struct SNAME *_queue_ = malloc(sizeof(struct SNAME));                                  \
                                                                                               \
        if (!_queue_)                                                                          \
            return NULL;                                                                       \
                                                                                               \
        _queue_->buffer = calloc(capacity, sizeof(V));                                         \
                                                                                               \
        if (!_queue_->buffer)                                                                  \
        {                                                                                      \
            free(_queue_);                                                                     \
            return NULL;                                                                       \
        }                                                                                      \
                                                                                               \
        _queue_->capacity = capacity;                                                          \
        _queue_->count = 0;                                                                    \
        _queue_->front = 0;                                                                    \
        _queue_->back = 0;                                                                     \
                                                                                               \
        _queue_->it_start = PFX##_impl_it_start;                                               \
        _queue_->it_end = PFX##_impl_it_end;                                                   \
                                                                                               \
        return _queue_;                                                                        \
    }                                                                                          \
                                                                                               \
    void PFX##_clear(struct SNAME *_queue_, void (*deallocator)(V))                            \
    {                                                                                          \
        if (deallocator)                                                                       \
        {                                                                                      \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)                    \
            {                                                                                  \
                deallocator(_queue_->buffer[i]);                                               \
                                                                                               \
                i = (i + 1) % _queue_->capacity;                                               \
            }                                                                                  \
        }                                                                                      \
                                                                                               \
        memset(_queue_->buffer, 0, sizeof(V) * _queue_->capacity);                             \
                                                                                               \
        _queue_->count = 0;                                                                    \
        _queue_->front = 0;                                                                    \
        _queue_->back = 0;                                                                     \
    }                                                                                          \
                                                                                               \
    void PFX##_free(struct SNAME *_queue_, void (*deallocator)(V))                             \
    {                                                                                          \
        if (deallocator)                                                                       \
        {                                                                                      \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)                    \
            {                                                                                  \
                deallocator(_queue_->buffer[i]);                                               \
                                                                                               \
                i = (i + 1) % _queue_->capacity;                                               \
            }                                                                                  \
        }                                                                                      \
                                                                                               \
        free(_queue_->buffer);                                                                 \
        free(_queue_);                                                                         \
    }                                                                                          \
                                                                                               \
    bool PFX##_enqueue(struct SNAME *_queue_, V element)                                       \
    {                                                                                          \
        if (PFX##_full(_queue_))                                                               \
        {                                                                                      \
            if (!PFX##_resize(_queue_, PFX##_capacity(_queue_) * 2))                           \
                return false;                                                                  \
        }                                                                                      \
                                                                                               \
        _queue_->buffer[_queue_->back] = element;                                              \
                                                                                               \
        _queue_->back = (_queue_->back == _queue_->capacity - 1) ? 0 : _queue_->back + 1;      \
        _queue_->count++;                                                                      \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    bool PFX##_dequeue(struct SNAME *_queue_)                                                  \
    {                                                                                          \
        if (PFX##_empty(_queue_))                                                              \
            return false;                                                                      \
                                                                                               \
        _queue_->buffer[_queue_->front] = (V){0};                                              \
                                                                                               \
        _queue_->front = (_queue_->front == _queue_->capacity - 1) ? 0 : _queue_->front + 1;   \
        _queue_->count--;                                                                      \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    V PFX##_peek(struct SNAME *_queue_)                                                        \
    {                                                                                          \
        if (PFX##_empty(_queue_))                                                              \
            return (V){0};                                                                     \
                                                                                               \
        return _queue_->buffer[_queue_->front];                                                \
    }                                                                                          \
                                                                                               \
    bool PFX##_contains(struct SNAME *_queue_, V element, int (*comparator)(V, V))             \
    {                                                                                          \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)                        \
        {                                                                                      \
            if (comparator(_queue_->buffer[i], element) == 0)                                  \
                return true;                                                                   \
                                                                                               \
            i = (i + 1) % _queue_->capacity;                                                   \
        }                                                                                      \
                                                                                               \
        return false;                                                                          \
    }                                                                                          \
                                                                                               \
    bool PFX##_empty(struct SNAME *_queue_)                                                    \
    {                                                                                          \
        return _queue_->count == 0;                                                            \
    }                                                                                          \
                                                                                               \
    bool PFX##_full(struct SNAME *_queue_)                                                     \
    {                                                                                          \
        return _queue_->count >= _queue_->capacity;                                            \
    }                                                                                          \
                                                                                               \
    size_t PFX##_count(struct SNAME *_queue_)                                                  \
    {                                                                                          \
        return _queue_->count;                                                                 \
    }                                                                                          \
                                                                                               \
    size_t PFX##_capacity(struct SNAME *_queue_)                                               \
    {                                                                                          \
        return _queue_->capacity;                                                              \
    }                                                                                          \
                                                                                               \
    bool PFX##_resize(struct SNAME *_queue_, size_t capacity)                                  \
    {                                                                                          \
        if (PFX##_capacity(_queue_) == capacity)                                               \
            return true;                                                                       \
                                                                                               \
        if (capacity < PFX##_count(_queue_))                                                   \
            return false;                                                                      \
                                                                                               \
        V *new_buffer = malloc(sizeof(V) * capacity);                                          \
                                                                                               \
        if (!new_buffer)                                                                       \
            return false;                                                                      \
                                                                                               \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)                        \
        {                                                                                      \
            new_buffer[j] = _queue_->buffer[i];                                                \
                                                                                               \
            i = (i + 1) % PFX##_capacity(_queue_);                                             \
        }                                                                                      \
                                                                                               \
        free(_queue_->buffer);                                                                 \
                                                                                               \
        _queue_->buffer = new_buffer;                                                          \
        _queue_->capacity = capacity;                                                          \
        _queue_->front = 0;                                                                    \
        _queue_->back = _queue_->count;                                                        \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    struct SNAME *PFX##_copy_of(struct SNAME *_queue_, V (*copy_func)(V))                      \
    {                                                                                          \
        struct SNAME *result = PFX##_new(_queue_->capacity);                                   \
                                                                                               \
        if (!result)                                                                           \
            return NULL;                                                                       \
                                                                                               \
        if (copy_func)                                                                         \
        {                                                                                      \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)                    \
            {                                                                                  \
                result->buffer[j] = copy_func(_queue_->buffer[i]);                             \
                                                                                               \
                i = (i + 1) % _queue_->capacity;                                               \
            }                                                                                  \
        }                                                                                      \
        else                                                                                   \
        {                                                                                      \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)                    \
            {                                                                                  \
                result->buffer[j] = _queue_->buffer[i];                                        \
                                                                                               \
                i = (i + 1) % _queue_->capacity;                                               \
            }                                                                                  \
        }                                                                                      \
                                                                                               \
        result->count = _queue_->count;                                                        \
                                                                                               \
        return result;                                                                         \
    }                                                                                          \
                                                                                               \
    bool PFX##_equals(struct SNAME *_queue1_, struct SNAME *_queue2_, int (*comparator)(V, V)) \
    {                                                                                          \
        if (PFX##_count(_queue1_) != PFX##_count(_queue2_))                                    \
            return false;                                                                      \
                                                                                               \
        size_t i, j, k;                                                                        \
        for (i = _queue1_->front, j = _queue2_->front, k = 0; k < PFX##_count(_queue1_); k++)  \
        {                                                                                      \
            if (comparator(_queue1_->buffer[i], _queue2_->buffer[j]) != 0)                     \
                return false;                                                                  \
                                                                                               \
            i = (i + 1) % _queue1_->capacity;                                                  \
            j = (j + 1) % _queue2_->capacity;                                                  \
        }                                                                                      \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    struct cmc_string PFX##_to_string(struct SNAME *_queue_)                                   \
    {                                                                                          \
        struct cmc_string str;                                                                 \
        struct SNAME *q_ = _queue_;                                                            \
        const char *name = #SNAME;                                                             \
                                                                                               \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_queue,                                  \
                 name, q_, q_->buffer, q_->capacity, q_->count, q_->front, q_->back);          \
                                                                                               \
        return str;                                                                            \
    }                                                                                          \
                                                                                               \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                                  \
    {                                                                                          \
        struct SNAME##_iter *iter = malloc(sizeof(struct SNAME##_iter));                       \
                                                                                               \
        if (!iter)                                                                             \
            return NULL;                                                                       \
                                                                                               \
        PFX##_iter_init(iter, target);                                                         \
                                                                                               \
        return iter;                                                                           \
    }                                                                                          \
                                                                                               \
    void PFX##_iter_free(struct SNAME##_iter *iter)                                            \
    {                                                                                          \
        free(iter);                                                                            \
    }                                                                                          \
                                                                                               \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)                      \
    {                                                                                          \
        iter->target = target;                                                                 \
        iter->cursor = target->front;                                                          \
        iter->index = 0;                                                                       \
        iter->start = true;                                                                    \
        iter->end = PFX##_empty(target);                                                       \
    }                                                                                          \
                                                                                               \
    bool PFX##_iter_start(struct SNAME##_iter *iter)                                           \
    {                                                                                          \
        return PFX##_empty(iter->target) || iter->start;                                       \
    }                                                                                          \
                                                                                               \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                                             \
    {                                                                                          \
        return PFX##_empty(iter->target) || iter->end;                                         \
    }                                                                                          \
                                                                                               \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                                        \
    {                                                                                          \
        iter->cursor = iter->target->front;                                                    \
        iter->index = 0;                                                                       \
        iter->start = true;                                                                    \
        iter->end = PFX##_empty(iter->target);                                                 \
    }                                                                                          \
                                                                                               \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                                          \
    {                                                                                          \
        if (PFX##_empty(iter->target))                                                         \
            iter->cursor = 0;                                                                  \
        else                                                                                   \
        {                                                                                      \
            if (iter->target->back == 0)                                                       \
                iter->cursor = iter->target->capacity - 1;                                     \
            else                                                                               \
                iter->cursor = iter->target->back - 1;                                         \
        }                                                                                      \
                                                                                               \
        iter->index = iter->target->count - 1;                                                 \
        iter->start = PFX##_empty(iter->target);                                               \
        iter->end = true;                                                                      \
    }                                                                                          \
                                                                                               \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                                            \
    {                                                                                          \
        if (iter->end)                                                                         \
            return false;                                                                      \
                                                                                               \
        if (iter->index + 1 == PFX##_count(iter->target))                                      \
        {                                                                                      \
            iter->end = true;                                                                  \
            return false;                                                                      \
        }                                                                                      \
                                                                                               \
        iter->start = PFX##_empty(iter->target);                                               \
                                                                                               \
        iter->cursor = (iter->cursor + 1) % (iter->target->capacity);                          \
        iter->index++;                                                                         \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                                            \
    {                                                                                          \
        if (iter->start)                                                                       \
            return false;                                                                      \
                                                                                               \
        if (iter->index == 0)                                                                  \
        {                                                                                      \
            iter->start = true;                                                                \
            return false;                                                                      \
        }                                                                                      \
                                                                                               \
        iter->end = PFX##_empty(iter->target);                                                 \
                                                                                               \
        iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;    \
        iter->index--;                                                                         \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    /* Returns true only if the iterator moved */                                              \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)                           \
    {                                                                                          \
        if (iter->end)                                                                         \
            return false;                                                                      \
                                                                                               \
        if (iter->index + 1 == PFX##_count(iter->target))                                      \
        {                                                                                      \
            iter->end = true;                                                                  \
            return false;                                                                      \
        }                                                                                      \
                                                                                               \
        if (steps == 0 || iter->index + steps >= PFX##_count(iter->target))                    \
            return false;                                                                      \
                                                                                               \
        iter->start = PFX##_empty(iter->target);                                               \
                                                                                               \
        iter->index += steps;                                                                  \
        iter->cursor = (iter->cursor + steps) % iter->target->capacity;                        \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    /* Returns true only if the iterator moved */                                              \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps)                            \
    {                                                                                          \
        if (iter->start)                                                                       \
            return false;                                                                      \
                                                                                               \
        if (iter->index == 0)                                                                  \
        {                                                                                      \
            iter->start = true;                                                                \
            return false;                                                                      \
        }                                                                                      \
                                                                                               \
        if (steps == 0 || iter->index < steps)                                                 \
            return false;                                                                      \
                                                                                               \
        iter->end = PFX##_empty(iter->target);                                                 \
                                                                                               \
        iter->index -= steps;                                                                  \
                                                                                               \
        /* Prevent underflow */                                                                \
        if (iter->cursor < steps)                                                              \
            iter->cursor += PFX##_capacity(iter->target);                                      \
                                                                                               \
        iter->cursor -= steps;                                                                 \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    /* Returns true only if the iterator was able to be positioned at the given index */       \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)                             \
    {                                                                                          \
        if (index >= PFX##_count(iter->target))                                                \
            return false;                                                                      \
                                                                                               \
        if (iter->index > index)                                                               \
            return PFX##_iter_rewind(iter, iter->index - index);                               \
        else if (iter->index < index)                                                          \
            return PFX##_iter_advance(iter, index - iter->index);                              \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    V PFX##_iter_value(struct SNAME##_iter *iter)                                              \
    {                                                                                          \
        if (PFX##_empty(iter->target))                                                         \
            return (V){0};                                                                     \
                                                                                               \
        return iter->target->buffer[iter->cursor];                                             \
    }                                                                                          \
                                                                                               \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter)                                            \
    {                                                                                          \
        if (PFX##_empty(iter->target))                                                         \
            return NULL;                                                                       \
                                                                                               \
        return &(iter->target->buffer[iter->cursor]);                                          \
    }                                                                                          \
                                                                                               \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                                         \
    {                                                                                          \
        return iter->index;                                                                    \
    }                                                                                          \
                                                                                               \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_queue_)                      \
    {                                                                                          \
        struct SNAME##_iter iter;                                                              \
                                                                                               \
        PFX##_iter_init(&iter, _queue_);                                                       \
        PFX##_iter_to_start(&iter);                                                            \
                                                                                               \
        return iter;                                                                           \
    }                                                                                          \
                                                                                               \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_queue_)                        \
    {                                                                                          \
        struct SNAME##_iter iter;                                                              \
                                                                                               \
        PFX##_iter_init(&iter, _queue_);                                                       \
        PFX##_iter_to_end(&iter);                                                              \
                                                                                               \
        return iter;                                                                           \
    }

#endif /* CMC_QUEUE_H */
