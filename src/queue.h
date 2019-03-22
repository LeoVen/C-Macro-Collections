/**
 * queue.h
 *
 * Last Update: 22/03/2019
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
#include <stdint.h>

/* PRIVATE *******************************************************************/
#define QUEUE_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, T) \
    QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, K, T)
#define QUEUE_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, T) \
    QUEUE_GENERATE_STRUCT(PFX, SNAME, FMOD, K, T)             \
    QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, K, T)
/* PUBLIC ********************************************************************/
#define QUEUE_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, T) \
    QUEUE_GENERATE_STRUCT(PFX, SNAME, FMOD, K, T)            \
    QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, K, T)
#define QUEUE_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, T) \
    QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, K, T)
/* STRUCT ********************************************************************/
#define QUEUE_GENERATE_STRUCT(PFX, SNAME, FMOD, K, T) \
                                                      \
    struct SNAME##_s                                  \
    {                                                 \
        T *buffer;                                    \
        size_t capacity;                              \
        size_t count;                                 \
        size_t front;                                 \
        size_t rear;                                  \
    };                                                \
                                                      \
    struct SNAME##_iter_s                             \
    {                                                 \
        struct SNAME##_s *target;                     \
        size_t cursor;                                \
        size_t count;                                 \
    };                                                \
                                                      \
/* HEADER ********************************************************************/
#define QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, K, T)                        \
                                                                             \
    typedef struct SNAME##_s SNAME;                                          \
    typedef struct SNAME##_iter_s SNAME##_iter;                              \
                                                                             \
    FMOD SNAME *PFX##_new(size_t size);                                      \
    FMOD void PFX##_free(SNAME *_queue_);                                    \
    FMOD bool PFX##_enqueue(SNAME *_queue_, T element);                      \
    FMOD bool PFX##_dequeue(SNAME *_queue_);                                 \
    FMOD T PFX##_peek(SNAME *_queue_);                                       \
    FMOD bool PFX##_enqueue_if(SNAME *_queue_, T element, bool condition);   \
    FMOD bool PFX##_dequeue_if(SNAME *_queue_, bool condition);              \
    FMOD bool PFX##_empty(SNAME *_queue_);                                   \
    FMOD bool PFX##_full(SNAME *_queue_);                                    \
    FMOD size_t PFX##_count(SNAME *_queue_);                                 \
    FMOD size_t PFX##_capacity(SNAME *_queue_);                              \
                                                                             \
    FMOD void PFX##_iter(SNAME##_iter *iter, SNAME *target);                 \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, T *result, size_t *index); \
                                                                             \
