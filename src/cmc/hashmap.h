/**
 * cmc/hashmap.h
 *
 * Creation Date: 03/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashMap
 *
 * A HashMap is an implementation of a Map with unique keys, where every key is
 * mapped to a value. The keys are not sorted. It is implemented as a flat
 * hashtable with linear probing and robin hood hashing.
 */

#ifndef CMC_CMC_HASHMAP_H
#define CMC_CMC_HASHMAP_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * Hashtable Implementation
 * ------------------------------------------------------------------------- */
#include "../cor/hashtable.h"

/**
 * Core HashMap implementation
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_CMC_HASHMAP_CORE(ACCESS, FILE, PARAMS) CMC_(CMC_(CMC_CMC_HASHMAP_CORE_, ACCESS), CMC_(_, FILE))(PARAMS)

/* PRIVATE or PUBLIC solver */
#define CMC_CMC_HASHMAP_CORE_PUBLIC_HEADER(PARAMS) \
    CMC_CMC_HASHMAP_CORE_STRUCT(PARAMS) \
    CMC_CMC_HASHMAP_CORE_HEADER(PARAMS)

#define CMC_CMC_HASHMAP_CORE_PUBLIC_SOURCE(PARAMS) CMC_CMC_HASHMAP_CORE_SOURCE(PARAMS)

#define CMC_CMC_HASHMAP_CORE_PRIVATE_HEADER(PARAMS) \
    struct CMC_PARAM_SNAME(PARAMS); \
    struct CMC_DEF_ENTRY(CMC_PARAM_SNAME(PARAMS)); \
    CMC_CMC_HASHMAP_CORE_HEADER(PARAMS)

#define CMC_CMC_HASHMAP_CORE_PRIVATE_SOURCE(PARAMS) \
    CMC_CMC_HASHMAP_CORE_STRUCT(PARAMS) \
    CMC_CMC_HASHMAP_CORE_SOURCE(PARAMS)

/* Lowest level API */
#define CMC_CMC_HASHMAP_CORE_STRUCT(PARAMS) \
    CMC_CMC_HASHMAP_CORE_STRUCT_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                 CMC_PARAM_V(PARAMS))

#define CMC_CMC_HASHMAP_CORE_HEADER(PARAMS) \
    CMC_CMC_HASHMAP_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                 CMC_PARAM_V(PARAMS))

#define CMC_CMC_HASHMAP_CORE_SOURCE(PARAMS) \
    CMC_CMC_HASHMAP_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                 CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Struct
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHMAP_CORE_STRUCT_(PFX, SNAME, K, V) \
\
    /* HashMap Structure */ \
    struct SNAME \
    { \
        /* Array of Entries */ \
        struct CMC_DEF_ENTRY(SNAME) * buffer; \
\
        /* Current array capacity */ \
        size_t capacity; \
\
        /* Current amount of keys */ \
        size_t count; \
\
        /* Load factor in range (0.0, 1.0) */ \
        double load; \
\
        /* Flags indicating errors or success */ \
        int flag; \
\
        /* Key function table */ \
        struct CMC_DEF_FKEY(SNAME) * f_key; \
\
        /* Value function table */ \
        struct CMC_DEF_FVAL(SNAME) * f_val; \
\
        /* Custom allocation functions */ \
        struct CMC_ALLOC_NODE_NAME *alloc; \
\
        /* Custom callback functions */ \
        CMC_CALLBACKS_DECL; \
    }; \
\
    /* HashMap Entry */ \
    struct CMC_DEF_ENTRY(SNAME) \
    { \
        /* Entry Key */ \
        K key; \
\
        /* Entry Value */ \
        V value; \
\
        /* The distance of this node to its original position, used by */ \
        /* robin-hood hashing */ \
        size_t dist; \
\
        /* The sate of this node (DELETED, EMPTY, FILLED) */ \
        enum cmc_entry_state state; \
    };

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHMAP_CORE_HEADER_(PFX, SNAME, K, V) \
\
    /* Key struct function table */ \
    struct CMC_DEF_FKEY(SNAME) \
    { \
        /* Comparator function */ \
        CMC_DEF_FTAB_CMP(K); \
\
        /* Copy function */ \
        CMC_DEF_FTAB_CPY(K); \
\
        /* To string function */ \
        CMC_DEF_FTAB_STR(K); \
\
        /* Free from memory function */ \
        CMC_DEF_FTAB_FREE(K); \
\
        /* Hash function */ \
        CMC_DEF_FTAB_HASH(K); \
\
        /* Priority function */ \
        CMC_DEF_FTAB_PRI(K); \
    }; \
