/**
 * cmc_hashmultiset.h
 *
 * Creation Date: 10/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashMultiSet
 *
 * In mathematics, a multiset is a modification of the concept of a set that,
 * unlike a set, allows for multiple instances for each of its elements. The
 * positive integer number of instances, given for each element is called the
 * multiplicity of this element in the multiset. A multiSet also has a
 * cardinality which equals the sum of the multiplicities of its elements.
 *
 * Implementation
 *
 * In this implementation, the values are stored in a flat hashtable and are
 * mapped to their multiplicity.
 */

#ifndef CMC_CMC_HASHMULTISET_H
#define CMC_CMC_HASHMULTISET_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "cor_core.h"

/* -------------------------------------------------------------------------
 * Hashtable Implementation
 * ------------------------------------------------------------------------- */
#include "cor_hashtable.h"

/**
 * Core HashMultiSet implementation
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_CMC_HASHMULTISET_CORE(ACCESS, FILE, PARAMS) \
    CMC_(CMC_(CMC_CMC_HASHMULTISET_CORE_, ACCESS), CMC_(_, FILE))(PARAMS)

/* PRIVATE or PUBLIC solver */
#define CMC_CMC_HASHMULTISET_CORE_PUBLIC_HEADER(PARAMS) \
    CMC_CMC_HASHMULTISET_CORE_STRUCT(PARAMS) \
    CMC_CMC_HASHMULTISET_CORE_HEADER(PARAMS)

#define CMC_CMC_HASHMULTISET_CORE_PUBLIC_SOURCE(PARAMS) CMC_CMC_HASHMULTISET_CORE_SOURCE(PARAMS)

#define CMC_CMC_HASHMULTISET_CORE_PRIVATE_HEADER(PARAMS) \
    struct CMC_PARAM_SNAME(PARAMS); \
    struct CMC_DEF_ENTRY(CMC_PARAM_SNAME(PARAMS)); \
    CMC_CMC_HASHMULTISET_CORE_HEADER(PARAMS)

#define CMC_CMC_HASHMULTISET_CORE_PRIVATE_SOURCE(PARAMS) \
    CMC_CMC_HASHMULTISET_CORE_STRUCT(PARAMS) \
    CMC_CMC_HASHMULTISET_CORE_SOURCE(PARAMS)

/* Lowest level API */
#define CMC_CMC_HASHMULTISET_CORE_STRUCT(PARAMS) \
    CMC_CMC_HASHMULTISET_CORE_STRUCT_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_HASHMULTISET_CORE_HEADER(PARAMS) \
    CMC_CMC_HASHMULTISET_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_HASHMULTISET_CORE_SOURCE(PARAMS) \
    CMC_CMC_HASHMULTISET_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Struct
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHMULTISET_CORE_STRUCT_(PFX, SNAME, V) \
\
    /* HashMultiset Structure */ \
    struct SNAME \
    { \
        /* Array of Entries */ \
        struct CMC_DEF_ENTRY(SNAME) * buffer; \
\
        /* Current Array Capcity */ \
        size_t capacity; \
\
        /* Current amount of unique elements */ \
        size_t count; \
\
        /* Total amount of elements taking into account their multiplicity */ \
        size_t cardinality; \
\
        /* Load factor in range (0.0, 1.0) */ \
        double load; \
\
        /* Flags indicating errors or success */ \
        int flag; \
\
        /* Key function table */ \
        struct CMC_DEF_FVAL(SNAME) * f_val; \
\
        /* Custom allocation functions */ \
        struct CMC_ALLOC_NODE_NAME *alloc; \
\
        /* Custom callback functions */ \
        CMC_CALLBACKS_DECL; \
    }; \
\
    struct CMC_DEF_ENTRY(SNAME) \
    { \
        /* Entry element */ \
        V value; \
\
        /* The element's multiplicity */ \
        size_t multiplicity; \
\
        /* The distance of this node to its original position, used by */ \
        /* robin-hood hashing */ \
        size_t dist; \
\
        /* The sate of this node (DELETED, EMPTY, FILLED) */ \
        enum cmc_entry_state state; \
    };

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHMULTISET_CORE_HEADER_(PFX, SNAME, V) \
\
    /* Value struct function table */ \
    struct CMC_DEF_FVAL(SNAME) \
    { \
        /* Comparator function */ \
        CMC_DEF_FTAB_CMP(V); \
\
        /* Copy function */ \
        CMC_DEF_FTAB_CPY(V); \
\
        /* To string function */ \
        CMC_DEF_FTAB_STR(V); \
\
        /* Free from memory function */ \
        CMC_DEF_FTAB_FREE(V); \
\
        /* Hash function */ \
        CMC_DEF_FTAB_HASH(V); \
\
        /* Priority function */ \
        CMC_DEF_FTAB_PRI(V); \
    }; \
