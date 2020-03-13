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

#ifndef CMC_QUEUE_H
#define CMC_QUEUE_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#ifndef CMC_CORE_H
#define CMC_CORE_H

/**
 * struct cmc_string
 *
 * Used by all collections when calling the to_string function.
 */
struct cmc_string
{
    char s[400];
};

static const size_t cmc_string_len = 400;

/**
 * struct cmc_alloc_node
 *
 * Custom allocation node. Allows collections to use custom allocation
 * functions.
 */
static struct cmc_alloc_node
{
    void *(*malloc)(size_t);
    void *(*calloc)(size_t, size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
} cmc_alloc_node_default = { malloc, calloc, realloc, free };

/**
 * enum cmc_flags
 *
 * Defines common error codes used by all collections. These are flags that
 * indicate if something went wrong in the last operation by the collection.
 */
static struct
{
    int OK;        // No errors
    int ALLOC;     // Allocation failed
    int EMPTY;     // The collection is empty when it should not
    int NOT_FOUND; // Key or value not found
    int INVALID;   // Invalid argument or operation
    int RANGE;     // Index out of range
    int DUPLICATE; // Duplicate key or value
    int ERROR;     // Generic error, usually caused by algorithm error
} cmc_flags = { 0, 1, 2, 3, 4, 5, 6, 7 };

#endif /* CMC_CORE_H */

/* -------------------------------------------------------------------------
 * Queue specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_string_fmt_queue = "struct %s<%s> "
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
 * Custom Queue callbacks.
 *
 * There are two types of callbacks, 'before' and 'after':
 *      <before|after>_<function_name>
 */
struct cmc_callbacks_queue
{
    void (*before_clear)(void *);
    void (*after_clear)(void *);
    void (*before_free)(void *);
    void (*after_free)(void *);
    // TODO implement all callbacks
};

#define CMC_GENERATE_QUEUE(PFX, SNAME, V)    \
    CMC_GENERATE_QUEUE_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_QUEUE_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_QUEUE_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_QUEUE_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_QUEUE_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_QUEUE_SOURCE(PFX, SNAME, V)

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_QUEUE_HEADER(PFX, SNAME, V)                              \
                                                                              \
    /* Queue Structure */                                                     \
    struct SNAME                                                              \
    {                                                                         \
        /* Dynamic circular array of elements */                              \
        V *buffer;                                                            \
                                                                              \
        /* Current circular array capacity */                                 \
        size_t capacity;                                                      \
                                                                              \
        /* Current amount of elements */                                      \
        size_t count;                                                         \
                                                                              \
        /* Index representing the front of the queue */                       \
        size_t front;                                                         \
                                                                              \
        /* Index representing the back of the queue */                        \
        size_t back;                                                          \
                                                                              \
        /* Flags indicating errors or success */                              \
        int flag;                                                             \
                                                                              \
        /* Value function table */                                            \
        struct SNAME##_ftab_val *f_val;                                       \
                                                                              \
        /* Custom allocation functions */                                     \
        struct cmc_alloc_node *alloc;                                         \
                                                                              \
        /* Custom callback functions */                                       \
        struct cmc_callbacks_queue *callbacks;                                \
                                                                              \
        /* Function that returns an iterator to the start of the queue */     \
        struct SNAME##_iter (*it_start)(struct SNAME *);                      \
                                                                              \
        /* Function that returns an iterator to the end of the queue */       \
        struct SNAME##_iter (*it_end)(struct SNAME *);                        \
    };                                                                        \
                                                                              \
    /* Value struct function table */                                         \
    struct SNAME##_ftab_val                                                   \
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
    /* Queue Iterator */                                                      \
    struct SNAME##_iter                                                       \
    {                                                                         \
        /* Target queue */                                                    \
        struct SNAME *target;                                                 \
                                                                              \
        /* Cursor's position (index) */                                       \
        size_t cursor;                                                        \
                                                                              \
        /* Keeps track of relative index to the iteration of elements */      \
        size_t index;                                                         \
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
    struct SNAME *PFX##_new(size_t capacity, struct SNAME##_ftab_val *f_val); \
    struct SNAME *PFX##_new_custom(                                           \
        size_t capacity, struct SNAME##_ftab_val *f_val,                      \
        struct cmc_alloc_node *alloc, struct cmc_callbacks_queue *callbacks); \
    void PFX##_clear(struct SNAME *_queue_);                                  \
    void PFX##_free(struct SNAME *_queue_);                                   \
    /* Customization of Allocation and Callbacks */                           \
    void PFX##_customize(struct SNAME *_queue_, struct cmc_alloc_node *alloc, \
                         struct cmc_callbacks_queue *callbacks);              \
    /* Collection Input and Output */                                         \
    bool PFX##_enqueue(struct SNAME *_queue_, V element);                     \
    bool PFX##_dequeue(struct SNAME *_queue_);                                \
    /* Element Access */                                                      \
    V PFX##_peek(struct SNAME *_queue_);                                      \
    /* Collection State */                                                    \
    bool PFX##_contains(struct SNAME *_queue_, V element);                    \
    bool PFX##_empty(struct SNAME *_queue_);                                  \
    bool PFX##_full(struct SNAME *_queue_);                                   \
    size_t PFX##_count(struct SNAME *_queue_);                                \
    size_t PFX##_capacity(struct SNAME *_queue_);                             \
    int PFX##_flag(struct SNAME *_queue_);                                    \
    /* Collection Utility */                                                  \
    bool PFX##_resize(struct SNAME *_queue_, size_t capacity);                \
    struct SNAME *PFX##_copy_of(struct SNAME *_queue_);                       \
    bool PFX##_equals(struct SNAME *_queue1_, struct SNAME *_queue2_);        \
    struct cmc_string PFX##_to_string(struct SNAME *_queue_);                 \
                                                                              \
    /* Iterator Functions */                                                  \
    /* Iterator Allocation and Deallocation */                                \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);                \
    void PFX##_iter_free(struct SNAME##_iter *iter);                          \
    /* Iterator Initialization */                                             \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);    \
    /* Iterator State */                                                      \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                         \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                           \
    /* Iterator Movement */                                                   \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                      \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                        \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                          \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                          \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);         \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);          \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);           \
    /* Iterator Access */                                                     \
    V PFX##_iter_value(struct SNAME##_iter *iter);                            \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter);                          \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_QUEUE_SOURCE(PFX, SNAME, V)                               \
                                                                               \
    /* Implementation Detail Functions */                                      \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_queue_);     \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_queue_);       \
                                                                               \
    struct SNAME *PFX##_new(size_t capacity, struct SNAME##_ftab_val *f_val)   \
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
        _queue_->flag = cmc_flags.OK;                                          \
        _queue_->f_val = f_val;                                                \
        _queue_->alloc = alloc;                                                \
        _queue_->callbacks = NULL;                                             \
        _queue_->it_start = PFX##_impl_it_start;                               \
        _queue_->it_end = PFX##_impl_it_end;                                   \
                                                                               \
        return _queue_;                                                        \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_new_custom(                                            \
        size_t capacity, struct SNAME##_ftab_val *f_val,                       \
        struct cmc_alloc_node *alloc, struct cmc_callbacks_queue *callbacks)   \
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
        _queue_->flag = cmc_flags.OK;                                          \
        _queue_->f_val = f_val;                                                \
        _queue_->alloc = alloc;                                                \
        _queue_->callbacks = callbacks;                                        \
        _queue_->it_start = PFX##_impl_it_start;                               \
        _queue_->it_end = PFX##_impl_it_end;                                   \
                                                                               \
        return _queue_;                                                        \
    }                                                                          \
                                                                               \
    void PFX##_clear(struct SNAME *_queue_)                                    \
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
        _queue_->flag = cmc_flags.OK;                                          \
    }                                                                          \
                                                                               \
    void PFX##_free(struct SNAME *_queue_)                                     \
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
    void PFX##_customize(struct SNAME *_queue_, struct cmc_alloc_node *alloc,  \
                         struct cmc_callbacks_queue *callbacks)                \
    {                                                                          \
        if (alloc)                                                             \
            _queue_->alloc = alloc;                                            \
                                                                               \
        if (callbacks)                                                         \
            _queue_->callbacks = callbacks;                                    \
                                                                               \
        _queue_->flag = cmc_flags.OK;                                          \
    }                                                                          \
                                                                               \
    bool PFX##_enqueue(struct SNAME *_queue_, V element)                       \
    {                                                                          \
        if (PFX##_full(_queue_))                                               \
        {                                                                      \
            if (!PFX##_resize(_queue_, _queue_->capacity * 2))                 \
                return false;                                                  \
        }                                                                      \
                                                                               \
        _queue_->buffer[_queue_->back] = element;                              \
                                                                               \
        _queue_->back =                                                        \
            (_queue_->back == _queue_->capacity - 1) ? 0 : _queue_->back + 1;  \
        _queue_->count++;                                                      \
        _queue_->flag = cmc_flags.OK;                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_dequeue(struct SNAME *_queue_)                                  \
    {                                                                          \
        if (PFX##_empty(_queue_))                                              \
        {                                                                      \
            _queue_->flag = cmc_flags.EMPTY;                                   \
            return false;                                                      \
        }                                                                      \
                                                                               \
        _queue_->buffer[_queue_->front] = (V){ 0 };                            \
                                                                               \
        _queue_->front = (_queue_->front == _queue_->capacity - 1)             \
                             ? 0                                               \
                             : _queue_->front + 1;                             \
        _queue_->count--;                                                      \
        _queue_->flag = cmc_flags.OK;                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    V PFX##_peek(struct SNAME *_queue_)                                        \
    {                                                                          \
        if (PFX##_empty(_queue_))                                              \
        {                                                                      \
            _queue_->flag = cmc_flags.EMPTY;                                   \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _queue_->flag = cmc_flags.OK;                                          \
                                                                               \
        return _queue_->buffer[_queue_->front];                                \
    }                                                                          \
                                                                               \
    bool PFX##_contains(struct SNAME *_queue_, V element)                      \
    {                                                                          \
        _queue_->flag = cmc_flags.OK;                                          \
                                                                               \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)        \
        {                                                                      \
            if (_queue_->f_val->cmp(_queue_->buffer[i], element) == 0)         \
                return true;                                                   \
                                                                               \
            i = (i + 1) % _queue_->capacity;                                   \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool PFX##_empty(struct SNAME *_queue_)                                    \
    {                                                                          \
        return _queue_->count == 0;                                            \
    }                                                                          \
                                                                               \
    bool PFX##_full(struct SNAME *_queue_)                                     \
    {                                                                          \
        return _queue_->count >= _queue_->capacity;                            \
    }                                                                          \
                                                                               \
    size_t PFX##_count(struct SNAME *_queue_)                                  \
    {                                                                          \
        return _queue_->count;                                                 \
    }                                                                          \
                                                                               \
    size_t PFX##_capacity(struct SNAME *_queue_)                               \
    {                                                                          \
        return _queue_->capacity;                                              \
    }                                                                          \
                                                                               \
    int PFX##_flag(struct SNAME *_queue_)                                      \
    {                                                                          \
        return _queue_->flag;                                                  \
    }                                                                          \
                                                                               \
    bool PFX##_resize(struct SNAME *_queue_, size_t capacity)                  \
    {                                                                          \
        _queue_->flag = cmc_flags.OK;                                          \
                                                                               \
        if (_queue_->capacity == capacity)                                     \
            return true;                                                       \
                                                                               \
        if (capacity < _queue_->count)                                         \
        {                                                                      \
            _queue_->flag = cmc_flags.INVALID;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        V *new_buffer = _queue_->alloc->malloc(sizeof(V) * capacity);          \
                                                                               \
        if (!new_buffer)                                                       \
        {                                                                      \
            _queue_->flag = cmc_flags.ALLOC;                                   \
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
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_copy_of(struct SNAME *_queue_)                         \
    {                                                                          \
        struct SNAME *result =                                                 \
            PFX##_new_custom(_queue_->capacity, _queue_->f_val,                \
                             _queue_->alloc, _queue_->callbacks);              \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _queue_->flag = cmc_flags.ERROR;                                   \
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
        _queue_->flag = cmc_flags.OK;                                          \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_equals(struct SNAME *_queue1_, struct SNAME *_queue2_)          \
    {                                                                          \
        _queue1_->flag = cmc_flags.OK;                                         \
        _queue2_->flag = cmc_flags.OK;                                         \
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
    struct cmc_string PFX##_to_string(struct SNAME *_queue_)                   \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *q_ = _queue_;                                            \
                                                                               \
        int n =                                                                \
            snprintf(str.s, cmc_string_len, cmc_string_fmt_queue, #SNAME, #V,  \
                     q_, q_->buffer, q_->capacity, q_->count, q_->front,       \
                     q_->back, q_->flag, q_->f_val, q_->alloc, q_->callbacks); \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                  \
    {                                                                          \
        struct SNAME##_iter *iter =                                            \
            target->alloc->malloc(sizeof(struct SNAME##_iter));                \
                                                                               \
        if (!iter)                                                             \
            return NULL;                                                       \
                                                                               \
        PFX##_iter_init(iter, target);                                         \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    void PFX##_iter_free(struct SNAME##_iter *iter)                            \
    {                                                                          \
        iter->target->alloc->free(iter);                                       \
    }                                                                          \
                                                                               \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)      \
    {                                                                          \
        iter->target = target;                                                 \
        iter->cursor = target->front;                                          \
        iter->index = 0;                                                       \
        iter->start = true;                                                    \
        iter->end = PFX##_empty(target);                                       \
    }                                                                          \
                                                                               \
    bool PFX##_iter_start(struct SNAME##_iter *iter)                           \
    {                                                                          \
        return PFX##_empty(iter->target) || iter->start;                       \
    }                                                                          \
                                                                               \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                             \
    {                                                                          \
        return PFX##_empty(iter->target) || iter->end;                         \
    }                                                                          \
                                                                               \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                        \
    {                                                                          \
        iter->cursor = iter->target->front;                                    \
        iter->index = 0;                                                       \
        iter->start = true;                                                    \
        iter->end = PFX##_empty(iter->target);                                 \
    }                                                                          \
                                                                               \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                          \
    {                                                                          \
        if (PFX##_empty(iter->target))                                         \
            iter->cursor = 0;                                                  \
        else                                                                   \
        {                                                                      \
            if (iter->target->back == 0)                                       \
                iter->cursor = iter->target->capacity - 1;                     \
            else                                                               \
                iter->cursor = iter->target->back - 1;                         \
        }                                                                      \
                                                                               \
        iter->index = iter->target->count - 1;                                 \
        iter->start = PFX##_empty(iter->target);                               \
        iter->end = true;                                                      \
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
        iter->cursor = (iter->cursor + 1) % (iter->target->capacity);          \
        iter->index++;                                                         \
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
        iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1        \
                                           : iter->cursor - 1;                 \
        iter->index--;                                                         \
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
        iter->start = PFX##_empty(iter->target);                               \
                                                                               \
        iter->index += steps;                                                  \
        iter->cursor = (iter->cursor + steps) % iter->target->capacity;        \
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
        iter->end = PFX##_empty(iter->target);                                 \
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
        return iter->target->buffer[iter->cursor];                             \
    }                                                                          \
                                                                               \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter)                            \
    {                                                                          \
        if (PFX##_empty(iter->target))                                         \
            return NULL;                                                       \
                                                                               \
        return &(iter->target->buffer[iter->cursor]);                          \
    }                                                                          \
                                                                               \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                         \
    {                                                                          \
        return iter->index;                                                    \
    }                                                                          \
                                                                               \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_queue_)      \
    {                                                                          \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _queue_);                                       \
        PFX##_iter_to_start(&iter);                                            \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_queue_)        \
    {                                                                          \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _queue_);                                       \
        PFX##_iter_to_end(&iter);                                              \
                                                                               \
        return iter;                                                           \
    }

#endif /* CMC_QUEUE_H */
