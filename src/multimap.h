/**
 * multimap.h
 *
 * Creation Date: 26/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/******************************************************************* MULTIMAP */
/*****************************************************************************/

#ifndef CMC_MULTIMAP_H
#define CMC_MULTIMAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    MULTIMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
    MULTIMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V) \
    MULTIMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)

/* PRIVATE *******************************************************************/
#define MULTIMAP_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    MULTIMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define MULTIMAP_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    MULTIMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)             \
    MULTIMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* PUBLIC ********************************************************************/
#define MULTIMAP_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    MULTIMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)            \
    MULTIMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define MULTIMAP_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    MULTIMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* STRUCT ********************************************************************/
#define MULTIMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
                                                         \
    struct SNAME##_s                                     \
    {                                                    \
        struct SNAME##_entry_s **buffer;                 \
        size_t capacity;                                 \
        size_t count;                                    \
        double load;                                     \
        int (*cmp)(K, K);                                \
        size_t (*hash)(K);                               \
    };                                                   \
                                                         \
    struct SNAME##_entry_s                               \
    {                                                    \
        K key;                                           \
        V value;                                         \
        struct SNAME##_entry_s *next;                    \
        struct SNAME##_entry_s *prev;                    \
    };                                                   \
                                                         \
    struct SNAME##_iter_s                                \
    {                                                    \
        struct SNAME##_s *target;                        \
        struct SNAME##_entry_s *curr_entry;              \
        size_t cursor;                                   \
        size_t first;                                    \
        size_t last;                                     \
        size_t index;                                    \
        bool start;                                      \
        bool end;                                        \
    };                                                   \
                                                         \
