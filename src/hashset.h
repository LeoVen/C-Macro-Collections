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

#define HASHSET_GENERATE(PFX, SNAME, FMOD, V)    \
    HASHSET_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
    HASHSET_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    HASHSET_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* PRIVATE *******************************************************************/
#define HASHSET_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    HASHSET_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define HASHSET_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    HASHSET_GENERATE_STRUCT(PFX, SNAME, FMOD, V)                \
    HASHSET_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* PUBLIC ********************************************************************/
#define HASHSET_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    HASHSET_GENERATE_STRUCT(PFX, SNAME, FMOD, V)               \
    HASHSET_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define HASHSET_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    HASHSET_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* STRUCT ********************************************************************/
#define HASHSET_GENERATE_STRUCT(PFX, SNAME, FMOD, V)

/* HEADER ********************************************************************/
#define HASHSET_GENERATE_HEADER(PFX, SNAME, FMOD, V)                                          \
                                                                                              \
    /* Hashset Structure */                                                                   \
    typedef struct SNAME##_s                                                                  \
    {                                                                                         \
        /* Array of Entries */                                                                \
        struct SNAME##_entry_s *buffer;                                                       \
                                                                                              \
        /* Current Array Capcity */                                                           \
        size_t capacity;                                                                      \
                                                                                              \
        /* Current amount of elements */                                                      \
        size_t count;                                                                         \
                                                                                              \
        /* Load factor in range (0.0, 1.0) */                                                 \
        double load;                                                                          \
                                                                                              \
        /* Element comparison function */                                                     \
        int (*cmp)(V, V);                                                                     \
                                                                                              \
        /* Element hash function */                                                           \
        size_t (*hash)(V);                                                                    \
                                                                                              \
        /* Function that returns an iterator to the start of the hashset */                   \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                                \
                                                                                              \
        /* Function that returns an iterator to the end of the hashset */                     \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                                  \
                                                                                              \
    } SNAME, *SNAME##_ptr;                                                                    \
                                                                                              \
    typedef struct SNAME##_entry_s                                                            \
    {                                                                                         \
        /* Entry element */                                                                   \
        V value;                                                                              \
                                                                                              \
        /* The distance of this node to its original position, used by robin-hood hashing */  \
        size_t dist;                                                                          \
                                                                                              \
        /* The sate of this node (DELETED, EMPTY, FILLED) */                                  \
        enum EntryState_e state;                                                              \
                                                                                              \
    } SNAME##_entry, *SNAME##_entry_ptr;                                                      \
                                                                                              \
    /* Hashset Iterator */                                                                    \
    typedef struct SNAME##_iter_s                                                             \
    {                                                                                         \
        /* Target Hashset */                                                                  \
        struct SNAME##_s *target;                                                             \
                                                                                              \
        /* Cursor's position (index) */                                                       \
        size_t cursor;                                                                        \
                                                                                              \
        /* Keeps track of relative index to the iteration of elements */                      \
        size_t index;                                                                         \
                                                                                              \
        /* The index of the first element */                                                  \
        size_t first;                                                                         \
                                                                                              \
        /* The index of the last element */                                                   \
        size_t last;                                                                          \
                                                                                              \
        /* If the iterator has reached the start of the iteration */                          \
        bool start;                                                                           \
                                                                                              \
        /* If the iterator has reached the end of the iteration */                            \
        bool end;                                                                             \
                                                                                              \
    } SNAME##_iter, *SNAME##_iter_ptr;                                                        \
                                                                                              \
    /* Collection Functions */                                                                \
    /* Collection Allocation and Deallocation */                                              \
    FMOD SNAME *PFX##_new(size_t size, double load, int (*compare)(V, V), size_t (*hash)(V)); \
    FMOD void PFX##_clear(SNAME *_set_);                                                      \
    FMOD void PFX##_free(SNAME *_set_);                                                       \
    /* Collection Input and Output */                                                         \
    FMOD bool PFX##_insert(SNAME *_set_, V element);                                          \
    FMOD bool PFX##_remove(SNAME *_set_, V element);                                          \
    /* Conditional Input and Output */                                                        \
    FMOD bool PFX##_insert_if(SNAME *_set_, V element, bool condition);                       \
    FMOD bool PFX##_remove_if(SNAME *_set_, V element, bool condition);                       \
    /* Element Access */                                                                      \
    FMOD bool PFX##_max(SNAME *_set_, V *value);                                              \
    FMOD bool PFX##_min(SNAME *_set_, V *value);                                              \
    /* Collection State */                                                                    \
    FMOD bool PFX##_contains(SNAME *_set_, V element);                                        \
    FMOD bool PFX##_empty(SNAME *_set_);                                                      \
    FMOD size_t PFX##_count(SNAME *_set_);                                                    \
    FMOD size_t PFX##_capacity(SNAME *_set_);                                                 \
                                                                                              \
    /* Set Operations */                                                                      \
    FMOD SNAME *PFX##_union(SNAME *_set1_, SNAME *_set2_);                                    \
    FMOD SNAME *PFX##_intersection(SNAME *_set1_, SNAME *_set2_);                             \
    FMOD SNAME *PFX##_difference(SNAME *_set1_, SNAME *_set2_);                               \
    FMOD SNAME *PFX##_symmetric_difference(SNAME *_set1_, SNAME *_set2_);                     \
                                                                                              \
    /* Iterator Functions */                                                                  \
    /* Iterator Allocation and Deallocation */                                                \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);                                         \
    FMOD void PFX##_iter_free(SNAME##_iter *iter);                                            \
    /* Iterator Initialization */                                                             \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                             \
    /* Iterator State */                                                                      \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                           \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                             \
    /* Iterator Movement */                                                                   \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter);                                        \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter);                                          \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter);                                            \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter);                                            \
    /* Iterator Access */                                                                     \
    FMOD V PFX##_iter_value(SNAME##_iter *iter);                                              \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter);                                         \
                                                                                              \
