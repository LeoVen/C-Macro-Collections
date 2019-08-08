/**
 * stack.h
 *
 * Creation Date: 21/06/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * A Purely Stack Allocated Stack
 *
 * This means that the Stack has a fixed size, but it also has the advantage of
 * not requiring for manually allocating and deallocating the struct.
 *
 * Generating a collection with too big of a storage can be dangerous and might
 * quickly cause a stack overflow.
 *
 * It is recommended to not generate any Stack Allocated Collection with less
 * than 1000 internal storage.
 */

#ifndef CMC_SAC_STACK_H
#define CMC_SAC_STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SAC_STACK_GENERATE(PFX, SNAME, FMOD, V, SIZE)    \
    SAC_STACK_GENERATE_HEADER(PFX, SNAME, FMOD, V, SIZE) \
    SAC_STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, V, SIZE)

#define SAC_STACK_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V, SIZE) \
    SAC_STACK_GENERATE_HEADER(PFX, SNAME, FMOD, V, SIZE)

#define SAC_STACK_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V, SIZE) \
    SAC_STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, V, SIZE)

/* HEADER ********************************************************************/
#define SAC_STACK_GENERATE_HEADER(PFX, SNAME, FMOD, V, SIZE)                      \
                                                                                  \
    /* Stack Structure */                                                         \
    typedef struct SNAME##_s                                                      \
    {                                                                             \
        /* Internal Storage */                                                    \
        V buffer[SIZE];                                                           \
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
    FMOD SNAME PFX##_new(void);                                                   \
    FMOD void PFX##_clear(SNAME *_stack_);                                        \
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
    FMOD size_t PFX##_capacity(void);                                             \
                                                                                  \
    /* Iterator Functions */                                                      \
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
#define SAC_STACK_GENERATE_SOURCE(PFX, SNAME, FMOD, V, SIZE)                     \
                                                                                 \
    /* Implementation Detail Functions */                                        \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_stack_);                     \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_stack_);                       \
                                                                                 \
    FMOD SNAME PFX##_new(void)                                                   \
    {                                                                            \
        SNAME result;                                                            \
                                                                                 \
        memset(&result, 0, sizeof(SNAME));                                       \
                                                                                 \
        result.it_start = PFX##_impl_it_start;                                   \
        result.it_end = PFX##_impl_it_end;                                       \
                                                                                 \
        return result;                                                           \
    }                                                                            \
                                                                                 \
    FMOD void PFX##_clear(SNAME *_stack_)                                        \
    {                                                                            \
        memset(_stack_->buffer, 0, sizeof(_stack_->buffer));                     \
                                                                                 \
        _stack_->count = 0;                                                      \
    }                                                                            \
                                                                                 \
    FMOD bool PFX##_push(SNAME *_stack_, V element)                              \
    {                                                                            \
        if (PFX##_full(_stack_))                                                 \
            return false;                                                        \
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
        return _stack_->count >= SIZE;                                           \
    }                                                                            \
                                                                                 \
    FMOD size_t PFX##_count(SNAME *_stack_)                                      \
    {                                                                            \
        return _stack_->count;                                                   \
    }                                                                            \
                                                                                 \
    FMOD size_t PFX##_capacity(void)                                             \
    {                                                                            \
        return SIZE;                                                             \
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
            iter->end = true;                                                    \
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
            iter->start = true;                                                  \
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
        return iter->target->count - 1 - iter->cursor;                           \
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

#endif /* CMC_SAC_STACK_H */
