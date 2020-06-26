/**
 * cmc/hashmultimap.h
 *
 * Creation Date: 26/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashMultiMap
 *
 * The HashMultiMap is a Map that allows for multiple keys. This is a data
 * structure that has a very narrow usage. A Map (either TreeMap or HashMap) can
 * also work like a HashMultiMap if a certain key is mapped to another
 * collection.
 *
 * Map<K = int, V = List<int>> maps an integer to a list of integer.
 * HashMultiMap<K = int, V = int> maps many integer keys to integer values.
 *
 * The difference is that in a HashMultiMap you can store keys that are the same
 * but might be different instances of the same value. This is also relevant if
 * your data type is more complex like a struct where its ID is the same but
 * some other members of this data type are different.
 *
 * Implementation
 *
 * The map uses separate chaining and robin hood hashing. Its internal buffer
 * is made of a pair of pointers to entries that represent a linked list. So
 * accessing map->buffer[0] would give the first pair of pointers. Each entry
 * has both a pointer to a previous entry and a next entry. This design choice
 * was made so that every collection in this library has a two-way iterator
 * with a very few exceptions.
 *
 * Each entry is composed of a Key and a Value. Entries with the same key should
 * always hash to the same linked list. Also, keys that hash to the same bucket
 * will also be in the same linked list.
 *
 * The order of inserting and removing the same keys will behave like a FIFO. So
 * the first key added will be the first to be removed.
 */

#ifndef CMC_CMC_HASHMULTIMAP_H
#define CMC_CMC_HASHMULTIMAP_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * Hashtable Implementation
 * ------------------------------------------------------------------------- */
#include "../cor/hashtable.h"

/**
 * Core HashMultiMap implementation
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_CMC_HASHMULTIMAP_CORE(ACCESS, FILE, PARAMS) \
    CMC_(CMC_(CMC_CMC_HASHMULTIMAP_CORE_, ACCESS), CMC_(_, FILE))(PARAMS)

/* PRIVATE or PUBLIC solver */
#define CMC_CMC_HASHMULTIMAP_CORE_PUBLIC_HEADER(PARAMS) \
    CMC_CMC_HASHMULTIMAP_CORE_STRUCT(PARAMS) \
    CMC_CMC_HASHMULTIMAP_CORE_HEADER(PARAMS)

#define CMC_CMC_HASHMULTIMAP_CORE_PUBLIC_SOURCE(PARAMS) CMC_CMC_HASHMULTIMAP_CORE_SOURCE(PARAMS)

#define CMC_CMC_HASHMULTIMAP_CORE_PRIVATE_HEADER(PARAMS) \
    struct CMC_PARAM_SNAME(PARAMS); \
    struct CMC_DEF_ENTRY(CMC_PARAM_SNAME(PARAMS)); \
    CMC_CMC_HASHMULTIMAP_CORE_HEADER(PARAMS)

#define CMC_CMC_HASHMULTIMAP_CORE_PRIVATE_SOURCE(PARAMS) \
    CMC_CMC_HASHMULTIMAP_CORE_STRUCT(PARAMS) \
    CMC_CMC_HASHMULTIMAP_CORE_SOURCE(PARAMS)

/* Lowest level API */
#define CMC_CMC_HASHMULTIMAP_CORE_STRUCT(PARAMS) \
    CMC_CMC_HASHMULTIMAP_CORE_STRUCT_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                      CMC_PARAM_V(PARAMS))

#define CMC_CMC_HASHMULTIMAP_CORE_HEADER(PARAMS) \
    CMC_CMC_HASHMULTIMAP_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                      CMC_PARAM_V(PARAMS))

