/**
 * cmc/stack.h
 *
 * Creation Date: 14/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Stack
 *
 * A Stack is a Last-in First-out (or First-in Last-out) data structure used in
 * a variety of algorithms. It is a Dynamic Array that can only add or remove
 * its elements at the end of the buffer, in this case, it represents the top of
 * the stack.
 *
 * It has three main functions: `push` which adds an element at the top of the
 * stack; `pop` which removes the top element from the stack; and `top` which
 * returns the top element without removing it (it is also sometimes called
 * `peek`).
 *
 * A Stack is used in algorithms like backtracking, depth-first search,
 * expression evaluation, syntax parsing and many more.
 */

#ifndef CMC_CMC_STACK_H
#define CMC_CMC_STACK_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/**
 * Core Stack implementation
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_CMC_STACK_CORE(ACCESS, FILE, PARAMS) CMC_(CMC_(CMC_CMC_STACK_CORE_, ACCESS), CMC_(_, FILE))(PARAMS)

/* PRIVATE or PUBLIC solver */
#define CMC_CMC_STACK_CORE_PUBLIC_HEADER(PARAMS) \
    CMC_CMC_STACK_CORE_STRUCT(PARAMS) \
    CMC_CMC_STACK_CORE_HEADER(PARAMS)

#define CMC_CMC_STACK_CORE_PUBLIC_SOURCE(PARAMS) CMC_CMC_STACK_CORE_SOURCE(PARAMS)

#define CMC_CMC_STACK_CORE_PRIVATE_HEADER(PARAMS) \
    struct CMC_PARAM_SNAME(PARAMS); \
    CMC_CMC_STACK_CORE_HEADER(PARAMS)

#define CMC_CMC_STACK_CORE_PRIVATE_SOURCE(PARAMS) \
    CMC_CMC_STACK_CORE_STRUCT(PARAMS) \
    CMC_CMC_STACK_CORE_SOURCE(PARAMS)

/* Lowest level API */
#define CMC_CMC_STACK_CORE_STRUCT(PARAMS) \
    CMC_CMC_STACK_CORE_STRUCT_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_STACK_CORE_HEADER(PARAMS) \
    CMC_CMC_STACK_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_STACK_CORE_SOURCE(PARAMS) \
    CMC_CMC_STACK_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Struct
 * ------------------------------------------------------------------------- */
#define CMC_CMC_STACK_CORE_STRUCT_(PFX, SNAME, V) \
\
    /* Stack Structure */ \
    struct SNAME \
    { \
        /* Dynamic array of elements */ \
        V *buffer; \
\
        /* Current array capacity */ \
        size_t capacity; \
\
        /* Current amount of elements */ \
        size_t count; \
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
#define CMC_CMC_STACK_CORE_HEADER_(PFX, SNAME, V) \
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
    void CMC_(PFX, _clear)(struct SNAME * _stack_); \
    void CMC_(PFX, _free)(struct SNAME * _stack_); \
    /* Customization of Allocation and Callbacks */ \
    void CMC_(PFX, _customize)(struct SNAME * _stack_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks); \
    /* Collection Input and Output */ \
    bool CMC_(PFX, _push)(struct SNAME * _stack_, V value); \
    bool CMC_(PFX, _pop)(struct SNAME * _stack_); \
    /* Element Access */ \
    V CMC_(PFX, _top)(struct SNAME * _stack_); \
    /* Collection State */ \
    bool CMC_(PFX, _contains)(struct SNAME * _stack_, V value); \
    bool CMC_(PFX, _empty)(struct SNAME * _stack_); \
    bool CMC_(PFX, _full)(struct SNAME * _stack_); \
    size_t CMC_(PFX, _count)(struct SNAME * _stack_); \
    size_t CMC_(PFX, _capacity)(struct SNAME * _stack_); \
    int CMC_(PFX, _flag)(struct SNAME * _stack_); \
    /* Collection Utility */ \
    bool CMC_(PFX, _resize)(struct SNAME * _stack_, size_t capacity); \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _stack_); \
    bool CMC_(PFX, _equals)(struct SNAME * _stack1_, struct SNAME * _stack2_);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_STACK_CORE_SOURCE_(PFX, SNAME, V) \
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
        if (!f_val) \
            return NULL; \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        struct SNAME *_stack_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_stack_) \
            return NULL; \
\
        _stack_->buffer = alloc->calloc(capacity, sizeof(V)); \
\
        if (!_stack_->buffer) \
        { \
            alloc->free(_stack_); \
            return NULL; \
        } \
\
        _stack_->capacity = capacity; \
        _stack_->count = 0; \
        _stack_->flag = CMC_FLAG_OK; \
        _stack_->f_val = f_val; \
        _stack_->alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(_stack_, callbacks); \
\
        return _stack_; \
    } \