\
    /* Value struct function table */ \
    struct CMC_DEF_FVAL(SNAME) \
    { \
        /* Comparator function */ \
        CMC_DEF_FTAB_CMP(V); \
\
        /* Copy function */ \
        CMC_DEF_FTAB_CPY(V); \
\
        /* To string function */ \
        CMC_DEF_FTAB_STR(V); \
\
        /* Free from memory function */ \
        CMC_DEF_FTAB_FREE(V); \
\
        /* Hash function */ \
        CMC_DEF_FTAB_HASH(V); \
\
        /* Priority function */ \
        CMC_DEF_FTAB_PRI(V); \
    }; \
\
    /* Collection Functions */ \
    /* Collection Allocation and Deallocation */ \
    struct SNAME *CMC_(PFX, _new)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key, \
                                  struct CMC_DEF_FVAL(SNAME) * f_val); \
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key, \
                                         struct CMC_DEF_FVAL(SNAME) * f_val, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks); \
    void CMC_(PFX, _clear)(struct SNAME * _map_); \
    void CMC_(PFX, _free)(struct SNAME * _map_); \
    /* Customization of Allocation and Callbacks */ \
    void CMC_(PFX, _customize)(struct SNAME * _map_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks); \
    /* Collection Input and Output */ \
    bool CMC_(PFX, _insert)(struct SNAME * _map_, K key, V value); \
    bool CMC_(PFX, _update)(struct SNAME * _map_, K key, V new_value, V * old_value); \
    bool CMC_(PFX, _remove)(struct SNAME * _map_, K key, V * out_value); \
    /* Element Access */ \
    bool CMC_(PFX, _max)(struct SNAME * _map_, K * key, V * value); \
    bool CMC_(PFX, _min)(struct SNAME * _map_, K * key, V * value); \
    V CMC_(PFX, _get)(struct SNAME * _map_, K key); \
    V *CMC_(PFX, _get_ref)(struct SNAME * _map_, K key); \
    /* Collection State */ \
    bool CMC_(PFX, _contains)(struct SNAME * _map_, K key); \
    bool CMC_(PFX, _empty)(struct SNAME * _map_); \
    bool CMC_(PFX, _full)(struct SNAME * _map_); \
    size_t CMC_(PFX, _count)(struct SNAME * _map_); \
    size_t CMC_(PFX, _capacity)(struct SNAME * _map_); \
    double CMC_(PFX, _load)(struct SNAME * _map_); \
    int CMC_(PFX, _flag)(struct SNAME * _map_); \
    /* Collection Utility */ \
    bool CMC_(PFX, _resize)(struct SNAME * _map_, size_t capacity); \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _map_); \
    bool CMC_(PFX, _equals)(struct SNAME * _map1_, struct SNAME * _map2_);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHMAP_CORE_SOURCE_(PFX, SNAME, K, V) \
\
    /* Implementation Detail Functions */ \
    static struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_get_entry)(struct SNAME * _map_, K key); \
    static size_t CMC_(PFX, _impl_calculate_size)(size_t required); \
\
    struct SNAME *CMC_(PFX, _new)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key, \
                                  struct CMC_DEF_FVAL(SNAME) * f_val) \
    { \
        return CMC_(PFX, _new_custom)(capacity, load, f_key, f_val, NULL, NULL); \
    } \
\
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key, \
                                         struct CMC_DEF_FVAL(SNAME) * f_val, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (capacity == 0 || load <= 0 || load >= 1) \
            return NULL; \
\
        /* Prevent integer overflow */ \
        if (capacity >= UINTMAX_MAX * load) \
            return NULL; \
