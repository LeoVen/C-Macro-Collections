/**
 * bidimap.h
 *
 * Creation Date: 26/09/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * BidiMap
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

#ifndef CMC_BIDIMAP_H
#define CMC_BIDIMAP_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * Hashtable Implementation
 * ------------------------------------------------------------------------- */
#include "../cor/hashtable.h"

/* -------------------------------------------------------------------------
 * Bidimap Specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_string_fmt_bidimap = "struct %s<%s, %s> "
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

#define CMC_GENERATE_BIDIMAP(PFX, SNAME, K, V)    \
    CMC_GENERATE_BIDIMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_BIDIMAP_SOURCE(PFX, SNAME, K, V)

#define CMC_WRAPGEN_BIDIMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_BIDIMAP_HEADER(PFX, SNAME, K, V)

#define CMC_WRAPGEN_BIDIMAP_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_BIDIMAP_SOURCE(PFX, SNAME, K, V)

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_BIDIMAP_HEADER(PFX, SNAME, K, V)                        \
                                                                             \
    /* BidiMap Structure */                                                  \
    struct SNAME                                                             \
    {                                                                        \
        /* Array 0 is K -> V and array 1 is V -> K */                        \
        struct SNAME##_entry *(*buffer)[2];                                  \
                                                                             \
        /* Current arrays capacity */                                        \
        size_t capacity;                                                     \
                                                                             \
        /* Current amount of keys */                                         \
        size_t count;                                                        \
                                                                             \
        /* Load factor in range (0.0, 1.0) */                                \
        double load;                                                         \
                                                                             \
        /* Flags indicating errors or success */                             \
        int flag;                                                            \
                                                                             \
        /* Key function table */                                             \
        struct SNAME##_fkey *f_key;                                          \
                                                                             \
        /* Value function table */                                           \
        struct SNAME##_fval *f_val;                                          \
                                                                             \
        /* Custom allocation functions */                                    \
        struct cmc_alloc_node *alloc;                                        \
                                                                             \
        /* Custom callback functions */                                      \
        struct cmc_callbacks *callbacks;                                     \
                                                                             \
        /* Methods */                                                        \
        /* Function that returns an iterator to the start of the bidimap */  \
        struct SNAME##_iter (*it_start)(struct SNAME *);                     \
                                                                             \
        /* Function that returns an iterator to the end of the bidimap */    \
        struct SNAME##_iter (*it_end)(struct SNAME *);                       \
    };                                                                       \
                                                                             \
    /* BidiMap Entry */                                                      \
    struct SNAME##_entry                                                     \
    {                                                                        \
        /* Entry Key */                                                      \
        K key;                                                               \
                                                                             \
        /* Entry Value */                                                    \
        V value;                                                             \
                                                                             \
        /* The distance of this node to its original position */             \
        /* dist[0] is relative to K -> V */                                  \
        /* dist[1] is relative to V -> K */                                  \
        size_t dist[2];                                                      \
                                                                             \
        /* References to this node in the bidimap buffer. Used to */         \
        /* prevent searching for this node twice for update() and */         \
        /* remove(). Increases memory overhead but reduces execution time */ \
        /* ref[0] is relative to K -> V */                                   \
        /* ref[1] is relative to V -> K */                                   \
        struct SNAME##_entry **ref[2];                                       \
    };                                                                       \
                                                                             \
    /* Key struct function table */                                          \
    struct SNAME##_fkey                                                      \
    {                                                                        \
        /* Comparator function */                                            \
        int (*cmp)(K, K);                                                    \
                                                                             \
        /* Copy function */                                                  \
        K (*cpy)(K);                                                         \
                                                                             \
        /* To string function */                                             \
        bool (*str)(FILE *, K);                                              \
                                                                             \
        /* Free from memory function */                                      \
        void (*free)(K);                                                     \
                                                                             \
        /* Hash function */                                                  \
        size_t (*hash)(K);                                                   \
                                                                             \
        /* Priority function */                                              \
        int (*pri)(K, K);                                                    \
    };                                                                       \
                                                                             \
    /* Value struct function table */                                        \
    struct SNAME##_fval                                                      \
    {                                                                        \
        /* Comparator function */                                            \
        int (*cmp)(V, V);                                                    \
                                                                             \
        /* Copy function */                                                  \
        V (*cpy)(V);                                                         \
                                                                             \
        /* To string function */                                             \
        bool (*str)(FILE *, V);                                              \
                                                                             \
        /* Free from memory function */                                      \
        void (*free)(V);                                                     \
                                                                             \
        /* Hash function */                                                  \
        size_t (*hash)(V);                                                   \
                                                                             \
        /* Priority function */                                              \
        int (*pri)(V, V);                                                    \
    };                                                                       \
                                                                             \
    /* BidiMap Iterator */                                                   \
    struct SNAME##_iter                                                      \
    {                                                                        \
        /* Target bidimap */                                                 \
        struct SNAME *target;                                                \
                                                                             \
        /* Cursor's position (index) */                                      \
        size_t cursor;                                                       \
                                                                             \
        /* Keeps track of relative index to the iteration of elements */     \
        size_t index;                                                        \
                                                                             \
        /* The index of the first element */                                 \
        size_t first;                                                        \
                                                                             \
        /* The index of the last element */                                  \
        size_t last;                                                         \
                                                                             \
        /* If the iterator has reached the start of the iteration */         \
        bool start;                                                          \
                                                                             \
        /* If the iterator has reached the end of the iteration */           \
        bool end;                                                            \
    };                                                                       \
                                                                             \
    /* Collection Functions */                                               \
    /* Collection Allocation and Deallocation */                             \
    struct SNAME *PFX##_new(size_t capacity, double load,                    \
                            struct SNAME##_fkey *f_key,                      \
                            struct SNAME##_fval *f_val);                     \
    struct SNAME *PFX##_new_custom(                                          \
        size_t capacity, double load, struct SNAME##_fkey *f_key,            \
        struct SNAME##_fval *f_val, struct cmc_alloc_node *alloc,            \
        struct cmc_callbacks *callbacks);                                    \
    void PFX##_clear(struct SNAME *_map_);                                   \
    void PFX##_free(struct SNAME *_map_);                                    \
    /* Customization of Allocation and Callbacks */                          \
    void PFX##_customize(struct SNAME *_map_, struct cmc_alloc_node *alloc,  \
                         struct cmc_callbacks *callbacks);                   \
    /* Collection Input and Output */                                        \
    bool PFX##_insert(struct SNAME *_map_, K key, V value);                  \
    bool PFX##_update_key(struct SNAME *_map_, V val, K new_key);            \
    bool PFX##_update_val(struct SNAME *_map_, K key, V new_val);            \
    bool PFX##_remove_by_key(struct SNAME *_map_, K key, K *out_key,         \
                             V *out_val);                                    \
    bool PFX##_remove_by_val(struct SNAME *_map_, V val, K *out_key,         \
                             V *out_val);                                    \
    /* Element Access */                                                     \
    K PFX##_get_key(struct SNAME *_map_, V val);                             \
    V PFX##_get_val(struct SNAME *_map_, K key);                             \
    /* Collection State */                                                   \
    bool PFX##_contains_key(struct SNAME *_map_, K key);                     \
    bool PFX##_contains_val(struct SNAME *_map_, V val);                     \
    bool PFX##_empty(struct SNAME *_map_);                                   \
    bool PFX##_full(struct SNAME *_map_);                                    \
    size_t PFX##_count(struct SNAME *_map_);                                 \
    size_t PFX##_capacity(struct SNAME *_map_);                              \
    double PFX##_load(struct SNAME *_map_);                                  \
    int PFX##_flag(struct SNAME *_map_);                                     \
    /* Collection Utility */                                                 \
    bool PFX##_resize(struct SNAME *_map_, size_t capacity);                 \
    struct SNAME *PFX##_copy_of(struct SNAME *_map_);                        \
    bool PFX##_equals(struct SNAME *_map1_, struct SNAME *_map2_);           \
    struct cmc_string PFX##_to_string(struct SNAME *_map_);                  \
    bool PFX##_print(struct SNAME *_map_, FILE *fptr);                       \
                                                                             \
    /* Iterator Functions */                                                 \
    /* Iterator Allocation and Deallocation */                               \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);               \
    void PFX##_iter_free(struct SNAME##_iter *iter);                         \
    /* Iterator Initialization */                                            \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);   \
    /* Iterator State */                                                     \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                        \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                          \
    /* Iterator Movement */                                                  \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                     \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                       \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                         \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                         \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);        \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);         \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);          \
    /* Iterator Access */                                                    \
    K PFX##_iter_key(struct SNAME##_iter *iter);                             \
    V PFX##_iter_value(struct SNAME##_iter *iter);                           \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_BIDIMAP_SOURCE(PFX, SNAME, K, V)                          \
                                                                               \
    /* Implementation Detail Functions */                                      \
    static struct SNAME##_entry *PFX##_impl_new_entry(struct SNAME *_map_,     \
                                                      K key, V value);         \
    static struct SNAME##_entry **PFX##_impl_get_entry_by_key(                 \
        struct SNAME *_map_, K key);                                           \
    static struct SNAME##_entry **PFX##_impl_get_entry_by_val(                 \
        struct SNAME *_map_, V val);                                           \
    static struct SNAME##_entry **PFX##_impl_add_entry_to_key(                 \
        struct SNAME *_map_, struct SNAME##_entry *entry);                     \
    static struct SNAME##_entry **PFX##_impl_add_entry_to_val(                 \
        struct SNAME *_map_, struct SNAME##_entry *entry);                     \
    static size_t PFX##_impl_calculate_size(size_t required);                  \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_map_);       \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_map_);         \
                                                                               \
    struct SNAME *PFX##_new(size_t capacity, double load,                      \
                            struct SNAME##_fkey *f_key,                        \
                            struct SNAME##_fval *f_val)                        \
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
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);     \
                                                                               \
        struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_map_)                                                            \
            return NULL;                                                       \
                                                                               \
        _map_->buffer =                                                        \
            alloc->calloc(real_capacity, sizeof(struct SNAME##_entry *[2]));   \
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
        _map_->flag = cmc_flags.OK;                                            \
        _map_->f_key = f_key;                                                  \
        _map_->f_val = f_val;                                                  \
        _map_->alloc = alloc;                                                  \
        _map_->callbacks = NULL;                                               \
        _map_->it_start = PFX##_impl_it_start;                                 \
        _map_->it_end = PFX##_impl_it_end;                                     \
                                                                               \
        return _map_;                                                          \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_new_custom(                                            \
        size_t capacity, double load, struct SNAME##_fkey *f_key,              \
        struct SNAME##_fval *f_val, struct cmc_alloc_node *alloc,              \
        struct cmc_callbacks *callbacks)                                       \
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
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);     \
                                                                               \
        if (!alloc)                                                            \
            alloc = &cmc_alloc_node_default;                                   \
                                                                               \
        struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_map_)                                                            \
            return NULL;                                                       \
                                                                               \
        _map_->buffer =                                                        \
            alloc->calloc(real_capacity, sizeof(struct SNAME##_entry *[2]));   \
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
        _map_->flag = cmc_flags.OK;                                            \
        _map_->f_key = f_key;                                                  \
        _map_->f_val = f_val;                                                  \
        _map_->flag = cmc_flags.OK;                                            \
        _map_->alloc = alloc;                                                  \
        _map_->callbacks = callbacks;                                          \
        _map_->it_start = PFX##_impl_it_start;                                 \
        _map_->it_end = PFX##_impl_it_end;                                     \
                                                                               \
        return _map_;                                                          \
    }                                                                          \
                                                                               \
    void PFX##_clear(struct SNAME *_map_)                                      \
    {                                                                          \
        for (size_t i = 0; i < _map_->capacity; i++)                           \
        {                                                                      \
            struct SNAME##_entry *entry = _map_->buffer[i][0];                 \
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
        _map_->flag = cmc_flags.OK;                                            \
    }                                                                          \
                                                                               \
    void PFX##_free(struct SNAME *_map_)                                       \
    {                                                                          \
        PFX##_clear(_map_);                                                    \
                                                                               \
        _map_->alloc->free(_map_->buffer);                                     \
        _map_->alloc->free(_map_);                                             \
    }                                                                          \
                                                                               \
    void PFX##_customize(struct SNAME *_map_, struct cmc_alloc_node *alloc,    \
                         struct cmc_callbacks *callbacks)                      \
    {                                                                          \
        if (!alloc)                                                            \
            _map_->alloc = &cmc_alloc_node_default;                            \
        else                                                                   \
            _map_->alloc = alloc;                                              \
                                                                               \
        _map_->callbacks = callbacks;                                          \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
    }                                                                          \
                                                                               \
    bool PFX##_insert(struct SNAME *_map_, K key, V value)                     \
    {                                                                          \
        if (PFX##_full(_map_))                                                 \
        {                                                                      \
            if (!PFX##_resize(_map_, _map_->capacity + 1))                     \
                return false;                                                  \
        }                                                                      \
                                                                               \
        if (PFX##_impl_get_entry_by_key(_map_, key) != NULL ||                 \
            PFX##_impl_get_entry_by_val(_map_, value) != NULL)                 \
        {                                                                      \
            _map_->flag = cmc_flags.DUPLICATE;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry *entry = PFX##_impl_new_entry(_map_, key, value); \
                                                                               \
        struct SNAME##_entry **key_entry =                                     \
            PFX##_impl_add_entry_to_key(_map_, entry);                         \
        struct SNAME##_entry **val_entry =                                     \
            PFX##_impl_add_entry_to_val(_map_, entry);                         \
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
            _map_->flag = cmc_flags.ERROR;                                     \
                                                                               \
            return false;                                                      \
        }                                                                      \
                                                                               \
        _map_->count++;                                                        \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->create)                      \
            _map_->callbacks->create();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_update_key(struct SNAME *_map_, V val, K new_key)               \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry **key_entry, **val_entry;                         \
                                                                               \
        val_entry = PFX##_impl_get_entry_by_val(_map_, val);                   \
                                                                               \
        if (!val_entry)                                                        \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* The mapping val -> new_key is already true */                       \
        if (_map_->f_key->cmp(new_key, (*val_entry)->key) == 0)                \
            goto success;                                                      \
                                                                               \
        if (PFX##_impl_get_entry_by_key(_map_, new_key) != NULL)               \
        {                                                                      \
            _map_->flag = cmc_flags.DUPLICATE;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        key_entry = (*val_entry)->ref[0];                                      \
                                                                               \
        if (!key_entry || *key_entry != *val_entry)                            \
        {                                                                      \
            /* Should never happen */                                          \
            _map_->flag = cmc_flags.ERROR;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* Remove entry from key buffer and add it again with new key */       \
        struct SNAME##_entry *to_add = *key_entry;                             \
        K tmp_key = to_add->key;                                               \
        to_add->key = new_key;                                                 \
                                                                               \
        *key_entry = CMC_ENTRY_DELETED;                                        \
                                                                               \
        if (!PFX##_impl_add_entry_to_key(_map_, to_add))                       \
        {                                                                      \
            /* Revert changes */                                               \
            to_add->key = tmp_key;                                             \
            *key_entry = to_add;                                               \
                                                                               \
            _map_->flag = cmc_flags.ERROR;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
    success:                                                                   \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->update)                      \
            _map_->callbacks->update();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_update_val(struct SNAME *_map_, K key, V new_val)               \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry **key_entry, **val_entry;                         \
                                                                               \
        key_entry = PFX##_impl_get_entry_by_key(_map_, key);                   \
                                                                               \
        if (!key_entry)                                                        \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* The mapping key -> new_val is already true */                       \
        if (_map_->f_key->cmp(new_val, (*key_entry)->value) == 0)              \
            goto success;                                                      \
                                                                               \
        if (PFX##_impl_get_entry_by_val(_map_, new_val) != NULL)               \
        {                                                                      \
            _map_->flag = cmc_flags.DUPLICATE;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        val_entry = (*key_entry)->ref[1];                                      \
                                                                               \
        if (!val_entry || *val_entry != *key_entry)                            \
        {                                                                      \
            /* Should never happen */                                          \
            _map_->flag = cmc_flags.ERROR;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* Remove entry from value buffer and add it again with new value */   \
        struct SNAME##_entry *to_add = *val_entry;                             \
        V tmp_val = to_add->value;                                             \
        to_add->value = new_val;                                               \
                                                                               \
        *val_entry = CMC_ENTRY_DELETED;                                        \
                                                                               \
        if (!PFX##_impl_add_entry_to_val(_map_, to_add))                       \
        {                                                                      \
            /* Revert changes */                                               \
            to_add->value = tmp_val;                                           \
            *val_entry = to_add;                                               \
                                                                               \
            _map_->flag = cmc_flags.ERROR;                                     \
                                                                               \
            return false;                                                      \
        }                                                                      \
                                                                               \
    success:                                                                   \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->update)                      \
            _map_->callbacks->update();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_remove_by_key(struct SNAME *_map_, K key, K *out_key,           \
                             V *out_val)                                       \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry **key_entry, **val_entry;                         \
                                                                               \
        key_entry = PFX##_impl_get_entry_by_key(_map_, key);                   \
                                                                               \
        if (!key_entry)                                                        \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        val_entry = (*key_entry)->ref[1];                                      \
                                                                               \
        if (!val_entry || *val_entry != *key_entry)                            \
        {                                                                      \
            /* Should never happen */                                          \
            _map_->flag = cmc_flags.ERROR;                                     \
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
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->delete)                      \
            _map_->callbacks->delete ();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_remove_by_val(struct SNAME *_map_, V val, K *out_key,           \
                             V *out_val)                                       \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry **key_entry, **val_entry;                         \
                                                                               \
        val_entry = PFX##_impl_get_entry_by_val(_map_, val);                   \
                                                                               \
        if (!val_entry)                                                        \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        key_entry = (*val_entry)->ref[0];                                      \
                                                                               \
        if (!key_entry || *key_entry != *val_entry)                            \
        {                                                                      \
            /* Should never happen */                                          \
            _map_->flag = cmc_flags.ERROR;                                     \
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
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->delete)                      \
            _map_->callbacks->delete ();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    K PFX##_get_key(struct SNAME *_map_, V val)                                \
    {                                                                          \
        struct SNAME##_entry **entry =                                         \
            PFX##_impl_get_entry_by_val(_map_, val);                           \
                                                                               \
        if (!entry)                                                            \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return (*entry)->key;                                                  \
    }                                                                          \
                                                                               \
    V PFX##_get_val(struct SNAME *_map_, K key)                                \
    {                                                                          \
        struct SNAME##_entry **entry =                                         \
            PFX##_impl_get_entry_by_key(_map_, key);                           \
                                                                               \
        if (!entry)                                                            \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return (*entry)->value;                                                \
    }                                                                          \
                                                                               \
    bool PFX##_contains_key(struct SNAME *_map_, K key)                        \
    {                                                                          \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        bool result = PFX##_impl_get_entry_by_key(_map_, key) != NULL;         \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_contains_val(struct SNAME *_map_, V val)                        \
    {                                                                          \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        bool result = PFX##_impl_get_entry_by_val(_map_, val) != NULL;         \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_empty(struct SNAME *_map_)                                      \
    {                                                                          \
        return _map_->count == 0;                                              \
    }                                                                          \
                                                                               \
    bool PFX##_full(struct SNAME *_map_)                                       \
    {                                                                          \
        return (double)_map_->capacity * _map_->load <= (double)_map_->count;  \
    }                                                                          \
                                                                               \
    size_t PFX##_count(struct SNAME *_map_)                                    \
    {                                                                          \
        return _map_->count;                                                   \
    }                                                                          \
                                                                               \
    size_t PFX##_capacity(struct SNAME *_map_)                                 \
    {                                                                          \
        return _map_->capacity;                                                \
    }                                                                          \
                                                                               \
    double PFX##_load(struct SNAME *_map_)                                     \
    {                                                                          \
        return _map_->load;                                                    \
    }                                                                          \
                                                                               \
    int PFX##_flag(struct SNAME *_map_)                                        \
    {                                                                          \
        return _map_->flag;                                                    \
    }                                                                          \
                                                                               \
    bool PFX##_resize(struct SNAME *_map_, size_t capacity)                    \
    {                                                                          \
        _map_->flag = cmc_flags.OK;                                            \
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
            _map_->flag = cmc_flags.ERROR;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* Calculate required capacity based on the prime numbers */           \
        size_t new_cap = PFX##_impl_calculate_size(capacity);                  \
                                                                               \
        /* Not possible to shrink with current available prime numbers */      \
        if (new_cap < _map_->count / _map_->load)                              \
        {                                                                      \
            _map_->flag = cmc_flags.INVALID;                                   \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* No callbacks since _new_map_ is just a temporary hashtable */       \
        struct SNAME *_new_map_ =                                              \
            PFX##_new_custom(capacity, _map_->load, _map_->f_key,              \
                             _map_->f_val, _map_->alloc, NULL);                \
                                                                               \
        if (!_new_map_)                                                        \
        {                                                                      \
            _map_->flag = cmc_flags.ALLOC;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        for (size_t i = 0; i < _map_->capacity; i++)                           \
        {                                                                      \
            struct SNAME##_entry *scan = _map_->buffer[i][0];                  \
                                                                               \
            if (scan && scan != CMC_ENTRY_DELETED)                             \
            {                                                                  \
                struct SNAME##_entry **e1 =                                    \
                    PFX##_impl_add_entry_to_key(_new_map_, scan);              \
                struct SNAME##_entry **e2 =                                    \
                    PFX##_impl_add_entry_to_val(_new_map_, scan);              \
                                                                               \
                if (!e1 || !e2)                                                \
                {                                                              \
                    _map_->alloc->free(_new_map_->buffer);                     \
                    _map_->alloc->free(_new_map_);                             \
                                                                               \
                    _map_->flag = cmc_flags.ERROR;                             \
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
            _map_->alloc->free(_new_map_->buffer);                             \
            _map_->alloc->free(_new_map_);                                     \
                                                                               \
            _map_->flag = cmc_flags.ERROR;                                     \
                                                                               \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry *(*tmp_buff)[2] = _map_->buffer;                  \
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
    struct SNAME *PFX##_copy_of(struct SNAME *_map_)                           \
    {                                                                          \
        /* TODO this function can be optimized */                              \
        struct SNAME *result = PFX##_new_custom(                               \
            _map_->capacity * _map_->load, _map_->load, _map_->f_key,          \
            _map_->f_val, _map_->alloc, _map_->callbacks);                     \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _map_->flag = cmc_flags.ERROR;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        for (size_t i = 0; i < _map_->capacity; i++)                           \
        {                                                                      \
            struct SNAME##_entry *scan = _map_->buffer[i][0];                  \
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
                struct SNAME##_entry *entry =                                  \
                    PFX##_impl_new_entry(result, tmp_key, tmp_val);            \
                                                                               \
                PFX##_impl_add_entry_to_key(result, entry);                    \
                PFX##_impl_add_entry_to_val(result, entry);                    \
                                                                               \
                result->count++;                                               \
            }                                                                  \
        }                                                                      \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_equals(struct SNAME *_map1_, struct SNAME *_map2_)              \
    {                                                                          \
        _map1_->flag = cmc_flags.OK;                                           \
        _map2_->flag = cmc_flags.OK;                                           \
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
            struct SNAME##_entry *scan = _mapA_->buffer[i][0];                 \
                                                                               \
            if (scan && scan != CMC_ENTRY_DELETED)                             \
            {                                                                  \
                struct SNAME##_entry **entry_B =                               \
                    PFX##_impl_get_entry_by_key(_mapB_, scan->key);            \
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
    struct cmc_string PFX##_to_string(struct SNAME *_map_)                     \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *m_ = _map_;                                              \
                                                                               \
        int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_bidimap,        \
                         #SNAME, #K, #V, m_, m_->buffer, m_->capacity,         \
                         m_->count, m_->load, m_->flag, m_->f_key, m_->f_val,  \
                         m_->alloc, m_->callbacks);                            \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    bool PFX##_print(struct SNAME *_map_, FILE *fptr)                          \
    {                                                                          \
        for (size_t i = 0; i < _map_->capacity; i++)                           \
        {                                                                      \
            struct SNAME##_entry *target = _map_->buffer[i][0];                \
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
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                  \
    {                                                                          \
        struct SNAME##_iter *iter =                                            \
            target->alloc->malloc(sizeof(struct SNAME##_iter));                \
                                                                               \
        if (!iter)                                                             \
            return NULL;                                                       \
                                                                               \
        PFX##_iter_init(iter, target);                                         \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    void PFX##_iter_free(struct SNAME##_iter *iter)                            \
    {                                                                          \
        iter->target->alloc->free(iter);                                       \
    }                                                                          \
                                                                               \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)      \
    {                                                                          \
        memset(iter, 0, sizeof(struct SNAME##_iter));                          \
                                                                               \
        iter->target = target;                                                 \
        iter->start = true;                                                    \
        iter->end = PFX##_empty(target);                                       \
                                                                               \
        if (!PFX##_empty(target))                                              \
        {                                                                      \
            for (size_t i = 0; i < target->capacity; i++)                      \
            {                                                                  \
                struct SNAME##_entry *tmp = target->buffer[i][0];              \
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
                struct SNAME##_entry *tmp = target->buffer[i - 1][0];          \
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
    bool PFX##_iter_start(struct SNAME##_iter *iter)                           \
    {                                                                          \
        return PFX##_empty(iter->target) || iter->start;                       \
    }                                                                          \
                                                                               \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                             \
    {                                                                          \
        return PFX##_empty(iter->target) || iter->end;                         \
    }                                                                          \
                                                                               \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                        \
    {                                                                          \
        if (!PFX##_empty(iter->target))                                        \
        {                                                                      \
            iter->cursor = iter->first;                                        \
            iter->index = 0;                                                   \
            iter->start = true;                                                \
            iter->end = false;                                                 \
        }                                                                      \
    }                                                                          \
                                                                               \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                          \
    {                                                                          \
        if (!PFX##_empty(iter->target))                                        \
        {                                                                      \
            iter->cursor = iter->last;                                         \
            iter->index = iter->target->count - 1;                             \
            iter->start = false;                                               \
            iter->end = true;                                                  \
        }                                                                      \
    }                                                                          \
                                                                               \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                            \
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
        iter->start = PFX##_empty(iter->target);                               \
                                                                               \
        struct SNAME##_entry *scan = iter->target->buffer[iter->cursor][0];    \
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
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                            \
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
        iter->end = PFX##_empty(iter->target);                                 \
                                                                               \
        struct SNAME##_entry *scan = iter->target->buffer[iter->cursor][0];    \
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
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)           \
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
            PFX##_iter_next(iter);                                             \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps)            \
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
            PFX##_iter_prev(iter);                                             \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator was able to be positioned at */       \
    /* the given index */                                                      \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)             \
    {                                                                          \
        if (index >= iter->target->count)                                      \
            return false;                                                      \
                                                                               \
        if (iter->index > index)                                               \
            return PFX##_iter_rewind(iter, iter->index - index);               \
        else if (iter->index < index)                                          \
            return PFX##_iter_advance(iter, index - iter->index);              \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    K PFX##_iter_key(struct SNAME##_iter *iter)                                \
    {                                                                          \
        if (PFX##_empty(iter->target))                                         \
        {                                                                      \
            iter->target->flag = cmc_flags.EMPTY;                              \
            return (K){ 0 };                                                   \
        }                                                                      \
                                                                               \
        return iter->target->buffer[iter->cursor][0]->key;                     \
    }                                                                          \
                                                                               \
    V PFX##_iter_value(struct SNAME##_iter *iter)                              \
    {                                                                          \
        if (PFX##_empty(iter->target))                                         \
        {                                                                      \
            iter->target->flag = cmc_flags.EMPTY;                              \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        return iter->target->buffer[iter->cursor][0]->value;                   \
    }                                                                          \
                                                                               \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                         \
    {                                                                          \
        return iter->index;                                                    \
    }                                                                          \
                                                                               \
    static struct SNAME##_entry *PFX##_impl_new_entry(struct SNAME *_map_,     \
                                                      K key, V value)          \
    {                                                                          \
        struct SNAME##_entry *entry =                                          \
            _map_->alloc->malloc(sizeof(struct SNAME##_entry));                \
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
    static struct SNAME##_entry **PFX##_impl_get_entry_by_key(                 \
        struct SNAME *_map_, K key)                                            \
    {                                                                          \
        size_t hash = _map_->f_key->hash(key);                                 \
        size_t pos = hash % _map_->capacity;                                   \
                                                                               \
        struct SNAME##_entry *target = _map_->buffer[pos][0];                  \
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
    static struct SNAME##_entry **PFX##_impl_get_entry_by_val(                 \
        struct SNAME *_map_, V val)                                            \
    {                                                                          \
        size_t hash = _map_->f_val->hash(val);                                 \
        size_t pos = hash % _map_->capacity;                                   \
                                                                               \
        struct SNAME##_entry *target = _map_->buffer[pos][1];                  \
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
    static struct SNAME##_entry **PFX##_impl_add_entry_to_key(                 \
        struct SNAME *_map_, struct SNAME##_entry *entry)                      \
    {                                                                          \
        struct SNAME##_entry **to_return = NULL;                               \
                                                                               \
        size_t hash = _map_->f_key->hash(entry->key);                          \
        size_t original_pos = hash % _map_->capacity;                          \
        size_t pos = original_pos;                                             \
                                                                               \
        struct SNAME##_entry **scan =                                          \
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
                    struct SNAME##_entry *_tmp_ = *scan;                       \
                    *scan = entry;                                             \
                    entry = _tmp_;                                             \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    static struct SNAME##_entry **PFX##_impl_add_entry_to_val(                 \
        struct SNAME *_map_, struct SNAME##_entry *entry)                      \
    {                                                                          \
        struct SNAME##_entry **to_return = NULL;                               \
                                                                               \
        size_t hash = _map_->f_val->hash(entry->value);                        \
        size_t original_pos = hash % _map_->capacity;                          \
        size_t pos = original_pos;                                             \
                                                                               \
        struct SNAME##_entry **scan =                                          \
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
                    struct SNAME##_entry *_tmp_ = *scan;                       \
                    *scan = entry;                                             \
                    entry = _tmp_;                                             \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    static size_t PFX##_impl_calculate_size(size_t required)                   \
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
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_map_)        \
    {                                                                          \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _map_);                                         \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_map_)          \
    {                                                                          \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _map_);                                         \
        PFX##_iter_to_end(&iter);                                              \
                                                                               \
        return iter;                                                           \
    }

#endif /* CMC_BIDIMAP_H */
