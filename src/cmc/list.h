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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

#define LIST_GENERATE(PFX, SNAME, FMOD, V)    \
    LIST_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

#define LIST_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_HEADER(PFX, SNAME, FMOD, V)

#define LIST_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* HEADER ********************************************************************/
#define LIST_GENERATE_HEADER(PFX, SNAME, FMOD, V)                                                  \
                                                                                                   \
    /* List Structure */                                                                           \
    typedef struct SNAME##_s                                                                       \
    {                                                                                              \
        /* Dynamic array of elements */                                                            \
        V *buffer;                                                                                 \
                                                                                                   \
        /* Current array capacity */                                                               \
        size_t capacity;                                                                           \
                                                                                                   \
        /* Current amount of elements */                                                           \
        size_t count;                                                                              \
                                                                                                   \
        /* Function that returns an iterator to the start of the list */                           \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                                     \
                                                                                                   \
        /* Function that returns an iterator to the end of the list */                             \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                                       \
                                                                                                   \
    } SNAME, *SNAME##_ptr;                                                                         \
                                                                                                   \
    /* List Iterator */                                                                            \
    typedef struct SNAME##_iter_s                                                                  \
    {                                                                                              \
        /* Target list */                                                                          \
        struct SNAME##_s *target;                                                                  \
                                                                                                   \
        /* Cursor's position (index) */                                                            \
        size_t cursor;                                                                             \
                                                                                                   \
        /* If the iterator has reached the start of the iteration */                               \
        bool start;                                                                                \
                                                                                                   \
        /* If the iterator has reached the end of the iteration */                                 \
        bool end;                                                                                  \
                                                                                                   \
    } SNAME##_iter, *SNAME##_iter_ptr;                                                             \
                                                                                                   \
    /* Collection Functions */                                                                     \
    /* Collection Allocation and Deallocation */                                                   \
    FMOD SNAME *PFX##_new(size_t capacity);                                                        \
    FMOD SNAME *PFX##_new_from(V *elements, size_t size);                                          \
    FMOD void PFX##_clear(SNAME *_list_);                                                          \
    FMOD void PFX##_free(SNAME *_list_);                                                           \
    /* Collection Input and Output */                                                              \
    FMOD bool PFX##_push_front(SNAME *_list_, V element);                                          \
    FMOD bool PFX##_push(SNAME *_list_, V element, size_t index);                                  \
    FMOD bool PFX##_push_back(SNAME *_list_, V element);                                           \
    FMOD bool PFX##_pop_front(SNAME *_list_);                                                      \
    FMOD bool PFX##_pop(SNAME *_list_, size_t index);                                              \
    FMOD bool PFX##_pop_back(SNAME *_list_);                                                       \
    /* Conditional Input and Output */                                                             \
    FMOD bool PFX##_push_if(SNAME *_list_, V element, size_t index, bool condition);               \
    FMOD bool PFX##_pop_if(SNAME *_list_, size_t index, bool condition);                           \
    /* Collection Range Input and Output */                                                        \
    FMOD bool PFX##_prepend(SNAME *_list_, V *elements, size_t size);                              \
    FMOD bool PFX##_insert(SNAME *_list_, V *elements, size_t size, size_t index);                 \
    FMOD bool PFX##_append(SNAME *_list_, V *elements, size_t size);                               \
    FMOD bool PFX##_remove(SNAME *_list_, size_t from, size_t to);                                 \
    FMOD SNAME *PFX##_extract(SNAME *_list_, size_t from, size_t to);                              \
    /* Element Access */                                                                           \
    FMOD V PFX##_front(SNAME *_list_);                                                             \
    FMOD V PFX##_get(SNAME *_list_, size_t index);                                                 \
    FMOD V *PFX##_get_ref(SNAME *_list_, size_t index);                                            \
    FMOD V PFX##_back(SNAME *_list_);                                                              \
    FMOD size_t PFX##_indexof(SNAME *_list_, V element, int (*comparator)(V, V), bool from_start); \
    /* Collection State */                                                                         \
    FMOD bool PFX##_contains(SNAME *_list_, V element, int (*comparator)(V, V));                   \
    FMOD bool PFX##_empty(SNAME *_list_);                                                          \
    FMOD bool PFX##_full(SNAME *_list_);                                                           \
    FMOD size_t PFX##_count(SNAME *_list_);                                                        \
    FMOD bool PFX##_fits(SNAME *_list_, size_t size);                                              \
    FMOD size_t PFX##_capacity(SNAME *_list_);                                                     \
    /* Collection Utility */                                                                       \
    FMOD cmc_string PFX##_to_string(SNAME *_list_);                                                \
                                                                                                   \
    /* Iterator Functions */                                                                       \
    /* Iterator Allocation and Deallocation */                                                     \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);                                              \
    FMOD void PFX##_iter_free(SNAME##_iter *iter);                                                 \
    /* Iterator Initialization */                                                                  \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                                  \
    /* Iterator State */                                                                           \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                                \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                                  \
    /* Iterator Movement */                                                                        \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter);                                             \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter);                                               \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter);                                                 \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter);                                                 \
    /* Iterator Access */                                                                          \
    FMOD V PFX##_iter_value(SNAME##_iter *iter);                                                   \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter);                                                 \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter);                                              \
                                                                                                   \
    /* Default Value */                                                                            \
    static inline V PFX##_impl_default_value(void)                                                 \
    {                                                                                              \
        V _empty_value_;                                                                           \
                                                                                                   \
        memset(&_empty_value_, 0, sizeof(V));                                                      \
                                                                                                   \
        return _empty_value_;                                                                      \
    }                                                                                              \
                                                                                                   \
