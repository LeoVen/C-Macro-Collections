/**
 * hashmultiset.h
 *
 * Creation Date: 30/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_HASHMULTISET_H
#define CMC_EXT_CMC_HASHMULTISET_H

#include "../../cor/core.h"

/**
 * All the EXT parts of CMC HashMultiSet.
 */
#define CMC_EXT_CMC_HASHMULTISET_PARTS ITER, STR

/* Used by STR */
static const char *cmc_cmc_string_fmt_hashmultiset =
    "struct %s<%s> "
    "at %p { "
    "buffer:%p, "
    "capacity:%" PRIuMAX ", "
    "count:%" PRIuMAX ", "
    "cardinality:%" PRIuMAX ", "
    "load:%lf, "
    "flag:%d, "
    "f_val:%p, "
    "alloc:%p, "
    "callbacks:%p }";

/**
 * ITER
 */
#define CMC_EXT_CMC_HASHMULTISET_ITER(BODY)    \
    CMC_EXT_CMC_HASHMULTISET_ITER_HEADER(BODY) \
    CMC_EXT_CMC_HASHMULTISET_ITER_SOURCE(BODY)

#define CMC_EXT_CMC_HASHMULTISET_ITER_HEADER(BODY) \
    CMC_EXT_CMC_HASHMULTISET_ITER_HEADER_(         \
        CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMULTISET_ITER_SOURCE(BODY) \
    CMC_EXT_CMC_HASHMULTISET_ITER_SOURCE_(         \
        CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMULTISET_ITER_HEADER_(PFX, SNAME, V)                  \
                                                                              \
    /* HashMultiSet Iterator */                                               \
    struct CMC_DEF_ITER(SNAME)                                                \
    {                                                                         \
        /* Target HashMultiset */                                             \
        struct SNAME *target;                                                 \
                                                                              \
        /* Cursor's position (index) */                                       \
        size_t cursor;                                                        \
                                                                              \
        /* Keeps track of relative index to the iteration of elements */      \
        size_t index;                                                         \
                                                                              \
        /* The index of the first element */                                  \
        size_t first;                                                         \
                                                                              \
        /* The index of the last element */                                   \
        size_t last;                                                          \
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
    size_t CMC_(PFX, _iter_multiplicity)(struct CMC_DEF_ITER(SNAME) * iter);  \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#define CMC_EXT_CMC_HASHMULTISET_ITER_SOURCE_(PFX, SNAME, V)                 \
                                                                             \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target) \
    {                                                                        \
        struct CMC_DEF_ITER(SNAME) iter;                                     \
                                                                             \
        iter.target = target;                                                \
        iter.cursor = 0;                                                     \
        iter.index = 0;                                                      \
        iter.first = 0;                                                      \
        iter.last = 0;                                                       \
        iter.start = true;                                                   \
        iter.end = CMC_(PFX, _empty)(target);                                \
                                                                             \
        if (!CMC_(PFX, _empty)(target))                                      \
        {                                                                    \
            for (size_t i = 0; i < target->capacity; i++)                    \
            {                                                                \
                if (target->buffer[i].state == CMC_ES_FILLED)                \
                {                                                            \
                    iter.first = i;                                          \
                    break;                                                   \
                }                                                            \
            }                                                                \
                                                                             \
            iter.cursor = iter.first;                                        \
                                                                             \
            for (size_t i = target->capacity; i > 0; i--)                    \
            {                                                                \
                if (target->buffer[i - 1].state == CMC_ES_FILLED)            \
                {                                                            \
                    iter.last = i - 1;                                       \
                    break;                                                   \
                }                                                            \
            }                                                                \
        }                                                                    \
                                                                             \
        return iter;                                                         \
    }                                                                        \
                                                                             \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target)   \
    {                                                                        \
        struct CMC_DEF_ITER(SNAME) iter;                                     \
                                                                             \
        iter.target = target;                                                \
        iter.cursor = 0;                                                     \
        iter.index = 0;                                                      \
        iter.first = 0;                                                      \
        iter.last = 0;                                                       \
        iter.start = CMC_(PFX, _empty)(target);                              \
        iter.end = true;                                                     \
                                                                             \
        if (!CMC_(PFX, _empty)(target))                                      \
        {                                                                    \
            for (size_t i = 0; i < target->capacity; i++)                    \
            {                                                                \
                if (target->buffer[i].state == CMC_ES_FILLED)                \
                {                                                            \
                    iter.first = i;                                          \
                    break;                                                   \
                }                                                            \
            }                                                                \
                                                                             \
            for (size_t i = target->capacity; i > 0; i--)                    \
            {                                                                \
                if (target->buffer[i - 1].state == CMC_ES_FILLED)            \
                {                                                            \
                    iter.last = i - 1;                                       \
                    break;                                                   \
                }                                                            \
            }                                                                \
                                                                             \
            iter.cursor = iter.last;                                         \
            iter.index = target->count - 1;                                  \
        }                                                                    \
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
            iter->cursor = iter->first;                                      \
            iter->index = 0;                                                 \
            iter->start = true;                                              \
            iter->end = CMC_(PFX, _empty)(iter->target);                     \
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
            iter->cursor = iter->last;                                       \
            iter->index = iter->target->count - 1;                           \
            iter->start = CMC_(PFX, _empty)(iter->target);                   \
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
        struct CMC_DEF_ENTRY(SNAME) *scan =                                  \
            &(iter->target->buffer[iter->cursor]);                           \
                                                                             \
        iter->index++;                                                       \
                                                                             \
        while (1)                                                            \
        {                                                                    \
            iter->cursor++;                                                  \
            scan = &(iter->target->buffer[iter->cursor]);                    \
                                                                             \
            if (scan->state == CMC_ES_FILLED)                                \
                break;                                                       \
        }                                                                    \
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
        struct CMC_DEF_ENTRY(SNAME) *scan =                                  \
            &(iter->target->buffer[iter->cursor]);                           \
                                                                             \
        iter->index--;                                                       \
                                                                             \
        while (1)                                                            \
        {                                                                    \
            iter->cursor--;                                                  \
            scan = &(iter->target->buffer[iter->cursor]);                    \
                                                                             \
            if (scan->state == CMC_ES_FILLED)                                \
                break;                                                       \
        }                                                                    \
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
        for (size_t i = 0; i < steps; i++)                                   \
            CMC_(PFX, _iter_next)(iter);                                     \
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
        for (size_t i = 0; i < steps; i++)                                   \
            CMC_(PFX, _iter_prev)(iter);                                     \
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
        return iter->target->buffer[iter->cursor].value;                     \
    }                                                                        \
                                                                             \
    size_t CMC_(PFX, _iter_multiplicity)(struct CMC_DEF_ITER(SNAME) * iter)  \
    {                                                                        \
        if (CMC_(PFX, _empty)(iter->target))                                 \
            return 0;                                                        \
                                                                             \
        return iter->target->buffer[iter->cursor].multiplicity;              \
    }                                                                        \
                                                                             \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)         \
    {                                                                        \
        return iter->index;                                                  \
    }

