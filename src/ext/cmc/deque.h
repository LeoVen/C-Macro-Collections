/**
 * deque.h
 *
 * Creation Date: 25/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_DEQUE_H
#define CMC_EXT_CMC_DEQUE_H

#include "../../cor/core.h"

/**
 * All the EXT parts of CMC Deque.
 */
#define CMC_EXT_CMC_DEQUE_PARTS INIT, ITER, STR

/* Used by STR */
static const char *cmc_cmc_string_fmt_deque = "struct %s<%s> "
                                              "at %p { "
                                              "buffer:%p, "
                                              "capacity:%" PRIuMAX ", "
                                              "count:%" PRIuMAX ", "
                                              "front:%" PRIuMAX ", "
                                              "back:%" PRIuMAX ", "
                                              "flag:%d, "
                                              "f_val:%p, "
                                              "alloc:%p, "
                                              "callbacks:%p }";

/**
 * INIT
 *
 * The part 'INIT' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers).
 */
#define CMC_EXT_CMC_DEQUE_INIT(BODY)    \
    CMC_EXT_CMC_DEQUE_INIT_HEADER(BODY) \
    CMC_EXT_CMC_DEQUE_INIT_SOURCE(BODY)

#define CMC_EXT_CMC_DEQUE_INIT_HEADER(BODY)                                    \
    CMC_EXT_CMC_DEQUE_INIT_HEADER_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                                   CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_DEQUE_INIT_SOURCE(BODY)                                    \
    CMC_EXT_CMC_DEQUE_INIT_SOURCE_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                                   CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_DEQUE_INIT_HEADER_(PFX, SNAME, V)                  \
                                                                       \
    struct SNAME CMC_(PFX, _init)(size_t capacity,                     \
                                  struct CMC_DEF_FVAL(SNAME) * f_val); \
    struct SNAME CMC_(PFX, _init_custom)(                              \
        size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,           \
        struct cmc_alloc_node * alloc, struct cmc_callbacks * callbacks);

#define CMC_EXT_CMC_DEQUE_INIT_SOURCE_(PFX, SNAME, V)                    \
                                                                         \
    struct SNAME CMC_(PFX, _init)(size_t capacity,                       \
                                  struct CMC_DEF_FVAL(SNAME) * f_val)    \
    {                                                                    \
        return CMC_(PFX, _init_custom)(capacity, f_val, NULL, NULL);     \
    }                                                                    \
                                                                         \
    struct SNAME CMC_(PFX, _init_custom)(                                \
        size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,             \
        struct cmc_alloc_node * alloc, struct cmc_callbacks * callbacks) \
    {                                                                    \
        struct SNAME _deque_ = { 0 };                                    \
                                                                         \
        if (capacity < 1)                                                \
            return _deque_;                                              \
                                                                         \
        if (!f_val)                                                      \
            return _deque_;                                              \
                                                                         \
        if (!alloc)                                                      \
            alloc = &cmc_alloc_node_default;                             \
                                                                         \
        _deque_.buffer = alloc->calloc(capacity, sizeof(V));             \
                                                                         \
        if (!_deque_.buffer)                                             \
            return _deque_;                                              \
                                                                         \
        _deque_.capacity = capacity;                                     \
        _deque_.count = 0;                                               \
        _deque_.front = 0;                                               \
        _deque_.back = 0;                                                \
        _deque_.flag = CMC_FLAG_OK;                                      \
        _deque_.f_val = f_val;                                           \
        _deque_.alloc = alloc;                                           \
        _deque_.callbacks = callbacks;                                   \
                                                                         \
        return _deque_;                                                  \
    }

/**
 * ITER
 */
#define CMC_EXT_CMC_DEQUE_ITER(BODY)    \
    CMC_EXT_CMC_DEQUE_ITER_HEADER(BODY) \
    CMC_EXT_CMC_DEQUE_ITER_SOURCE(BODY)