#define CMC_CMC_HASHMULTIMAP_CORE_SOURCE(PARAMS) \
    CMC_CMC_HASHMULTIMAP_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                      CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Struct
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHMULTIMAP_CORE_STRUCT_(PFX, SNAME, K, V) \
\
    /* HashMultimap Structure */ \
    struct SNAME \
    { \
        /* Array of linked list to entries */ \
        struct CMC_DEF_ENTRY(SNAME) * (*buffer)[2]; \
\
        /* Current array capacity */ \
        size_t capacity; \
\
        /* Current amount of keys */ \
        size_t count; \
\
        /* Load factor in range (0.0, infinity) */ \
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
    /* HashMultimap Entry */ \
    struct CMC_DEF_ENTRY(SNAME) \
    { \
        /* Entry Key */ \
        K key; \
\
        /* Entry Value */ \
        V value; \
\
        /* Next entry on the linked list */ \
        struct CMC_DEF_ENTRY(SNAME) * next; \
\
        /* Previous entry on the linked list */ \
        struct CMC_DEF_ENTRY(SNAME) * prev; \
    };

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHMULTIMAP_CORE_HEADER_(PFX, SNAME, K, V) \
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
    size_t CMC_(PFX, _update_all)(struct SNAME * _map_, K key, V new_value, V * *old_values); \
    bool CMC_(PFX, _remove)(struct SNAME * _map_, K key, V * out_value); \
    size_t CMC_(PFX, _remove_all)(struct SNAME * _map_, K key, V * *out_values); \
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
    size_t CMC_(PFX, _key_count)(struct SNAME * _map_, K key); \
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
#define CMC_CMC_HASHMULTIMAP_CORE_SOURCE_(PFX, SNAME, K, V) \
\
    /* Implementation Detail Functions */ \
    struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_new_entry)(struct SNAME * _map_, K key, V value); \
    struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_get_entry)(struct SNAME * _map_, K key); \
    size_t CMC_(PFX, _impl_key_count)(struct SNAME * _map_, K key); \
    size_t CMC_(PFX, _impl_calculate_size)(size_t required); \
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
        if (capacity == 0 || load <= 0) \
            return NULL; \
\
        /* Prevent integer overflow */ \
        if (capacity >= UINTMAX_MAX * load) \
            return NULL; \
\
        if (!f_key || !f_val) \
            return NULL; \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        size_t real_capacity = CMC_(PFX, _impl_calculate_size)(capacity / load); \
\
        struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_map_) \
            return NULL; \
\
        _map_->buffer = alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME) *[2])); \
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
        for (size_t i = 0; i < _map_->capacity; i++) \
        { \
            struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0]; \
\
            while (scan) \
            { \
                struct CMC_DEF_ENTRY(SNAME) *next = scan->next; \
\
                if (_map_->f_key->free) \
                    _map_->f_key->free(scan->key); \
                if (_map_->f_val->free) \
                    _map_->f_val->free(scan->value); \
\
                _map_->alloc->free(scan); \
\
                scan = next; \
            } \
        } \
\
        memset(_map_->buffer, 0, sizeof(struct CMC_DEF_ENTRY(SNAME) *[2]) * _map_->capacity); \
\
        _map_->count = 0; \
        _map_->flag = CMC_FLAG_OK; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _map_) \
    { \
        size_t index = 0; \
\
        while (index < _map_->capacity) \
        { \
            struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[index][0]; \
\
            if (scan) \
            { \
                if (scan->next == NULL && scan->prev == NULL) \
                { \
                    if (_map_->f_key->free) \
                        _map_->f_key->free(scan->key); \
                    if (_map_->f_val->free) \
                        _map_->f_val->free(scan->value); \
\
                    _map_->alloc->free(scan); \
                } \
                else \
                { \
                    while (scan) \
                    { \
                        struct CMC_DEF_ENTRY(SNAME) *tmp = scan; \
\
                        scan = scan->next; \
\
                        if (_map_->f_key->free) \
                            _map_->f_key->free(tmp->key); \
                        if (_map_->f_val->free) \
                            _map_->f_val->free(tmp->value); \
\
                        _map_->alloc->free(tmp); \
                    } \
                } \
            } \
\
            index++; \
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
        size_t hash = _map_->f_key->hash(key); \
        size_t pos = hash % _map_->capacity; \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_new_entry)(_map_, key, value); \
\
        if (_map_->buffer[pos][0] == NULL) \
        { \
            _map_->buffer[pos][0] = entry; \
            _map_->buffer[pos][1] = entry; \
        } \
        else \
        { \
            entry->prev = _map_->buffer[pos][1]; \
\
            _map_->buffer[pos][1]->next = entry; \
            _map_->buffer[pos][1] = entry; \
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
    size_t CMC_(PFX, _update_all)(struct SNAME * _map_, K key, V new_value, V * *old_values) \
    { \
        if (CMC_(PFX, _empty)(_map_)) \
        { \
            _map_->flag = CMC_FLAG_EMPTY; \
            return 0; \
        } \
\
        size_t hash = _map_->f_key->hash(key); \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[hash % _map_->capacity][0]; \
\
        if (entry == NULL) \
        { \
            _map_->flag = CMC_FLAG_NOT_FOUND; \
            return 0; \
        } \
\
        if (old_values) \
        { \
            size_t total = CMC_(PFX, _impl_key_count)(_map_, key); \
\
            if (total == 0) \
            { \
                _map_->flag = CMC_FLAG_NOT_FOUND; \
                return 0; \
            } \
\
            *old_values = _map_->alloc->malloc(sizeof(V) * total); \
\
            if (!(*old_values)) \
            { \
                _map_->flag = CMC_FLAG_ALLOC; \
                return 0; \
            } \
        } \
\
        size_t index = 0; \
\
        while (entry) \
        { \
            if (_map_->f_key->cmp(entry->key, key) == 0) \
            { \
                if (old_values) \
                    (*old_values)[index] = entry->value; \
\
                index++; \
                entry->value = new_value; \
            } \
\
            entry = entry->next; \
        } \
\
        _map_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_map_, update); \
\
        return index; \
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
        size_t hash = _map_->f_key->hash(key); \
\
        struct CMC_DEF_ENTRY(SNAME) **head = &(_map_->buffer[hash % _map_->capacity][0]); \
        struct CMC_DEF_ENTRY(SNAME) **tail = &(_map_->buffer[hash % _map_->capacity][1]); \
\
        if (*head == NULL) \
        { \
            _map_->flag = CMC_FLAG_NOT_FOUND; \
            return false; \
        } \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = *head; \
\
        if (entry->next == NULL && entry->prev == NULL) \
        { \
            if (_map_->f_key->cmp(entry->key, key) == 0) \
            { \
                *head = NULL; \
                *tail = NULL; \
\
                if (out_value) \
                    *out_value = entry->value; \
            } \
            else \
            { \
                _map_->flag = CMC_FLAG_NOT_FOUND; \
                return false; \
            } \
        } \
        else \
        { \
            bool found = false; \
\
            while (entry) \
            { \
                if (_map_->f_key->cmp(entry->key, key) == 0) \
                { \
                    if (*head == entry) \
                        *head = entry->next; \
                    if (*tail == entry) \
                        *tail = entry->prev; \
\
                    if (entry->prev) \
                        entry->prev->next = entry->next; \
                    if (entry->next) \
                        entry->next->prev = entry->prev; \
\
                    if (out_value) \
                        *out_value = entry->value; \
\
                    found = true; \
\
                    break; \
                } \
                else \
                    entry = entry->next; \
            } \
\
            if (!found) \
            { \
                _map_->flag = CMC_FLAG_NOT_FOUND; \
                return false; \
            } \
        } \
\
        _map_->alloc->free(entry); \
\
        _map_->count--; \
        _map_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_map_, delete); \
\
        return true; \
    } \
