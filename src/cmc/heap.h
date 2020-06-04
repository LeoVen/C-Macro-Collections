/**
 * cmc/heap.h
 *
 * Creation Date: 25/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Heap
 *
 * The Heap is a specialized tree-based data structure which is essentially a
 * complete tree that satisfies the heap property:
 *     - MaxHeap:
 *         - The root node is the greatest element of the unique set of keys.
 *         - A node key is always greater than or equal to its children keys.
 *     - Minheap:
 *         - The root node is the smallest element of the unique set of keys.
 *         - A node Key is always greater than or equal to its children keys.
 *
 * The heap is mostly used to implement priority queues. Sometimes it can also
 * be used to sort elements. There are three main functions:
 *     - insert : Adds an element to the heap
 *     - remove_(min/max): Removes the min/max element from the heap
 *     - min/max : Accesses the min/max element from the heap
 */

#ifndef CMC_CMC_HEAP_H
#define CMC_CMC_HEAP_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * Heap Implementation
 * ------------------------------------------------------------------------- */
#include "../cor/heap.h"

/**
 * Core Heap implementation
 */
#define CMC_CMC_HEAP_CORE(PARAMS) \
    CMC_CMC_HEAP_CORE_HEADER(PARAMS) \
    CMC_CMC_HEAP_CORE_SOURCE(PARAMS)

#define CMC_CMC_HEAP_CORE_HEADER(PARAMS) \
    CMC_CMC_HEAP_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_HEAP_CORE_SOURCE(PARAMS) \
    CMC_CMC_HEAP_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HEAP_CORE_HEADER_(PFX, SNAME, V) \
\
    /* Heap Structure */ \
    struct SNAME \
    { \
        /* Dynamic array of elements */ \
        V *buffer; \
\
        /* Current array capacity */ \
        size_t capacity; \
\
        /* Current amount of elements in the heap */ \
        size_t count; \
\
        /* Heap order (MaxHeap or MinHeap) */ \
        enum cmc_heap_order HO; \
\
        /* Flags indicating errors or success */ \
        int flag; \
\
        /* Value function table */ \
        struct CMC_DEF_FVAL(SNAME) * f_val; \
\
        /* Custom allocation functions */ \
        struct CMC_ALLOC_NODE_NAME *alloc; \
\
        /* Custom callback functions */ \
        CMC_CALLBACKS_DECL; \
    }; \
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
    struct SNAME *CMC_(PFX, _new)(size_t capacity, enum cmc_heap_order HO, struct CMC_DEF_FVAL(SNAME) * f_val); \
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, enum cmc_heap_order HO, struct CMC_DEF_FVAL(SNAME) * f_val, \
                                         struct CMC_ALLOC_NODE_NAME * alloc, struct CMC_CALLBACKS_NAME * callbacks); \
    void CMC_(PFX, _clear)(struct SNAME * _heap_); \
    void CMC_(PFX, _free)(struct SNAME * _heap_); \
    /* Customization of Allocation and Callbacks */ \
    void CMC_(PFX, _customize)(struct SNAME * _heap_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks); \
    /* Collection Input and Output */ \
    bool CMC_(PFX, _insert)(struct SNAME * _heap_, V value); \
    bool CMC_(PFX, _remove)(struct SNAME * _heap_); \
    /* Element Access */ \
    V CMC_(PFX, _peek)(struct SNAME * _heap_); \
    /* Collection State */ \
    bool CMC_(PFX, _contains)(struct SNAME * _heap_, V value); \
    bool CMC_(PFX, _empty)(struct SNAME * _heap_); \
    bool CMC_(PFX, _full)(struct SNAME * _heap_); \
    size_t CMC_(PFX, _count)(struct SNAME * _heap_); \
    size_t CMC_(PFX, _capacity)(struct SNAME * _heap_); \
    int CMC_(PFX, _flag)(struct SNAME * _heap_); \
    /* Collection Utility */ \
    bool CMC_(PFX, _resize)(struct SNAME * _heap_, size_t capacity); \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _heap_); \
    bool CMC_(PFX, _equals)(struct SNAME * _heap1_, struct SNAME * _heap2_);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_HEAP_CORE_SOURCE_(PFX, SNAME, V) \
\
    /* Implementation Detail Functions */ \
    static void CMC_(PFX, _impl_float_up)(struct SNAME * _heap_, size_t index); \
    static void CMC_(PFX, _impl_float_down)(struct SNAME * _heap_, size_t index); \
\
    struct SNAME *CMC_(PFX, _new)(size_t capacity, enum cmc_heap_order HO, struct CMC_DEF_FVAL(SNAME) * f_val) \
    { \
        return CMC_(PFX, _new_custom)(capacity, HO, f_val, NULL, NULL); \
    } \
