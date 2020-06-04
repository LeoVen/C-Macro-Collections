/**
 * ext/cmc/hashmap.h
 *
 * Creation Date: 25/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_HASHMAP_H
#define CMC_EXT_CMC_HASHMAP_H

#include "../../cor/core.h"

/**
 * All the EXT parts of CMC HashMap.
 */
#define CMC_EXT_CMC_HASHMAP_PARTS INIT, ITER, STR

/**
 * INIT
 *
 * The part 'INIT' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers).
 */
#define CMC_EXT_CMC_HASHMAP_INIT(PARAMS) \
    CMC_EXT_CMC_HASHMAP_INIT_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHMAP_INIT_SOURCE(PARAMS)

#define CMC_EXT_CMC_HASHMAP_INIT_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHMAP_INIT_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                     CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHMAP_INIT_SOURCE(PARAMS) \
    CMC_EXT_CMC_HASHMAP_INIT_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                     CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHMAP_INIT_HEADER_(PFX, SNAME, K, V) \
\
    struct SNAME CMC_(PFX, _init)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key, \
                                  struct CMC_DEF_FVAL(SNAME) * f_val); \
    struct SNAME CMC_(PFX, _init_custom)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key, \
                                         struct CMC_DEF_FVAL(SNAME) * f_val, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks); \
    void CMC_(PFX, _release)(struct SNAME _map_);

#define CMC_EXT_CMC_HASHMAP_INIT_SOURCE_(PFX, SNAME, K, V) \
\
    struct SNAME CMC_(PFX, _init)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key, \
                                  struct CMC_DEF_FVAL(SNAME) * f_val) \
    { \
        return CMC_(PFX, _init_custom)(capacity, load, f_key, f_val, NULL, NULL); \
    } \
\
    struct SNAME CMC_(PFX, _init_custom)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key, \
                                         struct CMC_DEF_FVAL(SNAME) * f_val, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        struct SNAME _map_ = { 0 }; \
\
        if (capacity == 0 || load <= 0 || load >= 1) \
            return _map_; \
\
        /* Prevent integer overflow */ \
        if (capacity >= UINTMAX_MAX * load) \
            return _map_; \
\
        if (!f_key || !f_val) \
            return _map_; \
\
        size_t real_capacity = CMC_(PFX, _impl_calculate_size)(capacity / load); \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        _map_.buffer = alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME))); \
\
        if (!_map_.buffer) \
            return _map_; \
\
        _map_.count = 0; \
        _map_.capacity = real_capacity; \
        _map_.load = load; \
        _map_.flag = CMC_FLAG_OK; \
        _map_.f_key = f_key; \
        _map_.f_val = f_val; \
        _map_.alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(&_map_, callbacks); \
\
        return _map_; \
    } \
\
    void CMC_(PFX, _release)(struct SNAME _map_) \
    { \
        if (_map_.f_key->free || _map_.f_val->free) \
        { \
            for (size_t i = 0; i < _map_.capacity; i++) \
            { \
                struct CMC_DEF_ENTRY(SNAME) *entry = &(_map_.buffer[i]); \
\
                if (entry->state == CMC_ES_FILLED) \
                { \
                    if (_map_.f_key->free) \
                        _map_.f_key->free(entry->key); \
                    if (_map_.f_val->free) \
                        _map_.f_val->free(entry->value); \
                } \
            } \
        } \
\
        _map_.alloc->free(_map_.buffer); \
    }

/**
 * ITER
 */
#define CMC_EXT_CMC_HASHMAP_ITER(PARAMS) \
    CMC_EXT_CMC_HASHMAP_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHMAP_ITER_SOURCE(PARAMS)

#define CMC_EXT_CMC_HASHMAP_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHMAP_ITER_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                     CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHMAP_ITER_SOURCE(PARAMS) \
    CMC_EXT_CMC_HASHMAP_ITER_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                     CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHMAP_ITER_HEADER_(PFX, SNAME, K, V) \
\
    /* HashMap Iterator */ \
    struct CMC_DEF_ITER(SNAME) \
    { \
        /* Target hashmap */ \
        struct SNAME *target; \
\
        /* Cursor's position (index) */ \
        size_t cursor; \
\
        /* Keeps track of relative index to the iteration of elements */ \
        size_t index; \
\
        /* The index of the first element */ \
        size_t first; \
\
        /* The index of the last element */ \
        size_t last; \
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
    K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter); \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter); \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter); \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#define CMC_EXT_CMC_HASHMAP_ITER_SOURCE_(PFX, SNAME, K, V) \
