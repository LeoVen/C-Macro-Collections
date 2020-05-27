/**
 * hashmap.h
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
#define CMC_EXT_CMC_HASHMAP_PARTS INIT, STR

/* Used by STR */
static const char *cmc_cmc_string_fmt_hashmap = "struct %s<%s, %s> "
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
 * INIT
 *
 * The part 'INIT' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers).
 */
#define CMC_EXT_CMC_HASHMAP_INIT(BODY)    \
    CMC_EXT_CMC_HASHMAP_INIT_HEADER(BODY) \
    CMC_EXT_CMC_HASHMAP_INIT_SOURCE(BODY)

#define CMC_EXT_CMC_HASHMAP_INIT_HEADER(BODY)                                  \
    CMC_EXT_CMC_HASHMAP_INIT_HEADER_(CMC_PARAM_PFX(BODY),                      \
                                     CMC_PARAM_SNAME(BODY), CMC_PARAM_K(BODY), \
                                     CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMAP_INIT_SOURCE(BODY)                                  \
    CMC_EXT_CMC_HASHMAP_INIT_SOURCE_(CMC_PARAM_PFX(BODY),                      \
                                     CMC_PARAM_SNAME(BODY), CMC_PARAM_K(BODY), \
                                     CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMAP_INIT_HEADER_(PFX, SNAME, K, V)                 \
                                                                           \
    struct SNAME CMC_(PFX, _init)(size_t capacity, double load,            \
                                  struct CMC_DEF_FKEY(SNAME) * f_key,      \
                                  struct CMC_DEF_FVAL(SNAME) * f_val);     \
    struct SNAME CMC_(PFX, _init_custom)(                                  \
        size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,  \
        struct CMC_DEF_FVAL(SNAME) * f_val, struct cmc_alloc_node * alloc, \
        struct cmc_callbacks * callbacks);

#define CMC_EXT_CMC_HASHMAP_INIT_SOURCE_(PFX, SNAME, K, V)                     \
                                                                               \
    struct SNAME CMC_(PFX, _init)(size_t capacity, double load,                \
                                  struct CMC_DEF_FKEY(SNAME) * f_key,          \
                                  struct CMC_DEF_FVAL(SNAME) * f_val)          \
    {                                                                          \
        return CMC_(PFX, _init_custom)(capacity, load, f_key, f_val, NULL,     \
                                       NULL);                                  \
    }                                                                          \
                                                                               \
    struct SNAME CMC_(PFX, _init_custom)(                                      \
        size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,      \
        struct CMC_DEF_FVAL(SNAME) * f_val, struct cmc_alloc_node * alloc,     \
        struct cmc_callbacks * callbacks)                                      \
    {                                                                          \
        struct SNAME _map_ = { 0 };                                            \
                                                                               \
        if (capacity == 0 || load <= 0 || load >= 1)                           \
            return _map_;                                                      \
                                                                               \
        /* Prevent integer overflow */                                         \
        if (capacity >= UINTMAX_MAX * load)                                    \
            return _map_;                                                      \
                                                                               \
        if (!f_key || !f_val)                                                  \
            return _map_;                                                      \
                                                                               \
        size_t real_capacity =                                                 \
            CMC_(PFX, _impl_calculate_size)(capacity / load);                  \
                                                                               \
        if (!alloc)                                                            \
            alloc = &cmc_alloc_node_default;                                   \
                                                                               \
        _map_.buffer =                                                         \
            alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME))); \
                                                                               \
        if (!_map_.buffer)                                                     \
            return _map_;                                                      \
                                                                               \
        _map_.count = 0;                                                       \
        _map_.capacity = real_capacity;                                        \
        _map_.load = load;                                                     \
        _map_.flag = CMC_FLAG_OK;                                              \
        _map_.f_key = f_key;                                                   \
        _map_.f_val = f_val;                                                   \
        _map_.alloc = alloc;                                                   \
        _map_.callbacks = callbacks;                                           \
                                                                               \
        return _map_;                                                          \
    }

/**
 * STR
 *
 * The part 'STR' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers).
 */
#define CMC_EXT_CMC_HASHMAP_STR(BODY)    \
    CMC_EXT_CMC_HASHMAP_STR_HEADER(BODY) \
    CMC_EXT_CMC_HASHMAP_STR_SOURCE(BODY)

#define CMC_EXT_CMC_HASHMAP_STR_HEADER(BODY)                                  \
    CMC_EXT_CMC_HASHMAP_STR_HEADER_(CMC_PARAM_PFX(BODY),                      \
                                    CMC_PARAM_SNAME(BODY), CMC_PARAM_K(BODY), \
                                    CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMAP_STR_SOURCE(BODY)                                  \
    CMC_EXT_CMC_HASHMAP_STR_SOURCE_(CMC_PARAM_PFX(BODY),                      \
                                    CMC_PARAM_SNAME(BODY), CMC_PARAM_K(BODY), \
                                    CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMAP_STR_HEADER_(PFX, SNAME, K, V)            \
                                                                     \
    bool CMC_(PFX, _to_string)(struct SNAME * _map_, FILE * fptr);   \
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr,        \
                           const char *start, const char *separator, \
                           const char *end, const char *key_val_sep);

#define CMC_EXT_CMC_HASHMAP_STR_SOURCE_(PFX, SNAME, K, V)                   \
                                                                            \
    bool CMC_(PFX, _to_string)(struct SNAME * _map_, FILE * fptr)           \
    {                                                                       \
        struct SNAME *m_ = _map_;                                           \
                                                                            \
        return 0 <= fprintf(fptr, cmc_cmc_string_fmt_hashmap,               \
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
            if ((_map_->buffer[i - 1]).state == CMC_ES_FILLED)              \
            {                                                               \
                last = i - 1;                                               \
                break;                                                      \
            }                                                               \
        }                                                                   \
                                                                            \
        for (size_t i = 0; i < _map_->capacity; i++)                        \
        {                                                                   \
            struct CMC_DEF_ENTRY(SNAME) *entry = &(_map_->buffer[i]);       \
                                                                            \
            if (entry->state == CMC_ES_FILLED)                              \
            {                                                               \
                if (!_map_->f_key->str(fptr, entry->key))                   \
                    return false;                                           \
                                                                            \
                fprintf(fptr, "%s", key_val_sep);                           \
                                                                            \
                if (!_map_->f_val->str(fptr, entry->value))                 \
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

#endif /* CMC_EXT_CMC_HASHMAP_H */