\
    /* Collection Functions */ \
    /* Collection Allocation and Deallocation */ \
    struct SNAME *CMC_(PFX, _new)(size_t capacity, double load, struct CMC_DEF_FVAL(SNAME) * f_val); \
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, double load, struct CMC_DEF_FVAL(SNAME) * f_val, \
                                         struct CMC_ALLOC_NODE_NAME * alloc, struct CMC_CALLBACKS_NAME * callbacks); \
    void CMC_(PFX, _clear)(struct SNAME * _set_); \
    void CMC_(PFX, _free)(struct SNAME * _set_); \
    /* Customization of Allocation and Callbacks */ \
    void CMC_(PFX, _customize)(struct SNAME * _set_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks); \
    /* Collection Input and Output */ \
    bool CMC_(PFX, _insert)(struct SNAME * _set_, V value); \
    bool CMC_(PFX, _insert_many)(struct SNAME * _set_, V value, size_t count); \
    bool CMC_(PFX, _update)(struct SNAME * _set_, V value, size_t multiplicity); \
    bool CMC_(PFX, _remove)(struct SNAME * _set_, V value); \
    size_t CMC_(PFX, _remove_all)(struct SNAME * _set_, V value); \
    /* Element Access */ \
    bool CMC_(PFX, _max)(struct SNAME * _set_, V * value); \
    bool CMC_(PFX, _min)(struct SNAME * _set_, V * value); \
    size_t CMC_(PFX, _multiplicity_of)(struct SNAME * _set_, V value); \
    /* Collection State */ \
    bool CMC_(PFX, _contains)(struct SNAME * _set_, V value); \
    bool CMC_(PFX, _empty)(struct SNAME * _set_); \
    bool CMC_(PFX, _full)(struct SNAME * _set_); \
    size_t CMC_(PFX, _count)(struct SNAME * _set_); \
    size_t CMC_(PFX, _cardinality)(struct SNAME * _set_); \
    size_t CMC_(PFX, _capacity)(struct SNAME * _set_); \
    double CMC_(PFX, _load)(struct SNAME * _set_); \
    int CMC_(PFX, _flag)(struct SNAME * _set_); \
    /* Collection Utility */ \
    bool CMC_(PFX, _resize)(struct SNAME * _set_, size_t capacity); \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _set_); \
    bool CMC_(PFX, _equals)(struct SNAME * _set1_, struct SNAME * _set2_);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HASHMULTISET_CORE_SOURCE_(PFX, SNAME, V) \
\
    /* Implementation Detail Functions */ \
    static size_t CMC_(PFX, _impl_multiplicity_of)(struct SNAME * _set_, V value); \
    static struct CMC_DEF_ENTRY(SNAME) * \
        CMC_(PFX, _impl_insert_and_return)(struct SNAME * _set_, V value, bool *new_node); \
    static struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_get_entry)(struct SNAME * _set_, V value); \
    static size_t CMC_(PFX, _impl_calculate_size)(size_t required); \
\
    struct SNAME *CMC_(PFX, _new)(size_t capacity, double load, struct CMC_DEF_FVAL(SNAME) * f_val) \
    { \
        return CMC_(PFX, _new_custom)(capacity, load, f_val, NULL, NULL); \
    } \
\
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, double load, struct CMC_DEF_FVAL(SNAME) * f_val, \
                                         struct CMC_ALLOC_NODE_NAME * alloc, struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (capacity == 0 || load <= 0 || load >= 1) \
            return NULL; \
\
        /* Prevent integer overflow */ \
        if (capacity >= UINTMAX_MAX * load) \
            return NULL; \
\
        if (!f_val) \
            return NULL; \
\
        size_t real_capacity = CMC_(PFX, _impl_calculate_size)(capacity / load); \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        struct SNAME *_set_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_set_) \
            return NULL; \
\
        _set_->buffer = alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME))); \
\
        if (!_set_->buffer) \
        { \
            alloc->free(_set_); \
            return NULL; \
        } \
