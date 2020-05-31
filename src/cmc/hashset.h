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

#ifndef CMC_CMC_HASHSET_H
#define CMC_CMC_HASHSET_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * Hashtable Implementation
 * ------------------------------------------------------------------------- */
#include "../cor/hashtable.h"

/**
 * Core HashSet implementation
 */
#define CMC_CMC_HASHSET_CORE(BODY)    \
    CMC_CMC_HASHSET_CORE_HEADER(BODY) \
    CMC_CMC_HASHSET_CORE_SOURCE(BODY)

#define CMC_CMC_HASHSET_CORE_HEADER(BODY)                                    \
    CMC_CMC_HASHSET_CORE_HEADER_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                                 CMC_PARAM_V(BODY))

#define CMC_CMC_HASHSET_CORE_SOURCE(BODY)                                    \
    CMC_CMC_HASHSET_CORE_SOURCE_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                                 CMC_PARAM_V(BODY))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHSET_CORE_HEADER_(PFX, SNAME, V)                           \
                                                                              \
    /* Hashset Structure */                                                   \
    struct SNAME                                                              \
    {                                                                         \
        /* Array of Entries */                                                \
        struct CMC_DEF_ENTRY(SNAME) * buffer;                                 \
                                                                              \
        /* Current Array Capcity */                                           \
        size_t capacity;                                                      \
                                                                              \
        /* Current amount of elements */                                      \
        size_t count;                                                         \
                                                                              \
        /* Load factor in range (0.0, 1.0) */                                 \
        double load;                                                          \
                                                                              \
        /* Flags indicating errors or success */                              \
        int flag;                                                             \
                                                                              \
        /* Value function table */                                            \
        struct CMC_DEF_FVAL(SNAME) * f_val;                                   \
                                                                              \
        /* Custom allocation functions */                                     \
        struct cmc_alloc_node *alloc;                                         \
                                                                              \
        /* Custom callback functions */                                       \
        struct cmc_callbacks *callbacks;                                      \
    };                                                                        \
                                                                              \
    struct CMC_DEF_ENTRY(SNAME)                                               \
    {                                                                         \
        /* Entry value */                                                     \
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
    /* Value struct function table */                                         \
    struct CMC_DEF_FVAL(SNAME)                                                \
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
    /* Hashset Iterator */                                                    \
    struct CMC_DEF_ITER(SNAME)                                                \
    {                                                                         \
        /* Target Hashset */                                                  \
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
    struct SNAME *CMC_(PFX, _new)(size_t capacity, double load,               \
                                  struct CMC_DEF_FVAL(SNAME) * f_val);        \
    struct SNAME *CMC_(PFX, _new_custom)(                                     \
        size_t capacity, double load, struct CMC_DEF_FVAL(SNAME) * f_val,     \
        struct cmc_alloc_node * alloc, struct cmc_callbacks * callbacks);     \
    void CMC_(PFX, _clear)(struct SNAME * _set_);                             \
    void CMC_(PFX, _free)(struct SNAME * _set_);                              \
    /* Customization of Allocation and Callbacks */                           \
    void CMC_(PFX, _customize)(struct SNAME * _set_,                          \
                               struct cmc_alloc_node * alloc,                 \
                               struct cmc_callbacks * callbacks);             \
    /* Collection Input and Output */                                         \
    bool CMC_(PFX, _insert)(struct SNAME * _set_, V value);                   \
    bool CMC_(PFX, _remove)(struct SNAME * _set_, V value);                   \
    /* Element Access */                                                      \
    bool CMC_(PFX, _max)(struct SNAME * _set_, V * value);                    \
    bool CMC_(PFX, _min)(struct SNAME * _set_, V * value);                    \
    /* Collection State */                                                    \
    bool CMC_(PFX, _contains)(struct SNAME * _set_, V value);                 \
    bool CMC_(PFX, _empty)(struct SNAME * _set_);                             \
    bool CMC_(PFX, _full)(struct SNAME * _set_);                              \
    size_t CMC_(PFX, _count)(struct SNAME * _set_);                           \
    size_t CMC_(PFX, _capacity)(struct SNAME * _set_);                        \
    double CMC_(PFX, _load)(struct SNAME * _set_);                            \
    int CMC_(PFX, _flag)(struct SNAME * _set_);                               \
    /* Collection Utility */                                                  \
    bool CMC_(PFX, _resize)(struct SNAME * _set_, size_t capacity);           \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _set_);                  \
    bool CMC_(PFX, _equals)(struct SNAME * _set1_, struct SNAME * _set2_);    \
                                                                              \
    /* Set Operations */                                                      \
    struct SNAME *CMC_(PFX, _union)(struct SNAME * _set1_,                    \
                                    struct SNAME * _set2_);                   \
    struct SNAME *CMC_(PFX, _intersection)(struct SNAME * _set1_,             \
                                           struct SNAME * _set2_);            \
    struct SNAME *CMC_(PFX, _difference)(struct SNAME * _set1_,               \
                                         struct SNAME * _set2_);              \
    struct SNAME *CMC_(PFX, _symmetric_difference)(struct SNAME * _set1_,     \
                                                   struct SNAME * _set2_);    \
    bool CMC_(PFX, _is_subset)(struct SNAME * _set1_, struct SNAME * _set2_); \
    bool CMC_(PFX, _is_superset)(struct SNAME * _set1_,                       \
                                 struct SNAME * _set2_);                      \
    bool CMC_(PFX, _is_proper_subset)(struct SNAME * _set1_,                  \
                                      struct SNAME * _set2_);                 \
    bool CMC_(PFX, _is_proper_superset)(struct SNAME * _set1_,                \
                                        struct SNAME * _set2_);               \
    bool CMC_(PFX, _is_disjointset)(struct SNAME * _set1_,                    \
                                    struct SNAME * _set2_);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHSET_CORE_SOURCE_(PFX, SNAME, V)                            \
                                                                               \
    /* Implementation Detail Functions */                                      \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        CMC_(PFX, _impl_get_entry)(struct SNAME * _set_, V value);             \
    static size_t CMC_(PFX, _impl_calculate_size)(size_t required);            \
    static struct CMC_DEF_ITER(SNAME)                                          \
        CMC_(PFX, _impl_it_start)(struct SNAME * _set_);                       \
    static struct CMC_DEF_ITER(SNAME)                                          \
        CMC_(PFX, _impl_it_end)(struct SNAME * _set_);                         \
                                                                               \
    struct SNAME *CMC_(PFX, _new)(size_t capacity, double load,                \
                                  struct CMC_DEF_FVAL(SNAME) * f_val)          \
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
        size_t real_capacity =                                                 \
            CMC_(PFX, _impl_calculate_size)(capacity / load);                  \
                                                                               \
        struct SNAME *_set_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_set_)                                                            \
            return NULL;                                                       \
                                                                               \
        _set_->buffer =                                                        \
            alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME))); \
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
        _set_->flag = CMC_FLAG_OK;                                             \
        _set_->f_val = f_val;                                                  \
        _set_->alloc = alloc;                                                  \
        _set_->callbacks = NULL;                                               \
                                                                               \
        return _set_;                                                          \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _new_custom)(                                      \
        size_t capacity, double load, struct CMC_DEF_FVAL(SNAME) * f_val,      \
        struct cmc_alloc_node * alloc, struct cmc_callbacks * callbacks)       \
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
        size_t real_capacity =                                                 \
            CMC_(PFX, _impl_calculate_size)(capacity / load);                  \
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
            alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME))); \
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
        _set_->flag = CMC_FLAG_OK;                                             \
        _set_->f_val = f_val;                                                  \
        _set_->alloc = alloc;                                                  \
        _set_->callbacks = callbacks;                                          \
                                                                               \
        return _set_;                                                          \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _clear)(struct SNAME * _set_)                               \
    {                                                                          \
        if (_set_->f_val->free)                                                \
        {                                                                      \
            for (size_t i = 0; i < _set_->capacity; i++)                       \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) *entry = &(_set_->buffer[i]);      \
                                                                               \
                if (entry->state == CMC_ES_FILLED)                             \
                {                                                              \
                    _set_->f_val->free(entry->value);                          \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        memset(_set_->buffer, 0,                                               \
               sizeof(struct CMC_DEF_ENTRY(SNAME)) * _set_->capacity);         \
                                                                               \
        _set_->count = 0;                                                      \
        _set_->flag = CMC_FLAG_OK;                                             \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _free)(struct SNAME * _set_)                                \
    {                                                                          \
        if (_set_->f_val->free)                                                \
        {                                                                      \
            for (size_t i = 0; i < _set_->capacity; i++)                       \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) *entry = &(_set_->buffer[i]);      \
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
    void CMC_(PFX, _customize)(struct SNAME * _set_,                           \
                               struct cmc_alloc_node * alloc,                  \
                               struct cmc_callbacks * callbacks)               \
    {                                                                          \
        if (!alloc)                                                            \
            _set_->alloc = &cmc_alloc_node_default;                            \
        else                                                                   \
            _set_->alloc = alloc;                                              \
                                                                               \
        _set_->callbacks = callbacks;                                          \
                                                                               \
        _set_->flag = CMC_FLAG_OK;                                             \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _insert)(struct SNAME * _set_, V value)                     \
    {                                                                          \
        if (CMC_(PFX, _full)(_set_))                                           \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_set_, _set_->capacity + 1))               \
                return false;                                                  \
        }                                                                      \
                                                                               \
        if (CMC_(PFX, _impl_get_entry)(_set_, value) != NULL)                  \
        {                                                                      \
            _set_->flag = CMC_FLAG_DUPLICATE;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        size_t hash = _set_->f_val->hash(value);                               \
        size_t original_pos = hash % _set_->capacity;                          \
        size_t pos = original_pos;                                             \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) *target = &(_set_->buffer[pos]);           \
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
        _set_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->create)                      \
            _set_->callbacks->create();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _remove)(struct SNAME * _set_, V value)                     \
    {                                                                          \
        if (CMC_(PFX, _empty)(_set_))                                          \
        {                                                                      \
            _set_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) *result =                                  \
            CMC_(PFX, _impl_get_entry)(_set_, value);                          \
                                                                               \
        if (result == NULL)                                                    \
        {                                                                      \
            _set_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        result->value = (V){ 0 };                                              \
        result->dist = 0;                                                      \
        result->state = CMC_ES_DELETED;                                        \
                                                                               \
        _set_->count--;                                                        \
        _set_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->delete)                      \
            _set_->callbacks->delete ();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _max)(struct SNAME * _set_, V * value)                      \
    {                                                                          \
        if (CMC_(PFX, _empty)(_set_))                                          \
        {                                                                      \
            _set_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        V max_value;                                                           \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set_);       \
                                                                               \
        /* TODO turn this into a normal loop */                                \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            V result = CMC_(PFX, _iter_value)(&iter);                          \
            size_t index = CMC_(PFX, _iter_index)(&iter);                      \
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
        _set_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->read)                        \
            _set_->callbacks->read();                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _min)(struct SNAME * _set_, V * value)                      \
    {                                                                          \
        if (CMC_(PFX, _empty)(_set_))                                          \
        {                                                                      \
            _set_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        V min_value;                                                           \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set_);       \
                                                                               \
        /* TODO turn this into a normal loop */                                \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            V result = CMC_(PFX, _iter_value)(&iter);                          \
            size_t index = CMC_(PFX, _iter_index)(&iter);                      \
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
        _set_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->read)                        \
            _set_->callbacks->read();                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _contains)(struct SNAME * _set_, V value)                   \
    {                                                                          \
        _set_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        bool result = CMC_(PFX, _impl_get_entry)(_set_, value) != NULL;        \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->read)                        \
            _set_->callbacks->read();                                          \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _empty)(struct SNAME * _set_)                               \
    {                                                                          \
        return _set_->count == 0;                                              \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _full)(struct SNAME * _set_)                                \
    {                                                                          \
        return (double)_set_->capacity * _set_->load <= (double)_set_->count;  \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _count)(struct SNAME * _set_)                             \
    {                                                                          \
        return _set_->count;                                                   \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _capacity)(struct SNAME * _set_)                          \
    {                                                                          \
        return _set_->capacity;                                                \
    }                                                                          \
                                                                               \
    double CMC_(PFX, _load)(struct SNAME * _set_)                              \
    {                                                                          \
        return _set_->load;                                                    \
    }                                                                          \
                                                                               \
    int CMC_(PFX, _flag)(struct SNAME * _set_)                                 \
    {                                                                          \
        return _set_->flag;                                                    \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _resize)(struct SNAME * _set_, size_t capacity)             \
    {                                                                          \
        _set_->flag = CMC_FLAG_OK;                                             \
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
            _set_->flag = CMC_FLAG_ERROR;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* Calculate required capacity based on the prime numbers */           \
        size_t theoretical_size = CMC_(PFX, _impl_calculate_size)(capacity);   \
                                                                               \
        /* Not possible to shrink with current available prime numbers */      \
        if (theoretical_size < _set_->count / _set_->load)                     \
        {                                                                      \
            _set_->flag = CMC_FLAG_INVALID;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* No callbacks since _new_set_ is just a temporary hashtable */       \
        struct SNAME *_new_set_ = CMC_(PFX, _new_custom)(                      \
            capacity, _set_->load, _set_->f_val, _set_->alloc, NULL);          \
                                                                               \
        if (!_new_set_)                                                        \
        {                                                                      \
            _set_->flag = CMC_FLAG_ALLOC;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set_);       \
                                                                               \
        /* TODO turn this into a normal loop */                                \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            V value = CMC_(PFX, _iter_value)(&iter);                           \
                                                                               \
            /* TODO check for error */                                         \
            CMC_(PFX, _insert)(_new_set_, value);                              \
        }                                                                      \
                                                                               \
        /* Unlikely */                                                         \
        if (_set_->count != _new_set_->count)                                  \
        {                                                                      \
            CMC_(PFX, _free)(_new_set_);                                       \
                                                                               \
            _set_->flag = CMC_FLAG_ERROR;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) *tmp_b = _set_->buffer;                    \
        _set_->buffer = _new_set_->buffer;                                     \
        _new_set_->buffer = tmp_b;                                             \
                                                                               \
        size_t tmp_c = _set_->capacity;                                        \
        _set_->capacity = _new_set_->capacity;                                 \
        _new_set_->capacity = tmp_c;                                           \
                                                                               \
        /* Prevent the set from freeing the data */                            \
        _new_set_->f_val = &(struct CMC_DEF_FVAL(SNAME)){ NULL };              \
                                                                               \
        CMC_(PFX, _free)(_new_set_);                                           \
                                                                               \
    success:                                                                   \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->resize)                      \
            _set_->callbacks->resize();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _set_)                    \
    {                                                                          \
        struct SNAME *result = CMC_(PFX, _new_custom)(                         \
            _set_->capacity * _set_->load, _set_->load, _set_->f_val,          \
            _set_->alloc, _set_->callbacks);                                   \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _set_->flag = CMC_FLAG_ERROR;                                      \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (_set_->f_val->cpy)                                                 \
        {                                                                      \
            for (size_t i = 0; i < _set_->capacity; i++)                       \
            {                                                                  \
                struct CMC_DEF_ENTRY(SNAME) *scan = &(_set_->buffer[i]);       \
                                                                               \
                if (scan->state != CMC_ES_EMPTY)                               \
                {                                                              \
                    struct CMC_DEF_ENTRY(SNAME) *target =                      \
                        &(result->buffer[i]);                                  \
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
                   sizeof(struct CMC_DEF_ENTRY(SNAME)) * _set_->capacity);     \
                                                                               \
        result->count = _set_->count;                                          \
                                                                               \
        _set_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _equals)(struct SNAME * _set1_, struct SNAME * _set2_)      \
    {                                                                          \
        _set1_->flag = CMC_FLAG_OK;                                            \
        _set2_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_set1_->count != _set2_->count)                                    \
            return false;                                                      \
                                                                               \
        if (_set1_->count == 0)                                                \
            return true;                                                       \
                                                                               \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_);      \
                                                                               \
        /* TODO optimize this loop */                                          \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            if (CMC_(PFX, _impl_get_entry)(                                    \
                    _set2_, CMC_(PFX, _iter_value)(&iter)) == NULL)            \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _union)(struct SNAME * _set1_,                     \
                                    struct SNAME * _set2_)                     \
    {                                                                          \
        /* Callbacks are added later */                                        \
        struct SNAME *_set_r_ =                                                \
            CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load,             \
                                   _set1_->f_val, _set1_->alloc, NULL);        \
                                                                               \
        if (!_set_r_)                                                          \
        {                                                                      \
            _set1_->flag = CMC_FLAG_ALLOC;                                     \
            _set2_->flag = CMC_FLAG_ALLOC;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ITER(SNAME) iter1 = CMC_(PFX, _iter_start)(_set1_);     \
        struct CMC_DEF_ITER(SNAME) iter2 = CMC_(PFX, _iter_start)(_set2_);     \
                                                                               \
        /* TODO turn this into a normal loop and check for errors */           \
        for (; !CMC_(PFX, _iter_at_end)(&iter1);                               \
             CMC_(PFX, _iter_next)(&iter1))                                    \
        {                                                                      \
            CMC_(PFX, _insert)(_set_r_, CMC_(PFX, _iter_value)(&iter1));       \
        }                                                                      \
                                                                               \
        for (; !CMC_(PFX, _iter_at_end)(&iter2);                               \
             CMC_(PFX, _iter_next)(&iter2))                                    \
        {                                                                      \
            CMC_(PFX, _insert)(_set_r_, CMC_(PFX, _iter_value)(&iter2));       \
        }                                                                      \
                                                                               \
        _set_r_->callbacks = _set1_->callbacks;                                \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _intersection)(struct SNAME * _set1_,              \
                                           struct SNAME * _set2_)              \
    {                                                                          \
        /* Callbacks are added later */                                        \
        struct SNAME *_set_r_ =                                                \
            CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load,             \
                                   _set1_->f_val, _set1_->alloc, NULL);        \
                                                                               \
        if (!_set_r_)                                                          \
        {                                                                      \
            _set1_->flag = CMC_FLAG_ALLOC;                                     \
            _set2_->flag = CMC_FLAG_ALLOC;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct SNAME *_set_A_ =                                                \
            _set1_->count < _set2_->count ? _set1_ : _set2_;                   \
        struct SNAME *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;           \
                                                                               \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set_A_);     \
                                                                               \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            V value = CMC_(PFX, _iter_value)(&iter);                           \
                                                                               \
            if (CMC_(PFX, _impl_get_entry)(_set_B_, value) != NULL)            \
                CMC_(PFX, _insert)(_set_r_, value);                            \
        }                                                                      \
                                                                               \
        _set_r_->callbacks = _set1_->callbacks;                                \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _difference)(struct SNAME * _set1_,                \
                                         struct SNAME * _set2_)                \
    {                                                                          \
        /* Callbacks are added later */                                        \
        struct SNAME *_set_r_ =                                                \
            CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load,             \
                                   _set1_->f_val, _set1_->alloc, NULL);        \
                                                                               \
        if (!_set_r_)                                                          \
        {                                                                      \
            _set1_->flag = CMC_FLAG_ALLOC;                                     \
            _set2_->flag = CMC_FLAG_ALLOC;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_);      \
                                                                               \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            V value = CMC_(PFX, _iter_value)(&iter);                           \
                                                                               \
            if (CMC_(PFX, _impl_get_entry)(_set2_, value) == NULL)             \
                CMC_(PFX, _insert)(_set_r_, value);                            \
        }                                                                      \
                                                                               \
        _set_r_->callbacks = _set1_->callbacks;                                \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _symmetric_difference)(struct SNAME * _set1_,      \
                                                   struct SNAME * _set2_)      \
    {                                                                          \
        /* Callbacks are added later */                                        \
        struct SNAME *_set_r_ =                                                \
            CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load,             \
                                   _set1_->f_val, _set1_->alloc, NULL);        \
                                                                               \
        if (!_set_r_)                                                          \
        {                                                                      \
            _set1_->flag = CMC_FLAG_ALLOC;                                     \
            _set2_->flag = CMC_FLAG_ALLOC;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ITER(SNAME) iter1 = CMC_(PFX, _iter_start)(_set1_);     \
        struct CMC_DEF_ITER(SNAME) iter2 = CMC_(PFX, _iter_start)(_set2_);     \
                                                                               \
        for (; !CMC_(PFX, _iter_at_end)(&iter1);                               \
             CMC_(PFX, _iter_next)(&iter1))                                    \
        {                                                                      \
            V value = CMC_(PFX, _iter_value)(&iter1);                          \
                                                                               \
            if (CMC_(PFX, _impl_get_entry)(_set2_, value) == NULL)             \
                CMC_(PFX, _insert)(_set_r_, value);                            \
        }                                                                      \
                                                                               \
        for (; !CMC_(PFX, _iter_at_end)(&iter2);                               \
             CMC_(PFX, _iter_next)(&iter2))                                    \
        {                                                                      \
            V value = CMC_(PFX, _iter_value)(&iter2);                          \
                                                                               \
            if (CMC_(PFX, _impl_get_entry)(_set1_, value) == NULL)             \
                CMC_(PFX, _insert)(_set_r_, value);                            \
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
    bool CMC_(PFX, _is_subset)(struct SNAME * _set1_, struct SNAME * _set2_)   \
    {                                                                          \
        _set1_->flag = CMC_FLAG_OK;                                            \
        _set2_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        /* If the cardinality of _set1_ is greater than that of _set2_, */     \
        /* then it is safe to say that _set1_ can't be a subset of _set2_ */   \
        if (_set1_->count > _set2_->count)                                     \
            return false;                                                      \
                                                                               \
        /* The empty set is a subset of all sets */                            \
        if (CMC_(PFX, _empty)(_set1_))                                         \
            return true;                                                       \
                                                                               \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_);      \
                                                                               \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            V value = CMC_(PFX, _iter_value)(&iter);                           \
                                                                               \
            if (CMC_(PFX, _impl_get_entry)(_set2_, value) == NULL)             \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Is _set1_ a superset of _set2_ ? */                                     \
    /* A set X is a superset of a set Y when: X >= Y */                        \
    /* If X is a superset of Y, then Y is a subset of X */                     \
    bool CMC_(PFX, _is_superset)(struct SNAME * _set1_, struct SNAME * _set2_) \
    {                                                                          \
        return CMC_(PFX, _is_subset)(_set2_, _set1_);                          \
    }                                                                          \
                                                                               \
    /* Is _set1_ a proper subset of _set2_ ? */                                \
    /* A set X is a proper subset of a set Y when: X < Y */                    \
    /* If X is a proper subset of Y, then Y is a proper superset of X */       \
    bool CMC_(PFX, _is_proper_subset)(struct SNAME * _set1_,                   \
                                      struct SNAME * _set2_)                   \
    {                                                                          \
        _set1_->flag = CMC_FLAG_OK;                                            \
        _set2_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        /* If the cardinality of _set1_ is greater than or equal to that of */ \
        /* _set2_, then it is safe to say that _set1_ can't be a proper */     \
        /* subset of _set2_ */                                                 \
        if (_set1_->count >= _set2_->count)                                    \
            return false;                                                      \
                                                                               \
        if (CMC_(PFX, _empty)(_set1_))                                         \
        {                                                                      \
            /* The empty set is a proper subset of all non-empty sets */       \
            if (!CMC_(PFX, _empty)(_set2_))                                    \
                return true;                                                   \
            /* The empty set is not a proper subset of itself (this is true */ \
            /* for any set) */                                                 \
            else                                                               \
                return false;                                                  \
        }                                                                      \
                                                                               \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_);      \
                                                                               \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            V value = CMC_(PFX, _iter_value)(&iter);                           \
                                                                               \
            if (CMC_(PFX, _impl_get_entry)(_set2_, value) == NULL)             \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Is _set1_ a proper superset of _set2_ ? */                              \
    /* A set X is a proper superset of a set Y when: X > Y */                  \
    /* If X is a proper superset of Y, then Y is a proper subset of X */       \
    bool CMC_(PFX, _is_proper_superset)(struct SNAME * _set1_,                 \
                                        struct SNAME * _set2_)                 \
    {                                                                          \
        return CMC_(PFX, _is_proper_subset)(_set2_, _set1_);                   \
    }                                                                          \
                                                                               \
    /* Is _set1_ a disjointset of _set2_ ? */                                  \
    /* A set X is a disjointset of a set Y if their intersection is empty, */  \
    /* that is, if there are no elements in common between the two */          \
    bool CMC_(PFX, _is_disjointset)(struct SNAME * _set1_,                     \
                                    struct SNAME * _set2_)                     \
    {                                                                          \
        _set1_->flag = CMC_FLAG_OK;                                            \
        _set2_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        /* The intersection of an empty set with any other set will result */  \
        /* in an empty set */                                                  \
        if (CMC_(PFX, _empty)(_set1_))                                         \
            return true;                                                       \
                                                                               \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_);      \
                                                                               \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            V value = CMC_(PFX, _iter_value)(&iter);                           \
                                                                               \
            if (CMC_(PFX, _impl_get_entry)(_set2_, value) != NULL)             \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    static struct CMC_DEF_ENTRY(SNAME) *                                       \
        CMC_(PFX, _impl_get_entry)(struct SNAME * _set_, V value)              \
    {                                                                          \
        size_t hash = _set_->f_val->hash(value);                               \
        size_t pos = hash % _set_->capacity;                                   \
                                                                               \
        struct CMC_DEF_ENTRY(SNAME) *target = &(_set_->buffer[pos]);           \
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
    static size_t CMC_(PFX, _impl_calculate_size)(size_t required)             \
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

#endif /* CMC_CMC_HASHSET_H */
