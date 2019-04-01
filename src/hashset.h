/**
 * hashset.h
 *
 * Creation Date: 01/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/******************************************************************* HASHSET */
/*****************************************************************************/

#ifndef CMC_HASHSET_H
#define CMC_HASHSET_H

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

static const size_t cmc_hashtable_primes[] = {53, 97, 193, 389, 769, 1543, 3079,
                                              6151, 12289, 24593, 49157, 98317,
                                              196613, 393241, 786433, 1572869,
                                              3145739, 6291469, 12582917,
                                              25165843, 50331653, 100663319,
                                              201326611, 402653189, 805306457,
                                              1610612741};

#endif /* CMC_HASH_TABLE_SETUP */

#define HASHSET_GENERATE(PFX, SNAME, FMOD, K, V)    \
    HASHSET_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
    HASHSET_GENERATE_HEADER(PFX, SNAME, FMOD, K, V) \
    HASHSET_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)

/* PRIVATE *******************************************************************/
#define HASHSET_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    HASHSET_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define HASHSET_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    HASHSET_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)             \
    HASHSET_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* PUBLIC ********************************************************************/
#define HASHSET_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    HASHSET_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)            \
    HASHSET_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define HASHSET_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    HASHSET_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* STRUCT ********************************************************************/
#define HASHSET_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
                                                        \
    struct SNAME##_s                                    \
    {                                                   \
        struct SNAME##_entry_s *buffer;                 \
        size_t capacity;                                \
        size_t count;                                   \
        double load;                                    \
        int (*cmp)(V, V);                               \
        size_t (*hash)(V);                              \
    };                                                  \
                                                        \
    struct SNAME##_entry_s                              \
    {                                                   \
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
#define HASHSET_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)                                       \
                                                                                              \
    typedef struct SNAME##_s SNAME;                                                           \
    typedef struct SNAME##_entry_s SNAME##_entry;                                             \
    typedef struct SNAME##_iter_s SNAME##_iter;                                               \
                                                                                              \
    FMOD SNAME *PFX##_new(size_t size, double load, int (*compare)(V, V), size_t (*hash)(V)); \
    FMOD void PFX##_free(SNAME *_set_);                                                       \
    FMOD bool PFX##_insert(SNAME *_set_, V element);                                          \
    FMOD bool PFX##_remove(SNAME *_set_, V element);                                          \
    FMOD bool PFX##_insert_if(SNAME *_set_, V element, bool condition);                       \
    FMOD bool PFX##_remove_if(SNAME *_set_, V element, bool condition);                       \
    FMOD V PFX##_max(SNAME *_set_);                                                           \
    FMOD V PFX##_min(SNAME *_set_);                                                           \
    FMOD bool PFX##_empty(SNAME *_set_);                                                      \
    FMOD size_t PFX##_count(SNAME *_set_);                                                    \
                                                                                              \
    FMOD SNAME *PFX##_union(SNAME *_set1_, SNAME *_set2_);                                    \
    FMOD SNAME *PFX##_intersection(SNAME *_set1_, SNAME *_set2_);                             \
    FMOD SNAME *PFX##_difference(SNAME *_set1_, SNAME *_set2_);                               \
    FMOD SNAME *PFX##_symmetric_difference(SNAME *_set1_, SNAME *_set2_);                     \
                                                                                              \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);                              \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                           \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                             \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                                         \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                                           \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index);                  \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index);                  \
                                                                                              \