\
        _set_->count = 0; \
        _set_->cardinality = 0; \
        _set_->capacity = real_capacity; \
        _set_->load = load; \
        _set_->flag = CMC_FLAG_OK; \
        _set_->f_val = f_val; \
        _set_->alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(_set_, callbacks); \
\
        return _set_; \
    } \
\
    void CMC_(PFX, _clear)(struct SNAME * _set_) \
    { \
        if (_set_->f_val->free) \
        { \
            for (size_t i = 0; i < _set_->capacity; i++) \
            { \
                struct CMC_DEF_ENTRY(SNAME) *entry = &(_set_->buffer[i]); \
\
                if (entry->state == CMC_ES_FILLED) \
                { \
                    _set_->f_val->free(entry->value); \
                } \
            } \
        } \
\
        memset(_set_->buffer, 0, sizeof(struct CMC_DEF_ENTRY(SNAME)) * _set_->capacity); \
\
        _set_->count = 0; \
        _set_->flag = CMC_FLAG_OK; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _set_) \
    { \
        if (_set_->f_val->free) \
        { \
            for (size_t i = 0; i < _set_->capacity; i++) \
            { \
                struct CMC_DEF_ENTRY(SNAME) *entry = &(_set_->buffer[i]); \
\
                if (entry->state == CMC_ES_FILLED) \
                { \
                    _set_->f_val->free(entry->value); \
                } \
            } \
        } \
\
        _set_->alloc->free(_set_->buffer); \
        _set_->alloc->free(_set_); \
    } \
\
    void CMC_(PFX, _customize)(struct SNAME * _set_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!alloc) \
            _set_->alloc = &cmc_alloc_node_default; \
        else \
            _set_->alloc = alloc; \
\
        CMC_CALLBACKS_ASSIGN(_set_, callbacks); \
\
        _set_->flag = CMC_FLAG_OK; \
    } \
\
    bool CMC_(PFX, _insert)(struct SNAME * _set_, V value) \
    { \
        bool new_node; \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_insert_and_return)(_set_, value, &new_node); \
\
        if (!entry) \
        { \
            _set_->flag = CMC_FLAG_ERROR; \
            return false; \
        } \
\
        if (!new_node) \
            entry->multiplicity++; \
\
        _set_->cardinality++; \
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _insert_many)(struct SNAME * _set_, V value, size_t count) \
    { \
        if (count == 0) \
            goto success; \
\
        bool new_node; \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_insert_and_return)(_set_, value, &new_node); \
\
        if (!entry) \
        { \
            _set_->flag = CMC_FLAG_ERROR; \
            return false; \
        } \
\
        if (new_node) \
            entry->multiplicity = count; \
        else \
            entry->multiplicity += count; \
\
        _set_->cardinality += count; \
\
    success: \
\
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _update)(struct SNAME * _set_, V value, size_t multiplicity) \
    { \
        if (multiplicity == 0) \
        { \
            /* Effectively delete the entry */ \
            struct CMC_DEF_ENTRY(SNAME) *result = CMC_(PFX, _impl_get_entry)(_set_, value); \
\
            if (!result) \
                /* If no entry was found then its multiplicity is already 0 */ \
                goto success; \
\
            _set_->count--; \
            _set_->cardinality -= result->multiplicity; \
\
            result->value = (V){ 0 }; \
            result->multiplicity = 0; \
            result->dist = 0; \
            result->state = CMC_ES_DELETED; \
\
            goto success; \
        } \
\
        bool new_node; \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_insert_and_return)(_set_, value, &new_node); \
\
        if (!entry) \
            return false; \
\
        if (new_node) \
            _set_->cardinality++; \
\
        _set_->cardinality = (_set_->cardinality - entry->multiplicity) + multiplicity; \
\
        entry->multiplicity = multiplicity; \
\
    success: \
\
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, update); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _remove)(struct SNAME * _set_, V value) \
    { \
        if (CMC_(PFX, _empty)(_set_)) \
        { \
            _set_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        struct CMC_DEF_ENTRY(SNAME) *result = CMC_(PFX, _impl_get_entry)(_set_, value); \
\
        if (!result) \
        { \
            _set_->flag = CMC_FLAG_NOT_FOUND; \
            return false; \
        } \
\
        if (result->multiplicity > 1) \
            result->multiplicity--; \
        else \
        { \
            result->value = (V){ 0 }; \
            result->multiplicity = 0; \
            result->dist = 0; \
            result->state = CMC_ES_DELETED; \
\
            _set_->count--; \
        } \
\
        _set_->cardinality--; \
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, delete); \
\
        return true; \
    } \
