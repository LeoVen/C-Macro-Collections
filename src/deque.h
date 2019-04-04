/**
 * deque.h
 *
 * Creation Date: 20/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/********************************************************************* DEQUE */
/*****************************************************************************/

#ifndef CMC_DEQUE_H
#define CMC_DEQUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEQUE_GENERATE(PFX, SNAME, FMOD, V)    \
    DEQUE_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
    DEQUE_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    DEQUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* PRIVATE *******************************************************************/
#define DEQUE_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    DEQUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define DEQUE_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    DEQUE_GENERATE_STRUCT(PFX, SNAME, FMOD, V)                \
    DEQUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* PUBLIC ********************************************************************/
#define DEQUE_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    DEQUE_GENERATE_STRUCT(PFX, SNAME, FMOD, V)               \
    DEQUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define DEQUE_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    DEQUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* STRUCT ********************************************************************/
#define DEQUE_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
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
        size_t count;                              \
        bool start;                                \
        bool end;                                  \
    };                                             \
                                                   \
/* HEADER ********************************************************************/
#define DEQUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)                            \
                                                                              \
    typedef struct SNAME##_s SNAME;                                           \
    typedef struct SNAME##_iter_s SNAME##_iter;                               \
                                                                              \
    FMOD SNAME *PFX##_new(size_t size);                                       \
    FMOD void PFX##_clear(SNAME *_deque_);                                    \
    FMOD void PFX##_free(SNAME *_deque_);                                     \
    FMOD bool PFX##_push_front(SNAME *_deque_, V element);                    \
    FMOD bool PFX##_push_back(SNAME *_deque_, V element);                     \
    FMOD bool PFX##_pop_front(SNAME *_deque_);                                \
    FMOD bool PFX##_pop_back(SNAME *_deque_);                                 \
    FMOD bool PFX##_push_front_if(SNAME *_deque_, V element, bool condition); \
    FMOD bool PFX##_push_back_if(SNAME *_deque_, V element, bool condition);  \
    FMOD bool PFX##_pop_front_if(SNAME *_deque_, bool condition);             \
    FMOD bool PFX##_pop_back_if(SNAME *_deque_, bool condition);              \
    FMOD V PFX##_front(SNAME *_deque_);                                       \
    FMOD V PFX##_back(SNAME *_deque_);                                        \
    FMOD bool PFX##_empty(SNAME *_deque_);                                    \
    FMOD bool PFX##_full(SNAME *_deque_);                                     \
    FMOD size_t PFX##_count(SNAME *_deque_);                                  \
    FMOD size_t PFX##_capacity(SNAME *_deque_);                               \
                                                                              \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);              \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                           \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                             \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                         \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                           \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index);  \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index);  \
                                                                              \