/* SOURCE ********************************************************************/
#define HASHSET_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                                           \
                                                                                               \
    /* Implementation Detail Functions */                                                      \
    FMOD bool PFX##_grow(SNAME *_set_);                                                        \
    FMOD SNAME##_entry *PFX##_get_entry(SNAME *_set_, V element);                              \
    FMOD size_t PFX##_calculate_size(size_t required);                                         \
    SNAME##_iter PFX##_impl_it_start(SNAME *_map_);                                            \
    SNAME##_iter PFX##_impl_it_end(SNAME *_map_);                                              \
                                                                                               \
    FMOD SNAME *PFX##_new(size_t size, double load, int (*compare)(V, V), size_t (*hash)(V))   \
    {                                                                                          \
        if (size == 0 || load <= 0 || load >= 1)                                               \
            return NULL;                                                                       \
                                                                                               \
        size_t real_size = PFX##_calculate_size(size);                                         \
                                                                                               \
        SNAME *_set_ = malloc(sizeof(SNAME));                                                  \
                                                                                               \
        if (!_set_)                                                                            \
            return NULL;                                                                       \
                                                                                               \
        _set_->buffer = malloc(sizeof(SNAME##_entry) * real_size);                             \
                                                                                               \
        if (!_set_->buffer)                                                                    \
        {                                                                                      \
            free(_set_);                                                                       \
            return NULL;                                                                       \
        }                                                                                      \
                                                                                               \
        memset(_set_->buffer, 0, sizeof(SNAME##_entry) * real_size);                           \
                                                                                               \
        _set_->count = 0;                                                                      \
        _set_->capacity = real_size;                                                           \
        _set_->load = load;                                                                    \
        _set_->cmp = compare;                                                                  \
        _set_->hash = hash;                                                                    \
                                                                                               \
        _set_->it_start = PFX##_impl_it_start;                                                 \
        _set_->it_end = PFX##_impl_it_end;                                                     \
                                                                                               \
        return _set_;                                                                          \
    }                                                                                          \
                                                                                               \
    FMOD void PFX##_clear(SNAME *_set_)                                                        \
    {                                                                                          \
        memset(_set_->buffer, 0, sizeof(SNAME##_entry) * _set_->capacity);                     \
                                                                                               \
        _set_->count = 0;                                                                      \
    }                                                                                          \
                                                                                               \
    FMOD void PFX##_free(SNAME *_set_)                                                         \
    {                                                                                          \
        free(_set_->buffer);                                                                   \
        free(_set_);                                                                           \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_insert(SNAME *_set_, V element)                                            \
    {                                                                                          \
        if ((double)_set_->capacity * _set_->load <= (double)_set_->count)                     \
        {                                                                                      \
            if (!PFX##_grow(_set_))                                                            \
                return false;                                                                  \
        }                                                                                      \
                                                                                               \
        size_t hash = _set_->hash(element);                                                    \
        size_t original_pos = hash % _set_->capacity;                                          \
        size_t pos = original_pos;                                                             \
                                                                                               \
        SNAME##_entry *target = &(_set_->buffer[pos]);                                         \
                                                                                               \
        if (PFX##_get_entry(_set_, element) != NULL)                                           \
            return false;                                                                      \
                                                                                               \
        if (target->state == ES_EMPTY || target->state == ES_DELETED)                          \
        {                                                                                      \
            target->value = element;                                                           \
            target->dist = pos - original_pos;                                                 \
            target->state = ES_FILLED;                                                         \
        }                                                                                      \
        else                                                                                   \
        {                                                                                      \
            while (true)                                                                       \
            {                                                                                  \
                pos++;                                                                         \
                target = &(_set_->buffer[pos % _set_->capacity]);                              \
                                                                                               \
                if (target->state == ES_EMPTY || target->state == ES_DELETED)                  \
                {                                                                              \
                    target->value = element;                                                   \
                    target->dist = pos - original_pos;                                         \
                    target->state = ES_FILLED;                                                 \
                                                                                               \
                    break;                                                                     \
                }                                                                              \
                else if (target->dist < pos - original_pos)                                    \
                {                                                                              \
                    V tmp = target->value;                                                     \
                    size_t tmp_dist = target->dist;                                            \
                                                                                               \
                    target->value = element;                                                   \
                    target->dist = pos - original_pos;                                         \
                                                                                               \
                    element = tmp;                                                             \
                    original_pos = pos - tmp_dist;                                             \
                }                                                                              \
            }                                                                                  \
        }                                                                                      \
                                                                                               \
        _set_->count++;                                                                        \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_remove(SNAME *_set_, V element)                                            \
    {                                                                                          \
        SNAME##_entry *result = PFX##_get_entry(_set_, element);                               \
                                                                                               \
        if (result == NULL)                                                                    \
            return false;                                                                      \
                                                                                               \
        result->value = 0;                                                                     \
        result->dist = 0;                                                                      \
        result->state = ES_DELETED;                                                            \
                                                                                               \
        _set_->count--;                                                                        \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_insert_if(SNAME *_set_, V element, bool condition)                         \
    {                                                                                          \
        if (condition)                                                                         \
            return PFX##_insert(_set_, element);                                               \
                                                                                               \
        return false;                                                                          \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_remove_if(SNAME *_set_, V element, bool condition)                         \
    {                                                                                          \
        if (condition)                                                                         \
            return PFX##_remove(_set_, element);                                               \
                                                                                               \
        return false;                                                                          \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_max(SNAME *_set_, V *value)                                                \
    {                                                                                          \
        if (PFX##_empty(_set_))                                                                \
            return false;                                                                      \
                                                                                               \
        SNAME##_iter iter;                                                                     \
                                                                                               \
        for (PFX##_iter_init(&iter, _set_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))    \
        {                                                                                      \
            V result = PFX##_iter_value(&iter);                                                \
            size_t index = PFX##_iter_index(&iter);                                            \
                                                                                               \
            if (index == 0)                                                                    \
                *value = result;                                                               \
            else if (_set_->cmp(result, *value) > 0)                                           \
                *value = result;                                                               \
        }                                                                                      \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_min(SNAME *_set_, V *value)                                                \
    {                                                                                          \
        if (PFX##_empty(_set_))                                                                \
            return false;                                                                      \
                                                                                               \
        SNAME##_iter iter;                                                                     \
                                                                                               \
        for (PFX##_iter_init(&iter, _set_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))    \
        {                                                                                      \
            V result = PFX##_iter_value(&iter);                                                \
            size_t index = PFX##_iter_index(&iter);                                            \
                                                                                               \
            if (index == 0)                                                                    \
                *value = result;                                                               \
            else if (_set_->cmp(result, *value) < 0)                                           \
                *value = result;                                                               \
        }                                                                                      \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_contains(SNAME *_set_, V element)                                          \
    {                                                                                          \
        return PFX##_get_entry(_set_, element) != NULL;                                        \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_empty(SNAME *_set_)                                                        \
    {                                                                                          \
        return _set_->count == 0;                                                              \
    }                                                                                          \
                                                                                               \
    FMOD size_t PFX##_count(SNAME *_set_)                                                      \
    {                                                                                          \
        return _set_->count;                                                                   \
    }                                                                                          \
                                                                                               \
    FMOD size_t PFX##_capacity(SNAME *_set_)                                                   \
    {                                                                                          \
        return _set_->capacity;                                                                \
    }                                                                                          \
                                                                                               \
    FMOD SNAME *PFX##_union(SNAME *_set1_, SNAME *_set2_)                                      \
    {                                                                                          \
        SNAME *_set_r_ = PFX##_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash); \
                                                                                               \
        if (!_set_r_)                                                                          \
            return false;                                                                      \
                                                                                               \
        SNAME##_iter iter1, iter2;                                                             \
        PFX##_iter_init(&iter1, _set1_);                                                       \
        PFX##_iter_init(&iter2, _set2_);                                                       \
                                                                                               \
        for (PFX##_iter_to_start(&iter1); !PFX##_iter_end(&iter1); PFX##_iter_next(&iter1))    \
        {                                                                                      \
            PFX##_insert(_set_r_, PFX##_iter_value(&iter1));                                   \
        }                                                                                      \
                                                                                               \
        for (PFX##_iter_to_start(&iter2); !PFX##_iter_end(&iter2); PFX##_iter_next(&iter2))    \
        {                                                                                      \
            PFX##_insert(_set_r_, PFX##_iter_value(&iter2));                                   \
        }                                                                                      \
                                                                                               \
        return _set_r_;                                                                        \
    }                                                                                          \
                                                                                               \
    FMOD SNAME *PFX##_intersection(SNAME *_set1_, SNAME *_set2_)                               \
    {                                                                                          \
        SNAME *_set_r_ = PFX##_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash); \
                                                                                               \
        if (!_set_r_)                                                                          \
            return false;                                                                      \
                                                                                               \
        SNAME *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;                      \
        SNAME *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;                                  \
                                                                                               \
        SNAME##_iter iter;                                                                     \
        PFX##_iter_init(&iter, _set_A_);                                                       \
                                                                                               \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))       \
        {                                                                                      \
            V value = PFX##_iter_value(&iter);                                                 \
                                                                                               \
            if (PFX##_get_entry(_set_B_, value) != NULL)                                       \
                PFX##_insert(_set_r_, value);                                                  \
        }                                                                                      \
                                                                                               \
        return _set_r_;                                                                        \
    }                                                                                          \
                                                                                               \
    FMOD SNAME *PFX##_difference(SNAME *_set1_, SNAME *_set2_)                                 \
    {                                                                                          \
        SNAME *_set_r_ = PFX##_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash); \
                                                                                               \
        if (!_set_r_)                                                                          \
            return false;                                                                      \
                                                                                               \
        SNAME##_iter iter;                                                                     \
        PFX##_iter_init(&iter, _set1_);                                                        \
                                                                                               \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))       \
        {                                                                                      \
            V value = PFX##_iter_value(&iter);                                                 \
                                                                                               \
            if (PFX##_get_entry(_set2_, value) == NULL)                                        \
                PFX##_insert(_set_r_, value);                                                  \
        }                                                                                      \
                                                                                               \
        return _set_r_;                                                                        \
    }                                                                                          \
                                                                                               \
    FMOD SNAME *PFX##_symmetric_difference(SNAME *_set1_, SNAME *_set2_)                       \
    {                                                                                          \
        SNAME##_iter iter1, iter2;                                                             \
                                                                                               \
        SNAME *_set_r_ = PFX##_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash); \
                                                                                               \
        if (!_set_r_)                                                                          \
            return false;                                                                      \
                                                                                               \
        PFX##_iter_init(&iter1, _set1_);                                                       \
        PFX##_iter_init(&iter2, _set2_);                                                       \
                                                                                               \
        for (PFX##_iter_to_start(&iter1); !PFX##_iter_end(&iter1); PFX##_iter_next(&iter1))    \
        {                                                                                      \
            V value = PFX##_iter_value(&iter1);                                                \
                                                                                               \
            if (PFX##_get_entry(_set2_, value) == NULL)                                        \
                PFX##_insert(_set_r_, value);                                                  \
        }                                                                                      \
                                                                                               \
        for (PFX##_iter_to_start(&iter2); !PFX##_iter_end(&iter2); PFX##_iter_next(&iter2))    \
        {                                                                                      \
            V value = PFX##_iter_value(&iter2);                                                \
                                                                                               \
            if (PFX##_get_entry(_set1_, value) == NULL)                                        \
                PFX##_insert(_set_r_, value);                                                  \
        }                                                                                      \
                                                                                               \
        return _set_r_;                                                                        \
    }                                                                                          \
                                                                                               \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target)                                           \
    {                                                                                          \
        SNAME##_iter *iter = malloc(sizeof(SNAME##_iter));                                     \
                                                                                               \
        if (!iter)                                                                             \
            return NULL;                                                                       \
                                                                                               \
        PFX##_iter_init(iter, target);                                                         \
                                                                                               \
        return iter;                                                                           \
    }                                                                                          \
                                                                                               \
    FMOD void PFX##_iter_free(SNAME##_iter *iter)                                              \
    {                                                                                          \
        free(iter);                                                                            \
    }                                                                                          \
                                                                                               \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                               \
    {                                                                                          \
        iter->target = target;                                                                 \
        iter->cursor = 0;                                                                      \
        iter->index = 0;                                                                       \
        iter->start = true;                                                                    \
        iter->end = PFX##_empty(target);                                                       \
                                                                                               \
        if (!PFX##_empty(target))                                                              \
        {                                                                                      \
            for (size_t i = 0; i < target->capacity; i++)                                      \
            {                                                                                  \
                if (target->buffer[i].state == ES_FILLED)                                      \
                {                                                                              \
                    iter->first = i;                                                           \
                    break;                                                                     \
                }                                                                              \
            }                                                                                  \
                                                                                               \
            iter->cursor = iter->first;                                                        \
                                                                                               \
            for (size_t i = target->capacity; i > 0; i--)                                      \
            {                                                                                  \
                if (target->buffer[i - 1].state == ES_FILLED)                                  \
                {                                                                              \
                    iter->last = i - 1;                                                        \
                    break;                                                                     \
                }                                                                              \
            }                                                                                  \
        }                                                                                      \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                             \
    {                                                                                          \
        return PFX##_empty(iter->target) || iter->start;                                       \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                               \
    {                                                                                          \
        return PFX##_empty(iter->target) || iter->end;                                         \
    }                                                                                          \
                                                                                               \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter)                                          \
    {                                                                                          \
        iter->cursor = iter->first;                                                            \
        iter->index = 0;                                                                       \
        iter->start = true;                                                                    \
        iter->end = PFX##_empty(iter->target);                                                 \
    }                                                                                          \
                                                                                               \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter)                                            \
    {                                                                                          \
        iter->cursor = iter->last;                                                             \
        iter->index = iter->target->count - 1;                                                 \
        iter->start = PFX##_empty(iter->target);                                               \
        iter->end = true;                                                                      \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter)                                              \
    {                                                                                          \
        if (iter->end)                                                                         \
            return false;                                                                      \
                                                                                               \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                           \
                                                                                               \
        if (iter->cursor == iter->last)                                                        \
            iter->end = true;                                                                  \
        else                                                                                   \
        {                                                                                      \
            iter->index++;                                                                     \
                                                                                               \
            while (1)                                                                          \
            {                                                                                  \
                iter->cursor++;                                                                \
                scan = &(iter->target->buffer[iter->cursor]);                                  \
                                                                                               \
                if (scan->state == ES_FILLED)                                                  \
                    break;                                                                     \
            }                                                                                  \
        }                                                                                      \
                                                                                               \
        iter->start = PFX##_empty(iter->target);                                               \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter)                                              \
    {                                                                                          \
        if (iter->start)                                                                       \
            return false;                                                                      \
                                                                                               \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                           \
                                                                                               \
        if (iter->cursor == iter->first)                                                       \
            iter->start = true;                                                                \
        else                                                                                   \
        {                                                                                      \
            iter->index--;                                                                     \
                                                                                               \
            while (1)                                                                          \
            {                                                                                  \
                iter->cursor--;                                                                \
                scan = &(iter->target->buffer[iter->cursor]);                                  \
                                                                                               \
                if (scan->state == ES_FILLED)                                                  \
                    break;                                                                     \
            }                                                                                  \
        }                                                                                      \
                                                                                               \
        iter->end = PFX##_empty(iter->target);                                                 \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    FMOD V PFX##_iter_value(SNAME##_iter *iter)                                                \
    {                                                                                          \
        return iter->target->buffer[iter->cursor].value;                                       \
    }                                                                                          \
                                                                                               \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter)                                           \
    {                                                                                          \
        return iter->index;                                                                    \
    }                                                                                          \
                                                                                               \
    FMOD bool PFX##_grow(SNAME *_set_)                                                         \
    {                                                                                          \
        size_t new_size = PFX##_calculate_size(_set_->capacity + _set_->capacity / 2);         \
                                                                                               \
        SNAME *_new_set_ = PFX##_new(new_size, _set_->load, _set_->cmp, _set_->hash);          \
                                                                                               \
        if (!_new_set_)                                                                        \
            return false;                                                                      \
                                                                                               \
        SNAME##_iter iter;                                                                     \
                                                                                               \
        for (PFX##_iter_init(&iter, _set_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))    \
        {                                                                                      \
            PFX##_insert(_new_set_, PFX##_iter_value(&iter));                                  \
        }                                                                                      \
                                                                                               \
        if (_set_->count != _new_set_->count)                                                  \
        {                                                                                      \
            PFX##_free(_new_set_);                                                             \
                                                                                               \
            return false;                                                                      \
        }                                                                                      \
                                                                                               \
        SNAME##_entry *tmp = _set_->buffer;                                                    \
        _set_->buffer = _new_set_->buffer;                                                     \
        _new_set_->buffer = tmp;                                                               \
                                                                                               \
        _set_->capacity = _new_set_->capacity;                                                 \
                                                                                               \
        PFX##_free(_new_set_);                                                                 \
                                                                                               \
        return true;                                                                           \
    }                                                                                          \
                                                                                               \
    FMOD SNAME##_entry *PFX##_get_entry(SNAME *_set_, V element)                               \
    {                                                                                          \
        size_t hash = _set_->hash(element);                                                    \
        size_t pos = hash % _set_->capacity;                                                   \
                                                                                               \
        SNAME##_entry *target = &(_set_->buffer[pos]);                                         \
                                                                                               \
        while (target->state == ES_FILLED || target->state == ES_DELETED)                      \
        {                                                                                      \
            if (_set_->cmp(target->value, element) == 0)                                       \
                return target;                                                                 \
                                                                                               \
            pos++;                                                                             \
            target = &(_set_->buffer[pos % _set_->capacity]);                                  \
        }                                                                                      \
                                                                                               \
        return NULL;                                                                           \
    }                                                                                          \
                                                                                               \
    FMOD size_t PFX##_calculate_size(size_t required)                                          \
    {                                                                                          \
        const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);   \
                                                                                               \
        if (cmc_hashtable_primes[count - 1] < required)                                        \
            return required;                                                                   \
                                                                                               \
        size_t i = 0;                                                                          \
        while (cmc_hashtable_primes[i] < required)                                             \
            i++;                                                                               \
                                                                                               \
        return cmc_hashtable_primes[i];                                                        \
    }                                                                                          \
                                                                                               \
    SNAME##_iter PFX##_impl_it_start(SNAME *_map_)                                             \
    {                                                                                          \
        SNAME##_iter iter;                                                                     \
                                                                                               \
        PFX##_iter_init(&iter, _map_);                                                         \
                                                                                               \
        return iter;                                                                           \
    }                                                                                          \
                                                                                               \
    SNAME##_iter PFX##_impl_it_end(SNAME *_map_)                                               \
    {                                                                                          \
        SNAME##_iter iter;                                                                     \
                                                                                               \
        PFX##_iter_init(&iter, _map_);                                                         \
        PFX##_iter_to_end(&iter);                                                              \
                                                                                               \
        return iter;                                                                           \
    }

#endif /* CMC_HASHSET_H */
