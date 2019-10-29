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

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_stack = "%s at %p { buffer:%p, capacity:%" PRIuMAX ", count:%" PRIuMAX " }";

#define CMC_GENERATE_STACK(PFX, SNAME, V)    \
    CMC_GENERATE_STACK_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_STACK_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_STACK_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_STACK_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_STACK_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_STACK_SOURCE(PFX, SNAME, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_STACK_HEADER(PFX, SNAME, V)                                                \
                                                                                                \
    /* Stack Structure */                                                                       \
    struct SNAME                                                                                \
    {                                                                                           \
        /* Dynamic array of elements */                                                         \
        V *buffer;                                                                              \
                                                                                                \
        /* Current array capacity */                                                            \
        size_t capacity;                                                                        \
                                                                                                \
        /* Current amount of elements */                                                        \
        size_t count;                                                                           \
                                                                                                \
        /* Function that returns an iterator to the start of the stack */                       \
        struct SNAME##_iter (*it_start)(struct SNAME *);                                        \
                                                                                                \
        /* Function that returns an iterator to the end of the stack */                         \
        struct SNAME##_iter (*it_end)(struct SNAME *);                                          \
    };                                                                                          \
                                                                                                \
    /* Stack Iterator */                                                                        \
    struct SNAME##_iter                                                                         \
    {                                                                                           \
        /* Target stack */                                                                      \
        struct SNAME *target;                                                                   \
                                                                                                \
        /* Cursor's position (index) */                                                         \
        size_t cursor;                                                                          \
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
    void PFX##_clear(struct SNAME *_stack_, void (*deallocator)(V));                            \
    void PFX##_free(struct SNAME *_stack_, void (*deallocator)(V));                             \
    /* Collection Input and Output */                                                           \
    bool PFX##_push(struct SNAME *_stack_, V element);                                          \
    bool PFX##_pop(struct SNAME *_stack_);                                                      \
    /* Element Access */                                                                        \
    V PFX##_top(struct SNAME *_stack_);                                                         \
    /* Collection State */                                                                      \
    bool PFX##_contains(struct SNAME *_stack_, V element, int (*comparator)(V, V));             \
    bool PFX##_empty(struct SNAME *_stack_);                                                    \
    bool PFX##_full(struct SNAME *_stack_);                                                     \
    size_t PFX##_count(struct SNAME *_stack_);                                                  \
    size_t PFX##_capacity(struct SNAME *_stack_);                                               \
    /* Collection Utility */                                                                    \
    bool PFX##_resize(struct SNAME *_stack_, size_t capacity);                                  \
    struct SNAME *PFX##_copy_of(struct SNAME *_stack_, V (*copy_func)(V));                      \
    bool PFX##_equals(struct SNAME *_stack1_, struct SNAME *_stack2_, int (*comparator)(V, V)); \
    struct cmc_string PFX##_to_string(struct SNAME *_stack_);                                   \
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
#define CMC_GENERATE_STACK_SOURCE(PFX, SNAME, V)                                               \
                                                                                               \
    /* Implementation Detail Functions */                                                      \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_stack_);                     \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_stack_);                       \
                                                                                               \
    struct SNAME *PFX##_new(size_t capacity)                                                   \
    {                                                                                          \
        if (capacity < 1)                                                                      \
            return NULL;                                                                       \
                                                                                               \
        struct SNAME *_stack_ = malloc(sizeof(struct SNAME));                                  \
                                                                                               \
        if (!_stack_)                                                                          \
            return NULL;                                                                       \
                                                                                               \
        _stack_->buffer = calloc(capacity, sizeof(V));                                         \
                                                                                               \
        if (!_stack_->buffer)                                                                  \
        {                                                                                      \
            free(_stack_);                                                                     \
            return NULL;                                                                       \
        }                                                                                      \
                                                                                               \
        _stack_->capacity = capacity;                                                          \
        _stack_->count = 0;                                                                    \
                                                                                               \
        _stack_->it_start = PFX##_impl_it_start;                                               \
        _stack_->it_end = PFX##_impl_it_end;                                                   \
                                                                                               \
        return _stack_;                                                                        \
    }                                                                                          \
                                                                                               \
    void PFX##_clear(struct SNAME *_stack_, void (*deallocator)(V))                            \
    {                                                                                          \
        if (deallocator)                                                                       \
        {                                                                                      \
            for (size_t i = 0; i < _stack_->count; i++)                                        \
                deallocator(_stack_->buffer[i]);                                               \
        }                                                                                      \
                                                                                               \
        memset(_stack_->buffer, 0, sizeof(V) * _stack_->capacity);                             \
                                                                                               \
        _stack_->count = 0;                                                                    \
    }                                                                                          \
                                                                                               \
    void PFX##_free(struct SNAME *_stack_, void (*deallocator)(V))                             \
    {                                                                                          \
        free(_stack_->buffer);                                                                 \
        if (deallocator)                                                                       \
        {                                                                                      \
            for (size_t i = 0; i < _stack_->count; i++)                                        \
                deallocator(_stack_->buffer[i]);                                               \
        }                                                                                      \
                                                                                               \
        free(_stack_);                                                                         \
    }                                                                                          \
                                                                                               \
    bool PFX##_push(struct SNAME *_stack_, V element)                                          \
    {                                                                                          \
        if (PFX##_full(_stack_))                                                               \
        {                                                                                      \
            if (!PFX##_resize(_stack_, PFX##_capacity(_stack_) * 2))                           \
                return false;                                                                  \
        }                                                                                      \
                                                                                               \
        _stack_->buffer[_stack_->count++] = element;                                           \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    bool PFX##_pop(struct SNAME *_stack_)                                                      \
    {                                                                                          \
        if (PFX##_empty(_stack_))                                                              \
            return false;                                                                      \
                                                                                               \
        _stack_->buffer[--_stack_->count] = (V){0};                                            \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    V PFX##_top(struct SNAME *_stack_)                                                         \
    {                                                                                          \
        if (PFX##_empty(_stack_))                                                              \
            return (V){0};                                                                     \
                                                                                               \
        return _stack_->buffer[_stack_->count - 1];                                            \
    }                                                                                          \
                                                                                               \
    bool PFX##_contains(struct SNAME *_stack_, V element, int (*comparator)(V, V))             \
    {                                                                                          \
        for (size_t i = 0; i < _stack_->count; i++)                                            \
        {                                                                                      \
            if (comparator(_stack_->buffer[i], element) == 0)                                  \
                return true;                                                                   \
        }                                                                                      \
                                                                                               \
        return false;                                                                          \
    }                                                                                          \
                                                                                               \
    bool PFX##_empty(struct SNAME *_stack_)                                                    \
    {                                                                                          \
        return _stack_->count == 0;                                                            \
    }                                                                                          \
                                                                                               \
    bool PFX##_full(struct SNAME *_stack_)                                                     \
    {                                                                                          \
        return _stack_->count >= _stack_->capacity;                                            \
    }                                                                                          \
                                                                                               \
    size_t PFX##_count(struct SNAME *_stack_)                                                  \
    {                                                                                          \
        return _stack_->count;                                                                 \
    }                                                                                          \
                                                                                               \
    size_t PFX##_capacity(struct SNAME *_stack_)                                               \
    {                                                                                          \
        return _stack_->capacity;                                                              \
    }                                                                                          \
                                                                                               \
    bool PFX##_resize(struct SNAME *_stack_, size_t capacity)                                  \
    {                                                                                          \
        if (PFX##_capacity(_stack_) == capacity)                                               \
            return true;                                                                       \
                                                                                               \
        if (capacity < PFX##_count(_stack_))                                                   \
            return false;                                                                      \
                                                                                               \
        V *new_buffer = realloc(_stack_->buffer, sizeof(V) * capacity);                        \
                                                                                               \
        if (!new_buffer)                                                                       \
            return false;                                                                      \
                                                                                               \
        _stack_->buffer = new_buffer;                                                          \
        _stack_->capacity = capacity;                                                          \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    struct SNAME *PFX##_copy_of(struct SNAME *_stack_, V (*copy_func)(V))                      \
    {                                                                                          \
        struct SNAME *result = PFX##_new(_stack_->capacity);                                   \
                                                                                               \
        if (!result)                                                                           \
            return NULL;                                                                       \
                                                                                               \
        if (copy_func)                                                                         \
        {                                                                                      \
            for (size_t i = 0; i < _stack_->count; i++)                                        \
                result->buffer[i] = copy_func(_stack_->buffer[i]);                             \
        }                                                                                      \
        else                                                                                   \
            memcpy(result->buffer, _stack_->buffer, sizeof(V) * _stack_->count);               \
                                                                                               \
        result->count = _stack_->count;                                                        \
                                                                                               \
        return result;                                                                         \
    }                                                                                          \
                                                                                               \
    bool PFX##_equals(struct SNAME *_stack1_, struct SNAME *_stack2_, int (*comparator)(V, V)) \
    {                                                                                          \
        if (PFX##_count(_stack1_) != PFX##_count(_stack2_))                                    \
            return false;                                                                      \
                                                                                               \
        for (size_t i = 0; i < PFX##_count(_stack1_); i++)                                     \
        {                                                                                      \
            if (comparator(_stack1_->buffer[i], _stack2_->buffer[i]) != 0)                     \
                return false;                                                                  \
        }                                                                                      \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    struct cmc_string PFX##_to_string(struct SNAME *_stack_)                                   \
    {                                                                                          \
        struct cmc_string str;                                                                 \
        struct SNAME *s_ = _stack_;                                                            \
        const char *name = #SNAME;                                                             \
                                                                                               \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_stack,                                  \
                 name, s_, s_->buffer, s_->capacity, s_->count);                               \
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
        iter->cursor = PFX##_empty(target) ? 0 : iter->target->count - 1;                      \
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
        if (!PFX##_empty(iter->target))                                                        \
        {                                                                                      \
            iter->cursor = iter->target->count - 1;                                            \
            iter->start = true;                                                                \
            iter->end = PFX##_empty(iter->target);                                             \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                                          \
    {                                                                                          \
        if (!PFX##_empty(iter->target))                                                        \
        {                                                                                      \
            iter->cursor = 0;                                                                  \
            iter->start = PFX##_empty(iter->target);                                           \
            iter->end = true;                                                                  \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                                            \
    {                                                                                          \
        if (iter->end)                                                                         \
            return false;                                                                      \
                                                                                               \
        if (iter->cursor == 0)                                                                 \
        {                                                                                      \
            iter->end = true;                                                                  \
            return false;                                                                      \
        }                                                                                      \
                                                                                               \
        iter->start = PFX##_empty(iter->target);                                               \
                                                                                               \
        iter->cursor--;                                                                        \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                                            \
    {                                                                                          \
        if (iter->start)                                                                       \
            return false;                                                                      \
                                                                                               \
        if (iter->cursor + 1 == PFX##_count(iter->target))                                     \
        {                                                                                      \
            iter->start = true;                                                                \
            return false;                                                                      \
        }                                                                                      \
                                                                                               \
        iter->end = PFX##_empty(iter->target);                                                 \
                                                                                               \
        iter->cursor++;                                                                        \
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
        if (iter->cursor == 0)                                                                 \
        {                                                                                      \
            iter->end = true;                                                                  \
            return false;                                                                      \
        }                                                                                      \
                                                                                               \
        if (steps == 0 || iter->cursor < steps)                                                \
            return false;                                                                      \
                                                                                               \
        iter->start = PFX##_empty(iter->target);                                               \
                                                                                               \
        iter->cursor -= steps;                                                                 \
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
        if (iter->cursor + 1 == PFX##_count(iter->target))                                     \
        {                                                                                      \
            iter->start = true;                                                                \
            return false;                                                                      \
        }                                                                                      \
                                                                                               \
        if (steps == 0 || iter->cursor + steps >= PFX##_count(iter->target))                   \
            return false;                                                                      \
                                                                                               \
        iter->end = PFX##_empty(iter->target);                                                 \
                                                                                               \
        iter->cursor += steps;                                                                 \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    /* Returns true only if the iterator was able to be positioned at the */                   \
    /* given index */                                                                          \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)                             \
    {                                                                                          \
        if (index >= PFX##_count(iter->target))                                                \
            return false;                                                                      \
                                                                                               \
        if (iter->cursor > index)                                                              \
            return PFX##_iter_rewind(iter, iter->cursor - index);                              \
        else if (iter->cursor < index)                                                         \
            return PFX##_iter_advance(iter, index - iter->cursor);                             \
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
        if (PFX##_empty(iter->target))                                                         \
            return 0;                                                                          \
                                                                                               \
        return iter->target->count - 1 - iter->cursor;                                         \
    }                                                                                          \
                                                                                               \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_stack_)                      \
    {                                                                                          \
        struct SNAME##_iter iter;                                                              \
                                                                                               \
        PFX##_iter_init(&iter, _stack_);                                                       \
        PFX##_iter_to_start(&iter);                                                            \
                                                                                               \
        return iter;                                                                           \
    }                                                                                          \
                                                                                               \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_stack_)                        \
    {                                                                                          \
        struct SNAME##_iter iter;                                                              \
                                                                                               \
        PFX##_iter_init(&iter, _stack_);                                                       \
        PFX##_iter_to_end(&iter);                                                              \
                                                                                               \
        return iter;                                                                           \
    }

#endif /* CMC_STACK_H */
