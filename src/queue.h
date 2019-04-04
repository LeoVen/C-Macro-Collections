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
#define QUEUE_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
                                                   \
    struct SNAME##_s                               \
    {                                              \
        V *buffer;                                 \
        size_t capacity;                           \
        size_t count;                              \
        size_t front;                              \
        size_t rear;                               \
    };                                             \
                                                   \
    struct SNAME##_iter_s                          \
    {                                              \
        struct SNAME##_s *target;                  \
        size_t cursor;                             \
        size_t index;                              \
        bool start;                                \
        bool end;                                  \
    };                                             \
                                                   \
/* HEADER ********************************************************************/
#define QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)                           \
                                                                             \
    typedef struct SNAME##_s SNAME;                                          \
    typedef struct SNAME##_iter_s SNAME##_iter;                              \
                                                                             \
    FMOD SNAME *PFX##_new(size_t size);                                      \
    FMOD void PFX##_clear(SNAME *_queue_);                                   \
    FMOD void PFX##_free(SNAME *_queue_);                                    \
    FMOD bool PFX##_enqueue(SNAME *_queue_, V element);                      \
    FMOD bool PFX##_dequeue(SNAME *_queue_);                                 \
    FMOD V PFX##_peek(SNAME *_queue_);                                       \
    FMOD bool PFX##_enqueue_if(SNAME *_queue_, V element, bool condition);   \
    FMOD bool PFX##_dequeue_if(SNAME *_queue_, bool condition);              \
    FMOD bool PFX##_empty(SNAME *_queue_);                                   \
    FMOD bool PFX##_full(SNAME *_queue_);                                    \
    FMOD size_t PFX##_count(SNAME *_queue_);                                 \
    FMOD size_t PFX##_capacity(SNAME *_queue_);                              \
                                                                             \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);             \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                          \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                            \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                        \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                          \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index); \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index); \
                                                                             \
