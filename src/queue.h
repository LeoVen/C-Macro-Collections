/**
 * queue.h
 *
 * Creation Date: 15/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/********************************************************************* QUEUE */
/*****************************************************************************/

#ifndef CMC_QUEUE_H
#define CMC_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define QUEUE_GENERATE(PFX, SNAME, FMOD, V)    \
    QUEUE_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
    QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* PRIVATE *******************************************************************/
#define QUEUE_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define QUEUE_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    QUEUE_GENERATE_STRUCT(PFX, SNAME, FMOD, V)                \
    QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* PUBLIC ********************************************************************/
#define QUEUE_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    QUEUE_GENERATE_STRUCT(PFX, SNAME, FMOD, V)               \
    QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define QUEUE_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* STRUCT ********************************************************************/
#define QUEUE_GENERATE_STRUCT(PFX, SNAME, FMOD, V)

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
        size_t rear;                                                              \
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
    FMOD SNAME *PFX##_new(size_t size);                                           \
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
/* SOURCE ********************************************************************/
#define QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                                                          \
                                                                                                            \
    /* Implementation Detail Functions */                                                                   \
    FMOD bool PFX##_grow(SNAME *_queue_);                                                                   \
    SNAME##_iter PFX##_impl_it_start(SNAME *_queue_);                                                       \
    SNAME##_iter PFX##_impl_it_end(SNAME *_queue_);                                                         \
                                                                                                            \
    FMOD SNAME *PFX##_new(size_t size)                                                                      \
    {                                                                                                       \
        if (size < 1)                                                                                       \
            return NULL;                                                                                    \
                                                                                                            \
        SNAME *_queue_ = malloc(sizeof(SNAME));                                                             \
                                                                                                            \
        if (!_queue_)                                                                                       \
            return NULL;                                                                                    \
                                                                                                            \
        _queue_->buffer = malloc(sizeof(V) * size);                                                         \
                                                                                                            \
        if (!_queue_->buffer)                                                                               \
        {                                                                                                   \
            free(_queue_);                                                                                  \
            return NULL;                                                                                    \
        }                                                                                                   \
                                                                                                            \
        memset(_queue_->buffer, 0, sizeof(V) * size);                                                       \
                                                                                                            \
        _queue_->capacity = size;                                                                           \
        _queue_->count = 0;                                                                                 \
        _queue_->front = 0;                                                                                 \
        _queue_->rear = 0;                                                                                  \
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
        _queue_->rear = 0;                                                                                  \
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
            if (!PFX##_grow(_queue_))                                                                       \
                return false;                                                                               \
        }                                                                                                   \
                                                                                                            \
        _queue_->buffer[_queue_->rear] = element;                                                           \
                                                                                                            \
        _queue_->rear = (_queue_->rear == _queue_->capacity - 1) ? 0 : _queue_->rear + 1;                   \
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
        _queue_->buffer[_queue_->front] = 0;                                                                \
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
            return 0;                                                                                       \
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
            iter->cursor = (iter->target->rear == 0) ? iter->target->capacity - 1 : iter->target->rear - 1; \
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
            return 0;                                                                                       \
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
    FMOD bool PFX##_grow(SNAME *_queue_)                                                                    \
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
        _queue_->rear = _queue_->count;                                                                     \
                                                                                                            \
        return true;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    SNAME##_iter PFX##_impl_it_start(SNAME *_queue_)                                                        \
    {                                                                                                       \
        SNAME##_iter iter;                                                                                  \
                                                                                                            \
        PFX##_iter_init(&iter, _queue_);                                                                    \
        PFX##_iter_to_start(&iter);                                                                         \
                                                                                                            \
        return iter;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    SNAME##_iter PFX##_impl_it_end(SNAME *_queue_)                                                          \
    {                                                                                                       \
        SNAME##_iter iter;                                                                                  \
                                                                                                            \
        PFX##_iter_init(&iter, _queue_);                                                                    \
        PFX##_iter_to_end(&iter);                                                                           \
                                                                                                            \
        return iter;                                                                                        \
    }

#endif /* CMC_QUEUE_H */
