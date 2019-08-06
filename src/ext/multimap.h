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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

#ifndef CMC_HASH_TABLE_SETUP
#define CMC_HASH_TABLE_SETUP

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

#endif /* CMC_HASH_TABLE_SETUP */

#define MULTIMAP_GENERATE(PFX, SNAME, FMOD, K, V)    \
    MULTIMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V) \
    MULTIMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)

#define MULTIMAP_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V) \
    MULTIMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)

#define MULTIMAP_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V) \
    MULTIMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)

/* HEADER ********************************************************************/
#define MULTIMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)                                          \
                                                                                                  \
    /* Multimap Structure */                                                                      \
    typedef struct SNAME##_s                                                                      \
    {                                                                                             \
        /* Array of linked list to entries */                                                     \
        struct SNAME##_entry_s *(*buffer)[2];                                                     \
                                                                                                  \
        /* Current array capacity */                                                              \
        size_t capacity;                                                                          \
                                                                                                  \
        /* Current amount of keys */                                                              \
        size_t count;                                                                             \
                                                                                                  \
        /* Load factor in range (0.0, infinity) */                                                \
        double load;                                                                              \
                                                                                                  \
        /* Key comparison functions */                                                            \
        int (*cmp)(K, K);                                                                         \
                                                                                                  \
        /* Key hash function */                                                                   \
        size_t (*hash)(K);                                                                        \
                                                                                                  \
        /* Function that returns an iterator to the start of the multimap */                      \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                                    \
                                                                                                  \
        /* Function that returns an iterator to the end of the multimap */                        \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                                      \
                                                                                                  \
    } SNAME, *SNAME##_ptr;                                                                        \
                                                                                                  \
    /* Multimap Entry */                                                                          \
    typedef struct SNAME##_entry_s                                                                \
    {                                                                                             \
        /* Entry Key */                                                                           \
        K key;                                                                                    \
                                                                                                  \
        /* Entry Value */                                                                         \
        V value;                                                                                  \
                                                                                                  \
        /* Next entry on the linked list */                                                       \
        struct SNAME##_entry_s *next;                                                             \
                                                                                                  \
        /* Previous entry on the linked list */                                                   \
        struct SNAME##_entry_s *prev;                                                             \
                                                                                                  \
    } SNAME##_entry, *SNAME##_entry_ptr;                                                          \
                                                                                                  \
    typedef struct SNAME##_iter_s                                                                 \
    {                                                                                             \
        /* Target multimap */                                                                     \
        struct SNAME##_s *target;                                                                 \
                                                                                                  \
        /* Current entry */                                                                       \
        struct SNAME##_entry_s *curr_entry;                                                       \
                                                                                                  \
        /* Cursor`s position (index) */                                                           \
        size_t cursor;                                                                            \
                                                                                                  \
        /* Keeps track of relative index to the iteration of elements */                          \
        size_t index;                                                                             \
                                                                                                  \
        /* The index of the first element */                                                      \
        size_t first;                                                                             \
                                                                                                  \
        /* The index of the last element */                                                       \
        size_t last;                                                                              \
                                                                                                  \
        /* If the iterator has reached the start of the iteration */                              \
        bool start;                                                                               \
                                                                                                  \
        /* If the iterator has reached the end of the iteration */                                \
        bool end;                                                                                 \
    } SNAME##_iter, *SNAME##_iter_ptr;                                                            \
                                                                                                  \
    /* Collection Functions */                                                                    \
    /* Collection Allocation and Deallocation */                                                  \
    FMOD SNAME *PFX##_new(size_t capacity, double load, int (*compare)(K, K), size_t (*hash)(K)); \
    FMOD void PFX##_clear(SNAME *_map_);                                                          \
    FMOD void PFX##_free(SNAME *_map_);                                                           \
    /* Collection Input and Output */                                                             \
    FMOD bool PFX##_insert(SNAME *_map_, K key, V value);                                         \
    FMOD bool PFX##_update(SNAME *_map_, K key, V new_value, V *old_value);                       \
    FMOD size_t PFX##_update_all(SNAME *_map_, K key, V new_value, V **old_values);               \
    FMOD bool PFX##_remove(SNAME *_map_, K key, V *value);                                        \
    FMOD size_t PFX##_remove_all(SNAME *_map_, K key);                                            \
    /* Conditional Input and Output */                                                            \
    FMOD bool PFX##_insert_if(SNAME *_map_, K key, V value, bool condition);                      \
    FMOD bool PFX##_remove_if(SNAME *_map_, K key, V *value, bool condition);                     \
    /* Element Access */                                                                          \
    FMOD bool PFX##_max(SNAME *_map_, K *key, V *value);                                          \
    FMOD bool PFX##_min(SNAME *_map_, K *key, V *value);                                          \
    FMOD V PFX##_get(SNAME *_map_, K key);                                                        \
    FMOD V *PFX##_get_ref(SNAME *_map_, K key);                                                   \
    FMOD bool PFX##_set(SNAME *_map_, K key, V new_value);                                        \
    FMOD size_t PFX##_set_all(SNAME *_map_, K key, V new_value);                                  \
    /* Collection State */                                                                        \
    FMOD bool PFX##_contains(SNAME *_map_, K key);                                                \
    FMOD bool PFX##_empty(SNAME *_map_);                                                          \
    FMOD size_t PFX##_count(SNAME *_map_);                                                        \
    FMOD size_t PFX##_capacity(SNAME *_map_);                                                     \
    /* Collection Utility */                                                                      \
    FMOD cmc_string PFX##_to_string(SNAME *_map_);                                                \
                                                                                                  \
    /* Iterator Functions */                                                                      \
    /* Iterator Allocation and Deallocation */                                                    \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);                                             \
    FMOD void PFX##_iter_free(SNAME##_iter *iter);                                                \
    /* Iterator Initialization */                                                                 \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                                 \
    /* Iterator State */                                                                          \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                               \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                                 \
    /* Iterator Movement */                                                                       \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter);                                            \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter);                                              \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter);                                                \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter);                                                \
    /* Iterator Access */                                                                         \
    FMOD K PFX##_iter_key(SNAME##_iter *iter);                                                    \
    FMOD V PFX##_iter_value(SNAME##_iter *iter);                                                  \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter);                                                \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter);                                             \
                                                                                                  \
    /* Default Key */                                                                             \
    static inline K PFX##_impl_default_key(void)                                                  \
    {                                                                                             \
        K _empty_key_;                                                                            \
                                                                                                  \
        memset(&_empty_key_, 0, sizeof(K));                                                       \
                                                                                                  \
        return _empty_key_;                                                                       \
    }                                                                                             \
                                                                                                  \
    /* Default Value */                                                                           \
    static inline V PFX##_impl_default_value(void)                                                \
    {                                                                                             \
        V _empty_value_;                                                                          \
                                                                                                  \
        memset(&_empty_value_, 0, sizeof(V));                                                     \
                                                                                                  \
        return _empty_value_;                                                                     \
    }                                                                                             \
