/**
 * ext/cmc/stack.h
 *
 * Creation Date: 07/06/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_STACK_H
#define CMC_EXT_CMC_STACK_H

#include "../../cor/core.h"

/**
 * All the EXT parts of CMC Stack.
 */
#define CMC_EXT_CMC_STACK_PARTS ITER, STR

/**
 * ITER
 */
#define CMC_EXT_CMC_STACK_ITER(PARAMS) \
    CMC_EXT_CMC_STACK_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_STACK_ITER_SOURCE(PARAMS)

#define CMC_EXT_CMC_STACK_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_STACK_ITER_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_STACK_ITER_SOURCE(PARAMS) \
    CMC_EXT_CMC_STACK_ITER_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_STACK_ITER_HEADER_(PFX, SNAME, V) \
\
    /* Stack Iterator */ \
    struct CMC_DEF_ITER(SNAME) \
    { \
        /* Target stack */ \
        struct SNAME *target; \
\
        /* Cursor's position (index) */ \
        size_t cursor; \
\
        /* If the iterator has reached the start of the iteration */ \
        bool start; \
\
        /* If the iterator has reached the end of the iteration */ \
        bool end; \
    }; \
\
    /* Iterator Initialization */ \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target); \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target); \
    /* Iterator State */ \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter); \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter); \
    /* Iterator Movement */ \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter); \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter); \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter); \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter); \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps); \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps); \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter, size_t index); \
    /* Iterator Access */ \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter); \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter); \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#define CMC_EXT_CMC_STACK_ITER_SOURCE_(PFX, SNAME, V) \
\
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target) \
    { \
        struct CMC_DEF_ITER(SNAME) iter; \
\
        iter.target = target; \
        iter.cursor = 0; \
        iter.start = true; \
        iter.end = CMC_(PFX, _empty)(target); \
\
        if (!CMC_(PFX, _empty)(target)) \
            iter.cursor = target->count - 1; \
\
        return iter; \
    } \
\
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target) \
    { \
        struct CMC_DEF_ITER(SNAME) iter; \
\
        iter.target = target; \
        iter.cursor = 0; \
        iter.start = CMC_(PFX, _empty)(target); \
        iter.end = true; \
\
        return iter; \
    } \
\
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        return CMC_(PFX, _empty)(iter->target) || iter->start; \
    } \
\
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        return CMC_(PFX, _empty)(iter->target) || iter->end; \
    } \
\
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (!CMC_(PFX, _empty)(iter->target)) \
        { \
            iter->cursor = iter->target->count - 1; \
            iter->start = true; \
            iter->end = CMC_(PFX, _empty)(iter->target); \
\
            return true; \
        } \
\
        return false; \
    } \
\
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (!CMC_(PFX, _empty)(iter->target)) \
        { \
            iter->cursor = 0; \
            iter->start = CMC_(PFX, _empty)(iter->target); \
            iter->end = true; \
\
            return true; \
        } \
\
        return false; \
    } \
\
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (iter->end) \
            return false; \
\
        if (iter->cursor == 0) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        iter->start = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor--; \
\
        return true; \
    } \
\
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (iter->start) \
            return false; \
\
        if (iter->cursor + 1 == iter->target->count) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        iter->end = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor++; \
\
        return true; \
    } \
\
    /* Returns true only if the iterator moved */ \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps) \
    { \
        if (iter->end) \
            return false; \
\
        if (iter->cursor == 0) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        if (steps == 0 || iter->cursor < steps) \
            return false; \
\
        iter->start = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor -= steps; \
\
        return true; \
    } \
\
    /* Returns true only if the iterator moved */ \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps) \
    { \
        if (iter->start) \
            return false; \
\
        if (iter->cursor + 1 == iter->target->count) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        if (steps == 0 || iter->cursor + steps >= iter->target->count) \
            return false; \
\
        iter->end = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor += steps; \
\
        return true; \
    } \
\
    /* Returns true only if the iterator was able to be positioned at the */ \
    /* given index */ \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter, size_t index) \
    { \
        if (index >= iter->target->count) \
            return false; \
\
        if (iter->cursor > index) \
            return CMC_(PFX, _iter_advance)(iter, iter->cursor - index); \
        else if (iter->cursor < index) \
            return CMC_(PFX, _iter_rewind)(iter, index - iter->cursor); \
\
        return true; \
    } \
\
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return (V){ 0 }; \
\
        return iter->target->buffer[iter->cursor]; \
    } \
\
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return NULL; \
\
        return &(iter->target->buffer[iter->cursor]); \
    } \
\
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return 0; \
\
        return iter->target->count - 1 - iter->cursor; \
    }

/**
 * STR
 */
#define CMC_EXT_CMC_STACK_STR(PARAMS) \
    CMC_EXT_CMC_STACK_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_STACK_STR_SOURCE(PARAMS)

#define CMC_EXT_CMC_STACK_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_STACK_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_STACK_STR_SOURCE(PARAMS) \
    CMC_EXT_CMC_STACK_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_STACK_STR_HEADER_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _stack_, FILE * fptr); \
    bool CMC_(PFX, _print)(struct SNAME * _stack_, FILE * fptr, const char *start, const char *separator, \
                           const char *end);

#define CMC_EXT_CMC_STACK_STR_SOURCE_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _stack_, FILE * fptr) \
    { \
        struct SNAME *s_ = _stack_; \
\
        return 0 <= fprintf(fptr, \
                            "struct %s<%s> " \
                            "at %p { " \
                            "buffer:%p, " \
                            "capacity:%" PRIuMAX ", " \
                            "count:%" PRIuMAX ", " \
                            "flag:%d, " \
                            "f_val:%p, " \
                            "alloc:%p, " \
                            "callbacks:%p }", \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(V), s_, s_->buffer, s_->capacity, s_->count, s_->flag, \
                            s_->f_val, s_->alloc, CMC_CALLBACKS_GET(s_)); \
    } \
\
    bool CMC_(PFX, _print)(struct SNAME * _stack_, FILE * fptr, const char *start, const char *separator, \
                           const char *end) \
    { \
        fprintf(fptr, "%s", start); \
\
        for (size_t i = _stack_->count; i > 0; i--) \
        { \
            if (!_stack_->f_val->str(fptr, _stack_->buffer[i - 1])) \
                return false; \
\
            if (i - 1 != 0) \
                fprintf(fptr, "%s", separator); \
        } \
\
        fprintf(fptr, "%s", end); \
\
        return true; \
    }

#endif /* CMC_EXT_CMC_STACK_H */