/**
 * STR
 *
 * The part 'STR' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers).
 */
#define CMC_EXT_CMC_HASHMULTISET_STR(BODY)    \
    CMC_EXT_CMC_HASHMULTISET_STR_HEADER(BODY) \
    CMC_EXT_CMC_HASHMULTISET_STR_SOURCE(BODY)

#define CMC_EXT_CMC_HASHMULTISET_STR_HEADER(BODY) \
    CMC_EXT_CMC_HASHMULTISET_STR_HEADER_(         \
        CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMULTISET_STR_SOURCE(BODY) \
    CMC_EXT_CMC_HASHMULTISET_STR_SOURCE_(         \
        CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMULTISET_STR_HEADER_(PFX, SNAME, V)          \
                                                                     \
    bool CMC_(PFX, _to_string)(struct SNAME * _set_, FILE * fptr);   \
    bool CMC_(PFX, _print)(struct SNAME * _set_, FILE * fptr,        \
                           const char *start, const char *separator, \
                           const char *end, const char *key_val_sep);

#define CMC_EXT_CMC_HASHMULTISET_STR_SOURCE_(PFX, SNAME, V)                 \
                                                                            \
    bool CMC_(PFX, _to_string)(struct SNAME * _set_, FILE * fptr)           \
    {                                                                       \
        struct SNAME *s_ = _set_;                                           \
                                                                            \
        return 0 <= fprintf(fptr, cmc_cmc_string_fmt_hashmultiset,          \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(V), s_,     \
                            s_->buffer, s_->capacity, s_->count,            \
                            s_->cardinality, s_->load, s_->flag, s_->f_val, \
                            s_->alloc, s_->callbacks);                      \
    }                                                                       \
                                                                            \
    bool CMC_(PFX, _print)(struct SNAME * _set_, FILE * fptr,               \
                           const char *start, const char *separator,        \
                           const char *end, const char *val_mul_sep)        \
    {                                                                       \
        fprintf(fptr, "%s", start);                                         \
                                                                            \
        size_t last = 0;                                                    \
        for (size_t i = _set_->capacity; i > 0; i--)                        \
        {                                                                   \
            if ((_set_->buffer[i - 1]).state == CMC_ES_FILLED)              \
            {                                                               \
                last = i - 1;                                               \
                break;                                                      \
            }                                                               \
        }                                                                   \
                                                                            \
        for (size_t i = 0; i < _set_->capacity; i++)                        \
        {                                                                   \
            struct CMC_DEF_ENTRY(SNAME) *entry = &(_set_->buffer[i]);       \
                                                                            \
            if (entry->state == CMC_ES_FILLED)                              \
            {                                                               \
                if (!_set_->f_val->str(fptr, entry->value))                 \
                    return false;                                           \
                                                                            \
                fprintf(fptr, "%s", val_mul_sep);                           \
                                                                            \
                if (fprintf(fptr, "%" PRIuMAX "", entry->multiplicity) < 0) \
                    return false;                                           \
                                                                            \
                if (i + 1 < last)                                           \
                    fprintf(fptr, "%s", separator);                         \
            }                                                               \
        }                                                                   \
                                                                            \
        fprintf(fptr, "%s", end);                                           \
                                                                            \
        return true;                                                        \
    }

#endif /* CMC_EXT_CMC_HASHMULTISET_H */