\
        if (!f_key || !f_val) \
            return NULL; \
\
        size_t real_capacity = CMC_(PFX, _impl_calculate_size)(capacity / load); \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_map_) \
            return NULL; \
\
        _map_->buffer = alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME))); \
\
        if (!_map_->buffer) \
        { \
            alloc->free(_map_); \
            return NULL; \
        } \
\
        _map_->count = 0; \
        _map_->capacity = real_capacity; \
        _map_->load = load; \
        _map_->flag = CMC_FLAG_OK; \
        _map_->f_key = f_key; \
        _map_->f_val = f_val; \
        _map_->alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(_map_, callbacks); \
\
        return _map_; \
    } \
\
    void CMC_(PFX, _clear)(struct SNAME * _map_) \
    { \
        if (_map_->f_key->free || _map_->f_val->free) \
        { \
            for (size_t i = 0; i < _map_->capacity; i++) \
            { \
                struct CMC_DEF_ENTRY(SNAME) *entry = &(_map_->buffer[i]); \
\
                if (entry->state == CMC_ES_FILLED) \
                { \
                    if (_map_->f_key->free) \
                        _map_->f_key->free(entry->key); \
                    if (_map_->f_val->free) \
                        _map_->f_val->free(entry->value); \
                } \
            } \
        } \
\
        memset(_map_->buffer, 0, sizeof(struct CMC_DEF_ENTRY(SNAME)) * _map_->capacity); \
\
        _map_->count = 0; \
        _map_->flag = CMC_FLAG_OK; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _map_) \
    { \
        if (_map_->f_key->free || _map_->f_val->free) \
        { \
            for (size_t i = 0; i < _map_->capacity; i++) \
            { \
                struct CMC_DEF_ENTRY(SNAME) *entry = &(_map_->buffer[i]); \
\
                if (entry->state == CMC_ES_FILLED) \
                { \
                    if (_map_->f_key->free) \
                        _map_->f_key->free(entry->key); \
                    if (_map_->f_val->free) \
                        _map_->f_val->free(entry->value); \
                } \
            } \
        } \
\
        _map_->alloc->free(_map_->buffer); \
        _map_->alloc->free(_map_); \
    } \
\
    void CMC_(PFX, _customize)(struct SNAME * _map_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!alloc) \
            _map_->alloc = &cmc_alloc_node_default; \
        else \
            _map_->alloc = alloc; \
\
        CMC_CALLBACKS_ASSIGN(_map_, callbacks); \
\
        _map_->flag = CMC_FLAG_OK; \
    } \
\
    bool CMC_(PFX, _insert)(struct SNAME * _map_, K key, V value) \
    { \
        if (CMC_(PFX, _full)(_map_)) \
        { \
            if (!CMC_(PFX, _resize)(_map_, _map_->capacity + 1)) \
                return false; \
        } \
\
        if (CMC_(PFX, _impl_get_entry)(_map_, key) != NULL) \
        { \
            _map_->flag = CMC_FLAG_DUPLICATE; \
            return false; \
        } \
\
        size_t hash = _map_->f_key->hash(key); \
        size_t original_pos = hash % _map_->capacity; \
        size_t pos = original_pos; \
\
        struct CMC_DEF_ENTRY(SNAME) *target = &(_map_->buffer[pos]); \
\
        if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED) \
        { \
            target->key = key; \
            target->value = value; \
            target->dist = 0; \
            target->state = CMC_ES_FILLED; \
        } \
        else \
        { \
            while (true) \
            { \
                pos++; \
                target = &(_map_->buffer[pos % _map_->capacity]); \
\
                if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED) \
                { \
                    target->key = key; \
                    target->value = value; \
                    target->dist = pos - original_pos; \
                    target->state = CMC_ES_FILLED; \
\
                    break; \
                } \
                else if (target->dist < pos - original_pos) \
                { \
                    K tmp_k = target->key; \
                    V tmp_v = target->value; \
                    size_t tmp_dist = target->dist; \
\
                    target->key = key; \
                    target->value = value; \
                    target->dist = pos - original_pos; \
\
                    key = tmp_k; \
                    value = tmp_v; \
                    original_pos = pos - tmp_dist; \
                } \
            } \
        } \
