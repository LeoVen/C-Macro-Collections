/**
 * hashmultimap.h
 *
 * Creation Date: 29/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_HASHMULTIMAP_H
#define CMC_EXT_CMC_HASHMULTIMAP_H

#include "../../cor/core.h"

/**
 * All the EXT parts of CMC HashMultiMap.
 */
#define CMC_EXT_CMC_HASHMULTIMAP_PARTS ITER, STR

/* Used by STR */
static const char *cmc_cmc_string_fmt_hashmultimap = "struct %s<%s, %s> "
                                                     "at %p { "
                                                     "buffer:%p, "
                                                     "capacity:%" PRIuMAX ", "
                                                     "count:%" PRIuMAX ", "
                                                     "load:%lf, "
                                                     "flag:%d, "
                                                     "f_key:%p, "
                                                     "f_val:%p, "
                                                     "alloc:%p, "
                                                     "callbacks:%p }";

/**
 * ITER
 */
#define CMC_EXT_CMC_HASHMULTIMAP_ITER(BODY)    \
    CMC_EXT_CMC_HASHMULTIMAP_ITER_HEADER(BODY) \
    CMC_EXT_CMC_HASHMULTIMAP_ITER_SOURCE(BODY)

#define CMC_EXT_CMC_HASHMULTIMAP_ITER_HEADER(BODY)                     \
    CMC_EXT_CMC_HASHMULTIMAP_ITER_HEADER_(                             \
        CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), CMC_PARAM_K(BODY), \
        CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMULTIMAP_ITER_SOURCE(BODY)                     \
    CMC_EXT_CMC_HASHMULTIMAP_ITER_SOURCE_(                             \
        CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), CMC_PARAM_K(BODY), \
        CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMULTIMAP_ITER_HEADER_(PFX, SNAME, K, V)               \
                                                                              \
    struct CMC_DEF_ITER(SNAME)                                                \
    {                                                                         \
        /* Target hashmultimap */                                             \
        struct SNAME *target;                                                 \
                                                                              \
        /* Current entry */                                                   \
        struct CMC_DEF_ENTRY(SNAME) * curr_entry;                             \
                                                                              \
        /* Cursor`s position (index) */                                       \
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
    K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter);                \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter);              \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter);            \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#define CMC_EXT_CMC_HASHMULTIMAP_ITER_SOURCE_(PFX, SNAME, K, V)                \
                                                                               \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target)   \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        iter.target = target;                                                  \
        iter.curr_entry = NULL;                                                \
        iter.cursor = 0;                                                       \
        iter.index = 0;                                                        \
        iter.first = 0;                                                        \
        iter.last = 0;                                                         \
        iter.start = true;                                                     \
        iter.end = CMC_(PFX, _empty)(target);                                  \
                                                                               \
        if (!CMC_(PFX, _empty)(target))                                        \
        {                                                                      \
            for (size_t i = 0; i < target->capacity; i++)                      \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i][0];     \
                                                                               \
                if (entry && entry != CMC_ENTRY_DELETED)                       \
                {                                                              \
                    iter.curr_entry = target->buffer[i][0];                    \
                    iter.first = i;                                            \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            iter.cursor = iter.first;                                          \
                                                                               \
            for (size_t i = target->capacity; i > 0; i--)                      \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i - 1][1]; \
                                                                               \
                if (entry && entry != CMC_ENTRY_DELETED)                       \
                {                                                              \
                    iter.last = i - 1;                                         \
                    break;                                                     \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target)     \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        iter.target = target;                                                  \
        iter.curr_entry = NULL;                                                \
        iter.cursor = 0;                                                       \
        iter.index = 0;                                                        \
        iter.first = 0;                                                        \
        iter.last = 0;                                                         \
        iter.start = CMC_(PFX, _empty)(target);                                \
        iter.end = true;                                                       \
                                                                               \
        if (!CMC_(PFX, _empty)(target))                                        \
        {                                                                      \
            for (size_t i = 0; i < target->capacity; i++)                      \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i][0];     \
                                                                               \
                if (entry && entry != CMC_ENTRY_DELETED)                       \
                {                                                              \
                    iter.first = i;                                            \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            for (size_t i = target->capacity; i > 0; i--)                      \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i - 1][1]; \
                                                                               \
                if (entry && entry != CMC_ENTRY_DELETED)                       \
                {                                                              \
                    iter.curr_entry = target->buffer[i - 1][1];                \
                    iter.last = i - 1;                                         \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            iter.cursor = iter.last;                                           \
            iter.index = target->count - 1;                                    \
        }                                                                      \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                          \
        return CMC_(PFX, _empty)(iter->target) || iter->start;                 \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                          \
        return CMC_(PFX, _empty)(iter->target) || iter->end;                   \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                          \
        if (!CMC_(PFX, _empty)(iter->target))                                  \
        {                                                                      \
            iter->cursor = iter->first;                                        \
            iter->index = 0;                                                   \
            iter->start = true;                                                \
            iter->end = CMC_(PFX, _empty)(iter->target);                       \
            iter->curr_entry = iter->target->buffer[iter->first][0];           \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                          \
        if (!CMC_(PFX, _empty)(iter->target))                                  \
        {                                                                      \
            iter->cursor = iter->last;                                         \
            iter->index = iter->target->count - 1;                             \
            iter->start = CMC_(PFX, _empty)(iter->target);                     \
            iter->end = true;                                                  \
            iter->curr_entry = iter->target->buffer[iter->last][1];            \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                          \
        if (iter->end)                                                         \
            return false;                                                      \
                                                                               \
        if (iter->curr_entry->next != NULL)                                    \
        {                                                                      \
            iter->curr_entry = iter->curr_entry->next;                         \
            iter->index++;                                                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            if (iter->cursor == iter->last)                                    \
            {                                                                  \
                iter->end = true;                                              \
                return false;                                                  \
            }                                                                  \
                                                                               \
            iter->cursor++;                                                    \
                                                                               \
            while (iter->target->buffer[iter->cursor][0] == NULL)              \
                iter->cursor++;                                                \
                                                                               \
            iter->curr_entry = iter->target->buffer[iter->cursor][0];          \
                                                                               \
            iter->index++;                                                     \
        }                                                                      \
                                                                               \
        iter->start = CMC_(PFX, _empty)(iter->target);                         \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                          \
        if (iter->start)                                                       \
            return false;                                                      \
                                                                               \
        if (iter->curr_entry->prev != NULL)                                    \
        {                                                                      \
            iter->curr_entry = iter->curr_entry->prev;                         \
            iter->index--;                                                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            if (iter->cursor == iter->first)                                   \
            {                                                                  \
                iter->start = true;                                            \
                return false;                                                  \
            }                                                                  \
                                                                               \
            iter->cursor--;                                                    \
                                                                               \
            while (iter->target->buffer[iter->cursor][1] == NULL)              \
                iter->cursor--;                                                \
                                                                               \
            iter->curr_entry = iter->target->buffer[iter->cursor][1];          \
                                                                               \
            iter->index--;                                                     \
        }                                                                      \
                                                                               \
        iter->end = CMC_(PFX, _empty)(iter->target);                           \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                  size_t steps)                                \
    {                                                                          \
        if (iter->end)                                                         \
            return false;                                                      \
                                                                               \
        if (iter->index + 1 == iter->target->count)                            \
        {                                                                      \
            iter->end = true;                                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (steps == 0 || iter->index + steps >= iter->target->count)          \
            return false;                                                      \
                                                                               \
        for (size_t i = 0; i < steps; i++)                                     \
            CMC_(PFX, _iter_next)(iter);                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter,            \
                                 size_t steps)                                 \
    {                                                                          \
        if (iter->start)                                                       \
            return false;                                                      \
                                                                               \
        if (iter->index == 0)                                                  \
        {                                                                      \
            iter->start = true;                                                \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (steps == 0 || iter->index < steps)                                 \
            return false;                                                      \
                                                                               \
        for (size_t i = 0; i < steps; i++)                                     \
            CMC_(PFX, _iter_prev)(iter);                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator was able to be positioned at the */   \
    /* given index */                                                          \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter,             \
                                size_t index)                                  \
    {                                                                          \
        if (index >= iter->target->count)                                      \
            return false;                                                      \
                                                                               \
        if (iter->index > index)                                               \
            return CMC_(PFX, _iter_rewind)(iter, iter->index - index);         \
        else if (iter->index < index)                                          \
            return CMC_(PFX, _iter_advance)(iter, index - iter->index);        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter)                  \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
            return (K){ 0 };                                                   \
                                                                               \
        return iter->curr_entry->key;                                          \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)                \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
            return (V){ 0 };                                                   \
                                                                               \
        return iter->curr_entry->value;                                        \
    }                                                                          \
                                                                               \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
            return NULL;                                                       \
                                                                               \
        return &(iter->curr_entry->value);                                     \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)           \
    {                                                                          \
        return iter->index;                                                    \
    }

