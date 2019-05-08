/**
 * stack.h
 *
 * Creation Date: 14/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/********************************************************************* STACK */
/*****************************************************************************/

#ifndef CMC_STACK_H
#define CMC_STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STACK_GENERATE(PFX, SNAME, FMOD, V)    \
    STACK_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
    STACK_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* PRIVATE *******************************************************************/
#define STACK_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    STACK_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define STACK_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    STACK_GENERATE_STRUCT(PFX, SNAME, FMOD, V)                \
    STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* PUBLIC ********************************************************************/
#define STACK_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    STACK_GENERATE_STRUCT(PFX, SNAME, FMOD, V)               \
    STACK_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define STACK_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* STRUCT ********************************************************************/
#define STACK_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
                                                   \
    struct SNAME##_s                               \
    {                                              \
        V *buffer;                                 \
        size_t capacity;                           \
        size_t count;                              \
    };                                             \
                                                   \
    struct SNAME##_iter_s                          \
    {                                              \
        struct SNAME##_s *target;                  \
        size_t cursor;                             \
        bool start;                                \
        bool end;                                  \
    };                                             \
                                                   \
/* HEADER ********************************************************************/
#define STACK_GENERATE_HEADER(PFX, SNAME, FMOD, V)                                \
                                                                                  \
    typedef struct SNAME##_s SNAME;                                               \
    typedef struct SNAME##_iter_s SNAME##_iter;                                   \
                                                                                  \
    FMOD SNAME *PFX##_new(size_t size);                                           \
    FMOD void PFX##_clear(SNAME *_stack_);                                        \
    FMOD void PFX##_free(SNAME *_stack_);                                         \
    FMOD bool PFX##_push(SNAME *_stack_, V element);                              \
    FMOD bool PFX##_pop(SNAME *_stack_);                                          \
    FMOD bool PFX##_push_if(SNAME *_stack_, V element, bool condition);           \
    FMOD bool PFX##_pop_if(SNAME *_stack_, bool condition);                       \
    FMOD V PFX##_top(SNAME *_stack_);                                             \
    FMOD bool PFX##_contains(SNAME *_stack_, V element, int (*comparator)(V, V)); \
    FMOD bool PFX##_empty(SNAME *_stack_);                                        \
    FMOD bool PFX##_full(SNAME *_stack_);                                         \
    FMOD size_t PFX##_count(SNAME *_stack_);                                      \
    FMOD size_t PFX##_capacity(SNAME *_stack_);                                   \
                                                                                  \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);                             \
    FMOD void PFX##_iter_free(SNAME##_iter *iter);                                \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                 \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                               \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                 \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                             \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                               \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index);      \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index);      \
                                                                                  \
