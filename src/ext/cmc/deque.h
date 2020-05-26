/**
 * deque.h
 *
 * Creation Date: 25/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Extensions to CMC<Deque>
 *
 * - INIT
 */

#ifndef CMC_EXT_CMC_DEQUE_H
#define CMC_EXT_CMC_DEQUE_H

#include "../../cor/core.h"

#define CMC_EXT_CMC_DEQUE_PARTS INIT,

/**
 * INIT
 *
 * The part 'INIT' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers)
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
        _deque_.flag = cmc_flags.OK;                                     \
        _deque_.f_val = f_val;                                           \
        _deque_.alloc = alloc;                                           \
        _deque_.callbacks = callbacks;                                   \
                                                                         \
        return _deque_;                                                  \
    }

#endif /* CMC_EXT_CMC_DEQUE_H */
