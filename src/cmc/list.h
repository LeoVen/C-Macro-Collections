/**
 * list.h
 *
 * Creation Date: 12/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * List
 *
 * A List is a Dynamic Array that store its elements contiguously. It has an
 * internal buffer that is pre-allocated with a greater capacity than necessary
 * in order to foresee new elements being added. The items can be added at both
 * ends of the list and at the middle (with a given index).
 *
 * Insertions at the start are more costly as it is necessary to shift all
 * elements one position to the right to give space to a new element being added
 * at index 0. Insertions at the end are instantaneous as long as there is
 * enough space in the buffer. Insertions at the middle will be based on an
 * index. The index represents where the element being added will be located
 * once the operation ends. This operation might also need to shift elements. It
 * is also possible to add arrays of elements in the list.
 *
 * When the buffer is filled, it is reallocated with a greater capacity, usually
 * being doubled.
 *
 * Removing elements follows the same principle. Removing the front element will
 * require to shift all other elements one position to the left, thus being
 * slower than removing from the end in which is done in constant time. Removing
 * elements in the middle of the list will also require shifting elements to the
 * left. Is is also possible to remove a range of elements or extract them,
 * creating a new list with the removed items.
 *
 * The iterator is a simple structure that is capable of going back and
 * forwards. Any modifications to the target list during iteration is considered
 * undefined behavior. Its sole purpose is to facilitate navigation through a
 * list.
 */

#ifndef CMC_LIST_H
#define CMC_LIST_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_list = "%s at %p { buffer:%p, capacity:%" PRIuMAX ", count:%" PRIuMAX " }";

