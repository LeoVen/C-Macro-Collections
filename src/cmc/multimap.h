/**
 * multimap.h
 *
 * Creation Date: 26/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * MultiMap
 *
 * The MultiMap is a Map that allows for multiple keys. This is a data structure
 * that has a very narrow usage. A Map (either TreeMap or HashMap) can also work
 * like a MultiMap if a certain key is mapped to another collection.
 *
 * Map<K = int, V = List<int>> maps an integer to a list of integer.
 * MultiMap<K = int, V = int> maps many integer keys to integer values.
 *
 * The difference is that in a MultiMap you can store keys that are the same but
 * might be different instances of the same value. This is also relevant if your
 * data type is more complex like a struct where its ID is the same but some
 * other members of this data type are different.
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

#ifndef CMC_MULTIMAP_H
#define CMC_MULTIMAP_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * Hashtable Implementation
 * ------------------------------------------------------------------------- */
#include "../cor/hashtable.h"

/* -------------------------------------------------------------------------
 * MultiMap specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_string_fmt_multimap = "struct %s<%s, %s> "
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

#define CMC_GENERATE_MULTIMAP(PFX, SNAME, K, V)    \
    CMC_GENERATE_MULTIMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_MULTIMAP_SOURCE(PFX, SNAME, K, V)

#define CMC_WRAPGEN_MULTIMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_MULTIMAP_HEADER(PFX, SNAME, K, V)

#define CMC_WRAPGEN_MULTIMAP_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_MULTIMAP_SOURCE(PFX, SNAME, K, V)

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_MULTIMAP_HEADER(PFX, SNAME, K, V)                        \
                                                                              \
    /* Multimap Structure */                                                  \
    struct SNAME                                                              \
    {                                                                         \
        /* Array of linked list to entries */                                 \
        struct SNAME##_entry *(*buffer)[2];                                   \
                                                                              \
        /* Current array capacity */                                          \
        size_t capacity;                                                      \
                                                                              \
        /* Current amount of keys */                                          \
        size_t count;                                                         \
                                                                              \
        /* Load factor in range (0.0, infinity) */                            \
        double load;                                                          \
                                                                              \
        /* Flags indicating errors or success */                              \
        int flag;                                                             \
                                                                              \
        /* Key function table */                                              \
        struct SNAME##_fkey *f_key;                                           \
                                                                              \
        /* Value function table */                                            \
        struct SNAME##_fval *f_val;                                           \
                                                                              \
        /* Custom allocation functions */                                     \
        struct cmc_alloc_node *alloc;                                         \
                                                                              \
        /* Custom callback functions */                                       \
        struct cmc_callbacks *callbacks;                                      \
                                                                              \
        /* Function that returns an iterator to the start of the multimap */  \
        struct SNAME##_iter (*it_start)(struct SNAME *);                      \
                                                                              \
        /* Function that returns an iterator to the end of the multimap */    \
        struct SNAME##_iter (*it_end)(struct SNAME *);                        \
    };                                                                        \
                                                                              \
    /* Multimap Entry */                                                      \
    struct SNAME##_entry                                                      \
    {                                                                         \
        /* Entry Key */                                                       \
        K key;                                                                \
                                                                              \
        /* Entry Value */                                                     \
        V value;                                                              \
                                                                              \
        /* Next entry on the linked list */                                   \
        struct SNAME##_entry *next;                                           \
                                                                              \
        /* Previous entry on the linked list */                               \
        struct SNAME##_entry *prev;                                           \
    };                                                                        \
                                                                              \
    /* Key struct function table */                                           \
    struct SNAME##_fkey                                                       \
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
    struct SNAME##_fval                                                       \
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
    struct SNAME##_iter                                                       \
    {                                                                         \
        /* Target multimap */                                                 \
        struct SNAME *target;                                                 \
                                                                              \
        /* Current entry */                                                   \
        struct SNAME##_entry *curr_entry;                                     \
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
    /* Collection Functions */                                                \
    /* Collection Allocation and Deallocation */                              \
    struct SNAME *PFX##_new(size_t capacity, double load,                     \
                            struct SNAME##_fkey *f_key,                       \
                            struct SNAME##_fval *f_val);                      \
    struct SNAME *PFX##_new_custom(                                           \
        size_t capacity, double load, struct SNAME##_fkey *f_key,             \
        struct SNAME##_fval *f_val, struct cmc_alloc_node *alloc,             \
        struct cmc_callbacks *callbacks);                                     \
    void PFX##_clear(struct SNAME *_map_);                                    \
    void PFX##_free(struct SNAME *_map_);                                     \
    /* Customization of Allocation and Callbacks */                           \
    void PFX##_customize(struct SNAME *_map_, struct cmc_alloc_node *alloc,   \
                         struct cmc_callbacks *callbacks);                    \
    /* Collection Input and Output */                                         \
    bool PFX##_insert(struct SNAME *_map_, K key, V value);                   \
    bool PFX##_update(struct SNAME *_map_, K key, V new_value, V *old_value); \
    size_t PFX##_update_all(struct SNAME *_map_, K key, V new_value,          \
                            V **old_values);                                  \
    bool PFX##_remove(struct SNAME *_map_, K key, V *out_value);              \
    size_t PFX##_remove_all(struct SNAME *_map_, K key, V **out_values);      \
    /* Element Access */                                                      \
    bool PFX##_max(struct SNAME *_map_, K *key, V *value);                    \
    bool PFX##_min(struct SNAME *_map_, K *key, V *value);                    \
    V PFX##_get(struct SNAME *_map_, K key);                                  \
    V *PFX##_get_ref(struct SNAME *_map_, K key);                             \
    /* Collection State */                                                    \
    bool PFX##_contains(struct SNAME *_map_, K key);                          \
    bool PFX##_empty(struct SNAME *_map_);                                    \
    bool PFX##_full(struct SNAME *_map_);                                     \
    size_t PFX##_count(struct SNAME *_map_);                                  \
    size_t PFX##_key_count(struct SNAME *_map_, K key);                       \
    size_t PFX##_capacity(struct SNAME *_map_);                               \
    double PFX##_load(struct SNAME *_map_);                                   \
    int PFX##_flag(struct SNAME *_map_);                                      \
    /* Collection Utility */                                                  \
    bool PFX##_resize(struct SNAME *_map_, size_t capacity);                  \
    struct SNAME *PFX##_copy_of(struct SNAME *_map_);                         \
    bool PFX##_equals(struct SNAME *_map1_, struct SNAME *_map2_);            \
    struct cmc_string PFX##_to_string(struct SNAME *_map_);                   \
    bool PFX##_print(struct SNAME *_map_, FILE *fptr);                        \
                                                                              \
    /* Iterator Functions */                                                  \
    /* Iterator Allocation and Deallocation */                                \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);                \
    void PFX##_iter_free(struct SNAME##_iter *iter);                          \
    /* Iterator Initialization */                                             \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);    \
    /* Iterator State */                                                      \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                         \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                           \
    /* Iterator Movement */                                                   \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                      \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                        \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                          \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                          \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);         \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);          \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);           \
    /* Iterator Access */                                                     \
    K PFX##_iter_key(struct SNAME##_iter *iter);                              \
    V PFX##_iter_value(struct SNAME##_iter *iter);                            \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter);                          \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_MULTIMAP_SOURCE(PFX, SNAME, K, V)                         \
                                                                               \
    /* Implementation Detail Functions */                                      \
    struct SNAME##_entry *PFX##_impl_new_entry(struct SNAME *_map_, K key,     \
                                               V value);                       \
    struct SNAME##_entry *PFX##_impl_get_entry(struct SNAME *_map_, K key);    \
    size_t PFX##_impl_key_count(struct SNAME *_map_, K key);                   \
    size_t PFX##_impl_calculate_size(size_t required);                         \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_map_);       \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_map_);         \
                                                                               \
    struct SNAME *PFX##_new(size_t capacity, double load,                      \
                            struct SNAME##_fkey *f_key,                        \
                            struct SNAME##_fval *f_val)                        \
    {                                                                          \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;                \
                                                                               \
        if (capacity == 0 || load <= 0)                                        \
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
        _map_->it_end = PFX##_impl_it_end;                                     \
        _map_->it_start = PFX##_impl_it_start;                                 \
                                                                               \
        return _map_;                                                          \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_new_custom(                                            \
        size_t capacity, double load, struct SNAME##_fkey *f_key,              \
        struct SNAME##_fval *f_val, struct cmc_alloc_node *alloc,              \
        struct cmc_callbacks *callbacks)                                       \
    {                                                                          \
        if (capacity == 0 || load <= 0)                                        \
            return NULL;                                                       \
                                                                               \
        /* Prevent integer overflow */                                         \
        if (capacity >= UINTMAX_MAX * load)                                    \
            return NULL;                                                       \
                                                                               \
        if (!f_key || !f_val)                                                  \
            return NULL;                                                       \
                                                                               \
        if (!alloc)                                                            \
            alloc = &cmc_alloc_node_default;                                   \
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
            struct SNAME##_entry *scan = _map_->buffer[i][0];                  \
                                                                               \
            while (scan != NULL)                                               \
            {                                                                  \
                struct SNAME##_entry *next = scan->next;                       \
                                                                               \
                if (_map_->f_key->free)                                        \
                    _map_->f_key->free(scan->key);                             \
                if (_map_->f_val->free)                                        \
                    _map_->f_val->free(scan->value);                           \
                                                                               \
                _map_->alloc->free(scan);                                      \
                                                                               \
                scan = next;                                                   \
            }                                                                  \
        }                                                                      \
                                                                               \
        memset(_map_->buffer, 0,                                               \
               sizeof(struct SNAME##_entry *[2]) * _map_->capacity);           \
                                                                               \
        _map_->count = 0;                                                      \
        _map_->flag = cmc_flags.OK;                                            \
    }                                                                          \
                                                                               \
    void PFX##_free(struct SNAME *_map_)                                       \
    {                                                                          \
        size_t index = 0;                                                      \
                                                                               \
        while (index < _map_->capacity)                                        \
        {                                                                      \
            struct SNAME##_entry *scan = _map_->buffer[index][0];              \
                                                                               \
            if (scan != NULL)                                                  \
            {                                                                  \
                if (scan->next == NULL && scan->prev == NULL)                  \
                {                                                              \
                    if (_map_->f_key->free)                                    \
                        _map_->f_key->free(scan->key);                         \
                    if (_map_->f_val->free)                                    \
                        _map_->f_val->free(scan->value);                       \
                                                                               \
                    _map_->alloc->free(scan);                                  \
                }                                                              \
                else                                                           \
                {                                                              \
                    while (scan != NULL)                                       \
                    {                                                          \
                        struct SNAME##_entry *tmp = scan;                      \
                                                                               \
                        scan = scan->next;                                     \
                                                                               \
                        if (_map_->f_key->free)                                \
                            _map_->f_key->free(tmp->key);                      \
                        if (_map_->f_val->free)                                \
                            _map_->f_val->free(tmp->value);                    \
                                                                               \
                        _map_->alloc->free(tmp);                               \
                    }                                                          \
                }                                                              \
            }                                                                  \
                                                                               \
            index++;                                                           \
        }                                                                      \
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
        size_t hash = _map_->f_key->hash(key);                                 \
        size_t pos = hash % _map_->capacity;                                   \
                                                                               \
        struct SNAME##_entry *entry = PFX##_impl_new_entry(_map_, key, value); \
                                                                               \
        if (_map_->buffer[pos][0] == NULL)                                     \
        {                                                                      \
            _map_->buffer[pos][0] = entry;                                     \
            _map_->buffer[pos][1] = entry;                                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            entry->prev = _map_->buffer[pos][1];                               \
                                                                               \
            _map_->buffer[pos][1]->next = entry;                               \
            _map_->buffer[pos][1] = entry;                                     \
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
    bool PFX##_update(struct SNAME *_map_, K key, V new_value, V *old_value)   \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);        \
                                                                               \
        if (!entry)                                                            \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (old_value)                                                         \
            *old_value = entry->value;                                         \
                                                                               \
        entry->value = new_value;                                              \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->update)                      \
            _map_->callbacks->update();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    size_t PFX##_update_all(struct SNAME *_map_, K key, V new_value,           \
                            V **old_values)                                    \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return 0;                                                          \
        }                                                                      \
                                                                               \
        size_t hash = _map_->f_key->hash(key);                                 \
                                                                               \
        struct SNAME##_entry *entry =                                          \
            _map_->buffer[hash % _map_->capacity][0];                          \
                                                                               \
        if (entry == NULL)                                                     \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return 0;                                                          \
        }                                                                      \
                                                                               \
        if (old_values)                                                        \
        {                                                                      \
            size_t total = PFX##_impl_key_count(_map_, key);                   \
                                                                               \
            if (total == 0)                                                    \
            {                                                                  \
                _map_->flag = cmc_flags.NOT_FOUND;                             \
                return 0;                                                      \
            }                                                                  \
                                                                               \
            *old_values = _map_->alloc->malloc(sizeof(V) * total);             \
                                                                               \
            if (!(*old_values))                                                \
            {                                                                  \
                _map_->flag = cmc_flags.ALLOC;                                 \
                return 0;                                                      \
            }                                                                  \
        }                                                                      \
                                                                               \
        size_t index = 0;                                                      \
                                                                               \
        while (entry != NULL)                                                  \
        {                                                                      \
            if (_map_->f_key->cmp(entry->key, key) == 0)                       \
            {                                                                  \
                if (old_values)                                                \
                    (*old_values)[index] = entry->value;                       \
                                                                               \
                index++;                                                       \
                entry->value = new_value;                                      \
            }                                                                  \
                                                                               \
            entry = entry->next;                                               \
        }                                                                      \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->update)                      \
            _map_->callbacks->update();                                        \
                                                                               \
        return index;                                                          \
    }                                                                          \
                                                                               \
    bool PFX##_remove(struct SNAME *_map_, K key, V *out_value)                \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        size_t hash = _map_->f_key->hash(key);                                 \
                                                                               \
        struct SNAME##_entry **head =                                          \
            &(_map_->buffer[hash % _map_->capacity][0]);                       \
        struct SNAME##_entry **tail =                                          \
            &(_map_->buffer[hash % _map_->capacity][1]);                       \
                                                                               \
        if (*head == NULL)                                                     \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry *entry = *head;                                   \
                                                                               \
        if (entry->next == NULL && entry->prev == NULL)                        \
        {                                                                      \
            if (_map_->f_key->cmp(entry->key, key) == 0)                       \
            {                                                                  \
                *head = NULL;                                                  \
                *tail = NULL;                                                  \
                                                                               \
                if (out_value)                                                 \
                    *out_value = entry->value;                                 \
            }                                                                  \
            else                                                               \
            {                                                                  \
                _map_->flag = cmc_flags.NOT_FOUND;                             \
                return false;                                                  \
            }                                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            bool found = false;                                                \
                                                                               \
            while (entry != NULL)                                              \
            {                                                                  \
                if (_map_->f_key->cmp(entry->key, key) == 0)                   \
                {                                                              \
                    if (*head == entry)                                        \
                        *head = entry->next;                                   \
                    if (*tail == entry)                                        \
                        *tail = entry->prev;                                   \
                                                                               \
                    if (entry->prev != NULL)                                   \
                        entry->prev->next = entry->next;                       \
                    if (entry->next != NULL)                                   \
                        entry->next->prev = entry->prev;                       \
                                                                               \
                    if (out_value)                                             \
                        *out_value = entry->value;                             \
                                                                               \
                    found = true;                                              \
                                                                               \
                    break;                                                     \
                }                                                              \
                else                                                           \
                    entry = entry->next;                                       \
            }                                                                  \
                                                                               \
            if (!found)                                                        \
            {                                                                  \
                _map_->flag = cmc_flags.NOT_FOUND;                             \
                return false;                                                  \
            }                                                                  \
        }                                                                      \
                                                                               \
        _map_->alloc->free(entry);                                             \
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
    size_t PFX##_remove_all(struct SNAME *_map_, K key, V **out_values)        \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        size_t hash = _map_->f_key->hash(key);                                 \
                                                                               \
        struct SNAME##_entry **head =                                          \
            &(_map_->buffer[hash % _map_->capacity][0]);                       \
        struct SNAME##_entry **tail =                                          \
            &(_map_->buffer[hash % _map_->capacity][1]);                       \
                                                                               \
        if (*head == NULL)                                                     \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return 0;                                                          \
        }                                                                      \
                                                                               \
        if (out_values)                                                        \
        {                                                                      \
            size_t total = PFX##_impl_key_count(_map_, key);                   \
                                                                               \
            if (total == 0)                                                    \
            {                                                                  \
                _map_->flag = cmc_flags.NOT_FOUND;                             \
                return 0;                                                      \
            }                                                                  \
                                                                               \
            *out_values = _map_->alloc->malloc(sizeof(V) * total);             \
                                                                               \
            if (!(*out_values))                                                \
            {                                                                  \
                _map_->flag = cmc_flags.ALLOC;                                 \
                return 0;                                                      \
            }                                                                  \
        }                                                                      \
                                                                               \
        size_t index = 0;                                                      \
        struct SNAME##_entry *entry = *head;                                   \
                                                                               \
        if (entry->next == NULL)                                               \
        {                                                                      \
            *head = NULL;                                                      \
            *tail = NULL;                                                      \
                                                                               \
            if (out_values)                                                    \
                (*out_values)[index] = entry->value;                           \
                                                                               \
            index++;                                                           \
            _map_->alloc->free(entry);                                         \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            while (entry != NULL)                                              \
            {                                                                  \
                if (_map_->f_key->cmp(entry->key, key) == 0)                   \
                {                                                              \
                    if (*head == entry)                                        \
                        *head = entry->next;                                   \
                    if (*tail == entry)                                        \
                        *tail = entry->prev;                                   \
                                                                               \
                    struct SNAME##_entry *next = entry->next;                  \
                                                                               \
                    if (entry->prev != NULL)                                   \
                        entry->prev->next = entry->next;                       \
                    if (entry->next != NULL)                                   \
                        entry->next->prev = entry->prev;                       \
                                                                               \
                    if (out_values)                                            \
                        (*out_values)[index] = entry->value;                   \
                                                                               \
                    index++;                                                   \
                    _map_->alloc->free(entry);                                 \
                                                                               \
                    entry = next;                                              \
                }                                                              \
                else                                                           \
                    entry = entry->next;                                       \
            }                                                                  \
        }                                                                      \
                                                                               \
        _map_->count -= index;                                                 \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->delete)                      \
            _map_->callbacks->delete ();                                       \
                                                                               \
        return index;                                                          \
    }                                                                          \
                                                                               \
    bool PFX##_max(struct SNAME *_map_, K *key, V *value)                      \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        K max_key;                                                             \
        V max_val;                                                             \
                                                                               \
        struct SNAME##_iter iter;                                              \
                                                                               \
        /* TODO Turn this into a normal loop */                                \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter);            \
             PFX##_iter_next(&iter))                                           \
        {                                                                      \
            K result_key = PFX##_iter_key(&iter);                              \
            V result_value = PFX##_iter_value(&iter);                          \
            size_t index = PFX##_iter_index(&iter);                            \
                                                                               \
            if (index == 0)                                                    \
            {                                                                  \
                max_key = result_key;                                          \
                max_val = result_value;                                        \
            }                                                                  \
            else if (_map_->f_key->cmp(result_key, max_key) > 0)               \
            {                                                                  \
                max_key = result_key;                                          \
                max_val = result_value;                                        \
            }                                                                  \
        }                                                                      \
                                                                               \
        if (key)                                                               \
            *key = max_key;                                                    \
        if (value)                                                             \
            *value = max_val;                                                  \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_min(struct SNAME *_map_, K *key, V *value)                      \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        K min_key;                                                             \
        V min_val;                                                             \
                                                                               \
        struct SNAME##_iter iter;                                              \
                                                                               \
        /* TODO Turn this into a normal loop */                                \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter);            \
             PFX##_iter_next(&iter))                                           \
        {                                                                      \
            K result_key = PFX##_iter_key(&iter);                              \
            V result_value = PFX##_iter_value(&iter);                          \
            size_t index = PFX##_iter_index(&iter);                            \
                                                                               \
            if (index == 0)                                                    \
            {                                                                  \
                min_key = result_key;                                          \
                min_val = result_value;                                        \
            }                                                                  \
            else if (_map_->f_key->cmp(result_key, min_key) < 0)               \
            {                                                                  \
                min_key = result_key;                                          \
                min_val = result_value;                                        \
            }                                                                  \
        }                                                                      \
                                                                               \
        if (key)                                                               \
            *key = min_key;                                                    \
        if (value)                                                             \
            *value = min_val;                                                  \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    V PFX##_get(struct SNAME *_map_, K key)                                    \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);        \
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
        return entry->value;                                                   \
    }                                                                          \
                                                                               \
    V *PFX##_get_ref(struct SNAME *_map_, K key)                               \
    {                                                                          \
        if (PFX##_empty(_map_))                                                \
        {                                                                      \
            _map_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);        \
                                                                               \
        if (!entry)                                                            \
        {                                                                      \
            _map_->flag = cmc_flags.NOT_FOUND;                                 \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return &(entry->value);                                                \
    }                                                                          \
                                                                               \
    bool PFX##_contains(struct SNAME *_map_, K key)                            \
    {                                                                          \
        _map_->flag = cmc_flags.OK;                                            \
                                                                               \
        bool result = PFX##_impl_get_entry(_map_, key) != NULL;                \
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
    size_t PFX##_key_count(struct SNAME *_map_, K key)                         \
    {                                                                          \
        return PFX##_impl_key_count(_map_, key);                               \
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
        size_t theoretical_size = PFX##_impl_calculate_size(capacity);         \
                                                                               \
        /* Not possible to shrink with current available prime numbers */      \
        if (theoretical_size < _map_->count / _map_->load)                     \
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
        struct SNAME##_iter iter;                                              \
                                                                               \
        /* TODO Turn this into a normal loop */                                \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter);            \
             PFX##_iter_next(&iter))                                           \
        {                                                                      \
            K key = PFX##_iter_key(&iter);                                     \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            /* TODO check for errors */                                        \
            PFX##_insert(_new_map_, key, value);                               \
        }                                                                      \
                                                                               \
        if (_map_->count != _new_map_->count)                                  \
        {                                                                      \
            PFX##_free(_new_map_);                                             \
                                                                               \
            _map_->flag = cmc_flags.ERROR;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry *(*tmp_b)[2] = _map_->buffer;                     \
        _map_->buffer = _new_map_->buffer;                                     \
        _new_map_->buffer = tmp_b;                                             \
                                                                               \
        size_t tmp_c = _map_->capacity;                                        \
        _map_->capacity = _new_map_->capacity;                                 \
        _new_map_->capacity = tmp_c;                                           \
                                                                               \
        /* Prevent the map from freeing the data */                            \
        _new_map_->f_key = &(struct SNAME##_fkey){ NULL };                     \
        _new_map_->f_val = &(struct SNAME##_fval){ NULL };                     \
                                                                               \
        PFX##_free(_new_map_);                                                 \
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
        /* Callback will be added later */                                     \
        struct SNAME *result =                                                 \
            PFX##_new_custom(_map_->capacity * _map_->load, _map_->load,       \
                             _map_->f_key, _map_->f_val, _map_->alloc, NULL);  \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _map_->flag = cmc_flags.ERROR;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct SNAME##_iter iter;                                              \
        PFX##_iter_init(&iter, _map_);                                         \
                                                                               \
        if (!PFX##_empty(_map_))                                               \
        {                                                                      \
            /* TODO Turn this into a normal loop */                            \
            for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter);           \
                 PFX##_iter_next(&iter))                                       \
            {                                                                  \
                K key = PFX##_iter_key(&iter);                                 \
                V value = PFX##_iter_value(&iter);                             \
                                                                               \
                if (_map_->f_key->cpy)                                         \
                    key = _map_->f_key->cpy(key);                              \
                if (_map_->f_val->cpy)                                         \
                    value = _map_->f_val->cpy(value);                          \
                                                                               \
                /* TODO check for errors */                                    \
                PFX##_insert(result, key, value);                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        result->callbacks = _map_->callbacks;                                  \
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
        struct SNAME##_iter iter;                                              \
        PFX##_iter_init(&iter, _map1_);                                        \
                                                                               \
        /* TODO Turn this into a normal loop */                                \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter);               \
             PFX##_iter_next(&iter))                                           \
        {                                                                      \
            K key = PFX##_iter_key(&iter);                                     \
                                                                               \
            if (PFX##_impl_key_count(_map1_, key) !=                           \
                PFX##_impl_key_count(_map2_, key))                             \
                return false;                                                  \
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
        int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_multimap,       \
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
            struct SNAME##_entry *scan = _map_->buffer[i][0];                  \
                                                                               \
            while (scan != NULL)                                               \
            {                                                                  \
                if (!_map_->f_key->str(fptr, scan->key) ||                     \
                    !_map_->f_val->str(fptr, scan->value))                     \
                    return false;                                              \
                                                                               \
                scan = scan->next;                                             \
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
                if (target->buffer[i][0] != NULL)                              \
                {                                                              \
                    iter->curr_entry = target->buffer[i][0];                   \
                    iter->first = i;                                           \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            iter->cursor = iter->first;                                        \
                                                                               \
            for (size_t i = target->capacity; i > 0; i--)                      \
            {                                                                  \
                if (target->buffer[i - 1][0] != NULL)                          \
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
            iter->end = PFX##_empty(iter->target);                             \
            iter->curr_entry = iter->target->buffer[iter->first][0];           \
        }                                                                      \
    }                                                                          \
                                                                               \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                          \
    {                                                                          \
        if (!PFX##_empty(iter->target))                                        \
        {                                                                      \
            iter->cursor = iter->last;                                         \
            iter->index = iter->target->count - 1;                             \
            iter->start = PFX##_empty(iter->target);                           \
            iter->end = true;                                                  \
            iter->curr_entry = iter->target->buffer[iter->last][1];            \
        }                                                                      \
    }                                                                          \
                                                                               \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                            \
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
        iter->start = PFX##_empty(iter->target);                               \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                            \
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
        iter->end = PFX##_empty(iter->target);                                 \
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
    /* Returns true only if the iterator was able to be positioned at the */   \
    /* given index */                                                          \
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
            return (K){ 0 };                                                   \
                                                                               \
        return iter->curr_entry->key;                                          \
    }                                                                          \
                                                                               \
    V PFX##_iter_value(struct SNAME##_iter *iter)                              \
    {                                                                          \
        if (PFX##_empty(iter->target))                                         \
            return (V){ 0 };                                                   \
                                                                               \
        return iter->curr_entry->value;                                        \
    }                                                                          \
                                                                               \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter)                            \
    {                                                                          \
        if (PFX##_empty(iter->target))                                         \
            return NULL;                                                       \
                                                                               \
        return &(iter->curr_entry->value);                                     \
    }                                                                          \
                                                                               \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                         \
    {                                                                          \
        return iter->index;                                                    \
    }                                                                          \
                                                                               \
    struct SNAME##_entry *PFX##_impl_new_entry(struct SNAME *_map_, K key,     \
                                               V value)                        \
    {                                                                          \
        struct SNAME##_entry *entry =                                          \
            _map_->alloc->malloc(sizeof(struct SNAME##_entry));                \
                                                                               \
        if (!entry)                                                            \
            return NULL;                                                       \
                                                                               \
        entry->key = key;                                                      \
        entry->value = value;                                                  \
        entry->next = NULL;                                                    \
        entry->prev = NULL;                                                    \
                                                                               \
        return entry;                                                          \
    }                                                                          \
                                                                               \
    struct SNAME##_entry *PFX##_impl_get_entry(struct SNAME *_map_, K key)     \
    {                                                                          \
        size_t hash = _map_->f_key->hash(key);                                 \
                                                                               \
        struct SNAME##_entry *entry =                                          \
            _map_->buffer[hash % _map_->capacity][0];                          \
                                                                               \
        while (entry != NULL)                                                  \
        {                                                                      \
            if (_map_->f_key->cmp(entry->key, key) == 0)                       \
                return entry;                                                  \
                                                                               \
            entry = entry->next;                                               \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    size_t PFX##_impl_key_count(struct SNAME *_map_, K key)                    \
    {                                                                          \
        size_t hash = _map_->f_key->hash(key);                                 \
                                                                               \
        struct SNAME##_entry *entry =                                          \
            _map_->buffer[hash % _map_->capacity][0];                          \
                                                                               \
        size_t total_count = 0;                                                \
                                                                               \
        if (!entry)                                                            \
            return total_count;                                                \
                                                                               \
        while (entry != NULL)                                                  \
        {                                                                      \
            if (_map_->f_key->cmp(entry->key, key) == 0)                       \
                total_count++;                                                 \
                                                                               \
            entry = entry->next;                                               \
        }                                                                      \
                                                                               \
        return total_count;                                                    \
    }                                                                          \
                                                                               \
    size_t PFX##_impl_calculate_size(size_t required)                          \
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

#endif /* CMC_MULTIMAP_H */