\
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, enum cmc_heap_order HO, struct CMC_DEF_FVAL(SNAME) * f_val, \
                                         struct CMC_ALLOC_NODE_NAME * alloc, struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        if (capacity < 1) \
            return NULL; \
\
        if (HO != CMC_MIN_HEAP && HO != CMC_MAX_HEAP) \
            return NULL; \
\
        if (!f_val) \
            return NULL; \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        struct SNAME *_heap_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_heap_) \
            return NULL; \
\
        _heap_->buffer = alloc->calloc(capacity, sizeof(V)); \
\
        if (!_heap_->buffer) \
        { \
            alloc->free(_heap_); \
            return NULL; \
        } \
\
        _heap_->capacity = capacity; \
        _heap_->count = 0; \
        _heap_->HO = HO; \
        _heap_->flag = CMC_FLAG_OK; \
        _heap_->f_val = f_val; \
        _heap_->alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(_heap_, callbacks); \
\
        return _heap_; \
    } \
\
    void CMC_(PFX, _clear)(struct SNAME * _heap_) \
    { \
        if (_heap_->f_val->free) \
        { \
            for (size_t i = 0; i < _heap_->count; i++) \
            { \
                _heap_->f_val->free(_heap_->buffer[i]); \
            } \
        } \
\
        memset(_heap_->buffer, 0, sizeof(V) * _heap_->capacity); \
\
        _heap_->count = 0; \
        _heap_->flag = CMC_FLAG_OK; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _heap_) \
    { \
        if (_heap_->f_val->free) \
        { \
            for (size_t i = 0; i < _heap_->count; i++) \
            { \
                _heap_->f_val->free(_heap_->buffer[i]); \
            } \
        } \
\
        _heap_->alloc->free(_heap_->buffer); \
        _heap_->alloc->free(_heap_); \
    } \
\
    void CMC_(PFX, _customize)(struct SNAME * _heap_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        if (!alloc) \
            _heap_->alloc = &cmc_alloc_node_default; \
        else \
            _heap_->alloc = alloc; \
\
        CMC_CALLBACKS_ASSIGN(_heap_, callbacks); \
\
        _heap_->flag = CMC_FLAG_OK; \
    } \