/* SOURCE ********************************************************************/
#define LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                                                                \
                                                                                                                 \
    /* Implementation Detail Functions */                                                                        \
    static bool PFX##_impl_grow(SNAME *_list_, size_t required);                                                 \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_list_);                                                      \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_list_);                                                        \
                                                                                                                 \
    FMOD SNAME *PFX##_new(size_t capacity)                                                                       \
    {                                                                                                            \
        if (capacity < 1)                                                                                        \
            return NULL;                                                                                         \
                                                                                                                 \
        SNAME *_list_ = malloc(sizeof(SNAME));                                                                   \
                                                                                                                 \
        if (!_list_)                                                                                             \
            return NULL;                                                                                         \
                                                                                                                 \
        _list_->buffer = malloc(capacity * sizeof(V));                                                           \
                                                                                                                 \
        if (!_list_->buffer)                                                                                     \
        {                                                                                                        \
            free(_list_);                                                                                        \
            return NULL;                                                                                         \
        }                                                                                                        \
                                                                                                                 \
        memset(_list_->buffer, 0, capacity * sizeof(V));                                                         \
                                                                                                                 \
        _list_->capacity = capacity;                                                                             \
        _list_->count = 0;                                                                                       \
                                                                                                                 \
        _list_->it_start = PFX##_impl_it_start;                                                                  \
        _list_->it_end = PFX##_impl_it_end;                                                                      \
                                                                                                                 \
        return _list_;                                                                                           \
    }                                                                                                            \
                                                                                                                 \
    FMOD SNAME *PFX##_new_from(V *elements, size_t size)                                                         \
    {                                                                                                            \
        if (size == 0)                                                                                           \
            return NULL;                                                                                         \
                                                                                                                 \
        SNAME *_list_ = PFX##_new(size + size / 2);                                                              \
                                                                                                                 \
        if (!_list_)                                                                                             \
            return NULL;                                                                                         \
                                                                                                                 \
        memcpy(_list_->buffer, elements, size * sizeof(V));                                                      \
                                                                                                                 \
        _list_->count = size;                                                                                    \
                                                                                                                 \
        return _list_;                                                                                           \
    }                                                                                                            \
                                                                                                                 \
    FMOD void PFX##_clear(SNAME *_list_)                                                                         \
    {                                                                                                            \
        memset(_list_->buffer, 0, sizeof(V) * _list_->capacity);                                                 \
                                                                                                                 \
        _list_->count = 0;                                                                                       \
    }                                                                                                            \
                                                                                                                 \
    FMOD void PFX##_free(SNAME *_list_)                                                                          \
    {                                                                                                            \
        free(_list_->buffer);                                                                                    \
        free(_list_);                                                                                            \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_push_front(SNAME *_list_, V element)                                                         \
    {                                                                                                            \
        if (PFX##_full(_list_))                                                                                  \
        {                                                                                                        \
            if (!PFX##_impl_grow(_list_, _list_->count + 1))                                                     \
                return false;                                                                                    \
        }                                                                                                        \
                                                                                                                 \
        if (!PFX##_empty(_list_))                                                                                \
        {                                                                                                        \
            memmove(_list_->buffer + 1, _list_->buffer, _list_->count * sizeof(V));                              \
        }                                                                                                        \
                                                                                                                 \
        _list_->buffer[0] = element;                                                                             \
                                                                                                                 \
        _list_->count++;                                                                                         \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_push(SNAME *_list_, V element, size_t index)                                                 \
    {                                                                                                            \
        if (index > _list_->count)                                                                               \
            return false;                                                                                        \
                                                                                                                 \
        if (index == 0)                                                                                          \
        {                                                                                                        \
            return PFX##_push_front(_list_, element);                                                            \
        }                                                                                                        \
        else if (index == _list_->count)                                                                         \
        {                                                                                                        \
            return PFX##_push_back(_list_, element);                                                             \
        }                                                                                                        \
                                                                                                                 \
        if (PFX##_full(_list_))                                                                                  \
        {                                                                                                        \
            if (!PFX##_impl_grow(_list_, _list_->count + 1))                                                     \
                return false;                                                                                    \
        }                                                                                                        \
                                                                                                                 \
        memmove(_list_->buffer + index + 1, _list_->buffer + index, (_list_->count - index) * sizeof(V));        \
                                                                                                                 \
        _list_->buffer[index] = element;                                                                         \
                                                                                                                 \
        _list_->count++;                                                                                         \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_push_back(SNAME *_list_, V element)                                                          \
    {                                                                                                            \
        if (PFX##_full(_list_))                                                                                  \
        {                                                                                                        \
            if (!PFX##_impl_grow(_list_, _list_->count + 1))                                                     \
                return false;                                                                                    \
        }                                                                                                        \
                                                                                                                 \
        _list_->buffer[_list_->count++] = element;                                                               \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_pop_front(SNAME *_list_)                                                                     \
    {                                                                                                            \
        if (PFX##_empty(_list_))                                                                                 \
            return false;                                                                                        \
                                                                                                                 \
        memmove(_list_->buffer, _list_->buffer + 1, _list_->count * sizeof(V));                                  \
                                                                                                                 \
        _list_->buffer[--_list_->count] = PFX##_impl_default_value();                                            \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_pop(SNAME *_list_, size_t index)                                                             \
    {                                                                                                            \
        if (PFX##_empty(_list_))                                                                                 \
            return false;                                                                                        \
                                                                                                                 \
        if (index >= _list_->count)                                                                              \
            return false;                                                                                        \
                                                                                                                 \
        if (index == 0)                                                                                          \
        {                                                                                                        \
            return PFX##_pop_front(_list_);                                                                      \
        }                                                                                                        \
        else if (index == _list_->count - 1)                                                                     \
        {                                                                                                        \
            return PFX##_pop_back(_list_);                                                                       \
        }                                                                                                        \
                                                                                                                 \
        memmove(_list_->buffer + index, _list_->buffer + index + 1, (_list_->count - index) * sizeof(V));        \
                                                                                                                 \
        _list_->buffer[--_list_->count] = PFX##_impl_default_value();                                            \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_pop_back(SNAME *_list_)                                                                      \
    {                                                                                                            \
        if (PFX##_empty(_list_))                                                                                 \
            return false;                                                                                        \
                                                                                                                 \
        _list_->buffer[--_list_->count] = PFX##_impl_default_value();                                            \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_push_if(SNAME *_list_, V element, size_t index, bool condition)                              \
    {                                                                                                            \
        if (condition)                                                                                           \
            return PFX##_push(_list_, element, index);                                                           \
                                                                                                                 \
        return false;                                                                                            \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_pop_if(SNAME *_list_, size_t index, bool condition)                                          \
    {                                                                                                            \
        if (condition)                                                                                           \
            return PFX##_pop(_list_, index);                                                                     \
                                                                                                                 \
        return false;                                                                                            \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_prepend(SNAME *_list_, V *elements, size_t size)                                             \
    {                                                                                                            \
        if (size == 0)                                                                                           \
            return false;                                                                                        \
                                                                                                                 \
        if (!PFX##_fits(_list_, size))                                                                           \
        {                                                                                                        \
            if (!PFX##_impl_grow(_list_, _list_->count + size))                                                  \
                return false;                                                                                    \
        }                                                                                                        \
                                                                                                                 \
        memmove(_list_->buffer + size, _list_->buffer, _list_->count * sizeof(V));                               \
                                                                                                                 \
        memcpy(_list_->buffer, elements, size * sizeof(V));                                                      \
                                                                                                                 \
        _list_->count += size;                                                                                   \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_insert(SNAME *_list_, V *elements, size_t size, size_t index)                                \
    {                                                                                                            \
        if (size == 0 || index > _list_->count)                                                                  \
            return false;                                                                                        \
                                                                                                                 \
        if (index == 0)                                                                                          \
            return PFX##_prepend(_list_, elements, size);                                                        \
        else if (index == _list_->count)                                                                         \
            return PFX##_append(_list_, elements, size);                                                         \
        else                                                                                                     \
        {                                                                                                        \
            if (!PFX##_fits(_list_, size))                                                                       \
            {                                                                                                    \
                if (!PFX##_impl_grow(_list_, _list_->count + size))                                              \
                    return false;                                                                                \
            }                                                                                                    \
                                                                                                                 \
            memmove(_list_->buffer + index + size, _list_->buffer + index, (_list_->count - index) * sizeof(V)); \
                                                                                                                 \
            memcpy(_list_->buffer + index, elements, size * sizeof(V));                                          \
                                                                                                                 \
            _list_->count += size;                                                                               \
        }                                                                                                        \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_append(SNAME *_list_, V *elements, size_t size)                                              \
    {                                                                                                            \
        if (size == 0)                                                                                           \
            return false;                                                                                        \
                                                                                                                 \
        if (!PFX##_fits(_list_, size))                                                                           \
        {                                                                                                        \
            if (!PFX##_impl_grow(_list_, _list_->count + size))                                                  \
                return false;                                                                                    \
        }                                                                                                        \
                                                                                                                 \
        memcpy(_list_->buffer + _list_->count, elements, size * sizeof(V));                                      \
                                                                                                                 \
        _list_->count += size;                                                                                   \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_remove(SNAME *_list_, size_t from, size_t to)                                                \
    {                                                                                                            \
        if (from > to || to >= _list_->count)                                                                    \
            return false;                                                                                        \
                                                                                                                 \
        size_t length = (to - from + 1);                                                                         \
                                                                                                                 \
        memmove(_list_->buffer + from, _list_->buffer + to + 1, (_list_->count - to - 1) * sizeof(V));           \
                                                                                                                 \
        memset(_list_->buffer + _list_->count - length, 0, length * sizeof(V));                                  \
                                                                                                                 \
        _list_->count -= to - from + 1;                                                                          \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD SNAME *PFX##_extract(SNAME *_list_, size_t from, size_t to)                                             \
    {                                                                                                            \
        if (from > to || to >= _list_->count)                                                                    \
            return false;                                                                                        \
                                                                                                                 \
        size_t length = to - from + 1;                                                                           \
                                                                                                                 \
        SNAME *result = PFX##_new_from(_list_->buffer + from, length);                                           \
                                                                                                                 \
        if (!result)                                                                                             \
            return NULL;                                                                                         \
                                                                                                                 \
        memmove(_list_->buffer + from, _list_->buffer + to + 1, (_list_->count - to - 1) * sizeof(V));           \
                                                                                                                 \
        memset(_list_->buffer + _list_->count - length, 0, length * sizeof(V));                                  \
                                                                                                                 \
        _list_->count -= length;                                                                                 \
                                                                                                                 \
        return result;                                                                                           \
    }                                                                                                            \
                                                                                                                 \
    FMOD V PFX##_front(SNAME *_list_)                                                                            \
    {                                                                                                            \
        if (PFX##_empty(_list_))                                                                                 \
            return PFX##_impl_default_value();                                                                   \
                                                                                                                 \
        return _list_->buffer[0];                                                                                \
    }                                                                                                            \
                                                                                                                 \
    FMOD V PFX##_get(SNAME *_list_, size_t index)                                                                \
    {                                                                                                            \
        if (index >= _list_->count || PFX##_empty(_list_))                                                       \
            return PFX##_impl_default_value();                                                                   \
                                                                                                                 \
        return _list_->buffer[index];                                                                            \
    }                                                                                                            \
                                                                                                                 \
    FMOD V *PFX##_get_ref(SNAME *_list_, size_t index)                                                           \
    {                                                                                                            \
        if (index >= _list_->count)                                                                              \
            return NULL;                                                                                         \
                                                                                                                 \
        if (PFX##_empty(_list_))                                                                                 \
            return NULL;                                                                                         \
                                                                                                                 \
        return &(_list_->buffer[index]);                                                                         \
    }                                                                                                            \
                                                                                                                 \
    FMOD V PFX##_back(SNAME *_list_)                                                                             \
    {                                                                                                            \
        if (PFX##_empty(_list_))                                                                                 \
            return PFX##_impl_default_value();                                                                   \
                                                                                                                 \
        return _list_->buffer[_list_->count - 1];                                                                \
    }                                                                                                            \
                                                                                                                 \
    FMOD size_t PFX##_indexof(SNAME *_list_, V element, int (*comparator)(V, V), bool from_start)                \
    {                                                                                                            \
        if (from_start)                                                                                          \
        {                                                                                                        \
            for (size_t i = 0; i < _list_->count; i++)                                                           \
            {                                                                                                    \
                if (comparator(_list_->buffer[i], element) == 0)                                                 \
                    return i;                                                                                    \
            }                                                                                                    \
        }                                                                                                        \
        else                                                                                                     \
        {                                                                                                        \
            for (size_t i = _list_->count; i > 0; i--)                                                           \
            {                                                                                                    \
                if (comparator(_list_->buffer[i - 1], element) == 0)                                             \
                    return i - 1;                                                                                \
            }                                                                                                    \
        }                                                                                                        \
                                                                                                                 \
        return _list_->count;                                                                                    \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_contains(SNAME *_list_, V element, int (*comparator)(V, V))                                  \
    {                                                                                                            \
        for (size_t i = 0; i < _list_->count; i++)                                                               \
        {                                                                                                        \
            if (comparator(_list_->buffer[i], element) == 0)                                                     \
                return true;                                                                                     \
        }                                                                                                        \
                                                                                                                 \
        return false;                                                                                            \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_empty(SNAME *_list_)                                                                         \
    {                                                                                                            \
        return _list_->count == 0;                                                                               \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_full(SNAME *_list_)                                                                          \
    {                                                                                                            \
        return _list_->count >= _list_->capacity;                                                                \
    }                                                                                                            \
                                                                                                                 \
    FMOD size_t PFX##_count(SNAME *_list_)                                                                       \
    {                                                                                                            \
        return _list_->count;                                                                                    \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_fits(SNAME *_list_, size_t size)                                                             \
    {                                                                                                            \
        return _list_->count + size <= _list_->capacity;                                                         \
    }                                                                                                            \
                                                                                                                 \
    FMOD size_t PFX##_capacity(SNAME *_list_)                                                                    \
    {                                                                                                            \
        return _list_->capacity;                                                                                 \
    }                                                                                                            \
                                                                                                                 \
    FMOD cmc_string PFX##_to_string(SNAME *_list_)                                                               \
    {                                                                                                            \
        cmc_string str;                                                                                          \
        SNAME *l_ = _list_;                                                                                      \
        const char *name = #SNAME;                                                                               \
                                                                                                                 \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_list,                                                     \
                 name, l_, l_->buffer, l_->capacity, l_->count);                                                 \
                                                                                                                 \
        return str;                                                                                              \
    }                                                                                                            \
                                                                                                                 \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target)                                                             \
    {                                                                                                            \
        SNAME##_iter *iter = malloc(sizeof(SNAME##_iter));                                                       \
                                                                                                                 \
        if (!iter)                                                                                               \
            return NULL;                                                                                         \
                                                                                                                 \
        PFX##_iter_init(iter, target);                                                                           \
                                                                                                                 \
        return iter;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD void PFX##_iter_free(SNAME##_iter *iter)                                                                \
    {                                                                                                            \
        free(iter);                                                                                              \
    }                                                                                                            \
                                                                                                                 \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                                                 \
    {                                                                                                            \
        iter->target = target;                                                                                   \
        iter->cursor = 0;                                                                                        \
        iter->start = true;                                                                                      \
        iter->end = PFX##_empty(target);                                                                         \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                                               \
    {                                                                                                            \
        return PFX##_empty(iter->target) || iter->start;                                                         \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                                                 \
    {                                                                                                            \
        return PFX##_empty(iter->target) || iter->end;                                                           \
    }                                                                                                            \
                                                                                                                 \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter)                                                            \
    {                                                                                                            \
        iter->cursor = 0;                                                                                        \
        iter->start = true;                                                                                      \
        iter->end = PFX##_empty(iter->target);                                                                   \
    }                                                                                                            \
                                                                                                                 \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter)                                                              \
    {                                                                                                            \
        iter->start = PFX##_empty(iter->target);                                                                 \
        iter->cursor = PFX##_empty(iter->target) ? 0 : iter->target->count - 1;                                  \
        iter->end = true;                                                                                        \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter)                                                                \
    {                                                                                                            \
        if (iter->end)                                                                                           \
            return false;                                                                                        \
                                                                                                                 \
        iter->start = PFX##_empty(iter->target);                                                                 \
                                                                                                                 \
        if (iter->cursor == iter->target->count - 1)                                                             \
            iter->end = true;                                                                                    \
        else                                                                                                     \
            iter->cursor++;                                                                                      \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter)                                                                \
    {                                                                                                            \
        if (iter->start)                                                                                         \
            return false;                                                                                        \
                                                                                                                 \
        iter->end = PFX##_empty(iter->target);                                                                   \
                                                                                                                 \
        if (iter->cursor == 0)                                                                                   \
            iter->start = true;                                                                                  \
        else                                                                                                     \
            iter->cursor--;                                                                                      \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    FMOD V PFX##_iter_value(SNAME##_iter *iter)                                                                  \
    {                                                                                                            \
        if (PFX##_empty(iter->target))                                                                           \
            return PFX##_impl_default_value();                                                                   \
                                                                                                                 \
        return iter->target->buffer[iter->cursor];                                                               \
    }                                                                                                            \
                                                                                                                 \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter)                                                                \
    {                                                                                                            \
        if (PFX##_empty(iter->target))                                                                           \
            return NULL;                                                                                         \
                                                                                                                 \
        return &(iter->target->buffer[iter->cursor]);                                                            \
    }                                                                                                            \
                                                                                                                 \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter)                                                             \
    {                                                                                                            \
        return iter->cursor;                                                                                     \
    }                                                                                                            \
                                                                                                                 \
    static bool PFX##_impl_grow(SNAME *_list_, size_t required)                                                  \
    {                                                                                                            \
        size_t new_capacity = _list_->capacity * 2;                                                              \
                                                                                                                 \
        if (new_capacity < required)                                                                             \
            new_capacity = required;                                                                             \
                                                                                                                 \
        V *new_buffer = realloc(_list_->buffer, sizeof(V) * new_capacity);                                       \
                                                                                                                 \
        if (!new_buffer)                                                                                         \
            return false;                                                                                        \
                                                                                                                 \
        _list_->buffer = new_buffer;                                                                             \
        _list_->capacity = new_capacity;                                                                         \
                                                                                                                 \
        return true;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_list_)                                                       \
    {                                                                                                            \
        SNAME##_iter iter;                                                                                       \
                                                                                                                 \
        PFX##_iter_init(&iter, _list_);                                                                          \
        PFX##_iter_to_start(&iter);                                                                              \
                                                                                                                 \
        return iter;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_list_)                                                         \
    {                                                                                                            \
        SNAME##_iter iter;                                                                                       \
                                                                                                                 \
        PFX##_iter_init(&iter, _list_);                                                                          \
        PFX##_iter_to_end(&iter);                                                                                \
                                                                                                                 \
        return iter;                                                                                             \
    }

#endif /* CMC_LIST_H */
