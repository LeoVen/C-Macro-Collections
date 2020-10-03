/**
 * ext_cmc_heap.h
 *
 * Creation Date: 01/06/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_HEAP_H
#define CMC_EXT_CMC_HEAP_H

#include "cor_core.h"

/**
 * All the EXT parts of CMC Heap.
 */
#define CMC_EXT_CMC_HEAP_PARTS ITER, STR

/**
 * ITER
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_EXT_CMC_HEAP_ITER(ACCESS, FILE, PARAMS) CMC_(CMC_(CMC_EXT_CMC_HEAP_ITER_, ACCESS), CMC_(_, FILE))(PARAMS)

#define CMC_EXT_CMC_HEAP_ITER_PUBLIC_HEADER(PARAMS) \
    CMC_EXT_CMC_HEAP_ITER_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HEAP_ITER_PUBLIC_SOURCE(PARAMS) \
    CMC_EXT_CMC_HEAP_ITER_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HEAP_ITER_PRIVATE_HEADER(PARAMS) \
    CMC_EXT_CMC_HEAP_ITER_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HEAP_ITER_PRIVATE_SOURCE(PARAMS) \
    CMC_EXT_CMC_HEAP_ITER_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HEAP_ITER_HEADER_(PFX, SNAME, V) \
\
    /* Heap Iterator */ \
    struct CMC_DEF_ITER(SNAME) \
    { \
        /* Target heap */ \
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
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#define CMC_EXT_CMC_HEAP_ITER_SOURCE_(PFX, SNAME, V) \
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
        if (!CMC_(PFX, _empty)(target)) \
            iter.cursor = target->count - 1; \
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
            iter->cursor = 0; \
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
            iter->cursor = iter->target->count - 1; \
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
        if (iter->cursor + 1 == iter->target->count) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        iter->start = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor++; \
\
        return true; \
    } \
\
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (iter->start) \
            return false; \
\
        if (iter->cursor == 0) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        iter->end = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor--; \
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
        if (iter->cursor + 1 == iter->target->count) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        if (steps == 0 || iter->cursor + steps >= iter->target->count) \
            return false; \
\
        if (iter->end) \
            return false; \
\
        iter->start = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor += steps; \
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
        if (iter->cursor == 0) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        if (steps == 0 || iter->cursor < steps) \
            return false; \
\
        iter->end = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor -= steps; \
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
            return CMC_(PFX, _iter_rewind)(iter, iter->cursor - index); \
        else if (iter->cursor < index) \
            return CMC_(PFX, _iter_advance)(iter, index - iter->cursor); \
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
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        return iter->cursor; \
    }

/**
 * STR
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_EXT_CMC_HEAP_STR(ACCESS, FILE, PARAMS) CMC_(CMC_(CMC_EXT_CMC_HEAP_STR_, ACCESS), CMC_(_, FILE))(PARAMS)

#define CMC_EXT_CMC_HEAP_STR_PUBLIC_HEADER(PARAMS) \
    CMC_EXT_CMC_HEAP_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HEAP_STR_PUBLIC_SOURCE(PARAMS) \
    CMC_EXT_CMC_HEAP_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HEAP_STR_PRIVATE_HEADER(PARAMS) \
    CMC_EXT_CMC_HEAP_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HEAP_STR_PRIVATE_SOURCE(PARAMS) \
    CMC_EXT_CMC_HEAP_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HEAP_STR_HEADER_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _heap_, FILE * fptr); \
    bool CMC_(PFX, _print)(struct SNAME * _heap_, FILE * fptr, const char *start, const char *separator, \
                           const char *end);

#define CMC_EXT_CMC_HEAP_STR_SOURCE_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _heap_, FILE * fptr) \
    { \
        struct SNAME *h_ = _heap_; \
        const char *t = h_->HO == 1 ? "MaxHeap" : "MinHeap"; \
\
        return 0 <= fprintf(fptr, \
                            "struct %s<%s> " \
                            "at %p { " \
                            "buffer:%p, " \
                            "capacity:%" PRIuMAX ", " \
                            "count:%" PRIuMAX ", " \
                            "type:%s, " \
                            "flag:%d, " \
                            "f_val:%p, " \
                            "alloc:%p, " \
                            "callbacks: %p}", \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(V), h_, h_->buffer, h_->capacity, h_->count, t, \
                            h_->flag, h_->f_val, h_->alloc, CMC_CALLBACKS_GET(h_)); \
    } \
\
    bool CMC_(PFX, _print)(struct SNAME * _heap_, FILE * fptr, const char *start, const char *separator, \
                           const char *end) \
    { \
        fprintf(fptr, "%s", start); \
\
        for (size_t i = 0; i < _heap_->count; i++) \
        { \
            if (!_heap_->f_val->str(fptr, _heap_->buffer[i])) \
                return false; \
\
            if (i + 1 < _heap_->count) \
                fprintf(fptr, "%s", separator); \
        } \
\
        fprintf(fptr, "%s", end); \
\
        return true; \
    }

#endif /* CMC_EXT_CMC_HEAP_H */