/* SOURCE ********************************************************************/
#define STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                               \
                                                                                 \
    FMOD bool PFX##_grow(SNAME *_stack_);                                        \
                                                                                 \
    FMOD SNAME *PFX##_new(size_t size)                                           \
    {                                                                            \
        if (size < 1)                                                            \
            return NULL;                                                         \
                                                                                 \
        SNAME *_stack_ = malloc(sizeof(SNAME));                                  \
                                                                                 \
        if (!_stack_)                                                            \
            return NULL;                                                         \
                                                                                 \
        _stack_->buffer = malloc(sizeof(V) * size);                              \
                                                                                 \
        if (!_stack_->buffer)                                                    \
        {                                                                        \
            free(_stack_);                                                       \
            return NULL;                                                         \
        }                                                                        \
                                                                                 \
        memset(_stack_->buffer, 0, sizeof(V) * size);                            \
                                                                                 \
        _stack_->capacity = size;                                                \
        _stack_->count = 0;                                                      \
                                                                                 \
        return _stack_;                                                          \
    }                                                                            \
                                                                                 \
    FMOD void PFX##_clear(SNAME *_stack_)                                        \
    {                                                                            \
        memset(_stack_->buffer, 0, sizeof(V) * _stack_->capacity);               \
                                                                                 \
        _stack_->count = 0;                                                      \
    }                                                                            \
                                                                                 \
    FMOD void PFX##_free(SNAME *_stack_)                                         \
    {                                                                            \
        free(_stack_->buffer);                                                   \
        free(_stack_);                                                           \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_push(SNAME *_stack_, V element)                              \
    {                                                                            \
        if (PFX##_full(_stack_))                                                 \
        {                                                                        \
            if (!PFX##_grow(_stack_))                                            \
                return false;                                                    \
        }                                                                        \
                                                                                 \
        _stack_->buffer[_stack_->count++] = element;                             \
                                                                                 \
        return true;                                                             \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_pop(SNAME *_stack_)                                          \
    {                                                                            \
        if (PFX##_empty(_stack_))                                                \
            return false;                                                        \
                                                                                 \
        _stack_->buffer[--_stack_->count] = 0;                                   \
                                                                                 \
        return true;                                                             \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_push_if(SNAME *_stack_, V element, bool condition)           \
    {                                                                            \
        if (condition)                                                           \
            return PFX##_push(_stack_, element);                                 \
                                                                                 \
        return false;                                                            \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_pop_if(SNAME *_stack_, bool condition)                       \
    {                                                                            \
        if (condition)                                                           \
            return PFX##_pop(_stack_);                                           \
                                                                                 \
        return false;                                                            \
    }                                                                            \
                                                                                 \
    FMOD V PFX##_top(SNAME *_stack_)                                             \
    {                                                                            \
        if (PFX##_empty(_stack_))                                                \
            return 0;                                                            \
                                                                                 \
        return _stack_->buffer[_stack_->count - 1];                              \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_contains(SNAME *_stack_, V element, int (*comparator)(V, V)) \
    {                                                                            \
        for (size_t i = 0; i < _stack_->count; i++)                              \
        {                                                                        \
            if (comparator(_stack_->buffer[i], element) == 0)                    \
                return true;                                                     \
        }                                                                        \
                                                                                 \
        return false;                                                            \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_empty(SNAME *_stack_)                                        \
    {                                                                            \
        return _stack_->count == 0;                                              \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_full(SNAME *_stack_)                                         \
    {                                                                            \
        return _stack_->count >= _stack_->capacity;                              \
    }                                                                            \
                                                                                 \
    FMOD size_t PFX##_count(SNAME *_stack_)                                      \
    {                                                                            \
        return _stack_->count;                                                   \
    }                                                                            \
                                                                                 \
    FMOD size_t PFX##_capacity(SNAME *_stack_)                                   \
    {                                                                            \
        return _stack_->capacity;                                                \
    }                                                                            \
                                                                                 \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target)                             \
    {                                                                            \
        SNAME##_iter *iter = malloc(sizeof(SNAME##_iter));                       \
                                                                                 \
        if (!iter)                                                               \
            return NULL;                                                         \
                                                                                 \
        PFX##_iter_init(iter, target);                                           \
                                                                                 \
        return iter;                                                             \
    }                                                                            \
                                                                                 \
    FMOD void PFX##_iter_free(SNAME##_iter *iter)                                \
    {                                                                            \
        free(iter);                                                              \
    }                                                                            \
                                                                                 \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                 \
    {                                                                            \
        iter->target = target;                                                   \
        iter->cursor = iter->target->count - 1;                                  \
        iter->start = true;                                                      \
        iter->end = PFX##_empty(target);                                         \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                               \
    {                                                                            \
        return PFX##_empty(iter->target) || iter->start;                         \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                 \
    {                                                                            \
        return PFX##_empty(iter->target) || iter->end;                           \
    }                                                                            \
                                                                                 \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter)                             \
    {                                                                            \
        iter->cursor = iter->target->count - 1;                                  \
        iter->start = true;                                                      \
        iter->end = PFX##_empty(iter->target);                                   \
    }                                                                            \
                                                                                 \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter)                               \
    {                                                                            \
        iter->cursor = 0;                                                        \
        iter->start = PFX##_empty(iter->target);                                 \
        iter->end = true;                                                        \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index)      \
    {                                                                            \
        if (iter->end)                                                           \
            return false;                                                        \
                                                                                 \
        *index = iter->target->count - iter->cursor - 1;                         \
        *result = iter->target->buffer[iter->cursor];                            \
        iter->start = false;                                                     \
                                                                                 \
        if (iter->cursor == 0)                                                   \
            iter->end = true;                                                    \
        else                                                                     \
            iter->cursor--;                                                      \
                                                                                 \
        return true;                                                             \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index)      \
    {                                                                            \
        if (iter->start)                                                         \
            return false;                                                        \
                                                                                 \
        *index = iter->target->count - iter->cursor - 1;                         \
        *result = iter->target->buffer[iter->cursor];                            \
        iter->end = false;                                                       \
                                                                                 \
        if (iter->cursor == iter->target->count - 1)                             \
            iter->start = true;                                                  \
        else                                                                     \
            iter->cursor++;                                                      \
                                                                                 \
        return true;                                                             \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_grow(SNAME *_stack_)                                         \
    {                                                                            \
        size_t new_capacity = _stack_->capacity * 2;                             \
                                                                                 \
        V *new_buffer = realloc(_stack_->buffer, sizeof(V) * new_capacity);      \
                                                                                 \
        if (!new_buffer)                                                         \
            return false;                                                        \
                                                                                 \
        _stack_->buffer = new_buffer;                                            \
        _stack_->capacity = new_capacity;                                        \
                                                                                 \
        return true;                                                             \
    }

#endif /* CMC_STACK_H */
