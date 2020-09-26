/**
 * cmc_deque.h
 *
 * Creation Date: 20/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Deque
 *
 * A Deque (double-ended queue) is a linear data structure that is able to add
 * or remove elements from both ends. It can also be thought of a double-ended
 * stack since you can push and pop elements from two ends. The Deque can also
 * be used as a Queue.
 *
 * There is no random access for a Deque. The only elements accessible are the
 * front element and the back.
 *
 * Implementation
 *
 * This implementation uses a circular buffer (ring buffer or cyclic buffer) in
 * order to operate on O(1) for push and pop on either ends (only case where it
 * takes longer than O(1) is when the buffer is reallocated). If it was
 * implemented as a regular array, adding or removing elements from the front
 * would take O(N) due to the need to shift all elements in the deque.
 */

#ifndef CMC_CMC_DEQUE_H
#define CMC_CMC_DEQUE_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "cor_core.h"

/**
 * Core Deque implementation
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_CMC_DEQUE_CORE(ACCESS, FILE, PARAMS) CMC_(CMC_(CMC_CMC_DEQUE_CORE_, ACCESS), CMC_(_, FILE))(PARAMS)

/* PRIVATE or PUBLIC solver */
#define CMC_CMC_DEQUE_CORE_PUBLIC_HEADER(PARAMS) \
    CMC_CMC_DEQUE_CORE_STRUCT(PARAMS) \
    CMC_CMC_DEQUE_CORE_HEADER(PARAMS)

#define CMC_CMC_DEQUE_CORE_PUBLIC_SOURCE(PARAMS) CMC_CMC_DEQUE_CORE_SOURCE(PARAMS)

#define CMC_CMC_DEQUE_CORE_PRIVATE_HEADER(PARAMS) \
    struct CMC_PARAM_SNAME(PARAMS); \
    CMC_CMC_DEQUE_CORE_HEADER(PARAMS)

#define CMC_CMC_DEQUE_CORE_PRIVATE_SOURCE(PARAMS) \
    CMC_CMC_DEQUE_CORE_STRUCT(PARAMS) \
    CMC_CMC_DEQUE_CORE_SOURCE(PARAMS)

/* Lowest level API */
#define CMC_CMC_DEQUE_CORE_STRUCT(PARAMS) \
    CMC_CMC_DEQUE_CORE_STRUCT_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_DEQUE_CORE_HEADER(PARAMS) \
    CMC_CMC_DEQUE_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_DEQUE_CORE_SOURCE(PARAMS) \
    CMC_CMC_DEQUE_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Struct
 * ------------------------------------------------------------------------- */
#define CMC_CMC_DEQUE_CORE_STRUCT_(PFX, SNAME, V) \
\
    /* Deque Structure */ \
    struct SNAME \
    { \
        /* Dynamic circular array of elements */ \
        V *buffer; \
\
        /* Current circular array capacity */ \
        size_t capacity; \
\
        /* Current amount of elements */ \
        size_t count; \
\
        /* Index representing the front of the deque */ \
        size_t front; \
\
        /* Index representing the back of the deque */ \
        size_t back; \
\
        /* Flags indicating errors or success */ \
        int flag; \
\
        struct CMC_DEF_FVAL(SNAME) * f_val; \
\
        /* Custom allocation functions */ \
        struct CMC_ALLOC_NODE_NAME *alloc; \
\
        /* Custom callback functions */ \
        CMC_CALLBACKS_DECL; \
    };

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_DEQUE_CORE_HEADER_(PFX, SNAME, V) \
\
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
    struct SNAME *CMC_(PFX, _new)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val); \
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val, \
                                         struct CMC_ALLOC_NODE_NAME * alloc, struct CMC_CALLBACKS_NAME * callbacks); \
    void CMC_(PFX, _clear)(struct SNAME * _deque_); \
    void CMC_(PFX, _free)(struct SNAME * _deque_); \
    /* Customization of Allocation and Callbacks */ \
    void CMC_(PFX, _customize)(struct SNAME * _deque_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks); \
    /* Collection Input and Output */ \
    bool CMC_(PFX, _push_front)(struct SNAME * _deque_, V value); \
    bool CMC_(PFX, _push_back)(struct SNAME * _deque_, V value); \
    bool CMC_(PFX, _pop_front)(struct SNAME * _deque_); \
    bool CMC_(PFX, _pop_back)(struct SNAME * _deque_); \
    /* Element Access */ \
    V CMC_(PFX, _front)(struct SNAME * _deque_); \
    V CMC_(PFX, _back)(struct SNAME * _deque_); \
    /* Collection State */ \
    bool CMC_(PFX, _contains)(struct SNAME * _deque_, V value); \
    bool CMC_(PFX, _empty)(struct SNAME * _deque_); \
    bool CMC_(PFX, _full)(struct SNAME * _deque_); \
    size_t CMC_(PFX, _count)(struct SNAME * _deque_); \
    size_t CMC_(PFX, _capacity)(struct SNAME * _deque_); \
    int CMC_(PFX, _flag)(struct SNAME * _deque_); \
    /* Collection Utility */ \
    bool CMC_(PFX, _resize)(struct SNAME * _deque_, size_t capacity); \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _deque_); \
    bool CMC_(PFX, _equals)(struct SNAME * _deque1_, struct SNAME * _deque2_);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_DEQUE_CORE_SOURCE_(PFX, SNAME, V) \
