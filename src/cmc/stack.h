/**
 * stack.h
 *
 * Creation Date: 14/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Stack
 *
 * A Stack is a Last-in First-out (or First-in Last-out) data structure used in
 * a variety of algorithms. It is a Dynamic Array that can only add or remove
 * its elements at the end of the buffer, in this case, it represents the top of
 * the stack.
 *
 * It has three main functions: `push` which adds an element at the top of the
 * stack; `pop` which removes the top element from the stack; and `top` which
 * returns the top element without removing it (it is also sometimes called
 * `peek`).
 *
 * A Stack is used in algorithms like backtracking, depth-first search,
 * expression evaluation, syntax parsing and many more.
 */

#ifndef CMC_STACK_H
#define CMC_STACK_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

#define STACK_GENERATE(PFX, SNAME, FMOD, V)    \
    STACK_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

#define STACK_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V) \
    STACK_GENERATE_HEADER(PFX, SNAME, FMOD, V)

#define STACK_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V) \
    STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* HEADER ********************************************************************/
#define STACK_GENERATE_HEADER(PFX, SNAME, FMOD, V)                                \
                                                                                  \
    /* Stack Structure */                                                         \
    typedef struct SNAME##_s                                                      \
    {                                                                             \
        /* Dynamic array of elements */                                           \
        V *buffer;                                                                \
                                                                                  \
        /* Current array capacity */                                              \
        size_t capacity;                                                          \
                                                                                  \
        /* Current amount of elements */                                          \
        size_t count;                                                             \
                                                                                  \
        /* Function that returns an iterator to the start of the stack */         \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                    \
                                                                                  \
        /* Function that returns an iterator to the end of the stack */           \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                      \
                                                                                  \
    } SNAME, *SNAME##_ptr;                                                        \
                                                                                  \
    /* Stack Iterator */                                                          \
    typedef struct SNAME##_iter_s                                                 \
    {                                                                             \
        /* Target stack */                                                        \
        struct SNAME##_s *target;                                                 \
                                                                                  \
        /* Cursor's position (index) */                                           \
        size_t cursor;                                                            \
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
    FMOD void PFX##_clear(SNAME *_stack_);                                        \
    FMOD void PFX##_free(SNAME *_stack_);                                         \
    /* Collection Input and Output */                                             \
    FMOD bool PFX##_push(SNAME *_stack_, V element);                              \
    FMOD bool PFX##_pop(SNAME *_stack_);                                          \
    /* Conditional Input and Output */                                            \
    FMOD bool PFX##_push_if(SNAME *_stack_, V element, bool condition);           \
    FMOD bool PFX##_pop_if(SNAME *_stack_, bool condition);                       \
    /* Element Access */                                                          \
    FMOD V PFX##_top(SNAME *_stack_);                                             \
    /* Collection State */                                                        \
    FMOD bool PFX##_contains(SNAME *_stack_, V element, int (*comparator)(V, V)); \
    FMOD bool PFX##_empty(SNAME *_stack_);                                        \
    FMOD bool PFX##_full(SNAME *_stack_);                                         \
    FMOD size_t PFX##_count(SNAME *_stack_);                                      \
    FMOD size_t PFX##_capacity(SNAME *_stack_);                                   \
    /* Collection Utility */                                                      \
    FMOD cmc_string PFX##_to_string(SNAME *_stack_);                              \
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
#define STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                               \
                                                                                 \
    /* Implementation Detail Functions */                                        \
    static bool PFX##_impl_grow(SNAME *_stack_);                                 \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_stack_);                     \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_stack_);                       \
                                                                                 \
    FMOD SNAME *PFX##_new(size_t capacity)                                       \
    {                                                                            \
        if (capacity < 1)                                                        \
            return NULL;                                                         \
                                                                                 \
        SNAME *_stack_ = malloc(sizeof(SNAME));                                  \
                                                                                 \
        if (!_stack_)                                                            \
            return NULL;                                                         \
                                                                                 \
        _stack_->buffer = malloc(sizeof(V) * capacity);                          \
                                                                                 \
        if (!_stack_->buffer)                                                    \
        {                                                                        \
            free(_stack_);                                                       \
            return NULL;                                                         \
        }                                                                        \
                                                                                 \
        memset(_stack_->buffer, 0, sizeof(V) * capacity);                        \
                                                                                 \
        _stack_->capacity = capacity;                                            \
        _stack_->count = 0;                                                      \
                                                                                 \
        _stack_->it_start = PFX##_impl_it_start;                                 \
        _stack_->it_end = PFX##_impl_it_end;                                     \
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
            if (!PFX##_impl_grow(_stack_))                                       \
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
        _stack_->buffer[--_stack_->count] = PFX##_impl_default_value();          \
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
            return PFX##_impl_default_value();                                   \
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
    FMOD cmc_string PFX##_to_string(SNAME *_stack_)                              \
    {                                                                            \
        cmc_string str;                                                          \
        SNAME *s_ = _stack_;                                                     \
        const char *name = #SNAME;                                               \
                                                                                 \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_stack,                    \
                 name, s_, s_->buffer, s_->capacity, s_->count);                 \
                                                                                 \
        return str;                                                              \
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
                                                                                 \
        if (PFX##_empty(target))                                                 \
            iter->cursor = 0;                                                    \
        else                                                                     \
            iter->cursor = iter->target->count - 1;                              \
                                                                                 \
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
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter)                            \
    {                                                                            \
        iter->cursor = iter->target->count - 1;                                  \
        iter->start = true;                                                      \
        iter->end = PFX##_empty(iter->target);                                   \
    }                                                                            \
                                                                                 \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter)                              \
    {                                                                            \
        iter->cursor = 0;                                                        \
        iter->start = PFX##_empty(iter->target);                                 \
        iter->end = true;                                                        \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter)                                \
    {                                                                            \
        if (iter->end)                                                           \
            return false;                                                        \
                                                                                 \
        iter->start = PFX##_empty(iter->target);                                 \
                                                                                 \
        if (iter->cursor == 0)                                                   \
        {                                                                        \
            iter->end = true;                                                    \
                                                                                 \
            return false;                                                        \
        }                                                                        \
        else                                                                     \
            iter->cursor--;                                                      \
                                                                                 \
        return true;                                                             \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter)                                \
    {                                                                            \
        if (iter->start)                                                         \
            return false;                                                        \
                                                                                 \
        iter->end = PFX##_empty(iter->target);                                   \
                                                                                 \
        if (iter->cursor == iter->target->count - 1)                             \
        {                                                                        \
            iter->start = true;                                                  \
                                                                                 \
            return false;                                                        \
        }                                                                        \
        else                                                                     \
            iter->cursor++;                                                      \
                                                                                 \
        return true;                                                             \
    }                                                                            \
                                                                                 \
    FMOD V PFX##_iter_value(SNAME##_iter *iter)                                  \
    {                                                                            \
        if (PFX##_empty(iter->target))                                           \
            return PFX##_impl_default_value();                                   \
                                                                                 \
        return iter->target->buffer[iter->cursor];                               \
    }                                                                            \
                                                                                 \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter)                                \
    {                                                                            \
        if (PFX##_empty(iter->target))                                           \
            return NULL;                                                         \
                                                                                 \
        return &(iter->target->buffer[iter->cursor]);                            \
    }                                                                            \
                                                                                 \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter)                             \
    {                                                                            \
        if (PFX##_empty(iter->target))                                           \
            return 0;                                                            \
                                                                                 \
        return iter->target->count - 1 - iter->cursor;                           \
    }                                                                            \
                                                                                 \
    static bool PFX##_impl_grow(SNAME *_stack_)                                  \
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
    }                                                                            \
                                                                                 \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_stack_)                      \
    {                                                                            \
        SNAME##_iter iter;                                                       \
                                                                                 \
        PFX##_iter_init(&iter, _stack_);                                         \
        PFX##_iter_to_start(&iter);                                              \
                                                                                 \
        return iter;                                                             \
    }                                                                            \
                                                                                 \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_stack_)                        \
    {                                                                            \
        SNAME##_iter iter;                                                       \
                                                                                 \
        PFX##_iter_init(&iter, _stack_);                                         \
        PFX##_iter_to_end(&iter);                                                \
                                                                                 \
        return iter;                                                             \
    }

#endif /* CMC_STACK_H */
