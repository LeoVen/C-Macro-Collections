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

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_multimap = "%s at %p { buffer:%p, capacity:%" PRIuMAX ", count:%" PRIuMAX ", load:%lf, cmp:%p, hash:%p }";

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

#define CMC_GENERATE_MULTIMAP(PFX, SNAME, K, V)    \
    CMC_GENERATE_MULTIMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_MULTIMAP_SOURCE(PFX, SNAME, K, V)

#define CMC_WRAPGEN_MULTIMAP_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_MULTIMAP_HEADER(PFX, SNAME, K, V)

#define CMC_WRAPGEN_MULTIMAP_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_MULTIMAP_SOURCE(PFX, SNAME, K, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_MULTIMAP_HEADER(PFX, SNAME, K, V)                                                \
                                                                                                      \
    /* Multimap Structure */                                                                          \
    struct SNAME                                                                                      \
    {                                                                                                 \
        /* Array of linked list to entries */                                                         \
        struct SNAME##_entry *(*buffer)[2];                                                           \
                                                                                                      \
        /* Current array capacity */                                                                  \
        size_t capacity;                                                                              \
                                                                                                      \
        /* Current amount of keys */                                                                  \
        size_t count;                                                                                 \
                                                                                                      \
        /* Load factor in range (0.0, infinity) */                                                    \
        double load;                                                                                  \
                                                                                                      \
        /* Key comparison functions */                                                                \
        int (*cmp)(K, K);                                                                             \
                                                                                                      \
        /* Key hash function */                                                                       \
        size_t (*hash)(K);                                                                            \
                                                                                                      \
        /* Function that returns an iterator to the start of the multimap */                          \
        struct SNAME##_iter (*it_start)(struct SNAME *);                                              \
                                                                                                      \
        /* Function that returns an iterator to the end of the multimap */                            \
        struct SNAME##_iter (*it_end)(struct SNAME *);                                                \
    };                                                                                                \
                                                                                                      \
    /* Multimap Entry */                                                                              \
    struct SNAME##_entry                                                                              \
    {                                                                                                 \
        /* Entry Key */                                                                               \
        K key;                                                                                        \
                                                                                                      \
        /* Entry Value */                                                                             \
        V value;                                                                                      \
                                                                                                      \
        /* Next entry on the linked list */                                                           \
        struct SNAME##_entry *next;                                                                   \
                                                                                                      \
        /* Previous entry on the linked list */                                                       \
        struct SNAME##_entry *prev;                                                                   \
    };                                                                                                \
                                                                                                      \
    struct SNAME##_iter                                                                               \
    {                                                                                                 \
        /* Target multimap */                                                                         \
        struct SNAME *target;                                                                         \
                                                                                                      \
        /* Current entry */                                                                           \
        struct SNAME##_entry *curr_entry;                                                             \
                                                                                                      \
        /* Cursor`s position (index) */                                                               \
        size_t cursor;                                                                                \
                                                                                                      \
        /* Keeps track of relative index to the iteration of elements */                              \
        size_t index;                                                                                 \
                                                                                                      \
        /* The index of the first element */                                                          \
        size_t first;                                                                                 \
                                                                                                      \
        /* The index of the last element */                                                           \
        size_t last;                                                                                  \
                                                                                                      \
        /* If the iterator has reached the start of the iteration */                                  \
        bool start;                                                                                   \
                                                                                                      \
        /* If the iterator has reached the end of the iteration */                                    \
        bool end;                                                                                     \
    };                                                                                                \
                                                                                                      \
    /* Collection Functions */                                                                        \
    /* Collection Allocation and Deallocation */                                                      \
    struct SNAME *PFX##_new(size_t capacity, double load, int (*compare)(K, K), size_t (*hash)(K));   \
    void PFX##_clear(struct SNAME *_map_, void (*deallocator)(K, V));                                 \
    void PFX##_free(struct SNAME *_map_, void (*deallocator)(K, V));                                  \
    /* Collection Input and Output */                                                                 \
    bool PFX##_insert(struct SNAME *_map_, K key, V value);                                           \
    bool PFX##_update(struct SNAME *_map_, K key, V new_value, V *old_value);                         \
    size_t PFX##_update_all(struct SNAME *_map_, K key, V new_value, V **old_values);                 \
    bool PFX##_remove(struct SNAME *_map_, K key, V *out_value);                                      \
    size_t PFX##_remove_all(struct SNAME *_map_, K key, V **out_values);                              \
    /* Element Access */                                                                              \
    bool PFX##_max(struct SNAME *_map_, K *key, V *value);                                            \
    bool PFX##_min(struct SNAME *_map_, K *key, V *value);                                            \
    V PFX##_get(struct SNAME *_map_, K key);                                                          \
    V *PFX##_get_ref(struct SNAME *_map_, K key);                                                     \
    /* Collection State */                                                                            \
    bool PFX##_contains(struct SNAME *_map_, K key);                                                  \
    bool PFX##_empty(struct SNAME *_map_);                                                            \
    bool PFX##_full(struct SNAME *_map_);                                                             \
    size_t PFX##_count(struct SNAME *_map_);                                                          \
    size_t PFX##_key_count(struct SNAME *_map_, K key);                                               \
    size_t PFX##_capacity(struct SNAME *_map_);                                                       \
    double PFX##_load(struct SNAME *_map_);                                                           \
    /* Collection Utility */                                                                          \
    bool PFX##_resize(struct SNAME *_map_, size_t capacity);                                          \
    struct SNAME *PFX##_copy_of(struct SNAME *_map_, K (*key_copy_func)(K), V (*value_copy_func)(V)); \
    bool PFX##_equals(struct SNAME *_map1_, struct SNAME *_map2_, bool ignore_key_count);             \
    struct cmc_string PFX##_to_string(struct SNAME *_map_);                                           \
                                                                                                      \
    /* Iterator Functions */                                                                          \
    /* Iterator Allocation and Deallocation */                                                        \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);                                        \
    void PFX##_iter_free(struct SNAME##_iter *iter);                                                  \
    /* Iterator Initialization */                                                                     \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);                            \
    /* Iterator State */                                                                              \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                                                 \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                                                   \
    /* Iterator Movement */                                                                           \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                                              \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                                                \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                                                  \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                                                  \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);                                 \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);                                  \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);                                   \
    /* Iterator Access */                                                                             \
    K PFX##_iter_key(struct SNAME##_iter *iter);                                                      \
    V PFX##_iter_value(struct SNAME##_iter *iter);                                                    \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter);                                                  \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);                                               \
                                                                                                      \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_MULTIMAP_SOURCE(PFX, SNAME, K, V)                                               \
                                                                                                     \
    /* Implementation Detail Functions */                                                            \
    struct SNAME##_entry *PFX##_impl_new_entry(K key, V value);                                      \
    struct SNAME##_entry *PFX##_impl_get_entry(struct SNAME *_map_, K key);                          \
    size_t PFX##_impl_calculate_size(size_t required);                                               \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_map_);                             \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_map_);                               \
                                                                                                     \
    struct SNAME *PFX##_new(size_t capacity, double load, int (*compare)(K, K), size_t (*hash)(K))   \
    {                                                                                                \
        if (capacity == 0 || load <= 0)                                                              \
            return NULL;                                                                             \
                                                                                                     \
        /* Prevent integer overflow */                                                               \
        if (capacity >= UINTMAX_MAX * load)                                                          \
            return NULL;                                                                             \
                                                                                                     \
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);                           \
                                                                                                     \
        struct SNAME *_map_ = malloc(sizeof(struct SNAME));                                          \
                                                                                                     \
        if (!_map_)                                                                                  \
            return NULL;                                                                             \
                                                                                                     \
        _map_->buffer = calloc(real_capacity, sizeof(struct SNAME##_entry * [2]));                   \
                                                                                                     \
        if (!_map_->buffer)                                                                          \
        {                                                                                            \
            free(_map_);                                                                             \
            return NULL;                                                                             \
        }                                                                                            \
                                                                                                     \
        _map_->count = 0;                                                                            \
        _map_->capacity = real_capacity;                                                             \
        _map_->load = load;                                                                          \
        _map_->cmp = compare;                                                                        \
        _map_->hash = hash;                                                                          \
                                                                                                     \
        _map_->it_start = PFX##_impl_it_start;                                                       \
        _map_->it_end = PFX##_impl_it_end;                                                           \
                                                                                                     \
        return _map_;                                                                                \
    }                                                                                                \
                                                                                                     \
    void PFX##_clear(struct SNAME *_map_, void (*deallocator)(K, V))                                 \
    {                                                                                                \
        size_t index = 0;                                                                            \
        struct SNAME##_entry *scan;                                                                  \
                                                                                                     \
        while (index < _map_->capacity)                                                              \
        {                                                                                            \
            scan = _map_->buffer[index][0];                                                          \
                                                                                                     \
            if (scan != NULL)                                                                        \
            {                                                                                        \
                if (scan->next == NULL && scan->prev == NULL)                                        \
                {                                                                                    \
                    if (deallocator)                                                                 \
                        deallocator(scan->key, scan->value);                                         \
                                                                                                     \
                    free(scan);                                                                      \
                }                                                                                    \
                else                                                                                 \
                {                                                                                    \
                    while (scan != NULL)                                                             \
                    {                                                                                \
                        struct SNAME##_entry *tmp = scan;                                            \
                                                                                                     \
                        scan = scan->next;                                                           \
                                                                                                     \
                        if (deallocator)                                                             \
                            deallocator(tmp->key, tmp->value);                                       \
                                                                                                     \
                        free(tmp);                                                                   \
                    }                                                                                \
                }                                                                                    \
            }                                                                                        \
                                                                                                     \
            index++;                                                                                 \
        }                                                                                            \
                                                                                                     \
        memset(_map_->buffer, 0, sizeof(struct SNAME##_entry * [2]) * _map_->capacity);              \
                                                                                                     \
        _map_->count = 0;                                                                            \
    }                                                                                                \
                                                                                                     \
    void PFX##_free(struct SNAME *_map_, void (*deallocator)(K, V))                                  \
    {                                                                                                \
        size_t index = 0;                                                                            \
        struct SNAME##_entry *scan;                                                                  \
                                                                                                     \
        while (index < _map_->capacity)                                                              \
        {                                                                                            \
            scan = _map_->buffer[index][0];                                                          \
                                                                                                     \
            if (scan != NULL)                                                                        \
            {                                                                                        \
                if (scan->next == NULL && scan->prev == NULL)                                        \
                    free(scan);                                                                      \
                else                                                                                 \
                {                                                                                    \
                    while (scan != NULL)                                                             \
                    {                                                                                \
                        struct SNAME##_entry *tmp = scan;                                            \
                                                                                                     \
                        scan = scan->next;                                                           \
                                                                                                     \
                        if (deallocator)                                                             \
                            deallocator(tmp->key, tmp->value);                                       \
                                                                                                     \
                        free(tmp);                                                                   \
                    }                                                                                \
                }                                                                                    \
            }                                                                                        \
                                                                                                     \
            index++;                                                                                 \
        }                                                                                            \
                                                                                                     \
        free(_map_->buffer);                                                                         \
        free(_map_);                                                                                 \
    }                                                                                                \
                                                                                                     \
    bool PFX##_insert(struct SNAME *_map_, K key, V value)                                           \
    {                                                                                                \
        if (PFX##_full(_map_))                                                                       \
        {                                                                                            \
            if (!PFX##_resize(_map_, PFX##_capacity(_map_) + 1))                                     \
                return false;                                                                        \
        }                                                                                            \
                                                                                                     \
        size_t hash = _map_->hash(key);                                                              \
        size_t pos = hash % _map_->capacity;                                                         \
                                                                                                     \
        struct SNAME##_entry *entry = PFX##_impl_new_entry(key, value);                              \
                                                                                                     \
        if (_map_->buffer[pos][0] == NULL)                                                           \
        {                                                                                            \
            _map_->buffer[pos][0] = entry;                                                           \
            _map_->buffer[pos][1] = entry;                                                           \
        }                                                                                            \
        else                                                                                         \
        {                                                                                            \
            entry->prev = _map_->buffer[pos][1];                                                     \
                                                                                                     \
            _map_->buffer[pos][1]->next = entry;                                                     \
            _map_->buffer[pos][1] = entry;                                                           \
        }                                                                                            \
                                                                                                     \
        _map_->count++;                                                                              \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    bool PFX##_update(struct SNAME *_map_, K key, V new_value, V *old_value)                         \
    {                                                                                                \
        struct SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);                              \
                                                                                                     \
        if (!entry)                                                                                  \
            return false;                                                                            \
                                                                                                     \
        if (old_value)                                                                               \
            *old_value = entry->value;                                                               \
                                                                                                     \
        entry->value = new_value;                                                                    \
                                                                                                     \
        return false;                                                                                \
    }                                                                                                \
                                                                                                     \
    size_t PFX##_update_all(struct SNAME *_map_, K key, V new_value, V **old_values)                 \
    {                                                                                                \
        size_t total = PFX##_key_count(_map_, key);                                                  \
        size_t index = 0;                                                                            \
                                                                                                     \
        if (total == 0)                                                                              \
            return total;                                                                            \
                                                                                                     \
        if (old_values)                                                                              \
        {                                                                                            \
            *old_values = malloc(sizeof(V) * total);                                                 \
                                                                                                     \
            if (!(*old_values))                                                                      \
                return false;                                                                        \
        }                                                                                            \
                                                                                                     \
        size_t hash = _map_->hash(key);                                                              \
                                                                                                     \
        struct SNAME##_entry *entry = _map_->buffer[hash % _map_->capacity][0];                      \
                                                                                                     \
        while (entry != NULL)                                                                        \
        {                                                                                            \
            if (_map_->cmp(entry->key, key) == 0)                                                    \
            {                                                                                        \
                if (old_values)                                                                      \
                    (*old_values)[index++] = entry->value;                                           \
                                                                                                     \
                entry->value = new_value;                                                            \
            }                                                                                        \
                                                                                                     \
            entry = entry->next;                                                                     \
        }                                                                                            \
                                                                                                     \
        return index;                                                                                \
    }                                                                                                \
                                                                                                     \
    bool PFX##_remove(struct SNAME *_map_, K key, V *out_value)                                      \
    {                                                                                                \
        size_t hash = _map_->hash(key);                                                              \
                                                                                                     \
        struct SNAME##_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);                   \
        struct SNAME##_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);                   \
                                                                                                     \
        if (*head == NULL)                                                                           \
            return false;                                                                            \
                                                                                                     \
        struct SNAME##_entry *entry = *head;                                                         \
                                                                                                     \
        if (entry->next == NULL && entry->prev == NULL)                                              \
        {                                                                                            \
            if (_map_->cmp(entry->key, key) == 0)                                                    \
            {                                                                                        \
                *head = NULL;                                                                        \
                *tail = NULL;                                                                        \
                                                                                                     \
                if (out_value)                                                                       \
                    *out_value = entry->value;                                                       \
            }                                                                                        \
            else                                                                                     \
                return false;                                                                        \
        }                                                                                            \
        else                                                                                         \
        {                                                                                            \
            bool found = false;                                                                      \
                                                                                                     \
            while (entry != NULL)                                                                    \
            {                                                                                        \
                if (_map_->cmp(entry->key, key) == 0)                                                \
                {                                                                                    \
                    if (*head == entry)                                                              \
                        *head = entry->next;                                                         \
                    if (*tail == entry)                                                              \
                        *tail = entry->prev;                                                         \
                                                                                                     \
                    if (entry->prev != NULL)                                                         \
                        entry->prev->next = entry->next;                                             \
                    if (entry->next != NULL)                                                         \
                        entry->next->prev = entry->prev;                                             \
                                                                                                     \
                    if (out_value)                                                                   \
                        *out_value = entry->value;                                                   \
                                                                                                     \
                    found = true;                                                                    \
                                                                                                     \
                    break;                                                                           \
                }                                                                                    \
                else                                                                                 \
                    entry = entry->next;                                                             \
            }                                                                                        \
                                                                                                     \
            if (!found)                                                                              \
                return false;                                                                        \
        }                                                                                            \
                                                                                                     \
        free(entry);                                                                                 \
                                                                                                     \
        _map_->count--;                                                                              \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    size_t PFX##_remove_all(struct SNAME *_map_, K key, V **out_values)                              \
    {                                                                                                \
        size_t hash = _map_->hash(key);                                                              \
                                                                                                     \
        struct SNAME##_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);                   \
        struct SNAME##_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);                   \
                                                                                                     \
        if (*head == NULL)                                                                           \
            return 0;                                                                                \
                                                                                                     \
        size_t total = PFX##_key_count(_map_, key);                                                  \
        size_t index = 0;                                                                            \
                                                                                                     \
        if (out_values)                                                                              \
            *out_values = malloc(sizeof(struct SNAME##_entry) * total);                              \
                                                                                                     \
        struct SNAME##_entry *entry = *head;                                                         \
                                                                                                     \
        if (entry->next == NULL)                                                                     \
        {                                                                                            \
            *head = NULL;                                                                            \
            *tail = NULL;                                                                            \
                                                                                                     \
            if (out_values)                                                                          \
                (*out_values)[index++] = entry->value;                                               \
                                                                                                     \
            free(entry);                                                                             \
        }                                                                                            \
        else                                                                                         \
        {                                                                                            \
            while (entry != NULL)                                                                    \
            {                                                                                        \
                if (_map_->cmp(entry->key, key) == 0)                                                \
                {                                                                                    \
                    if (*head == entry)                                                              \
                        *head = entry->next;                                                         \
                    if (*tail == entry)                                                              \
                        *tail = entry->prev;                                                         \
                                                                                                     \
                    struct SNAME##_entry *temp = entry->next;                                        \
                                                                                                     \
                    if (entry->prev != NULL)                                                         \
                        entry->prev->next = entry->next;                                             \
                    if (entry->next != NULL)                                                         \
                        entry->next->prev = entry->prev;                                             \
                                                                                                     \
                    if (out_values)                                                                  \
                        (*out_values)[index++] = entry->value;                                       \
                                                                                                     \
                    free(entry);                                                                     \
                                                                                                     \
                    entry = temp;                                                                    \
                }                                                                                    \
                else                                                                                 \
                    entry = entry->next;                                                             \
            }                                                                                        \
        }                                                                                            \
                                                                                                     \
        _map_->count -= index;                                                                       \
                                                                                                     \
        return index;                                                                                \
    }                                                                                                \
                                                                                                     \
    bool PFX##_max(struct SNAME *_map_, K *key, V *value)                                            \
    {                                                                                                \
        if (PFX##_empty(_map_))                                                                      \
            return false;                                                                            \
                                                                                                     \
        struct SNAME##_iter iter;                                                                    \
                                                                                                     \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))          \
        {                                                                                            \
            K result_key = PFX##_iter_key(&iter);                                                    \
            V result_value = PFX##_iter_value(&iter);                                                \
            size_t index = PFX##_iter_index(&iter);                                                  \
                                                                                                     \
            if (index == 0)                                                                          \
            {                                                                                        \
                *key = result_key;                                                                   \
                *value = result_value;                                                               \
            }                                                                                        \
            else if (_map_->cmp(result_key, *key) > 0)                                               \
            {                                                                                        \
                *key = result_key;                                                                   \
                *value = result_value;                                                               \
            }                                                                                        \
        }                                                                                            \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    bool PFX##_min(struct SNAME *_map_, K *key, V *value)                                            \
    {                                                                                                \
        if (PFX##_empty(_map_))                                                                      \
            return false;                                                                            \
                                                                                                     \
        struct SNAME##_iter iter;                                                                    \
                                                                                                     \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))          \
        {                                                                                            \
            K result_key = PFX##_iter_key(&iter);                                                    \
            V result_value = PFX##_iter_value(&iter);                                                \
            size_t index = PFX##_iter_index(&iter);                                                  \
                                                                                                     \
            if (index == 0)                                                                          \
            {                                                                                        \
                *key = result_key;                                                                   \
                *value = result_value;                                                               \
            }                                                                                        \
            else if (_map_->cmp(result_key, *key) < 0)                                               \
            {                                                                                        \
                *key = result_key;                                                                   \
                *value = result_value;                                                               \
            }                                                                                        \
        }                                                                                            \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    V PFX##_get(struct SNAME *_map_, K key)                                                          \
    {                                                                                                \
        struct SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);                              \
                                                                                                     \
        if (!entry)                                                                                  \
            return (V){0};                                                                           \
                                                                                                     \
        return entry->value;                                                                         \
    }                                                                                                \
                                                                                                     \
    V *PFX##_get_ref(struct SNAME *_map_, K key)                                                     \
    {                                                                                                \
        struct SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);                              \
                                                                                                     \
        if (!entry)                                                                                  \
            return NULL;                                                                             \
                                                                                                     \
        return &(entry->value);                                                                      \
    }                                                                                                \
                                                                                                     \
    bool PFX##_contains(struct SNAME *_map_, K key)                                                  \
    {                                                                                                \
        return PFX##_impl_get_entry(_map_, key) != NULL;                                             \
    }                                                                                                \
                                                                                                     \
    bool PFX##_empty(struct SNAME *_map_)                                                            \
    {                                                                                                \
        return _map_->count == 0;                                                                    \
    }                                                                                                \
                                                                                                     \
    bool PFX##_full(struct SNAME *_map_)                                                             \
    {                                                                                                \
        return (double)PFX##_capacity(_map_) * PFX##_load(_map_) <= (double)PFX##_count(_map_);      \
    }                                                                                                \
                                                                                                     \
    size_t PFX##_count(struct SNAME *_map_)                                                          \
    {                                                                                                \
        return _map_->count;                                                                         \
    }                                                                                                \
                                                                                                     \
    size_t PFX##_key_count(struct SNAME *_map_, K key)                                               \
    {                                                                                                \
        size_t hash = _map_->hash(key);                                                              \
                                                                                                     \
        struct SNAME##_entry *entry = _map_->buffer[hash % _map_->capacity][0];                      \
                                                                                                     \
        size_t total_count = 0;                                                                      \
                                                                                                     \
        if (!entry)                                                                                  \
            return total_count;                                                                      \
                                                                                                     \
        while (entry != NULL)                                                                        \
        {                                                                                            \
            if (_map_->cmp(entry->key, key) == 0)                                                    \
                total_count++;                                                                       \
                                                                                                     \
            entry = entry->next;                                                                     \
        }                                                                                            \
                                                                                                     \
        return total_count;                                                                          \
    }                                                                                                \
                                                                                                     \
    size_t PFX##_capacity(struct SNAME *_map_)                                                       \
    {                                                                                                \
        return _map_->capacity;                                                                      \
    }                                                                                                \
                                                                                                     \
    double PFX##_load(struct SNAME *_map_)                                                           \
    {                                                                                                \
        return _map_->load;                                                                          \
    }                                                                                                \
                                                                                                     \
    bool PFX##_resize(struct SNAME *_map_, size_t capacity)                                          \
    {                                                                                                \
        if (PFX##_capacity(_map_) == capacity)                                                       \
            return true;                                                                             \
                                                                                                     \
        if (PFX##_capacity(_map_) > capacity / PFX##_load(_map_))                                    \
            return true;                                                                             \
                                                                                                     \
        /* Prevent integer overflow */                                                               \
        if (capacity >= UINTMAX_MAX * PFX##_load(_map_))                                             \
            return false;                                                                            \
                                                                                                     \
        /* Calculate required capacity based on the prime numbers */                                 \
        size_t theoretical_size = PFX##_impl_calculate_size(capacity);                               \
                                                                                                     \
        /* Not possible to shrink with current available prime numbers */                            \
        if (theoretical_size < PFX##_count(_map_) / PFX##_load(_map_))                               \
            return false;                                                                            \
                                                                                                     \
        struct SNAME *_new_map_ = PFX##_new(capacity, PFX##_load(_map_), _map_->cmp, _map_->hash);   \
                                                                                                     \
        if (!_new_map_)                                                                              \
            return false;                                                                            \
                                                                                                     \
        struct SNAME##_iter iter;                                                                    \
                                                                                                     \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))          \
        {                                                                                            \
            K key = PFX##_iter_key(&iter);                                                           \
            V value = PFX##_iter_value(&iter);                                                       \
                                                                                                     \
            PFX##_insert(_new_map_, key, value);                                                     \
        }                                                                                            \
                                                                                                     \
        if (PFX##_count(_map_) != PFX##_count(_new_map_))                                            \
        {                                                                                            \
            PFX##_free(_new_map_, NULL);                                                             \
            return false;                                                                            \
        }                                                                                            \
                                                                                                     \
        struct SNAME##_entry *(*tmp_b)[2] = _map_->buffer;                                           \
        _map_->buffer = _new_map_->buffer;                                                           \
        _new_map_->buffer = tmp_b;                                                                   \
                                                                                                     \
        size_t tmp_c = _map_->capacity;                                                              \
        _map_->capacity = _new_map_->capacity;                                                       \
        _new_map_->capacity = tmp_c;                                                                 \
                                                                                                     \
        PFX##_free(_new_map_, NULL);                                                                 \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    struct SNAME *PFX##_copy_of(struct SNAME *_map_, K (*key_copy_func)(K), V (*value_copy_func)(V)) \
    {                                                                                                \
        struct SNAME *result = PFX##_new(_map_->capacity, _map_->load, _map_->cmp, _map_->hash);     \
                                                                                                     \
        if (!result)                                                                                 \
            return NULL;                                                                             \
                                                                                                     \
        struct SNAME##_iter iter;                                                                    \
        PFX##_iter_init(&iter, _map_);                                                               \
                                                                                                     \
        if (!PFX##_empty(_map_))                                                                     \
        {                                                                                            \
            for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))         \
            {                                                                                        \
                K key = PFX##_iter_key(&iter);                                                       \
                V value = PFX##_iter_value(&iter);                                                   \
                                                                                                     \
                if (key_copy_func)                                                                   \
                    key = key_copy_func(key);                                                        \
                if (value_copy_func)                                                                 \
                    value = value_copy_func(value);                                                  \
                                                                                                     \
                PFX##_insert(result, key, value);                                                    \
            }                                                                                        \
        }                                                                                            \
                                                                                                     \
        return result;                                                                               \
    }                                                                                                \
                                                                                                     \
    bool PFX##_equals(struct SNAME *_map1_, struct SNAME *_map2_, bool ignore_key_count)             \
    {                                                                                                \
        if (ignore_key_count)                                                                        \
        {                                                                                            \
            struct SNAME##_iter iter;                                                                \
            PFX##_iter_init(&iter, _map1_);                                                          \
                                                                                                     \
            for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))         \
            {                                                                                        \
                if (PFX##_impl_get_entry(_map2_, PFX##_iter_key(&iter)) == NULL)                     \
                    return false;                                                                    \
            }                                                                                        \
        }                                                                                            \
        else                                                                                         \
        {                                                                                            \
            /* Since the key count matters this can be checked and an early exit */                  \
            if (PFX##_count(_map1_) != PFX##_count(_map2_))                                          \
                return false;                                                                        \
                                                                                                     \
            struct SNAME##_iter iter;                                                                \
            PFX##_iter_init(&iter, _map1_);                                                          \
                                                                                                     \
            for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))         \
            {                                                                                        \
                K key = PFX##_iter_key(&iter);                                                       \
                                                                                                     \
                if (PFX##_key_count(_map1_, key) != PFX##_key_count(_map2_, key))                    \
                    return false;                                                                    \
            }                                                                                        \
        }                                                                                            \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    struct cmc_string PFX##_to_string(struct SNAME *_map_)                                           \
    {                                                                                                \
        struct cmc_string str;                                                                       \
        struct SNAME *m_ = _map_;                                                                    \
        const char *name = #SNAME;                                                                   \
                                                                                                     \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_multimap,                                     \
                 name, m_, m_->buffer, m_->capacity, m_->count, m_->load, m_->cmp, m_->hash);        \
                                                                                                     \
        return str;                                                                                  \
    }                                                                                                \
                                                                                                     \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                                        \
    {                                                                                                \
        struct SNAME##_iter *iter = malloc(sizeof(struct SNAME##_iter));                             \
                                                                                                     \
        if (!iter)                                                                                   \
            return NULL;                                                                             \
                                                                                                     \
        PFX##_iter_init(iter, target);                                                               \
                                                                                                     \
        return iter;                                                                                 \
    }                                                                                                \
                                                                                                     \
    void PFX##_iter_free(struct SNAME##_iter *iter)                                                  \
    {                                                                                                \
        free(iter);                                                                                  \
    }                                                                                                \
                                                                                                     \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)                            \
    {                                                                                                \
        memset(iter, 0, sizeof(struct SNAME##_iter));                                                \
                                                                                                     \
        iter->target = target;                                                                       \
        iter->start = true;                                                                          \
        iter->end = PFX##_empty(target);                                                             \
                                                                                                     \
        if (!PFX##_empty(target))                                                                    \
        {                                                                                            \
            for (size_t i = 0; i < target->capacity; i++)                                            \
            {                                                                                        \
                if (target->buffer[i][0] != NULL)                                                    \
                {                                                                                    \
                    iter->curr_entry = target->buffer[i][0];                                         \
                    iter->first = i;                                                                 \
                    break;                                                                           \
                }                                                                                    \
            }                                                                                        \
                                                                                                     \
            iter->cursor = iter->first;                                                              \
                                                                                                     \
            for (size_t i = target->capacity; i > 0; i--)                                            \
            {                                                                                        \
                if (target->buffer[i - 1][0] != NULL)                                                \
                {                                                                                    \
                    iter->last = i - 1;                                                              \
                    break;                                                                           \
                }                                                                                    \
            }                                                                                        \
        }                                                                                            \
    }                                                                                                \
                                                                                                     \
    bool PFX##_iter_start(struct SNAME##_iter *iter)                                                 \
    {                                                                                                \
        return PFX##_empty(iter->target) || iter->start;                                             \
    }                                                                                                \
                                                                                                     \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                                                   \
    {                                                                                                \
        return PFX##_empty(iter->target) || iter->end;                                               \
    }                                                                                                \
                                                                                                     \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                                              \
    {                                                                                                \
        if (!PFX##_empty(iter->target))                                                              \
        {                                                                                            \
            iter->cursor = iter->first;                                                              \
            iter->index = 0;                                                                         \
            iter->start = true;                                                                      \
            iter->end = PFX##_empty(iter->target);                                                   \
            iter->curr_entry = iter->target->buffer[iter->first][0];                                 \
        }                                                                                            \
    }                                                                                                \
                                                                                                     \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                                                \
    {                                                                                                \
        if (!PFX##_empty(iter->target))                                                              \
        {                                                                                            \
            iter->cursor = iter->last;                                                               \
            iter->index = iter->target->count - 1;                                                   \
            iter->start = PFX##_empty(iter->target);                                                 \
            iter->end = true;                                                                        \
            iter->curr_entry = iter->target->buffer[iter->last][1];                                  \
        }                                                                                            \
    }                                                                                                \
                                                                                                     \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                                                  \
    {                                                                                                \
        if (iter->end)                                                                               \
            return false;                                                                            \
                                                                                                     \
        if (iter->curr_entry->next != NULL)                                                          \
        {                                                                                            \
            iter->curr_entry = iter->curr_entry->next;                                               \
            iter->index++;                                                                           \
        }                                                                                            \
        else                                                                                         \
        {                                                                                            \
            if (iter->cursor == iter->last)                                                          \
            {                                                                                        \
                iter->end = true;                                                                    \
                return false;                                                                        \
            }                                                                                        \
                                                                                                     \
            iter->cursor++;                                                                          \
                                                                                                     \
            while (iter->target->buffer[iter->cursor][0] == NULL)                                    \
                iter->cursor++;                                                                      \
                                                                                                     \
            iter->curr_entry = iter->target->buffer[iter->cursor][0];                                \
                                                                                                     \
            iter->index++;                                                                           \
        }                                                                                            \
                                                                                                     \
        iter->start = PFX##_empty(iter->target);                                                     \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                                                  \
    {                                                                                                \
        if (iter->start)                                                                             \
            return false;                                                                            \
                                                                                                     \
        if (iter->curr_entry->prev != NULL)                                                          \
        {                                                                                            \
            iter->curr_entry = iter->curr_entry->prev;                                               \
            iter->index--;                                                                           \
        }                                                                                            \
        else                                                                                         \
        {                                                                                            \
            if (iter->cursor == iter->first)                                                         \
            {                                                                                        \
                iter->start = true;                                                                  \
                return false;                                                                        \
            }                                                                                        \
                                                                                                     \
            iter->cursor--;                                                                          \
                                                                                                     \
            while (iter->target->buffer[iter->cursor][1] == NULL)                                    \
                iter->cursor--;                                                                      \
                                                                                                     \
            iter->curr_entry = iter->target->buffer[iter->cursor][1];                                \
                                                                                                     \
            iter->index--;                                                                           \
        }                                                                                            \
                                                                                                     \
        iter->end = PFX##_empty(iter->target);                                                       \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    /* Returns true only if the iterator moved */                                                    \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)                                 \
    {                                                                                                \
        if (iter->end)                                                                               \
            return false;                                                                            \
                                                                                                     \
        if (iter->index + 1 == PFX##_count(iter->target))                                            \
        {                                                                                            \
            iter->end = true;                                                                        \
            return false;                                                                            \
        }                                                                                            \
                                                                                                     \
        if (steps == 0 || iter->index + steps >= PFX##_count(iter->target))                          \
            return false;                                                                            \
                                                                                                     \
        for (size_t i = 0; i < steps; i++)                                                           \
            PFX##_iter_next(iter);                                                                   \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    /* Returns true only if the iterator moved */                                                    \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps)                                  \
    {                                                                                                \
        if (iter->start)                                                                             \
            return false;                                                                            \
                                                                                                     \
        if (iter->index == 0)                                                                        \
        {                                                                                            \
            iter->start = true;                                                                      \
            return false;                                                                            \
        }                                                                                            \
                                                                                                     \
        if (steps == 0 || iter->index < steps)                                                       \
            return false;                                                                            \
                                                                                                     \
        for (size_t i = 0; i < steps; i++)                                                           \
            PFX##_iter_prev(iter);                                                                   \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    /* Returns true only if the iterator was able to be positioned at the given index */             \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)                                   \
    {                                                                                                \
        if (index >= PFX##_count(iter->target))                                                      \
            return false;                                                                            \
                                                                                                     \
        if (iter->index > index)                                                                     \
            return PFX##_iter_rewind(iter, iter->index - index);                                     \
        else if (iter->index < index)                                                                \
            return PFX##_iter_advance(iter, index - iter->index);                                    \
                                                                                                     \
        return true;                                                                                 \
    }                                                                                                \
                                                                                                     \
    K PFX##_iter_key(struct SNAME##_iter *iter)                                                      \
    {                                                                                                \
        if (PFX##_empty(iter->target))                                                               \
            return (K){0};                                                                           \
                                                                                                     \
        return iter->curr_entry->key;                                                                \
    }                                                                                                \
                                                                                                     \
    V PFX##_iter_value(struct SNAME##_iter *iter)                                                    \
    {                                                                                                \
        if (PFX##_empty(iter->target))                                                               \
            return (V){0};                                                                           \
                                                                                                     \
        return iter->curr_entry->value;                                                              \
    }                                                                                                \
                                                                                                     \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter)                                                  \
    {                                                                                                \
        if (PFX##_empty(iter->target))                                                               \
            return NULL;                                                                             \
                                                                                                     \
        return &(iter->curr_entry->value);                                                           \
    }                                                                                                \
                                                                                                     \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                                               \
    {                                                                                                \
        return iter->index;                                                                          \
    }                                                                                                \
                                                                                                     \
    struct SNAME##_entry *PFX##_impl_new_entry(K key, V value)                                       \
    {                                                                                                \
        struct SNAME##_entry *entry = malloc(sizeof(struct SNAME##_entry));                          \
                                                                                                     \
        if (!entry)                                                                                  \
            return NULL;                                                                             \
                                                                                                     \
        entry->key = key;                                                                            \
        entry->value = value;                                                                        \
        entry->next = NULL;                                                                          \
        entry->prev = NULL;                                                                          \
                                                                                                     \
        return entry;                                                                                \
    }                                                                                                \
                                                                                                     \
    struct SNAME##_entry *PFX##_impl_get_entry(struct SNAME *_map_, K key)                           \
    {                                                                                                \
        size_t hash = _map_->hash(key);                                                              \
                                                                                                     \
        struct SNAME##_entry *entry = _map_->buffer[hash % _map_->capacity][0];                      \
                                                                                                     \
        while (entry != NULL)                                                                        \
        {                                                                                            \
            if (_map_->cmp(entry->key, key) == 0)                                                    \
                return entry;                                                                        \
                                                                                                     \
            entry = entry->next;                                                                     \
        }                                                                                            \
                                                                                                     \
        return NULL;                                                                                 \
    }                                                                                                \
                                                                                                     \
    size_t PFX##_impl_calculate_size(size_t required)                                                \
    {                                                                                                \
        const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);         \
                                                                                                     \
        if (cmc_hashtable_primes[count - 1] < required)                                              \
            return required;                                                                         \
                                                                                                     \
        size_t i = 0;                                                                                \
        while (cmc_hashtable_primes[i] < required)                                                   \
            i++;                                                                                     \
                                                                                                     \
        return cmc_hashtable_primes[i];                                                              \
    }                                                                                                \
                                                                                                     \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_map_)                              \
    {                                                                                                \
        struct SNAME##_iter iter;                                                                    \
                                                                                                     \
        PFX##_iter_init(&iter, _map_);                                                               \
                                                                                                     \
        return iter;                                                                                 \
    }                                                                                                \
                                                                                                     \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_map_)                                \
    {                                                                                                \
        struct SNAME##_iter iter;                                                                    \
                                                                                                     \
        PFX##_iter_init(&iter, _map_);                                                               \
        PFX##_iter_to_end(&iter);                                                                    \
                                                                                                     \
        return iter;                                                                                 \
    }

#endif /* CMC_MULTIMAP_H */