/* SOURCE ********************************************************************/
#define HASHSET_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)                                      \
                                                                                             \
    FMOD bool PFX##_grow(SNAME *_set_);                                                      \
    FMOD SNAME##_entry *PFX##_get_entry(SNAME *_set_, V element);                            \
    FMOD size_t PFX##_calculate_size(size_t required);                                       \
                                                                                             \
    FMOD SNAME *PFX##_new(size_t size, double load, int (*compare)(V, V), size_t (*hash)(V)) \
    {                                                                                        \
        if (size == 0 || load <= 0 || load >= 1)                                             \
            return NULL;                                                                     \
                                                                                             \
        size_t real_size = PFX##_calculate_size(size);                                       \
                                                                                             \
        SNAME *_set_ = malloc(sizeof(SNAME));                                                \
                                                                                             \
        if (!_set_)                                                                          \
            return NULL;                                                                     \
                                                                                             \
        _set_->buffer = malloc(sizeof(SNAME##_entry) * real_size);                           \
                                                                                             \
        if (!_set_->buffer)                                                                  \
        {                                                                                    \
            free(_set_);                                                                     \
            return NULL;                                                                     \
        }                                                                                    \
                                                                                             \
        memset(_set_->buffer, 0, sizeof(SNAME##_entry) * real_size);                         \
                                                                                             \
        _set_->count = 0;                                                                    \
        _set_->capacity = real_size;                                                         \
        _set_->load = load;                                                                  \
        _set_->cmp = compare;                                                                \
        _set_->hash = hash;                                                                  \
                                                                                             \
        return _set_;                                                                        \
    }                                                                                        \
                                                                                             \
    FMOD void PFX##_free(SNAME *_set_)                                                       \
    {                                                                                        \
        free(_set_->buffer);                                                                 \
        free(_set_);                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_insert(SNAME *_set_, V element)                                          \
    {                                                                                        \
        if ((double)_set_->capacity * _set_->load <= (double)_set_->count)                   \
        {                                                                                    \
            if (!PFX##_grow(_set_))                                                          \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        size_t hash = _set_->hash(element);                                                  \
        size_t original_pos = hash % _set_->capacity;                                        \
        size_t pos = original_pos;                                                           \
                                                                                             \
        SNAME##_entry *target = &(_set_->buffer[pos]);                                       \
                                                                                             \
        if (PFX##_get_entry(_set_, element) != NULL)                                         \
            return false;                                                                    \
                                                                                             \
        if (target->state == ES_EMPTY || target->state == ES_DELETED)                        \
        {                                                                                    \
            target->value = element;                                                         \
            target->dist = original_pos - pos;                                               \
            target->state = ES_FILLED;                                                       \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            while (true)                                                                     \
            {                                                                                \
                pos++;                                                                       \
                target = &(_set_->buffer[pos % _set_->capacity]);                            \
                                                                                             \
                if (target->state == ES_EMPTY || target->state == ES_DELETED)                \
                {                                                                            \
                    target->value = element;                                                 \
                    target->dist = pos - original_pos;                                       \
                    target->state = ES_FILLED;                                               \
                    break;                                                                   \
                }                                                                            \
                else if (target->dist < original_pos - pos)                                  \
                {                                                                            \
                    V tmp = target->value;                                                   \
                    size_t tmp_dist = target->dist;                                          \
                                                                                             \
                    target->value = element;                                                 \
                    target->dist = pos - original_pos;                                       \
                                                                                             \
                    element = tmp;                                                           \
                    original_pos = pos - tmp_dist;                                           \
                }                                                                            \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        _set_->count++;                                                                      \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_remove(SNAME *_set_, V element)                                          \
    {                                                                                        \
        SNAME##_entry *result = PFX##_get_entry(_set_, element);                             \
                                                                                             \
        if (result == NULL)                                                                  \
            return false;                                                                    \
                                                                                             \
        result->value = 0;                                                                   \
        result->dist = 0;                                                                    \
        result->state = ES_DELETED;                                                          \
                                                                                             \
        _set_->count--;                                                                      \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_insert_if(SNAME *_set_, V element, bool condition)                       \
    {                                                                                        \
        if (condition)                                                                       \
            return PFX##_insert(_set_, element);                                             \
                                                                                             \
        return false;                                                                        \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_remove_if(SNAME *_set_, V element, bool condition)                       \
    {                                                                                        \
        if (condition)                                                                       \
            return PFX##_remove(_set_, element);                                             \
                                                                                             \
        return false;                                                                        \
    }                                                                                        \
                                                                                             \
    FMOD V PFX##_max(SNAME *_set_)                                                           \
    {                                                                                        \
        if (PFX##_empty(_set_))                                                              \
            return 0;                                                                        \
                                                                                             \
        SNAME##_iter iter;                                                                   \
        V result, max;                                                                       \
        size_t index;                                                                        \
                                                                                             \
        for (PFX##_iter_new(&iter, _set_); !PFX##_iter_end(&iter);)                          \
        {                                                                                    \
            PFX##_iter_next(&iter, &result, &index);                                         \
                                                                                             \
            if (index == 0)                                                                  \
                max = result;                                                                \
            else if (_set_->cmp(result, max) > 0)                                            \
                max = result;                                                                \
        }                                                                                    \
                                                                                             \
        return max;                                                                          \
    }                                                                                        \
                                                                                             \
    FMOD V PFX##_min(SNAME *_set_)                                                           \
    {                                                                                        \
        if (PFX##_empty(_set_))                                                              \
            return 0;                                                                        \
                                                                                             \
        SNAME##_iter iter;                                                                   \
        V result, min;                                                                       \
        size_t index;                                                                        \
                                                                                             \
        for (PFX##_iter_new(&iter, _set_); !PFX##_iter_end(&iter);)                          \
        {                                                                                    \
            PFX##_iter_next(&iter, &result, &index);                                         \
                                                                                             \
            if (index == 0)                                                                  \
                min = result;                                                                \
            else if (_set_->cmp(result, min) < 0)                                            \
                min = result;                                                                \
        }                                                                                    \
                                                                                             \
        return min;                                                                          \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_empty(SNAME *_set_)                                                      \
    {                                                                                        \
        return _set_->count == 0;                                                            \
    }                                                                                        \
                                                                                             \
    FMOD size_t PFX##_count(SNAME *_set_)                                                    \
    {                                                                                        \
        return _set_->count;                                                                 \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_grow(SNAME *_set_)                                                       \
    {                                                                                        \
        size_t new_size = PFX##_calculate_size((size_t)((double)_set_->capacity * 1.5));     \
                                                                                             \
        SNAME *_new_set_ = PFX##_new(new_size, _set_->load, _set_->cmp, _set_->hash);        \
                                                                                             \
        if (!_new_set_)                                                                      \
            return false;                                                                    \
                                                                                             \
        SNAME##_iter iter;                                                                   \
        V value;                                                                             \
        size_t index;                                                                        \
                                                                                             \
        for (PFX##_iter_new(&iter, _set_); !PFX##_iter_end(&iter);)                          \
        {                                                                                    \
            PFX##_iter_next(&iter, &value, &index);                                          \
            PFX##_insert(_new_set_, value);                                                  \
        }                                                                                    \
                                                                                             \
        if (_set_->count != _new_set_->count)                                                \
        {                                                                                    \
            PFX##_free(_new_set_);                                                           \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        SNAME##_entry *tmp = _set_->buffer;                                                  \
        _set_->buffer = _new_set_->buffer;                                                   \
        _new_set_->buffer = tmp;                                                             \
                                                                                             \
        _set_->capacity = _new_set_->capacity;                                               \
                                                                                             \
        PFX##_free(_new_set_);                                                               \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    FMOD SNAME##_entry *PFX##_get_entry(SNAME *_set_, V element)                             \
    {                                                                                        \
        size_t hash = _set_->hash(element);                                                  \
        size_t pos = hash % _set_->capacity;                                                 \
                                                                                             \
        SNAME##_entry *target = &(_set_->buffer[pos % _set_->capacity]);                     \
                                                                                             \
        while (target->state == ES_FILLED || target->state == ES_DELETED)                    \
        {                                                                                    \
            if (_set_->cmp(target->value, element) == 0)                                     \
                return target;                                                               \
                                                                                             \
            pos++;                                                                           \
            target = &(_set_->buffer[pos % _set_->capacity]);                                \
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
            return required * 2;                                                             \
                                                                                             \
        size_t i = 0;                                                                        \
        while (cmc_hashtable_primes[i] < required)                                           \
            i++;                                                                             \
                                                                                             \
        return cmc_hashtable_primes[i];                                                      \
    }                                                                                        \
                                                                                             \
    FMOD SNAME *PFX##_union(SNAME *_set1_, SNAME *_set2_);                                   \
    FMOD SNAME *PFX##_intersection(SNAME *_set1_, SNAME *_set2_);                            \
    FMOD SNAME *PFX##_difference(SNAME *_set1_, SNAME *_set2_);                              \
    FMOD SNAME *PFX##_symmetric_difference(SNAME *_set1_, SNAME *_set2_);                    \
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
        return iter->start && iter->cursor == iter->first;                                   \
    }                                                                                        \
                                                                                             \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                             \
    {                                                                                        \
        return iter->end && iter->cursor == iter->last;                                      \
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
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index)                  \
    {                                                                                        \
        if (iter->end)                                                                       \
            return false;                                                                    \
                                                                                             \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                         \
                                                                                             \
        *result = scan->value;                                                               \
        *index = iter->index;                                                                \
                                                                                             \
        if (iter->cursor == iter->last)                                                      \
            iter->end = true;                                                                \
        else                                                                                 \
        {                                                                                    \
            iter->index++;                                                                   \
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
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index)                  \
    {                                                                                        \
        if (iter->start)                                                                     \
            return false;                                                                    \
                                                                                             \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                         \
                                                                                             \
        *result = scan->value;                                                               \
        *index = iter->index;                                                                \
                                                                                             \
        if (iter->cursor == iter->first)                                                     \
            iter->start = true;                                                              \
        else                                                                                 \
        {                                                                                    \
            iter->index--;                                                                   \
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
    }

#endif /* CMC_HASHSET_H */
