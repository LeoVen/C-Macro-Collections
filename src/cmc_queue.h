/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Libray.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * cmc_queue.h
 *
 * Creation Date: 15/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Queue
 *
 * A Queue is a First-In First-out (or Last-in Last-out) data structure. It is
 * a Dynamic Circular Array where elements are added from one end of the array
 * and removed from the other end. The circular array here (also known as
 * circular buffer or ring buffer) is very important so that both adding and
 * removing elements from the Queue are done instantly. The array is linear but
 * with the modulo operator it is treated as a circular sequence of elements.
 *
 * If the Queue was implemented as a regular Dynamic Array, when adding or
 * removing an element at the front, it would be necessary to shift all elements
 * currently present in the Queue and this would add up a lot of computing time.
 * Shifting `100000` elements in memory by one position every time an element is
 * added to the Queue is simply not efficient.
 *
 * The Queue has two ends. The `front` and `back`. In this implementation all
 * elements are added to the back of the Queue and removed from the front, which
 * is more or less how queues work in real life. Unlike a Stack that only has
 * operations at one end of the buffer, the Queue needs to be implemented as a
 * circular array in order to quickly add or remove elements.
 *
 * The Queue has three main functions: `enqueue` which adds an element to the
 * Queue; `dequeue` which removes an element from the Queue; and `peek` which
 * return the element at the front of the Queue, that is, the next element to
 * be removed from it.
 *
 * The Queue is used in many applications where a resource is shared among
 * multiple consumers and the Queue is responsible for scheduling the access to
 * the resource.
 */

#ifndef CMC_CMC_QUEUE_H
#define CMC_CMC_QUEUE_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "cor_core.h"

/**
 * Core Queue implementation
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_CMC_QUEUE_CORE(ACCESS, FILE, PARAMS) CMC_(CMC_(CMC_CMC_QUEUE_CORE_, ACCESS), CMC_(_, FILE))(PARAMS)

/* PRIVATE or PUBLIC solver */
#define CMC_CMC_QUEUE_CORE_PUBLIC_HEADER(PARAMS) \
    CMC_CMC_QUEUE_CORE_STRUCT(PARAMS) \
    CMC_CMC_QUEUE_CORE_HEADER(PARAMS)

#define CMC_CMC_QUEUE_CORE_PUBLIC_SOURCE(PARAMS) CMC_CMC_QUEUE_CORE_SOURCE(PARAMS)

#define CMC_CMC_QUEUE_CORE_PRIVATE_HEADER(PARAMS) \
    struct CMC_PARAM_SNAME(PARAMS); \
    CMC_CMC_QUEUE_CORE_HEADER(PARAMS)

#define CMC_CMC_QUEUE_CORE_PRIVATE_SOURCE(PARAMS) \
    CMC_CMC_QUEUE_CORE_STRUCT(PARAMS) \
    CMC_CMC_QUEUE_CORE_SOURCE(PARAMS)

/* Lowest level API */
#define CMC_CMC_QUEUE_CORE_STRUCT(PARAMS) \
    CMC_CMC_QUEUE_CORE_STRUCT_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_QUEUE_CORE_HEADER(PARAMS) \
    CMC_CMC_QUEUE_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_QUEUE_CORE_SOURCE(PARAMS) \
    CMC_CMC_QUEUE_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Struct
 * ------------------------------------------------------------------------- */
#define CMC_CMC_QUEUE_CORE_STRUCT_(PFX, SNAME, V) \
\
    /* Queue Structure */ \
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
        /* Index representing the front of the queue */ \
        size_t front; \