/* SOURCE ********************************************************************/
#define QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                                                        \
                                                                                                          \
    FMOD bool PFX##_grow(SNAME *_queue_);                                                                 \
                                                                                                          \
    FMOD SNAME *PFX##_new(size_t size)                                                                    \
    {                                                                                                     \
        if (size < 1)                                                                                     \
            return NULL;                                                                                  \
                                                                                                          \
        SNAME *_queue_ = malloc(sizeof(SNAME));                                                           \
                                                                                                          \
        if (!_queue_)                                                                                     \
            return NULL;                                                                                  \
                                                                                                          \
        _queue_->buffer = malloc(sizeof(V) * size);                                                       \
                                                                                                          \
        if (!_queue_->buffer)                                                                             \
        {                                                                                                 \
            free(_queue_);                                                                                \
            return NULL;                                                                                  \
        }                                                                                                 \
                                                                                                          \
        memset(_queue_->buffer, 0, sizeof(V) * size);                                                     \
                                                                                                          \
        _queue_->capacity = size;                                                                         \
        _queue_->count = 0;                                                                               \
        _queue_->front = 0;                                                                               \
        _queue_->rear = 0;                                                                                \
                                                                                                          \
        return _queue_;                                                                                   \
    }                                                                                                     \
                                                                                                          \
    FMOD void PFX##_clear(SNAME *_queue_)                                                                 \
    {                                                                                                     \
        memset(_queue_->buffer, 0, sizeof(V) * _queue_->capacity);                                        \
                                                                                                          \
        _queue_->count = 0;                                                                               \
        _queue_->front = 0;                                                                               \
        _queue_->rear = 0;                                                                                \
    }                                                                                                     \
                                                                                                          \
    FMOD void PFX##_free(SNAME *_queue_)                                                                  \
    {                                                                                                     \
        free(_queue_->buffer);                                                                            \
        free(_queue_);                                                                                    \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_enqueue(SNAME *_queue_, V element)                                                    \
    {                                                                                                     \
        if (PFX##_full(_queue_))                                                                          \
        {                                                                                                 \
            if (!PFX##_grow(_queue_))                                                                     \
                return false;                                                                             \
        }                                                                                                 \
                                                                                                          \
        _queue_->buffer[_queue_->rear] = element;                                                         \
                                                                                                          \
        _queue_->rear = (_queue_->rear == _queue_->capacity - 1) ? 0 : _queue_->rear + 1;                 \
        _queue_->count++;                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_dequeue(SNAME *_queue_)                                                               \
    {                                                                                                     \
        if (PFX##_empty(_queue_))                                                                         \
            return false;                                                                                 \
                                                                                                          \
        _queue_->buffer[_queue_->front] = 0;                                                              \
                                                                                                          \
        _queue_->front = (_queue_->front == _queue_->capacity - 1) ? 0 : _queue_->front + 1;              \
        _queue_->count--;                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }                                                                                                     \
                                                                                                          \
    FMOD V PFX##_peek(SNAME *_queue_)                                                                     \
    {                                                                                                     \
        if (PFX##_empty(_queue_))                                                                         \
            return 0;                                                                                     \
                                                                                                          \
        return _queue_->buffer[_queue_->front];                                                           \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_enqueue_if(SNAME *_queue_, V element, bool condition)                                 \
    {                                                                                                     \
        if (condition)                                                                                    \
            return PFX##_enqueue(_queue_, element);                                                       \
                                                                                                          \
        return false;                                                                                     \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_dequeue_if(SNAME *_queue_, bool condition)                                            \
    {                                                                                                     \
        if (condition)                                                                                    \
            return PFX##_dequeue(_queue_);                                                                \
                                                                                                          \
        return false;                                                                                     \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_empty(SNAME *_queue_)                                                                 \
    {                                                                                                     \
        return _queue_->count == 0;                                                                       \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_full(SNAME *_queue_)                                                                  \
    {                                                                                                     \
        return _queue_->count >= _queue_->capacity;                                                       \
    }                                                                                                     \
                                                                                                          \
    FMOD size_t PFX##_count(SNAME *_queue_)                                                               \
    {                                                                                                     \
        return _queue_->count;                                                                            \
    }                                                                                                     \
                                                                                                          \
    FMOD size_t PFX##_capacity(SNAME *_queue_)                                                            \
    {                                                                                                     \
        return _queue_->capacity;                                                                         \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_grow(SNAME *_queue_)                                                                  \
    {                                                                                                     \
                                                                                                          \
        size_t new_capacity = _queue_->capacity * 2;                                                      \
                                                                                                          \
        V *new_buffer = malloc(sizeof(V) * new_capacity);                                                 \
                                                                                                          \
        if (!new_buffer)                                                                                  \
            return false;                                                                                 \
                                                                                                          \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; i = (i + 1) % _queue_->capacity, j++)  \
        {                                                                                                 \
            new_buffer[j] = _queue_->buffer[i];                                                           \
        }                                                                                                 \
                                                                                                          \
        free(_queue_->buffer);                                                                            \
                                                                                                          \
        _queue_->buffer = new_buffer;                                                                     \
        _queue_->capacity = new_capacity;                                                                 \
        _queue_->front = 0;                                                                               \
        _queue_->rear = _queue_->count;                                                                   \
                                                                                                          \
        return true;                                                                                      \
    }                                                                                                     \
                                                                                                          \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target)                                           \
    {                                                                                                     \
        iter->target = target;                                                                            \
        iter->cursor = target->front;                                                                     \
        iter->index = 0;                                                                                  \
        iter->start = true;                                                                               \
        iter->end = PFX##_empty(target);                                                                  \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                                        \
    {                                                                                                     \
        return iter->cursor == iter->target->front && iter->start;                                        \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                                          \
    {                                                                                                     \
        size_t real_rear = iter->target->rear == 0 ? iter->target->capacity - 1 : iter->target->rear - 1; \
        return iter->cursor == real_rear && iter->end;                                                    \
    }                                                                                                     \
                                                                                                          \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter)                                                      \
    {                                                                                                     \
        iter->cursor = iter->target->front;                                                               \
        iter->index = 0;                                                                                  \
        iter->start = true;                                                                               \
        iter->end = PFX##_empty(iter->target);                                                            \
    }                                                                                                     \
                                                                                                          \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter)                                                        \
    {                                                                                                     \
        if (PFX##_empty(iter->target))                                                                    \
            iter->cursor = 0;                                                                             \
        else                                                                                              \
            iter->cursor = (iter->target->rear == 0) ? iter->target->count - 1 : iter->target->rear - 1;  \
                                                                                                          \
        iter->index = iter->target->count - 1;                                                            \
        iter->start = PFX##_empty(iter->target);                                                          \
        iter->end = true;                                                                                 \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index)                               \
    {                                                                                                     \
        if (iter->end)                                                                                    \
            return false;                                                                                 \
                                                                                                          \
        *index = iter->index;                                                                             \
        *result = iter->target->buffer[iter->cursor];                                                     \
        iter->start = false;                                                                              \
                                                                                                          \
        if (iter->index == iter->target->count - 1)                                                       \
            iter->end = true;                                                                             \
        else                                                                                              \
        {                                                                                                 \
            iter->cursor = (iter->cursor + 1) % (iter->target->capacity);                                 \
            iter->index++;                                                                                \
        }                                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index)                               \
    {                                                                                                     \
        if (iter->start)                                                                                  \
            return false;                                                                                 \
                                                                                                          \
        *index = iter->index;                                                                             \
        *result = iter->target->buffer[iter->cursor];                                                     \
        iter->end = false;                                                                                \
                                                                                                          \
        if (iter->index == 0)                                                                             \
            iter->start = true;                                                                           \
        else                                                                                              \
        {                                                                                                 \
            iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;           \
            iter->index--;                                                                                \
        }                                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }

#endif /* CMC_QUEUE_H */
