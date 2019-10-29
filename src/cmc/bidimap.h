/**
 * bidimap.h
 *
 * Creation Date: 26/09/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_BIDIMAP_H
#define CMC_BIDIMAP_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/**
 * BidiMap
 *
 * A bidirectional map is a map that allows you to create a bijection in both
 * directions between two sets of elements (K <-> V).
 *
 * This implementation uses two arrays of pointers to an entry containing both
 * the key and the value.
 */

/* to_string format */
static const char *cmc_string_fmt_bidimap = "%s at %p { key_buffer:%p, val_buffer:%p, capacity:%" PRIuMAX ", count:%" PRIuMAX ", load:%lf, key_cmp:%p, val_cmp:%p, key_hash:%p, val_hash:%p }";

#ifndef CMC_ENTRY_DELETED

#define CMC_ENTRY_DELETED ((void *)1)

#endif /* CMC_ENTRY_DELETED */

#ifndef CMC_IMPL_HASHTABLE_SETUP
#define CMC_IMPL_HASHTABLE_SETUP

static const size_t cmc_hashtable_primes[] = {53, 97, 191, 383, 769, 1531,
                                              3067, 6143, 12289, 24571, 49157,
                                              98299, 196613, 393209, 786431,
                                              1572869, 3145721, 6291449,
                                              12582917, 25165813, 50331653,
                                              100663291, 201326611, 402653189,
                                              805306357, 1610612741,
                                              3221225473, 6442450939,
                                              12884901893, 25769803799,
                                              51539607551, 103079215111,
                                              206158430209, 412316860441,
                                              824633720831, 1649267441651,
                                              3298534883309, 6597069766657,
                                              13194139533299, 26388279066623,
                                              52776558133303, 105553116266489,
                                              211106232532969, 422212465066001,
                                              844424930131963,
                                              1688849860263953,
                                              3377699720527861,
                                              6755399441055731,
                                              13510798882111483,
                                              27021597764222939,
                                              54043195528445957,
                                              108086391056891903,
                                              216172782113783773,
                                              432345564227567621,
                                              864691128455135207,
                                              1729382256910270481,
                                              3458764513820540933,
                                              6917529027641081903,
                                              13835058055282163729llu};

#endif /* CMC_IMPL_HASHTABLE_SETUP */

#define CMC_GENERATE_BIDIMAP(PFX, SNAME, K, V)    \
    CMC_GENERATE_BIDIMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_BIDIMAP_SOURCE(PFX, SNAME, K, V)

#define CMC_WRAPGEN_BIDIMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_BIDIMAP_HEADER(PFX, SNAME, K, V)