\
    /* Implementation Detail Functions */ \
    /* None */ \
\
    struct SNAME *CMC_(PFX, _new)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val) \
    { \
        return CMC_(PFX, _new_custom)(capacity, f_val, NULL, NULL); \
    } \
\
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val, \
                                         struct CMC_ALLOC_NODE_NAME * alloc, struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (capacity < 1) \
            return NULL; \
\
        if (!f_val) \
            return NULL; \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        struct SNAME *_deque_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_deque_) \
            return NULL; \
\
        _deque_->buffer = alloc->calloc(capacity, sizeof(V)); \
\
        if (!_deque_->buffer) \
        { \
            alloc->free(_deque_); \
            return NULL; \
        } \
\
        _deque_->capacity = capacity; \
        _deque_->count = 0; \
        _deque_->front = 0; \
        _deque_->back = 0; \
        _deque_->flag = CMC_FLAG_OK; \
        _deque_->f_val = f_val; \
        _deque_->alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(_deque_, callbacks); \
\
        return _deque_; \
    } \
\
    void CMC_(PFX, _clear)(struct SNAME * _deque_) \
    { \
        if (_deque_->f_val->free) \
        { \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++) \
            { \
                _deque_->f_val->free(_deque_->buffer[i]); \
\
                i = (i + 1) % _deque_->capacity; \
            } \
        } \
\
        memset(_deque_->buffer, 0, sizeof(V) * _deque_->capacity); \
\
        _deque_->count = 0; \
        _deque_->front = 0; \
        _deque_->back = 0; \
\
        _deque_->flag = CMC_FLAG_OK; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _deque_) \
    { \
        if (_deque_->f_val->free) \
        { \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++) \
            { \
                _deque_->f_val->free(_deque_->buffer[i]); \
\
                i = (i + 1) % _deque_->capacity; \
            } \
        } \
\
        _deque_->alloc->free(_deque_->buffer); \
        _deque_->alloc->free(_deque_); \
    } \
\
    void CMC_(PFX, _customize)(struct SNAME * _deque_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!alloc) \
            _deque_->alloc = &cmc_alloc_node_default; \
        else \
            _deque_->alloc = alloc; \
\
        CMC_CALLBACKS_ASSIGN(_deque_, callbacks); \
\
        _deque_->flag = CMC_FLAG_OK; \
    } \
