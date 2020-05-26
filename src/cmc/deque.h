/**
 * deque.h
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
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * Deque Specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_cmc_string_fmt_deque = "struct %s<%s> "
                                              "at %p { "
                                              "buffer:%p, "
                                              "capacity:%" PRIuMAX ", "
                                              "count:%" PRIuMAX ", "
                                              "front:%" PRIuMAX ", "
                                              "back:%" PRIuMAX ", "
                                              "flag:%d, "
                                              "f_val:%p, "
                                              "alloc:%p, "
                                              "callbacks:%p }";

/**
 * Core Deque implementation
 */
#define CMC_CMC_DEQUE_CORE(BODY)    \
    CMC_CMC_DEQUE_CORE_HEADER(BODY) \
    CMC_CMC_DEQUE_CORE_SOURCE(BODY)

#define CMC_CMC_DEQUE_CORE_HEADER(BODY)                                    \
    CMC_CMC_DEQUE_CORE_HEADER_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                               CMC_PARAM_V(BODY))

#define CMC_CMC_DEQUE_CORE_SOURCE(BODY)                                    \
    CMC_CMC_DEQUE_CORE_SOURCE_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                               CMC_PARAM_V(BODY))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_DEQUE_CORE_HEADER_(PFX, SNAME, V)                              \
                                                                               \
    /* Deque Structure */                                                      \
    struct SNAME                                                               \
    {                                                                          \
        /* Dynamic circular array of elements */                               \
        V *buffer;                                                             \
                                                                               \
        /* Current circular array capacity */                                  \
        size_t capacity;                                                       \
                                                                               \
        /* Current amount of elements */                                       \
        size_t count;                                                          \
                                                                               \
        /* Index representing the front of the deque */                        \
        size_t front;                                                          \
                                                                               \
        /* Index representing the back of the deque */                         \
        size_t back;                                                           \
                                                                               \
        /* Flags indicating errors or success */                               \
        int flag;                                                              \
                                                                               \
        struct CMC_DEF_FVAL(SNAME) * f_val;                                    \
                                                                               \
        /* Custom allocation functions */                                      \
        struct cmc_alloc_node *alloc;                                          \
                                                                               \
        /* Custom callback functions */                                        \
        struct cmc_callbacks *callbacks;                                       \
    };                                                                         \
                                                                               \
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
    /* Deque Iterator */                                                       \
    struct CMC_DEF_ITER(SNAME)                                                 \
    {                                                                          \
        /* Target deque */                                                     \
        struct SNAME *target;                                                  \
                                                                               \
        /* Cursor's position (index) */                                        \
        size_t cursor;                                                         \
                                                                               \
        /* Keeps track of relative index to the iteration of elements */       \
        size_t index;                                                          \
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
    void CMC_(PFX, _clear)(struct SNAME * _deque_);                            \
    void CMC_(PFX, _free)(struct SNAME * _deque_);                             \
    void CMC_(PFX, _release)(struct SNAME _deque_);                            \
    /* Customization of Allocation and Callbacks */                            \
    void CMC_(PFX, _customize)(struct SNAME * _deque_,                         \
                               struct cmc_alloc_node * alloc,                  \
                               struct cmc_callbacks * callbacks);              \
    /* Collection Input and Output */                                          \
    bool CMC_(PFX, _push_front)(struct SNAME * _deque_, V value);              \
    bool CMC_(PFX, _push_back)(struct SNAME * _deque_, V value);               \
    bool CMC_(PFX, _pop_front)(struct SNAME * _deque_);                        \
    bool CMC_(PFX, _pop_back)(struct SNAME * _deque_);                         \
    /* Element Access */                                                       \
    V CMC_(PFX, _front)(struct SNAME * _deque_);                               \
    V CMC_(PFX, _back)(struct SNAME * _deque_);                                \
    /* Collection State */                                                     \
    bool CMC_(PFX, _contains)(struct SNAME * _deque_, V value);                \
    bool CMC_(PFX, _empty)(struct SNAME * _deque_);                            \
    bool CMC_(PFX, _full)(struct SNAME * _deque_);                             \
    size_t CMC_(PFX, _count)(struct SNAME * _deque_);                          \
    size_t CMC_(PFX, _capacity)(struct SNAME * _deque_);                       \
    int CMC_(PFX, _flag)(struct SNAME * _deque_);                              \
    /* Collection Utility */                                                   \
    bool CMC_(PFX, _resize)(struct SNAME * _deque_, size_t capacity);          \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _deque_);                 \
    bool CMC_(PFX, _equals)(struct SNAME * _deque1_, struct SNAME * _deque2_); \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _deque_);           \
    bool CMC_(PFX, _print)(struct SNAME * _deque_, FILE * fptr);               \
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
#define CMC_CMC_DEQUE_CORE_SOURCE_(PFX, SNAME, V)                              \
                                                                               \
    /* Implementation Detail Functions */                                      \
    /* None */                                                                 \
                                                                               \
    struct SNAME *CMC_(PFX, _new)(size_t capacity,                             \
                                  struct CMC_DEF_FVAL(SNAME) * f_val)          \
    {                                                                          \
        return CMC_(PFX, _new_custom)(capacity, f_val, NULL, NULL);            \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _new_custom)(                                      \
        size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,                   \
        struct cmc_alloc_node * alloc, struct cmc_callbacks * callbacks)       \
    {                                                                          \
        if (capacity < 1)                                                      \
            return NULL;                                                       \
                                                                               \
        if (!f_val)                                                            \
            return NULL;                                                       \
                                                                               \
        if (!alloc)                                                            \
            alloc = &cmc_alloc_node_default;                                   \
                                                                               \
        struct SNAME *_deque_ = alloc->malloc(sizeof(struct SNAME));           \
                                                                               \
        if (!_deque_)                                                          \
            return NULL;                                                       \
                                                                               \
        _deque_->buffer = alloc->calloc(capacity, sizeof(V));                  \
                                                                               \
        if (!_deque_->buffer)                                                  \
        {                                                                      \
            alloc->free(_deque_);                                              \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _deque_->capacity = capacity;                                          \
        _deque_->count = 0;                                                    \
        _deque_->front = 0;                                                    \
        _deque_->back = 0;                                                     \
        _deque_->flag = CMC_FLAG_OK;                                           \
        _deque_->f_val = f_val;                                                \
        _deque_->alloc = alloc;                                                \
        _deque_->callbacks = callbacks;                                        \
                                                                               \
        return _deque_;                                                        \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _clear)(struct SNAME * _deque_)                             \
    {                                                                          \
        if (_deque_->f_val->free)                                              \
        {                                                                      \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)    \
            {                                                                  \
                _deque_->f_val->free(_deque_->buffer[i]);                      \
                                                                               \
                i = (i + 1) % _deque_->capacity;                               \
            }                                                                  \
        }                                                                      \
                                                                               \
        memset(_deque_->buffer, 0, sizeof(V) * _deque_->capacity);             \
                                                                               \
        _deque_->count = 0;                                                    \
        _deque_->front = 0;                                                    \
        _deque_->back = 0;                                                     \
                                                                               \
        _deque_->flag = CMC_FLAG_OK;                                           \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _free)(struct SNAME * _deque_)                              \
    {                                                                          \
        if (_deque_->f_val->free)                                              \
        {                                                                      \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)    \
            {                                                                  \
                _deque_->f_val->free(_deque_->buffer[i]);                      \
                                                                               \
                i = (i + 1) % _deque_->capacity;                               \
            }                                                                  \
        }                                                                      \
                                                                               \
        _deque_->alloc->free(_deque_->buffer);                                 \
        _deque_->alloc->free(_deque_);                                         \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _release)(struct SNAME _deque_)                             \
    {                                                                          \
        if (_deque_.f_val->free)                                               \
        {                                                                      \
            for (size_t i = _deque_.front, j = 0; j < _deque_.count; j++)      \
            {                                                                  \
                _deque_.f_val->free(_deque_.buffer[i]);                        \
                                                                               \
                i = (i + 1) % _deque_.capacity;                                \
            }                                                                  \
        }                                                                      \
                                                                               \
        _deque_.alloc->free(_deque_.buffer);                                   \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _customize)(struct SNAME * _deque_,                         \
                               struct cmc_alloc_node * alloc,                  \
                               struct cmc_callbacks * callbacks)               \
    {                                                                          \
        if (!alloc)                                                            \
            _deque_->alloc = &cmc_alloc_node_default;                          \
        else                                                                   \
            _deque_->alloc = alloc;                                            \
                                                                               \
        _deque_->callbacks = callbacks;                                        \
                                                                               \
        _deque_->flag = CMC_FLAG_OK;                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _push_front)(struct SNAME * _deque_, V value)               \
    {                                                                          \
        if (CMC_(PFX, _full)(_deque_))                                         \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_deque_, _deque_->capacity * 2))           \
                return false;                                                  \
        }                                                                      \
                                                                               \
        _deque_->front = (_deque_->front == 0) ? _deque_->capacity - 1         \
                                               : _deque_->front - 1;           \
                                                                               \
        _deque_->buffer[_deque_->front] = value;                               \
                                                                               \
        _deque_->count++;                                                      \
        _deque_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_deque_->callbacks && _deque_->callbacks->create)                  \
            _deque_->callbacks->create();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _push_back)(struct SNAME * _deque_, V value)                \
    {                                                                          \
        if (CMC_(PFX, _full)(_deque_))                                         \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_deque_, _deque_->capacity * 2))           \
                return false;                                                  \
        }                                                                      \
                                                                               \
        _deque_->buffer[_deque_->back] = value;                                \
                                                                               \
        _deque_->back =                                                        \
            (_deque_->back == _deque_->capacity - 1) ? 0 : _deque_->back + 1;  \
                                                                               \
        _deque_->count++;                                                      \
        _deque_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_deque_->callbacks && _deque_->callbacks->create)                  \
            _deque_->callbacks->create();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _pop_front)(struct SNAME * _deque_)                         \
    {                                                                          \
        if (CMC_(PFX, _empty)(_deque_))                                        \
        {                                                                      \
            _deque_->flag = CMC_FLAG_EMPTY;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        _deque_->buffer[_deque_->front] = (V){ 0 };                            \
                                                                               \
        _deque_->front = (_deque_->front == _deque_->capacity - 1)             \
                             ? 0                                               \
                             : _deque_->front + 1;                             \
                                                                               \
        _deque_->count--;                                                      \
        _deque_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_deque_->callbacks && _deque_->callbacks->delete)                  \
            _deque_->callbacks->delete ();                                     \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _pop_back)(struct SNAME * _deque_)                          \
    {                                                                          \
        if (CMC_(PFX, _empty)(_deque_))                                        \
        {                                                                      \
            _deque_->flag = CMC_FLAG_EMPTY;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        _deque_->back =                                                        \
            (_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1;  \
                                                                               \
        _deque_->buffer[_deque_->back] = (V){ 0 };                             \
                                                                               \
        _deque_->count--;                                                      \
        _deque_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_deque_->callbacks && _deque_->callbacks->delete)                  \
            _deque_->callbacks->delete ();                                     \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _front)(struct SNAME * _deque_)                                \
    {                                                                          \
        if (CMC_(PFX, _empty)(_deque_))                                        \
        {                                                                      \
            _deque_->flag = CMC_FLAG_EMPTY;                                    \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _deque_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_deque_->callbacks && _deque_->callbacks->read)                    \
            _deque_->callbacks->read();                                        \
                                                                               \
        return _deque_->buffer[_deque_->front];                                \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _back)(struct SNAME * _deque_)                                 \
    {                                                                          \
        if (CMC_(PFX, _empty)(_deque_))                                        \
        {                                                                      \
            _deque_->flag = CMC_FLAG_EMPTY;                                    \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _deque_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_deque_->callbacks && _deque_->callbacks->read)                    \
            _deque_->callbacks->read();                                        \
                                                                               \
        return _deque_->buffer[(_deque_->back == 0) ? _deque_->capacity - 1    \
                                                    : _deque_->back - 1];      \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _contains)(struct SNAME * _deque_, V value)                 \
    {                                                                          \
        _deque_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        bool result = false;                                                   \
                                                                               \
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)        \
        {                                                                      \
            if (_deque_->f_val->cmp(_deque_->buffer[i], value) == 0)           \
            {                                                                  \
                result = true;                                                 \
                break;                                                         \
            }                                                                  \
                                                                               \
            i = (i + 1) % _deque_->capacity;                                   \
        }                                                                      \
                                                                               \
        if (_deque_->callbacks && _deque_->callbacks->read)                    \
            _deque_->callbacks->read();                                        \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _empty)(struct SNAME * _deque_)                             \
    {                                                                          \
        return _deque_->count == 0;                                            \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _full)(struct SNAME * _deque_)                              \
    {                                                                          \
        return _deque_->count >= _deque_->capacity;                            \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _count)(struct SNAME * _deque_)                           \
    {                                                                          \
        return _deque_->count;                                                 \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _capacity)(struct SNAME * _deque_)                        \
    {                                                                          \
        return _deque_->capacity;                                              \
    }                                                                          \
                                                                               \
    int CMC_(PFX, _flag)(struct SNAME * _deque_)                               \
    {                                                                          \
        return _deque_->flag;                                                  \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _resize)(struct SNAME * _deque_, size_t capacity)           \
    {                                                                          \
        _deque_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_deque_->capacity == capacity)                                     \
            goto success;                                                      \
                                                                               \
        if (capacity < _deque_->count)                                         \
        {                                                                      \
            _deque_->flag = CMC_FLAG_INVALID;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        V *new_buffer = _deque_->alloc->malloc(sizeof(V) * capacity);          \
                                                                               \
        if (!new_buffer)                                                       \
        {                                                                      \
            _deque_->flag = CMC_FLAG_ALLOC;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)        \
        {                                                                      \
            new_buffer[j] = _deque_->buffer[i];                                \
                                                                               \
            i = (i + 1) % _deque_->capacity;                                   \
        }                                                                      \
                                                                               \
        _deque_->alloc->free(_deque_->buffer);                                 \
                                                                               \
        _deque_->buffer = new_buffer;                                          \
        _deque_->capacity = capacity;                                          \
        _deque_->front = 0;                                                    \
        _deque_->back = _deque_->count;                                        \
                                                                               \
    success:                                                                   \
                                                                               \
        if (_deque_->callbacks && _deque_->callbacks->resize)                  \
            _deque_->callbacks->resize();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _deque_)                  \
    {                                                                          \
        struct SNAME *result =                                                 \
            CMC_(PFX, _new_custom)(_deque_->capacity, _deque_->f_val,          \
                                   _deque_->alloc, _deque_->callbacks);        \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _deque_->flag = CMC_FLAG_ERROR;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (_deque_->f_val->cpy)                                               \
        {                                                                      \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)    \
            {                                                                  \
                result->buffer[j] = _deque_->f_val->cpy(_deque_->buffer[i]);   \
                                                                               \
                i = (i + 1) % _deque_->capacity;                               \
            }                                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)    \
            {                                                                  \
                result->buffer[j] = _deque_->buffer[i];                        \
                                                                               \
                i = (i + 1) % _deque_->capacity;                               \
            }                                                                  \
        }                                                                      \
                                                                               \
        result->count = _deque_->count;                                        \
        result->front = 0;                                                     \
        result->back = _deque_->count;                                         \
                                                                               \
        _deque_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _equals)(struct SNAME * _deque1_, struct SNAME * _deque2_)  \
    {                                                                          \
        _deque1_->flag = CMC_FLAG_OK;                                          \
        _deque2_->flag = CMC_FLAG_OK;                                          \
                                                                               \
        if (_deque1_->count != _deque2_->count)                                \
            return false;                                                      \
                                                                               \
        size_t i, j, k;                                                        \
        for (i = _deque1_->front, j = _deque2_->front, k = 0;                  \
             k < _deque1_->count; k++)                                         \
        {                                                                      \
            if (_deque1_->f_val->cmp(_deque1_->buffer[i],                      \
                                     _deque2_->buffer[j]) != 0)                \
                return false;                                                  \
                                                                               \
            i = (i + 1) % _deque1_->capacity;                                  \
            j = (j + 1) % _deque2_->capacity;                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _deque_)            \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *d_ = _deque_;                                            \
                                                                               \
        int n =                                                                \
            snprintf(str.s, cmc_string_len, cmc_cmc_string_fmt_deque, #SNAME,  \
                     #V, d_, d_->buffer, d_->capacity, d_->count, d_->front,   \
                     d_->back, d_->flag, d_->f_val, d_->alloc, d_->callbacks); \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _print)(struct SNAME * _deque_, FILE * fptr)                \
    {                                                                          \
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)        \
        {                                                                      \
            if (!_deque_->f_val->str(fptr, _deque_->buffer[i]))                \
                return false;                                                  \
                                                                               \
            i = (i + 1) % _deque_->capacity;                                   \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target)   \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        iter.target = target;                                                  \
        iter.cursor = target->front;                                           \
        iter.index = 0;                                                        \
        iter.start = true;                                                     \
        iter.end = CMC_(PFX, _empty)(target);                                  \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target)     \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        iter.target = target;                                                  \
                                                                               \
        if (!CMC_(PFX, _empty)(target))                                        \
        {                                                                      \
            if (iter.target->back == 0)                                        \
                iter.cursor = iter.target->capacity - 1;                       \
            else                                                               \
                iter.cursor = iter.target->back - 1;                           \
                                                                               \
            iter.index = iter.target->count - 1;                               \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            iter.cursor = 0;                                                   \
            iter.index = 0;                                                    \
        }                                                                      \
                                                                               \
        iter.start = CMC_(PFX, _empty)(target);                                \
        iter.end = true;                                                       \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                          \
        return CMC_(PFX, _empty)(iter->target) || iter->start;                 \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                          \
        return CMC_(PFX, _empty)(iter->target) || iter->end;                   \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                          \
        if (!CMC_(PFX, _empty)(iter->target))                                  \
        {                                                                      \
            iter->cursor = iter->target->front;                                \
            iter->index = 0;                                                   \
            iter->start = true;                                                \
            iter->end = false;                                                 \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                          \
        if (!CMC_(PFX, _empty)(iter->target))                                  \
        {                                                                      \
            if (iter->target->back == 0)                                       \
                iter->cursor = iter->target->capacity - 1;                     \
            else                                                               \
                iter->cursor = iter->target->back - 1;                         \
                                                                               \
            iter->index = iter->target->count - 1;                             \
                                                                               \
            iter->start = false;                                               \
            iter->end = true;                                                  \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)              \
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
        iter->start = CMC_(PFX, _empty)(iter->target);                         \
                                                                               \
        iter->cursor = (iter->cursor + 1) % (iter->target->capacity);          \
        iter->index++;                                                         \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)              \
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
        iter->end = CMC_(PFX, _empty)(iter->target);                           \
                                                                               \
        iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1        \
                                           : iter->cursor - 1;                 \
        iter->index--;                                                         \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                  size_t steps)                                \
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
        iter->start = CMC_(PFX, _empty)(iter->target);                         \
                                                                               \
        iter->index += steps;                                                  \
        iter->cursor = (iter->cursor + steps) % iter->target->capacity;        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter,            \
                                 size_t steps)                                 \
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
        iter->end = CMC_(PFX, _empty)(iter->target);                           \
                                                                               \
        iter->index -= steps;                                                  \
                                                                               \
        /* Prevent underflow */                                                \
        if (iter->cursor < steps)                                              \
            iter->cursor += iter->target->capacity;                            \
                                                                               \
        iter->cursor -= steps;                                                 \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator was able to be positioned at the */   \
    /* given index */                                                          \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter,             \
                                size_t index)                                  \
    {                                                                          \
        if (index >= iter->target->count)                                      \
            return false;                                                      \
                                                                               \
        if (iter->index > index)                                               \
            return CMC_(PFX, _iter_rewind)(iter, iter->index - index);         \
        else if (iter->index < index)                                          \
            return CMC_(PFX, _iter_advance)(iter, index - iter->index);        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)                \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
            return (V){ 0 };                                                   \
                                                                               \
        return iter->target->buffer[iter->cursor];                             \
    }                                                                          \
                                                                               \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
            return NULL;                                                       \
                                                                               \
        return &(iter->target->buffer[iter->cursor]);                          \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)           \
    {                                                                          \
        return iter->index;                                                    \
    }

#endif /* CMC_CMC_DEQUE_H */