#define CMC_EXT_CMC_DEQUE_ITER_HEADER(BODY)                                    \
    CMC_EXT_CMC_DEQUE_ITER_HEADER_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                                   CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_DEQUE_ITER_SOURCE(BODY)                                    \
    CMC_EXT_CMC_DEQUE_ITER_SOURCE_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                                   CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_DEQUE_ITER_HEADER_(PFX, SNAME, V)                         \
                                                                              \
    /* Deque Iterator */                                                      \
    struct CMC_DEF_ITER(SNAME)                                                \
    {                                                                         \
        /* Target deque */                                                    \
        struct SNAME *target;                                                 \
                                                                              \
        /* Cursor's position (index) */                                       \
        size_t cursor;                                                        \
                                                                              \
        /* Keeps track of relative index to the iteration of elements */      \
        size_t index;                                                         \
                                                                              \
        /* If the iterator has reached the start of the iteration */          \
        bool start;                                                           \
                                                                              \
        /* If the iterator has reached the end of the iteration */            \
        bool end;                                                             \
    };                                                                        \
                                                                              \
    /* Iterator Initialization */                                             \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target); \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target);   \
    /* Iterator State */                                                      \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter);        \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter);          \
    /* Iterator Movement */                                                   \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter);        \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter);          \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter);            \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter);            \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,          \
                                  size_t steps);                              \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                 size_t steps);                               \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter,            \
                                size_t index);                                \
    /* Iterator Access */                                                     \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter);              \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter);            \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#define CMC_EXT_CMC_DEQUE_ITER_SOURCE_(PFX, SNAME, V)                        \
                                                                             \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target) \
    {                                                                        \
        struct CMC_DEF_ITER(SNAME) iter;                                     \
                                                                             \
        iter.target = target;                                                \
        iter.cursor = target->front;                                         \
        iter.index = 0;                                                      \
        iter.start = true;                                                   \
        iter.end = CMC_(PFX, _empty)(target);                                \
                                                                             \
        return iter;                                                         \
    }                                                                        \
                                                                             \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target)   \
    {                                                                        \
        struct CMC_DEF_ITER(SNAME) iter;                                     \
                                                                             \
        iter.target = target;                                                \
                                                                             \
        if (!CMC_(PFX, _empty)(target))                                      \
        {                                                                    \
            if (iter.target->back == 0)                                      \
                iter.cursor = iter.target->capacity - 1;                     \
            else                                                             \
                iter.cursor = iter.target->back - 1;                         \
                                                                             \
            iter.index = iter.target->count - 1;                             \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            iter.cursor = 0;                                                 \
            iter.index = 0;                                                  \
        }                                                                    \
                                                                             \
        iter.start = CMC_(PFX, _empty)(target);                              \
        iter.end = true;                                                     \
                                                                             \
        return iter;                                                         \
    }                                                                        \
                                                                             \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter)        \
    {                                                                        \
        return CMC_(PFX, _empty)(iter->target) || iter->start;               \
    }                                                                        \
                                                                             \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                        \
        return CMC_(PFX, _empty)(iter->target) || iter->end;                 \
    }                                                                        \
                                                                             \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)        \
    {                                                                        \
        if (!CMC_(PFX, _empty)(iter->target))                                \
        {                                                                    \
            iter->cursor = iter->target->front;                              \
            iter->index = 0;                                                 \
            iter->start = true;                                              \
            iter->end = false;                                               \
                                                                             \
            return true;                                                     \
        }                                                                    \
                                                                             \
        return false;                                                        \
    }                                                                        \
                                                                             \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                        \
        if (!CMC_(PFX, _empty)(iter->target))                                \
        {                                                                    \
            if (iter->target->back == 0)                                     \
                iter->cursor = iter->target->capacity - 1;                   \
            else                                                             \
                iter->cursor = iter->target->back - 1;                       \
                                                                             \
            iter->index = iter->target->count - 1;                           \
                                                                             \
            iter->start = false;                                             \
            iter->end = true;                                                \
                                                                             \
            return true;                                                     \
        }                                                                    \
                                                                             \
        return false;                                                        \
    }                                                                        \
                                                                             \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                        \
        if (iter->end)                                                       \
            return false;                                                    \
                                                                             \
        if (iter->index + 1 == iter->target->count)                          \
        {                                                                    \
            iter->end = true;                                                \
            return false;                                                    \
        }                                                                    \
                                                                             \
        iter->start = CMC_(PFX, _empty)(iter->target);                       \
                                                                             \
        iter->cursor = (iter->cursor + 1) % (iter->target->capacity);        \
        iter->index++;                                                       \
                                                                             \
        return true;                                                         \
    }                                                                        \
                                                                             \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                        \
        if (iter->start)                                                     \
            return false;                                                    \
                                                                             \
        if (iter->index == 0)                                                \
        {                                                                    \
            iter->start = true;                                              \
            return false;                                                    \
        }                                                                    \
                                                                             \
        iter->end = CMC_(PFX, _empty)(iter->target);                         \
                                                                             \
        iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1      \
                                           : iter->cursor - 1;               \
        iter->index--;                                                       \
                                                                             \
        return true;                                                         \
    }                                                                        \
                                                                             \
    /* Returns true only if the iterator moved */                            \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,         \
                                  size_t steps)                              \
    {                                                                        \
        if (iter->end)                                                       \
            return false;                                                    \
                                                                             \
        if (iter->index + 1 == iter->target->count)                          \
        {                                                                    \
            iter->end = true;                                                \
            return false;                                                    \
        }                                                                    \
                                                                             \
        if (steps == 0 || iter->index + steps >= iter->target->count)        \
            return false;                                                    \
                                                                             \
        iter->start = CMC_(PFX, _empty)(iter->target);                       \
                                                                             \
        iter->index += steps;                                                \
        iter->cursor = (iter->cursor + steps) % iter->target->capacity;      \
                                                                             \
        return true;                                                         \
    }                                                                        \
                                                                             \
    /* Returns true only if the iterator moved */                            \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter,          \
                                 size_t steps)                               \
    {                                                                        \
        if (iter->start)                                                     \
            return false;                                                    \
                                                                             \
        if (iter->index == 0)                                                \
        {                                                                    \
            iter->start = true;                                              \
            return false;                                                    \
        }                                                                    \
                                                                             \
        if (steps == 0 || iter->index < steps)                               \
            return false;                                                    \
                                                                             \
        iter->end = CMC_(PFX, _empty)(iter->target);                         \
                                                                             \
        iter->index -= steps;                                                \
                                                                             \
        /* Prevent underflow */                                              \
        if (iter->cursor < steps)                                            \
            iter->cursor += iter->target->capacity;                          \
                                                                             \
        iter->cursor -= steps;                                               \
                                                                             \
        return true;                                                         \
    }                                                                        \
                                                                             \
    /* Returns true only if the iterator was able to be positioned at the */ \
    /* given index */                                                        \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                size_t index)                                \
    {                                                                        \
        if (index >= iter->target->count)                                    \
            return false;                                                    \
                                                                             \
        if (iter->index > index)                                             \
            return CMC_(PFX, _iter_rewind)(iter, iter->index - index);       \
        else if (iter->index < index)                                        \
            return CMC_(PFX, _iter_advance)(iter, index - iter->index);      \
                                                                             \
        return true;                                                         \
    }                                                                        \
                                                                             \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                        \
        if (CMC_(PFX, _empty)(iter->target))                                 \
            return (V){ 0 };                                                 \
                                                                             \
        return iter->target->buffer[iter->cursor];                           \
    }                                                                        \
                                                                             \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                        \
        if (CMC_(PFX, _empty)(iter->target))                                 \
            return NULL;                                                     \
                                                                             \
        return &(iter->target->buffer[iter->cursor]);                        \
    }                                                                        \
                                                                             \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)         \
    {                                                                        \
        return iter->index;                                                  \
    }