\
    bool CMC_(PFX, _push_front)(struct SNAME * _deque_, V value) \
    { \
        if (CMC_(PFX, _full)(_deque_)) \
        { \
            if (!CMC_(PFX, _resize)(_deque_, _deque_->capacity * 2)) \
                return false; \
        } \
\
        _deque_->front = (_deque_->front == 0) ? _deque_->capacity - 1 : _deque_->front - 1; \
\
        _deque_->buffer[_deque_->front] = value; \
\
        _deque_->count++; \
        _deque_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_deque_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _push_back)(struct SNAME * _deque_, V value) \
    { \
        if (CMC_(PFX, _full)(_deque_)) \
        { \
            if (!CMC_(PFX, _resize)(_deque_, _deque_->capacity * 2)) \
                return false; \
        } \
\
        _deque_->buffer[_deque_->back] = value; \
\
        _deque_->back = (_deque_->back == _deque_->capacity - 1) ? 0 : _deque_->back + 1; \
\
        _deque_->count++; \
        _deque_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_deque_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _pop_front)(struct SNAME * _deque_) \
    { \
        if (CMC_(PFX, _empty)(_deque_)) \
        { \
            _deque_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        _deque_->buffer[_deque_->front] = (V){ 0 }; \
\
        _deque_->front = (_deque_->front == _deque_->capacity - 1) ? 0 : _deque_->front + 1; \
\
        _deque_->count--; \
        _deque_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_deque_, delete); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _pop_back)(struct SNAME * _deque_) \
    { \
        if (CMC_(PFX, _empty)(_deque_)) \
        { \
            _deque_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        _deque_->back = (_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1; \
\
        _deque_->buffer[_deque_->back] = (V){ 0 }; \
\
        _deque_->count--; \
        _deque_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_deque_, delete); \
\
        return true; \
    } \
\
    V CMC_(PFX, _front)(struct SNAME * _deque_) \
    { \
        if (CMC_(PFX, _empty)(_deque_)) \
        { \
            _deque_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        _deque_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_deque_, read); \
\
        return _deque_->buffer[_deque_->front]; \
    } \
\
    V CMC_(PFX, _back)(struct SNAME * _deque_) \
    { \
        if (CMC_(PFX, _empty)(_deque_)) \
        { \
            _deque_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        _deque_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_deque_, read); \
\
        return _deque_->buffer[(_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1]; \
    } \
\
    bool CMC_(PFX, _contains)(struct SNAME * _deque_, V value) \
    { \
        _deque_->flag = CMC_FLAG_OK; \
\
        bool result = false; \
\
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++) \
        { \
            if (_deque_->f_val->cmp(_deque_->buffer[i], value) == 0) \
            { \
                result = true; \
                break; \
            } \
\
            i = (i + 1) % _deque_->capacity; \
        } \
\
        CMC_CALLBACKS_CALL(_deque_, read); \
\
        return result; \
    } \
\
    bool CMC_(PFX, _empty)(struct SNAME * _deque_) \
    { \
        return _deque_->count == 0; \
    } \
\
    bool CMC_(PFX, _full)(struct SNAME * _deque_) \
    { \
        return _deque_->count >= _deque_->capacity; \
    } \
\
    size_t CMC_(PFX, _count)(struct SNAME * _deque_) \
    { \
        return _deque_->count; \
    } \
\
    size_t CMC_(PFX, _capacity)(struct SNAME * _deque_) \
    { \
        return _deque_->capacity; \
    } \
\
    int CMC_(PFX, _flag)(struct SNAME * _deque_) \
    { \
        return _deque_->flag; \
    } \
\
    bool CMC_(PFX, _resize)(struct SNAME * _deque_, size_t capacity) \
    { \
        _deque_->flag = CMC_FLAG_OK; \
\
        if (_deque_->capacity == capacity) \
            goto success; \
\
        if (capacity < _deque_->count) \
        { \
            _deque_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        V *new_buffer = _deque_->alloc->malloc(sizeof(V) * capacity); \
\
        if (!new_buffer) \
        { \
            _deque_->flag = CMC_FLAG_ALLOC; \
            return false; \
        } \
\
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++) \
        { \
            new_buffer[j] = _deque_->buffer[i]; \
\
            i = (i + 1) % _deque_->capacity; \
        } \
\
        _deque_->alloc->free(_deque_->buffer); \
\
        _deque_->buffer = new_buffer; \
        _deque_->capacity = capacity; \
        _deque_->front = 0; \
        _deque_->back = _deque_->count; \
\
    success: \
\
        CMC_CALLBACKS_CALL(_deque_, resize); \
\
        return true; \
    } \
\
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _deque_) \
    { \
        struct SNAME *result = CMC_(PFX, _new_custom)(_deque_->capacity, _deque_->f_val, _deque_->alloc, NULL); \
\
        if (!result) \
        { \
            _deque_->flag = CMC_FLAG_ERROR; \
            return NULL; \
        } \
\
        CMC_CALLBACKS_ASSIGN(result, _deque_->callbacks); \
\
        if (_deque_->f_val->cpy) \
        { \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++) \
            { \
                result->buffer[j] = _deque_->f_val->cpy(_deque_->buffer[i]); \
\
                i = (i + 1) % _deque_->capacity; \
            } \
        } \
        else \
        { \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++) \
            { \
                result->buffer[j] = _deque_->buffer[i]; \
\
                i = (i + 1) % _deque_->capacity; \
            } \
        } \
\
        result->count = _deque_->count; \
        result->front = 0; \
        result->back = _deque_->count; \
\
        _deque_->flag = CMC_FLAG_OK; \
\
        return result; \
    } \
\
    bool CMC_(PFX, _equals)(struct SNAME * _deque1_, struct SNAME * _deque2_) \
    { \
        _deque1_->flag = CMC_FLAG_OK; \
        _deque2_->flag = CMC_FLAG_OK; \
\
        if (_deque1_->count != _deque2_->count) \
            return false; \
\
        size_t i, j, k; \
        for (i = _deque1_->front, j = _deque2_->front, k = 0; k < _deque1_->count; k++) \
        { \
            if (_deque1_->f_val->cmp(_deque1_->buffer[i], _deque2_->buffer[j]) != 0) \
                return false; \
\
            i = (i + 1) % _deque1_->capacity; \
            j = (j + 1) % _deque2_->capacity; \
        } \
\
        return true; \
    }

#endif /* CMC_CMC_DEQUE_H */