/* SOURCE ********************************************************************/
#define DEQUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                                                        \
                                                                                                          \
    FMOD bool PFX##_grow(SNAME *_deque_);                                                                 \
                                                                                                          \
    FMOD SNAME *PFX##_new(size_t size)                                                                    \
    {                                                                                                     \
        if (size < 1)                                                                                     \
            return NULL;                                                                                  \
                                                                                                          \
        SNAME *_deque_ = malloc(sizeof(SNAME));                                                           \
                                                                                                          \
        if (!_deque_)                                                                                     \
            return NULL;                                                                                  \
                                                                                                          \
        _deque_->buffer = malloc(sizeof(V) * size);                                                       \
                                                                                                          \
        if (!_deque_->buffer)                                                                             \
        {                                                                                                 \
            free(_deque_);                                                                                \
            return NULL;                                                                                  \
        }                                                                                                 \
                                                                                                          \
        memset(_deque_->buffer, 0, sizeof(V) * size);                                                     \
                                                                                                          \
        _deque_->capacity = size;                                                                         \
        _deque_->count = 0;                                                                               \
        _deque_->front = 0;                                                                               \
        _deque_->rear = 0;                                                                                \
                                                                                                          \
        return _deque_;                                                                                   \
    }                                                                                                     \
                                                                                                          \
    FMOD void PFX##_clear(SNAME *_deque_)                                                                 \
    {                                                                                                     \
        memset(_deque_->buffer, 0, sizeof(V) * _deque_->capacity);                                        \
                                                                                                          \
        _deque_->count = 0;                                                                               \
        _deque_->front = 0;                                                                               \
        _deque_->rear = 0;                                                                                \
    }                                                                                                     \
                                                                                                          \
    FMOD void PFX##_free(SNAME *_deque_)                                                                  \
    {                                                                                                     \
        free(_deque_->buffer);                                                                            \
        free(_deque_);                                                                                    \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_push_front(SNAME *_deque_, V element)                                                 \
    {                                                                                                     \
        if (PFX##_full(_deque_))                                                                          \
        {                                                                                                 \
            if (!PFX##_grow(_deque_))                                                                     \
                return false;                                                                             \
        }                                                                                                 \
                                                                                                          \
        _deque_->front = (_deque_->front == 0) ? _deque_->capacity - 1 : _deque_->front - 1;              \
                                                                                                          \
        _deque_->buffer[_deque_->front] = element;                                                        \
                                                                                                          \
        _deque_->count++;                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_push_back(SNAME *_deque_, V element)                                                  \
    {                                                                                                     \
        if (PFX##_full(_deque_))                                                                          \
        {                                                                                                 \
            if (!PFX##_grow(_deque_))                                                                     \
                return false;                                                                             \
        }                                                                                                 \
                                                                                                          \
        _deque_->buffer[_deque_->rear] = element;                                                         \
                                                                                                          \
        _deque_->rear = (_deque_->rear == _deque_->capacity - 1) ? 0 : _deque_->rear + 1;                 \
                                                                                                          \
        _deque_->count++;                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_pop_front(SNAME *_deque_)                                                             \
    {                                                                                                     \
        if (PFX##_empty(_deque_))                                                                         \
            return false;                                                                                 \
                                                                                                          \
        _deque_->buffer[_deque_->front] = 0;                                                              \
                                                                                                          \
        _deque_->front = (_deque_->front == _deque_->capacity - 1) ? 0 : _deque_->front + 1;              \
                                                                                                          \
        _deque_->count--;                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_pop_back(SNAME *_deque_)                                                              \
    {                                                                                                     \
        if (PFX##_empty(_deque_))                                                                         \
            return false;                                                                                 \
                                                                                                          \
        _deque_->rear = (_deque_->rear == 0) ? _deque_->capacity - 1 : _deque_->rear - 1;                 \
                                                                                                          \
        _deque_->buffer[_deque_->rear] = 0;                                                               \
                                                                                                          \
        _deque_->count--;                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_push_front_if(SNAME *_deque_, V element, bool condition)                              \
    {                                                                                                     \
        if (condition)                                                                                    \
            return PFX##_push_front(_deque_, element);                                                    \
                                                                                                          \
        return false;                                                                                     \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_push_back_if(SNAME *_deque_, V element, bool condition)                               \
    {                                                                                                     \
        if (condition)                                                                                    \
            return PFX##_push_back(_deque_, element);                                                     \
                                                                                                          \
        return false;                                                                                     \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_pop_front_if(SNAME *_deque_, bool condition)                                          \
    {                                                                                                     \
        if (condition)                                                                                    \
            return PFX##_pop_front(_deque_);                                                              \
                                                                                                          \
        return false;                                                                                     \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_pop_back_if(SNAME *_deque_, bool condition)                                           \
    {                                                                                                     \
        if (condition)                                                                                    \
            return PFX##_pop_back(_deque_);                                                               \
                                                                                                          \
        return false;                                                                                     \
    }                                                                                                     \
                                                                                                          \
    FMOD V PFX##_front(SNAME *_deque_)                                                                    \
    {                                                                                                     \
        if (PFX##_empty(_deque_))                                                                         \
            return 0;                                                                                     \
                                                                                                          \
        return _deque_->buffer[_deque_->front];                                                           \
    }                                                                                                     \
                                                                                                          \
    FMOD V PFX##_back(SNAME *_deque_)                                                                     \
    {                                                                                                     \
        if (PFX##_empty(_deque_))                                                                         \
            return 0;                                                                                     \
                                                                                                          \
        return _deque_->buffer[(_deque_->rear == 0) ? _deque_->capacity - 1 : _deque_->rear - 1];         \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_empty(SNAME *_deque_)                                                                 \
    {                                                                                                     \
        return _deque_->count == 0;                                                                       \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_full(SNAME *_deque_)                                                                  \
    {                                                                                                     \
        return _deque_->count >= _deque_->capacity;                                                       \
    }                                                                                                     \
                                                                                                          \
    FMOD size_t PFX##_count(SNAME *_deque_)                                                               \
    {                                                                                                     \
        return _deque_->count;                                                                            \
    }                                                                                                     \
                                                                                                          \
    FMOD size_t PFX##_capacity(SNAME *_deque_)                                                            \
    {                                                                                                     \
        return _deque_->capacity;                                                                         \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_grow(SNAME *_deque_)                                                                  \
    {                                                                                                     \
                                                                                                          \
        size_t new_capacity = _deque_->capacity * 2;                                                      \
                                                                                                          \
        V *new_buffer = malloc(sizeof(V) * new_capacity);                                                 \
                                                                                                          \
        if (!new_buffer)                                                                                  \
            return false;                                                                                 \
                                                                                                          \
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; i = (i + 1) % _deque_->capacity, j++)  \
        {                                                                                                 \
            new_buffer[j] = _deque_->buffer[i];                                                           \
        }                                                                                                 \
                                                                                                          \
        free(_deque_->buffer);                                                                            \
                                                                                                          \
        _deque_->buffer = new_buffer;                                                                     \
        _deque_->capacity = new_capacity;                                                                 \
        _deque_->front = 0;                                                                               \
        _deque_->rear = _deque_->count;                                                                   \
                                                                                                          \
        return true;                                                                                      \
    }                                                                                                     \
                                                                                                          \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target)                                           \
    {                                                                                                     \
        iter->target = target;                                                                            \
        iter->cursor = target->front;                                                                     \
        iter->count = 0;                                                                                  \
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
        iter->count = 0;                                                                                  \
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
        iter->count = iter->target->count - 1;                                                            \
        iter->start = PFX##_empty(iter->target);                                                          \
        iter->end = true;                                                                                 \
    }                                                                                                     \
                                                                                                          \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index)                               \
    {                                                                                                     \
        if (iter->end)                                                                                    \
            return false;                                                                                 \
                                                                                                          \
        *index = iter->count;                                                                             \
        *result = iter->target->buffer[iter->cursor];                                                     \
        iter->start = false;                                                                              \
                                                                                                          \
        if (iter->count == iter->target->count - 1)                                                       \
            iter->end = true;                                                                             \
        else                                                                                              \
        {                                                                                                 \
            iter->cursor = (iter->cursor + 1) % (iter->target->capacity);                                 \
            iter->count++;                                                                                \
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
        *index = iter->count;                                                                             \
        *result = iter->target->buffer[iter->cursor];                                                     \
        iter->end = false;                                                                                \
                                                                                                          \
        if (iter->count == 0)                                                                             \
            iter->start = true;                                                                           \
        else                                                                                              \
        {                                                                                                 \
            iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;           \
            iter->count--;                                                                                \
        }                                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }

#endif /* CMC_DEQUE_H */
