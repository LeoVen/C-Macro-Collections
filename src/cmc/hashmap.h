/**
 * hashmap.h
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

#ifndef CMC_HASHMAP_H
#define CMC_HASHMAP_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#ifndef CMC_CORE_H
#define CMC_CORE_H

/**
 * struct cmc_string
 *
 * Used by all collections when calling the to_string function.
 */
struct cmc_string
{
    char s[400];
};

static const size_t cmc_string_len = 400;

/**
 * struct cmc_alloc_node
 *
 * Custom allocation node. Allows collections to use custom allocation
 * functions.
 */
static struct cmc_alloc_node
{
    void *(*malloc)(size_t);
    void *(*calloc)(size_t, size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
} cmc_alloc_node_default = { malloc, calloc, realloc, free };

/**
 * enum cmc_flags
 *
 * Defines common error codes used by all collections. These are flags that
 * indicate if something went wrong in the last operation by the collection.
 */
static struct
{
    int OK;        // No errors
    int ALLOC;     // Allocation failed
    int EMPTY;     // The collection is empty when it should not
    int NOT_FOUND; // Key or value not found
    int INVALID;   // Invalid argument or operation
    int RANGE;     // Index out of range
    int DUPLICATE; // Duplicate key or value
    int ERROR;     // Generic error, usually caused by algorithm error
} cmc_flags = { 0, 1, 2, 3, 4, 5, 6, 7 };

#endif /* CMC_CORE_H */

/* -------------------------------------------------------------------------
 * Hashtable Implementation
 * ------------------------------------------------------------------------- */
#ifndef CMC_IMPL_HASHTABLE_STATE
#define CMC_IMPL_HASHTABLE_STATE

enum cmc_entry_state
{
    CMC_ES_DELETED = -1,
    CMC_ES_EMPTY = 0,
    CMC_ES_FILLED = 1
};

#endif /* CMC_IMPL_HASHTABLE_STATE */

#ifndef CMC_IMPL_HASHTABLE_PRIMES
#define CMC_IMPL_HASHTABLE_PRIMES

// clang-format off
static const size_t cmc_hashtable_primes[] = {
    /* < 1e3 */
    53, 97, 191, 383, 769,
    /* < 1e4 */
    1531, 3067, 6143,
    /* < 1e5 */
    12289, 24571, 49157, 98299,
    /* < 1e6 */
    196613, 393209, 786431,
    /* < 1e7 */
    1572869, 3145721, 6291449,
    /* < 1e8 */
    12582917, 25165813, 50331653,
    /* < 1e9 */
    100663291, 201326611, 402653189, 805306357,
    /* < 1e10 */
    1610612741, 3221225473, 6442450939,
    /* < 1e11 */
    12884901893, 25769803799, 51539607551,
    /* < 1e12 */
    103079215111, 206158430209, 412316860441, 824633720831,
    /* < 1e13 */
    1649267441651, 3298534883309, 6597069766657,
    /* < 1e14 */
    13194139533299, 26388279066623, 52776558133303,
    /* < 1e15 */
    105553116266489, 211106232532969, 422212465066001, 844424930131963,
    /* < 1e16 */
    1688849860263953, 3377699720527861, 6755399441055731,
    /* < 1e17 */
    13510798882111483, 27021597764222939, 54043195528445957,
    /* < 1e18 */
    108086391056891903, 216172782113783773, 432345564227567621, 864691128455135207,
    /* < 1e19 */
    1729382256910270481, 3458764513820540933, 6917529027641081903,
    /* < 1e20 */
    13835058055282163729llu,
};
// clang-format on

#endif /* CMC_IMPL_HASHTABLE_PRIMES */

/* -------------------------------------------------------------------------
 * HashMap Specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_string_fmt_hashmap = "struct %s<%s, %s> "
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
 * Custom HashMap callbacks.
 *
 * There are two types of callbacks, 'before' and 'after':
 *      <before|after>_<function_name>
 */
struct cmc_callbacks_hashmap
{
    void (*before_clear)(void *);
    void (*after_clear)(void *);
    void (*before_free)(void *);
    void (*after_free)(void *);
    // TODO implement all callbacks
};

#define CMC_GENERATE_HASHMAP(PFX, SNAME, K, V)    \
    CMC_GENERATE_HASHMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_HASHMAP_SOURCE(PFX, SNAME, K, V)

#define CMC_WRAPGEN_HASHMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_HASHMAP_HEADER(PFX, SNAME, K, V)

#define CMC_WRAPGEN_HASHMAP_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_HASHMAP_SOURCE(PFX, SNAME, K, V)

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_HASHMAP_HEADER(PFX, SNAME, K, V)                         \
                                                                              \
    /* Hashmap Structure */                                                   \
    struct SNAME                                                              \
    {                                                                         \
        /* Array of Entries */                                                \
        struct SNAME##_entry *buffer;                                         \
                                                                              \
        /* Current array capacity */                                          \
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
        struct SNAME##_ftab_key *f_key;                                       \
                                                                              \
        /* Value function table */                                            \
        struct SNAME##_ftab_val *f_val;                                       \
                                                                              \
        /* Custom allocation functions */                                     \
        struct cmc_alloc_node *alloc;                                         \
                                                                              \
        /* Custom callback functions */                                       \
        struct cmc_callbacks_hashmap *callbacks;                              \
                                                                              \
        /* Function that returns an iterator to the start of the hashmap */   \
        struct SNAME##_iter (*it_start)(struct SNAME *);                      \
                                                                              \
        /* Function that returns an iterator to the end of the hashmap */     \
        struct SNAME##_iter (*it_end)(struct SNAME *);                        \
    };                                                                        \
                                                                              \
    /* Hashmap Entry */                                                       \
    struct SNAME##_entry                                                      \
    {                                                                         \
        /* Entry Key */                                                       \
        K key;                                                                \
                                                                              \
        /* Entry Value */                                                     \
        V value;                                                              \
                                                                              \
        /* The distance of this node to its original position, used by */     \
        /* robin-hood hashing */                                              \
        size_t dist;                                                          \
                                                                              \
        /* The sate of this node (DELETED, EMPTY, FILLED) */                  \
        enum cmc_entry_state state;                                           \
    };                                                                        \
                                                                              \
    /* Key struct function table */                                           \
    struct SNAME##_ftab_key                                                   \
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
    struct SNAME##_ftab_val                                                   \
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
    /* Hashmap Iterator */                                                    \
    struct SNAME##_iter                                                       \
    {                                                                         \
        /* Target hashmap */                                                  \
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
    struct SNAME *PFX##_new(size_t capacity, double load,                     \
                            struct SNAME##_ftab_key *f_key,                   \
                            struct SNAME##_ftab_val *f_val);                  \
    struct SNAME *PFX##_new_custom(                                           \
        size_t capacity, double load, struct SNAME##_ftab_key *f_key,         \
        struct SNAME##_ftab_val *f_val, struct cmc_alloc_node *alloc,         \
        struct cmc_callbacks_hashmap *callbacks);                             \
    void PFX##_clear(struct SNAME *_map_);                                    \
    void PFX##_free(struct SNAME *_map_);                                     \
    /* Customization of Allocation and Callbacks */                           \
    void PFX##_customize(struct SNAME *_map_, struct cmc_alloc_node *alloc,   \
                         struct cmc_callbacks_hashmap *callbacks);            \
    /* Collection Input and Output */                                         \
    bool PFX##_insert(struct SNAME *_map_, K key, V value);                   \
    bool PFX##_update(struct SNAME *_map_, K key, V new_value, V *old_value); \
    bool PFX##_remove(struct SNAME *_map_, K key, V *out_value);              \
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
#define CMC_GENERATE_HASHMAP_SOURCE(PFX, SNAME, K, V)                         \
                                                                              \
    /* Implementation Detail Functions */                                     \
    static struct SNAME##_entry *PFX##_impl_get_entry(struct SNAME *_map_,    \
                                                      K key);                 \
    static size_t PFX##_impl_calculate_size(size_t required);                 \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_map_);      \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_map_);        \
                                                                              \
    struct SNAME *PFX##_new(size_t capacity, double load,                     \
                            struct SNAME##_ftab_key *f_key,                   \
                            struct SNAME##_ftab_val *f_val)                   \
    {                                                                         \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;               \
                                                                              \
        if (capacity == 0 || load <= 0 || load >= 1)                          \
            return NULL;                                                      \
                                                                              \
        /* Prevent integer overflow */                                        \
        if (capacity >= UINTMAX_MAX * load)                                   \
            return NULL;                                                      \
                                                                              \
        if (!f_key || !f_val)                                                 \
            return NULL;                                                      \
                                                                              \
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);    \
                                                                              \
        struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME));            \
                                                                              \
        if (!_map_)                                                           \
            return NULL;                                                      \
                                                                              \
        _map_->buffer =                                                       \
            alloc->calloc(real_capacity, sizeof(struct SNAME##_entry));       \
                                                                              \
        if (!_map_->buffer)                                                   \
        {                                                                     \
            alloc->free(_map_);                                               \
            return NULL;                                                      \
        }                                                                     \
                                                                              \
        _map_->count = 0;                                                     \
        _map_->capacity = real_capacity;                                      \
        _map_->load = load;                                                   \
        _map_->flag = cmc_flags.OK;                                           \
        _map_->f_key = f_key;                                                 \
        _map_->f_val = f_val;                                                 \
        _map_->alloc = alloc;                                                 \
        _map_->callbacks = NULL;                                              \
        _map_->it_start = PFX##_impl_it_start;                                \
        _map_->it_end = PFX##_impl_it_end;                                    \
                                                                              \
        return _map_;                                                         \
    }                                                                         \
                                                                              \
    struct SNAME *PFX##_new_custom(                                           \
        size_t capacity, double load, struct SNAME##_ftab_key *f_key,         \
        struct SNAME##_ftab_val *f_val, struct cmc_alloc_node *alloc,         \
        struct cmc_callbacks_hashmap *callbacks)                              \
    {                                                                         \
        if (capacity == 0 || load <= 0 || load >= 1)                          \
            return NULL;                                                      \
                                                                              \
        /* Prevent integer overflow */                                        \
        if (capacity >= UINTMAX_MAX * load)                                   \
            return NULL;                                                      \
                                                                              \
        if (!f_key || !f_val)                                                 \
            return NULL;                                                      \
                                                                              \
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);    \
                                                                              \
        if (!alloc)                                                           \
            alloc = &cmc_alloc_node_default;                                  \
                                                                              \
        struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME));            \
                                                                              \
        if (!_map_)                                                           \
            return NULL;                                                      \
                                                                              \
        _map_->buffer =                                                       \
            alloc->calloc(real_capacity, sizeof(struct SNAME##_entry));       \
                                                                              \
        if (!_map_->buffer)                                                   \
        {                                                                     \
            alloc->free(_map_);                                               \
            return NULL;                                                      \
        }                                                                     \
                                                                              \
        _map_->count = 0;                                                     \
        _map_->capacity = real_capacity;                                      \
        _map_->load = load;                                                   \
        _map_->flag = cmc_flags.OK;                                           \
        _map_->f_key = f_key;                                                 \
        _map_->f_val = f_val;                                                 \
        _map_->alloc = alloc;                                                 \
        _map_->callbacks = callbacks;                                         \
        _map_->it_start = PFX##_impl_it_start;                                \
        _map_->it_end = PFX##_impl_it_end;                                    \
                                                                              \
        return _map_;                                                         \
    }                                                                         \
                                                                              \
    void PFX##_clear(struct SNAME *_map_)                                     \
    {                                                                         \
        if (_map_->f_key->free || _map_->f_val->free)                         \
        {                                                                     \
            for (size_t i = 0; i < _map_->capacity; i++)                      \
            {                                                                 \
                struct SNAME##_entry *entry = &(_map_->buffer[i]);            \
                                                                              \
                if (entry->state == CMC_ES_FILLED)                            \
                {                                                             \
                    if (_map_->f_key->free)                                   \
                        _map_->f_key->free(entry->key);                       \
                    if (_map_->f_val->free)                                   \
                        _map_->f_val->free(entry->value);                     \
                }                                                             \
            }                                                                 \
        }                                                                     \
                                                                              \
        memset(_map_->buffer, 0,                                              \
               sizeof(struct SNAME##_entry) * _map_->capacity);               \
                                                                              \
        _map_->count = 0;                                                     \
        _map_->flag = cmc_flags.OK;                                           \
    }                                                                         \
                                                                              \
    void PFX##_free(struct SNAME *_map_)                                      \
    {                                                                         \
        if (_map_->f_key->free || _map_->f_val->free)                         \
        {                                                                     \
            for (size_t i = 0; i < _map_->capacity; i++)                      \
            {                                                                 \
                struct SNAME##_entry *entry = &(_map_->buffer[i]);            \
                                                                              \
                if (entry->state == CMC_ES_FILLED)                            \
                {                                                             \
                    if (_map_->f_key->free)                                   \
                        _map_->f_key->free(entry->key);                       \
                    if (_map_->f_val->free)                                   \
                        _map_->f_val->free(entry->value);                     \
                }                                                             \
            }                                                                 \
        }                                                                     \
                                                                              \
        _map_->alloc->free(_map_->buffer);                                    \
        _map_->alloc->free(_map_);                                            \
    }                                                                         \
                                                                              \
    void PFX##_customize(struct SNAME *_map_, struct cmc_alloc_node *alloc,   \
                         struct cmc_callbacks_hashmap *callbacks)             \
    {                                                                         \
        if (alloc)                                                            \
            _map_->alloc = alloc;                                             \
                                                                              \
        if (callbacks)                                                        \
            _map_->callbacks = callbacks;                                     \
                                                                              \
        _map_->flag = cmc_flags.OK;                                           \
    }                                                                         \
                                                                              \
    bool PFX##_insert(struct SNAME *_map_, K key, V value)                    \
    {                                                                         \
        if (PFX##_full(_map_))                                                \
        {                                                                     \
            if (!PFX##_resize(_map_, _map_->capacity + 1))                    \
                return false;                                                 \
        }                                                                     \
                                                                              \
        size_t hash = _map_->f_key->hash(key);                                \
        size_t original_pos = hash % _map_->capacity;                         \
        size_t pos = original_pos;                                            \
                                                                              \
        struct SNAME##_entry *target = &(_map_->buffer[pos]);                 \
                                                                              \
        if (PFX##_impl_get_entry(_map_, key) != NULL)                         \
        {                                                                     \
            _map_->flag = cmc_flags.DUPLICATE;                                \
            return false;                                                     \
        }                                                                     \
                                                                              \
        if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED) \
        {                                                                     \
            target->key = key;                                                \
            target->value = value;                                            \
            target->dist = 0;                                                 \
            target->state = CMC_ES_FILLED;                                    \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            while (true)                                                      \
            {                                                                 \
                pos++;                                                        \
                target = &(_map_->buffer[pos % _map_->capacity]);             \
                                                                              \
                if (target->state == CMC_ES_EMPTY ||                          \
                    target->state == CMC_ES_DELETED)                          \
                {                                                             \
                    target->key = key;                                        \
                    target->value = value;                                    \
                    target->dist = pos - original_pos;                        \
                    target->state = CMC_ES_FILLED;                            \
                                                                              \
                    break;                                                    \
                }                                                             \
                else if (target->dist < pos - original_pos)                   \
                {                                                             \
                    K tmp_k = target->key;                                    \
                    V tmp_v = target->value;                                  \
                    size_t tmp_dist = target->dist;                           \
                                                                              \
                    target->key = key;                                        \
                    target->value = value;                                    \
                    target->dist = pos - original_pos;                        \
                                                                              \
                    key = tmp_k;                                              \
                    value = tmp_v;                                            \
                    original_pos = pos - tmp_dist;                            \
                }                                                             \
            }                                                                 \
        }                                                                     \
                                                                              \
        _map_->count++;                                                       \
        _map_->flag = cmc_flags.OK;                                           \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    bool PFX##_update(struct SNAME *_map_, K key, V new_value, V *old_value)  \
    {                                                                         \
        if (PFX##_empty(_map_))                                               \
        {                                                                     \
            _map_->flag = cmc_flags.EMPTY;                                    \
            return false;                                                     \
        }                                                                     \
                                                                              \
        struct SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);       \
                                                                              \
        if (!entry)                                                           \
        {                                                                     \
            _map_->flag = cmc_flags.NOT_FOUND;                                \
            return false;                                                     \
        }                                                                     \
                                                                              \
        if (old_value)                                                        \
            *old_value = entry->value;                                        \
                                                                              \
        entry->value = new_value;                                             \
                                                                              \
        _map_->flag = cmc_flags.OK;                                           \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    bool PFX##_remove(struct SNAME *_map_, K key, V *out_value)               \
    {                                                                         \
        if (PFX##_empty(_map_))                                               \
        {                                                                     \
            _map_->flag = cmc_flags.EMPTY;                                    \
            return false;                                                     \
        }                                                                     \
                                                                              \
        struct SNAME##_entry *result = PFX##_impl_get_entry(_map_, key);      \
                                                                              \
        if (result == NULL)                                                   \
        {                                                                     \
            _map_->flag = cmc_flags.NOT_FOUND;                                \
            return false;                                                     \
        }                                                                     \
                                                                              \
        if (out_value)                                                        \
            *out_value = result->value;                                       \
                                                                              \
        result->key = (K){ 0 };                                               \
        result->value = (V){ 0 };                                             \
        result->dist = 0;                                                     \
        result->state = CMC_ES_DELETED;                                       \
                                                                              \
        _map_->count--;                                                       \
        _map_->flag = cmc_flags.OK;                                           \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    bool PFX##_max(struct SNAME *_map_, K *key, V *value)                     \
    {                                                                         \
        if (PFX##_empty(_map_))                                               \
        {                                                                     \
            _map_->flag = cmc_flags.EMPTY;                                    \
            return false;                                                     \
        }                                                                     \
                                                                              \
        struct SNAME##_iter iter;                                             \
        PFX##_iter_init(&iter, _map_);                                        \
                                                                              \
        K max_key = PFX##_iter_key(&iter);                                    \
        V max_val = PFX##_iter_value(&iter);                                  \
                                                                              \
        PFX##_iter_next(&iter);                                               \
                                                                              \
        /* TODO Turn this into a normal loop */                               \
        for (; !PFX##_iter_end(&iter); PFX##_iter_next(&iter))                \
        {                                                                     \
            K iter_key = PFX##_iter_key(&iter);                               \
            V iter_val = PFX##_iter_value(&iter);                             \
                                                                              \
            if (_map_->f_key->cmp(iter_key, max_key) > 0)                     \
            {                                                                 \
                max_key = iter_key;                                           \
                max_val = iter_val;                                           \
            }                                                                 \
        }                                                                     \
                                                                              \
        if (key)                                                              \
            *key = max_key;                                                   \
        if (value)                                                            \
            *value = max_val;                                                 \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    bool PFX##_min(struct SNAME *_map_, K *key, V *value)                     \
    {                                                                         \
        if (PFX##_empty(_map_))                                               \
        {                                                                     \
            _map_->flag = cmc_flags.EMPTY;                                    \
            return false;                                                     \
        }                                                                     \
                                                                              \
        struct SNAME##_iter iter;                                             \
        PFX##_iter_init(&iter, _map_);                                        \
                                                                              \
        K min_key = PFX##_iter_key(&iter);                                    \
        V min_val = PFX##_iter_value(&iter);                                  \
                                                                              \
        PFX##_iter_next(&iter);                                               \
                                                                              \
        /* TODO Turn this into a normal loop */                               \
        for (; !PFX##_iter_end(&iter); PFX##_iter_next(&iter))                \
        {                                                                     \
            K iter_key = PFX##_iter_key(&iter);                               \
            V iter_val = PFX##_iter_value(&iter);                             \
                                                                              \
            if (_map_->f_key->cmp(iter_key, min_key) < 0)                     \
            {                                                                 \
                min_key = iter_key;                                           \
                min_val = iter_val;                                           \
            }                                                                 \
        }                                                                     \
                                                                              \
        if (key)                                                              \
            *key = min_key;                                                   \
        if (value)                                                            \
            *value = min_val;                                                 \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    V PFX##_get(struct SNAME *_map_, K key)                                   \
    {                                                                         \
        if (PFX##_empty(_map_))                                               \
        {                                                                     \
            _map_->flag = cmc_flags.EMPTY;                                    \
            return false;                                                     \
        }                                                                     \
                                                                              \
        struct SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);       \
                                                                              \
        if (!entry)                                                           \
        {                                                                     \
            _map_->flag = cmc_flags.NOT_FOUND;                                \
            return (V){ 0 };                                                  \
        }                                                                     \
                                                                              \
        _map_->flag = cmc_flags.OK;                                           \
                                                                              \
        return entry->value;                                                  \
    }                                                                         \
                                                                              \
    V *PFX##_get_ref(struct SNAME *_map_, K key)                              \
    {                                                                         \
        if (PFX##_empty(_map_))                                               \
        {                                                                     \
            _map_->flag = cmc_flags.EMPTY;                                    \
            return false;                                                     \
        }                                                                     \
                                                                              \
        struct SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);       \
                                                                              \
        if (!entry)                                                           \
        {                                                                     \
            _map_->flag = cmc_flags.NOT_FOUND;                                \
            return NULL;                                                      \
        }                                                                     \
                                                                              \
        _map_->flag = cmc_flags.OK;                                           \
                                                                              \
        return &(entry->value);                                               \
    }                                                                         \
                                                                              \
    bool PFX##_contains(struct SNAME *_map_, K key)                           \
    {                                                                         \
        _map_->flag = cmc_flags.OK;                                           \
                                                                              \
        return PFX##_impl_get_entry(_map_, key) != NULL;                      \
    }                                                                         \
                                                                              \
    bool PFX##_empty(struct SNAME *_map_)                                     \
    {                                                                         \
        return _map_->count == 0;                                             \
    }                                                                         \
                                                                              \
    bool PFX##_full(struct SNAME *_map_)                                      \
    {                                                                         \
        return (double)_map_->capacity * _map_->load <= (double)_map_->count; \
    }                                                                         \
                                                                              \
    size_t PFX##_count(struct SNAME *_map_)                                   \
    {                                                                         \
        return _map_->count;                                                  \
    }                                                                         \
                                                                              \
    size_t PFX##_capacity(struct SNAME *_map_)                                \
    {                                                                         \
        return _map_->capacity;                                               \
    }                                                                         \
                                                                              \
    double PFX##_load(struct SNAME *_map_)                                    \
    {                                                                         \
        return _map_->load;                                                   \
    }                                                                         \
                                                                              \
    int PFX##_flag(struct SNAME *_map_)                                       \
    {                                                                         \
        return _map_->flag;                                                   \
    }                                                                         \
                                                                              \
    bool PFX##_resize(struct SNAME *_map_, size_t capacity)                   \
    {                                                                         \
        _map_->flag = cmc_flags.OK;                                           \
                                                                              \
        if (_map_->capacity == capacity)                                      \
            return true;                                                      \
                                                                              \
        if (_map_->capacity > capacity / _map_->load)                         \
            return true;                                                      \
                                                                              \
        /* Prevent integer overflow */                                        \
        if (capacity >= UINTMAX_MAX * _map_->load)                            \
        {                                                                     \
            _map_->flag = cmc_flags.ERROR;                                    \
            return false;                                                     \
        }                                                                     \
                                                                              \
        /* Calculate required capacity based on the prime numbers */          \
        size_t theoretical_size = PFX##_impl_calculate_size(capacity);        \
                                                                              \
        /* Not possible to shrink with current available prime numbers */     \
        if (theoretical_size < _map_->count / _map_->load)                    \
        {                                                                     \
            _map_->flag = cmc_flags.ERROR;                                    \
            return false;                                                     \
        }                                                                     \
                                                                              \
        struct SNAME *_new_map_ =                                             \
            PFX##_new_custom(capacity, _map_->load, _map_->f_key,             \
                             _map_->f_val, _map_->alloc, _map_->callbacks);   \
                                                                              \
        if (!_new_map_)                                                       \
        {                                                                     \
            _map_->flag = cmc_flags.ALLOC;                                    \
            return false;                                                     \
        }                                                                     \
                                                                              \
        struct SNAME##_iter iter;                                             \
                                                                              \
        /* TODO turn this into a normal loop */                               \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter);           \
             PFX##_iter_next(&iter))                                          \
        {                                                                     \
            K key = PFX##_iter_key(&iter);                                    \
            V value = PFX##_iter_value(&iter);                                \
                                                                              \
            PFX##_insert(_new_map_, key, value);                              \
        }                                                                     \
                                                                              \
        if (_map_->count != _new_map_->count)                                 \
        {                                                                     \
            PFX##_free(_new_map_);                                            \
                                                                              \
            _map_->flag = cmc_flags.ERROR;                                    \
            return false;                                                     \
        }                                                                     \
                                                                              \
        struct SNAME##_entry *tmp_b = _map_->buffer;                          \
        _map_->buffer = _new_map_->buffer;                                    \
        _new_map_->buffer = tmp_b;                                            \
                                                                              \
        size_t tmp_c = _map_->capacity;                                       \
        _map_->capacity = _new_map_->capacity;                                \
        _new_map_->capacity = tmp_c;                                          \
                                                                              \
        PFX##_free(_new_map_);                                                \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    struct SNAME *PFX##_copy_of(struct SNAME *_map_)                          \
    {                                                                         \
        struct SNAME *result = PFX##_new_custom(                              \
            _map_->capacity * _map_->load, _map_->load, _map_->f_key,         \
            _map_->f_val, _map_->alloc, _map_->callbacks);                    \
                                                                              \
        if (!result)                                                          \
        {                                                                     \
            _map_->flag = cmc_flags.ERROR;                                    \
            return NULL;                                                      \
        }                                                                     \
                                                                              \
        if (_map_->f_key->cpy || _map_->f_val->cpy)                           \
        {                                                                     \
            for (size_t i = 0; i < _map_->capacity; i++)                      \
            {                                                                 \
                struct SNAME##_entry *scan = &(_map_->buffer[i]);             \
                                                                              \
                if (scan->state != CMC_ES_EMPTY)                              \
                {                                                             \
                    struct SNAME##_entry *target = &(result->buffer[i]);      \
                                                                              \
                    if (scan->state == CMC_ES_DELETED)                        \
                        target->state = CMC_ES_DELETED;                       \
                    else                                                      \
                    {                                                         \
                        target->state = scan->state;                          \
                        target->dist = scan->dist;                            \
                                                                              \
                        if (_map_->f_key->cpy)                                \
                            target->key = _map_->f_key->cpy(scan->key);       \
                        else                                                  \
                            target->key = scan->key;                          \
                                                                              \
                        if (_map_->f_val->cpy)                                \
                            target->value = _map_->f_val->cpy(scan->value);   \
                        else                                                  \
                            target->value = scan->value;                      \
                    }                                                         \
                }                                                             \
            }                                                                 \
        }                                                                     \
        else                                                                  \
            memcpy(result->buffer, _map_->buffer,                             \
                   sizeof(struct SNAME##_entry) * _map_->capacity);           \
                                                                              \
        result->count = _map_->count;                                         \
                                                                              \
        _map_->flag = cmc_flags.OK;                                           \
                                                                              \
        return result;                                                        \
    }                                                                         \
                                                                              \
    bool PFX##_equals(struct SNAME *_map1_, struct SNAME *_map2_)             \
    {                                                                         \
        _map1_->flag = cmc_flags.OK;                                          \
        _map2_->flag = cmc_flags.OK;                                          \
                                                                              \
        if (_map1_->count != _map2_->count)                                   \
            return false;                                                     \
                                                                              \
        struct SNAME##_iter iter;                                             \
        PFX##_iter_init(&iter, _map1_);                                       \
                                                                              \
        /* TODO optimize this loop */                                         \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter);              \
             PFX##_iter_next(&iter))                                          \
        {                                                                     \
            struct SNAME##_entry *entry =                                     \
                PFX##_impl_get_entry(_map2_, PFX##_iter_key(&iter));          \
                                                                              \
            if (entry == NULL)                                                \
                return false;                                                 \
                                                                              \
            if (_map1_->f_val->cmp(entry->value, PFX##_iter_value(&iter)) !=  \
                0)                                                            \
                return false;                                                 \
        }                                                                     \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    struct cmc_string PFX##_to_string(struct SNAME *_map_)                    \
    {                                                                         \
        struct cmc_string str;                                                \
        struct SNAME *m_ = _map_;                                             \
                                                                              \
        int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_hashmap,       \
                         #SNAME, #K, #V, m_, m_->buffer, m_->capacity,        \
                         m_->count, m_->load, m_->flag, m_->f_key, m_->f_val, \
                         m_->alloc, m_->callbacks);                           \
                                                                              \
        return n >= 0 ? str : (struct cmc_string){ 0 };                       \
    }                                                                         \
                                                                              \
    bool PFX##_print(struct SNAME *_map_, FILE *fptr)                         \
    {                                                                         \
        for (size_t i = 0; i < _map_->capacity; i++)                          \
        {                                                                     \
            struct SNAME##_entry *entry = &(_map_->buffer[i]);                \
                                                                              \
            if (entry->state == CMC_ES_FILLED)                                \
            {                                                                 \
                if (!_map_->f_key->str(fptr, entry->key) ||                   \
                    !_map_->f_val->str(fptr, entry->value))                   \
                    return false;                                             \
            }                                                                 \
        }                                                                     \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                 \
    {                                                                         \
        struct SNAME##_iter *iter =                                           \
            target->alloc->malloc(sizeof(struct SNAME##_iter));               \
                                                                              \
        if (!iter)                                                            \
            return NULL;                                                      \
                                                                              \
        PFX##_iter_init(iter, target);                                        \
                                                                              \
        return iter;                                                          \
    }                                                                         \
                                                                              \
    void PFX##_iter_free(struct SNAME##_iter *iter)                           \
    {                                                                         \
        iter->target->alloc->free(iter);                                      \
    }                                                                         \
                                                                              \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)     \
    {                                                                         \
        memset(iter, 0, sizeof(struct SNAME##_iter));                         \
                                                                              \
        iter->target = target;                                                \
        iter->start = true;                                                   \
        iter->end = PFX##_empty(target);                                      \
                                                                              \
        if (!PFX##_empty(target))                                             \
        {                                                                     \
            for (size_t i = 0; i < target->capacity; i++)                     \
            {                                                                 \
                if (target->buffer[i].state == CMC_ES_FILLED)                 \
                {                                                             \
                    iter->first = i;                                          \
                    break;                                                    \
                }                                                             \
            }                                                                 \
                                                                              \
            iter->cursor = iter->first;                                       \
                                                                              \
            for (size_t i = target->capacity; i > 0; i--)                     \
            {                                                                 \
                if (target->buffer[i - 1].state == CMC_ES_FILLED)             \
                {                                                             \
                    iter->last = i - 1;                                       \
                    break;                                                    \
                }                                                             \
            }                                                                 \
        }                                                                     \
    }                                                                         \
                                                                              \
    bool PFX##_iter_start(struct SNAME##_iter *iter)                          \
    {                                                                         \
        return PFX##_empty(iter->target) || iter->start;                      \
    }                                                                         \
                                                                              \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                            \
    {                                                                         \
        return PFX##_empty(iter->target) || iter->end;                        \
    }                                                                         \
                                                                              \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                       \
    {                                                                         \
        if (!PFX##_empty(iter->target))                                       \
        {                                                                     \
            iter->cursor = iter->first;                                       \
            iter->index = 0;                                                  \
            iter->start = true;                                               \
            iter->end = PFX##_empty(iter->target);                            \
        }                                                                     \
    }                                                                         \
                                                                              \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                         \
    {                                                                         \
        if (!PFX##_empty(iter->target))                                       \
        {                                                                     \
            iter->cursor = iter->last;                                        \
            iter->index = iter->target->count - 1;                            \
            iter->start = PFX##_empty(iter->target);                          \
            iter->end = true;                                                 \
        }                                                                     \
    }                                                                         \
                                                                              \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                           \
    {                                                                         \
        if (iter->end)                                                        \
            return false;                                                     \
                                                                              \
        if (iter->index + 1 == iter->target->count)                           \
        {                                                                     \
            iter->end = true;                                                 \
            return false;                                                     \
        }                                                                     \
                                                                              \
        iter->start = PFX##_empty(iter->target);                              \
                                                                              \
        struct SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);   \
                                                                              \
        iter->index++;                                                        \
                                                                              \
        while (1)                                                             \
        {                                                                     \
            iter->cursor++;                                                   \
            scan = &(iter->target->buffer[iter->cursor]);                     \
                                                                              \
            if (scan->state == CMC_ES_FILLED)                                 \
                break;                                                        \
        }                                                                     \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                           \
    {                                                                         \
        if (iter->start)                                                      \
            return false;                                                     \
                                                                              \
        if (iter->index == 0)                                                 \
        {                                                                     \
            iter->start = true;                                               \
            return false;                                                     \
        }                                                                     \
                                                                              \
        iter->end = PFX##_empty(iter->target);                                \
                                                                              \
        struct SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);   \
                                                                              \
        iter->index--;                                                        \
                                                                              \
        while (1)                                                             \
        {                                                                     \
            iter->cursor--;                                                   \
            scan = &(iter->target->buffer[iter->cursor]);                     \
                                                                              \
            if (scan->state == CMC_ES_FILLED)                                 \
                break;                                                        \
        }                                                                     \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    /* Returns true only if the iterator moved */                             \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)          \
    {                                                                         \
        if (iter->end)                                                        \
            return false;                                                     \
                                                                              \
        if (iter->index + 1 == iter->target->count)                           \
        {                                                                     \
            iter->end = true;                                                 \
            return false;                                                     \
        }                                                                     \
                                                                              \
        if (steps == 0 || iter->index + steps >= iter->target->count)         \
            return false;                                                     \
                                                                              \
        for (size_t i = 0; i < steps; i++)                                    \
            PFX##_iter_next(iter);                                            \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    /* Returns true only if the iterator moved */                             \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps)           \
    {                                                                         \
        if (iter->start)                                                      \
            return false;                                                     \
                                                                              \
        if (iter->index == 0)                                                 \
        {                                                                     \
            iter->start = true;                                               \
            return false;                                                     \
        }                                                                     \
                                                                              \
        if (steps == 0 || iter->index < steps)                                \
            return false;                                                     \
                                                                              \
        for (size_t i = 0; i < steps; i++)                                    \
            PFX##_iter_prev(iter);                                            \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    /* Returns true only if the iterator was able to be positioned at the */  \
    /* given index */                                                         \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)            \
    {                                                                         \
        if (index >= iter->target->count)                                     \
            return false;                                                     \
                                                                              \
        if (iter->index > index)                                              \
            return PFX##_iter_rewind(iter, iter->index - index);              \
        else if (iter->index < index)                                         \
            return PFX##_iter_advance(iter, index - iter->index);             \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    K PFX##_iter_key(struct SNAME##_iter *iter)                               \
    {                                                                         \
        if (PFX##_empty(iter->target))                                        \
            return (K){ 0 };                                                  \
                                                                              \
        return iter->target->buffer[iter->cursor].key;                        \
    }                                                                         \
                                                                              \
    V PFX##_iter_value(struct SNAME##_iter *iter)                             \
    {                                                                         \
        if (PFX##_empty(iter->target))                                        \
            return (V){ 0 };                                                  \
                                                                              \
        return iter->target->buffer[iter->cursor].value;                      \
    }                                                                         \
                                                                              \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter)                           \
    {                                                                         \
        if (PFX##_empty(iter->target))                                        \
            return NULL;                                                      \
                                                                              \
        return &(iter->target->buffer[iter->cursor].value);                   \
    }                                                                         \
                                                                              \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                        \
    {                                                                         \
        return iter->index;                                                   \
    }                                                                         \
                                                                              \
    static struct SNAME##_entry *PFX##_impl_get_entry(struct SNAME *_map_,    \
                                                      K key)                  \
    {                                                                         \
        size_t hash = _map_->f_key->hash(key);                                \
        size_t pos = hash % _map_->capacity;                                  \
                                                                              \
        struct SNAME##_entry *target = &(_map_->buffer[pos]);                 \
                                                                              \
        while (target->state == CMC_ES_FILLED ||                              \
               target->state == CMC_ES_DELETED)                               \
        {                                                                     \
            if (_map_->f_key->cmp(target->key, key) == 0)                     \
                return target;                                                \
                                                                              \
            pos++;                                                            \
            target = &(_map_->buffer[pos % _map_->capacity]);                 \
        }                                                                     \
                                                                              \
        return NULL;                                                          \
    }                                                                         \
                                                                              \
    static size_t PFX##_impl_calculate_size(size_t required)                  \
    {                                                                         \
        const size_t count =                                                  \
            sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);   \
                                                                              \
        if (cmc_hashtable_primes[count - 1] < required)                       \
            return required;                                                  \
                                                                              \
        size_t i = 0;                                                         \
        while (cmc_hashtable_primes[i] < required)                            \
            i++;                                                              \
                                                                              \
        return cmc_hashtable_primes[i];                                       \
    }                                                                         \
                                                                              \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_map_)       \
    {                                                                         \
        struct SNAME##_iter iter;                                             \
                                                                              \
        PFX##_iter_init(&iter, _map_);                                        \
                                                                              \
        return iter;                                                          \
    }                                                                         \
                                                                              \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_map_)         \
    {                                                                         \
        struct SNAME##_iter iter;                                             \
                                                                              \
        PFX##_iter_init(&iter, _map_);                                        \
        PFX##_iter_to_end(&iter);                                             \
                                                                              \
        return iter;                                                          \
    }

#endif /* CMC_HASHMAP_H */
