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
 * mapped to a value. The keys are not sorted. It is implemented as a hashtable
 * with robin hood hashing.
 */

#ifndef CMC_HASHMAP_H
#define CMC_HASHMAP_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

#ifndef CMC_HASH_TABLE_SETUP
#define CMC_HASH_TABLE_SETUP

typedef enum cmc_entry_state_e
{
    CMC_ES_DELETED = -1,
    CMC_ES_EMPTY = 0,
    CMC_ES_FILLED = 1
} cmc_entry_state;

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

#define HASHMAP_GENERATE(PFX, SNAME, FMOD, K, V)    \
    HASHMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V) \
    HASHMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)

#define HASHMAP_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V) \
    HASHMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)

#define HASHMAP_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V) \
    HASHMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)

/* HEADER ********************************************************************/
#define HASHMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)                                           \
                                                                                                  \
    /* Hashmap Structure */                                                                       \
    typedef struct SNAME##_s                                                                      \
    {                                                                                             \
        /* Array of Entries */                                                                    \
        struct SNAME##_entry_s *buffer;                                                           \
                                                                                                  \
        /* Current array capacity */                                                              \
        size_t capacity;                                                                          \
                                                                                                  \
        /* Current amount of keys */                                                              \
        size_t count;                                                                             \
                                                                                                  \
        /* Load factor in range (0.0, 1.0) */                                                     \
        double load;                                                                              \
                                                                                                  \
        /* Key comparison function */                                                             \
        int (*cmp)(K, K);                                                                         \
                                                                                                  \
        /* Key hash function */                                                                   \
        size_t (*hash)(K);                                                                        \
                                                                                                  \
        /* Function that returns an iterator to the start of the hashmap */                       \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                                    \
                                                                                                  \
        /* Function that returns an iterator to the end of the hashmap */                         \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                                      \
                                                                                                  \
    } SNAME, *SNAME##_ptr;                                                                        \
                                                                                                  \
    /* Hashmap Entry */                                                                           \
    typedef struct SNAME##_entry_s                                                                \
    {                                                                                             \
        /* Entry Key */                                                                           \
        K key;                                                                                    \
                                                                                                  \
        /* Entry Value */                                                                         \
        V value;                                                                                  \
                                                                                                  \
        /* The distance of this node to its original position, used by robin-hood hashing */      \
        size_t dist;                                                                              \
                                                                                                  \
        /* The sate of this node (DELETED, EMPTY, FILLED) */                                      \
        enum cmc_entry_state_e state;                                                             \
                                                                                                  \
    } SNAME##_entry, *SNAME##_entry_ptr;                                                          \
                                                                                                  \
    /* Hashmap Iterator */                                                                        \
    typedef struct SNAME##_iter_s                                                                 \
    {                                                                                             \
        /* Target hashmap */                                                                      \
        struct SNAME##_s *target;                                                                 \
                                                                                                  \
        /* Cursor's position (index) */                                                           \
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
                                                                                                  \
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
    FMOD bool PFX##_remove(SNAME *_map_, K key, V *value);                                        \
    /* Conditional Input and Output */                                                            \
    FMOD bool PFX##_insert_if(SNAME *_map_, K key, V value, bool condition);                      \
    FMOD bool PFX##_remove_if(SNAME *_map_, K key, V *value, bool condition);                     \
    /* Element Access */                                                                          \
    FMOD bool PFX##_max(SNAME *_map_, K *key, V *value);                                          \
    FMOD bool PFX##_min(SNAME *_map_, K *key, V *value);                                          \
    FMOD V PFX##_get(SNAME *_map_, K key);                                                        \
    FMOD V *PFX##_get_ref(SNAME *_map_, K key);                                                   \
    FMOD bool PFX##_set(SNAME *_map_, K key, V new_value);                                        \
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
                                                                                                  \
/* SOURCE ********************************************************************/
#define HASHMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)                                          \
                                                                                                 \
    /* Implementation Detail Functions */                                                        \
    static bool PFX##_impl_grow(SNAME *_map_);                                                   \
    static SNAME##_entry *PFX##_impl_get_entry(SNAME *_map_, K key);                             \
    static size_t PFX##_impl_calculate_size(size_t required);                                    \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_map_);                                       \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_map_);                                         \
                                                                                                 \
    FMOD SNAME *PFX##_new(size_t capacity, double load, int (*compare)(K, K), size_t (*hash)(K)) \
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
        SNAME *_map_ = malloc(sizeof(SNAME));                                                    \
                                                                                                 \
        if (!_map_)                                                                              \
            return NULL;                                                                         \
                                                                                                 \
        _map_->buffer = malloc(sizeof(SNAME##_entry) * real_capacity);                           \
                                                                                                 \
        if (!_map_->buffer)                                                                      \
        {                                                                                        \
            free(_map_);                                                                         \
            return NULL;                                                                         \
        }                                                                                        \
                                                                                                 \
        memset(_map_->buffer, 0, sizeof(SNAME##_entry) * real_capacity);                         \
                                                                                                 \
        _map_->count = 0;                                                                        \
        _map_->capacity = real_capacity;                                                         \
        _map_->load = load;                                                                      \
        _map_->cmp = compare;                                                                    \
        _map_->hash = hash;                                                                      \
                                                                                                 \
        _map_->it_start = PFX##_impl_it_start;                                                   \
        _map_->it_end = PFX##_impl_it_end;                                                       \
                                                                                                 \
        return _map_;                                                                            \
    }                                                                                            \
                                                                                                 \
    FMOD void PFX##_clear(SNAME *_map_)                                                          \
    {                                                                                            \
        memset(_map_->buffer, 0, sizeof(SNAME##_entry) * _map_->capacity);                       \
                                                                                                 \
        _map_->count = 0;                                                                        \
    }                                                                                            \
                                                                                                 \
    FMOD void PFX##_free(SNAME *_map_)                                                           \
    {                                                                                            \
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
        size_t original_pos = hash % _map_->capacity;                                            \
        size_t pos = original_pos;                                                               \
                                                                                                 \
        SNAME##_entry *target = &(_map_->buffer[pos]);                                           \
                                                                                                 \
        if (PFX##_impl_get_entry(_map_, key) != NULL)                                            \
            return false;                                                                        \
                                                                                                 \
        if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)                    \
        {                                                                                        \
            target->key = key;                                                                   \
            target->value = value;                                                               \
            target->dist = original_pos - pos;                                                   \
            target->state = CMC_ES_FILLED;                                                       \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            while (true)                                                                         \
            {                                                                                    \
                pos++;                                                                           \
                target = &(_map_->buffer[pos % _map_->capacity]);                                \
                                                                                                 \
                if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)            \
                {                                                                                \
                    target->key = key;                                                           \
                    target->value = value;                                                       \
                    target->dist = pos - original_pos;                                           \
                    target->state = CMC_ES_FILLED;                                               \
                                                                                                 \
                    break;                                                                       \
                }                                                                                \
                else if (target->dist < original_pos - pos)                                      \
                {                                                                                \
                    K tmp_k = target->key;                                                       \
                    V tmp_v = target->value;                                                     \
                    size_t tmp_dist = target->dist;                                              \
                                                                                                 \
                    target->key = key;                                                           \
                    target->value = value;                                                       \
                    target->dist = pos - original_pos;                                           \
                                                                                                 \
                    key = tmp_k;                                                                 \
                    value = tmp_v;                                                               \
                    original_pos = pos - tmp_dist;                                               \
                }                                                                                \
            }                                                                                    \
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
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_remove(SNAME *_map_, K key, V *value)                                        \
    {                                                                                            \
        SNAME##_entry *result = PFX##_impl_get_entry(_map_, key);                                \
                                                                                                 \
        if (result == NULL)                                                                      \
            return false;                                                                        \
                                                                                                 \
        *value = result->value;                                                                  \
                                                                                                 \
        result->key = PFX##_impl_default_key();                                                  \
        result->value = PFX##_impl_default_value();                                              \
        result->dist = 0;                                                                        \
        result->state = CMC_ES_DELETED;                                                          \
                                                                                                 \
        _map_->count--;                                                                          \
                                                                                                 \
        return true;                                                                             \
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
        return true;                                                                             \
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
                if (target->buffer[i].state == CMC_ES_FILLED)                                    \
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
                if (target->buffer[i - 1].state == CMC_ES_FILLED)                                \
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
    }                                                                                            \
                                                                                                 \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter)                                              \
    {                                                                                            \
        iter->cursor = iter->last;                                                               \
        iter->index = iter->target->count - 1;                                                   \
        iter->start = PFX##_empty(iter->target);                                                 \
        iter->end = true;                                                                        \
    }                                                                                            \
                                                                                                 \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter)                                                \
    {                                                                                            \
        if (iter->end)                                                                           \
            return false;                                                                        \
                                                                                                 \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                             \
                                                                                                 \
        if (iter->cursor == iter->last)                                                          \
        {                                                                                        \
            iter->end = true;                                                                    \
                                                                                                 \
            return false;                                                                        \
        }                                                                                        \
                                                                                                 \
        iter->index++;                                                                           \
                                                                                                 \
        while (1)                                                                                \
        {                                                                                        \
            iter->cursor++;                                                                      \
            scan = &(iter->target->buffer[iter->cursor]);                                        \
                                                                                                 \
            if (scan->state == CMC_ES_FILLED)                                                    \
                break;                                                                           \
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
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                             \
                                                                                                 \
        if (iter->cursor == iter->first)                                                         \
        {                                                                                        \
            iter->start = true;                                                                  \
                                                                                                 \
            return false;                                                                        \
        }                                                                                        \
                                                                                                 \
        iter->index--;                                                                           \
                                                                                                 \
        while (1)                                                                                \
        {                                                                                        \
            iter->cursor--;                                                                      \
            scan = &(iter->target->buffer[iter->cursor]);                                        \
                                                                                                 \
            if (scan->state == CMC_ES_FILLED)                                                    \
                break;                                                                           \
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
        return iter->target->buffer[iter->cursor].key;                                           \
    }                                                                                            \
                                                                                                 \
    FMOD V PFX##_iter_value(SNAME##_iter *iter)                                                  \
    {                                                                                            \
        if (PFX##_empty(iter->target))                                                           \
            return PFX##_impl_default_value();                                                   \
                                                                                                 \
        return iter->target->buffer[iter->cursor].value;                                         \
    }                                                                                            \
                                                                                                 \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter)                                                \
    {                                                                                            \
        if (PFX##_empty(iter->target))                                                           \
            return NULL;                                                                         \
                                                                                                 \
        return &(iter->target->buffer[iter->cursor].value);                                      \
    }                                                                                            \
                                                                                                 \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter)                                             \
    {                                                                                            \
        return iter->index;                                                                      \
    }                                                                                            \
                                                                                                 \
    static bool PFX##_impl_grow(SNAME *_map_)                                                    \
    {                                                                                            \
        size_t new_size = PFX##_impl_calculate_size(_map_->capacity + _map_->capacity / 2);      \
                                                                                                 \
        SNAME *_new_map_ = PFX##_new(new_size, _map_->load, _map_->cmp, _map_->hash);            \
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
        SNAME##_entry *tmp = _map_->buffer;                                                      \
        _map_->buffer = _new_map_->buffer;                                                       \
        _new_map_->buffer = tmp;                                                                 \
                                                                                                 \
        _map_->capacity = _new_map_->capacity;                                                   \
                                                                                                 \
        PFX##_free(_new_map_);                                                                   \
                                                                                                 \
        return true;                                                                             \
    }                                                                                            \
                                                                                                 \
    static SNAME##_entry *PFX##_impl_get_entry(SNAME *_map_, K key)                              \
    {                                                                                            \
        size_t hash = _map_->hash(key);                                                          \
        size_t pos = hash % _map_->capacity;                                                     \
                                                                                                 \
        SNAME##_entry *target = &(_map_->buffer[pos]);                                           \
                                                                                                 \
        while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)                \
        {                                                                                        \
            if (_map_->cmp(target->key, key) == 0)                                               \
                return target;                                                                   \
                                                                                                 \
            pos++;                                                                               \
            target = &(_map_->buffer[pos % _map_->capacity]);                                    \
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

#endif /* CMC_HASHMAP_H */