\
        /* Index representing the back of the queue */ \
        size_t back; \
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
    };

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_QUEUE_CORE_HEADER_(PFX, SNAME, V) \
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
    struct SNAME *CMC_(PFX, _new)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val); \
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val, \
                                         struct CMC_ALLOC_NODE_NAME * alloc, struct CMC_CALLBACKS_NAME * callbacks); \
    void CMC_(PFX, _clear)(struct SNAME * _queue_); \
    void CMC_(PFX, _free)(struct SNAME * _queue_); \
    /* Customization of Allocation and Callbacks */ \
    void CMC_(PFX, _customize)(struct SNAME * _queue_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks); \
    /* Collection Input and Output */ \
    bool CMC_(PFX, _enqueue)(struct SNAME * _queue_, V value); \
    bool CMC_(PFX, _dequeue)(struct SNAME * _queue_); \
    /* Element Access */ \
    V CMC_(PFX, _peek)(struct SNAME * _queue_); \
    /* Collection State */ \
    bool CMC_(PFX, _contains)(struct SNAME * _queue_, V value); \
    bool CMC_(PFX, _empty)(struct SNAME * _queue_); \
    bool CMC_(PFX, _full)(struct SNAME * _queue_); \
    size_t CMC_(PFX, _count)(struct SNAME * _queue_); \
    size_t CMC_(PFX, _capacity)(struct SNAME * _queue_); \
    int CMC_(PFX, _flag)(struct SNAME * _queue_); \
    /* Collection Utility */ \
    bool CMC_(PFX, _resize)(struct SNAME * _queue_, size_t capacity); \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _queue_); \
    bool CMC_(PFX, _equals)(struct SNAME * _queue1_, struct SNAME * _queue2_);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_QUEUE_CORE_SOURCE_(PFX, SNAME, V) \
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
        struct SNAME *_queue_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_queue_) \
            return NULL; \
\
        _queue_->buffer = alloc->calloc(capacity, sizeof(V)); \
\
        if (!_queue_->buffer) \
        { \
            alloc->free(_queue_); \
            return NULL; \
        } \
\
        _queue_->capacity = capacity; \
        _queue_->count = 0; \
        _queue_->front = 0; \
        _queue_->back = 0; \
        _queue_->flag = CMC_FLAG_OK; \
        _queue_->f_val = f_val; \
        _queue_->alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(_queue_, callbacks); \
\
        return _queue_; \
    } \
\
    void CMC_(PFX, _clear)(struct SNAME * _queue_) \
    { \
        if (_queue_->f_val->free) \
        { \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++) \
            { \
                _queue_->f_val->free(_queue_->buffer[i]); \
\
                i = (i + 1) % _queue_->capacity; \
            } \
        } \
\
        memset(_queue_->buffer, 0, sizeof(V) * _queue_->capacity); \
\
        _queue_->count = 0; \
        _queue_->front = 0; \
        _queue_->back = 0; \
        _queue_->flag = CMC_FLAG_OK; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _queue_) \
    { \
        if (_queue_->f_val->free) \
        { \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++) \
            { \
                _queue_->f_val->free(_queue_->buffer[i]); \
\
                i = (i + 1) % _queue_->capacity; \
            } \
        } \
\
        _queue_->alloc->free(_queue_->buffer); \
        _queue_->alloc->free(_queue_); \
    } \
\
    void CMC_(PFX, _customize)(struct SNAME * _queue_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!alloc) \
            _queue_->alloc = &cmc_alloc_node_default; \
        else \
            _queue_->alloc = alloc; \
\
        CMC_CALLBACKS_ASSIGN(_queue_, callbacks); \
\
        _queue_->flag = CMC_FLAG_OK; \
    } \
\
    bool CMC_(PFX, _enqueue)(struct SNAME * _queue_, V value) \
    { \
        if (CMC_(PFX, _full)(_queue_)) \
        { \
            if (!CMC_(PFX, _resize)(_queue_, _queue_->capacity * 2)) \
                return false; \
        } \
\
        _queue_->buffer[_queue_->back] = value; \
\
        _queue_->back = (_queue_->back == _queue_->capacity - 1) ? 0 : _queue_->back + 1; \
        _queue_->count++; \
        _queue_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_queue_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _dequeue)(struct SNAME * _queue_) \
    { \
        if (CMC_(PFX, _empty)(_queue_)) \
        { \
            _queue_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        _queue_->buffer[_queue_->front] = (V){ 0 }; \
\
        _queue_->front = (_queue_->front == _queue_->capacity - 1) ? 0 : _queue_->front + 1; \
        _queue_->count--; \
        _queue_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_queue_, delete); \
\
        return true; \
    } \
