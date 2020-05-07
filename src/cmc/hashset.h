/**
 * hashset.h
 *
 * Creation Date: 01/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashSet
 *
 * A HashSet is an implementation of a Set with unique keys. The keys are not
 * sorted. It is implemented as a flat hashtable with linear probing and robin
 * hood hashing.
 */

#ifndef CMC_HASHSET_H
#define CMC_HASHSET_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * Hashtable Implementation
 * ------------------------------------------------------------------------- */
#include "../cor/hashtable.h"

/* -------------------------------------------------------------------------
 * HashSet specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_string_fmt_hashset = "struct %s<%s> "
                                            "at %p { "
                                            "buffer:%p, "
                                            "capacity:%" PRIuMAX ", "
                                            "count:%" PRIuMAX ", "
                                            "load:%lf, "
                                            "flag:%d, "
                                            "f_val:%p, "
                                            "alloc:%p, "
                                            "callbacks: %p }";

#define CMC_GENERATE_HASHSET(PFX, SNAME, V)    \
    CMC_GENERATE_HASHSET_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_HASHSET_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_HASHSET_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_HASHSET_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_HASHSET_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_HASHSET_SOURCE(PFX, SNAME, V)

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_HASHSET_HEADER(PFX, SNAME, V)                             \
                                                                               \
    /* Hashset Structure */                                                    \
    struct SNAME                                                               \
    {                                                                          \
        /* Array of Entries */                                                 \
        struct SNAME##_entry *buffer;                                          \
                                                                               \
        /* Current Array Capcity */                                            \
        size_t capacity;                                                       \
                                                                               \
        /* Current amount of elements */                                       \
        size_t count;                                                          \
                                                                               \
        /* Load factor in range (0.0, 1.0) */                                  \
        double load;                                                           \
                                                                               \
        /* Flags indicating errors or success */                               \
        int flag;                                                              \
                                                                               \
        /* Value function table */                                             \
        struct SNAME##_fval *f_val;                                            \
                                                                               \
        /* Custom allocation functions */                                      \
        struct cmc_alloc_node *alloc;                                          \
                                                                               \
        /* Custom callback functions */                                        \
        struct cmc_callbacks *callbacks;                                       \
    };                                                                         \
                                                                               \
    struct SNAME##_entry                                                       \
    {                                                                          \
        /* Entry value */                                                      \
        V value;                                                               \
                                                                               \
        /* The distance of this node to its original position, used by */      \
        /* robin-hood hashing */                                               \
        size_t dist;                                                           \
                                                                               \
        /* The sate of this node (DELETED, EMPTY, FILLED) */                   \
        enum cmc_entry_state state;                                            \
    };                                                                         \
                                                                               \
    /* Value struct function table */                                          \
    struct SNAME##_fval                                                        \
    {                                                                          \
        /* Comparator function */                                              \
        int (*cmp)(V, V);                                                      \
                                                                               \
        /* Copy function */                                                    \
        V (*cpy)(V);                                                           \
                                                                               \
        /* To string function */                                               \
        bool (*str)(FILE *, V);                                                \
                                                                               \
        /* Free from memory function */                                        \
        void (*free)(V);                                                       \
                                                                               \
        /* Hash function */                                                    \
        size_t (*hash)(V);                                                     \
                                                                               \
        /* Priority function */                                                \
        int (*pri)(V, V);                                                      \
    };                                                                         \
                                                                               \
    /* Hashset Iterator */                                                     \
    struct SNAME##_iter                                                        \
    {                                                                          \
        /* Target Hashset */                                                   \
        struct SNAME *target;                                                  \
                                                                               \
        /* Cursor's position (index) */                                        \
        size_t cursor;                                                         \
                                                                               \
        /* Keeps track of relative index to the iteration of elements */       \
        size_t index;                                                          \
                                                                               \
        /* The index of the first element */                                   \
        size_t first;                                                          \
                                                                               \
        /* The index of the last element */                                    \
        size_t last;                                                           \
                                                                               \
        /* If the iterator has reached the start of the iteration */           \
        bool start;                                                            \
                                                                               \
        /* If the iterator has reached the end of the iteration */             \
        bool end;                                                              \
    };                                                                         \
                                                                               \
    /* Collection Functions */                                                 \
    /* Collection Allocation and Deallocation */                               \
    struct SNAME *PFX##_new(size_t capacity, double load,                      \
                            struct SNAME##_fval *f_val);                       \
    struct SNAME *PFX##_new_custom(                                            \
        size_t capacity, double load, struct SNAME##_fval *f_val,              \
        struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);        \
    void PFX##_clear(struct SNAME *_set_);                                     \
    void PFX##_free(struct SNAME *_set_);                                      \
    /* Customization of Allocation and Callbacks */                            \
    void PFX##_customize(struct SNAME *_set_, struct cmc_alloc_node *alloc,    \
                         struct cmc_callbacks *callbacks);                     \
    /* Collection Input and Output */                                          \
    bool PFX##_insert(struct SNAME *_set_, V value);                           \
    bool PFX##_remove(struct SNAME *_set_, V value);                           \
    /* Element Access */                                                       \
    bool PFX##_max(struct SNAME *_set_, V *value);                             \
    bool PFX##_min(struct SNAME *_set_, V *value);                             \
    /* Collection State */                                                     \
    bool PFX##_contains(struct SNAME *_set_, V value);                         \
    bool PFX##_empty(struct SNAME *_set_);                                     \
    bool PFX##_full(struct SNAME *_set_);                                      \
    size_t PFX##_count(struct SNAME *_set_);                                   \
    size_t PFX##_capacity(struct SNAME *_set_);                                \
    double PFX##_load(struct SNAME *_set_);                                    \
    int PFX##_flag(struct SNAME *_set_);                                       \
    /* Collection Utility */                                                   \
    bool PFX##_resize(struct SNAME *_set_, size_t capacity);                   \
    struct SNAME *PFX##_copy_of(struct SNAME *_set_);                          \
    bool PFX##_equals(struct SNAME *_set1_, struct SNAME *_set2_);             \
    struct cmc_string PFX##_to_string(struct SNAME *_set_);                    \
    bool PFX##_print(struct SNAME *_set_, FILE *fptr);                         \
                                                                               \
    /* Set Operations */                                                       \
    struct SNAME *PFX##_union(struct SNAME *_set1_, struct SNAME *_set2_);     \
    struct SNAME *PFX##_intersection(struct SNAME *_set1_,                     \
                                     struct SNAME *_set2_);                    \
    struct SNAME *PFX##_difference(struct SNAME *_set1_,                       \
                                   struct SNAME *_set2_);                      \
    struct SNAME *PFX##_symmetric_difference(struct SNAME *_set1_,             \
                                             struct SNAME *_set2_);            \
    bool PFX##_is_subset(struct SNAME *_set1_, struct SNAME *_set2_);          \
    bool PFX##_is_superset(struct SNAME *_set1_, struct SNAME *_set2_);        \
    bool PFX##_is_proper_subset(struct SNAME *_set1_, struct SNAME *_set2_);   \
    bool PFX##_is_proper_superset(struct SNAME *_set1_, struct SNAME *_set2_); \
    bool PFX##_is_disjointset(struct SNAME *_set1_, struct SNAME *_set2_);     \
                                                                               \
    /* Iterator Functions */                                                   \
    /* Iterator Initialization */                                              \
    struct SNAME##_iter PFX##_iter_start(struct SNAME *target);                \
    struct SNAME##_iter PFX##_iter_end(struct SNAME *target);                  \
    /* Iterator State */                                                       \
    bool PFX##_iter_at_start(struct SNAME##_iter *iter);                       \
    bool PFX##_iter_at_end(struct SNAME##_iter *iter);                         \
    /* Iterator Movement */                                                    \
    bool PFX##_iter_to_start(struct SNAME##_iter *iter);                       \
    bool PFX##_iter_to_end(struct SNAME##_iter *iter);                         \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                           \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                           \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);          \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);           \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);            \
    /* Iterator Access */                                                      \
    V PFX##_iter_value(struct SNAME##_iter *iter);                             \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_HASHSET_SOURCE(PFX, SNAME, V)                             \
                                                                               \
    /* Implementation Detail Functions */                                      \
    static struct SNAME##_entry *PFX##_impl_get_entry(struct SNAME *_set_,     \
                                                      V value);                \
    static size_t PFX##_impl_calculate_size(size_t required);                  \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_set_);       \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_set_);         \
                                                                               \
    struct SNAME *PFX##_new(size_t capacity, double load,                      \
                            struct SNAME##_fval *f_val)                        \
    {                                                                          \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;                \
                                                                               \
        if (capacity == 0 || load <= 0 || load >= 1)                           \
            return NULL;                                                       \
                                                                               \
        /* Prevent integer overflow */                                         \
        if (capacity >= UINTMAX_MAX * load)                                    \
            return NULL;                                                       \
                                                                               \
        if (!f_val)                                                            \
            return NULL;                                                       \
                                                                               \
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);     \
                                                                               \
        struct SNAME *_set_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_set_)                                                            \
            return NULL;                                                       \
                                                                               \
        _set_->buffer =                                                        \
            alloc->calloc(real_capacity, sizeof(struct SNAME##_entry));        \
                                                                               \
        if (!_set_->buffer)                                                    \
        {                                                                      \
            alloc->free(_set_);                                                \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _set_->count = 0;                                                      \
        _set_->capacity = real_capacity;                                       \
        _set_->load = load;                                                    \
        _set_->flag = cmc_flags.OK;                                            \
        _set_->f_val = f_val;                                                  \
        _set_->alloc = alloc;                                                  \
        _set_->callbacks = NULL;                                               \
                                                                               \
        return _set_;                                                          \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_new_custom(                                            \
        size_t capacity, double load, struct SNAME##_fval *f_val,              \
        struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)         \
    {                                                                          \
        if (capacity == 0 || load <= 0 || load >= 1)                           \
            return NULL;                                                       \
                                                                               \
        /* Prevent integer overflow */                                         \
        if (capacity >= UINTMAX_MAX * load)                                    \
            return NULL;                                                       \
                                                                               \
        if (!f_val)                                                            \
            return NULL;                                                       \
                                                                               \
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);     \
                                                                               \
        if (!alloc)                                                            \
            alloc = &cmc_alloc_node_default;                                   \
                                                                               \
        struct SNAME *_set_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_set_)                                                            \
            return NULL;                                                       \
                                                                               \
        _set_->buffer =                                                        \
            alloc->calloc(real_capacity, sizeof(struct SNAME##_entry));        \
                                                                               \
        if (!_set_->buffer)                                                    \
        {                                                                      \
            alloc->free(_set_);                                                \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _set_->count = 0;                                                      \
        _set_->capacity = real_capacity;                                       \
        _set_->load = load;                                                    \
        _set_->flag = cmc_flags.OK;                                            \
        _set_->f_val = f_val;                                                  \
        _set_->alloc = alloc;                                                  \
        _set_->callbacks = callbacks;                                          \
                                                                               \
        return _set_;                                                          \
    }                                                                          \
                                                                               \
    void PFX##_clear(struct SNAME *_set_)                                      \
    {                                                                          \
        if (_set_->f_val->free)                                                \
        {                                                                      \
            for (size_t i = 0; i < _set_->capacity; i++)                       \
            {                                                                  \
                struct SNAME##_entry *entry = &(_set_->buffer[i]);             \
                                                                               \
                if (entry->state == CMC_ES_FILLED)                             \
                {                                                              \
                    _set_->f_val->free(entry->value);                          \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        memset(_set_->buffer, 0,                                               \
               sizeof(struct SNAME##_entry) * _set_->capacity);                \
                                                                               \
        _set_->count = 0;                                                      \
        _set_->flag = cmc_flags.OK;                                            \
    }                                                                          \
                                                                               \
    void PFX##_free(struct SNAME *_set_)                                       \
    {                                                                          \
        if (_set_->f_val->free)                                                \
        {                                                                      \
            for (size_t i = 0; i < _set_->capacity; i++)                       \
            {                                                                  \
                struct SNAME##_entry *entry = &(_set_->buffer[i]);             \
                                                                               \
                if (entry->state == CMC_ES_FILLED)                             \
                {                                                              \
                    _set_->f_val->free(entry->value);                          \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        _set_->alloc->free(_set_->buffer);                                     \
        _set_->alloc->free(_set_);                                             \
    }                                                                          \
                                                                               \
    void PFX##_customize(struct SNAME *_set_, struct cmc_alloc_node *alloc,    \
                         struct cmc_callbacks *callbacks)                      \
    {                                                                          \
        if (!alloc)                                                            \
            _set_->alloc = &cmc_alloc_node_default;                            \
        else                                                                   \
            _set_->alloc = alloc;                                              \
                                                                               \
        _set_->callbacks = callbacks;                                          \
                                                                               \
        _set_->flag = cmc_flags.OK;                                            \
    }                                                                          \
                                                                               \
    bool PFX##_insert(struct SNAME *_set_, V value)                            \
    {                                                                          \
        if (PFX##_full(_set_))                                                 \
        {                                                                      \
            if (!PFX##_resize(_set_, _set_->capacity + 1))                     \
                return false;                                                  \
        }                                                                      \
                                                                               \
        if (PFX##_impl_get_entry(_set_, value) != NULL)                        \
        {                                                                      \
            _set_->flag = cmc_flags.DUPLICATE;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        size_t hash = _set_->f_val->hash(value);                               \
        size_t original_pos = hash % _set_->capacity;                          \
        size_t pos = original_pos;                                             \
                                                                               \
        struct SNAME##_entry *target = &(_set_->buffer[pos]);                  \
                                                                               \
        if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)  \
        {                                                                      \
            target->value = value;                                             \
            target->dist = 0;                                                  \
            target->state = CMC_ES_FILLED;                                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            while (true)                                                       \
            {                                                                  \
                pos++;                                                         \
                target = &(_set_->buffer[pos % _set_->capacity]);              \
                                                                               \
                if (target->state == CMC_ES_EMPTY ||                           \
                    target->state == CMC_ES_DELETED)                           \
                {                                                              \
                    target->value = value;                                     \
                    target->dist = pos - original_pos;                         \
                    target->state = CMC_ES_FILLED;                             \
                                                                               \
                    break;                                                     \
                }                                                              \
                else if (target->dist < pos - original_pos)                    \
                {                                                              \
                    V tmp = target->value;                                     \
                    size_t tmp_dist = target->dist;                            \
                                                                               \
                    target->value = value;                                     \
                    target->dist = pos - original_pos;                         \
                                                                               \
                    value = tmp;                                               \
                    original_pos = pos - tmp_dist;                             \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        _set_->count++;                                                        \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->create)                      \
            _set_->callbacks->create();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_remove(struct SNAME *_set_, V value)                            \
    {                                                                          \
        if (PFX##_empty(_set_))                                                \
        {                                                                      \
            _set_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry *result = PFX##_impl_get_entry(_set_, value);     \
                                                                               \
        if (result == NULL)                                                    \
        {                                                                      \
            _set_->flag = cmc_flags.NOT_FOUND;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        result->value = (V){ 0 };                                              \
        result->dist = 0;                                                      \
        result->state = CMC_ES_DELETED;                                        \
                                                                               \
        _set_->count--;                                                        \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->delete)                      \
            _set_->callbacks->delete ();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_max(struct SNAME *_set_, V *value)                              \
    {                                                                          \
        if (PFX##_empty(_set_))                                                \
        {                                                                      \
            _set_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        V max_value;                                                           \
        struct SNAME##_iter iter = PFX##_iter_start(_set_);                    \
                                                                               \
        /* TODO turn this into a normal loop */                                \
        for (; !PFX##_iter_at_end(&iter); PFX##_iter_next(&iter))              \
        {                                                                      \
            V result = PFX##_iter_value(&iter);                                \
            size_t index = PFX##_iter_index(&iter);                            \
                                                                               \
            if (index == 0)                                                    \
                max_value = result;                                            \
            else if (_set_->f_val->cmp(result, max_value) > 0)                 \
                max_value = result;                                            \
        }                                                                      \
                                                                               \
        if (value)                                                             \
            *value = max_value;                                                \
                                                                               \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->read)                        \
            _set_->callbacks->read();                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_min(struct SNAME *_set_, V *value)                              \
    {                                                                          \
        if (PFX##_empty(_set_))                                                \
        {                                                                      \
            _set_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        V min_value;                                                           \
        struct SNAME##_iter iter = PFX##_iter_start(_set_);                    \
                                                                               \
        /* TODO turn this into a normal loop */                                \
        for (; !PFX##_iter_at_end(&iter); PFX##_iter_next(&iter))              \
        {                                                                      \
            V result = PFX##_iter_value(&iter);                                \
            size_t index = PFX##_iter_index(&iter);                            \
                                                                               \
            if (index == 0)                                                    \
                min_value = result;                                            \
            else if (_set_->f_val->cmp(result, min_value) < 0)                 \
                min_value = result;                                            \
        }                                                                      \
                                                                               \
        if (value)                                                             \
            *value = min_value;                                                \
                                                                               \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->read)                        \
            _set_->callbacks->read();                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_contains(struct SNAME *_set_, V value)                          \
    {                                                                          \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        bool result = PFX##_impl_get_entry(_set_, value) != NULL;              \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->read)                        \
            _set_->callbacks->read();                                          \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_empty(struct SNAME *_set_)                                      \
    {                                                                          \
        return _set_->count == 0;                                              \
    }                                                                          \
                                                                               \
    bool PFX##_full(struct SNAME *_set_)                                       \
    {                                                                          \
        return (double)_set_->capacity * _set_->load <= (double)_set_->count;  \
    }                                                                          \
                                                                               \
    size_t PFX##_count(struct SNAME *_set_)                                    \
    {                                                                          \
        return _set_->count;                                                   \
    }                                                                          \
                                                                               \
    size_t PFX##_capacity(struct SNAME *_set_)                                 \
    {                                                                          \
        return _set_->capacity;                                                \
    }                                                                          \
                                                                               \
    double PFX##_load(struct SNAME *_set_)                                     \
    {                                                                          \
        return _set_->load;                                                    \
    }                                                                          \
                                                                               \
    int PFX##_flag(struct SNAME *_set_)                                        \
    {                                                                          \
        return _set_->flag;                                                    \
    }                                                                          \
                                                                               \
    bool PFX##_resize(struct SNAME *_set_, size_t capacity)                    \
    {                                                                          \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_set_->capacity == capacity)                                       \
            goto success;                                                      \
                                                                               \
        if (_set_->capacity > capacity / _set_->load)                          \
            goto success;                                                      \
                                                                               \
        /* Prevent integer overflow */                                         \
        if (capacity >= UINTMAX_MAX * _set_->load)                             \
        {                                                                      \
            _set_->flag = cmc_flags.ERROR;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* Calculate required capacity based on the prime numbers */           \
        size_t theoretical_size = PFX##_impl_calculate_size(capacity);         \
                                                                               \
        /* Not possible to shrink with current available prime numbers */      \
        if (theoretical_size < _set_->count / _set_->load)                     \
        {                                                                      \
            _set_->flag = cmc_flags.INVALID;                                   \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* No callbacks since _new_set_ is just a temporary hashtable */       \
        struct SNAME *_new_set_ = PFX##_new_custom(                            \
            capacity, _set_->load, _set_->f_val, _set_->alloc, NULL);          \
                                                                               \
        if (!_new_set_)                                                        \
        {                                                                      \
            _set_->flag = cmc_flags.ALLOC;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_iter iter = PFX##_iter_start(_set_);                    \
                                                                               \
        /* TODO turn this into a normal loop */                                \
        for (; !PFX##_iter_at_end(&iter); PFX##_iter_next(&iter))              \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            /* TODO check for error */                                         \
            PFX##_insert(_new_set_, value);                                    \
        }                                                                      \
                                                                               \
        /* Unlikely */                                                         \
        if (_set_->count != _new_set_->count)                                  \
        {                                                                      \
            PFX##_free(_new_set_);                                             \
                                                                               \
            _set_->flag = cmc_flags.ERROR;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_entry *tmp_b = _set_->buffer;                           \
        _set_->buffer = _new_set_->buffer;                                     \
        _new_set_->buffer = tmp_b;                                             \
                                                                               \
        size_t tmp_c = _set_->capacity;                                        \
        _set_->capacity = _new_set_->capacity;                                 \
        _new_set_->capacity = tmp_c;                                           \
                                                                               \
        /* Prevent the set from freeing the data */                            \
        _new_set_->f_val = &(struct SNAME##_fval){ NULL };                     \
                                                                               \
        PFX##_free(_new_set_);                                                 \
                                                                               \
    success:                                                                   \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->resize)                      \
            _set_->callbacks->resize();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_copy_of(struct SNAME *_set_)                           \
    {                                                                          \
        struct SNAME *result =                                                 \
            PFX##_new_custom(_set_->capacity * _set_->load, _set_->load,       \
                             _set_->f_val, _set_->alloc, _set_->callbacks);    \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _set_->flag = cmc_flags.ERROR;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (_set_->f_val->cpy)                                                 \
        {                                                                      \
            for (size_t i = 0; i < _set_->capacity; i++)                       \
            {                                                                  \
                struct SNAME##_entry *scan = &(_set_->buffer[i]);              \
                                                                               \
                if (scan->state != CMC_ES_EMPTY)                               \
                {                                                              \
                    struct SNAME##_entry *target = &(result->buffer[i]);       \
                                                                               \
                    if (scan->state == CMC_ES_DELETED)                         \
                        target->state = CMC_ES_DELETED;                        \
                    else                                                       \
                    {                                                          \
                        target->state = scan->state;                           \
                        target->dist = scan->dist;                             \
                                                                               \
                        target->value = _set_->f_val->cpy(scan->value);        \
                    }                                                          \
                }                                                              \
            }                                                                  \
        }                                                                      \
        else                                                                   \
            memcpy(result->buffer, _set_->buffer,                              \
                   sizeof(struct SNAME##_entry) * _set_->capacity);            \
                                                                               \
        result->count = _set_->count;                                          \
                                                                               \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_equals(struct SNAME *_set1_, struct SNAME *_set2_)              \
    {                                                                          \
        _set1_->flag = cmc_flags.OK;                                           \
        _set2_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_set1_->count != _set2_->count)                                    \
            return false;                                                      \
                                                                               \
        if (_set1_->count == 0)                                                \
            return true;                                                       \
                                                                               \
        struct SNAME##_iter iter = PFX##_iter_start(_set1_);                   \
                                                                               \
        /* TODO optimize this loop */                                          \
        for (; !PFX##_iter_at_end(&iter); PFX##_iter_next(&iter))              \
        {                                                                      \
            if (PFX##_impl_get_entry(_set2_, PFX##_iter_value(&iter)) == NULL) \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct cmc_string PFX##_to_string(struct SNAME *_set_)                     \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *s_ = _set_;                                              \
                                                                               \
        int n =                                                                \
            snprintf(str.s, cmc_string_len, cmc_string_fmt_hashset, #SNAME,    \
                     #V, s_, s_->buffer, s_->capacity, s_->count, s_->load,    \
                     s_->flag, s_->f_val, s_->alloc, s_->callbacks);           \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    bool PFX##_print(struct SNAME *_set_, FILE *fptr)                          \
    {                                                                          \
        for (size_t i = 0; i < _set_->capacity; i++)                           \
        {                                                                      \
            struct SNAME##_entry *entry = &(_set_->buffer[i]);                 \
                                                                               \
            if (entry->state == CMC_ES_FILLED)                                 \
            {                                                                  \
                if (!_set_->f_val->str(fptr, entry->value))                    \
                    return false;                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_union(struct SNAME *_set1_, struct SNAME *_set2_)      \
    {                                                                          \
        /* Callbacks are added later */                                        \
        struct SNAME *_set_r_ =                                                \
            PFX##_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val,    \
                             _set1_->alloc, NULL);                             \
                                                                               \
        if (!_set_r_)                                                          \
        {                                                                      \
            _set1_->flag = cmc_flags.ALLOC;                                    \
            _set2_->flag = cmc_flags.ALLOC;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct SNAME##_iter iter1 = PFX##_iter_start(_set1_);                  \
        struct SNAME##_iter iter2 = PFX##_iter_start(_set2_);                  \
                                                                               \
        /* TODO turn this into a normal loop and check for errors */           \
        for (; !PFX##_iter_at_end(&iter1); PFX##_iter_next(&iter1))            \
        {                                                                      \
            PFX##_insert(_set_r_, PFX##_iter_value(&iter1));                   \
        }                                                                      \
                                                                               \
        for (; !PFX##_iter_at_end(&iter2); PFX##_iter_next(&iter2))            \
        {                                                                      \
            PFX##_insert(_set_r_, PFX##_iter_value(&iter2));                   \
        }                                                                      \
                                                                               \
        _set_r_->callbacks = _set1_->callbacks;                                \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_intersection(struct SNAME *_set1_,                     \
                                     struct SNAME *_set2_)                     \
    {                                                                          \
        /* Callbacks are added later */                                        \
        struct SNAME *_set_r_ =                                                \
            PFX##_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val,    \
                             _set1_->alloc, NULL);                             \
                                                                               \
        if (!_set_r_)                                                          \
        {                                                                      \
            _set1_->flag = cmc_flags.ALLOC;                                    \
            _set2_->flag = cmc_flags.ALLOC;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct SNAME *_set_A_ =                                                \
            _set1_->count < _set2_->count ? _set1_ : _set2_;                   \
        struct SNAME *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;           \
                                                                               \
        struct SNAME##_iter iter = PFX##_iter_start(_set_A_);                  \
                                                                               \
        for (; !PFX##_iter_at_end(&iter); PFX##_iter_next(&iter))              \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            if (PFX##_impl_get_entry(_set_B_, value) != NULL)                  \
                PFX##_insert(_set_r_, value);                                  \
        }                                                                      \
                                                                               \
        _set_r_->callbacks = _set1_->callbacks;                                \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_difference(struct SNAME *_set1_, struct SNAME *_set2_) \
    {                                                                          \
        /* Callbacks are added later */                                        \
        struct SNAME *_set_r_ =                                                \
            PFX##_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val,    \
                             _set1_->alloc, NULL);                             \
                                                                               \
        if (!_set_r_)                                                          \
        {                                                                      \
            _set1_->flag = cmc_flags.ALLOC;                                    \
            _set2_->flag = cmc_flags.ALLOC;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct SNAME##_iter iter = PFX##_iter_start(_set1_);                   \
                                                                               \
        for (; !PFX##_iter_at_end(&iter); PFX##_iter_next(&iter))              \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            if (PFX##_impl_get_entry(_set2_, value) == NULL)                   \
                PFX##_insert(_set_r_, value);                                  \
        }                                                                      \
                                                                               \
        _set_r_->callbacks = _set1_->callbacks;                                \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_symmetric_difference(struct SNAME *_set1_,             \
                                             struct SNAME *_set2_)             \
    {                                                                          \
        /* Callbacks are added later */                                        \
        struct SNAME *_set_r_ =                                                \
            PFX##_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val,    \
                             _set1_->alloc, NULL);                             \
                                                                               \
        if (!_set_r_)                                                          \
        {                                                                      \
            _set1_->flag = cmc_flags.ALLOC;                                    \
            _set2_->flag = cmc_flags.ALLOC;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct SNAME##_iter iter1 = PFX##_iter_start(_set1_);                  \
        struct SNAME##_iter iter2 = PFX##_iter_start(_set2_);                  \
                                                                               \
        for (; !PFX##_iter_at_end(&iter1); PFX##_iter_next(&iter1))            \
        {                                                                      \
            V value = PFX##_iter_value(&iter1);                                \
                                                                               \
            if (PFX##_impl_get_entry(_set2_, value) == NULL)                   \
                PFX##_insert(_set_r_, value);                                  \
        }                                                                      \
                                                                               \
        for (; !PFX##_iter_at_end(&iter2); PFX##_iter_next(&iter2))            \
        {                                                                      \
            V value = PFX##_iter_value(&iter2);                                \
                                                                               \
            if (PFX##_impl_get_entry(_set1_, value) == NULL)                   \
                PFX##_insert(_set_r_, value);                                  \
        }                                                                      \
                                                                               \
        _set_r_->callbacks = _set1_->callbacks;                                \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    /* Is _set1_ a subset of _set2_ ? */                                       \
    /* A set X is a subset of a set Y when: X <= Y */                          \
    /* If X is a subset of Y, then Y is a superset of X */                     \
    bool PFX##_is_subset(struct SNAME *_set1_, struct SNAME *_set2_)           \
    {                                                                          \
        _set1_->flag = cmc_flags.OK;                                           \
        _set2_->flag = cmc_flags.OK;                                           \
                                                                               \
        /* If the cardinality of _set1_ is greater than that of _set2_, */     \
        /* then it is safe to say that _set1_ can't be a subset of _set2_ */   \
        if (_set1_->count > _set2_->count)                                     \
            return false;                                                      \
                                                                               \
        /* The empty set is a subset of all sets */                            \
        if (PFX##_empty(_set1_))                                               \
            return true;                                                       \
                                                                               \
        struct SNAME##_iter iter = PFX##_iter_start(_set1_);                   \
                                                                               \
        for (; !PFX##_iter_at_end(&iter); PFX##_iter_next(&iter))              \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            if (PFX##_impl_get_entry(_set2_, value) == NULL)                   \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Is _set1_ a superset of _set2_ ? */                                     \
    /* A set X is a superset of a set Y when: X >= Y */                        \
    /* If X is a superset of Y, then Y is a subset of X */                     \
    bool PFX##_is_superset(struct SNAME *_set1_, struct SNAME *_set2_)         \
    {                                                                          \
        return PFX##_is_subset(_set2_, _set1_);                                \
    }                                                                          \
                                                                               \
    /* Is _set1_ a proper subset of _set2_ ? */                                \
    /* A set X is a proper subset of a set Y when: X < Y */                    \
    /* If X is a proper subset of Y, then Y is a proper superset of X */       \
    bool PFX##_is_proper_subset(struct SNAME *_set1_, struct SNAME *_set2_)    \
    {                                                                          \
        _set1_->flag = cmc_flags.OK;                                           \
        _set2_->flag = cmc_flags.OK;                                           \
                                                                               \
        /* If the cardinality of _set1_ is greater than or equal to that of */ \
        /* _set2_, then it is safe to say that _set1_ can't be a proper */     \
        /* subset of _set2_ */                                                 \
        if (_set1_->count >= _set2_->count)                                    \
            return false;                                                      \
                                                                               \
        if (PFX##_empty(_set1_))                                               \
        {                                                                      \
            /* The empty set is a proper subset of all non-empty sets */       \
            if (!PFX##_empty(_set2_))                                          \
                return true;                                                   \
            /* The empty set is not a proper subset of itself (this is true */ \
            /* for any set) */                                                 \
            else                                                               \
                return false;                                                  \
        }                                                                      \
                                                                               \
        struct SNAME##_iter iter = PFX##_iter_start(_set1_);                   \
                                                                               \
        for (; !PFX##_iter_at_end(&iter); PFX##_iter_next(&iter))              \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            if (PFX##_impl_get_entry(_set2_, value) == NULL)                   \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Is _set1_ a proper superset of _set2_ ? */                              \
    /* A set X is a proper superset of a set Y when: X > Y */                  \
    /* If X is a proper superset of Y, then Y is a proper subset of X */       \
    bool PFX##_is_proper_superset(struct SNAME *_set1_, struct SNAME *_set2_)  \
    {                                                                          \
        return PFX##_is_proper_subset(_set2_, _set1_);                         \
    }                                                                          \
                                                                               \
    /* Is _set1_ a disjointset of _set2_ ? */                                  \
    /* A set X is a disjointset of a set Y if their intersection is empty, */  \
    /* that is, if there are no elements in common between the two */          \
    bool PFX##_is_disjointset(struct SNAME *_set1_, struct SNAME *_set2_)      \
    {                                                                          \
        _set1_->flag = cmc_flags.OK;                                           \
        _set2_->flag = cmc_flags.OK;                                           \
                                                                               \
        /* The intersection of an empty set with any other set will result */  \
        /* in an empty set */                                                  \
        if (PFX##_empty(_set1_))                                               \
            return true;                                                       \
                                                                               \
        struct SNAME##_iter iter = PFX##_iter_start(_set1_);                   \
                                                                               \
        for (; !PFX##_iter_at_end(&iter); PFX##_iter_next(&iter))              \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            if (PFX##_impl_get_entry(_set2_, value) != NULL)                   \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME##_iter PFX##_iter_start(struct SNAME *target)                 \
    {                                                                          \
        struct SNAME##_iter iter;                                              \
                                                                               \
        iter.target = target;                                                  \
        iter.cursor = 0;                                                       \
        iter.index = 0;                                                        \
        iter.first = 0;                                                        \
        iter.last = 0;                                                         \
        iter.start = true;                                                     \
        iter.end = PFX##_empty(target);                                        \
                                                                               \
        if (!PFX##_empty(target))                                              \
        {                                                                      \
            for (size_t i = 0; i < target->capacity; i++)                      \
            {                                                                  \
                if (target->buffer[i].state == CMC_ES_FILLED)                  \
                {                                                              \
                    iter.first = i;                                            \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            iter.cursor = iter.first;                                          \
                                                                               \
            for (size_t i = target->capacity; i > 0; i--)                      \
            {                                                                  \
                if (target->buffer[i - 1].state == CMC_ES_FILLED)              \
                {                                                              \
                    iter.last = i - 1;                                         \
                    break;                                                     \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME##_iter PFX##_iter_end(struct SNAME *target)                   \
    {                                                                          \
        struct SNAME##_iter iter;                                              \
                                                                               \
        iter.target = target;                                                  \
        iter.cursor = 0;                                                       \
        iter.index = 0;                                                        \
        iter.first = 0;                                                        \
        iter.last = 0;                                                         \
        iter.start = PFX##_empty(target);                                      \
        iter.end = true;                                                       \
                                                                               \
        if (!PFX##_empty(target))                                              \
        {                                                                      \
            for (size_t i = 0; i < target->capacity; i++)                      \
            {                                                                  \
                if (target->buffer[i].state == CMC_ES_FILLED)                  \
                {                                                              \
                    iter.first = i;                                            \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            for (size_t i = target->capacity; i > 0; i--)                      \
            {                                                                  \
                if (target->buffer[i - 1].state == CMC_ES_FILLED)              \
                {                                                              \
                    iter.last = i - 1;                                         \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            iter.cursor = iter.last;                                           \
            iter.index = target->count - 1;                                    \
        }                                                                      \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_iter_at_start(struct SNAME##_iter *iter)                        \
    {                                                                          \
        return PFX##_empty(iter->target) || iter->start;                       \
    }                                                                          \
                                                                               \
    bool PFX##_iter_at_end(struct SNAME##_iter *iter)                          \
    {                                                                          \
        return PFX##_empty(iter->target) || iter->end;                         \
    }                                                                          \
                                                                               \
    bool PFX##_iter_to_start(struct SNAME##_iter *iter)                        \
    {                                                                          \
        if (!PFX##_empty(iter->target))                                        \
        {                                                                      \
            iter->cursor = iter->first;                                        \
            iter->index = 0;                                                   \
            iter->start = true;                                                \
            iter->end = PFX##_empty(iter->target);                             \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool PFX##_iter_to_end(struct SNAME##_iter *iter)                          \
    {                                                                          \
        if (!PFX##_empty(iter->target))                                        \
        {                                                                      \
            iter->cursor = iter->last;                                         \
            iter->index = iter->target->count - 1;                             \
            iter->start = PFX##_empty(iter->target);                           \
            iter->end = true;                                                  \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                            \
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
        iter->start = PFX##_empty(iter->target);                               \
                                                                               \
        struct SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);    \
                                                                               \
        iter->index++;                                                         \
                                                                               \
        while (1)                                                              \
        {                                                                      \
            iter->cursor++;                                                    \
            scan = &(iter->target->buffer[iter->cursor]);                      \
                                                                               \
            if (scan->state == CMC_ES_FILLED)                                  \
                break;                                                         \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                            \
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
        iter->end = PFX##_empty(iter->target);                                 \
                                                                               \
        struct SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);    \
                                                                               \
        iter->index--;                                                         \
                                                                               \
        while (1)                                                              \
        {                                                                      \
            iter->cursor--;                                                    \
            scan = &(iter->target->buffer[iter->cursor]);                      \
                                                                               \
            if (scan->state == CMC_ES_FILLED)                                  \
                break;                                                         \
        }                                                                      \
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
    V PFX##_iter_value(struct SNAME##_iter *iter)                              \
    {                                                                          \
        if (PFX##_empty(iter->target))                                         \
            return (V){ 0 };                                                   \
                                                                               \
        return iter->target->buffer[iter->cursor].value;                       \
    }                                                                          \
                                                                               \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                         \
    {                                                                          \
        return iter->index;                                                    \
    }                                                                          \
                                                                               \
    static struct SNAME##_entry *PFX##_impl_get_entry(struct SNAME *_set_,     \
                                                      V value)                 \
    {                                                                          \
        size_t hash = _set_->f_val->hash(value);                               \
        size_t pos = hash % _set_->capacity;                                   \
                                                                               \
        struct SNAME##_entry *target = &(_set_->buffer[pos]);                  \
                                                                               \
        while (target->state == CMC_ES_FILLED ||                               \
               target->state == CMC_ES_DELETED)                                \
        {                                                                      \
            if (_set_->f_val->cmp(target->value, value) == 0)                  \
                return target;                                                 \
                                                                               \
            pos++;                                                             \
            target = &(_set_->buffer[pos % _set_->capacity]);                  \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    static size_t PFX##_impl_calculate_size(size_t required)                   \
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
    }

#endif /* CMC_HASHSET_H */
