/**
 * queue.h
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
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * Queue specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_cmc_string_fmt_queue = "struct %s<%s> "
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
 * Core Queue implementation
 */
#define CMC_CMC_QUEUE_CORE(BODY)    \
    CMC_CMC_QUEUE_CORE_HEADER(BODY) \
    CMC_CMC_QUEUE_CORE_SOURCE(BODY)

#define CMC_CMC_QUEUE_CORE_HEADER(BODY)                                    \
    CMC_CMC_QUEUE_CORE_HEADER_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                               CMC_PARAM_V(BODY))

#define CMC_CMC_QUEUE_CORE_SOURCE(BODY)                                    \
    CMC_CMC_QUEUE_CORE_SOURCE_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                               CMC_PARAM_V(BODY))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_QUEUE_CORE_HEADER_(PFX, SNAME, V)                              \
                                                                               \
    /* Queue Structure */                                                      \
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
        /* Index representing the front of the queue */                        \
        size_t front;                                                          \
                                                                               \
        /* Index representing the back of the queue */                         \
        size_t back;                                                           \
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
    /* Queue Iterator */                                                       \
    struct CMC_DEF_ITER(SNAME)                                                 \
    {                                                                          \
        /* Target queue */                                                     \
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
    void CMC_(PFX, _clear)(struct SNAME * _queue_);                            \
    void CMC_(PFX, _free)(struct SNAME * _queue_);                             \
    /* Customization of Allocation and Callbacks */                            \
    void CMC_(PFX, _customize)(struct SNAME * _queue_,                         \
                               struct cmc_alloc_node * alloc,                  \
                               struct cmc_callbacks * callbacks);              \
    /* Collection Input and Output */                                          \
    bool CMC_(PFX, _enqueue)(struct SNAME * _queue_, V value);                 \
    bool CMC_(PFX, _dequeue)(struct SNAME * _queue_);                          \
    /* Element Access */                                                       \
    V CMC_(PFX, _peek)(struct SNAME * _queue_);                                \
    /* Collection State */                                                     \
    bool CMC_(PFX, _contains)(struct SNAME * _queue_, V value);                \
    bool CMC_(PFX, _empty)(struct SNAME * _queue_);                            \
    bool CMC_(PFX, _full)(struct SNAME * _queue_);                             \
    size_t CMC_(PFX, _count)(struct SNAME * _queue_);                          \
    size_t CMC_(PFX, _capacity)(struct SNAME * _queue_);                       \
    int CMC_(PFX, _flag)(struct SNAME * _queue_);                              \
    /* Collection Utility */                                                   \
    bool CMC_(PFX, _resize)(struct SNAME * _queue_, size_t capacity);          \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _queue_);                 \
    bool CMC_(PFX, _equals)(struct SNAME * _queue1_, struct SNAME * _queue2_); \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _queue_);           \
    bool CMC_(PFX, _print)(struct SNAME * _queue_, FILE * fptr);               \
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
#define CMC_CMC_QUEUE_CORE_SOURCE_(PFX, SNAME, V)                              \
                                                                               \
    /* Implementation Detail Functions */                                      \
    /* None */                                                                 \
                                                                               \
    struct SNAME *CMC_(PFX, _new)(size_t capacity,                             \
                                  struct CMC_DEF_FVAL(SNAME) * f_val)          \
    {                                                                          \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;                \
                                                                               \
        if (capacity < 1)                                                      \
            return NULL;                                                       \
                                                                               \
        if (!f_val)                                                            \
            return NULL;                                                       \
                                                                               \
        struct SNAME *_queue_ = alloc->malloc(sizeof(struct SNAME));           \
                                                                               \
        if (!_queue_)                                                          \
            return NULL;                                                       \
                                                                               \
        _queue_->buffer = alloc->calloc(capacity, sizeof(V));                  \
                                                                               \
        if (!_queue_->buffer)                                                  \
        {                                                                      \
            alloc->free(_queue_);                                              \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _queue_->capacity = capacity;                                          \
        _queue_->count = 0;                                                    \
        _queue_->front = 0;                                                    \
        _queue_->back = 0;                                                     \
        _queue_->flag = CMC_FLAG_OK;                                           \
        _queue_->f_val = f_val;                                                \
        _queue_->alloc = alloc;                                                \
        _queue_->callbacks = NULL;                                             \
                                                                               \
        return _queue_;                                                        \
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
        struct SNAME *_queue_ = alloc->malloc(sizeof(struct SNAME));           \
                                                                               \
        if (!_queue_)                                                          \
            return NULL;                                                       \
                                                                               \
        _queue_->buffer = alloc->calloc(capacity, sizeof(V));                  \
                                                                               \
        if (!_queue_->buffer)                                                  \
        {                                                                      \
            alloc->free(_queue_);                                              \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _queue_->capacity = capacity;                                          \
        _queue_->count = 0;                                                    \
        _queue_->front = 0;                                                    \
        _queue_->back = 0;                                                     \
        _queue_->flag = CMC_FLAG_OK;                                           \
        _queue_->f_val = f_val;                                                \
        _queue_->alloc = alloc;                                                \
        _queue_->callbacks = callbacks;                                        \
                                                                               \
        return _queue_;                                                        \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _clear)(struct SNAME * _queue_)                             \
    {                                                                          \
        if (_queue_->f_val->free)                                              \
        {                                                                      \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)    \
            {                                                                  \
                _queue_->f_val->free(_queue_->buffer[i]);                      \
                                                                               \
                i = (i + 1) % _queue_->capacity;                               \
            }                                                                  \
        }                                                                      \
                                                                               \
        memset(_queue_->buffer, 0, sizeof(V) * _queue_->capacity);             \
                                                                               \
        _queue_->count = 0;                                                    \
        _queue_->front = 0;                                                    \
        _queue_->back = 0;                                                     \
        _queue_->flag = CMC_FLAG_OK;                                           \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _free)(struct SNAME * _queue_)                              \
    {                                                                          \
        if (_queue_->f_val->free)                                              \
        {                                                                      \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)    \
            {                                                                  \
                _queue_->f_val->free(_queue_->buffer[i]);                      \
                                                                               \
                i = (i + 1) % _queue_->capacity;                               \
            }                                                                  \
        }                                                                      \
                                                                               \
        _queue_->alloc->free(_queue_->buffer);                                 \
        _queue_->alloc->free(_queue_);                                         \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _customize)(struct SNAME * _queue_,                         \
                               struct cmc_alloc_node * alloc,                  \
                               struct cmc_callbacks * callbacks)               \
    {                                                                          \
        if (!alloc)                                                            \
            _queue_->alloc = &cmc_alloc_node_default;                          \
        else                                                                   \
            _queue_->alloc = alloc;                                            \
                                                                               \
        _queue_->callbacks = callbacks;                                        \
                                                                               \
        _queue_->flag = CMC_FLAG_OK;                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _enqueue)(struct SNAME * _queue_, V value)                  \
    {                                                                          \
        if (CMC_(PFX, _full)(_queue_))                                         \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_queue_, _queue_->capacity * 2))           \
                return false;                                                  \
        }                                                                      \
                                                                               \
        _queue_->buffer[_queue_->back] = value;                                \
                                                                               \
        _queue_->back =                                                        \
            (_queue_->back == _queue_->capacity - 1) ? 0 : _queue_->back + 1;  \
        _queue_->count++;                                                      \
        _queue_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_queue_->callbacks && _queue_->callbacks->create)                  \
            _queue_->callbacks->create();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _dequeue)(struct SNAME * _queue_)                           \
    {                                                                          \
        if (CMC_(PFX, _empty)(_queue_))                                        \
        {                                                                      \
            _queue_->flag = CMC_FLAG_EMPTY;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        _queue_->buffer[_queue_->front] = (V){ 0 };                            \
                                                                               \
        _queue_->front = (_queue_->front == _queue_->capacity - 1)             \
                             ? 0                                               \
                             : _queue_->front + 1;                             \
        _queue_->count--;                                                      \
        _queue_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_queue_->callbacks && _queue_->callbacks->delete)                  \
            _queue_->callbacks->delete ();                                     \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _peek)(struct SNAME * _queue_)                                 \
    {                                                                          \
        if (CMC_(PFX, _empty)(_queue_))                                        \
        {                                                                      \
            _queue_->flag = CMC_FLAG_EMPTY;                                    \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _queue_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_queue_->callbacks && _queue_->callbacks->read)                    \
            _queue_->callbacks->read();                                        \
                                                                               \
        return _queue_->buffer[_queue_->front];                                \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _contains)(struct SNAME * _queue_, V value)                 \
    {                                                                          \
        _queue_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        bool result = false;                                                   \
                                                                               \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)        \
        {                                                                      \
            if (_queue_->f_val->cmp(_queue_->buffer[i], value) == 0)           \
            {                                                                  \
                result = true;                                                 \
                break;                                                         \
            }                                                                  \
                                                                               \
            i = (i + 1) % _queue_->capacity;                                   \
        }                                                                      \
                                                                               \
        if (_queue_->callbacks && _queue_->callbacks->read)                    \
            _queue_->callbacks->read();                                        \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _empty)(struct SNAME * _queue_)                             \
    {                                                                          \
        return _queue_->count == 0;                                            \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _full)(struct SNAME * _queue_)                              \
    {                                                                          \
        return _queue_->count >= _queue_->capacity;                            \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _count)(struct SNAME * _queue_)                           \
    {                                                                          \
        return _queue_->count;                                                 \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _capacity)(struct SNAME * _queue_)                        \
    {                                                                          \
        return _queue_->capacity;                                              \
    }                                                                          \
                                                                               \
    int CMC_(PFX, _flag)(struct SNAME * _queue_)                               \
    {                                                                          \
        return _queue_->flag;                                                  \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _resize)(struct SNAME * _queue_, size_t capacity)           \
    {                                                                          \
        if (_queue_->capacity == capacity)                                     \
            goto success;                                                      \
                                                                               \
        if (capacity < _queue_->count)                                         \
        {                                                                      \
            _queue_->flag = CMC_FLAG_INVALID;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        V *new_buffer = _queue_->alloc->malloc(sizeof(V) * capacity);          \
                                                                               \
        if (!new_buffer)                                                       \
        {                                                                      \
            _queue_->flag = CMC_FLAG_ALLOC;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)        \
        {                                                                      \
            new_buffer[j] = _queue_->buffer[i];                                \
                                                                               \
            i = (i + 1) % _queue_->capacity;                                   \
        }                                                                      \
                                                                               \
        _queue_->alloc->free(_queue_->buffer);                                 \
                                                                               \
        _queue_->buffer = new_buffer;                                          \
        _queue_->capacity = capacity;                                          \
        _queue_->front = 0;                                                    \
        _queue_->back = _queue_->count;                                        \
                                                                               \
    success:                                                                   \
                                                                               \
        _queue_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_queue_->callbacks && _queue_->callbacks->resize)                  \
            _queue_->callbacks->resize();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _queue_)                  \
    {                                                                          \
        struct SNAME *result =                                                 \
            CMC_(PFX, _new_custom)(_queue_->capacity, _queue_->f_val,          \
                                   _queue_->alloc, _queue_->callbacks);        \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _queue_->flag = CMC_FLAG_ERROR;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (_queue_->f_val->cpy)                                               \
        {                                                                      \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)    \
            {                                                                  \
                result->buffer[j] = _queue_->f_val->cpy(_queue_->buffer[i]);   \
                                                                               \
                i = (i + 1) % _queue_->capacity;                               \
            }                                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)    \
            {                                                                  \
                result->buffer[j] = _queue_->buffer[i];                        \
                                                                               \
                i = (i + 1) % _queue_->capacity;                               \
            }                                                                  \
        }                                                                      \
                                                                               \
        result->count = _queue_->count;                                        \
        result->front = 0;                                                     \
        result->back = _queue_->count;                                         \
                                                                               \
        _queue_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _equals)(struct SNAME * _queue1_, struct SNAME * _queue2_)  \
    {                                                                          \
        _queue1_->flag = CMC_FLAG_OK;                                          \
        _queue2_->flag = CMC_FLAG_OK;                                          \
                                                                               \
        if (_queue1_->count != _queue2_->count)                                \
            return false;                                                      \
                                                                               \
        size_t i, j, k;                                                        \
        for (i = _queue1_->front, j = _queue2_->front, k = 0;                  \
             k < _queue1_->count; k++)                                         \
        {                                                                      \
            if (_queue1_->f_val->cmp(_queue1_->buffer[i],                      \
                                     _queue2_->buffer[j]) != 0)                \
                return false;                                                  \
                                                                               \
            i = (i + 1) % _queue1_->capacity;                                  \
            j = (j + 1) % _queue2_->capacity;                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _queue_)            \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *q_ = _queue_;                                            \
                                                                               \
        int n =                                                                \
            snprintf(str.s, cmc_string_len, cmc_cmc_string_fmt_queue, #SNAME,  \
                     #V, q_, q_->buffer, q_->capacity, q_->count, q_->front,   \
                     q_->back, q_->flag, q_->f_val, q_->alloc, q_->callbacks); \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _print)(struct SNAME * _queue_, FILE * fptr)                \
    {                                                                          \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)        \
        {                                                                      \
            if (!_queue_->f_val->str(fptr, _queue_->buffer[i]))                \
                return false;                                                  \
                                                                               \
            i = (i + 1) % _queue_->capacity;                                   \
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

#endif /* CMC_CMC_QUEUE_H */
