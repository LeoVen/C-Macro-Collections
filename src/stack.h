/**
 * stack.h
 *
 * Last Update: 22/03/2019
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
#include <stdint.h>

#define STACK_GENERATE(PFX, SNAME, FMOD, K, T)    \
    STACK_GENERATE_STRUCT(PFX, SNAME, FMOD, K, T) \
    STACK_GENERATE_HEADER(PFX, SNAME, FMOD, K, T) \
    STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, K, T)

/* PRIVATE *******************************************************************/
#define STACK_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, T) \
    STACK_GENERATE_HEADER(PFX, SNAME, FMOD, K, T)
#define STACK_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, T) \
    STACK_GENERATE_STRUCT(PFX, SNAME, FMOD, K, T)             \
    STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, K, T)
/* PUBLIC ********************************************************************/
#define STACK_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, T) \
    STACK_GENERATE_STRUCT(PFX, SNAME, FMOD, K, T)            \
    STACK_GENERATE_HEADER(PFX, SNAME, FMOD, K, T)
#define STACK_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, T) \
    STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, K, T)
/* STRUCT ********************************************************************/
#define STACK_GENERATE_STRUCT(PFX, SNAME, FMOD, K, T) \
                                                      \
    struct SNAME##_s                                  \
    {                                                 \
        T *buffer;                                    \
        size_t capacity;                              \
        size_t count;                                 \
    };                                                \
                                                      \
    struct SNAME##_iter_s                             \
    {                                                 \
        struct SNAME##_s *target;                     \
        size_t cursor;                                \
        bool start;                                   \
        bool end;                                     \
    };                                                \
                                                      \
/* HEADER ********************************************************************/
#define STACK_GENERATE_HEADER(PFX, SNAME, FMOD, K, T)                        \
                                                                             \
    typedef struct SNAME##_s SNAME;                                          \
    typedef struct SNAME##_iter_s SNAME##_iter;                              \
                                                                             \
    FMOD SNAME *PFX##_new(size_t size);                                      \
    FMOD void PFX##_free(SNAME *_stack_);                                    \
    FMOD bool PFX##_push(SNAME *_stack_, T element);                         \
    FMOD bool PFX##_pop(SNAME *_stack_);                                     \
    FMOD T PFX##_top(SNAME *_stack_);                                        \
    FMOD bool PFX##_push_if(SNAME *_stack_, T element, bool condition);      \
    FMOD bool PFX##_pop_if(SNAME *_stack_, bool condition);                  \
    FMOD bool PFX##_empty(SNAME *_stack_);                                   \
    FMOD bool PFX##_full(SNAME *_stack_);                                    \
    FMOD size_t PFX##_count(SNAME *_stack_);                                 \
    FMOD size_t PFX##_capacity(SNAME *_stack_);                              \
                                                                             \
    FMOD void PFX##_iter(SNAME##_iter *iter, SNAME *target);                 \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                          \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                            \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                        \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                          \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, T *result, size_t *index); \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, T *result, size_t *index); \
                                                                             \