#define CMC_GENERATE_LIST(PFX, SNAME, V)    \
    CMC_GENERATE_LIST_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_LIST_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_LIST_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_LIST_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_LIST_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_LIST_SOURCE(PFX, SNAME, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_LIST_HEADER(PFX, SNAME, V)                                               \
                                                                                              \
    /* List Structure */                                                                      \
    struct SNAME                                                                              \
    {                                                                                         \
        /* Dynamic array of elements */                                                       \
        V *buffer;                                                                            \
                                                                                              \
        /* Current array capacity */                                                          \
        size_t capacity;                                                                      \
                                                                                              \
        /* Current amount of elements */                                                      \
        size_t count;                                                                         \
                                                                                              \
        /* Function that returns an iterator to the start of the list */                      \
        struct SNAME##_iter (*it_start)(struct SNAME *);                                      \
                                                                                              \
        /* Function that returns an iterator to the end of the list */                        \
        struct SNAME##_iter (*it_end)(struct SNAME *);                                        \
    };                                                                                        \
                                                                                              \
    /* List Iterator */                                                                       \
    struct SNAME##_iter                                                                       \
    {                                                                                         \
        /* Target list */                                                                     \
        struct SNAME *target;                                                                 \
                                                                                              \
        /* Cursor's position (index) */                                                       \
        size_t cursor;                                                                        \
                                                                                              \
        /* If the iterator has reached the start of the iteration */                          \
        bool start;                                                                           \
                                                                                              \
        /* If the iterator has reached the end of the iteration */                            \
        bool end;                                                                             \
    };                                                                                        \
                                                                                              \
    /* Collection Functions */                                                                \
    /* Collection Allocation and Deallocation */                                              \
    struct SNAME *PFX##_new(size_t capacity);                                                 \
    struct SNAME *PFX##_new_from(V *elements, size_t size);                                   \
    void PFX##_clear(struct SNAME *_list_, void (*deallocator)(V));                           \
    void PFX##_free(struct SNAME *_list_, void (*deallocator)(V));                            \
    /* Collection Input and Output */                                                         \
    bool PFX##_push_front(struct SNAME *_list_, V element);                                   \
    bool PFX##_push_at(struct SNAME *_list_, V element, size_t index);                        \
    bool PFX##_push_back(struct SNAME *_list_, V element);                                    \
    bool PFX##_pop_front(struct SNAME *_list_);                                               \
    bool PFX##_pop_at(struct SNAME *_list_, size_t index);                                    \
    bool PFX##_pop_back(struct SNAME *_list_);                                                \
    /* Collection Sequence Input and Output */                                                \
    bool PFX##_seq_push_front(struct SNAME *_list_, V *elements, size_t size);                \
    bool PFX##_seq_push_at(struct SNAME *_list_, V *elements, size_t size, size_t index);     \
    bool PFX##_seq_push_back(struct SNAME *_list_, V *elements, size_t size);                 \
    bool PFX##_seq_pop_at(struct SNAME *_list_, size_t from, size_t to);                      \
    struct SNAME *PFX##_seq_sublist(struct SNAME *_list_, size_t from, size_t to);            \
    /* Element Access */                                                                      \
    V PFX##_front(struct SNAME *_list_);                                                      \
    V PFX##_get(struct SNAME *_list_, size_t index);                                          \
    V *PFX##_get_ref(struct SNAME *_list_, size_t index);                                     \
    V PFX##_back(struct SNAME *_list_);                                                       \
    size_t PFX##_indexof(struct SNAME *_list_, V element, int (*comparator)(V, V),            \
                         bool from_start);                                                    \
    /* Collection State */                                                                    \
    bool PFX##_contains(struct SNAME *_list_, V element, int (*comparator)(V, V));            \
    bool PFX##_empty(struct SNAME *_list_);                                                   \
    bool PFX##_full(struct SNAME *_list_);                                                    \
    size_t PFX##_count(struct SNAME *_list_);                                                 \
    bool PFX##_fits(struct SNAME *_list_, size_t size);                                       \
    size_t PFX##_capacity(struct SNAME *_list_);                                              \
    /* Collection Utility */                                                                  \
    bool PFX##_resize(struct SNAME *_list_, size_t capacity);                                 \
    struct SNAME *PFX##_copy_of(struct SNAME *_list_, V (*copy_func)(V));                     \
    bool PFX##_equals(struct SNAME *_list1_, struct SNAME *_list2_, int (*comparator)(V, V)); \
    struct cmc_string PFX##_to_string(struct SNAME *_list_);                                  \
                                                                                              \
    /* Iterator Functions */                                                                  \
    /* Iterator Allocation and Deallocation */                                                \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);                                \
    void PFX##_iter_free(struct SNAME##_iter *iter);                                          \
    /* Iterator Initialization */                                                             \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);                    \
    /* Iterator State */                                                                      \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                                         \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                                           \
    /* Iterator Movement */                                                                   \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                                      \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                                        \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                                          \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                                          \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);                         \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);                          \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);                           \
    /* Iterator Access */                                                                     \
    V PFX##_iter_value(struct SNAME##_iter *iter);                                            \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter);                                          \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);                                       \
                                                                                              \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_LIST_SOURCE(PFX, SNAME, V)                                              \
                                                                                             \
    /* Implementation Detail Functions */                                                    \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_list_);                    \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_list_);                      \
                                                                                             \
    struct SNAME *PFX##_new(size_t capacity)                                                 \
    {                                                                                        \
        if (capacity < 1)                                                                    \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME *_list_ = malloc(sizeof(struct SNAME));                                 \
                                                                                             \
        if (!_list_)                                                                         \
            return NULL;                                                                     \
                                                                                             \
        _list_->buffer = calloc(capacity, sizeof(V));                                        \
                                                                                             \
        if (!_list_->buffer)                                                                 \
        {                                                                                    \
            free(_list_);                                                                    \
            return NULL;                                                                     \
        }                                                                                    \
                                                                                             \
        _list_->capacity = capacity;                                                         \
        _list_->count = 0;                                                                   \
                                                                                             \
        _list_->it_start = PFX##_impl_it_start;                                              \
        _list_->it_end = PFX##_impl_it_end;                                                  \
                                                                                             \
        return _list_;                                                                       \
    }                                                                                        \
                                                                                             \
    struct SNAME *PFX##_new_from(V *elements, size_t size)                                   \
    {                                                                                        \
        if (size == 0)                                                                       \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME *_list_ = PFX##_new(size + size / 2);                                   \
                                                                                             \
        if (!_list_)                                                                         \
            return NULL;                                                                     \
                                                                                             \
        memcpy(_list_->buffer, elements, size * sizeof(V));                                  \
                                                                                             \
        _list_->count = size;                                                                \
                                                                                             \
        return _list_;                                                                       \
    }                                                                                        \
                                                                                             \
    void PFX##_clear(struct SNAME *_list_, void (*deallocator)(V))                           \
    {                                                                                        \
        if (deallocator)                                                                     \
        {                                                                                    \
            for (size_t i = 0; i < _list_->count; i++)                                       \
                deallocator(_list_->buffer[i]);                                              \
        }                                                                                    \
                                                                                             \
        memset(_list_->buffer, 0, sizeof(V) * _list_->capacity);                             \
                                                                                             \
        _list_->count = 0;                                                                   \
    }                                                                                        \
                                                                                             \
    void PFX##_free(struct SNAME *_list_, void (*deallocator)(V))                            \
    {                                                                                        \
        if (deallocator)                                                                     \
        {                                                                                    \
            for (size_t i = 0; i < _list_->count; i++)                                       \
                deallocator(_list_->buffer[i]);                                              \
        }                                                                                    \
                                                                                             \
        free(_list_->buffer);                                                                \
        free(_list_);                                                                        \
    }                                                                                        \
                                                                                             \
    bool PFX##_push_front(struct SNAME *_list_, V element)                                   \
    {                                                                                        \
        if (PFX##_full(_list_))                                                              \
        {                                                                                    \
            if (!PFX##_resize(_list_, PFX##_count(_list_) * 2))                              \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        if (!PFX##_empty(_list_))                                                            \
        {                                                                                    \
            memmove(_list_->buffer + 1, _list_->buffer, _list_->count * sizeof(V));          \
        }                                                                                    \
                                                                                             \
        _list_->buffer[0] = element;                                                         \
                                                                                             \
        _list_->count++;                                                                     \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_push_at(struct SNAME *_list_, V element, size_t index)                        \
    {                                                                                        \
        if (index > _list_->count)                                                           \
            return false;                                                                    \
                                                                                             \
        if (PFX##_full(_list_))                                                              \
        {                                                                                    \
            if (!PFX##_resize(_list_, PFX##_count(_list_) * 2))                              \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        memmove(_list_->buffer + index + 1, _list_->buffer + index,                          \
                (_list_->count - index) * sizeof(V));                                        \
                                                                                             \
        _list_->buffer[index] = element;                                                     \
        _list_->count++;                                                                     \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_push_back(struct SNAME *_list_, V element)                                    \
    {                                                                                        \
        if (PFX##_full(_list_))                                                              \
        {                                                                                    \
            if (!PFX##_resize(_list_, PFX##_count(_list_) * 2))                              \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        _list_->buffer[_list_->count++] = element;                                           \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_pop_front(struct SNAME *_list_)                                               \
    {                                                                                        \
        if (PFX##_empty(_list_))                                                             \
            return false;                                                                    \
                                                                                             \
        memmove(_list_->buffer, _list_->buffer + 1, _list_->count * sizeof(V));              \
                                                                                             \
        _list_->buffer[--_list_->count] = (V){0};                                            \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_pop_at(struct SNAME *_list_, size_t index)                                    \
    {                                                                                        \
        if (index >= _list_->count)                                                          \
            return false;                                                                    \
                                                                                             \
        memmove(_list_->buffer + index, _list_->buffer + index + 1,                          \
                (_list_->count - index) * sizeof(V));                                        \
                                                                                             \
        _list_->buffer[--_list_->count] = (V){0};                                            \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_pop_back(struct SNAME *_list_)                                                \
    {                                                                                        \
        if (PFX##_empty(_list_))                                                             \
            return false;                                                                    \
                                                                                             \
        _list_->buffer[--_list_->count] = (V){0};                                            \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_seq_push_front(struct SNAME *_list_, V *elements, size_t size)                \
    {                                                                                        \
        if (size == 0)                                                                       \
            return false;                                                                    \
                                                                                             \
        if (!PFX##_fits(_list_, size))                                                       \
        {                                                                                    \
            if (!PFX##_resize(_list_, PFX##_count(_list_) + size))                           \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        memmove(_list_->buffer + size, _list_->buffer, _list_->count * sizeof(V));           \
                                                                                             \
        memcpy(_list_->buffer, elements, size * sizeof(V));                                  \
                                                                                             \
        _list_->count += size;                                                               \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_seq_push_at(struct SNAME *_list_, V *elements, size_t size, size_t index)     \
    {                                                                                        \
        if (size == 0 || index > _list_->count)                                              \
            return false;                                                                    \
                                                                                             \
        if (index == 0)                                                                      \
            return PFX##_seq_push_front(_list_, elements, size);                             \
        else if (index == _list_->count)                                                     \
            return PFX##_seq_push_back(_list_, elements, size);                              \
        else                                                                                 \
        {                                                                                    \
            if (!PFX##_fits(_list_, size))                                                   \
            {                                                                                \
                if (!PFX##_resize(_list_, PFX##_count(_list_) + size))                       \
                    return false;                                                            \
            }                                                                                \
                                                                                             \
            memmove(_list_->buffer + index + size, _list_->buffer + index,                   \
                    (_list_->count - index) * sizeof(V));                                    \
                                                                                             \
            memcpy(_list_->buffer + index, elements, size * sizeof(V));                      \
                                                                                             \
            _list_->count += size;                                                           \
        }                                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_seq_push_back(struct SNAME *_list_, V *elements, size_t size)                 \
    {                                                                                        \
        if (size == 0)                                                                       \
            return false;                                                                    \
                                                                                             \
        if (!PFX##_fits(_list_, size))                                                       \
        {                                                                                    \
            if (!PFX##_resize(_list_, _list_->count + size))                                 \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        memcpy(_list_->buffer + _list_->count, elements, size * sizeof(V));                  \
                                                                                             \
        _list_->count += size;                                                               \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_seq_pop_at(struct SNAME *_list_, size_t from, size_t to)                      \
    {                                                                                        \
        if (from > to || to >= _list_->count)                                                \
            return false;                                                                    \
                                                                                             \
        size_t length = (to - from + 1);                                                     \
                                                                                             \
        memmove(_list_->buffer + from, _list_->buffer + to + 1,                              \
                (_list_->count - to - 1) * sizeof(V));                                       \
                                                                                             \
        memset(_list_->buffer + _list_->count - length, 0, length * sizeof(V));              \
                                                                                             \
        _list_->count -= to - from + 1;                                                      \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    struct SNAME *PFX##_seq_sublist(struct SNAME *_list_, size_t from, size_t to)            \
    {                                                                                        \
        if (from > to || to >= _list_->count)                                                \
            return false;                                                                    \
                                                                                             \
        size_t length = to - from + 1;                                                       \
                                                                                             \
        struct SNAME *result = PFX##_new(length);                                            \
                                                                                             \
        if (!result)                                                                         \
            return NULL;                                                                     \
                                                                                             \
        memcpy(result->buffer, _list_->buffer, _list_->count * sizeof(V));                   \
                                                                                             \
        memmove(_list_->buffer + from, _list_->buffer + to + 1,                              \
                (_list_->count - to - 1) * sizeof(V));                                       \
                                                                                             \
        memset(_list_->buffer + _list_->count - length, 0, length * sizeof(V));              \
                                                                                             \
        _list_->count -= length;                                                             \
        result->count = length;                                                              \
                                                                                             \
        return result;                                                                       \
    }                                                                                        \
                                                                                             \
    V PFX##_front(struct SNAME *_list_)                                                      \
    {                                                                                        \
        if (PFX##_empty(_list_))                                                             \
            return (V){0};                                                                   \
                                                                                             \
        return _list_->buffer[0];                                                            \
    }                                                                                        \
                                                                                             \
    V PFX##_get(struct SNAME *_list_, size_t index)                                          \
    {                                                                                        \
        if (index >= _list_->count || PFX##_empty(_list_))                                   \
            return (V){0};                                                                   \
                                                                                             \
        return _list_->buffer[index];                                                        \
    }                                                                                        \
                                                                                             \
    V *PFX##_get_ref(struct SNAME *_list_, size_t index)                                     \
    {                                                                                        \
        if (index >= _list_->count)                                                          \
            return NULL;                                                                     \
                                                                                             \
        if (PFX##_empty(_list_))                                                             \
            return NULL;                                                                     \
                                                                                             \
        return &(_list_->buffer[index]);                                                     \
    }                                                                                        \
                                                                                             \
    V PFX##_back(struct SNAME *_list_)                                                       \
    {                                                                                        \
        if (PFX##_empty(_list_))                                                             \
            return (V){0};                                                                   \
                                                                                             \
        return _list_->buffer[_list_->count - 1];                                            \
    }                                                                                        \
                                                                                             \
    size_t PFX##_indexof(struct SNAME *_list_, V element, int (*comparator)(V, V),           \
                         bool from_start)                                                    \
    {                                                                                        \
        if (from_start)                                                                      \
        {                                                                                    \
            for (size_t i = 0; i < _list_->count; i++)                                       \
            {                                                                                \
                if (comparator(_list_->buffer[i], element) == 0)                             \
                    return i;                                                                \
            }                                                                                \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            for (size_t i = _list_->count; i > 0; i--)                                       \
            {                                                                                \
                if (comparator(_list_->buffer[i - 1], element) == 0)                         \
                    return i - 1;                                                            \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        return _list_->count;                                                                \
    }                                                                                        \
                                                                                             \
    bool PFX##_contains(struct SNAME *_list_, V element, int (*comparator)(V, V))            \
    {                                                                                        \
        for (size_t i = 0; i < _list_->count; i++)                                           \
        {                                                                                    \
            if (comparator(_list_->buffer[i], element) == 0)                                 \
                return true;                                                                 \
        }                                                                                    \
                                                                                             \
        return false;                                                                        \
    }                                                                                        \
                                                                                             \
    bool PFX##_empty(struct SNAME *_list_)                                                   \
    {                                                                                        \
        return _list_->count == 0;                                                           \
    }                                                                                        \
                                                                                             \
    bool PFX##_full(struct SNAME *_list_)                                                    \
    {                                                                                        \
        return _list_->count >= _list_->capacity;                                            \
    }                                                                                        \
                                                                                             \
    size_t PFX##_count(struct SNAME *_list_)                                                 \
    {                                                                                        \
        return _list_->count;                                                                \
    }                                                                                        \
                                                                                             \
    bool PFX##_fits(struct SNAME *_list_, size_t size)                                       \
    {                                                                                        \
        return _list_->count + size <= _list_->capacity;                                     \
    }                                                                                        \
                                                                                             \
    size_t PFX##_capacity(struct SNAME *_list_)                                              \
    {                                                                                        \
        return _list_->capacity;                                                             \
    }                                                                                        \
                                                                                             \
    bool PFX##_resize(struct SNAME *_list_, size_t capacity)                                 \
    {                                                                                        \
        if (PFX##_capacity(_list_) == capacity)                                              \
            return true;                                                                     \
                                                                                             \
        if (capacity < PFX##_count(_list_))                                                  \
            return false;                                                                    \
                                                                                             \
        V *new_buffer = realloc(_list_->buffer, sizeof(V) * capacity);                       \
                                                                                             \
        if (!new_buffer)                                                                     \
            return false;                                                                    \
                                                                                             \
        _list_->buffer = new_buffer;                                                         \
        _list_->capacity = capacity;                                                         \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    struct SNAME *PFX##_copy_of(struct SNAME *_list_, V (*copy_func)(V))                     \
    {                                                                                        \
        struct SNAME *result = PFX##_new(_list_->capacity);                                  \
                                                                                             \
        if (!result)                                                                         \
            return NULL;                                                                     \
                                                                                             \
        if (copy_func)                                                                       \
        {                                                                                    \
            for (size_t i = 0; i < _list_->count; i++)                                       \
                result->buffer[i] = copy_func(_list_->buffer[i]);                            \
        }                                                                                    \
        else                                                                                 \
            memcpy(result->buffer, _list_->buffer, sizeof(V) * _list_->count);               \
                                                                                             \
        result->count = _list_->count;                                                       \
                                                                                             \
        return result;                                                                       \
    }                                                                                        \
                                                                                             \
    bool PFX##_equals(struct SNAME *_list1_, struct SNAME *_list2_, int (*comparator)(V, V)) \
    {                                                                                        \
        if (PFX##_count(_list1_) != PFX##_count(_list2_))                                    \
            return false;                                                                    \
                                                                                             \
        for (size_t i = 0; i < PFX##_count(_list1_); i++)                                    \
        {                                                                                    \
            if (comparator(_list1_->buffer[i], _list2_->buffer[i]) != 0)                     \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        return false;                                                                        \
    }                                                                                        \
                                                                                             \
    struct cmc_string PFX##_to_string(struct SNAME *_list_)                                  \
    {                                                                                        \
        struct cmc_string str;                                                               \
        struct SNAME *l_ = _list_;                                                           \
        const char *name = #SNAME;                                                           \
                                                                                             \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_list,                                 \
                 name, l_, l_->buffer, l_->capacity, l_->count);                             \
                                                                                             \
        return str;                                                                          \
    }                                                                                        \
                                                                                             \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                                \
    {                                                                                        \
        struct SNAME##_iter *iter = malloc(sizeof(struct SNAME##_iter));                     \
                                                                                             \
        if (!iter)                                                                           \
            return NULL;                                                                     \
                                                                                             \
        PFX##_iter_init(iter, target);                                                       \
                                                                                             \
        return iter;                                                                         \
    }                                                                                        \
                                                                                             \
    void PFX##_iter_free(struct SNAME##_iter *iter)                                          \
    {                                                                                        \
        free(iter);                                                                          \
    }                                                                                        \
                                                                                             \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)                    \
    {                                                                                        \
        iter->target = target;                                                               \
        iter->cursor = 0;                                                                    \
        iter->start = true;                                                                  \
        iter->end = PFX##_empty(target);                                                     \
    }                                                                                        \
                                                                                             \
    bool PFX##_iter_start(struct SNAME##_iter *iter)                                         \
    {                                                                                        \
        return PFX##_empty(iter->target) || iter->start;                                     \
    }                                                                                        \
                                                                                             \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                                           \
    {                                                                                        \
        return PFX##_empty(iter->target) || iter->end;                                       \
    }                                                                                        \
                                                                                             \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                                      \
    {                                                                                        \
        if (!PFX##_empty(iter->target))                                                      \
        {                                                                                    \
            iter->cursor = 0;                                                                \
            iter->start = true;                                                              \
            iter->end = PFX##_empty(iter->target);                                           \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                                        \
    {                                                                                        \
        if (!PFX##_empty(iter->target))                                                      \
        {                                                                                    \
            iter->start = PFX##_empty(iter->target);                                         \
            iter->cursor = PFX##_empty(iter->target) ? 0 : iter->target->count - 1;          \
            iter->end = true;                                                                \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                                          \
    {                                                                                        \
        if (iter->end)                                                                       \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor + 1 == PFX##_count(iter->target))                                   \
        {                                                                                    \
            iter->end = true;                                                                \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        iter->start = PFX##_empty(iter->target);                                             \
                                                                                             \
        iter->cursor++;                                                                      \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                                          \
    {                                                                                        \
        if (iter->start)                                                                     \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor == 0)                                                               \
        {                                                                                    \
            iter->start = true;                                                              \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        iter->end = PFX##_empty(iter->target);                                               \
                                                                                             \
        iter->cursor--;                                                                      \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    /* Returns true only if the iterator moved */                                            \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)                         \
    {                                                                                        \
        if (iter->end)                                                                       \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor + 1 == iter->target->count)                                         \
        {                                                                                    \
            iter->end = true;                                                                \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        if (steps == 0 || iter->cursor + steps >= PFX##_count(iter->target))                 \
            return false;                                                                    \
                                                                                             \
        iter->start = PFX##_empty(iter->target);                                             \
                                                                                             \
        iter->cursor += steps;                                                               \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    /* Returns true only if the iterator moved */                                            \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps)                          \
    {                                                                                        \
        if (iter->start)                                                                     \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor == 0)                                                               \
        {                                                                                    \
            iter->start = true;                                                              \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        if (steps == 0 || iter->cursor < steps)                                              \
            return false;                                                                    \
                                                                                             \
        iter->end = PFX##_empty(iter->target);                                               \
                                                                                             \
        iter->cursor -= steps;                                                               \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    /* Returns true only if the iterator was able to be positioned at the given index */     \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)                           \
    {                                                                                        \
        if (index >= PFX##_count(iter->target))                                              \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor > index)                                                            \
            return PFX##_iter_rewind(iter, iter->cursor - index);                            \
        else if (iter->cursor < index)                                                       \
            return PFX##_iter_advance(iter, index - iter->cursor);                           \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    V PFX##_iter_value(struct SNAME##_iter *iter)                                            \
    {                                                                                        \
        if (PFX##_empty(iter->target))                                                       \
            return (V){0};                                                                   \
                                                                                             \
        return iter->target->buffer[iter->cursor];                                           \
    }                                                                                        \
                                                                                             \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter)                                          \
    {                                                                                        \
        if (PFX##_empty(iter->target))                                                       \
            return NULL;                                                                     \
                                                                                             \
        return &(iter->target->buffer[iter->cursor]);                                        \
    }                                                                                        \
                                                                                             \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                                       \
    {                                                                                        \
        return iter->cursor;                                                                 \
    }                                                                                        \
                                                                                             \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_list_)                     \
    {                                                                                        \
        struct SNAME##_iter iter;                                                            \
                                                                                             \
        PFX##_iter_init(&iter, _list_);                                                      \
        PFX##_iter_to_start(&iter);                                                          \
                                                                                             \
        return iter;                                                                         \
    }                                                                                        \
                                                                                             \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_list_)                       \
    {                                                                                        \
        struct SNAME##_iter iter;                                                            \
                                                                                             \
        PFX##_iter_init(&iter, _list_);                                                      \
        PFX##_iter_to_end(&iter);                                                            \
                                                                                             \
        return iter;                                                                         \
    }

#endif /* CMC_LIST_H */