\
    size_t CMC_(PFX, _remove_all)(struct SNAME * _map_, K key, V * *out_values) \
    { \
        if (CMC_(PFX, _empty)(_map_)) \
        { \
            _map_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        size_t hash = _map_->f_key->hash(key); \
\
        struct CMC_DEF_ENTRY(SNAME) **head = &(_map_->buffer[hash % _map_->capacity][0]); \
        struct CMC_DEF_ENTRY(SNAME) **tail = &(_map_->buffer[hash % _map_->capacity][1]); \
\
        if (*head == NULL) \
        { \
            _map_->flag = CMC_FLAG_NOT_FOUND; \
            return 0; \
        } \
\
        if (out_values) \
        { \
            size_t total = CMC_(PFX, _impl_key_count)(_map_, key); \
\
            if (total == 0) \
            { \
                _map_->flag = CMC_FLAG_NOT_FOUND; \
                return 0; \
            } \
\
            *out_values = _map_->alloc->malloc(sizeof(V) * total); \
\
            if (!(*out_values)) \
            { \
                _map_->flag = CMC_FLAG_ALLOC; \
                return 0; \
            } \
        } \
\
        size_t index = 0; \
        struct CMC_DEF_ENTRY(SNAME) *entry = *head; \
\
        if (entry->next == NULL) \
        { \
            *head = NULL; \
            *tail = NULL; \
\
            if (out_values) \
                (*out_values)[index] = entry->value; \
\
            index++; \
            _map_->alloc->free(entry); \
        } \
        else \
        { \
            while (entry) \
            { \
                if (_map_->f_key->cmp(entry->key, key) == 0) \
                { \
                    if (*head == entry) \
                        *head = entry->next; \
                    if (*tail == entry) \
                        *tail = entry->prev; \
\
                    struct CMC_DEF_ENTRY(SNAME) *next = entry->next; \
\
                    if (entry->prev) \
                        entry->prev->next = entry->next; \
                    if (entry->next) \
                        entry->next->prev = entry->prev; \
\
                    if (out_values) \
                        (*out_values)[index] = entry->value; \
\
                    index++; \
                    _map_->alloc->free(entry); \
\
                    entry = next; \
                } \
                else \
                    entry = entry->next; \
            } \
        } \
\
        _map_->count -= index; \
        _map_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_map_, delete); \
\
        return index; \
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
            struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0]; \