/* HEADER ********************************************************************/
#define MULTIMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)                                      \
                                                                                              \
    typedef struct SNAME##_s SNAME;                                                           \
    typedef struct SNAME##_entry_s SNAME##_entry;                                             \
    typedef struct SNAME##_iter_s SNAME##_iter;                                               \
                                                                                              \
    FMOD SNAME *PFX##_new(size_t size, double load, int (*compare)(K, K), size_t (*hash)(K)); \
    FMOD void PFX##_clear(SNAME *_map_);                                                      \
    FMOD void PFX##_free(SNAME *_map_);                                                       \
    FMOD bool PFX##_insert(SNAME *_map_, K key, V value);                                     \
    FMOD bool PFX##_remove(SNAME *_map_, K key);                                              \
    FMOD size_t PFX##_remove_all(SNAME *_map_, K key);                                        \
    FMOD bool PFX##_insert_if(SNAME *_map_, K key, V value, bool condition);                  \
    FMOD bool PFX##_remove_if(SNAME *_map_, K key, bool condition);                           \
    FMOD bool PFX##_max(SNAME *_map_, K *key, V *value);                                      \
    FMOD bool PFX##_min(SNAME *_map_, K *key, V *value);                                      \
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
#define MULTIMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)                                     \
                                                                                             \
    FMOD bool PFX##_grow(SNAME *_map_);                                                      \
    FMOD SNAME##_entry *PFX##_new_entry(K key, V value);                                     \
    FMOD SNAME##_entry *PFX##_get_entry(SNAME *_map_, K key);                                \
    FMOD size_t PFX##_calculate_size(size_t required);                                       \
                                                                                             \
    FMOD SNAME *PFX##_new(size_t size, double load, int (*compare)(K, K), size_t (*hash)(K)) \
    {                                                                                        \
        if (size == 0 || load <= 0)                                                          \
            return NULL;                                                                     \
                                                                                             \
        size_t real_size = PFX##_calculate_size(size);                                       \
                                                                                             \
        SNAME *_map_ = malloc(sizeof(SNAME));                                                \
                                                                                             \
        if (!_map_)                                                                          \
            return NULL;                                                                     \
                                                                                             \
        _map_->buffer = malloc(sizeof(SNAME##_entry *) * real_size);                         \
                                                                                             \
        if (!_map_->buffer)                                                                  \
        {                                                                                    \
            free(_map_);                                                                     \
            return NULL;                                                                     \
        }                                                                                    \
                                                                                             \
        memset(_map_->buffer, 0, sizeof(SNAME##_entry *) * real_size);                       \
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
        size_t index = 0;                                                                    \
        SNAME##_entry *scan;                                                                 \
                                                                                             \
        while (index < _map_->capacity)                                                      \
        {                                                                                    \
            scan = _map_->buffer[index];                                                     \
                                                                                             \
            if (scan != NULL)                                                                \
            {                                                                                \
                if (scan->next == NULL && scan->prev == NULL)                                \
                    free(scan);                                                              \
                else                                                                         \
                {                                                                            \
                    while (scan != NULL)                                                     \
                    {                                                                        \
                        SNAME##_entry *tmp = scan;                                           \
                                                                                             \
                        scan = scan->next;                                                   \
                                                                                             \
                        free(tmp);                                                           \
                    }                                                                        \
                }                                                                            \
            }                                                                                \
                                                                                             \
            index++;                                                                         \
        }                                                                                    \
                                                                                             \
        memset(_map_->buffer, 0, sizeof(SNAME##_entry *) * _map_->capacity);                 \
                                                                                             \
        _map_->count = 0;                                                                    \
    }                                                                                        \
                                                                                             \
    FMOD void PFX##_free(SNAME *_map_)                                                       \
    {                                                                                        \
        PFX##_clear(_map_);                                                                  \
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
        size_t pos = hash % _map_->capacity;                                                 \
                                                                                             \
        SNAME##_entry **target = &(_map_->buffer[pos]);                                      \
                                                                                             \
        SNAME##_entry *entry = PFX##_new_entry(key, value);                                  \
                                                                                             \
        if (*target == NULL)                                                                 \
        {                                                                                    \
            *target = entry;                                                                 \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            entry->next = *target;                                                           \
            (*target)->prev = entry;                                                         \
                                                                                             \
            *target = entry;                                                                 \
        }                                                                                    \
                                                                                             \
        _map_->count++;                                                                      \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_remove(SNAME *_map_, K key)                                              \
    {                                                                                        \
        size_t hash = _map_->hash(key);                                                      \
                                                                                             \
        SNAME##_entry **target = &(_map_->buffer[hash % _map_->capacity]);                   \
                                                                                             \
        if (*target == NULL)                                                                 \
            return false;                                                                    \
                                                                                             \
        SNAME##_entry *entry = *target;                                                      \
                                                                                             \
        if (entry->next == NULL && entry->prev == NULL)                                      \
        {                                                                                    \
            if (_map_->cmp(entry->key, key) == 0)                                            \
                *target = NULL;                                                              \
            else                                                                             \
                return false;                                                                \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            bool found = false;                                                              \
                                                                                             \
            while (entry != NULL)                                                            \
            {                                                                                \
                if (_map_->cmp(entry->key, key) == 0)                                        \
                {                                                                            \
                    if (*target == entry)                                                    \
                    {                                                                        \
                        if (entry->prev != NULL)                                             \
                            *target = entry->prev;                                           \
                        else                                                                 \
                            *target = entry->next;                                           \
                    }                                                                        \
                                                                                             \
                    if (entry->prev != NULL)                                                 \
                        entry->prev->next = entry->next;                                     \
                    if (entry->next != NULL)                                                 \
                        entry->next->prev = entry->prev;                                     \
                                                                                             \
                    found = true;                                                            \
                                                                                             \
                    break;                                                                   \
                }                                                                            \
                else                                                                         \
                    entry = entry->next;                                                     \
            }                                                                                \
                                                                                             \
            if (!found)                                                                      \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        free(entry);                                                                         \
                                                                                             \
        _map_->count--;                                                                      \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD size_t PFX##_remove_all(SNAME *_map_, K key)                                        \
    {                                                                                        \
        size_t total = 0;                                                                    \
        size_t hash = _map_->hash(key);                                                      \
                                                                                             \
        SNAME##_entry **target = &(_map_->buffer[hash % _map_->capacity]);                   \
                                                                                             \
        if (*target == NULL)                                                                 \
            return total;                                                                    \
                                                                                             \
        SNAME##_entry *entry = *target;                                                      \
                                                                                             \
        if (entry->next == NULL)                                                             \
        {                                                                                    \
            *target = NULL;                                                                  \
                                                                                             \
            free(entry);                                                                     \
                                                                                             \
            total++;                                                                         \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            while (entry != NULL)                                                            \
            {                                                                                \
                if (_map_->cmp(entry->key, key) == 0)                                        \
                {                                                                            \
                    if (*target == entry)                                                    \
                    {                                                                        \
                        if (entry->prev != NULL)                                             \
                            *target = entry->prev;                                           \
                        else                                                                 \
                            *target = entry->next;                                           \
                    }                                                                        \
                                                                                             \
                    SNAME##_entry *tmp = entry->next;                                        \
                                                                                             \
                    if (entry->prev != NULL)                                                 \
                        entry->prev->next = entry->next;                                     \
                    if (entry->next != NULL)                                                 \
                        entry->next->prev = entry->prev;                                     \
                                                                                             \
                    if (entry->next == NULL && entry->prev == NULL)                          \
                        *target = NULL;                                                      \
                                                                                             \
                    free(entry);                                                             \
                    entry = tmp;                                                             \
                                                                                             \
                    total++;                                                                 \
                }                                                                            \
                else                                                                         \
                    entry = entry->next;                                                     \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        _map_->count -= total;                                                               \
                                                                                             \
        return total;                                                                        \
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
    FMOD bool PFX##_remove_if(SNAME *_map_, K key, bool condition)                           \
    {                                                                                        \
        if (condition)                                                                       \
            return PFX##_remove(_map_, key);                                                 \
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
        K result_key;                                                                        \
        K max_key;                                                                           \
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
        K result_key;                                                                        \
        K min_key;                                                                           \
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
        memset(iter, 0, sizeof(SNAME##_iter));                                               \
        iter->target = target;                                                               \
        iter->start = true;                                                                  \
        iter->end = PFX##_empty(target);                                                     \
                                                                                             \
        if (!PFX##_empty(target))                                                            \
        {                                                                                    \
            for (size_t i = 0; i < target->capacity; i++)                                    \
            {                                                                                \
                if (target->buffer[i] != NULL)                                               \
                {                                                                            \
                    iter->curr_entry = target->buffer[i];                                    \
                    iter->cursor = i;                                                        \
                    iter->first = i;                                                         \
                                                                                             \
                    break;                                                                   \
                }                                                                            \
            }                                                                                \
                                                                                             \
            for (size_t i = target->capacity; i > 0; i--)                                    \
            {                                                                                \
                if (target->buffer[i - 1] != NULL)                                           \
                {                                                                            \
                    iter->last = i - 1;                                                      \
                                                                                             \
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
                                                                                             \
        iter->curr_entry = iter->target->buffer[iter->cursor];                               \
    }                                                                                        \
                                                                                             \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter)                                           \
    {                                                                                        \
        iter->cursor = iter->last;                                                           \
        iter->index = iter->target->count - 1;                                               \
        iter->start = PFX##_empty(iter->target);                                             \
        iter->end = true;                                                                    \
        iter->curr_entry = iter->target->buffer[iter->cursor];                               \
                                                                                             \
        if (iter->curr_entry != NULL)                                                        \
        {                                                                                    \
            while (iter->curr_entry->next != NULL)                                           \
                iter->curr_entry = iter->curr_entry->next;                                   \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, K *key, V *value, size_t *index)           \
    {                                                                                        \
        if (iter->end)                                                                       \
            return false;                                                                    \
                                                                                             \
        *key = iter->curr_entry->key;                                                        \
        *value = iter->curr_entry->value;                                                    \
        *index = iter->index;                                                                \
                                                                                             \
        if (iter->curr_entry->next != NULL)                                                  \
        {                                                                                    \
            iter->curr_entry = iter->curr_entry->next;                                       \
            iter->index++;                                                                   \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            if (iter->cursor == iter->last)                                                  \
                iter->end = true;                                                            \
            else                                                                             \
            {                                                                                \
                iter->cursor++;                                                              \
                                                                                             \
                while (iter->target->buffer[iter->cursor] == NULL)                           \
                    iter->cursor++;                                                          \
                                                                                             \
                iter->curr_entry = iter->target->buffer[iter->cursor];                       \
                                                                                             \
                iter->index++;                                                               \
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
        *key = iter->curr_entry->key;                                                        \
        *value = iter->curr_entry->value;                                                    \
        *index = iter->index;                                                                \
                                                                                             \
        if (iter->curr_entry->prev != NULL)                                                  \
        {                                                                                    \
            iter->curr_entry = iter->curr_entry->prev;                                       \
            iter->index--;                                                                   \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            if (iter->cursor == iter->first)                                                 \
                iter->start = true;                                                          \
            else                                                                             \
            {                                                                                \
                iter->cursor--;                                                              \
                                                                                             \
                while (iter->target->buffer[iter->cursor] == NULL)                           \
                    iter->cursor--;                                                          \
                                                                                             \
                iter->curr_entry = iter->target->buffer[iter->cursor];                       \
                                                                                             \
                while (iter->curr_entry->next != NULL)                                       \
                    iter->curr_entry = iter->curr_entry->next;                               \
                                                                                             \
                iter->index--;                                                               \
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
        size_t new_size = PFX##_calculate_size((size_t)((double)_map_->capacity * 1.5));     \
                                                                                             \
        SNAME *_new_map_ = PFX##_new(new_size, _map_->load, _map_->cmp, _map_->hash);        \
                                                                                             \
        if (!_new_map_)                                                                      \
            return false;                                                                    \
                                                                                             \
        SNAME##_iter iter;                                                                   \
        K key;                                                                               \
        V value;                                                                             \
        size_t index;                                                                        \
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
        SNAME##_entry **tmp_b = _map_->buffer;                                               \
        _map_->buffer = _new_map_->buffer;                                                   \
        _new_map_->buffer = tmp_b;                                                           \
                                                                                             \
        size_t tmp_c = _map_->capacity;                                                      \
        _map_->capacity = _new_map_->capacity;                                               \
        _new_map_->capacity = tmp_c;                                                         \
                                                                                             \
        PFX##_free(_new_map_);                                                               \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD SNAME##_entry *PFX##_new_entry(K key, V value)                                      \
    {                                                                                        \
        SNAME##_entry *entry = malloc(sizeof(SNAME##_entry));                                \
                                                                                             \
        if (!entry)                                                                          \
            return NULL;                                                                     \
                                                                                             \
        entry->key = key;                                                                    \
        entry->value = value;                                                                \
        entry->next = NULL;                                                                  \
        entry->prev = NULL;                                                                  \
                                                                                             \
        return entry;                                                                        \
    }                                                                                        \
                                                                                             \
    FMOD SNAME##_entry *PFX##_get_entry(SNAME *_map_, K key)                                 \
    {                                                                                        \
        size_t hash = _map_->hash(key);                                                      \
                                                                                             \
        SNAME##_entry *entry = _map_->buffer[hash % _map_->capacity];                        \
                                                                                             \
        if (entry == NULL)                                                                   \
            return NULL;                                                                     \
                                                                                             \
        if (entry->next == NULL && entry->prev == NULL)                                      \
        {                                                                                    \
            if (_map_->cmp(entry->key, key) == 0)                                            \
                return entry;                                                                \
            else                                                                             \
                return NULL;                                                                 \
        }                                                                                    \
                                                                                             \
        while (entry != NULL)                                                                \
        {                                                                                    \
            if (_map_->cmp(entry->key, key) == 0)                                            \
                return entry;                                                                \
                                                                                             \
            entry = entry->next;                                                             \
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

#endif /* CMC_MULTIMAP_H */
