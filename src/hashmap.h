/**
 * hashmap.h
 *
 * Creation Date: 03/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/******************************************************************* HASHMAP */
/*****************************************************************************/

#ifndef CMC_HASHMAP_H
#define CMC_HASHMAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef CMC_HASH_TABLE_SETUP
#define CMC_HASH_TABLE_SETUP

typedef enum EntryState_e
{
    ES_DELETED = -1,
    ES_EMPTY = 0,
    ES_FILLED = 1
} EntryState;

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
    HASHMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
    HASHMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V) \
    HASHMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)

/* PRIVATE *******************************************************************/
#define HASHMAP_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    HASHMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define HASHMAP_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    HASHMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)             \
    HASHMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* PUBLIC ********************************************************************/
#define HASHMAP_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    HASHMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)            \
    HASHMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define HASHMAP_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    HASHMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* STRUCT ********************************************************************/
#define HASHMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
                                                        \
    struct SNAME##_s                                    \
    {                                                   \
        struct SNAME##_entry_s *buffer;                 \
        size_t capacity;                                \
        size_t count;                                   \
        double load;                                    \
        int (*cmp)(K, K);                               \
        size_t (*hash)(K);                              \
    };                                                  \
                                                        \
    struct SNAME##_entry_s                              \
    {                                                   \
        K key;                                          \
        V value;                                        \
        size_t dist;                                    \
        enum EntryState_e state;                        \
    };                                                  \
                                                        \
    struct SNAME##_iter_s                               \
    {                                                   \
        struct SNAME##_s *target;                       \
        size_t cursor;                                  \
        size_t index;                                   \
        size_t first;                                   \
        size_t last;                                    \
        bool start;                                     \
        bool end;                                       \
    };                                                  \
                                                        \
/* HEADER ********************************************************************/
#define HASHMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)                                       \
                                                                                              \
    typedef struct SNAME##_s SNAME;                                                           \
    typedef struct SNAME##_entry_s SNAME##_entry;                                             \
    typedef struct SNAME##_iter_s SNAME##_iter;                                               \
                                                                                              \
    FMOD SNAME *PFX##_new(size_t size, double load, int (*compare)(K, K), size_t (*hash)(K)); \
    FMOD void PFX##_clear(SNAME *_map_);                                                      \
    FMOD void PFX##_free(SNAME *_map_);                                                       \
    FMOD bool PFX##_insert(SNAME *_map_, K key, V value);                                     \
    FMOD bool PFX##_remove(SNAME *_map_, K key, V *value);                                    \
    FMOD bool PFX##_insert_if(SNAME *_map_, K key, V value, bool condition);                  \
    FMOD bool PFX##_remove_if(SNAME *_map_, K key, V *value, bool condition);                 \
    FMOD bool PFX##_max(SNAME *_map_, K *key, V *value);                                      \
    FMOD bool PFX##_min(SNAME *_map_, K *key, V *value);                                      \
    FMOD bool PFX##_contains(SNAME *_map_, K key);                                            \
    FMOD V PFX##_get(SNAME *_map_, K key);                                                    \
    FMOD bool PFX##_empty(SNAME *_map_);                                                      \
    FMOD size_t PFX##_count(SNAME *_map_);                                                    \
                                                                                              \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);                              \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                           \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                             \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                                         \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                                           \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, K *key, V *value, size_t *index);           \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, K *key, V *value, size_t *index);           \
                                                                                              \