\
        _map_->count++; \
        _map_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_map_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _update)(struct SNAME * _map_, K key, V new_value, V * old_value) \
    { \
        if (CMC_(PFX, _empty)(_map_)) \
        { \
            _map_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_get_entry)(_map_, key); \
\
        if (!entry) \
        { \
            _map_->flag = CMC_FLAG_NOT_FOUND; \
            return false; \
        } \
\
        if (old_value) \
            *old_value = entry->value; \
\
        entry->value = new_value; \
\
        _map_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_map_, update); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _remove)(struct SNAME * _map_, K key, V * out_value) \
    { \
        if (CMC_(PFX, _empty)(_map_)) \
        { \
            _map_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        struct CMC_DEF_ENTRY(SNAME) *result = CMC_(PFX, _impl_get_entry)(_map_, key); \
\
        if (result == NULL) \
        { \
            _map_->flag = CMC_FLAG_NOT_FOUND; \
            return false; \
        } \
\
        if (out_value) \
            *out_value = result->value; \
\
        result->key = (K){ 0 }; \
        result->value = (V){ 0 }; \
        result->dist = 0; \
        result->state = CMC_ES_DELETED; \
\
        _map_->count--; \
        _map_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_map_, delete); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _max)(struct SNAME * _map_, K * key, V * value) \
    { \
        if (CMC_(PFX, _empty)(_map_)) \
        { \
            _map_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        bool first = true; \
        K max_key = (K){ 0 }; \
        V max_val = (V){ 0 }; \
\
        for (size_t i = 0; i < _map_->capacity; i++) \
        { \
            if (_map_->buffer[i].state == CMC_ES_FILLED) \
            { \
                if (first) \
                { \
                    max_key = _map_->buffer[i].key; \
                    max_val = _map_->buffer[i].value; \
                    first = false; \
                } \
                else if (_map_->f_key->cmp(_map_->buffer[i].key, max_key) > 0) \
                { \
                    max_key = _map_->buffer[i].key; \
                    max_val = _map_->buffer[i].value; \
                } \
            } \
        } \
\
        if (key) \
            *key = max_key; \
        if (value) \
            *value = max_val; \
\
        CMC_CALLBACKS_CALL(_map_, read); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _min)(struct SNAME * _map_, K * key, V * value) \
    { \
        if (CMC_(PFX, _empty)(_map_)) \
        { \
            _map_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        bool first = true; \
        K min_key = (K){ 0 }; \
        V min_val = (V){ 0 }; \
\
        for (size_t i = 0; i < _map_->capacity; i++) \
        { \
            if (_map_->buffer[i].state == CMC_ES_FILLED) \
            { \
                if (first) \
                { \
                    min_key = _map_->buffer[i].key; \
                    min_val = _map_->buffer[i].value; \
                    first = false; \
                } \
                else if (_map_->f_key->cmp(_map_->buffer[i].key, min_key) < 0) \
                { \
                    min_key = _map_->buffer[i].key; \
                    min_val = _map_->buffer[i].value; \
                } \
            } \
        } \
\
        if (key) \
            *key = min_key; \
        if (value) \
            *value = min_val; \
\
        CMC_CALLBACKS_CALL(_map_, read); \
\
        return true; \
    } \
\
    V CMC_(PFX, _get)(struct SNAME * _map_, K key) \
    { \
        if (CMC_(PFX, _empty)(_map_)) \
        { \
            _map_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_get_entry)(_map_, key); \
\
        if (!entry) \
        { \
            _map_->flag = CMC_FLAG_NOT_FOUND; \
            return (V){ 0 }; \
        } \
\
        _map_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_map_, read); \
\
        return entry->value; \
    } \
\
    V *CMC_(PFX, _get_ref)(struct SNAME * _map_, K key) \
    { \
        if (CMC_(PFX, _empty)(_map_)) \
        { \
            _map_->flag = CMC_FLAG_EMPTY; \
            return NULL; \
        } \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_get_entry)(_map_, key); \
\
        if (!entry) \
        { \
            _map_->flag = CMC_FLAG_NOT_FOUND; \
            return NULL; \
        } \
\
        _map_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_map_, read); \