\
    size_t CMC_(PFX, _remove_all)(struct SNAME * _set_, V value) \
    { \
        if (CMC_(PFX, _empty)(_set_)) \
        { \
            _set_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        struct CMC_DEF_ENTRY(SNAME) *result = CMC_(PFX, _impl_get_entry)(_set_, value); \
\
        if (!result) \
        { \
            _set_->flag = CMC_FLAG_NOT_FOUND; \
            return 0; \
        } \
\
        size_t removed = result->multiplicity; \
\
        result->value = (V){ 0 }; \
        result->multiplicity = 0; \
        result->dist = 0; \
        result->state = CMC_ES_DELETED; \
\
        _set_->count--; \
        _set_->cardinality -= removed; \
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, delete); \
\
        return removed; \
    } \
\
    bool CMC_(PFX, _max)(struct SNAME * _set_, V * value) \
    { \
        if (CMC_(PFX, _empty)(_set_)) \
        { \
            _set_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        bool first = true; \
        V max_val = (V){ 0 }; \
\
        for (size_t i = 0; i < _set_->capacity; i++) \
        { \
            if (_set_->buffer[i].state == CMC_ES_FILLED) \
            { \
                if (first) \
                { \
                    max_val = _set_->buffer[i].value; \
                    first = false; \
                } \
                else if (_set_->f_val->cmp(_set_->buffer[i].value, max_val) > 0) \
                { \
                    max_val = _set_->buffer[i].value; \
                } \
            } \
        } \
\
        if (value) \
            *value = max_val; \
\
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, read); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _min)(struct SNAME * _set_, V * value) \
    { \
        if (CMC_(PFX, _empty)(_set_)) \
        { \
            _set_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        bool first = true; \
        V min_val = (V){ 0 }; \
\
        for (size_t i = 0; i < _set_->capacity; i++) \
        { \
            if (_set_->buffer[i].state == CMC_ES_FILLED) \
            { \
                if (first) \
                { \
                    min_val = _set_->buffer[i].value; \
                    first = false; \
                } \
                else if (_set_->f_val->cmp(_set_->buffer[i].value, min_val) < 0) \
                { \
                    min_val = _set_->buffer[i].value; \
                } \
            } \
        } \
\
        if (value) \
            *value = min_val; \
\
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, read); \
\
        return true; \
    } \
\
    size_t CMC_(PFX, _multiplicity_of)(struct SNAME * _set_, V value) \
    { \
        struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_get_entry)(_set_, value); \
\
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, read); \
\
        if (!entry) \
            return 0; \
\
        return entry->multiplicity; \
    } \
\
    bool CMC_(PFX, _contains)(struct SNAME * _set_, V value) \
    { \
        _set_->flag = CMC_FLAG_OK; \
\
        bool result = CMC_(PFX, _impl_get_entry)(_set_, value) != NULL; \
\
        CMC_CALLBACKS_CALL(_set_, read); \
\
        return result; \
    } \
\
    bool CMC_(PFX, _empty)(struct SNAME * _set_) \
    { \
        return _set_->count == 0; \
    } \
\
    bool CMC_(PFX, _full)(struct SNAME * _set_) \
    { \
        return (double)_set_->capacity * _set_->load <= (double)_set_->count; \
    } \
\
    size_t CMC_(PFX, _count)(struct SNAME * _set_) \
    { \
        return _set_->count; \
    } \
\
    size_t CMC_(PFX, _cardinality)(struct SNAME * _set_) \
    { \
        return _set_->cardinality; \
    } \
\
    size_t CMC_(PFX, _capacity)(struct SNAME * _set_) \
    { \
        return _set_->capacity; \
    } \
\
    double CMC_(PFX, _load)(struct SNAME * _set_) \
    { \
        return _set_->load; \
    } \
\
    int CMC_(PFX, _flag)(struct SNAME * _set_) \
    { \
        return _set_->flag; \
    } \