/* SOURCE ********************************************************************/
#define HASHMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)                                      \
                                                                                             \
    FMOD bool PFX##_grow(SNAME *_map_);                                                      \
    FMOD SNAME##_entry *PFX##_get_entry(SNAME *_map_, K key);                                \
    FMOD size_t PFX##_calculate_size(size_t required);                                       \
                                                                                             \
    FMOD SNAME *PFX##_new(size_t size, double load, int (*compare)(K, K), size_t (*hash)(K)) \
    {                                                                                        \
        if (size == 0 || load <= 0 || load >= 1)                                             \
            return NULL;                                                                     \
                                                                                             \
        size_t real_size = PFX##_calculate_size(size);                                       \
                                                                                             \
        SNAME *_map_ = malloc(sizeof(SNAME));                                                \
                                                                                             \
        if (!_map_)                                                                          \
            return NULL;                                                                     \
                                                                                             \
        _map_->buffer = malloc(sizeof(SNAME##_entry) * real_size);                           \
                                                                                             \
        if (!_map_->buffer)                                                                  \
        {                                                                                    \
            free(_map_);                                                                     \
            return NULL;                                                                     \
        }                                                                                    \
                                                                                             \
        memset(_map_->buffer, 0, sizeof(SNAME##_entry) * real_size);                         \
                                                                                             \
        _map_->count = 0;                                                                    \
        _map_->capacity = real_size;                                                         \
        _map_->load = load;                                                                  \
        _map_->cmp = compare;                                                                \
        _map_->hash = hash;                                                                  \
                                                                                             \
        return _map_;                                                                        \
    }                                                                                        \
                                                                                             \
    FMOD void PFX##_clear(SNAME *_map_)                                                      \
    {                                                                                        \
        memset(_map_->buffer, 0, sizeof(SNAME##_entry) * _map_->capacity);                   \
                                                                                             \
        _map_->count = 0;                                                                    \
    }                                                                                        \
                                                                                             \
    FMOD void PFX##_free(SNAME *_map_)                                                       \
    {                                                                                        \
        free(_map_->buffer);                                                                 \
        free(_map_);                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_insert(SNAME *_map_, K key, V value)                                     \
    {                                                                                        \
        if ((double)_map_->capacity * _map_->load <= (double)_map_->count)                   \
        {                                                                                    \
            if (!PFX##_grow(_map_))                                                          \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        size_t hash = _map_->hash(key);                                                      \
        size_t original_pos = hash % _map_->capacity;                                        \
        size_t pos = original_pos;                                                           \
                                                                                             \
        SNAME##_entry *target = &(_map_->buffer[pos]);                                       \
                                                                                             \
        if (PFX##_get_entry(_map_, key) != NULL)                                             \
            return false;                                                                    \
                                                                                             \
        if (target->state == ES_EMPTY || target->state == ES_DELETED)                        \
        {                                                                                    \
            target->key = key;                                                               \
            target->value = value;                                                           \
            target->dist = original_pos - pos;                                               \
            target->state = ES_FILLED;                                                       \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            while (true)                                                                     \
            {                                                                                \
                pos++;                                                                       \
                target = &(_map_->buffer[pos % _map_->capacity]);                            \
                                                                                             \
                if (target->state == ES_EMPTY || target->state == ES_DELETED)                \
                {                                                                            \
                    target->key = key;                                                       \
                    target->value = value;                                                   \
                    target->dist = pos - original_pos;                                       \
                    target->state = ES_FILLED;                                               \
                                                                                             \
                    break;                                                                   \
                }                                                                            \
                else if (target->dist < original_pos - pos)                                  \
                {                                                                            \
                    K tmp_k = target->key;                                                   \
                    V tmp_v = target->value;                                                 \
                    size_t tmp_dist = target->dist;                                          \
                                                                                             \
                    target->key = key;                                                       \
                    target->value = value;                                                   \
                    target->dist = pos - original_pos;                                       \
                                                                                             \
                    key = tmp_k;                                                             \
                    value = tmp_v;                                                           \
                    original_pos = pos - tmp_dist;                                           \
                }                                                                            \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        _map_->count++;                                                                      \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_remove(SNAME *_map_, K key, V *value)                                    \
    {                                                                                        \
        SNAME##_entry *result = PFX##_get_entry(_map_, key);                                 \
                                                                                             \
        if (result == NULL)                                                                  \
            return false;                                                                    \
                                                                                             \
        *value = result->value;                                                              \
                                                                                             \
        result->value = 0;                                                                   \
        result->dist = 0;                                                                    \
        result->state = ES_DELETED;                                                          \
                                                                                             \
        _map_->count--;                                                                      \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_insert_if(SNAME *_map_, K key, V value, bool condition)                  \
    {                                                                                        \
        if (condition)                                                                       \
            return PFX##_insert(_map_, key, value);                                          \
                                                                                             \
        return false;                                                                        \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_remove_if(SNAME *_map_, K key, V *value, bool condition)                 \
    {                                                                                        \
        if (condition)                                                                       \
            return PFX##_remove(_map_, key, value);                                          \
                                                                                             \
        return false;                                                                        \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_max(SNAME *_map_, K *key, V *value)                                      \
    {                                                                                        \
        if (PFX##_empty(_map_))                                                              \
            return false;                                                                    \
                                                                                             \
        SNAME##_iter iter;                                                                   \
        K result_key, max_key;                                                               \
        V result_value;                                                                      \
        size_t index;                                                                        \
                                                                                             \
        for (PFX##_iter_new(&iter, _map_); !PFX##_iter_end(&iter);)                          \
        {                                                                                    \
            PFX##_iter_next(&iter, &result_key, &result_value, &index);                      \
                                                                                             \
            if (index == 0)                                                                  \
            {                                                                                \
                max_key = result_key;                                                        \
                *key = result_key;                                                           \
                *value = result_value;                                                       \
            }                                                                                \
            else if (_map_->cmp(result_key, max_key) > 0)                                    \
            {                                                                                \
                max_key = result_key;                                                        \
                *key = result_key;                                                           \
                *value = result_value;                                                       \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_min(SNAME *_map_, K *key, V *value)                                      \
    {                                                                                        \
        if (PFX##_empty(_map_))                                                              \
            return false;                                                                    \
                                                                                             \
        SNAME##_iter iter;                                                                   \
        K result_key, min_key;                                                               \
        V result_value;                                                                      \
        size_t index;                                                                        \
                                                                                             \
        for (PFX##_iter_new(&iter, _map_); !PFX##_iter_end(&iter);)                          \
        {                                                                                    \
            PFX##_iter_next(&iter, &result_key, &result_value, &index);                      \
                                                                                             \
            if (index == 0)                                                                  \
            {                                                                                \
                min_key = result_key;                                                        \
                *key = result_key;                                                           \
                *value = result_value;                                                       \
            }                                                                                \
            else if (_map_->cmp(result_key, min_key) < 0)                                    \
            {                                                                                \
                min_key = result_key;                                                        \
                *key = min_key;                                                              \
                *value = result_value;                                                       \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_contains(SNAME *_map_, K key)                                            \
    {                                                                                        \
        return PFX##_get_entry(_map_, key) != NULL;                                          \
    }                                                                                        \
                                                                                             \
    FMOD V PFX##_get(SNAME *_map_, K key)                                                    \
    {                                                                                        \
        SNAME##_entry *entry = PFX##_get_entry(_map_, key);                                  \
                                                                                             \
        if (!entry)                                                                          \
            return 0;                                                                        \
                                                                                             \
        return entry->value;                                                                 \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_empty(SNAME *_map_)                                                      \
    {                                                                                        \
        return _map_->count == 0;                                                            \
    }                                                                                        \
                                                                                             \
    FMOD size_t PFX##_count(SNAME *_map_)                                                    \
    {                                                                                        \
        return _map_->count;                                                                 \
    }                                                                                        \
                                                                                             \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target)                              \
    {                                                                                        \
        iter->target = target;                                                               \
        iter->cursor = 0;                                                                    \
        iter->index = 0;                                                                     \
        iter->start = true;                                                                  \
        iter->end = PFX##_empty(target);                                                     \
                                                                                             \
        if (!PFX##_empty(target))                                                            \
        {                                                                                    \
            for (size_t i = 0; i < target->capacity; i++)                                    \
            {                                                                                \
                if (target->buffer[i].state == ES_FILLED)                                    \
                {                                                                            \
                    iter->first = i;                                                         \
                    break;                                                                   \
                }                                                                            \
            }                                                                                \
                                                                                             \
            iter->cursor = iter->first;                                                      \
                                                                                             \
            for (size_t i = target->capacity; i > 0; i--)                                    \
            {                                                                                \
                if (target->buffer[i - 1].state == ES_FILLED)                                \
                {                                                                            \
                    iter->last = i - 1;                                                      \
                    break;                                                                   \
                }                                                                            \
            }                                                                                \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                           \
    {                                                                                        \
        return PFX##_empty(iter->target) || iter->start;                                     \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                             \
    {                                                                                        \
        return PFX##_empty(iter->target) || iter->end;                                       \
    }                                                                                        \
                                                                                             \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter)                                         \
    {                                                                                        \
        iter->cursor = iter->first;                                                          \
        iter->index = 0;                                                                     \
        iter->start = true;                                                                  \
        iter->end = PFX##_empty(iter->target);                                               \
    }                                                                                        \
                                                                                             \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter)                                           \
    {                                                                                        \
        iter->cursor = iter->last;                                                           \
        iter->index = iter->target->count - 1;                                               \
        iter->start = PFX##_empty(iter->target);                                             \
        iter->end = true;                                                                    \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, K *key, V *value, size_t *index)           \
    {                                                                                        \
        if (iter->end)                                                                       \
            return false;                                                                    \
                                                                                             \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                         \
                                                                                             \
        *key = scan->key;                                                                    \
        *value = scan->value;                                                                \
        *index = iter->index;                                                                \
                                                                                             \
        if (iter->cursor == iter->last)                                                      \
            iter->end = true;                                                                \
        else                                                                                 \
        {                                                                                    \
            iter->index++;                                                                   \
                                                                                             \
            while (1)                                                                        \
            {                                                                                \
                iter->cursor++;                                                              \
                scan = &(iter->target->buffer[iter->cursor]);                                \
                                                                                             \
                if (scan->state == ES_FILLED)                                                \
                    break;                                                                   \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        iter->start = PFX##_empty(iter->target);                                             \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, K *key, V *value, size_t *index)           \
    {                                                                                        \
        if (iter->start)                                                                     \
            return false;                                                                    \
                                                                                             \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                         \
                                                                                             \
        *key = scan->key;                                                                    \
        *value = scan->value;                                                                \
        *index = iter->index;                                                                \
                                                                                             \
        if (iter->cursor == iter->first)                                                     \
            iter->start = true;                                                              \
        else                                                                                 \
        {                                                                                    \
            iter->index--;                                                                   \
                                                                                             \
            while (1)                                                                        \
            {                                                                                \
                iter->cursor--;                                                              \
                scan = &(iter->target->buffer[iter->cursor]);                                \
                                                                                             \
                if (scan->state == ES_FILLED)                                                \
                    break;                                                                   \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        iter->end = PFX##_empty(iter->target);                                               \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_grow(SNAME *_map_)                                                       \
    {                                                                                        \
        size_t new_size = PFX##_calculate_size(_map_->capacity + _map_->capacity / 2);       \
                                                                                             \
        SNAME *_new_map_ = PFX##_new(new_size, _map_->load, _map_->cmp, _map_->hash);        \
                                                                                             \
        if (!_new_map_)                                                                      \
            return false;                                                                    \
                                                                                             \
        K key;                                                                               \
        V value;                                                                             \
        size_t index;                                                                        \
        SNAME##_iter iter;                                                                   \
                                                                                             \
        for (PFX##_iter_new(&iter, _map_); !PFX##_iter_end(&iter);)                          \
        {                                                                                    \
            PFX##_iter_next(&iter, &key, &value, &index);                                    \
            PFX##_insert(_new_map_, key, value);                                             \
        }                                                                                    \
                                                                                             \
        if (_map_->count != _new_map_->count)                                                \
        {                                                                                    \
            PFX##_free(_new_map_);                                                           \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        SNAME##_entry *tmp = _map_->buffer;                                                  \
        _map_->buffer = _new_map_->buffer;                                                   \
        _new_map_->buffer = tmp;                                                             \
                                                                                             \
        _map_->capacity = _new_map_->capacity;                                               \
                                                                                             \
        PFX##_free(_new_map_);                                                               \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD SNAME##_entry *PFX##_get_entry(SNAME *_map_, K key)                                 \
    {                                                                                        \
        size_t hash = _map_->hash(key);                                                      \
        size_t pos = hash % _map_->capacity;                                                 \
                                                                                             \
        SNAME##_entry *target = &(_map_->buffer[pos % _map_->capacity]);                     \
                                                                                             \
        while (target->state == ES_FILLED || target->state == ES_DELETED)                    \
        {                                                                                    \
            if (_map_->cmp(target->key, key) == 0)                                           \
                return target;                                                               \
                                                                                             \
            pos++;                                                                           \
            target = &(_map_->buffer[pos % _map_->capacity]);                                \
        }                                                                                    \
                                                                                             \
        return NULL;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD size_t PFX##_calculate_size(size_t required)                                        \
    {                                                                                        \
        const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]); \
                                                                                             \
        if (cmc_hashtable_primes[count - 1] < required)                                      \
            return required;                                                                 \
                                                                                             \
        size_t i = 0;                                                                        \
        while (cmc_hashtable_primes[i] < required)                                           \
            i++;                                                                             \
                                                                                             \
        return cmc_hashtable_primes[i];                                                      \
    }

#endif /* CMC_HASHMAP_H */
