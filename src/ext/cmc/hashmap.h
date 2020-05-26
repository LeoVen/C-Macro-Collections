/**
 * hashmap.h
 *
 * Creation Date: 25/05/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Extensions to CMC<HashMap>
 *
 * - INIT
 */

#ifndef CMC_EXT_CMC_HASHMAP_H
#define CMC_EXT_CMC_HASHMAP_H

#include "../../cor/params.h"

/**
 * INIT
 *
 * The part 'INIT' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers).
 */
#define CMC_EXT_CMC_HASHMAP_INIT(BODY)    \
    CMC_EXT_CMC_HASHMAP_INIT_HEADER(BODY) \
    CMC_EXT_CMC_HASHMAP_INIT_SOURCE(BODY)

#define CMC_EXT_CMC_HASHMAP_INIT_HEADER(BODY)             \
    CMC_EXT_CMC_HASHMAP_INIT_HEADER_(CMC_PARAM_PFX(BODY), \
                                     CMC_PARAM_SNAME(BODY), CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMAP_INIT_SOURCE(BODY)             \
    CMC_EXT_CMC_HASHMAP_INIT_SOURCE_(CMC_PARAM_PFX(BODY), \
                                     CMC_PARAM_SNAME(BODY), CMC_PARAM_V(BODY))

#define CMC_EXT_CMC_HASHMAP_INIT_HEADER_(PFX, SNAME, V)           \
                                                                  \
    struct SNAME PFX##_init(size_t capacity, double load,         \
                            struct SNAME##_fkey *f_key,           \
                            struct SNAME##_fval *f_val);          \
    struct SNAME PFX##_init_custom(                               \
        size_t capacity, double load, struct SNAME##_fkey *f_key, \
        struct SNAME##_fval *f_val, struct cmc_alloc_node *alloc, \
        struct cmc_callbacks *callbacks);

#define CMC_EXT_CMC_HASHMAP_INIT_SOURCE_(PFX, SNAME, V)                     \
                                                                            \
    struct SNAME PFX##_init(size_t capacity, double load,                   \
                            struct SNAME##_fkey *f_key,                     \
                            struct SNAME##_fval *f_val)                     \
    {                                                                       \
        return PFX##_init_custom(capacity, load, f_key, f_val, NULL, NULL); \
    }                                                                       \
                                                                            \
    struct SNAME PFX##_init_custom(                                         \
        size_t capacity, double load, struct SNAME##_fkey *f_key,           \
        struct SNAME##_fval *f_val, struct cmc_alloc_node *alloc,           \
        struct cmc_callbacks *callbacks)                                    \
    {                                                                       \
        struct SNAME _map_ = { 0 };                                         \
                                                                            \
        if (capacity == 0 || load <= 0 || load >= 1)                        \
            return _map_;                                                   \
                                                                            \
        /* Prevent integer overflow */                                      \
        if (capacity >= UINTMAX_MAX * load)                                 \
            return _map_;                                                   \
                                                                            \
        if (!f_key || !f_val)                                               \
            return _map_;                                                   \
                                                                            \
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);  \
                                                                            \
        if (!alloc)                                                         \
            alloc = &cmc_alloc_node_default;                                \
                                                                            \
        _map_.buffer =                                                      \
            alloc->calloc(real_capacity, sizeof(struct SNAME##_entry));     \
                                                                            \
        if (!_map_.buffer)                                                  \
            return _map_;                                                   \
                                                                            \
        _map_.count = 0;                                                    \
        _map_.capacity = real_capacity;                                     \
        _map_.load = load;                                                  \
        _map_.flag = cmc_flags.OK;                                          \
        _map_.f_key = f_key;                                                \
        _map_.f_val = f_val;                                                \
        _map_.alloc = alloc;                                                \
        _map_.callbacks = callbacks;                                        \
                                                                            \
        return _map_;                                                       \
    }

#endif /* CMC_EXT_CMC_HASHMAP_H */