/* SOURCE ********************************************************************/
#define STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, K, T)                       \
                                                                            \
    FMOD bool PFX##_grow(SNAME *_stack_);                                   \
                                                                            \
    FMOD SNAME *PFX##_new(size_t size)                                      \
    {                                                                       \
        if (size < 1)                                                       \
            return NULL;                                                    \
                                                                            \
        SNAME *_stack_ = malloc(sizeof(SNAME));                             \
                                                                            \
        if (!_stack_)                                                       \
            return NULL;                                                    \
                                                                            \
        _stack_->buffer = malloc(sizeof(T) * size);                         \
                                                                            \
        if (!_stack_->buffer)                                               \
        {                                                                   \
            free(_stack_);                                                  \
            return NULL;                                                    \
        }                                                                   \
                                                                            \
        for (size_t i = 0; i < size; i++)                                   \
        {                                                                   \
            _stack_->buffer[i] = 0;                                         \
        }                                                                   \
                                                                            \
        _stack_->capacity = size;                                           \
        _stack_->count = 0;                                                 \
                                                                            \
        return _stack_;                                                     \
    }                                                                       \
                                                                            \
    FMOD void PFX##_free(SNAME *_stack_)                                    \
    {                                                                       \
        free(_stack_->buffer);                                              \
        free(_stack_);                                                      \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_push(SNAME *_stack_, T element)                         \
    {                                                                       \
        if (PFX##_full(_stack_))                                            \
        {                                                                   \
            if (!PFX##_grow(_stack_))                                       \
                return false;                                               \
        }                                                                   \
                                                                            \
        _stack_->buffer[_stack_->count++] = element;                        \
                                                                            \
        return true;                                                        \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_pop(SNAME *_stack_)                                     \
    {                                                                       \
        if (PFX##_empty(_stack_))                                           \
            return false;                                                   \
                                                                            \
        _stack_->buffer[--_stack_->count] = 0;                              \
                                                                            \
        return true;                                                        \
    }                                                                       \
                                                                            \
    FMOD T PFX##_top(SNAME *_stack_)                                        \
    {                                                                       \
        if (PFX##_empty(_stack_))                                           \
            return 0;                                                       \
                                                                            \
        return _stack_->buffer[_stack_->count - 1];                         \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_push_if(SNAME *_stack_, T element, bool condition)      \
    {                                                                       \
        if (condition)                                                      \
            return PFX##_push(_stack_, element);                            \
                                                                            \
        return false;                                                       \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_pop_if(SNAME *_stack_, bool condition)                  \
    {                                                                       \
        if (condition)                                                      \
            return PFX##_pop(_stack_);                                      \
                                                                            \
        return false;                                                       \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_empty(SNAME *_stack_)                                   \
    {                                                                       \
        return _stack_->count == 0;                                         \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_full(SNAME *_stack_)                                    \
    {                                                                       \
        return _stack_->count >= _stack_->capacity;                         \
    }                                                                       \
                                                                            \
    FMOD size_t PFX##_count(SNAME *_stack_)                                 \
    {                                                                       \
        return _stack_->count;                                              \
    }                                                                       \
                                                                            \
    FMOD size_t PFX##_capacity(SNAME *_stack_)                              \
    {                                                                       \
        return _stack_->capacity;                                           \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_grow(SNAME *_stack_)                                    \
    {                                                                       \
        size_t new_capacity = _stack_->capacity * 2;                        \
                                                                            \
        T *new_buffer = realloc(_stack_->buffer, sizeof(T) * new_capacity); \
                                                                            \
        if (!new_buffer)                                                    \
            return false;                                                   \
                                                                            \
        _stack_->buffer = new_buffer;                                       \
        _stack_->capacity = new_capacity;                                   \
                                                                            \
        return true;                                                        \
    }                                                                       \
                                                                            \
    FMOD void PFX##_iter(SNAME##_iter *iter, SNAME *target)                 \
    {                                                                       \
        iter->target = target;                                              \
        iter->cursor = iter->target->count - 1;                             \
        iter->start = true;                                                 \
        iter->end = false;                                                  \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                          \
    {                                                                       \
        return iter->cursor == iter->target->count - 1 && iter->start;      \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                            \
    {                                                                       \
        return iter->cursor == 0 && iter->end;                              \
    }                                                                       \
                                                                            \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter)                        \
    {                                                                       \
        iter->cursor = iter->target->count - 1;                             \
        iter->start = true;                                                 \
        iter->end = false;                                                  \
    }                                                                       \
                                                                            \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter)                          \
    {                                                                       \
        iter->cursor = 0;                                                   \
        iter->start = false;                                                \
        iter->end = true;                                                   \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, T *result, size_t *index) \
    {                                                                       \
        if (iter->end)                                                      \
            return false;                                                   \
                                                                            \
        *index = iter->cursor;                                              \
        *result = iter->target->buffer[iter->cursor];                       \
        iter->start = false;                                                \
                                                                            \
        if (iter->cursor == 0)                                              \
            iter->end = true;                                               \
        else                                                                \
            iter->cursor--;                                                 \
                                                                            \
        return true;                                                        \
    }                                                                       \
                                                                            \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, T *result, size_t *index) \
    {                                                                       \
        if (iter->start)                                                    \
            return false;                                                   \
                                                                            \
        *index = iter->cursor;                                              \
        *result = iter->target->buffer[iter->cursor];                       \
        iter->end = false;                                                  \
                                                                            \
        if (iter->cursor == iter->target->count - 1)                        \
            iter->start = true;                                             \
        else                                                                \
            iter->cursor++;                                                 \
                                                                            \
        return true;                                                        \
    }

#endif /* CMC_STACK_H */