/* SOURCE ********************************************************************/
#define MULTIMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)                                         \
                                                                                                 \
    /* Implementation Detail Functions */                                                        \
    FMOD bool PFX##_impl_grow(SNAME *_map_);                                                     \
    FMOD SNAME##_entry *PFX##_impl_new_entry(K key, V value);                                    \
    FMOD SNAME##_entry *PFX##_impl_get_entry(SNAME *_map_, K key);                               \
    FMOD size_t PFX##_impl_calculate_size(size_t required);                                      \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_map_);                                       \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_map_);                                         \
                                                                                                 \
    FMOD SNAME *PFX##_new(size_t capacity, double load, int (*compare)(K, K), size_t (*hash)(K)) \
    {                                                                                            \
        if (capacity == 0 || load <= 0)                                                          \
            return NULL;                                                                         \
                                                                                                 \
        /* Prevent integer overflow */                                                           \
        if (capacity >= UINTMAX_MAX * load)                                                      \
            return NULL;                                                                         \
                                                                                                 \
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);                       \
                                                                                                 \
        SNAME *_map_ = malloc(sizeof(SNAME));                                                    \
                                                                                                 \
        if (!_map_)                                                                              \
            return NULL;                                                                         \
                                                                                                 \
        _map_->buffer = malloc(sizeof(SNAME##_entry_ptr[2]) * real_capacity);                    \
                                                                                                 \
        if (!_map_->buffer)                                                                      \
        {                                                                                        \
            free(_map_);                                                                         \
            return NULL;                                                                         \
        }                                                                                        \
                                                                                                 \
        memset(_map_->buffer, 0, sizeof(SNAME##_entry_ptr[2]) * real_capacity);                  \
                                                                                                 \
        _map_->count = 0;                                                                        \
        _map_->capacity = real_capacity;                                                         \
        _map_->load = load;                                                                      \
        _map_->cmp = compare;                                                                    \
        _map_->hash = hash;                                                                      \
                                                                                                 \
        return _map_;                                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD void PFX##_clear(SNAME *_map_)                                                          \
    {                                                                                            \
        size_t index = 0;                                                                        \
        SNAME##_entry *scan;                                                                     \
                                                                                                 \
        while (index < _map_->capacity)                                                          \
        {                                                                                        \
            scan = _map_->buffer[index][0];                                                      \
                                                                                                 \
            if (scan != NULL)                                                                    \
            {                                                                                    \
                if (scan->next == NULL && scan->prev == NULL)                                    \
                    free(scan);                                                                  \
                else                                                                             \
                {                                                                                \
                    while (scan != NULL)                                                         \
                    {                                                                            \
                        SNAME##_entry *tmp = scan;                                               \
                                                                                                 \
                        scan = scan->next;                                                       \
                                                                                                 \
                        free(tmp);                                                               \
                    }                                                                            \
                }                                                                                \
            }                                                                                    \
                                                                                                 \
            index++;                                                                             \
        }                                                                                        \
                                                                                                 \
        memset(_map_->buffer, 0, sizeof(SNAME##_entry_ptr[2]) * _map_->capacity);                \
                                                                                                 \
        _map_->count = 0;                                                                        \
    }                                                                                            \
                                                                                                 \
    FMOD void PFX##_free(SNAME *_map_)                                                           \
    {                                                                                            \
        PFX##_clear(_map_);                                                                      \
        free(_map_->buffer);                                                                     \
        free(_map_);                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_insert(SNAME *_map_, K key, V value)                                         \
    {                                                                                            \
        if ((double)_map_->capacity * _map_->load <= (double)_map_->count)                       \
        {                                                                                        \
            if (!PFX##_impl_grow(_map_))                                                         \
                return false;                                                                    \
        }                                                                                        \
                                                                                                 \
        size_t hash = _map_->hash(key);                                                          \
        size_t pos = hash % _map_->capacity;                                                     \
                                                                                                 \
        SNAME##_entry *entry = PFX##_impl_new_entry(key, value);                                 \
                                                                                                 \
        if (_map_->buffer[pos][0] == NULL)                                                       \
        {                                                                                        \
            _map_->buffer[pos][0] = entry;                                                       \
            _map_->buffer[pos][1] = entry;                                                       \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            entry->prev = _map_->buffer[pos][1];                                                 \
                                                                                                 \
            _map_->buffer[pos][1]->next = entry;                                                 \
            _map_->buffer[pos][1] = entry;                                                       \
        }                                                                                        \
                                                                                                 \
        _map_->count++;                                                                          \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_update(SNAME *_map_, K key, V new_value, V *old_value)                       \
    {                                                                                            \
        SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);                                 \
                                                                                                 \
        if (!entry)                                                                              \
            return false;                                                                        \
                                                                                                 \
        *old_value = entry->value;                                                               \
        entry->value = new_value;                                                                \
                                                                                                 \
        return false;                                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD size_t PFX##_update_all(SNAME *_map_, K key, V new_value, V **old_values)               \
    {                                                                                            \
        size_t hash = _map_->hash(key);                                                          \
                                                                                                 \
        SNAME##_entry *bucket = _map_->buffer[hash % _map_->capacity][0];                        \
        SNAME##_entry *entry = bucket;                                                           \
                                                                                                 \
        size_t total = 0;                                                                        \
                                                                                                 \
        while (entry != NULL)                                                                    \
        {                                                                                        \
            if (_map_->cmp(entry->key, key) == 0)                                                \
                total++;                                                                         \
                                                                                                 \
            entry = entry->next;                                                                 \
        }                                                                                        \
                                                                                                 \
        if (total == 0)                                                                          \
            return total;                                                                        \
                                                                                                 \
        *old_values = malloc(sizeof(V) * total);                                                 \
                                                                                                 \
        if (!(*old_values))                                                                      \
            return false;                                                                        \
                                                                                                 \
        entry = bucket;                                                                          \
        size_t index = 0;                                                                        \
                                                                                                 \
        while (entry != NULL)                                                                    \
        {                                                                                        \
            if (_map_->cmp(entry->key, key) == 0)                                                \
            {                                                                                    \
                (*old_values)[index++] = entry->value;                                           \
                                                                                                 \
                entry->value = new_value;                                                        \
            }                                                                                    \
                                                                                                 \
            entry = entry->next;                                                                 \
        }                                                                                        \
                                                                                                 \
        return total;                                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_remove(SNAME *_map_, K key, V *value)                                        \
    {                                                                                            \
        size_t hash = _map_->hash(key);                                                          \
                                                                                                 \
        SNAME##_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);                      \
        SNAME##_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);                      \
                                                                                                 \
        if (*head == NULL)                                                                       \
            return false;                                                                        \
                                                                                                 \
        SNAME##_entry *entry = *head;                                                            \
                                                                                                 \
        if (entry->next == NULL && entry->prev == NULL)                                          \
        {                                                                                        \
            if (_map_->cmp(entry->key, key) == 0)                                                \
            {                                                                                    \
                *head = NULL;                                                                    \
                *tail = NULL;                                                                    \
                                                                                                 \
                *value = entry->value;                                                           \
            }                                                                                    \
            else                                                                                 \
                return false;                                                                    \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            bool found = false;                                                                  \
                                                                                                 \
            while (entry != NULL)                                                                \
            {                                                                                    \
                if (_map_->cmp(entry->key, key) == 0)                                            \
                {                                                                                \
                    if (*head == entry)                                                          \
                        *head = entry->next;                                                     \
                    if (*tail == entry)                                                          \
                        *tail = entry->prev;                                                     \
                                                                                                 \
                    if (entry->prev != NULL)                                                     \
                        entry->prev->next = entry->next;                                         \
                    if (entry->next != NULL)                                                     \
                        entry->next->prev = entry->prev;                                         \
                                                                                                 \
                    *value = entry->value;                                                       \
                    found = true;                                                                \
                                                                                                 \
                    break;                                                                       \
                }                                                                                \
                else                                                                             \
                    entry = entry->next;                                                         \
            }                                                                                    \
                                                                                                 \
            if (!found)                                                                          \
                return false;                                                                    \
        }                                                                                        \
                                                                                                 \
        free(entry);                                                                             \
                                                                                                 \
        _map_->count--;                                                                          \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD size_t PFX##_remove_all(SNAME *_map_, K key)                                            \
    {                                                                                            \
        size_t total = 0;                                                                        \
        size_t hash = _map_->hash(key);                                                          \
                                                                                                 \
        SNAME##_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);                      \
        SNAME##_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);                      \
                                                                                                 \
        if (*head == NULL)                                                                       \
            return total;                                                                        \
                                                                                                 \
        SNAME##_entry *entry = *head;                                                            \
                                                                                                 \
        if (entry->next == NULL)                                                                 \
        {                                                                                        \
            *head = NULL;                                                                        \
            *tail = NULL;                                                                        \
                                                                                                 \
            free(entry);                                                                         \
                                                                                                 \
            total++;                                                                             \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            while (entry != NULL)                                                                \
            {                                                                                    \
                if (_map_->cmp(entry->key, key) == 0)                                            \
                {                                                                                \
                    if (*head == entry)                                                          \
                        *head = entry->next;                                                     \
                    if (*tail == entry)                                                          \
                        *tail = entry->prev;                                                     \
                                                                                                 \
                    SNAME##_entry *temp = entry->next;                                           \
                                                                                                 \
                    if (entry->prev != NULL)                                                     \
                        entry->prev->next = entry->next;                                         \
                    if (entry->next != NULL)                                                     \
                        entry->next->prev = entry->prev;                                         \
                                                                                                 \
                    free(entry);                                                                 \
                    entry = temp;                                                                \
                                                                                                 \
                    total++;                                                                     \
                }                                                                                \
                else                                                                             \
                    entry = entry->next;                                                         \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        _map_->count -= total;                                                                   \
                                                                                                 \
        return total;                                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_insert_if(SNAME *_map_, K key, V value, bool condition)                      \
    {                                                                                            \
        if (condition)                                                                           \
            return PFX##_insert(_map_, key, value);                                              \
                                                                                                 \
        return false;                                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_remove_if(SNAME *_map_, K key, V *value, bool condition)                     \
    {                                                                                            \
        if (condition)                                                                           \
            return PFX##_remove(_map_, key, value);                                              \
                                                                                                 \
        return false;                                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_max(SNAME *_map_, K *key, V *value)                                          \
    {                                                                                            \
        if (PFX##_empty(_map_))                                                                  \
            return false;                                                                        \
                                                                                                 \
        SNAME##_iter iter;                                                                       \
                                                                                                 \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))      \
        {                                                                                        \
            K result_key = PFX##_iter_key(&iter);                                                \
            V result_value = PFX##_iter_value(&iter);                                            \
            size_t index = PFX##_iter_index(&iter);                                              \
                                                                                                 \
            if (index == 0)                                                                      \
            {                                                                                    \
                *key = result_key;                                                               \
                *value = result_value;                                                           \
            }                                                                                    \
            else if (_map_->cmp(result_key, *key) > 0)                                           \
            {                                                                                    \
                *key = result_key;                                                               \
                *value = result_value;                                                           \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_min(SNAME *_map_, K *key, V *value)                                          \
    {                                                                                            \
        if (PFX##_empty(_map_))                                                                  \
            return false;                                                                        \
                                                                                                 \
        SNAME##_iter iter;                                                                       \
                                                                                                 \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))      \
        {                                                                                        \
            K result_key = PFX##_iter_key(&iter);                                                \
            V result_value = PFX##_iter_value(&iter);                                            \
            size_t index = PFX##_iter_index(&iter);                                              \
                                                                                                 \
            if (index == 0)                                                                      \
            {                                                                                    \
                *key = result_key;                                                               \
                *value = result_value;                                                           \
            }                                                                                    \
            else if (_map_->cmp(result_key, *key) < 0)                                           \
            {                                                                                    \
                *key = result_key;                                                               \
                *value = result_value;                                                           \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD V PFX##_get(SNAME *_map_, K key)                                                        \
    {                                                                                            \
        SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);                                 \
                                                                                                 \
        if (!entry)                                                                              \
            return PFX##_impl_default_value();                                                   \
                                                                                                 \
        return entry->value;                                                                     \
    }                                                                                            \
                                                                                                 \
    FMOD V *PFX##_get_ref(SNAME *_map_, K key)                                                   \
    {                                                                                            \
        SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);                                 \
                                                                                                 \
        if (!entry)                                                                              \
            return NULL;                                                                         \
                                                                                                 \
        return &(entry->value);                                                                  \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_set(SNAME *_map_, K key, V new_value)                                        \
    {                                                                                            \
        SNAME##_entry *entry = PFX##_impl_get_entry(_map_, key);                                 \
                                                                                                 \
        if (!entry)                                                                              \
            return false;                                                                        \
                                                                                                 \
        entry->value = new_value;                                                                \
                                                                                                 \
        return false;                                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD size_t PFX##_set_all(SNAME *_map_, K key, V new_value)                                  \
    {                                                                                            \
        size_t hash = _map_->hash(key);                                                          \
                                                                                                 \
        SNAME##_entry *entry = _map_->buffer[hash % _map_->capacity][0];                         \
                                                                                                 \
        size_t total = 0;                                                                        \
                                                                                                 \
        while (entry != NULL)                                                                    \
        {                                                                                        \
            if (_map_->cmp(entry->key, key) == 0)                                                \
            {                                                                                    \
                entry->value = new_value;                                                        \
                                                                                                 \
                total++;                                                                         \
            }                                                                                    \
                                                                                                 \
            entry = entry->next;                                                                 \
        }                                                                                        \
                                                                                                 \
        return total;                                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_contains(SNAME *_map_, K key)                                                \
    {                                                                                            \
        return PFX##_impl_get_entry(_map_, key) != NULL;                                         \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_empty(SNAME *_map_)                                                          \
    {                                                                                            \
        return _map_->count == 0;                                                                \
    }                                                                                            \
                                                                                                 \
    FMOD size_t PFX##_count(SNAME *_map_)                                                        \
    {                                                                                            \
        return _map_->count;                                                                     \
    }                                                                                            \
                                                                                                 \
    FMOD size_t PFX##_capacity(SNAME *_map_)                                                     \
    {                                                                                            \
        return _map_->capacity;                                                                  \
    }                                                                                            \
                                                                                                 \
    FMOD cmc_string PFX##_to_string(SNAME *_map_)                                                \
    {                                                                                            \
        cmc_string str;                                                                          \
        SNAME *m_ = _map_;                                                                       \
        const char *name = #SNAME;                                                               \
                                                                                                 \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_hashmap,                                  \
                 name, m_, m_->buffer, m_->capacity, m_->count, m_->load, m_->cmp, m_->hash);    \
                                                                                                 \
        return str;                                                                              \
    }                                                                                            \
                                                                                                 \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target)                                             \
    {                                                                                            \
        SNAME##_iter *iter = malloc(sizeof(SNAME##_iter));                                       \
                                                                                                 \
        if (!iter)                                                                               \
            return NULL;                                                                         \
                                                                                                 \
        PFX##_iter_init(iter, target);                                                           \
                                                                                                 \
        return iter;                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD void PFX##_iter_free(SNAME##_iter *iter)                                                \
    {                                                                                            \
        free(iter);                                                                              \
    }                                                                                            \
                                                                                                 \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                                 \
    {                                                                                            \
        iter->target = target;                                                                   \
        iter->cursor = 0;                                                                        \
        iter->index = 0;                                                                         \
        iter->start = true;                                                                      \
        iter->end = PFX##_empty(target);                                                         \
                                                                                                 \
        if (!PFX##_empty(target))                                                                \
        {                                                                                        \
            for (size_t i = 0; i < target->capacity; i++)                                        \
            {                                                                                    \
                if (target->buffer[i][0] != NULL)                                                \
                {                                                                                \
                    iter->curr_entry = target->buffer[i][0];                                     \
                    iter->first = i;                                                             \
                    break;                                                                       \
                }                                                                                \
            }                                                                                    \
                                                                                                 \
            iter->cursor = iter->first;                                                          \
                                                                                                 \
            for (size_t i = target->capacity; i > 0; i--)                                        \
            {                                                                                    \
                if (target->buffer[i - 1][0] != NULL)                                            \
                {                                                                                \
                    iter->last = i - 1;                                                          \
                    break;                                                                       \
                }                                                                                \
            }                                                                                    \
        }                                                                                        \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                               \
    {                                                                                            \
        return PFX##_empty(iter->target) || iter->start;                                         \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                                 \
    {                                                                                            \
        return PFX##_empty(iter->target) || iter->end;                                           \
    }                                                                                            \
                                                                                                 \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter)                                            \
    {                                                                                            \
        iter->cursor = iter->first;                                                              \
        iter->index = 0;                                                                         \
        iter->start = true;                                                                      \
        iter->end = PFX##_empty(iter->target);                                                   \
        iter->curr_entry = iter->target->buffer[iter->first][0];                                 \
    }                                                                                            \
                                                                                                 \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter)                                              \
    {                                                                                            \
        iter->cursor = iter->last;                                                               \
        iter->index = iter->target->count - 1;                                                   \
        iter->start = PFX##_empty(iter->target);                                                 \
        iter->end = true;                                                                        \
        iter->curr_entry = iter->target->buffer[iter->last][1];                                  \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter)                                                \
    {                                                                                            \
        if (iter->end)                                                                           \
            return false;                                                                        \
                                                                                                 \
        if (iter->curr_entry->next != NULL)                                                      \
        {                                                                                        \
            iter->curr_entry = iter->curr_entry->next;                                           \
            iter->index++;                                                                       \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            if (iter->cursor == iter->last)                                                      \
            {                                                                                    \
                iter->end = true;                                                                \
                return false;                                                                    \
            }                                                                                    \
            else                                                                                 \
            {                                                                                    \
                iter->cursor++;                                                                  \
                                                                                                 \
                while (iter->target->buffer[iter->cursor][0] == NULL)                            \
                    iter->cursor++;                                                              \
                                                                                                 \
                iter->curr_entry = iter->target->buffer[iter->cursor][0];                        \
                                                                                                 \
                iter->index++;                                                                   \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        iter->start = PFX##_empty(iter->target);                                                 \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter)                                                \
    {                                                                                            \
        if (iter->start)                                                                         \
            return false;                                                                        \
                                                                                                 \
        if (iter->curr_entry->prev != NULL)                                                      \
        {                                                                                        \
            iter->curr_entry = iter->curr_entry->prev;                                           \
            iter->index--;                                                                       \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            if (iter->cursor == iter->first)                                                     \
            {                                                                                    \
                iter->start = true;                                                              \
                return false;                                                                    \
            }                                                                                    \
            else                                                                                 \
            {                                                                                    \
                iter->cursor--;                                                                  \
                                                                                                 \
                while (iter->target->buffer[iter->cursor][1] == NULL)                            \
                    iter->cursor--;                                                              \
                                                                                                 \
                iter->curr_entry = iter->target->buffer[iter->cursor][1];                        \
                                                                                                 \
                iter->index--;                                                                   \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        iter->end = PFX##_empty(iter->target);                                                   \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD K PFX##_iter_key(SNAME##_iter *iter)                                                    \
    {                                                                                            \
        if (PFX##_empty(iter->target))                                                           \
            return PFX##_impl_default_key();                                                     \
                                                                                                 \
        return iter->curr_entry->key;                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD V PFX##_iter_value(SNAME##_iter *iter)                                                  \
    {                                                                                            \
        if (PFX##_empty(iter->target))                                                           \
            return PFX##_impl_default_value();                                                   \
                                                                                                 \
        return iter->curr_entry->value;                                                          \
    }                                                                                            \
                                                                                                 \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter)                                                \
    {                                                                                            \
        if (PFX##_empty(iter->target))                                                           \
            return NULL;                                                                         \
                                                                                                 \
        return &(iter->curr_entry->value);                                                       \
    }                                                                                            \
                                                                                                 \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter)                                             \
    {                                                                                            \
        return iter->index;                                                                      \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_impl_grow(SNAME *_map_)                                                      \
    {                                                                                            \
        size_t new_capacity = PFX##_impl_calculate_size(_map_->capacity * 1.5);                  \
                                                                                                 \
        SNAME *_new_map_ = PFX##_new(new_capacity, _map_->load, _map_->cmp, _map_->hash);        \
                                                                                                 \
        if (!_new_map_)                                                                          \
            return false;                                                                        \
                                                                                                 \
        SNAME##_iter iter;                                                                       \
                                                                                                 \
        for (PFX##_iter_init(&iter, _map_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))      \
        {                                                                                        \
            K key = PFX##_iter_key(&iter);                                                       \
            V value = PFX##_iter_value(&iter);                                                   \
                                                                                                 \
            PFX##_insert(_new_map_, key, value);                                                 \
        }                                                                                        \
                                                                                                 \
        if (_map_->count != _new_map_->count)                                                    \
        {                                                                                        \
            PFX##_free(_new_map_);                                                               \
            return false;                                                                        \
        }                                                                                        \
                                                                                                 \
        SNAME##_entry *(*tmp_b)[2] = _map_->buffer;                                              \
        _map_->buffer = _new_map_->buffer;                                                       \
        _new_map_->buffer = tmp_b;                                                               \
                                                                                                 \
        size_t tmp_c = _map_->capacity;                                                          \
        _map_->capacity = _new_map_->capacity;                                                   \
        _new_map_->capacity = tmp_c;                                                             \
                                                                                                 \
        PFX##_free(_new_map_);                                                                   \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD SNAME##_entry *PFX##_impl_new_entry(K key, V value)                                     \
    {                                                                                            \
        SNAME##_entry *entry = malloc(sizeof(SNAME##_entry));                                    \
                                                                                                 \
        if (!entry)                                                                              \
            return NULL;                                                                         \
                                                                                                 \
        entry->key = key;                                                                        \
        entry->value = value;                                                                    \
        entry->next = NULL;                                                                      \
        entry->prev = NULL;                                                                      \
                                                                                                 \
        return entry;                                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD SNAME##_entry *PFX##_impl_get_entry(SNAME *_map_, K key)                                \
    {                                                                                            \
        size_t hash = _map_->hash(key);                                                          \
                                                                                                 \
        SNAME##_entry *entry = _map_->buffer[hash % _map_->capacity][0];                         \
                                                                                                 \
        while (entry != NULL)                                                                    \
        {                                                                                        \
            if (_map_->cmp(entry->key, key) == 0)                                                \
                return entry;                                                                    \
                                                                                                 \
            entry = entry->next;                                                                 \
        }                                                                                        \
                                                                                                 \
        return NULL;                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD size_t PFX##_impl_calculate_size(size_t required)                                       \
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
    static SNAME##_iter PFX##_impl_it_start(SNAME *_map_)                                        \
    {                                                                                            \
        SNAME##_iter iter;                                                                       \
                                                                                                 \
        PFX##_iter_init(&iter, _map_);                                                           \
                                                                                                 \
        return iter;                                                                             \
    }                                                                                            \
                                                                                                 \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_map_)                                          \
    {                                                                                            \
        SNAME##_iter iter;                                                                       \
                                                                                                 \
        PFX##_iter_init(&iter, _map_);                                                           \
        PFX##_iter_to_end(&iter);                                                                \
                                                                                                 \
        return iter;                                                                             \
    }

#endif /* CMC_MULTIMAP_H */