/**
 * STR
 */
#define CMC_EXT_CMC_DEQUE_STR(BODY)    \
    CMC_EXT_CMC_DEQUE_STR_HEADER(BODY) \
    CMC_EXT_CMC_DEQUE_STR_SOURCE(BODY)

#define CMC_EXT_CMC_DEQUE_STR_HEADER(BODY)                                    \
    CMC_EXT_CMC_DEQUE_STR_HEADER_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                                  CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_DEQUE_STR_SOURCE(BODY)                                    \
    CMC_EXT_CMC_DEQUE_STR_SOURCE_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                                  CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_DEQUE_STR_HEADER_(PFX, SNAME, V)                 \
                                                                     \
    bool CMC_(PFX, _to_string)(struct SNAME * _deque_, FILE * fptr); \
    bool CMC_(PFX, _print)(struct SNAME * _deque_, FILE * fptr,      \
                           const char *start, const char *separator, \
                           const char *end);

#define CMC_EXT_CMC_DEQUE_STR_SOURCE_(PFX, SNAME, V)                        \
                                                                            \
    bool CMC_(PFX, _to_string)(struct SNAME * _deque_, FILE * fptr)         \
    {                                                                       \
        struct SNAME *d_ = _deque_;                                         \
                                                                            \
        return 0 <= fprintf(fptr, cmc_cmc_string_fmt_deque,                 \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(V), d_,     \
                            d_->buffer, d_->capacity, d_->count, d_->front, \
                            d_->back, d_->flag, d_->f_val, d_->alloc,       \
                            d_->callbacks);                                 \
    }                                                                       \
                                                                            \
    bool CMC_(PFX, _print)(struct SNAME * _deque_, FILE * fptr,             \
                           const char *start, const char *separator,        \
                           const char *end)                                 \
    {                                                                       \
        fprintf(fptr, "%s", start);                                         \
                                                                            \
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)     \
        {                                                                   \
            if (!_deque_->f_val->str(fptr, _deque_->buffer[i]))             \
                return false;                                               \
                                                                            \
            i = (i + 1) % _deque_->capacity;                                \
                                                                            \
            if (j + 1 < _deque_->count)                                     \
                fprintf(fptr, "%s", separator);                             \
        }                                                                   \
                                                                            \
        fprintf(fptr, "%s", end);                                           \
                                                                            \
        return true;                                                        \
    }

#endif /* CMC_EXT_CMC_DEQUE_H */