\
    V CMC_(PFX, _peek)(struct SNAME * _queue_) \
    { \
        if (CMC_(PFX, _empty)(_queue_)) \
        { \
            _queue_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        _queue_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_queue_, read); \
\
        return _queue_->buffer[_queue_->front]; \
    } \
\
    bool CMC_(PFX, _contains)(struct SNAME * _queue_, V value) \
    { \
        _queue_->flag = CMC_FLAG_OK; \
\
        bool result = false; \
\
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++) \
        { \
            if (_queue_->f_val->cmp(_queue_->buffer[i], value) == 0) \
            { \
                result = true; \
                break; \
            } \
\
            i = (i + 1) % _queue_->capacity; \
        } \
\
        CMC_CALLBACKS_CALL(_queue_, read); \
\
        return result; \
    } \
\
    bool CMC_(PFX, _empty)(struct SNAME * _queue_) \
    { \
        return _queue_->count == 0; \
    } \
\
    bool CMC_(PFX, _full)(struct SNAME * _queue_) \
    { \
        return _queue_->count >= _queue_->capacity; \
    } \
\
    size_t CMC_(PFX, _count)(struct SNAME * _queue_) \
    { \
        return _queue_->count; \
    } \
\
    size_t CMC_(PFX, _capacity)(struct SNAME * _queue_) \
    { \
        return _queue_->capacity; \
    } \
\
    int CMC_(PFX, _flag)(struct SNAME * _queue_) \
    { \
        return _queue_->flag; \
    } \
\
    bool CMC_(PFX, _resize)(struct SNAME * _queue_, size_t capacity) \
    { \
        if (_queue_->capacity == capacity) \
            goto success; \
\
        if (capacity < _queue_->count) \
        { \
            _queue_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        V *new_buffer = _queue_->alloc->malloc(sizeof(V) * capacity); \
\
        if (!new_buffer) \
        { \
            _queue_->flag = CMC_FLAG_ALLOC; \
            return false; \
        } \
\
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++) \
        { \
            new_buffer[j] = _queue_->buffer[i]; \
\
            i = (i + 1) % _queue_->capacity; \
        } \
\
        _queue_->alloc->free(_queue_->buffer); \
\
        _queue_->buffer = new_buffer; \
        _queue_->capacity = capacity; \
        _queue_->front = 0; \
        _queue_->back = _queue_->count; \
\
    success: \
\
        _queue_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_queue_, resize); \
\
        return true; \
    } \
\
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _queue_) \
    { \
        struct SNAME *result = CMC_(PFX, _new_custom)(_queue_->capacity, _queue_->f_val, _queue_->alloc, NULL); \
\
        if (!result) \
        { \
            _queue_->flag = CMC_FLAG_ERROR; \
            return NULL; \
        } \
\
        CMC_CALLBACKS_ASSIGN(result, _queue_->callbacks); \
\
        if (_queue_->f_val->cpy) \
        { \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++) \
            { \
                result->buffer[j] = _queue_->f_val->cpy(_queue_->buffer[i]); \
\
                i = (i + 1) % _queue_->capacity; \
            } \
        } \
        else \
        { \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++) \
            { \
                result->buffer[j] = _queue_->buffer[i]; \
\
                i = (i + 1) % _queue_->capacity; \
            } \
        } \
\
        result->count = _queue_->count; \
        result->front = 0; \
        result->back = _queue_->count; \
\
        _queue_->flag = CMC_FLAG_OK; \
\
        return result; \
    } \
\
    bool CMC_(PFX, _equals)(struct SNAME * _queue1_, struct SNAME * _queue2_) \
    { \
        _queue1_->flag = CMC_FLAG_OK; \
        _queue2_->flag = CMC_FLAG_OK; \
\
        if (_queue1_->count != _queue2_->count) \
            return false; \
\
        size_t i, j, k; \
        for (i = _queue1_->front, j = _queue2_->front, k = 0; k < _queue1_->count; k++) \
        { \
            if (_queue1_->f_val->cmp(_queue1_->buffer[i], _queue2_->buffer[j]) != 0) \
                return false; \
\
            i = (i + 1) % _queue1_->capacity; \
            j = (j + 1) % _queue2_->capacity; \
        } \
\
        return true; \
    }

#endif /* CMC_CMC_QUEUE_H */