\
    bool CMC_(PFX, _resize)(struct SNAME * _set_, size_t capacity) \
    { \
        _set_->flag = CMC_FLAG_OK; \
\
        if (_set_->capacity == capacity) \
            goto success; \
\
        if (_set_->capacity > capacity / _set_->load) \
            goto success; \
\
        /* Prevent integer overflow */ \
        if (capacity >= UINTMAX_MAX * _set_->load) \
        { \
            _set_->flag = CMC_FLAG_ERROR; \
            return false; \
        } \
\
        /* Calculate required capacity based on the prime numbers */ \
        size_t theoretical_size = CMC_(PFX, _impl_calculate_size)(capacity); \
\
        /* Not possible to shrink with current available prime numbers */ \
        if (theoretical_size < _set_->count / _set_->load) \
        { \
            _set_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        /* No callbacks since _new_set_ is just a temporary hashtable */ \
        struct SNAME *_new_set_ = CMC_(PFX, _new_custom)(capacity, _set_->load, _set_->f_val, _set_->alloc, NULL); \
\
        if (!_new_set_) \
        { \
            _set_->flag = CMC_FLAG_ERROR; \
            return false; \
        } \
\
        for (size_t i = 0; i < _set_->capacity; i++) \
        { \
            if (_set_->buffer[i].state == CMC_ES_FILLED) \
            { \
                V value = _set_->buffer[i].value; \
                size_t multiplicity = _set_->buffer[i].multiplicity; \
\
                /* TODO check this for possible errors where entry == NULL */ \
                struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_insert_and_return)(_new_set_, value, NULL); \
\
                entry->multiplicity = multiplicity; \
                /* Setting cardinality not required, _new_set_ is temporary */ \
            } \
        } \
\
        if (_set_->count != _new_set_->count) \
        { \
            CMC_(PFX, _free)(_new_set_); \
\
            _set_->flag = CMC_FLAG_ERROR; \
            return false; \
        } \
\
        struct CMC_DEF_ENTRY(SNAME) *tmp_b = _set_->buffer; \
        _set_->buffer = _new_set_->buffer; \
        _new_set_->buffer = tmp_b; \
\
        size_t tmp_c = _set_->capacity; \
        _set_->capacity = _new_set_->capacity; \
        _new_set_->capacity = tmp_c; \
\
        /* Prevent the set from freeing the data */ \
        _new_set_->f_val = &(struct CMC_DEF_FVAL(SNAME)){ NULL }; \
\
        CMC_(PFX, _free)(_new_set_); \
\
    success: \
\
        CMC_CALLBACKS_CALL(_set_, resize); \
\
        return true; \
    } \
\
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _set_) \
    { \
        struct SNAME *result = \
            CMC_(PFX, _new_custom)(_set_->capacity * _set_->load, _set_->load, _set_->f_val, _set_->alloc, NULL); \
\
        if (!result) \
        { \
            _set_->flag = CMC_FLAG_ERROR; \
            return NULL; \
        } \
\
        CMC_CALLBACKS_ASSIGN(result, _set_->callbacks); \
\
        if (_set_->f_val->cpy) \
        { \
            for (size_t i = 0; i < _set_->capacity; i++) \
            { \
                struct CMC_DEF_ENTRY(SNAME) *scan = &(_set_->buffer[i]); \
\
                if (scan->state != CMC_ES_EMPTY) \
                { \
                    struct CMC_DEF_ENTRY(SNAME) *target = &(result->buffer[i]); \
\
                    if (scan->state == CMC_ES_DELETED) \
                        target->state = CMC_ES_DELETED; \
                    else \
                    { \
                        target->state = scan->state; \
                        target->dist = scan->dist; \
                        target->multiplicity = scan->multiplicity; \
\
                        target->value = _set_->f_val->cpy(scan->value); \
                    } \
                } \
            } \
        } \
        else \
            memcpy(result->buffer, _set_->buffer, sizeof(struct CMC_DEF_ENTRY(SNAME)) * _set_->capacity); \
\
        result->count = _set_->count; \
        result->cardinality = _set_->cardinality; \
\
        _set_->flag = CMC_FLAG_OK; \
\
        return result; \
    } \
\
    bool CMC_(PFX, _equals)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        _set1_->flag = CMC_FLAG_OK; \
        _set2_->flag = CMC_FLAG_OK; \
\
        if (_set1_->count != _set2_->count) \
            return false; \
\
        if (_set1_->cardinality != _set2_->cardinality) \
            return false; \
\
        if (_set1_->count == 0) \
            return true; \
\
        /* Optimize loop using the smallest hashtable */ \
        struct SNAME *_set_a_; \
        struct SNAME *_set_b_; \
\
        _set_a_ = _set1_->capacity < _set2_->capacity ? _set1_ : _set2_; \
        _set_b_ = _set_a_ == _set1_ ? _set2_ : _set1_; \