\
    void CMC_(PFX, _clear)(struct SNAME * _stack_) \
    { \
        if (_stack_->f_val->free) \
        { \
            for (size_t i = 0; i < _stack_->count; i++) \
                _stack_->f_val->free(_stack_->buffer[i]); \
        } \
\
        memset(_stack_->buffer, 0, sizeof(V) * _stack_->capacity); \
\
        _stack_->count = 0; \
        _stack_->flag = CMC_FLAG_OK; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _stack_) \
    { \
        if (_stack_->f_val->free) \
        { \
            for (size_t i = 0; i < _stack_->count; i++) \
                _stack_->f_val->free(_stack_->buffer[i]); \
        } \
\
        _stack_->alloc->free(_stack_->buffer); \
        _stack_->alloc->free(_stack_); \
    } \
\
    void CMC_(PFX, _customize)(struct SNAME * _stack_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!alloc) \
            _stack_->alloc = &cmc_alloc_node_default; \
        else \
            _stack_->alloc = alloc; \
\
        CMC_CALLBACKS_ASSIGN(_stack_, callbacks); \
\
        _stack_->flag = CMC_FLAG_OK; \
    } \
\
    bool CMC_(PFX, _push)(struct SNAME * _stack_, V value) \
    { \
        if (CMC_(PFX, _full)(_stack_)) \
        { \
            if (!CMC_(PFX, _resize)(_stack_, _stack_->capacity * 2)) \
                return false; \
        } \
\
        _stack_->buffer[_stack_->count++] = value; \
\
        _stack_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_stack_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _pop)(struct SNAME * _stack_) \
    { \
        if (CMC_(PFX, _empty)(_stack_)) \
        { \
            _stack_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        _stack_->buffer[--_stack_->count] = (V){ 0 }; \
\
        _stack_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_stack_, delete); \
\
        return true; \
    } \
\
    V CMC_(PFX, _top)(struct SNAME * _stack_) \
    { \
        if (CMC_(PFX, _empty)(_stack_)) \
        { \
            _stack_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        _stack_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_stack_, read); \
\
        return _stack_->buffer[_stack_->count - 1]; \
    } \
\
    bool CMC_(PFX, _contains)(struct SNAME * _stack_, V value) \
    { \
        _stack_->flag = CMC_FLAG_OK; \
\
        bool result = false; \
\
        for (size_t i = 0; i < _stack_->count; i++) \
        { \
            if (_stack_->f_val->cmp(_stack_->buffer[i], value) == 0) \
            { \
                result = true; \
                break; \
            } \
        } \
\
        CMC_CALLBACKS_CALL(_stack_, read); \
\
        return result; \
    } \
\
    bool CMC_(PFX, _empty)(struct SNAME * _stack_) \
    { \
        return _stack_->count == 0; \
    } \
\
    bool CMC_(PFX, _full)(struct SNAME * _stack_) \
    { \
        return _stack_->count >= _stack_->capacity; \
    } \
\
    size_t CMC_(PFX, _count)(struct SNAME * _stack_) \
    { \
        return _stack_->count; \
    } \
\
    size_t CMC_(PFX, _capacity)(struct SNAME * _stack_) \
    { \
        return _stack_->capacity; \
    } \
\
    int CMC_(PFX, _flag)(struct SNAME * _stack_) \
    { \
        return _stack_->flag; \
    } \
\
    bool CMC_(PFX, _resize)(struct SNAME * _stack_, size_t capacity) \
    { \
        if (_stack_->capacity == capacity) \
            goto success; \
\
        if (capacity < _stack_->count) \
        { \
            _stack_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        V *new_buffer = _stack_->alloc->realloc(_stack_->buffer, sizeof(V) * capacity); \
\
        if (!new_buffer) \
        { \
            _stack_->flag = CMC_FLAG_ALLOC; \
            return false; \
        } \
\
        /* TODO zero out new slots */ \
\
        _stack_->buffer = new_buffer; \
        _stack_->capacity = capacity; \
\
    success: \
\
        _stack_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_stack_, resize); \
\
        return true; \
    } \
\
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _stack_) \
    { \
        struct SNAME *result = CMC_(PFX, _new_custom)(_stack_->capacity, _stack_->f_val, _stack_->alloc, NULL); \
\
        if (!result) \
        { \
            _stack_->flag = CMC_FLAG_ERROR; \
            return NULL; \
        } \
\
        CMC_CALLBACKS_ASSIGN(result, _stack_->callbacks); \
\
        if (_stack_->f_val->cpy) \
        { \
            for (size_t i = 0; i < _stack_->count; i++) \
                result->buffer[i] = _stack_->f_val->cpy(_stack_->buffer[i]); \
        } \
        else \
            memcpy(result->buffer, _stack_->buffer, sizeof(V) * _stack_->count); \
\
        result->count = _stack_->count; \
\
        _stack_->flag = CMC_FLAG_OK; \
\
        return result; \
    } \
\
    bool CMC_(PFX, _equals)(struct SNAME * _stack1_, struct SNAME * _stack2_) \
    { \
        _stack1_->flag = CMC_FLAG_OK; \
        _stack2_->flag = CMC_FLAG_OK; \
\
        if (_stack1_->count != _stack2_->count) \
            return false; \
\
        for (size_t i = 0; i < _stack1_->count; i++) \
        { \
            if (_stack1_->f_val->cmp(_stack1_->buffer[i], _stack2_->buffer[i]) != 0) \
                return false; \
        } \
\
        return true; \
    }

#endif /* CMC_CMC_STACK_H */