#define CMC_WRAPGEN_BIDIMAP_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_BIDIMAP_SOURCE(PFX, SNAME, K, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_BIDIMAP_HEADER(PFX, SNAME, K, V)                       \
                                                                            \
    /* BidiMap Structure */                                                 \
    struct SNAME                                                            \
    {                                                                       \
        /* Array of Entries mapping K -> V */                               \
        struct SNAME##_entry **key_buffer;                                  \
                                                                            \
        /* Array of entries mapping V -> K */                               \
        struct SNAME##_entry **val_buffer;                                  \
                                                                            \
        /* Current arrays capacity */                                       \
        size_t capacity;                                                    \
                                                                            \
        /* Current amount of keys */                                        \
        size_t count;                                                       \
                                                                            \
        /* Load factor in range (0.0, 1.0) */                               \
        double load;                                                        \
                                                                            \
        /* Key comparison function */                                       \
        int (*key_cmp)(K, K);                                               \
                                                                            \
        /* Value comparison function */                                     \
        int (*val_cmp)(V, V);                                               \
                                                                            \
        /* Key hash function */                                             \
        size_t (*key_hash)(K);                                              \
                                                                            \
        /* Value hash function */                                           \
        size_t (*val_hash)(V);                                              \
                                                                            \
        /* Function that returns an iterator to the start of the bidimap */ \
        struct SNAME##_iter (*it_start)(struct SNAME *);                    \
                                                                            \
        /* Function that returns an iterator to the end of the bidimap */   \
        struct SNAME##_iter (*it_end)(struct SNAME *);                      \
    };                                                                      \
                                                                            \
    /* BidiMap Entry */                                                     \
    struct SNAME##_entry                                                    \
    {                                                                       \
        /* Entry Key */                                                     \
        K key;                                                              \
                                                                            \
        /* Entry Value */                                                   \
        V value;                                                            \
                                                                            \
        /* The distance of this node to its original position, used by */   \
        /* robin-hood hashing relative to the key_buffer */                 \
        size_t key_dist;                                                    \
                                                                            \
        /* The distance of this node to its original position, used by */   \
        /* robin-hood hashing relative to the value_buffer */               \
        size_t val_dist;                                                    \
    };                                                                      \
                                                                            \
    /* BidiMap Iterator */                                                  \
    struct SNAME##_iter                                                     \
    {                                                                       \
        /* Target bidimap */                                                \
        struct SNAME *target;                                               \
                                                                            \
        /* Cursor's position (index) */                                     \
        size_t cursor;                                                      \
                                                                            \
        /* Keeps track of relative index to the iteration of elements */    \
        size_t index;                                                       \
                                                                            \
        /* The index of the first element */                                \
        size_t first;                                                       \
                                                                            \
        /* The index of the last element */                                 \
        size_t last;                                                        \
                                                                            \
        /* If the iterator has reached the start of the iteration */        \
        bool start;                                                         \
                                                                            \
        /* If the iterator has reached the end of the iteration */          \
        bool end;                                                           \
    };                                                                      \
                                                                            \
    /* Collection Functions */                                              \
    /* Collection Allocation and Deallocation */                            \
    struct SNAME *PFX##_new(size_t capacity, double load,                   \
                            int (*key_cmp)(K, K), size_t (*key_hash)(K),    \
                            int (*val_cmp)(V, V), size_t (*val_hash)(V));   \
    void PFX##_clear(struct SNAME *_map_, void (*deallocator)(K, V));       \
    void PFX##_free(struct SNAME *_map_, void (*deallocator)(K, V));        \
    /* Collection Input and Output */                                       \
    bool PFX##_insert(struct SNAME *_map_, K key, V value);                 \
    bool PFX##_update_key(struct SNAME *_map_, K key, K *old_key);          \
    bool PFX##_update_val(struct SNAME *_map_, V val, V *old_val);          \
    bool PFX##_remove_by_key(struct SNAME *_map_, K key, V *out_value);     \
    bool PFX##_remove_by_val(struct SNAME *_map_, V value, K *out_key);     \
    /* Element Access */                                                    \
    K PFX##_get_key(struct SNAME *_map_, V val);                            \
    V PFX##_get_val(struct SNAME *_map_, K key);                            \
    /* Collection State */                                                  \
    bool PFX##_contains_key(struct SNAME *_map_, K key);                    \
    bool PFX##_contains_val(struct SNAME *_map_, V val);                    \
    bool PFX##_empty(struct SNAME *_map_);                                  \
    bool PFX##_full(struct SNAME *_map_);                                   \
    size_t PFX##_count(struct SNAME *_map_);                                \
    size_t PFX##_capacity(struct SNAME *_map_);                             \
    double PFX##_load(struct SNAME *_map_);                                 \
    /* Collection Utility */                                                \
    bool PFX##_resize(struct SNAME *_map_, size_t capacity);                \
    struct SNAME *PFX##_copy_of(struct SNAME *_map_, K (*key_copy_func)(K), \
                                V (*value_copy_func)(V));                   \
    bool PFX##_equals(struct SNAME *_map1_, struct SNAME *_map2_);          \
    struct cmc_string PFX##_to_string(struct SNAME *_map_);                 \
                                                                            \
    /* Iterator Functions */                                                \
    /* Iterator Allocation and Deallocation */                              \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);              \
    void PFX##_iter_free(struct SNAME##_iter *iter);                        \
    /* Iterator Initialization */                                           \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);  \
    /* Iterator State */                                                    \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                       \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                         \
    /* Iterator Movement */                                                 \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                    \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                      \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                        \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                        \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);       \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);        \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);         \
    /* Iterator Access */                                                   \
    K PFX##_iter_key(struct SNAME##_iter *iter);                            \
    V PFX##_iter_value(struct SNAME##_iter *iter);                          \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);                     \
                                                                            \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_BIDIMAP_SOURCE(PFX, SNAME, K, V)                                            \
                                                                                                 \
    /* Implementation Detail Functions */                                                        \
    static struct SNAME##_entry *PFX##_impl_new_entry(K key, V value);                           \
    static struct SNAME##_entry **PFX##_impl_get_entry_by_key(struct SNAME *_map_, K key);       \
    static struct SNAME##_entry **PFX##_impl_get_entry_by_val(struct SNAME *_map_, V val);       \
    static struct SNAME##_entry **PFX##_impl_add_entry_to_key(struct SNAME *_map_,               \
                                                              struct SNAME##_entry *entry);      \
    static struct SNAME##_entry **PFX##_impl_add_entry_to_val(struct SNAME *_map_,               \
                                                              struct SNAME##_entry *entry);      \
    static size_t PFX##_impl_calculate_size(size_t required);                                    \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_map_);                         \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_map_);                           \
                                                                                                 \
    struct SNAME *PFX##_new(size_t capacity, double load,                                        \
                            int (*key_cmp)(K, K), size_t (*key_hash)(K),                         \
                            int (*val_cmp)(V, V), size_t (*val_hash)(V))                         \
    {                                                                                            \
        if (capacity == 0 || load <= 0 || load >= 1)                                             \
            return NULL;                                                                         \
                                                                                                 \
        /* Prevent integer overflow */                                                           \
        if (capacity >= UINTMAX_MAX * load)                                                      \
            return NULL;                                                                         \
                                                                                                 \
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);                       \
                                                                                                 \
        struct SNAME *_map_ = malloc(sizeof(struct SNAME));                                      \
                                                                                                 \
        if (!_map_)                                                                              \
            return NULL;                                                                         \
                                                                                                 \
        _map_->key_buffer = calloc(real_capacity, sizeof(struct SNAME##_entry *));               \
        _map_->val_buffer = calloc(real_capacity, sizeof(struct SNAME##_entry *));               \
                                                                                                 \
        if (!_map_->key_buffer || !_map_->val_buffer)                                            \
        {                                                                                        \
            free(_map_->key_buffer);                                                             \
            free(_map_->val_buffer);                                                             \
            free(_map_);                                                                         \
            return NULL;                                                                         \
        }                                                                                        \
                                                                                                 \
        _map_->count = 0;                                                                        \
        _map_->capacity = real_capacity;                                                         \
        _map_->load = load;                                                                      \
        _map_->key_cmp = key_cmp;                                                                \
        _map_->val_cmp = val_cmp;                                                                \
        _map_->key_hash = key_hash;                                                              \
        _map_->val_hash = val_hash;                                                              \
                                                                                                 \
        _map_->it_start = PFX##_impl_it_start;                                                   \
        _map_->it_end = PFX##_impl_it_end;                                                       \
                                                                                                 \
        return _map_;                                                                            \
    }                                                                                            \
                                                                                                 \
    void PFX##_clear(struct SNAME *_map_, void (*deallocator)(K, V))                             \
    {                                                                                            \
        for (size_t i = 0; i < _map_->capacity; i++)                                             \
        {                                                                                        \
            struct SNAME##_entry *entry = _map_->key_buffer[i];                                  \
                                                                                                 \
            if (entry != NULL && entry != CMC_ENTRY_DELETED)                                     \
            {                                                                                    \
                if (deallocator)                                                                 \
                    deallocator(entry->key, entry->value);                                       \
                                                                                                 \
                free(entry);                                                                     \
            }                                                                                    \
                                                                                                 \
            _map_->key_buffer[i] = NULL;                                                         \
            _map_->val_buffer[i] = NULL;                                                         \
        }                                                                                        \
                                                                                                 \
        _map_->count = 0;                                                                        \
    }                                                                                            \
                                                                                                 \
    void PFX##_free(struct SNAME *_map_, void (*deallocator)(K, V))                              \
    {                                                                                            \
        PFX##_clear(_map_, deallocator);                                                         \
                                                                                                 \
        free(_map_->key_buffer);                                                                 \
        free(_map_->val_buffer);                                                                 \
        free(_map_);                                                                             \
    }                                                                                            \
                                                                                                 \
    bool PFX##_insert(struct SNAME *_map_, K key, V value)                                       \
    {                                                                                            \
        if (PFX##_full(_map_))                                                                   \
        {                                                                                        \
            if (!PFX##_resize(_map_, PFX##_capacity(_map_) + 1))                                 \
                return false;                                                                    \
        }                                                                                        \
                                                                                                 \
        if (PFX##_impl_get_entry_by_key(_map_, key) != NULL ||                                   \
            PFX##_impl_get_entry_by_val(_map_, value) != NULL)                                   \
            return false;                                                                        \
                                                                                                 \
        struct SNAME##_entry *entry = PFX##_impl_new_entry(key, value);                          \
                                                                                                 \
        struct SNAME##_entry **key_entry = PFX##_impl_add_entry_to_key(_map_, entry);            \
        struct SNAME##_entry **val_entry = PFX##_impl_add_entry_to_val(_map_, entry);            \
                                                                                                 \
        if (!key_entry || !val_entry)                                                            \
        {                                                                                        \
            if (key_entry)                                                                       \
                *key_entry = CMC_ENTRY_DELETED;                                                  \
            if (val_entry)                                                                       \
                *val_entry = CMC_ENTRY_DELETED;                                                  \
                                                                                                 \
            free(entry);                                                                         \
                                                                                                 \
            return false;                                                                        \
        }                                                                                        \
                                                                                                 \
        _map_->count++;                                                                          \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    bool PFX##_update_key(struct SNAME *_map_, K key, K *old_key)                                \
    {                                                                                            \
        if (PFX##_empty(_map_))                                                                  \
            return false;                                                                        \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    bool PFX##_update_val(struct SNAME *_map_, V val, V *old_val)                                \
    {                                                                                            \
        if (PFX##_empty(_map_))                                                                  \
            return false;                                                                        \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    bool PFX##_remove_by_key(struct SNAME *_map_, K key, V *out_value)                           \
    {                                                                                            \
        if (PFX##_empty(_map_))                                                                  \
            return false;                                                                        \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    bool PFX##_remove_by_val(struct SNAME *_map_, V value, K *out_key)                           \
    {                                                                                            \
        if (PFX##_empty(_map_))                                                                  \
            return false;                                                                        \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    K PFX##_get_key(struct SNAME *_map_, V val)                                                  \
    {                                                                                            \
        struct SNAME##_entry **entry = PFX##_impl_get_entry_by_val(_map_, val);                  \
                                                                                                 \
        if (!entry)                                                                              \
            return (V){0};                                                                       \
                                                                                                 \
        return (*entry)->key;                                                                    \
    }                                                                                            \
                                                                                                 \
    V PFX##_get_val(struct SNAME *_map_, K key)                                                  \
    {                                                                                            \
        struct SNAME##_entry **entry = PFX##_impl_get_entry_by_key(_map_, key);                  \
                                                                                                 \
        if (!entry)                                                                              \
            return (V){0};                                                                       \
                                                                                                 \
        return (*entry)->value;                                                                  \
    }                                                                                            \
                                                                                                 \
    bool PFX##_contains_key(struct SNAME *_map_, K key)                                          \
    {                                                                                            \
        return PFX##_impl_get_entry_by_key(_map_, key) != NULL;                                  \
    }                                                                                            \
                                                                                                 \
    bool PFX##_contains_val(struct SNAME *_map_, V val)                                          \
    {                                                                                            \
        return PFX##_impl_get_entry_by_val(_map_, val) != NULL;                                  \
    }                                                                                            \
                                                                                                 \
    bool PFX##_empty(struct SNAME *_map_)                                                        \
    {                                                                                            \
        return _map_->count == 0;                                                                \
    }                                                                                            \
                                                                                                 \
    bool PFX##_full(struct SNAME *_map_)                                                         \
    {                                                                                            \
        return (double)PFX##_capacity(_map_) * PFX##_load(_map_) <= (double)PFX##_count(_map_);  \
    }                                                                                            \
                                                                                                 \
    size_t PFX##_count(struct SNAME *_map_)                                                      \
    {                                                                                            \
        return _map_->count;                                                                     \
    }                                                                                            \
                                                                                                 \
    size_t PFX##_capacity(struct SNAME *_map_)                                                   \
    {                                                                                            \
        return _map_->capacity;                                                                  \
    }                                                                                            \
                                                                                                 \
    double PFX##_load(struct SNAME *_map_)                                                       \
    {                                                                                            \
        return _map_->load;                                                                      \
    }                                                                                            \
                                                                                                 \
    bool PFX##_resize(struct SNAME *_map_, size_t capacity)                                      \
    {                                                                                            \
        if (PFX##_capacity(_map_) == capacity)                                                   \
            return true;                                                                         \
                                                                                                 \
        if (PFX##_capacity(_map_) > capacity / PFX##_load(_map_))                                \
            return true;                                                                         \
                                                                                                 \
        /* Prevent integer overflow */                                                           \
        if (capacity >= UINTMAX_MAX * PFX##_load(_map_))                                         \
            return false;                                                                        \
                                                                                                 \
        /* Calculate required capacity based on the prime numbers */                             \
        size_t new_cap = PFX##_impl_calculate_size(capacity);                                    \
                                                                                                 \
        /* Not possible to shrink with current available prime numbers */                        \
        if (new_cap < PFX##_count(_map_) / PFX##_load(_map_))                                    \
            return false;                                                                        \
                                                                                                 \
        struct SNAME *_new_map_ = PFX##_new(capacity, PFX##_load(_map_),                         \
                                            _map_->key_cmp, _map_->key_hash,                     \
                                            _map_->val_cmp, _map_->val_hash);                    \
                                                                                                 \
        if (!_new_map_)                                                                          \
            return false;                                                                        \
                                                                                                 \
        for (size_t i = 0; i < _map_->capacity; i++)                                             \
        {                                                                                        \
            struct SNAME##_entry *scan = _map_->key_buffer[i];                                   \
                                                                                                 \
            if (scan && scan != CMC_ENTRY_DELETED)                                               \
            {                                                                                    \
                struct SNAME##_entry **e1 = PFX##_impl_add_entry_to_key(_new_map_, scan);        \
                struct SNAME##_entry **e2 = PFX##_impl_add_entry_to_val(_new_map_, scan);        \
                                                                                                 \
                if (!e1 || !e2)                                                                  \
                {                                                                                \
                    free(_new_map_->key_buffer);                                                 \
                    free(_new_map_->val_buffer);                                                 \
                    free(_new_map_);                                                             \
                                                                                                 \
                    return false;                                                                \
                }                                                                                \
                                                                                                 \
                _new_map_->count++;                                                              \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        if (PFX##_count(_map_) != PFX##_count(_new_map_))                                        \
        {                                                                                        \
            free(_new_map_->key_buffer);                                                         \
            free(_new_map_->val_buffer);                                                         \
            free(_new_map_);                                                                     \
                                                                                                 \
            return false;                                                                        \
        }                                                                                        \
                                                                                                 \
        struct SNAME##_entry **tmp_key_buf = _map_->key_buffer;                                  \
        struct SNAME##_entry **tmp_val_buf = _map_->val_buffer;                                  \
                                                                                                 \
        _map_->key_buffer = _new_map_->key_buffer;                                               \
        _map_->val_buffer = _new_map_->val_buffer;                                               \
        _map_->capacity = _new_map_->capacity;                                                   \
                                                                                                 \
        free(tmp_key_buf);                                                                       \
        free(tmp_val_buf);                                                                       \
        free(_new_map_);                                                                         \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    struct SNAME *PFX##_copy_of(struct SNAME *_map_, K (*key_copy_func)(K),                      \
                                V (*value_copy_func)(V))                                         \
    {                                                                                            \
        struct SNAME *result = PFX##_new(PFX##_capacity(_map_), PFX##_load(_map_),               \
                                         _map_->key_cmp, _map_->key_hash,                        \
                                         _map_->val_cmp, _map_->val_hash);                       \
                                                                                                 \
        for (size_t i = 0; i < _map_->capacity; i++)                                             \
        {                                                                                        \
            struct SNAME##_entry *scan = _map_->key_buffer[i];                                   \
                                                                                                 \
            if (scan && scan != CMC_ENTRY_DELETED)                                               \
            {                                                                                    \
                K tmp_key;                                                                       \
                V tmp_val;                                                                       \
                                                                                                 \
                if (key_copy_func)                                                               \
                    tmp_key = key_copy_func(scan->key);                                          \
                else                                                                             \
                    tmp_key = scan->key;                                                         \
                                                                                                 \
                if (value_copy_func)                                                             \
                    tmp_val = value_copy_func(scan->value);                                      \
                else                                                                             \
                    tmp_val = scan->value;                                                       \
                                                                                                 \
                PFX##_insert(result, tmp_key, tmp_val);                                          \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        return result;                                                                           \
    }                                                                                            \
                                                                                                 \
    bool PFX##_equals(struct SNAME *_map1_, struct SNAME *_map2_)                                \
    {                                                                                            \
        if (PFX##_count(_map1_) != PFX##_count(_map2_))                                          \
            return false;                                                                        \
                                                                                                 \
        struct SNAME *_mapA_;                                                                    \
        struct SNAME *_mapB_;                                                                    \
                                                                                                 \
        if (PFX##_capacity(_map1_) < PFX##_capacity(_map2_))                                     \
        {                                                                                        \
            _mapA_ = _map1_;                                                                     \
            _mapB_ = _map2_;                                                                     \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            _mapA_ = _map2_;                                                                     \
            _mapB_ = _map1_;                                                                     \
        }                                                                                        \
                                                                                                 \
        for (size_t i = 0; i < _mapA_->capacity; i++)                                            \
        {                                                                                        \
            struct SNAME##_entry *scan = _mapA_->key_buffer[i];                                  \
                                                                                                 \
            if (scan && scan != CMC_ENTRY_DELETED)                                               \
            {                                                                                    \
                struct SNAME##_entry **entry_B = PFX##_impl_get_entry_by_key(_mapB_, scan->key); \
                                                                                                 \
                if (!entry_B)                                                                    \
                    return false;                                                                \
                                                                                                 \
                if (_mapA_->val_cmp((*entry_B)->value, scan->value) != 0)                        \
                    return false;                                                                \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    struct cmc_string PFX##_to_string(struct SNAME *_map_)                                       \
    {                                                                                            \
        struct cmc_string str;                                                                   \
        struct SNAME *m_ = _map_;                                                                \
        const char *name = #SNAME;                                                               \
                                                                                                 \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_bidimap,                                  \
                 name, m_, m_->key_buffer, m_->val_buffer, m_->capacity, m_->count,              \
                 m_->load, m_->key_cmp, m_->val_cmp, m_->key_hash, m_->val_hash);                \
                                                                                                 \
        return str;                                                                              \
    }                                                                                            \
                                                                                                 \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                                    \
    {                                                                                            \
        struct SNAME##_iter *iter = malloc(sizeof(struct SNAME##_iter));                         \
                                                                                                 \
        if (!iter)                                                                               \
            return NULL;                                                                         \
                                                                                                 \
        PFX##_iter_init(iter, target);                                                           \
                                                                                                 \
        return iter;                                                                             \
    }                                                                                            \
                                                                                                 \
    void PFX##_iter_free(struct SNAME##_iter *iter)                                              \
    {                                                                                            \
        free(iter);                                                                              \
    }                                                                                            \
                                                                                                 \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)                        \
    {                                                                                            \
        memset(iter, 0, sizeof(struct SNAME##_iter));                                            \
                                                                                                 \
        iter->target = target;                                                                   \
        iter->start = true;                                                                      \
        iter->end = PFX##_empty(target);                                                         \
                                                                                                 \
        if (!PFX##_empty(target))                                                                \
        {                                                                                        \
            for (size_t i = 0; i < target->capacity; i++)                                        \
            {                                                                                    \
                struct SNAME##_entry *tmp = target->key_buffer[i];                               \
                                                                                                 \
                if (tmp != NULL && tmp != CMC_ENTRY_DELETED)                                     \
                {                                                                                \
                    iter->first = i;                                                             \
                    break;                                                                       \
                }                                                                                \
            }                                                                                    \
                                                                                                 \
            iter->cursor = iter->first;                                                          \
                                                                                                 \
            for (size_t i = target->capacity; i > 0; i--)                                        \
            {                                                                                    \
                struct SNAME##_entry *tmp = target->key_buffer[i - 1];                           \
                                                                                                 \
                if (tmp != NULL && tmp != CMC_ENTRY_DELETED)                                     \
                {                                                                                \
                    iter->last = i - 1;                                                          \
                    break;                                                                       \
                }                                                                                \
            }                                                                                    \
        }                                                                                        \
    }                                                                                            \
                                                                                                 \
    bool PFX##_iter_start(struct SNAME##_iter *iter)                                             \
    {                                                                                            \
        return PFX##_empty(iter->target) || iter->start;                                         \
    }                                                                                            \
                                                                                                 \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                                               \
    {                                                                                            \
        return PFX##_empty(iter->target) || iter->end;                                           \
    }                                                                                            \
                                                                                                 \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                                          \
    {                                                                                            \
        if (!PFX##_empty(iter->target))                                                          \
        {                                                                                        \
            iter->cursor = iter->first;                                                          \
            iter->index = 0;                                                                     \
            iter->start = true;                                                                  \
            iter->end = false;                                                                   \
        }                                                                                        \
    }                                                                                            \
                                                                                                 \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                                            \
    {                                                                                            \
        if (!PFX##_empty(iter->target))                                                          \
        {                                                                                        \
            iter->cursor = iter->last;                                                           \
            iter->index = PFX##_count(iter->target) - 1;                                         \
            iter->start = false;                                                                 \
            iter->end = true;                                                                    \
        }                                                                                        \
    }                                                                                            \
                                                                                                 \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                                              \
    {                                                                                            \
        if (iter->end)                                                                           \
            return false;                                                                        \
                                                                                                 \
        if (iter->index + 1 == PFX##_count(iter->target))                                        \
        {                                                                                        \
            iter->end = true;                                                                    \
            return false;                                                                        \
        }                                                                                        \
                                                                                                 \
        iter->start = PFX##_empty(iter->target);                                                 \
                                                                                                 \
        struct SNAME##_entry *scan = iter->target->key_buffer[iter->cursor];                     \
                                                                                                 \
        iter->index++;                                                                           \
                                                                                                 \
        while (1)                                                                                \
        {                                                                                        \
            iter->cursor++;                                                                      \
            scan = iter->target->key_buffer[iter->cursor];                                       \
                                                                                                 \
            if (scan != NULL && scan != CMC_ENTRY_DELETED)                                       \
                break;                                                                           \
        }                                                                                        \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                                              \
    {                                                                                            \
        if (iter->start)                                                                         \
            return false;                                                                        \
                                                                                                 \
        if (iter->index == 0)                                                                    \
        {                                                                                        \
            iter->start = true;                                                                  \
            return false;                                                                        \
        }                                                                                        \
                                                                                                 \
        iter->end = PFX##_empty(iter->target);                                                   \
                                                                                                 \
        struct SNAME##_entry *scan = iter->target->key_buffer[iter->cursor];                     \
                                                                                                 \
        iter->index--;                                                                           \
                                                                                                 \
        while (1)                                                                                \
        {                                                                                        \
            iter->cursor--;                                                                      \
            scan = iter->target->key_buffer[iter->cursor];                                       \
                                                                                                 \
            if (scan != NULL && scan != CMC_ENTRY_DELETED)                                       \
                break;                                                                           \
        }                                                                                        \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    /* Returns true only if the iterator moved */                                                \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)                             \
    {                                                                                            \
        if (iter->end)                                                                           \
            return false;                                                                        \
                                                                                                 \
        if (iter->index + 1 == PFX##_count(iter->target))                                        \
        {                                                                                        \
            iter->end = true;                                                                    \
            return false;                                                                        \
        }                                                                                        \
                                                                                                 \
        if (steps == 0 || iter->index + steps >= PFX##_count(iter->target))                      \
            return false;                                                                        \
                                                                                                 \
        for (size_t i = 0; i < steps; i++)                                                       \
            PFX##_iter_next(iter);                                                               \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    /* Returns true only if the iterator moved */                                                \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps)                              \
    {                                                                                            \
        if (iter->start)                                                                         \
            return false;                                                                        \
                                                                                                 \
        if (iter->index == 0)                                                                    \
        {                                                                                        \
            iter->start = true;                                                                  \
            return false;                                                                        \
        }                                                                                        \
                                                                                                 \
        if (steps == 0 || iter->index < steps)                                                   \
            return false;                                                                        \
                                                                                                 \
        for (size_t i = 0; i < steps; i++)                                                       \
            PFX##_iter_prev(iter);                                                               \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    /* Returns true only if the iterator was able to be positioned at the given index */         \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)                               \
    {                                                                                            \
        if (index >= PFX##_count(iter->target))                                                  \
            return false;                                                                        \
                                                                                                 \
        if (iter->index > index)                                                                 \
            return PFX##_iter_rewind(iter, iter->index - index);                                 \
        else if (iter->index < index)                                                            \
            return PFX##_iter_advance(iter, index - iter->index);                                \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    K PFX##_iter_key(struct SNAME##_iter *iter)                                                  \
    {                                                                                            \
        if (PFX##_empty(iter->target))                                                           \
            return (K){0};                                                                       \
                                                                                                 \
        return iter->target->key_buffer[iter->cursor]->key;                                      \
    }                                                                                            \
                                                                                                 \
    V PFX##_iter_value(struct SNAME##_iter *iter)                                                \
    {                                                                                            \
        if (PFX##_empty(iter->target))                                                           \
            return (V){0};                                                                       \
                                                                                                 \
        return iter->target->key_buffer[iter->cursor]->value;                                    \
    }                                                                                            \
                                                                                                 \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                                           \
    {                                                                                            \
        return iter->index;                                                                      \
    }                                                                                            \
                                                                                                 \
    static struct SNAME##_entry *PFX##_impl_new_entry(K key, V value)                            \
    {                                                                                            \
        struct SNAME##_entry *entry = malloc(sizeof(struct SNAME##_entry));                      \
                                                                                                 \
        if (!entry)                                                                              \
            return NULL;                                                                         \
                                                                                                 \
        entry->key = key;                                                                        \
        entry->value = value;                                                                    \
        entry->key_dist = 0;                                                                     \
        entry->val_dist = 0;                                                                     \
                                                                                                 \
        return entry;                                                                            \
    }                                                                                            \
                                                                                                 \
    static struct SNAME##_entry **PFX##_impl_get_entry_by_key(struct SNAME *_map_, K key)        \
    {                                                                                            \
        size_t hash = _map_->key_hash(key);                                                      \
        size_t pos = hash % _map_->capacity;                                                     \
                                                                                                 \
        struct SNAME##_entry *target = _map_->key_buffer[pos];                                   \
                                                                                                 \
        while (target != NULL)                                                                   \
        {                                                                                        \
            if (target != CMC_ENTRY_DELETED && _map_->key_cmp(target->key, key) == 0)            \
                return &(_map_->key_buffer[pos % _map_->capacity]);                              \
                                                                                                 \
            pos++;                                                                               \
            target = _map_->key_buffer[pos % _map_->capacity];                                   \
        }                                                                                        \
                                                                                                 \
        return NULL;                                                                             \
    }                                                                                            \
                                                                                                 \
    static struct SNAME##_entry **PFX##_impl_get_entry_by_val(struct SNAME *_map_, V val)        \
    {                                                                                            \
        size_t hash = _map_->val_hash(val);                                                      \
        size_t pos = hash % _map_->capacity;                                                     \
                                                                                                 \
        struct SNAME##_entry *target = _map_->val_buffer[pos];                                   \
                                                                                                 \
        while (target != NULL)                                                                   \
        {                                                                                        \
            if (target != CMC_ENTRY_DELETED && _map_->val_cmp(target->value, val) == 0)          \
                return &(_map_->val_buffer[pos % _map_->capacity]);                              \
                                                                                                 \
            pos++;                                                                               \
            target = _map_->val_buffer[pos % _map_->capacity];                                   \
        }                                                                                        \
                                                                                                 \
        return NULL;                                                                             \
    }                                                                                            \
                                                                                                 \
    static struct SNAME##_entry **PFX##_impl_add_entry_to_key(struct SNAME *_map_,               \
                                                              struct SNAME##_entry *entry)       \
    {                                                                                            \
        struct SNAME##_entry **to_return = NULL;                                                 \
                                                                                                 \
        size_t hash = _map_->key_hash(entry->key);                                               \
        size_t original_pos = hash % _map_->capacity;                                            \
        size_t pos = original_pos;                                                               \
                                                                                                 \
        struct SNAME##_entry **scan = &(_map_->key_buffer[hash % _map_->capacity]);              \
                                                                                                 \
        if (*scan == NULL)                                                                       \
        {                                                                                        \
            *scan = entry;                                                                       \
                                                                                                 \
            return scan;                                                                         \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            while (true)                                                                         \
            {                                                                                    \
                pos++;                                                                           \
                scan = &(_map_->key_buffer[pos % _map_->capacity]);                              \
                                                                                                 \
                if (*scan == NULL || *scan == CMC_ENTRY_DELETED)                                 \
                {                                                                                \
                    if (!to_return)                                                              \
                        to_return = scan;                                                        \
                                                                                                 \
                    *scan = entry;                                                               \
                    entry->key_dist = pos - original_pos;                                        \
                                                                                                 \
                    return to_return;                                                            \
                }                                                                                \
                else if ((*scan)->key_dist < pos - original_pos)                                 \
                {                                                                                \
                    if (!to_return)                                                              \
                        to_return = scan;                                                        \
                                                                                                 \
                    size_t tmp_dist = (*scan)->key_dist;                                         \
                    entry->key_dist = pos - original_pos;                                        \
                    original_pos = pos - tmp_dist;                                               \
                                                                                                 \
                    struct SNAME##_entry *_tmp_ = *scan;                                         \
                    *scan = entry;                                                               \
                    entry = _tmp_;                                                               \
                }                                                                                \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        return NULL;                                                                             \
    }                                                                                            \
                                                                                                 \
    static struct SNAME##_entry **PFX##_impl_add_entry_to_val(struct SNAME *_map_,               \
                                                              struct SNAME##_entry *entry)       \
    {                                                                                            \
        struct SNAME##_entry **to_return = NULL;                                                 \
                                                                                                 \
        size_t hash = _map_->val_hash(entry->value);                                             \
        size_t original_pos = hash % _map_->capacity;                                            \
        size_t pos = original_pos;                                                               \
                                                                                                 \
        struct SNAME##_entry **scan = &(_map_->val_buffer[hash % _map_->capacity]);              \
                                                                                                 \
        if (*scan == NULL)                                                                       \
        {                                                                                        \
            *scan = entry;                                                                       \
                                                                                                 \
            return scan;                                                                         \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            while (true)                                                                         \
            {                                                                                    \
                pos++;                                                                           \
                scan = &(_map_->val_buffer[pos % _map_->capacity]);                              \
                                                                                                 \
                if (*scan == NULL || *scan == CMC_ENTRY_DELETED)                                 \
                {                                                                                \
                    if (!to_return)                                                              \
                        to_return = scan;                                                        \
                                                                                                 \
                    *scan = entry;                                                               \
                    entry->val_dist = pos - original_pos;                                        \
                                                                                                 \
                    return to_return;                                                            \
                }                                                                                \
                else if ((*scan)->val_dist < pos - original_pos)                                 \
                {                                                                                \
                    if (!to_return)                                                              \
                        to_return = scan;                                                        \
                                                                                                 \
                    size_t tmp_dist = (*scan)->val_dist;                                         \
                    entry->val_dist = pos - original_pos;                                        \
                    original_pos = pos - tmp_dist;                                               \
                                                                                                 \
                    struct SNAME##_entry *_tmp_ = *scan;                                         \
                    *scan = entry;                                                               \
                    entry = _tmp_;                                                               \
                }                                                                                \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        return NULL;                                                                             \
    }                                                                                            \
                                                                                                 \
    static size_t PFX##_impl_calculate_size(size_t required)                                     \
    {                                                                                            \
        const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);     \
                                                                                                 \
        if (cmc_hashtable_primes[count - 1] < required)                                          \
            return required;                                                                     \
                                                                                                 \
        size_t i = 0;                                                                            \
        while (cmc_hashtable_primes[i] < required)                                               \
            i++;                                                                                 \
                                                                                                 \
        return cmc_hashtable_primes[i];                                                          \
    }                                                                                            \
                                                                                                 \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_map_)                          \
    {                                                                                            \
        struct SNAME##_iter iter;                                                                \
                                                                                                 \
        PFX##_iter_init(&iter, _map_);                                                           \
                                                                                                 \
        return iter;                                                                             \
    }                                                                                            \
                                                                                                 \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_map_)                            \
    {                                                                                            \
        struct SNAME##_iter iter;                                                                \
                                                                                                 \
        PFX##_iter_init(&iter, _map_);                                                           \
        PFX##_iter_to_end(&iter);                                                                \
                                                                                                 \
        return iter;                                                                             \
    }

#endif /* CMC_BIDIMAP_H */