\
    bool CMC_(PFX, _insert)(struct SNAME * _heap_, V value) \
    { \
        if (CMC_(PFX, _full)(_heap_)) \
        { \
            if (!CMC_(PFX, _resize)(_heap_, _heap_->count * 2)) \
                return false; \
        } \
\
        _heap_->buffer[_heap_->count++] = value; \
\
        if (_heap_->count > 1) \
        { \
            CMC_(PFX, _impl_float_up)(_heap_, _heap_->count - 1); \
        } \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        if (_heap_->callbacks && _heap_->callbacks->create) \
            _heap_->callbacks->create(); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _remove)(struct SNAME * _heap_) \
    { \
        if (CMC_(PFX, _empty)(_heap_)) \
        { \
            _heap_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        _heap_->buffer[0] = _heap_->buffer[_heap_->count - 1]; \
        _heap_->buffer[_heap_->count - 1] = (V){ 0 }; \
\
        _heap_->count--; \
\
        CMC_(PFX, _impl_float_down)(_heap_, 0); \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        if (_heap_->callbacks && _heap_->callbacks->delete) \
            _heap_->callbacks->delete (); \
\
        return true; \
    } \
\
    V CMC_(PFX, _peek)(struct SNAME * _heap_) \
    { \
        if (CMC_(PFX, _empty)(_heap_)) \
        { \
            _heap_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        if (_heap_->callbacks && _heap_->callbacks->read) \
            _heap_->callbacks->read(); \
\
        return _heap_->buffer[0]; \
    } \
\
    bool CMC_(PFX, _contains)(struct SNAME * _heap_, V value) \
    { \
        _heap_->flag = CMC_FLAG_OK; \
\
        bool result = false; \
\
        for (size_t i = 0; i < _heap_->count; i++) \
        { \
            if (_heap_->f_val->cmp(_heap_->buffer[i], value) == 0) \
            { \
                result = true; \
                break; \
            } \
        } \
\
        if (_heap_->callbacks && _heap_->callbacks->read) \
            _heap_->callbacks->read(); \
\
        return result; \
    } \
\
    bool CMC_(PFX, _empty)(struct SNAME * _heap_) \
    { \
        return _heap_->count == 0; \
    } \
\
    bool CMC_(PFX, _full)(struct SNAME * _heap_) \
    { \
        return _heap_->count >= _heap_->capacity; \
    } \
\
    size_t CMC_(PFX, _count)(struct SNAME * _heap_) \
    { \
        return _heap_->count; \
    } \
\
    size_t CMC_(PFX, _capacity)(struct SNAME * _heap_) \
    { \
        return _heap_->capacity; \
    } \
\
    int CMC_(PFX, _flag)(struct SNAME * _heap_) \
    { \
        return _heap_->flag; \
    } \
\
    bool CMC_(PFX, _resize)(struct SNAME * _heap_, size_t capacity) \
    { \
        _heap_->flag = CMC_FLAG_OK; \
\
        if (_heap_->capacity == capacity) \
            goto success; \
\
        if (capacity < _heap_->count) \
        { \
            _heap_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        V *new_buffer = _heap_->alloc->realloc(_heap_->buffer, sizeof(V) * capacity); \
\
        if (!new_buffer) \
        { \
            _heap_->flag = CMC_FLAG_ALLOC; \
            return false; \
        } \
\
        /* TODO zero out new slots */ \
\
        _heap_->buffer = new_buffer; \
        _heap_->capacity = capacity; \
\
    success: \
\
        if (_heap_->callbacks && _heap_->callbacks->resize) \
            _heap_->callbacks->resize(); \
\
        return true; \
    } \
\
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _heap_) \
    { \
        struct SNAME *result = \
            CMC_(PFX, _new_custom)(_heap_->capacity, _heap_->HO, _heap_->f_val, _heap_->alloc, NULL); \
\
        if (!result) \
        { \
            _heap_->flag = CMC_FLAG_ERROR; \
            return NULL; \
        } \
\
        CMC_CALLBACKS_ASSIGN(result, _heap_->callbacks); \
\
        if (_heap_->f_val->cpy) \
        { \
            for (size_t i = 0; i < _heap_->count; i++) \
                result->buffer[i] = _heap_->f_val->cpy(_heap_->buffer[i]); \
        } \
        else \
            memcpy(result->buffer, _heap_->buffer, sizeof(V) * _heap_->count); \
\
        result->count = _heap_->count; \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        return result; \
    } \
\
    bool CMC_(PFX, _equals)(struct SNAME * _heap1_, struct SNAME * _heap2_) \
    { \
        _heap1_->flag = CMC_FLAG_OK; \
        _heap2_->flag = CMC_FLAG_OK; \
\
        if (_heap1_->count != _heap2_->count) \
            return false; \
\
        for (size_t i = 0; i < _heap1_->count; i++) \
        { \
            if (_heap1_->f_val->cmp(_heap1_->buffer[i], _heap2_->buffer[i]) != 0) \
                return false; \
        } \
\
        return true; \
    } \
\
    static void CMC_(PFX, _impl_float_up)(struct SNAME * _heap_, size_t index) \
    { \
        /* Current index */ \
        size_t C = index; \
        V child = _heap_->buffer[C]; \
        V parent = _heap_->buffer[(index - 1) / 2]; \
\
        int mod = _heap_->HO; \
\
        while (C > 0 && _heap_->f_val->cmp(child, parent) * mod > 0) \
        { \
            /* Swap between C (current element) and its parent */ \
            V tmp = _heap_->buffer[C]; \
            _heap_->buffer[C] = _heap_->buffer[(C - 1) / 2]; \
            _heap_->buffer[(C - 1) / 2] = tmp; \
\
            /* Go to parent */ \
            C = (C - 1) / 2; \
\
            /* Prevent the two lines below from accessing a negative index */ \
            if (C == 0) \
                break; \
\
            child = _heap_->buffer[C]; \
            parent = _heap_->buffer[(C - 1) / 2]; \
        } \
    } \
\
    static void CMC_(PFX, _impl_float_down)(struct SNAME * _heap_, size_t index) \
    { \
        int mod = _heap_->HO; \
\
        while (index < _heap_->count) \
        { \
            size_t L = 2 * index + 1; \
            size_t R = 2 * index + 2; \
            size_t C = index; \
\
            /* Determine if we swap with the left or right element */ \
            if (L < _heap_->count && _heap_->f_val->cmp(_heap_->buffer[L], _heap_->buffer[C]) * mod > 0) \
            { \
                C = L; \
            } \
\
            if (R < _heap_->count && _heap_->f_val->cmp(_heap_->buffer[R], _heap_->buffer[C]) * mod > 0) \
            { \
                C = R; \
            } \
\
            /* Swap only if either left or right was picked, otherwise done */ \
            if (C != index) \
            { \
                V tmp = _heap_->buffer[index]; \
                _heap_->buffer[index] = _heap_->buffer[C]; \
                _heap_->buffer[C] = tmp; \
\
                index = C; \
            } \
            else \
                break; \
        } \
    }

#endif /* CMC_CMC_HEAP_H */