\
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target) \
    { \
        struct CMC_DEF_ITER(SNAME) iter; \
\
        iter.target = target; \
        iter.cursor = 0; \
        iter.index = 0; \
        iter.first = 0; \
        iter.last = 0; \
        iter.start = true; \
        iter.end = CMC_(PFX, _empty)(target); \
\
        if (!CMC_(PFX, _empty)(target)) \
        { \
            for (size_t i = 0; i < target->capacity; i++) \
            { \
                if (target->buffer[i].state == CMC_ES_FILLED) \
                { \
                    iter.first = i; \
                    break; \
                } \
            } \
\
            iter.cursor = iter.first; \
\
            for (size_t i = target->capacity; i > 0; i--) \
            { \
                if (target->buffer[i - 1].state == CMC_ES_FILLED) \
                { \
                    iter.last = i - 1; \
                    break; \
                } \
            } \
        } \
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
        iter.index = 0; \
        iter.first = 0; \
        iter.last = 0; \
        iter.start = CMC_(PFX, _empty)(target); \
        iter.end = true; \
\
        if (!CMC_(PFX, _empty)(target)) \
        { \
            for (size_t i = 0; i < target->capacity; i++) \
            { \
                if (target->buffer[i].state == CMC_ES_FILLED) \
                { \
                    iter.first = i; \
                    break; \
                } \
            } \
\
            for (size_t i = target->capacity; i > 0; i--) \
            { \
                if (target->buffer[i - 1].state == CMC_ES_FILLED) \
                { \
                    iter.last = i - 1; \
                    break; \
                } \
            } \
\
            iter.cursor = iter.last; \
            iter.index = target->count - 1; \
        } \
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
            iter->cursor = iter->first; \
            iter->index = 0; \
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
            iter->cursor = iter->last; \
            iter->index = iter->target->count - 1; \
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
        if (iter->index + 1 == iter->target->count) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        iter->start = CMC_(PFX, _empty)(iter->target); \
\
        struct CMC_DEF_ENTRY(SNAME) *scan = &(iter->target->buffer[iter->cursor]); \
\
        iter->index++; \
\
        while (1) \
        { \
            iter->cursor++; \
            scan = &(iter->target->buffer[iter->cursor]); \
\
            if (scan->state == CMC_ES_FILLED) \
                break; \
        } \
\
        return true; \
    } \
\
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (iter->start) \
            return false; \
\
        if (iter->index == 0) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        iter->end = CMC_(PFX, _empty)(iter->target); \
\
        struct CMC_DEF_ENTRY(SNAME) *scan = &(iter->target->buffer[iter->cursor]); \
\
        iter->index--; \
\
        while (1) \
        { \
            iter->cursor--; \
            scan = &(iter->target->buffer[iter->cursor]); \
\
            if (scan->state == CMC_ES_FILLED) \
                break; \
        } \
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
        if (iter->index + 1 == iter->target->count) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        if (steps == 0 || iter->index + steps >= iter->target->count) \
            return false; \
\
        for (size_t i = 0; i < steps; i++) \
            CMC_(PFX, _iter_next)(iter); \
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
        if (iter->index == 0) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        if (steps == 0 || iter->index < steps) \
            return false; \
\
        for (size_t i = 0; i < steps; i++) \
            CMC_(PFX, _iter_prev)(iter); \
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
        if (iter->index > index) \
            return CMC_(PFX, _iter_rewind)(iter, iter->index - index); \
        else if (iter->index < index) \
            return CMC_(PFX, _iter_advance)(iter, index - iter->index); \
\
        return true; \
    } \
\
    K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return (K){ 0 }; \
\
        return iter->target->buffer[iter->cursor].key; \
    } \
\
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return (V){ 0 }; \
\
        return iter->target->buffer[iter->cursor].value; \
    } \
\
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return NULL; \
\
        return &(iter->target->buffer[iter->cursor].value); \
    } \
\
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        return iter->index; \
    }

/**
 * STR
 */
#define CMC_EXT_CMC_HASHMAP_STR(PARAMS) \
    CMC_EXT_CMC_HASHMAP_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHMAP_STR_SOURCE(PARAMS)

#define CMC_EXT_CMC_HASHMAP_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHMAP_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                    CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHMAP_STR_SOURCE(PARAMS) \
    CMC_EXT_CMC_HASHMAP_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                    CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHMAP_STR_HEADER_(PFX, SNAME, K, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _map_, FILE * fptr); \
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr, const char *start, const char *separator, \
                           const char *end, const char *key_val_sep);

#define CMC_EXT_CMC_HASHMAP_STR_SOURCE_(PFX, SNAME, K, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _map_, FILE * fptr) \
    { \
        struct SNAME *m_ = _map_; \
\
        return 0 <= fprintf(fptr, \
                            "struct %s<%s, %s> " \
                            "at %p { " \
                            "buffer:%p, " \
                            "capacity:%" PRIuMAX ", " \
                            "count:%" PRIuMAX ", " \
                            "load:%lf, " \
                            "flag:%d, " \
                            "f_key:%p, " \
                            "f_val:%p, " \
                            "alloc:%p, " \
                            "callbacks:%p }", \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(K), CMC_TO_STRING(V), m_, m_->buffer, m_->capacity, \
                            m_->count, m_->load, m_->flag, m_->f_key, m_->f_val, m_->alloc, CMC_CALLBACKS_GET(m_)); \
    } \
\
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr, const char *start, const char *separator, \
                           const char *end, const char *key_val_sep) \
    { \
        fprintf(fptr, "%s", start); \
\
        size_t last = 0; \
        for (size_t i = _map_->capacity; i > 0; i--) \
        { \
            if ((_map_->buffer[i - 1]).state == CMC_ES_FILLED) \
            { \
                last = i - 1; \
                break; \
            } \
        } \
\
        for (size_t i = 0; i < _map_->capacity; i++) \
        { \
            struct CMC_DEF_ENTRY(SNAME) *entry = &(_map_->buffer[i]); \
\
            if (entry->state == CMC_ES_FILLED) \
            { \
                if (!_map_->f_key->str(fptr, entry->key)) \
                    return false; \
\
                fprintf(fptr, "%s", key_val_sep); \
\
                if (!_map_->f_val->str(fptr, entry->value)) \
                    return false; \
\
                if (i + 1 < last) \
                    fprintf(fptr, "%s", separator); \
            } \
        } \
\
        fprintf(fptr, "%s", end); \
\
        return true; \
    }

#endif /* CMC_EXT_CMC_HASHMAP_H */
