/**
 * stack.h
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

/* -------------------------------------------------------------------------
 * Stack specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_cmc_string_fmt_stack = "struct %s<%s> "
                                              "at %p { "
                                              "buffer:%p, "
                                              "capacity:%" PRIuMAX ", "
                                              "count:%" PRIuMAX ", "
                                              "flag:%d, "
                                              "f_val:%p, "
                                              "alloc:%p, "
                                              "callbacks:%p }";

/**
 * Core Stack implementation
 */
#define CMC_CMC_STACK_CORE(BODY)    \
    CMC_CMC_STACK_CORE_HEADER(BODY) \
    CMC_CMC_STACK_CORE_SOURCE(BODY)

#define CMC_CMC_STACK_CORE_HEADER(BODY)                                    \
    CMC_CMC_STACK_CORE_HEADER_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                               CMC_PARAM_V(BODY))

#define CMC_CMC_STACK_CORE_SOURCE(BODY)                                    \
    CMC_CMC_STACK_CORE_SOURCE_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                               CMC_PARAM_V(BODY))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_STACK_CORE_HEADER_(PFX, SNAME, V)                              \
                                                                               \
    /* Stack Structure */                                                      \
    struct SNAME                                                               \
    {                                                                          \
        /* Dynamic array of elements */                                        \
        V *buffer;                                                             \
                                                                               \
        /* Current array capacity */                                           \
        size_t capacity;                                                       \
                                                                               \
        /* Current amount of elements */                                       \
        size_t count;                                                          \
                                                                               \
        /* Flags indicating errors or success */                               \
        int flag;                                                              \
                                                                               \
        /* Value function table */                                             \
        struct CMC_DEF_FVAL(SNAME) * f_val;                                    \
                                                                               \
        /* Custom allocation functions */                                      \
        struct cmc_alloc_node *alloc;                                          \
                                                                               \
        /* Custom callback functions */                                        \
        struct cmc_callbacks *callbacks;                                       \
    };                                                                         \
                                                                               \
    /* Value struct function table */                                          \
    struct CMC_DEF_FVAL(SNAME)                                                 \
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
    /* Stack Iterator */                                                       \
    struct CMC_DEF_ITER(SNAME)                                                 \
    {                                                                          \
        /* Target stack */                                                     \
        struct SNAME *target;                                                  \
                                                                               \
        /* Cursor's position (index) */                                        \
        size_t cursor;                                                         \
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
    struct SNAME *CMC_(PFX, _new)(size_t capacity,                             \
                                  struct CMC_DEF_FVAL(SNAME) * f_val);         \
    struct SNAME *CMC_(PFX, _new_custom)(                                      \
        size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,                   \
        struct cmc_alloc_node * alloc, struct cmc_callbacks * callbacks);      \
    void CMC_(PFX, _clear)(struct SNAME * _stack_);                            \
    void CMC_(PFX, _free)(struct SNAME * _stack_);                             \
    /* Customization of Allocation and Callbacks */                            \
    void CMC_(PFX, _customize)(struct SNAME * _stack_,                         \
                               struct cmc_alloc_node * alloc,                  \
                               struct cmc_callbacks * callbacks);              \
    /* Collection Input and Output */                                          \
    bool CMC_(PFX, _push)(struct SNAME * _stack_, V value);                    \
    bool CMC_(PFX, _pop)(struct SNAME * _stack_);                              \
    /* Element Access */                                                       \
    V CMC_(PFX, _top)(struct SNAME * _stack_);                                 \
    /* Collection State */                                                     \
    bool CMC_(PFX, _contains)(struct SNAME * _stack_, V value);                \
    bool CMC_(PFX, _empty)(struct SNAME * _stack_);                            \
    bool CMC_(PFX, _full)(struct SNAME * _stack_);                             \
    size_t CMC_(PFX, _count)(struct SNAME * _stack_);                          \
    size_t CMC_(PFX, _capacity)(struct SNAME * _stack_);                       \
    int CMC_(PFX, _flag)(struct SNAME * _stack_);                              \
    /* Collection Utility */                                                   \
    bool CMC_(PFX, _resize)(struct SNAME * _stack_, size_t capacity);          \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _stack_);                 \
    bool CMC_(PFX, _equals)(struct SNAME * _stack1_, struct SNAME * _stack2_); \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _stack_);           \
    bool CMC_(PFX, _print)(struct SNAME * _stack_, FILE * fptr);               \
                                                                               \
    /* Iterator Functions */                                                   \
    /* Iterator Initialization */                                              \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target);  \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target);    \
    /* Iterator State */                                                       \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter);         \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter);           \
    /* Iterator Movement */                                                    \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter);         \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter);           \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter);             \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter);             \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                  size_t steps);                               \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter,            \
                                 size_t steps);                                \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter,             \
                                size_t index);                                 \
    /* Iterator Access */                                                      \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter);               \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter);             \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_STACK_CORE_SOURCE_(PFX, SNAME, V)                             \
                                                                              \
    /* Implementation Detail Functions */                                     \
    /* None */                                                                \
                                                                              \
    struct SNAME *CMC_(PFX, _new)(size_t capacity,                            \
                                  struct CMC_DEF_FVAL(SNAME) * f_val)         \
    {                                                                         \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;               \
                                                                              \
        if (capacity < 1)                                                     \
            return NULL;                                                      \
                                                                              \
        if (!f_val)                                                           \
            return NULL;                                                      \
                                                                              \
        struct SNAME *_stack_ = alloc->malloc(sizeof(struct SNAME));          \
                                                                              \
        if (!_stack_)                                                         \
            return NULL;                                                      \
                                                                              \
        _stack_->buffer = alloc->calloc(capacity, sizeof(V));                 \
                                                                              \
        if (!_stack_->buffer)                                                 \
        {                                                                     \
            alloc->free(_stack_);                                             \
            return NULL;                                                      \
        }                                                                     \
                                                                              \
        _stack_->capacity = capacity;                                         \
        _stack_->count = 0;                                                   \
        _stack_->flag = CMC_FLAG_OK;                                          \
        _stack_->f_val = f_val;                                               \
        _stack_->alloc = alloc;                                               \
        _stack_->callbacks = NULL;                                            \
                                                                              \
        return _stack_;                                                       \
    }                                                                         \
                                                                              \
    struct SNAME *CMC_(PFX, _new_custom)(                                     \
        size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,                  \
        struct cmc_alloc_node * alloc, struct cmc_callbacks * callbacks)      \
    {                                                                         \
        if (capacity < 1)                                                     \
            return NULL;                                                      \
        if (!f_val)                                                           \
            return NULL;                                                      \
                                                                              \
        if (!alloc)                                                           \
            alloc = &cmc_alloc_node_default;                                  \
                                                                              \
        struct SNAME *_stack_ = alloc->malloc(sizeof(struct SNAME));          \
                                                                              \
        if (!_stack_)                                                         \
            return NULL;                                                      \
                                                                              \
        _stack_->buffer = alloc->calloc(capacity, sizeof(V));                 \
                                                                              \
        if (!_stack_->buffer)                                                 \
        {                                                                     \
            alloc->free(_stack_);                                             \
            return NULL;                                                      \
        }                                                                     \
                                                                              \
        _stack_->capacity = capacity;                                         \
        _stack_->count = 0;                                                   \
        _stack_->flag = CMC_FLAG_OK;                                          \
        _stack_->f_val = f_val;                                               \
        _stack_->alloc = alloc;                                               \
        _stack_->callbacks = callbacks;                                       \
                                                                              \
        return _stack_;                                                       \
    }                                                                         \
                                                                              \
    void CMC_(PFX, _clear)(struct SNAME * _stack_)                            \
    {                                                                         \
        if (_stack_->f_val->free)                                             \
        {                                                                     \
            for (size_t i = 0; i < _stack_->count; i++)                       \
                _stack_->f_val->free(_stack_->buffer[i]);                     \
        }                                                                     \
                                                                              \
        memset(_stack_->buffer, 0, sizeof(V) * _stack_->capacity);            \
                                                                              \
        _stack_->count = 0;                                                   \
        _stack_->flag = CMC_FLAG_OK;                                          \
    }                                                                         \
                                                                              \
    void CMC_(PFX, _free)(struct SNAME * _stack_)                             \
    {                                                                         \
        if (_stack_->f_val->free)                                             \
        {                                                                     \
            for (size_t i = 0; i < _stack_->count; i++)                       \
                _stack_->f_val->free(_stack_->buffer[i]);                     \
        }                                                                     \
                                                                              \
        _stack_->alloc->free(_stack_->buffer);                                \
        _stack_->alloc->free(_stack_);                                        \
    }                                                                         \
                                                                              \
    void CMC_(PFX, _customize)(struct SNAME * _stack_,                        \
                               struct cmc_alloc_node * alloc,                 \
                               struct cmc_callbacks * callbacks)              \
    {                                                                         \
        if (!alloc)                                                           \
            _stack_->alloc = &cmc_alloc_node_default;                         \
        else                                                                  \
            _stack_->alloc = alloc;                                           \
                                                                              \
        _stack_->callbacks = callbacks;                                       \
                                                                              \
        _stack_->flag = CMC_FLAG_OK;                                          \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _push)(struct SNAME * _stack_, V value)                    \
    {                                                                         \
        if (CMC_(PFX, _full)(_stack_))                                        \
        {                                                                     \
            if (!CMC_(PFX, _resize)(_stack_, _stack_->capacity * 2))          \
                return false;                                                 \
        }                                                                     \
                                                                              \
        _stack_->buffer[_stack_->count++] = value;                            \
                                                                              \
        _stack_->flag = CMC_FLAG_OK;                                          \
                                                                              \
        if (_stack_->callbacks && _stack_->callbacks->create)                 \
            _stack_->callbacks->create();                                     \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _pop)(struct SNAME * _stack_)                              \
    {                                                                         \
        if (CMC_(PFX, _empty)(_stack_))                                       \
        {                                                                     \
            _stack_->flag = CMC_FLAG_EMPTY;                                   \
            return false;                                                     \
        }                                                                     \
                                                                              \
        _stack_->buffer[--_stack_->count] = (V){ 0 };                         \
                                                                              \
        _stack_->flag = CMC_FLAG_OK;                                          \
                                                                              \
        if (_stack_->callbacks && _stack_->callbacks->delete)                 \
            _stack_->callbacks->delete ();                                    \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    V CMC_(PFX, _top)(struct SNAME * _stack_)                                 \
    {                                                                         \
        if (CMC_(PFX, _empty)(_stack_))                                       \
        {                                                                     \
            _stack_->flag = CMC_FLAG_EMPTY;                                   \
            return (V){ 0 };                                                  \
        }                                                                     \
                                                                              \
        _stack_->flag = CMC_FLAG_OK;                                          \
                                                                              \
        if (_stack_->callbacks && _stack_->callbacks->read)                   \
            _stack_->callbacks->read();                                       \
                                                                              \
        return _stack_->buffer[_stack_->count - 1];                           \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _contains)(struct SNAME * _stack_, V value)                \
    {                                                                         \
        _stack_->flag = CMC_FLAG_OK;                                          \
                                                                              \
        bool result = false;                                                  \
                                                                              \
        for (size_t i = 0; i < _stack_->count; i++)                           \
        {                                                                     \
            if (_stack_->f_val->cmp(_stack_->buffer[i], value) == 0)          \
            {                                                                 \
                result = true;                                                \
                break;                                                        \
            }                                                                 \
        }                                                                     \
                                                                              \
        if (_stack_->callbacks && _stack_->callbacks->read)                   \
            _stack_->callbacks->read();                                       \
                                                                              \
        return result;                                                        \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _empty)(struct SNAME * _stack_)                            \
    {                                                                         \
        return _stack_->count == 0;                                           \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _full)(struct SNAME * _stack_)                             \
    {                                                                         \
        return _stack_->count >= _stack_->capacity;                           \
    }                                                                         \
                                                                              \
    size_t CMC_(PFX, _count)(struct SNAME * _stack_)                          \
    {                                                                         \
        return _stack_->count;                                                \
    }                                                                         \
                                                                              \
    size_t CMC_(PFX, _capacity)(struct SNAME * _stack_)                       \
    {                                                                         \
        return _stack_->capacity;                                             \
    }                                                                         \
                                                                              \
    int CMC_(PFX, _flag)(struct SNAME * _stack_)                              \
    {                                                                         \
        return _stack_->flag;                                                 \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _resize)(struct SNAME * _stack_, size_t capacity)          \
    {                                                                         \
        if (_stack_->capacity == capacity)                                    \
            goto success;                                                     \
                                                                              \
        if (capacity < _stack_->count)                                        \
        {                                                                     \
            _stack_->flag = CMC_FLAG_INVALID;                                 \
            return false;                                                     \
        }                                                                     \
                                                                              \
        V *new_buffer =                                                       \
            _stack_->alloc->realloc(_stack_->buffer, sizeof(V) * capacity);   \
                                                                              \
        if (!new_buffer)                                                      \
        {                                                                     \
            _stack_->flag = CMC_FLAG_ALLOC;                                   \
            return false;                                                     \
        }                                                                     \
                                                                              \
        /* TODO zero out new slots */                                         \
                                                                              \
        _stack_->buffer = new_buffer;                                         \
        _stack_->capacity = capacity;                                         \
                                                                              \
    success:                                                                  \
                                                                              \
        _stack_->flag = CMC_FLAG_OK;                                          \
                                                                              \
        if (_stack_->callbacks && _stack_->callbacks->resize)                 \
            _stack_->callbacks->resize();                                     \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _stack_)                 \
    {                                                                         \
        struct SNAME *result =                                                \
            CMC_(PFX, _new_custom)(_stack_->capacity, _stack_->f_val,         \
                                   _stack_->alloc, _stack_->callbacks);       \
                                                                              \
        if (!result)                                                          \
        {                                                                     \
            _stack_->flag = CMC_FLAG_ERROR;                                   \
            return NULL;                                                      \
        }                                                                     \
                                                                              \
        if (_stack_->f_val->cpy)                                              \
        {                                                                     \
            for (size_t i = 0; i < _stack_->count; i++)                       \
                result->buffer[i] = _stack_->f_val->cpy(_stack_->buffer[i]);  \
        }                                                                     \
        else                                                                  \
            memcpy(result->buffer, _stack_->buffer,                           \
                   sizeof(V) * _stack_->count);                               \
                                                                              \
        result->count = _stack_->count;                                       \
                                                                              \
        _stack_->flag = CMC_FLAG_OK;                                          \
                                                                              \
        return result;                                                        \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _equals)(struct SNAME * _stack1_, struct SNAME * _stack2_) \
    {                                                                         \
        _stack1_->flag = CMC_FLAG_OK;                                         \
        _stack2_->flag = CMC_FLAG_OK;                                         \
                                                                              \
        if (_stack1_->count != _stack2_->count)                               \
            return false;                                                     \
                                                                              \
        for (size_t i = 0; i < _stack1_->count; i++)                          \
        {                                                                     \
            if (_stack1_->f_val->cmp(_stack1_->buffer[i],                     \
                                     _stack2_->buffer[i]) != 0)               \
                return false;                                                 \
        }                                                                     \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _stack_)           \
    {                                                                         \
        struct cmc_string str;                                                \
        struct SNAME *s_ = _stack_;                                           \
                                                                              \
        int n = snprintf(str.s, cmc_string_len, cmc_cmc_string_fmt_stack,     \
                         #SNAME, #V, s_, s_->buffer, s_->capacity, s_->count, \
                         s_->flag, s_->f_val, s_->alloc, s_->callbacks);      \
                                                                              \
        return n >= 0 ? str : (struct cmc_string){ 0 };                       \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _print)(struct SNAME * _stack_, FILE * fptr)               \
    {                                                                         \
        for (size_t i = 0; i < _stack_->count; i++)                           \
        {                                                                     \
            if (!_stack_->f_val->str(fptr, _stack_->buffer[i]))               \
                return false;                                                 \
        }                                                                     \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target)  \
    {                                                                         \
        struct CMC_DEF_ITER(SNAME) iter;                                      \
                                                                              \
        iter.target = target;                                                 \
        iter.cursor = 0;                                                      \
        iter.start = true;                                                    \
        iter.end = CMC_(PFX, _empty)(target);                                 \
                                                                              \
        if (!CMC_(PFX, _empty)(target))                                       \
            iter.cursor = target->count - 1;                                  \
                                                                              \
        return iter;                                                          \
    }                                                                         \
                                                                              \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target)    \
    {                                                                         \
        struct CMC_DEF_ITER(SNAME) iter;                                      \
                                                                              \
        iter.target = target;                                                 \
        iter.cursor = 0;                                                      \
        iter.start = CMC_(PFX, _empty)(target);                               \
        iter.end = true;                                                      \
                                                                              \
        return iter;                                                          \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter)         \
    {                                                                         \
        return CMC_(PFX, _empty)(iter->target) || iter->start;                \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter)           \
    {                                                                         \
        return CMC_(PFX, _empty)(iter->target) || iter->end;                  \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)         \
    {                                                                         \
        if (!CMC_(PFX, _empty)(iter->target))                                 \
        {                                                                     \
            iter->cursor = iter->target->count - 1;                           \
            iter->start = true;                                               \
            iter->end = CMC_(PFX, _empty)(iter->target);                      \
                                                                              \
            return true;                                                      \
        }                                                                     \
                                                                              \
        return false;                                                         \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)           \
    {                                                                         \
        if (!CMC_(PFX, _empty)(iter->target))                                 \
        {                                                                     \
            iter->cursor = 0;                                                 \
            iter->start = CMC_(PFX, _empty)(iter->target);                    \
            iter->end = true;                                                 \
                                                                              \
            return true;                                                      \
        }                                                                     \
                                                                              \
        return false;                                                         \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)             \
    {                                                                         \
        if (iter->end)                                                        \
            return false;                                                     \
                                                                              \
        if (iter->cursor == 0)                                                \
        {                                                                     \
            iter->end = true;                                                 \
            return false;                                                     \
        }                                                                     \
                                                                              \
        iter->start = CMC_(PFX, _empty)(iter->target);                        \
                                                                              \
        iter->cursor--;                                                       \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)             \
    {                                                                         \
        if (iter->start)                                                      \
            return false;                                                     \
                                                                              \
        if (iter->cursor + 1 == iter->target->count)                          \
        {                                                                     \
            iter->start = true;                                               \
            return false;                                                     \
        }                                                                     \
                                                                              \
        iter->end = CMC_(PFX, _empty)(iter->target);                          \
                                                                              \
        iter->cursor++;                                                       \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    /* Returns true only if the iterator moved */                             \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,          \
                                  size_t steps)                               \
    {                                                                         \
        if (iter->end)                                                        \
            return false;                                                     \
                                                                              \
        if (iter->cursor == 0)                                                \
        {                                                                     \
            iter->end = true;                                                 \
            return false;                                                     \
        }                                                                     \
                                                                              \
        if (steps == 0 || iter->cursor < steps)                               \
            return false;                                                     \
                                                                              \
        iter->start = CMC_(PFX, _empty)(iter->target);                        \
                                                                              \
        iter->cursor -= steps;                                                \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    /* Returns true only if the iterator moved */                             \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                 size_t steps)                                \
    {                                                                         \
        if (iter->start)                                                      \
            return false;                                                     \
                                                                              \
        if (iter->cursor + 1 == iter->target->count)                          \
        {                                                                     \
            iter->start = true;                                               \
            return false;                                                     \
        }                                                                     \
                                                                              \
        if (steps == 0 || iter->cursor + steps >= iter->target->count)        \
            return false;                                                     \
                                                                              \
        iter->end = CMC_(PFX, _empty)(iter->target);                          \
                                                                              \
        iter->cursor += steps;                                                \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    /* Returns true only if the iterator was able to be positioned at the */  \
    /* given index */                                                         \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter,            \
                                size_t index)                                 \
    {                                                                         \
        if (index >= iter->target->count)                                     \
            return false;                                                     \
                                                                              \
        if (iter->cursor > index)                                             \
            return CMC_(PFX, _iter_advance)(iter, iter->cursor - index);      \
        else if (iter->cursor < index)                                        \
            return CMC_(PFX, _iter_rewind)(iter, index - iter->cursor);       \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)               \
    {                                                                         \
        if (CMC_(PFX, _empty)(iter->target))                                  \
            return (V){ 0 };                                                  \
                                                                              \
        return iter->target->buffer[iter->cursor];                            \
    }                                                                         \
                                                                              \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter)             \
    {                                                                         \
        if (CMC_(PFX, _empty)(iter->target))                                  \
            return NULL;                                                      \
                                                                              \
        return &(iter->target->buffer[iter->cursor]);                         \
    }                                                                         \
                                                                              \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                         \
        if (CMC_(PFX, _empty)(iter->target))                                  \
            return 0;                                                         \
                                                                              \
        return iter->target->count - 1 - iter->cursor;                        \
    }

#endif /* CMC_CMC_STACK_H */
