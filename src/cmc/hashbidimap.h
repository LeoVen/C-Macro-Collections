/**
 * hashbidimap.h
 *
 * Creation Date: 26/09/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashBidiMap
 *
 * A bidirectional map is a map that allows you to create a bijection in both
 * directions between two sets of elements (K <-> V). It is possible to retrieve
 * the value using a key or retrieve a key using a value. The naming is simply
 * used to differentiate between one set of elements and the other set of
 * elements.
 *
 * Implementation
 *
 * This implementation uses two arrays of pointers to an entry containing both
 * the key and the value.
 */

#ifndef CMC_CMC_HASHBIDIMAP_H
#define CMC_CMC_HASHBIDIMAP_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * Hashtable Implementation
 * ------------------------------------------------------------------------- */
#include "../cor/hashtable.h"

/* -------------------------------------------------------------------------
 * HashBidimap Specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_cmc_string_fmt_hashbidimap = "struct %s<%s, %s> "
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
 * Core HashBidiMap implementation
 */
#define CMC_CMC_HASHBIDIMAP_CORE(BODY)    \
    CMC_CMC_HASHBIDIMAP_CORE_HEADER(BODY) \
    CMC_CMC_HASHBIDIMAP_CORE_SOURCE(BODY)

#define CMC_CMC_HASHBIDIMAP_CORE_HEADER(BODY)                                  \
    CMC_CMC_HASHBIDIMAP_CORE_HEADER_(CMC_PARAM_PFX(BODY),                      \
                                     CMC_PARAM_SNAME(BODY), CMC_PARAM_K(BODY), \
                                     CMC_PARAM_V(BODY))