\
        return &(entry->value); \
    } \
\
    bool CMC_(PFX, _contains)(struct SNAME * _map_, K key) \
    { \
        _map_->flag = CMC_FLAG_OK; \
\
        bool result = CMC_(PFX, _impl_get_entry)(_map_, key) != NULL; \
\
        CMC_CALLBACKS_CALL(_map_, read); \
\
        return result; \
    } \
\
    bool CMC_(PFX, _empty)(struct SNAME * _map_) \
    { \
        return _map_->count == 0; \
    } \
\
    bool CMC_(PFX, _full)(struct SNAME * _map_) \
    { \
        return (double)_map_->capacity * _map_->load <= (double)_map_->count; \
    } \
\
    size_t CMC_(PFX, _count)(struct SNAME * _map_) \
    { \
        return _map_->count; \
    } \
\
    size_t CMC_(PFX, _capacity)(struct SNAME * _map_) \
    { \
        return _map_->capacity; \
    } \
\
    double CMC_(PFX, _load)(struct SNAME * _map_) \
    { \
        return _map_->load; \
    } \
\
    int CMC_(PFX, _flag)(struct SNAME * _map_) \
    { \
        return _map_->flag; \
    } \
\
    bool CMC_(PFX, _resize)(struct SNAME * _map_, size_t capacity) \
    { \
        _map_->flag = CMC_FLAG_OK; \
\
        if (_map_->capacity == capacity) \
            goto success; \
\
        if (_map_->capacity > capacity / _map_->load) \
            goto success; \
\
        /* Prevent integer overflow */ \
        if (capacity >= UINTMAX_MAX * _map_->load) \
        { \
            _map_->flag = CMC_FLAG_ERROR; \
            return false; \
        } \
\
        /* Calculate required capacity based on the prime numbers */ \
        size_t theoretical_size = CMC_(PFX, _impl_calculate_size)(capacity); \
\
        /* Not possible to shrink with current available prime numbers */ \
        if (theoretical_size < _map_->count / _map_->load) \
        { \
            _map_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        /* No callbacks since _new_map_ is just a temporary hashtable */ \
        struct SNAME *_new_map_ = \
            CMC_(PFX, _new_custom)(capacity, _map_->load, _map_->f_key, _map_->f_val, _map_->alloc, NULL); \
\
        if (!_new_map_) \
        { \
            _map_->flag = CMC_FLAG_ALLOC; \
            return false; \
        } \
\
        for (size_t i = 0; i < _map_->capacity; i++) \
        { \
            if (_map_->buffer[i].state == CMC_ES_FILLED) \
            { \
                K key = _map_->buffer[i].key; \
                V value = _map_->buffer[i].value; \
\
                /* TODO check this for possible errors */ \
                CMC_(PFX, _insert)(_new_map_, key, value); \
            } \
        } \
\
        /* Unlikely */ \
        if (_map_->count != _new_map_->count) \
        { \
            CMC_(PFX, _free)(_new_map_); \
\
            _map_->flag = CMC_FLAG_ERROR; \
            return false; \
        } \
\
        struct CMC_DEF_ENTRY(SNAME) *tmp_b = _map_->buffer; \
        _map_->buffer = _new_map_->buffer; \
        _new_map_->buffer = tmp_b; \
\
        size_t tmp_c = _map_->capacity; \
        _map_->capacity = _new_map_->capacity; \
        _new_map_->capacity = tmp_c; \
\
        /* Prevent the map from freeing the data */ \
        _new_map_->f_key = &(struct CMC_DEF_FKEY(SNAME)){ NULL }; \
        _new_map_->f_val = &(struct CMC_DEF_FVAL(SNAME)){ NULL }; \
\
        CMC_(PFX, _free)(_new_map_); \
\
    success: \
\
        CMC_CALLBACKS_CALL(_map_, resize); \
\
        return true; \
    } \
\
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _map_) \
    { \
        struct SNAME *result = CMC_(PFX, _new_custom)(_map_->capacity * _map_->load, _map_->load, _map_->f_key, \
                                                      _map_->f_val, _map_->alloc, NULL); \
\
        if (!result) \
        { \
            _map_->flag = CMC_FLAG_ERROR; \
            return NULL; \
        } \
\
        CMC_CALLBACKS_ASSIGN(result, _map_->callbacks); \
\
        if (_map_->f_key->cpy || _map_->f_val->cpy) \
        { \
            for (size_t i = 0; i < _map_->capacity; i++) \
            { \
                struct CMC_DEF_ENTRY(SNAME) *scan = &(_map_->buffer[i]); \
\
                if (scan->state != CMC_ES_EMPTY) \
                { \
                    struct CMC_DEF_ENTRY(SNAME) *target = &(result->buffer[i]); \
\
                    if (scan->state == CMC_ES_DELETED) \
                        target->state = CMC_ES_DELETED; \
                    else \
                    { \
                        target->state = scan->state; \
                        target->dist = scan->dist; \
\
                        if (_map_->f_key->cpy) \
                            target->key = _map_->f_key->cpy(scan->key); \
                        else \
                            target->key = scan->key; \
\
                        if (_map_->f_val->cpy) \
                            target->value = _map_->f_val->cpy(scan->value); \
                        else \
                            target->value = scan->value; \
                    } \
                } \
            } \
        } \
        else \
            memcpy(result->buffer, _map_->buffer, sizeof(struct CMC_DEF_ENTRY(SNAME)) * _map_->capacity); \
\
        result->count = _map_->count; \
\
        _map_->flag = CMC_FLAG_OK; \
\
        return result; \
    } \