/**
 * STR
 */
#define CMC_EXT_CMC_HASHMULTIMAP_STR(BODY)    \
    CMC_EXT_CMC_HASHMULTIMAP_STR_HEADER(BODY) \
    CMC_EXT_CMC_HASHMULTIMAP_STR_SOURCE(BODY)

#define CMC_EXT_CMC_HASHMULTIMAP_STR_HEADER(BODY)               \
    CMC_EXT_CMC_HASHMULTIMAP_STR_HEADER_(CMC_PARAM_PFX(BODY),   \
                                         CMC_PARAM_SNAME(BODY), \
                                         CMC_PARAM_K(BODY), CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMULTIMAP_STR_SOURCE(BODY)               \
    CMC_EXT_CMC_HASHMULTIMAP_STR_SOURCE_(CMC_PARAM_PFX(BODY),   \
                                         CMC_PARAM_SNAME(BODY), \
                                         CMC_PARAM_K(BODY), CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMULTIMAP_STR_HEADER_(PFX, SNAME, K, V)       \
                                                                     \
    bool CMC_(PFX, _to_string)(struct SNAME * _map_, FILE * fptr);   \
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr,        \
                           const char *start, const char *separator, \
                           const char *end, const char *key_val_sep);

#define CMC_EXT_CMC_HASHMULTIMAP_STR_SOURCE_(PFX, SNAME, K, V)              \
                                                                            \
    bool CMC_(PFX, _to_string)(struct SNAME * _map_, FILE * fptr)           \
    {                                                                       \
        struct SNAME *m_ = _map_;                                           \
                                                                            \
        return 0 <= fprintf(fptr, cmc_cmc_string_fmt_hashmultimap,          \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(K),         \
                            CMC_TO_STRING(V), m_, m_->buffer, m_->capacity, \
                            m_->count, m_->load, m_->flag, m_->f_key,       \
                            m_->f_val, m_->alloc, m_->callbacks);           \
    }                                                                       \
                                                                            \
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr,               \
                           const char *start, const char *separator,        \
                           const char *end, const char *key_val_sep)        \
    {                                                                       \
        fprintf(fptr, "%s", start);                                         \
                                                                            \
        size_t last = 0;                                                    \
        for (size_t i = _map_->capacity; i > 0; i--)                        \
        {                                                                   \
            struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[i - 1][1];   \
                                                                            \
            if (entry && entry != CMC_ENTRY_DELETED)                        \
            {                                                               \
                last = i - 1;                                               \
                break;                                                      \
            }                                                               \
        }                                                                   \
                                                                            \
        for (size_t i = 0; i < _map_->capacity; i++)                        \
        {                                                                   \
            struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0];        \
                                                                            \
            while (scan != NULL)                                            \
            {                                                               \
                if (!_map_->f_key->str(fptr, scan->key))                    \
                    return false;                                           \
                                                                            \
                fprintf(fptr, "%s", key_val_sep);                           \
                                                                            \
                if (!_map_->f_val->str(fptr, scan->value))                  \
                    return false;                                           \
                                                                            \
                scan = scan->next;                                          \
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

#endif /* CMC_EXT_CMC_HASHMULTIMAP_H */