#define CMC_CMC_HASHBIDIMAP_CORE_SOURCE(BODY)                                  \
    CMC_CMC_HASHBIDIMAP_CORE_SOURCE_(CMC_PARAM_PFX(BODY),                      \
                                     CMC_PARAM_SNAME(BODY), CMC_PARAM_K(BODY), \
                                     CMC_PARAM_V(BODY))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHBIDIMAP_CORE_HEADER_(PFX, SNAME, K, V)                    \
                                                                              \
    /* HashBidiMap Structure */                                               \
    struct SNAME                                                              \
    {                                                                         \
        /* Array 0 is K -> V and array 1 is V -> K */                         \
        struct CMC_DEF_ENTRY(SNAME) * (*buffer)[2];                           \
                                                                              \
        /* Current arrays capacity */                                         \
        size_t capacity;                                                      \
                                                                              \
        /* Current amount of keys */                                          \
        size_t count;                                                         \
                                                                              \
        /* Load factor in range (0.0, 1.0) */                                 \
        double load;                                                          \
                                                                              \
        /* Flags indicating errors or success */                              \
        int flag;                                                             \
                                                                              \
        /* Key function table */                                              \
        struct CMC_DEF_FKEY(SNAME) * f_key;                                   \
                                                                              \
        /* Value function table */                                            \
        struct CMC_DEF_FVAL(SNAME) * f_val;                                   \
                                                                              \
        /* Custom allocation functions */                                     \
        struct cmc_alloc_node *alloc;                                         \
                                                                              \
        /* Custom callback functions */                                       \
        struct cmc_callbacks *callbacks;                                      \
                                                                              \
        /* Methods */                                                         \
        /* Returns an iterator to the start of the hashbidimap */             \
        struct CMC_DEF_ITER(SNAME) (*it_start)(struct SNAME *);               \
                                                                              \
        /* Returns an iterator to the end of the hashbidimap */               \
        struct CMC_DEF_ITER(SNAME) (*it_end)(struct SNAME *);                 \
    };                                                                        \
                                                                              \
    /* HashBidiMap Entry */                                                   \
    struct CMC_DEF_ENTRY(SNAME)                                               \
    {                                                                         \
        /* Entry Key */                                                       \
        K key;                                                                \
                                                                              \
        /* Entry Value */                                                     \
        V value;                                                              \
                                                                              \
        /* The distance of this node to its original position */              \
        /* dist[0] is relative to K -> V */                                   \
        /* dist[1] is relative to V -> K */                                   \
        size_t dist[2];                                                       \
                                                                              \
        /* References to this node in the hashbidimap buffer. Used to */      \
        /* prevent searching for this node twice for update() and */          \
        /* remove(). Increases memory overhead but reduces execution time */  \
        /* ref[0] is relative to K -> V */                                    \
        /* ref[1] is relative to V -> K */                                    \
        struct CMC_DEF_ENTRY(SNAME) * *ref[2];                                \
    };                                                                        \
                                                                              \
    /* Key struct function table */                                           \
    struct CMC_DEF_FKEY(SNAME)                                                \
    {                                                                         \
        /* Comparator function */                                             \
        int (*cmp)(K, K);                                                     \
                                                                              \
        /* Copy function */                                                   \
        K (*cpy)(K);                                                          \
                                                                              \
        /* To string function */                                              \
        bool (*str)(FILE *, K);                                               \
                                                                              \
        /* Free from memory function */                                       \
        void (*free)(K);                                                      \
                                                                              \
        /* Hash function */                                                   \
        size_t (*hash)(K);                                                    \
                                                                              \
        /* Priority function */                                               \
        int (*pri)(K, K);                                                     \
    };                                                                        \
                                                                              \
    /* Value struct function table */                                         \
    struct CMC_DEF_FVAL(SNAME)                                                \
    {                                                                         \
        /* Comparator function */                                             \
        int (*cmp)(V, V);                                                     \
                                                                              \
        /* Copy function */                                                   \
        V (*cpy)(V);                                                          \
                                                                              \
        /* To string function */                                              \
        bool (*str)(FILE *, V);                                               \
                                                                              \
        /* Free from memory function */                                       \
        void (*free)(V);                                                      \
                                                                              \
        /* Hash function */                                                   \
        size_t (*hash)(V);                                                    \
                                                                              \
        /* Priority function */                                               \
        int (*pri)(V, V);                                                     \
    };                                                                        \
                                                                              \
    /* HashBidiMap Iterator */                                                \
    struct CMC_DEF_ITER(SNAME)                                                \
    {                                                                         \
        /* Target hashbidimap */                                              \
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
    /* Collection Functions */                                                \
    /* Collection Allocation and Deallocation */                              \
    struct SNAME *CMC_(PFX, _new)(size_t capacity, double load,               \
                                  struct CMC_DEF_FKEY(SNAME) * f_key,         \
                                  struct CMC_DEF_FVAL(SNAME) * f_val);        \
    struct SNAME *CMC_(PFX, _new_custom)(                                     \
        size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,     \
        struct CMC_DEF_FVAL(SNAME) * f_val, struct cmc_alloc_node * alloc,    \
        struct cmc_callbacks * callbacks);                                    \
    void CMC_(PFX, _clear)(struct SNAME * _map_);                             \
    void CMC_(PFX, _free)(struct SNAME * _map_);                              \
    /* Customization of Allocation and Callbacks */                           \
    void CMC_(PFX, _customize)(struct SNAME * _map_,                          \
                               struct cmc_alloc_node * alloc,                 \
                               struct cmc_callbacks * callbacks);             \
    /* Collection Input and Output */                                         \
    bool CMC_(PFX, _insert)(struct SNAME * _map_, K key, V value);            \
    bool CMC_(PFX, _update_key)(struct SNAME * _map_, V val, K new_key);      \
    bool CMC_(PFX, _update_val)(struct SNAME * _map_, K key, V new_val);      \
    bool CMC_(PFX, _remove_by_key)(struct SNAME * _map_, K key, K * out_key,  \
                                   V * out_val);                              \
    bool CMC_(PFX, _remove_by_val)(struct SNAME * _map_, V val, K * out_key,  \
                                   V * out_val);                              \
    /* Element Access */                                                      \
    K CMC_(PFX, _get_key)(struct SNAME * _map_, V val);                       \
    V CMC_(PFX, _get_val)(struct SNAME * _map_, K key);                       \
    /* Collection State */                                                    \
    bool CMC_(PFX, _contains_key)(struct SNAME * _map_, K key);               \
    bool CMC_(PFX, _contains_val)(struct SNAME * _map_, V val);               \
    bool CMC_(PFX, _empty)(struct SNAME * _map_);                             \
    bool CMC_(PFX, _full)(struct SNAME * _map_);                              \
    size_t CMC_(PFX, _count)(struct SNAME * _map_);                           \
    size_t CMC_(PFX, _capacity)(struct SNAME * _map_);                        \
    double CMC_(PFX, _load)(struct SNAME * _map_);                            \
    int CMC_(PFX, _flag)(struct SNAME * _map_);                               \
    /* Collection Utility */                                                  \
    bool CMC_(PFX, _resize)(struct SNAME * _map_, size_t capacity);           \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _map_);                  \
    bool CMC_(PFX, _equals)(struct SNAME * _map1_, struct SNAME * _map2_);    \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _map_);            \
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr);                \
                                                                              \
    /* Iterator Functions */                                                  \
    /* Iterator Allocation and Deallocation */                                \
    struct CMC_DEF_ITER(SNAME) * CMC_(PFX, _iter_new)(struct SNAME * target); \
    void CMC_(PFX, _iter_free)(struct CMC_DEF_ITER(SNAME) * iter);            \
    /* Iterator Initialization */                                             \
    void CMC_(PFX, _iter_init)(struct CMC_DEF_ITER(SNAME) * iter,             \
                               struct SNAME * target);                        \
    /* Iterator State */                                                      \
    bool CMC_(PFX, _iter_start)(struct CMC_DEF_ITER(SNAME) * iter);           \
    bool CMC_(PFX, _iter_end)(struct CMC_DEF_ITER(SNAME) * iter);             \
    /* Iterator Movement */                                                   \
    void CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter);        \
    void CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter);          \
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
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHBIDIMAP_CORE_SOURCE_(PFX, SNAME, K, V)                     \
                                                                               \
    /* Implementation Detail Functions */                                      \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        CMC_(PFX, _impl_new_entry)(struct SNAME * _map_, K key, V value);      \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        *CMC_(PFX, _impl_get_entry_by_key)(struct SNAME * _map_, K key);       \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        *CMC_(PFX, _impl_get_entry_by_val)(struct SNAME * _map_, V val);       \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        *CMC_(PFX, _impl_add_entry_to_key)(                                    \
            struct SNAME * _map_, struct CMC_DEF_ENTRY(SNAME) * entry);        \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        *CMC_(PFX, _impl_add_entry_to_val)(                                    \
            struct SNAME * _map_, struct CMC_DEF_ENTRY(SNAME) * entry);        \
    static size_t CMC_(PFX, _impl_calculate_size)(size_t required);            \
    static struct CMC_DEF_ITER(SNAME)                                          \
        CMC_(PFX, _impl_it_start)(struct SNAME * _map_);                       \
    static struct CMC_DEF_ITER(SNAME)                                          \
        CMC_(PFX, _impl_it_end)(struct SNAME * _map_);                         \
                                                                               \
    struct SNAME *CMC_(PFX, _new)(size_t capacity, double load,                \
                                  struct CMC_DEF_FKEY(SNAME) * f_key,          \
                                  struct CMC_DEF_FVAL(SNAME) * f_val)          \
    {                                                                          \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;                \
                                                                               \
        if (capacity == 0 || load <= 0 || load >= 1)                           \
            return NULL;                                                       \
                                                                               \
        /* Prevent integer overflow */                                         \
        if (capacity >= UINTMAX_MAX * load)                                    \
            return NULL;                                                       \
                                                                               \
        if (!f_key || !f_val)                                                  \
            return NULL;                                                       \
                                                                               \
        size_t real_capacity =                                                 \
            CMC_(PFX, _impl_calculate_size)(capacity / load);                  \
                                                                               \
        struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_map_)                                                            \
            return NULL;                                                       \
                                                                               \
        _map_->buffer = alloc->calloc(                                         \
            real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME) *[2]));          \
                                                                               \
        if (!_map_->buffer)                                                    \
        {                                                                      \
            alloc->free(_map_->buffer);                                        \
            alloc->free(_map_);                                                \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _map_->count = 0;                                                      \
        _map_->capacity = real_capacity;                                       \
        _map_->load = load;                                                    \
        _map_->flag = CMC_FLAG_OK;                                             \
        _map_->f_key = f_key;                                                  \
        _map_->f_val = f_val;                                                  \
        _map_->alloc = alloc;                                                  \
        _map_->callbacks = NULL;                                               \
        _map_->it_start = CMC_(PFX, _impl_it_start);                           \
        _map_->it_end = CMC_(PFX, _impl_it_end);                               \
                                                                               \
        return _map_;                                                          \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _new_custom)(                                      \
        size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,      \
        struct CMC_DEF_FVAL(SNAME) * f_val, struct cmc_alloc_node * alloc,     \
        struct cmc_callbacks * callbacks)                                      \
    {                                                                          \
        if (capacity == 0 || load <= 0 || load >= 1)                           \
            return NULL;                                                       \
                                                                               \
        /* Prevent integer overflow */                                         \
        if (capacity >= UINTMAX_MAX * load)                                    \
            return NULL;                                                       \
                                                                               \
        if (!f_key || !f_val)                                                  \
            return NULL;                                                       \
                                                                               \
        size_t real_capacity =                                                 \
            CMC_(PFX, _impl_calculate_size)(capacity / load);                  \
                                                                               \
        if (!alloc)                                                            \
            alloc = &cmc_alloc_node_default;                                   \
                                                                               \
        struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_map_)                                                            \
            return NULL;                                                       \
                                                                               \
        _map_->buffer = alloc->calloc(                                         \
            real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME) *[2]));          \
                                                                               \
        if (!_map_->buffer)                                                    \
        {                                                                      \
            alloc->free(_map_->buffer);                                        \
            alloc->free(_map_);                                                \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _map_->count = 0;                                                      \
        _map_->capacity = real_capacity;                                       \
        _map_->load = load;                                                    \
        _map_->flag = CMC_FLAG_OK;                                             \
        _map_->f_key = f_key;                                                  \
        _map_->f_val = f_val;                                                  \
        _map_->flag = CMC_FLAG_OK;                                             \
        _map_->alloc = alloc;                                                  \
        _map_->callbacks = callbacks;                                          \
        _map_->it_start = CMC_(PFX, _impl_it_start);                           \
        _map_->it_end = CMC_(PFX, _impl_it_end);                               \
                                                                               \
        return _map_;                                                          \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _clear)(struct SNAME * _map_)                               \
    {                                                                          \
        for (size_t i = 0; i < _map_->capacity; i++)                           \
        {                                                                      \
            struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[i][0];          \
                                                                               \
            if (entry && entry != CMC_ENTRY_DELETED)                           \
            {                                                                  \
                if (_map_->f_key->free)                                        \
                    _map_->f_key->free(entry->key);                            \
                if (_map_->f_val->free)                                        \
                    _map_->f_val->free(entry->value);                          \
                                                                               \
                _map_->alloc->free(entry);                                     \
            }                                                                  \
                                                                               \
            _map_->buffer[i][0] = NULL;                                        \
            _map_->buffer[i][1] = NULL;                                        \
        }                                                                      \
                                                                               \
        _map_->count = 0;                                                      \
        _map_->flag = CMC_FLAG_OK;                                             \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _free)(struct SNAME * _map_)                                \
    {                                                                          \
        CMC_(PFX, _clear)(_map_);                                              \
                                                                               \
        _map_->alloc->free(_map_->buffer);                                     \
        _map_->alloc->free(_map_);                                             \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _customize)(struct SNAME * _map_,                           \
                               struct cmc_alloc_node * alloc,                  \
                               struct cmc_callbacks * callbacks)               \
    {                                                                          \
        if (!alloc)                                                            \
            _map_->alloc = &cmc_alloc_node_default;                            \
        else                                                                   \
            _map_->alloc = alloc;                                              \
                                                                               \
        _map_->callbacks = callbacks;                                          \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _insert)(struct SNAME * _map_, K key, V value)              \
    {                                                                          \
        if (CMC_(PFX, _full)(_map_))                                           \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_map_, _map_->capacity + 1))               \
                return false;                                                  \
        }                                                                      \
                                                                               \
        if (CMC_(PFX, _impl_get_entry_by_key)(_map_, key) != NULL ||           \
            CMC_(PFX, _impl_get_entry_by_val)(_map_, value) != NULL)           \
        {                                                                      \
            _map_->flag = CMC_FLAG_DUPLICATE;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) *entry =                                   \
            CMC_(PFX, _impl_new_entry)(_map_, key, value);                     \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) **key_entry =                              \
            CMC_(PFX, _impl_add_entry_to_key)(_map_, entry);                   \
        struct CMC_DEF_ENTRY(SNAME) **val_entry =                              \
            CMC_(PFX, _impl_add_entry_to_val)(_map_, entry);                   \
                                                                               \
        if (!key_entry || !val_entry)                                          \
        {                                                                      \
            if (key_entry)                                                     \
                *key_entry = CMC_ENTRY_DELETED;                                \
            if (val_entry)                                                     \
                *val_entry = CMC_ENTRY_DELETED;                                \
                                                                               \
            _map_->alloc->free(entry);                                         \
                                                                               \
            _map_->flag = CMC_FLAG_ERROR;                                      \
                                                                               \
            return false;                                                      \
        }                                                                      \
                                                                               \
        _map_->count++;                                                        \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->create)                      \
            _map_->callbacks->create();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _update_key)(struct SNAME * _map_, V val, K new_key)        \
    {                                                                          \
        if (CMC_(PFX, _empty)(_map_))                                          \
        {                                                                      \
            _map_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) * *key_entry, **val_entry;                 \
                                                                               \
        val_entry = CMC_(PFX, _impl_get_entry_by_val)(_map_, val);             \
                                                                               \
        if (!val_entry)                                                        \
        {                                                                      \
            _map_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* The mapping val -> new_key is already true */                       \
        if (_map_->f_key->cmp(new_key, (*val_entry)->key) == 0)                \
            goto success;                                                      \
                                                                               \
        if (CMC_(PFX, _impl_get_entry_by_key)(_map_, new_key) != NULL)         \
        {                                                                      \
            _map_->flag = CMC_FLAG_DUPLICATE;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        key_entry = (*val_entry)->ref[0];                                      \
                                                                               \
        if (!key_entry || *key_entry != *val_entry)                            \
        {                                                                      \
            /* Should never happen */                                          \
            _map_->flag = CMC_FLAG_ERROR;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* Remove entry from key buffer and add it again with new key */       \
        struct CMC_DEF_ENTRY(SNAME) *to_add = *key_entry;                      \
        K tmp_key = to_add->key;                                               \
        to_add->key = new_key;                                                 \
                                                                               \
        *key_entry = CMC_ENTRY_DELETED;                                        \
                                                                               \
        if (!CMC_(PFX, _impl_add_entry_to_key)(_map_, to_add))                 \
        {                                                                      \
            /* Revert changes */                                               \
            to_add->key = tmp_key;                                             \
            *key_entry = to_add;                                               \
                                                                               \
            _map_->flag = CMC_FLAG_ERROR;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
    success:                                                                   \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->update)                      \
            _map_->callbacks->update();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _update_val)(struct SNAME * _map_, K key, V new_val)        \
    {                                                                          \
        if (CMC_(PFX, _empty)(_map_))                                          \
        {                                                                      \
            _map_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) * *key_entry, **val_entry;                 \
                                                                               \
        key_entry = CMC_(PFX, _impl_get_entry_by_key)(_map_, key);             \
                                                                               \
        if (!key_entry)                                                        \
        {                                                                      \
            _map_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* The mapping key -> new_val is already true */                       \
        if (_map_->f_key->cmp(new_val, (*key_entry)->value) == 0)              \
            goto success;                                                      \
                                                                               \
        if (CMC_(PFX, _impl_get_entry_by_val)(_map_, new_val) != NULL)         \
        {                                                                      \
            _map_->flag = CMC_FLAG_DUPLICATE;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        val_entry = (*key_entry)->ref[1];                                      \
                                                                               \
        if (!val_entry || *val_entry != *key_entry)                            \
        {                                                                      \
            /* Should never happen */                                          \
            _map_->flag = CMC_FLAG_ERROR;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* Remove entry from value buffer and add it again with new value */   \
        struct CMC_DEF_ENTRY(SNAME) *to_add = *val_entry;                      \
        V tmp_val = to_add->value;                                             \
        to_add->value = new_val;                                               \
                                                                               \
        *val_entry = CMC_ENTRY_DELETED;                                        \
                                                                               \
        if (!CMC_(PFX, _impl_add_entry_to_val)(_map_, to_add))                 \
        {                                                                      \
            /* Revert changes */                                               \
            to_add->value = tmp_val;                                           \
            *val_entry = to_add;                                               \
                                                                               \
            _map_->flag = CMC_FLAG_ERROR;                                      \
                                                                               \
            return false;                                                      \
        }                                                                      \
                                                                               \
    success:                                                                   \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->update)                      \
            _map_->callbacks->update();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _remove_by_key)(struct SNAME * _map_, K key, K * out_key,   \
                                   V * out_val)                                \
    {                                                                          \
        if (CMC_(PFX, _empty)(_map_))                                          \
        {                                                                      \
            _map_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) * *key_entry, **val_entry;                 \
                                                                               \
        key_entry = CMC_(PFX, _impl_get_entry_by_key)(_map_, key);             \
                                                                               \
        if (!key_entry)                                                        \
        {                                                                      \
            _map_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        val_entry = (*key_entry)->ref[1];                                      \
                                                                               \
        if (!val_entry || *val_entry != *key_entry)                            \
        {                                                                      \
            /* Should never happen */                                          \
            _map_->flag = CMC_FLAG_ERROR;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (out_key)                                                           \
            *out_key = (*key_entry)->key;                                      \
        if (out_val)                                                           \
            *out_val = (*key_entry)->value;                                    \
                                                                               \
        _map_->alloc->free(*key_entry);                                        \
                                                                               \
        *key_entry = CMC_ENTRY_DELETED;                                        \
        *val_entry = CMC_ENTRY_DELETED;                                        \
                                                                               \
        _map_->count--;                                                        \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->delete)                      \
            _map_->callbacks->delete ();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _remove_by_val)(struct SNAME * _map_, V val, K * out_key,   \
                                   V * out_val)                                \
    {                                                                          \
        if (CMC_(PFX, _empty)(_map_))                                          \
        {                                                                      \
            _map_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) * *key_entry, **val_entry;                 \
                                                                               \
        val_entry = CMC_(PFX, _impl_get_entry_by_val)(_map_, val);             \
                                                                               \
        if (!val_entry)                                                        \
        {                                                                      \
            _map_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        key_entry = (*val_entry)->ref[0];                                      \
                                                                               \
        if (!key_entry || *key_entry != *val_entry)                            \
        {                                                                      \
            /* Should never happen */                                          \
            _map_->flag = CMC_FLAG_ERROR;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (out_key)                                                           \
            *out_key = (*val_entry)->key;                                      \
        if (out_val)                                                           \
            *out_val = (*val_entry)->value;                                    \
                                                                               \
        _map_->alloc->free(*val_entry);                                        \
                                                                               \
        *key_entry = CMC_ENTRY_DELETED;                                        \
        *val_entry = CMC_ENTRY_DELETED;                                        \
                                                                               \
        _map_->count--;                                                        \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->delete)                      \
            _map_->callbacks->delete ();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    K CMC_(PFX, _get_key)(struct SNAME * _map_, V val)                         \
    {                                                                          \
        struct CMC_DEF_ENTRY(SNAME) **entry =                                  \
            CMC_(PFX, _impl_get_entry_by_val)(_map_, val);                     \
                                                                               \
        if (!entry)                                                            \
        {                                                                      \
            _map_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return (*entry)->key;                                                  \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _get_val)(struct SNAME * _map_, K key)                         \
    {                                                                          \
        struct CMC_DEF_ENTRY(SNAME) **entry =                                  \
            CMC_(PFX, _impl_get_entry_by_key)(_map_, key);                     \
                                                                               \
        if (!entry)                                                            \
        {                                                                      \
            _map_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return (*entry)->value;                                                \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _contains_key)(struct SNAME * _map_, K key)                 \
    {                                                                          \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        bool result = CMC_(PFX, _impl_get_entry_by_key)(_map_, key) != NULL;   \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _contains_val)(struct SNAME * _map_, V val)                 \
    {                                                                          \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        bool result = CMC_(PFX, _impl_get_entry_by_val)(_map_, val) != NULL;   \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _empty)(struct SNAME * _map_)                               \
    {                                                                          \
        return _map_->count == 0;                                              \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _full)(struct SNAME * _map_)                                \
    {                                                                          \
        return (double)_map_->capacity * _map_->load <= (double)_map_->count;  \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _count)(struct SNAME * _map_)                             \
    {                                                                          \
        return _map_->count;                                                   \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _capacity)(struct SNAME * _map_)                          \
    {                                                                          \
        return _map_->capacity;                                                \
    }                                                                          \
                                                                               \
    double CMC_(PFX, _load)(struct SNAME * _map_)                              \
    {                                                                          \
        return _map_->load;                                                    \
    }                                                                          \
                                                                               \
    int CMC_(PFX, _flag)(struct SNAME * _map_)                                 \
    {                                                                          \
        return _map_->flag;                                                    \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _resize)(struct SNAME * _map_, size_t capacity)             \
    {                                                                          \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->capacity == capacity)                                       \
            goto success;                                                      \
                                                                               \
        if (_map_->capacity > capacity / _map_->load)                          \
            goto success;                                                      \
                                                                               \
        /* Prevent integer overflow */                                         \
        if (capacity >= UINTMAX_MAX * _map_->load)                             \
        {                                                                      \
            _map_->flag = CMC_FLAG_ERROR;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* Calculate required capacity based on the prime numbers */           \
        size_t new_cap = CMC_(PFX, _impl_calculate_size)(capacity);            \
                                                                               \
        /* Not possible to shrink with current available prime numbers */      \
        if (new_cap < _map_->count / _map_->load)                              \
        {                                                                      \
            _map_->flag = CMC_FLAG_INVALID;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* No callbacks since _new_map_ is just a temporary hashtable */       \
        struct SNAME *_new_map_ =                                              \
            CMC_(PFX, _new_custom)(capacity, _map_->load, _map_->f_key,        \
                                   _map_->f_val, _map_->alloc, NULL);          \
                                                                               \
        if (!_new_map_)                                                        \
        {                                                                      \
            _map_->flag = CMC_FLAG_ALLOC;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        for (size_t i = 0; i < _map_->capacity; i++)                           \
        {                                                                      \
            struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0];           \
                                                                               \
            if (scan && scan != CMC_ENTRY_DELETED)                             \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) **e1 =                             \
                    CMC_(PFX, _impl_add_entry_to_key)(_new_map_, scan);        \
                struct CMC_DEF_ENTRY(SNAME) **e2 =                             \
                    CMC_(PFX, _impl_add_entry_to_val)(_new_map_, scan);        \
                                                                               \
                if (!e1 || !e2)                                                \
                {                                                              \
                    /* Prevent the map from freeing the data */                \
                    _new_map_->f_key = &(struct CMC_DEF_FKEY(SNAME)){ NULL };  \
                    _new_map_->f_val = &(struct CMC_DEF_FVAL(SNAME)){ NULL };  \
                                                                               \
                    _map_->alloc->free(_new_map_->buffer);                     \
                    _map_->alloc->free(_new_map_);                             \
                                                                               \
                    _map_->flag = CMC_FLAG_ERROR;                              \
                                                                               \
                    return false;                                              \
                }                                                              \
                                                                               \
                _new_map_->count++;                                            \
            }                                                                  \
        }                                                                      \
                                                                               \
        if (_map_->count != _new_map_->count)                                  \
        {                                                                      \
            /* Prevent the map from freeing the data */                        \
            _new_map_->f_key = &(struct CMC_DEF_FKEY(SNAME)){ NULL };          \
            _new_map_->f_val = &(struct CMC_DEF_FVAL(SNAME)){ NULL };          \
                                                                               \
            _map_->alloc->free(_new_map_->buffer);                             \
            _map_->alloc->free(_new_map_);                                     \
                                                                               \
            _map_->flag = CMC_FLAG_ERROR;                                      \
                                                                               \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) * (*tmp_buff)[2] = _map_->buffer;          \
                                                                               \
        _map_->buffer = _new_map_->buffer;                                     \
        _map_->capacity = _new_map_->capacity;                                 \
                                                                               \
        _map_->alloc->free(tmp_buff);                                          \
        _map_->alloc->free(_new_map_);                                         \
                                                                               \
    success:                                                                   \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->resize)                      \
            _map_->callbacks->resize();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _map_)                    \
    {                                                                          \
        /* TODO this function can be optimized */                              \
        struct SNAME *result = CMC_(PFX, _new_custom)(                         \
            _map_->capacity * _map_->load, _map_->load, _map_->f_key,          \
            _map_->f_val, _map_->alloc, _map_->callbacks);                     \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _map_->flag = CMC_FLAG_ERROR;                                      \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        for (size_t i = 0; i < _map_->capacity; i++)                           \
        {                                                                      \
            struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0];           \
                                                                               \
            if (scan && scan != CMC_ENTRY_DELETED)                             \
            {                                                                  \
                K tmp_key;                                                     \
                V tmp_val;                                                     \
                                                                               \
                if (_map_->f_key->cpy)                                         \
                    tmp_key = _map_->f_key->cpy(scan->key);                    \
                else                                                           \
                    tmp_key = scan->key;                                       \
                                                                               \
                if (_map_->f_val->cpy)                                         \
                    tmp_val = _map_->f_val->cpy(scan->value);                  \
                else                                                           \
                    tmp_val = scan->value;                                     \
                                                                               \
                /* TODO treat the possible errors */                           \
                struct CMC_DEF_ENTRY(SNAME) *entry =                           \
                    CMC_(PFX, _impl_new_entry)(result, tmp_key, tmp_val);      \
                                                                               \
                CMC_(PFX, _impl_add_entry_to_key)(result, entry);              \
                CMC_(PFX, _impl_add_entry_to_val)(result, entry);              \
                                                                               \
                result->count++;                                               \
            }                                                                  \
        }                                                                      \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _equals)(struct SNAME * _map1_, struct SNAME * _map2_)      \
    {                                                                          \
        _map1_->flag = CMC_FLAG_OK;                                            \
        _map2_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_map1_->count != _map2_->count)                                    \
            return false;                                                      \
                                                                               \
        struct SNAME *_mapA_;                                                  \
        struct SNAME *_mapB_;                                                  \
                                                                               \
        if (_map1_->capacity < _map2_->capacity)                               \
        {                                                                      \
            _mapA_ = _map1_;                                                   \
            _mapB_ = _map2_;                                                   \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            _mapA_ = _map2_;                                                   \
            _mapB_ = _map1_;                                                   \
        }                                                                      \
                                                                               \
        for (size_t i = 0; i < _mapA_->capacity; i++)                          \
        {                                                                      \
            struct CMC_DEF_ENTRY(SNAME) *scan = _mapA_->buffer[i][0];          \
                                                                               \
            if (scan && scan != CMC_ENTRY_DELETED)                             \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) **entry_B =                        \
                    CMC_(PFX, _impl_get_entry_by_key)(_mapB_, scan->key);      \
                                                                               \
                if (!entry_B)                                                  \
                    return false;                                              \
                                                                               \
                if (_mapA_->f_val->cmp((*entry_B)->value, scan->value) != 0)   \
                    return false;                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _map_)              \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *m_ = _map_;                                              \
                                                                               \
        int n = snprintf(                                                      \
            str.s, cmc_string_len, cmc_cmc_string_fmt_hashbidimap, #SNAME, #K, \
            #V, m_, m_->buffer, m_->capacity, m_->count, m_->load, m_->flag,   \
            m_->f_key, m_->f_val, m_->alloc, m_->callbacks);                   \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr)                  \
    {                                                                          \
        for (size_t i = 0; i < _map_->capacity; i++)                           \
        {                                                                      \
            struct CMC_DEF_ENTRY(SNAME) *target = _map_->buffer[i][0];         \
                                                                               \
            if (target && target != CMC_ENTRY_DELETED)                         \
            {                                                                  \
                if (!_map_->f_key->str(fptr, target->key) ||                   \
                    !_map_->f_val->str(fptr, target->value))                   \
                    return false;                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_ITER(SNAME) * CMC_(PFX, _iter_new)(struct SNAME * target)   \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) *iter =                                     \
            target->alloc->malloc(sizeof(struct CMC_DEF_ITER(SNAME)));         \
                                                                               \
        if (!iter)                                                             \
            return NULL;                                                       \
                                                                               \
        CMC_(PFX, _iter_init)(iter, target);                                   \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _iter_free)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                          \
        iter->target->alloc->free(iter);                                       \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _iter_init)(struct CMC_DEF_ITER(SNAME) * iter,              \
                               struct SNAME * target)                          \
    {                                                                          \
        memset(iter, 0, sizeof(struct CMC_DEF_ITER(SNAME)));                   \
                                                                               \
        iter->target = target;                                                 \
        iter->start = true;                                                    \
        iter->end = CMC_(PFX, _empty)(target);                                 \
                                                                               \
        if (!CMC_(PFX, _empty)(target))                                        \
        {                                                                      \
            for (size_t i = 0; i < target->capacity; i++)                      \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) *tmp = target->buffer[i][0];       \
                                                                               \
                if (tmp != NULL && tmp != CMC_ENTRY_DELETED)                   \
                {                                                              \
                    iter->first = i;                                           \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            iter->cursor = iter->first;                                        \
                                                                               \
            for (size_t i = target->capacity; i > 0; i--)                      \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) *tmp = target->buffer[i - 1][0];   \
                                                                               \
                if (tmp != NULL && tmp != CMC_ENTRY_DELETED)                   \
                {                                                              \
                    iter->last = i - 1;                                        \
                    break;                                                     \
                }                                                              \
            }                                                                  \
        }                                                                      \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_start)(struct CMC_DEF_ITER(SNAME) * iter)             \
    {                                                                          \
        return CMC_(PFX, _empty)(iter->target) || iter->start;                 \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_end)(struct CMC_DEF_ITER(SNAME) * iter)               \
    {                                                                          \
        return CMC_(PFX, _empty)(iter->target) || iter->end;                   \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                          \
        if (!CMC_(PFX, _empty)(iter->target))                                  \
        {                                                                      \
            iter->cursor = iter->first;                                        \
            iter->index = 0;                                                   \
            iter->start = true;                                                \
            iter->end = false;                                                 \
        }                                                                      \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                          \
        if (!CMC_(PFX, _empty)(iter->target))                                  \
        {                                                                      \
            iter->cursor = iter->last;                                         \
            iter->index = iter->target->count - 1;                             \
            iter->start = false;                                               \
            iter->end = true;                                                  \
        }                                                                      \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)              \
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
        iter->start = CMC_(PFX, _empty)(iter->target);                         \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) *scan =                                    \
            iter->target->buffer[iter->cursor][0];                             \
                                                                               \
        iter->index++;                                                         \
                                                                               \
        while (1)                                                              \
        {                                                                      \
            iter->cursor++;                                                    \
            scan = iter->target->buffer[iter->cursor][0];                      \
                                                                               \
            if (scan != NULL && scan != CMC_ENTRY_DELETED)                     \
                break;                                                         \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)              \
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
        iter->end = CMC_(PFX, _empty)(iter->target);                           \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) *scan =                                    \
            iter->target->buffer[iter->cursor][0];                             \
                                                                               \
        iter->index--;                                                         \
                                                                               \
        while (1)                                                              \
        {                                                                      \
            iter->cursor--;                                                    \
            scan = iter->target->buffer[iter->cursor][0];                      \
                                                                               \
            if (scan != NULL && scan != CMC_ENTRY_DELETED)                     \
                break;                                                         \
        }                                                                      \
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
    /* Returns true only if the iterator was able to be positioned at */       \
    /* the given index */                                                      \
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
        {                                                                      \
            iter->target->flag = CMC_FLAG_EMPTY;                               \
            return (K){ 0 };                                                   \
        }                                                                      \
                                                                               \
        return iter->target->buffer[iter->cursor][0]->key;                     \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)                \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
        {                                                                      \
            iter->target->flag = CMC_FLAG_EMPTY;                               \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        return iter->target->buffer[iter->cursor][0]->value;                   \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)           \
    {                                                                          \
        return iter->index;                                                    \
    }                                                                          \
                                                                               \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        CMC_(PFX, _impl_new_entry)(struct SNAME * _map_, K key, V value)       \
    {                                                                          \
        struct CMC_DEF_ENTRY(SNAME) *entry =                                   \
            _map_->alloc->malloc(sizeof(struct CMC_DEF_ENTRY(SNAME)));         \
                                                                               \
        if (!entry)                                                            \
            return NULL;                                                       \
                                                                               \
        entry->key = key;                                                      \
        entry->value = value;                                                  \
        entry->dist[0] = 0;                                                    \
        entry->dist[1] = 0;                                                    \
        entry->ref[0] = NULL;                                                  \
        entry->ref[1] = NULL;                                                  \
                                                                               \
        return entry;                                                          \
    }                                                                          \
                                                                               \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        *CMC_(PFX, _impl_get_entry_by_key)(struct SNAME * _map_, K key)        \
    {                                                                          \
        size_t hash = _map_->f_key->hash(key);                                 \
        size_t pos = hash % _map_->capacity;                                   \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) *target = _map_->buffer[pos][0];           \
                                                                               \
        while (target != NULL)                                                 \
        {                                                                      \
            if (target != CMC_ENTRY_DELETED &&                                 \
                _map_->f_key->cmp(target->key, key) == 0)                      \
                return &(_map_->buffer[pos % _map_->capacity][0]);             \
                                                                               \
            pos++;                                                             \
            target = _map_->buffer[pos % _map_->capacity][0];                  \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        *CMC_(PFX, _impl_get_entry_by_val)(struct SNAME * _map_, V val)        \
    {                                                                          \
        size_t hash = _map_->f_val->hash(val);                                 \
        size_t pos = hash % _map_->capacity;                                   \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) *target = _map_->buffer[pos][1];           \
                                                                               \
        while (target != NULL)                                                 \
        {                                                                      \
            if (target != CMC_ENTRY_DELETED &&                                 \
                _map_->f_val->cmp(target->value, val) == 0)                    \
                return &(_map_->buffer[pos % _map_->capacity][1]);             \
                                                                               \
            pos++;                                                             \
            target = _map_->buffer[pos % _map_->capacity][1];                  \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        *CMC_(PFX, _impl_add_entry_to_key)(                                    \
            struct SNAME * _map_, struct CMC_DEF_ENTRY(SNAME) * entry)         \
    {                                                                          \
        struct CMC_DEF_ENTRY(SNAME) **to_return = NULL;                        \
                                                                               \
        size_t hash = _map_->f_key->hash(entry->key);                          \
        size_t original_pos = hash % _map_->capacity;                          \
        size_t pos = original_pos;                                             \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) **scan =                                   \
            &(_map_->buffer[hash % _map_->capacity][0]);                       \
                                                                               \
        if (*scan == NULL)                                                     \
        {                                                                      \
            *scan = entry;                                                     \
                                                                               \
            entry->ref[0] = scan;                                              \
                                                                               \
            return scan;                                                       \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            while (true)                                                       \
            {                                                                  \
                pos++;                                                         \
                scan = &(_map_->buffer[pos % _map_->capacity][0]);             \
                                                                               \
                if (*scan == NULL || *scan == CMC_ENTRY_DELETED)               \
                {                                                              \
                    if (!to_return)                                            \
                        to_return = scan;                                      \
                                                                               \
                    *scan = entry;                                             \
                    entry->ref[0] = scan;                                      \
                    entry->dist[0] = pos - original_pos;                       \
                                                                               \
                    return to_return;                                          \
                }                                                              \
                else if ((*scan)->dist[0] < pos - original_pos)                \
                {                                                              \
                    if (!to_return)                                            \
                        to_return = scan;                                      \
                                                                               \
                    size_t tmp_dist = (*scan)->dist[0];                        \
                    entry->dist[0] = pos - original_pos;                       \
                    original_pos = pos - tmp_dist;                             \
                                                                               \
                    entry->ref[0] = scan;                                      \
                                                                               \
                    struct CMC_DEF_ENTRY(SNAME) *_tmp_ = *scan;                \
                    *scan = entry;                                             \
                    entry = _tmp_;                                             \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        *CMC_(PFX, _impl_add_entry_to_val)(                                    \
            struct SNAME * _map_, struct CMC_DEF_ENTRY(SNAME) * entry)         \
    {                                                                          \
        struct CMC_DEF_ENTRY(SNAME) **to_return = NULL;                        \
                                                                               \
        size_t hash = _map_->f_val->hash(entry->value);                        \
        size_t original_pos = hash % _map_->capacity;                          \
        size_t pos = original_pos;                                             \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) **scan =                                   \
            &(_map_->buffer[hash % _map_->capacity][1]);                       \
                                                                               \
        if (*scan == NULL)                                                     \
        {                                                                      \
            *scan = entry;                                                     \
                                                                               \
            entry->ref[1] = scan;                                              \
                                                                               \
            return scan;                                                       \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            while (true)                                                       \
            {                                                                  \
                pos++;                                                         \
                scan = &(_map_->buffer[pos % _map_->capacity][1]);             \
                                                                               \
                if (*scan == NULL || *scan == CMC_ENTRY_DELETED)               \
                {                                                              \
                    if (!to_return)                                            \
                        to_return = scan;                                      \
                                                                               \
                    *scan = entry;                                             \
                    entry->ref[1] = scan;                                      \
                    entry->dist[1] = pos - original_pos;                       \
                                                                               \
                    return to_return;                                          \
                }                                                              \
                else if ((*scan)->dist[1] < pos - original_pos)                \
                {                                                              \
                    if (!to_return)                                            \
                        to_return = scan;                                      \
                                                                               \
                    size_t tmp_dist = (*scan)->dist[1];                        \
                    entry->dist[1] = pos - original_pos;                       \
                    original_pos = pos - tmp_dist;                             \
                                                                               \
                    entry->ref[1] = scan;                                      \
                                                                               \
                    struct CMC_DEF_ENTRY(SNAME) *_tmp_ = *scan;                \
                    *scan = entry;                                             \
                    entry = _tmp_;                                             \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    static size_t CMC_(PFX, _impl_calculate_size)(size_t required)             \
    {                                                                          \
        const size_t count =                                                   \
            sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);    \
                                                                               \
        if (cmc_hashtable_primes[count - 1] < required)                        \
            return required;                                                   \
                                                                               \
        size_t i = 0;                                                          \
        while (cmc_hashtable_primes[i] < required)                             \
            i++;                                                               \
                                                                               \
        return cmc_hashtable_primes[i];                                        \
    }                                                                          \
                                                                               \
    static struct CMC_DEF_ITER(SNAME)                                          \
        CMC_(PFX, _impl_it_start)(struct SNAME * _map_)                        \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        CMC_(PFX, _iter_init)(&iter, _map_);                                   \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    static struct CMC_DEF_ITER(SNAME)                                          \
        CMC_(PFX, _impl_it_end)(struct SNAME * _map_)                          \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        CMC_(PFX, _iter_init)(&iter, _map_);                                   \
        CMC_(PFX, _iter_to_end)(&iter);                                        \
                                                                               \
        return iter;                                                           \
    }

#endif /* CMC_CMC_HASHBIDIMAP_H */