\
    bool CMC_(PFX, _equals)(struct SNAME * _map1_, struct SNAME * _map2_) \
    { \
        _map1_->flag = CMC_FLAG_OK; \
        _map2_->flag = CMC_FLAG_OK; \
\
        if (_map1_->count != _map2_->count) \
            return false; \
\
        /* Optimize loop using the smallest hashtable */ \
        struct SNAME *_map_a_; \
        struct SNAME *_map_b_; \
\
        _map_a_ = _map1_->capacity < _map2_->capacity ? _map1_ : _map2_; \
        _map_b_ = _map_a_ == _map1_ ? _map2_ : _map1_; \
\
        for (size_t i = 0; i < _map_a_->capacity; i++) \
        { \
            if (_map_a_->buffer[i].state == CMC_ES_FILLED) \
            { \
                struct CMC_DEF_ENTRY(SNAME) *entry_a = &(_map_a_->buffer[i]); \
                struct CMC_DEF_ENTRY(SNAME) *entry_b = CMC_(PFX, _impl_get_entry)(_map_b_, entry_a->key); \
\
                if (!entry_b) \
                    return false; \
\
                if (_map_a_->f_val->cmp(entry_a->value, entry_b->value) != 0) \
                    return false; \
            } \
        } \
\
        return true; \
    } \
\
    static struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_get_entry)(struct SNAME * _map_, K key) \
    { \
        size_t hash = _map_->f_key->hash(key); \
        size_t pos = hash % _map_->capacity; \
\
        struct CMC_DEF_ENTRY(SNAME) *target = &(_map_->buffer[pos]); \
\
        while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED) \
        { \
            if (_map_->f_key->cmp(target->key, key) == 0) \
                return target; \
\
            pos++; \
            target = &(_map_->buffer[pos % _map_->capacity]); \
        } \
\
        return NULL; \
    } \
\
    static size_t CMC_(PFX, _impl_calculate_size)(size_t required) \
    { \
        const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]); \
\
        if (cmc_hashtable_primes[count - 1] < required) \
            return required; \
\
        size_t i = 0; \
        while (cmc_hashtable_primes[i] < required) \
            i++; \
\
        return cmc_hashtable_primes[i]; \
    }

#endif /* CMC_CMC_HASHMAP_H */