/* SOURCE ********************************************************************/
#define QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, K, T)                                                    \
                                                                                                         \
    FMOD bool PFX##_grow(SNAME *_queue_);                                                                \
                                                                                                         \
    FMOD SNAME *PFX##_new(size_t size)                                                                   \
    {                                                                                                    \
        if (size < 1)                                                                                    \
            return NULL;                                                                                 \
                                                                                                         \
        SNAME *_queue_ = malloc(sizeof(SNAME));                                                          \
                                                                                                         \
        if (!_queue_)                                                                                    \
            return NULL;                                                                                 \
                                                                                                         \
        _queue_->buffer = malloc(sizeof(T) * size);                                                      \
                                                                                                         \
        if (!_queue_->buffer)                                                                            \
        {                                                                                                \
            free(_queue_);                                                                               \
            return NULL;                                                                                 \
        }                                                                                                \
                                                                                                         \
        for (size_t i = 0; i < size; i++)                                                                \
        {                                                                                                \
            _queue_->buffer[i] = 0;                                                                      \
        }                                                                                                \
                                                                                                         \
        _queue_->capacity = size;                                                                        \
        _queue_->count = 0;                                                                              \
        _queue_->front = 0;                                                                              \
        _queue_->rear = 0;                                                                               \
                                                                                                         \
        return _queue_;                                                                                  \
    }                                                                                                    \
                                                                                                         \
    FMOD void PFX##_free(SNAME *_queue_)                                                                 \
    {                                                                                                    \
        free(_queue_->buffer);                                                                           \
        free(_queue_);                                                                                   \
    }                                                                                                    \
                                                                                                         \
    FMOD bool PFX##_enqueue(SNAME *_queue_, T element)                                                   \
    {                                                                                                    \
        if (PFX##_full(_queue_))                                                                         \
        {                                                                                                \
            if (!PFX##_grow(_queue_))                                                                    \
                return false;                                                                            \
        }                                                                                                \
                                                                                                         \
        _queue_->buffer[_queue_->rear] = element;                                                        \
                                                                                                         \
        _queue_->rear = (_queue_->rear == _queue_->capacity - 1) ? 0 : _queue_->rear + 1;                \
        _queue_->count++;                                                                                \
                                                                                                         \
        return true;                                                                                     \
    }                                                                                                    \
                                                                                                         \
    FMOD bool PFX##_dequeue(SNAME *_queue_)                                                              \
    {                                                                                                    \
        if (PFX##_empty(_queue_))                                                                        \
            return false;                                                                                \
                                                                                                         \
        _queue_->buffer[_queue_->front] = 0;                                                             \
                                                                                                         \
        _queue_->front = (_queue_->front == _queue_->capacity - 1) ? 0 : _queue_->front + 1;             \
        _queue_->count--;                                                                                \
                                                                                                         \
        return true;                                                                                     \
    }                                                                                                    \
                                                                                                         \
    FMOD T PFX##_peek(SNAME *_queue_)                                                                    \
    {                                                                                                    \
        if (PFX##_empty(_queue_))                                                                        \
            return 0;                                                                                    \
                                                                                                         \
        return _queue_->buffer[_queue_->front];                                                          \
    }                                                                                                    \
                                                                                                         \
    FMOD bool PFX##_enqueue_if(SNAME *_queue_, T element, bool condition)                                \
    {                                                                                                    \
        if (condition)                                                                                   \
            return PFX##_enqueue(_queue_, element);                                                      \
                                                                                                         \
        return false;                                                                                    \
    }                                                                                                    \
                                                                                                         \
    FMOD bool PFX##_dequeue_if(SNAME *_queue_, bool condition)                                           \
    {                                                                                                    \
        if (condition)                                                                                   \
            return PFX##_dequeue(_queue_);                                                               \
                                                                                                         \
        return false;                                                                                    \
    }                                                                                                    \
                                                                                                         \
    FMOD bool PFX##_empty(SNAME *_queue_)                                                                \
    {                                                                                                    \
        return _queue_->count == 0;                                                                      \
    }                                                                                                    \
                                                                                                         \
    FMOD bool PFX##_full(SNAME *_queue_)                                                                 \
    {                                                                                                    \
        return _queue_->count >= _queue_->capacity;                                                      \
    }                                                                                                    \
                                                                                                         \
    FMOD size_t PFX##_count(SNAME *_queue_)                                                              \
    {                                                                                                    \
        return _queue_->count;                                                                           \
    }                                                                                                    \
                                                                                                         \
    FMOD size_t PFX##_capacity(SNAME *_queue_)                                                           \
    {                                                                                                    \
        return _queue_->capacity;                                                                        \
    }                                                                                                    \
                                                                                                         \
    FMOD bool PFX##_grow(SNAME *_queue_)                                                                 \
    {                                                                                                    \
                                                                                                         \
        size_t new_capacity = _queue_->capacity * 2;                                                     \
                                                                                                         \
        T *new_buffer = malloc(sizeof(T) * new_capacity);                                                \
                                                                                                         \
        if (!new_buffer)                                                                                 \
            return false;                                                                                \
                                                                                                         \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; i = (i + 1) % _queue_->capacity, j++) \
        {                                                                                                \
            new_buffer[j] = _queue_->buffer[i];                                                          \
        }                                                                                                \
                                                                                                         \
        free(_queue_->buffer);                                                                           \
                                                                                                         \
        _queue_->buffer = new_buffer;                                                                    \
        _queue_->capacity = new_capacity;                                                                \
        _queue_->front = 0;                                                                              \
        _queue_->rear = _queue_->count;                                                                  \
                                                                                                         \
        return true;                                                                                     \
    }                                                                                                    \
                                                                                                         \
    FMOD void PFX##_iter(SNAME##_iter *iter, SNAME *target)                                              \
    {                                                                                                    \
        iter->target = target;                                                                           \
        iter->cursor = target->front;                                                                    \
        iter->count = 0;                                                                                 \
    }                                                                                                    \
                                                                                                         \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, T *result, size_t *index)                              \
    {                                                                                                    \
        if (iter->count < iter->target->count)                                                           \
        {                                                                                                \
            *result = iter->target->buffer[iter->cursor];                                                \
            iter->cursor = (iter->cursor + 1) % (iter->target->capacity);                                \
            *index = iter->count;                                                                        \
            iter->count++;                                                                               \
            return true;                                                                                 \
        }                                                                                                \
                                                                                                         \
        return false;                                                                                    \
    }

#endif /* CMC_QUEUE_H */