\
        for (size_t i = 0; i < _set_a_->capacity; i++) \
        { \
            if (_set_a_->buffer[i].state == CMC_ES_FILLED) \
            { \
                struct CMC_DEF_ENTRY(SNAME) *entry_a = &(_set_a_->buffer[i]); \
                struct CMC_DEF_ENTRY(SNAME) *entry_b = CMC_(PFX, _impl_get_entry)(_set_b_, entry_a->value); \
\
                if (!entry_b) \
                    return false; \
\
                if (entry_a->multiplicity != entry_b->multiplicity) \
                    return false; \
            } \
        } \
\
        return true; \
    } \
\
    static size_t CMC_(PFX, _impl_multiplicity_of)(struct SNAME * _set_, V value) \
    { \
        struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_get_entry)(_set_, value); \
\
        if (!entry) \
            return 0; \
\
        return entry->multiplicity; \
    } \
\
    static struct CMC_DEF_ENTRY(SNAME) * \
        CMC_(PFX, _impl_insert_and_return)(struct SNAME * _set_, V value, bool *new_node) \
    { \
        /* If the entry already exists simply return it as we might do */ \
        /* something with it. This function only guarantees that there is */ \
        /* a valid entry for a given value */ \
\
        if (new_node) \
            *new_node = false; \
\
        struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_get_entry)(_set_, value); \
\
        if (entry != NULL) \
            return entry; \
\
        if (new_node) \
            *new_node = true; \
\
        if (CMC_(PFX, _full)(_set_)) \
        { \
            if (!CMC_(PFX, _resize)(_set_, _set_->capacity + 1)) \
                return NULL; \
        } \
\
        size_t hash = _set_->f_val->hash(value); \
        size_t original_pos = hash % _set_->capacity; \
        size_t pos = original_pos; \
        /* Current multiplicity. Might change due to robin hood hashing */ \
        size_t curr_mul = 1; \
\
        struct CMC_DEF_ENTRY(SNAME) *target = &(_set_->buffer[pos]); \
        struct CMC_DEF_ENTRY(SNAME) *to_return = NULL; \
\
        if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED) \
        { \
            target->value = value; \
            target->multiplicity = curr_mul; \
            target->dist = pos - original_pos; \
            target->state = CMC_ES_FILLED; \
\
            to_return = target; \
        } \
        else \
        { \
            while (true) \
            { \
                pos++; \
                target = &(_set_->buffer[pos % _set_->capacity]); \
\
                if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED) \
                { \
                    target->value = value; \
                    target->multiplicity = curr_mul; \
                    target->dist = pos - original_pos; \
                    target->state = CMC_ES_FILLED; \
\
                    if (!to_return) \
                        to_return = target; \
\
                    break; \
                } \
                else if (target->dist < pos - original_pos) \
                { \
                    /* Swap everything */ \
                    V tmp = target->value; \
                    size_t tmp_dist = target->dist; \
                    size_t tmp_mul = target->multiplicity; \
\
                    target->value = value; \
                    target->dist = pos - original_pos; \
                    target->multiplicity = curr_mul; \
\
                    value = tmp; \
                    original_pos = pos - tmp_dist; \
                    curr_mul = tmp_mul; \
\
                    if (!to_return) \
                        to_return = target; \
                } \
            } \
        } \
\
        _set_->count++; \
\
        return to_return; \
    } \
\
    static struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_get_entry)(struct SNAME * _set_, V value) \
    { \
        size_t hash = _set_->f_val->hash(value); \
        size_t pos = hash % _set_->capacity; \
\
        struct CMC_DEF_ENTRY(SNAME) *target = &(_set_->buffer[pos]); \
\
        while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED) \
        { \
            if (_set_->f_val->cmp(target->value, value) == 0) \
                return target; \
\
            pos++; \
            target = &(_set_->buffer[pos % _set_->capacity]); \
        } \
\
        return NULL; \
    } \
\
    static size_t CMC_(PFX, _impl_calculate_size)(size_t required) \
    { \
        const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]); \
\
        if (cmc_hashtable_primes[count - 1] < required) \
            return required; \
\
        size_t i = 0; \
        while (cmc_hashtable_primes[i] < required) \
            i++; \
\
        return cmc_hashtable_primes[i]; \
    }

#endif /* CMC_CMC_HASHMULTISET_H */