\
            while (scan) \
            { \
                if (first) \
                { \
                    max_key = scan->key; \
                    max_val = scan->value; \
\
                    first = false; \
                } \
                else if (_map_->f_key->cmp(scan->key, max_key) > 0) \
                { \
                    max_key = scan->key; \
                    max_val = scan->value; \
                } \
\
                scan = scan->next; \
            } \
        } \
\
        if (key) \
            *key = max_key; \
        if (value) \
            *value = max_val; \
\
        _map_->flag = CMC_FLAG_OK; \
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
            struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0]; \
\
            while (scan) \
            { \
                if (first) \
                { \
                    min_key = scan->key; \
                    min_val = scan->value; \
\
                    first = false; \
                } \
                else if (_map_->f_key->cmp(scan->key, min_key) < 0) \
                { \
                    min_key = scan->key; \
                    min_val = scan->value; \
                } \
\
                scan = scan->next; \
            } \
        } \
\
        if (key) \
            *key = min_key; \
        if (value) \
            *value = min_val; \
\
        _map_->flag = CMC_FLAG_OK; \
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
    size_t CMC_(PFX, _key_count)(struct SNAME * _map_, K key) \
    { \
        return CMC_(PFX, _impl_key_count)(_map_, key); \
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
            struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0]; \
\
            while (scan) \
            { \
                /* TODO check for errors */ \
                CMC_(PFX, _insert)(_new_map_, scan->key, scan->value); \
\
                scan = scan->next; \
            } \
        } \
\
        if (_map_->count != _new_map_->count) \
        { \
            CMC_(PFX, _free)(_new_map_); \
\
            _map_->flag = CMC_FLAG_ERROR; \
            return false; \
        } \
\
        struct CMC_DEF_ENTRY(SNAME) * (*tmp_b)[2] = _map_->buffer; \
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
        /* Callback will be added later */ \
        struct SNAME *result = CMC_(PFX, _new_custom)(_map_->capacity * _map_->load, _map_->load, _map_->f_key, \
                                                      _map_->f_val, _map_->alloc, NULL); \
\
        if (!result) \
        { \
            _map_->flag = CMC_FLAG_ERROR; \
            return NULL; \
        } \
\
        for (size_t i = 0; i < _map_->capacity; i++) \
        { \
            struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0]; \
\
            while (scan) \
            { \
                K key;\
                V value;\
\
                if (_map_->f_key->cpy) \
                    key = _map_->f_key->cpy(scan->key); \
                else\
                    key = scan->key;\
                if (_map_->f_val->cpy) \
                    value = _map_->f_val->cpy(scan->value); \
                else\
                    value = scan->value;\
\
                /* TODO check for errors */ \
                CMC_(PFX, _insert)(result, key, value); \
\
                scan = scan->next; \
            } \
        } \
\
        CMC_CALLBACKS_ASSIGN(result, _map_->callbacks); \
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
            struct CMC_DEF_ENTRY(SNAME) *scan = _map_a_->buffer[i][0]; \
\
            while (scan) \
            { \
                /* OPTIMIZE - This is calling key_count for repeating keys */\
                if (CMC_(PFX, _impl_key_count)(_map_a_, scan->key) != CMC_(PFX, _impl_key_count)(_map_b_, scan->key))\
                    return false;\
\
                scan = scan->next; \
            } \
        } \
\
        return true; \
    } \
\
    struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_new_entry)(struct SNAME * _map_, K key, V value) \
    { \
        struct CMC_DEF_ENTRY(SNAME) *entry = _map_->alloc->malloc(sizeof(struct CMC_DEF_ENTRY(SNAME))); \
\
        if (!entry) \
            return NULL; \
\
        entry->key = key; \
        entry->value = value; \
        entry->next = NULL; \
        entry->prev = NULL; \
\
        return entry; \
    } \
\
    struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_get_entry)(struct SNAME * _map_, K key) \
    { \
        size_t hash = _map_->f_key->hash(key); \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[hash % _map_->capacity][0]; \
\
        while (entry) \
        { \
            if (_map_->f_key->cmp(entry->key, key) == 0) \
                return entry; \
\
            entry = entry->next; \
        } \
\
        return NULL; \
    } \
\
    size_t CMC_(PFX, _impl_key_count)(struct SNAME * _map_, K key) \
    { \
        size_t hash = _map_->f_key->hash(key); \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[hash % _map_->capacity][0]; \
\
        size_t total_count = 0; \
\
        if (!entry) \
            return total_count; \
\
        while (entry) \
        { \
            if (_map_->f_key->cmp(entry->key, key) == 0) \
                total_count++; \
\
            entry = entry->next; \
        } \
\
        return total_count; \
    } \
\
    size_t CMC_(PFX, _impl_calculate_size)(size_t required) \
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

#endif /* CMC_CMC_HASHMULTIMAP_H */
