/**
 * sortedlist.h
 *
 * Creation Date: 17/09/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * SortedList
 *
 * A sorted list is a dynamic array, meaning that you can store as many elements
 * as you like and when its capacity is full, the buffer is reallocated. The
 * elements are only sorted when a certain action requires that the array is
 * sorted like accessing min() or max(). This prevents the array from being
 * sorted after every insertion or removal. The array is sorted using a
 * variation of quick sort that uses insertion sort for small partitions.
 */

#ifndef CMC_SORTEDLIST_H
#define CMC_SORTEDLIST_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_sortedlist = "%s at %p { buffer:%p, capacity:%" PRIuMAX ", count:%" PRIuMAX ", is_sorted:%s, cmp:%p }";

#define CMC_GENERATE_SORTEDLIST(PFX, SNAME, V)    \
    CMC_GENERATE_SORTEDLIST_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_SORTEDLIST_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_SORTEDLIST_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_SORTEDLIST_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_SORTEDLIST_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_SORTEDLIST_SOURCE(PFX, SNAME, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_SORTEDLIST_HEADER(PFX, SNAME, V)                       \
                                                                            \
    /* List Structure */                                                    \
    struct SNAME                                                            \
    {                                                                       \
        /* Dynamic array of elements */                                     \
        V *buffer;                                                          \
                                                                            \
        /* Current array capacity */                                        \
        size_t capacity;                                                    \
                                                                            \
        /* Current amount of elements */                                    \
        size_t count;                                                       \
                                                                            \
        /* Flag if the list is sorted or not, used by lazy evaluation */    \
        bool is_sorted;                                                     \
                                                                            \
        /* Element comparison function */                                   \
        int (*cmp)(V, V);                                                   \
                                                                            \
        /* Function that returns an iterator to the start of the list */    \
        struct SNAME##_iter (*it_start)(struct SNAME *);                    \
                                                                            \
        /* Function that returns an iterator to the end of the list */      \
        struct SNAME##_iter (*it_end)(struct SNAME *);                      \
    };                                                                      \
                                                                            \
    /* List Iterator */                                                     \
    struct SNAME##_iter                                                     \
    {                                                                       \
        /* Target list */                                                   \
        struct SNAME *target;                                               \
                                                                            \
        /* Cursor's position (index) */                                     \
        size_t cursor;                                                      \
                                                                            \
        /* If the iterator has reached the start of the iteration */        \
        bool start;                                                         \
                                                                            \
        /* If the iterator has reached the end of the iteration */          \
        bool end;                                                           \
    };                                                                      \
                                                                            \
    /* Collection Functions */                                              \
    /* Collection Allocation and Deallocation */                            \
    struct SNAME *PFX##_new(size_t capacity, int (*compare)(V, V));         \
    void PFX##_clear(struct SNAME *_list_, void (*deallocator)(V));         \
    void PFX##_free(struct SNAME *_list_, void (*deallocator)(V));          \
    /* Collection Input and Output */                                       \
    bool PFX##_insert(struct SNAME *_list_, V element);                     \
    bool PFX##_remove(struct SNAME *_list_, size_t index);                  \
    /* Element Access */                                                    \
    bool PFX##_max(struct SNAME *_list_, V *result);                        \
    bool PFX##_min(struct SNAME *_list_, V *result);                        \
    V PFX##_get(struct SNAME *_list_, size_t index);                        \
    size_t PFX##_indexof(struct SNAME *_list_, V element, bool from_start); \
    /* Collection State */                                                  \
    bool PFX##_contains(struct SNAME *_list_, V element);                   \
    bool PFX##_empty(struct SNAME *_list_);                                 \
    bool PFX##_full(struct SNAME *_list_);                                  \
    size_t PFX##_count(struct SNAME *_list_);                               \
    size_t PFX##_capacity(struct SNAME *_list_);                            \
    /* Collection Utility */                                                \
    bool PFX##_resize(struct SNAME *_list_, size_t capacity);               \
    void PFX##_sort(struct SNAME *_list_);                                  \
    struct SNAME *PFX##_copy_of(struct SNAME *_list_, V (*copy_func)(V));   \
    bool PFX##_equals(struct SNAME *_list1_, struct SNAME *_list2_);        \
    struct cmc_string PFX##_to_string(struct SNAME *_list_);                \
                                                                            \
    /* Iterator Functions */                                                \
    /* Iterator Allocation and Deallocation */                              \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);              \
    void PFX##_iter_free(struct SNAME##_iter *iter);                        \
    /* Iterator Initialization */                                           \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);  \
    /* Iterator State */                                                    \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                       \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                         \
    /* Iterator Movement */                                                 \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                    \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                      \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                        \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                        \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);       \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);        \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);         \
    /* Iterator Access */                                                   \
    V PFX##_iter_value(struct SNAME##_iter *iter);                          \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);                     \
                                                                            \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_SORTEDLIST_SOURCE(PFX, SNAME, V)                                     \
                                                                                          \
    /* Implementation Detail Functions */                                                 \
    static size_t PFX##_impl_binary_search_first(struct SNAME *_list_, V value);          \
    static size_t PFX##_impl_binary_search_last(struct SNAME *_list_, V value);           \
    void PFX##_impl_sort_quicksort(V *array, int (*cmp)(V, V), size_t low, size_t high);  \
    void PFX##_impl_sort_insertion(V *array, int (*cmp)(V, V), size_t low, size_t high);  \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_list_);                 \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_list_);                   \
                                                                                          \
    struct SNAME *PFX##_new(size_t capacity, int (*compare)(V, V))                        \
    {                                                                                     \
        if (capacity < 1)                                                                 \
            return NULL;                                                                  \
                                                                                          \
        struct SNAME *_list_ = malloc(sizeof(struct SNAME));                              \
                                                                                          \
        if (!_list_)                                                                      \
            return NULL;                                                                  \
                                                                                          \
        _list_->buffer = calloc(capacity, sizeof(V));                                     \
                                                                                          \
        if (!_list_->buffer)                                                              \
        {                                                                                 \
            free(_list_);                                                                 \
            return NULL;                                                                  \
        }                                                                                 \
                                                                                          \
        _list_->capacity = capacity;                                                      \
        _list_->count = 0;                                                                \
        _list_->cmp = compare;                                                            \
        _list_->is_sorted = false;                                                        \
                                                                                          \
        _list_->it_start = PFX##_impl_it_start;                                           \
        _list_->it_end = PFX##_impl_it_end;                                               \
                                                                                          \
        return _list_;                                                                    \
    }                                                                                     \
                                                                                          \
    void PFX##_clear(struct SNAME *_list_, void (*deallocator)(V))                        \
    {                                                                                     \
        if (deallocator)                                                                  \
        {                                                                                 \
            for (size_t i = 0; i < _list_->count; i++)                                    \
                deallocator(_list_->buffer[i]);                                           \
        }                                                                                 \
                                                                                          \
        memset(_list_->buffer, 0, sizeof(V) * _list_->capacity);                          \
                                                                                          \
        _list_->count = 0;                                                                \
    }                                                                                     \
                                                                                          \
    void PFX##_free(struct SNAME *_list_, void (*deallocator)(V))                         \
    {                                                                                     \
        if (deallocator)                                                                  \
        {                                                                                 \
            for (size_t i = 0; i < _list_->count; i++)                                    \
                deallocator(_list_->buffer[i]);                                           \
        }                                                                                 \
                                                                                          \
        free(_list_->buffer);                                                             \
        free(_list_);                                                                     \
    }                                                                                     \
                                                                                          \
    bool PFX##_insert(struct SNAME *_list_, V element)                                    \
    {                                                                                     \
        if (PFX##_full(_list_))                                                           \
        {                                                                                 \
            if (!PFX##_resize(_list_, PFX##_capacity(_list_) * 2))                        \
                return false;                                                             \
        }                                                                                 \
                                                                                          \
        _list_->buffer[_list_->count++] = element;                                        \
                                                                                          \
        _list_->is_sorted = false;                                                        \
                                                                                          \
        return true;                                                                      \
    }                                                                                     \
                                                                                          \
    bool PFX##_remove(struct SNAME *_list_, size_t index)                                 \
    {                                                                                     \
        if (index >= _list_->count)                                                       \
            return false;                                                                 \
                                                                                          \
        memmove(_list_->buffer + index, _list_->buffer + index + 1,                       \
                (_list_->count - index) * sizeof(V));                                     \
                                                                                          \
        _list_->buffer[--_list_->count] = (V){0};                                         \
                                                                                          \
        return true;                                                                      \
    }                                                                                     \
                                                                                          \
    bool PFX##_min(struct SNAME *_list_, V *result)                                       \
    {                                                                                     \
        if (PFX##_empty(_list_))                                                          \
            return false;                                                                 \
                                                                                          \
        PFX##_sort(_list_);                                                               \
                                                                                          \
        *result = _list_->buffer[0];                                                      \
                                                                                          \
        return true;                                                                      \
    }                                                                                     \
                                                                                          \
    bool PFX##_max(struct SNAME *_list_, V *result)                                       \
    {                                                                                     \
        if (PFX##_empty(_list_))                                                          \
            return false;                                                                 \
                                                                                          \
        PFX##_sort(_list_);                                                               \
                                                                                          \
        *result = _list_->buffer[_list_->count - 1];                                      \
                                                                                          \
        return true;                                                                      \
    }                                                                                     \
                                                                                          \
    V PFX##_get(struct SNAME *_list_, size_t index)                                       \
    {                                                                                     \
        if (index >= _list_->count)                                                       \
            return (V){0};                                                                \
                                                                                          \
        PFX##_sort(_list_);                                                               \
                                                                                          \
        return _list_->buffer[index];                                                     \
    }                                                                                     \
                                                                                          \
    size_t PFX##_indexof(struct SNAME *_list_, V element, bool from_start)                \
    {                                                                                     \
        PFX##_sort(_list_);                                                               \
                                                                                          \
        if (from_start)                                                                   \
        {                                                                                 \
            return PFX##_impl_binary_search_first(_list_, element);                       \
        }                                                                                 \
                                                                                          \
        return PFX##_impl_binary_search_last(_list_, element);                            \
    }                                                                                     \
                                                                                          \
    bool PFX##_contains(struct SNAME *_list_, V element)                                  \
    {                                                                                     \
        if (PFX##_empty(_list_))                                                          \
            return false;                                                                 \
                                                                                          \
        PFX##_sort(_list_);                                                               \
                                                                                          \
        return PFX##_impl_binary_search_first(_list_, element) < PFX##_count(_list_);     \
    }                                                                                     \
                                                                                          \
    bool PFX##_empty(struct SNAME *_list_)                                                \
    {                                                                                     \
        return _list_->count == 0;                                                        \
    }                                                                                     \
                                                                                          \
    bool PFX##_full(struct SNAME *_list_)                                                 \
    {                                                                                     \
        return _list_->count >= _list_->capacity;                                         \
    }                                                                                     \
                                                                                          \
    size_t PFX##_count(struct SNAME *_list_)                                              \
    {                                                                                     \
        return _list_->count;                                                             \
    }                                                                                     \
                                                                                          \
    size_t PFX##_capacity(struct SNAME *_list_)                                           \
    {                                                                                     \
        return _list_->capacity;                                                          \
    }                                                                                     \
                                                                                          \
    bool PFX##_resize(struct SNAME *_list_, size_t capacity)                              \
    {                                                                                     \
        if (PFX##_capacity(_list_) == capacity)                                           \
            return true;                                                                  \
                                                                                          \
        if (capacity < PFX##_count(_list_))                                               \
            return false;                                                                 \
                                                                                          \
        V *new_buffer = realloc(_list_->buffer, sizeof(V) * capacity);                    \
                                                                                          \
        if (!new_buffer)                                                                  \
            return false;                                                                 \
                                                                                          \
        _list_->buffer = new_buffer;                                                      \
        _list_->capacity = capacity;                                                      \
                                                                                          \
        return true;                                                                      \
    }                                                                                     \
                                                                                          \
    void PFX##_sort(struct SNAME *_list_)                                                 \
    {                                                                                     \
        if (!_list_->is_sorted && _list_->count > 1)                                      \
        {                                                                                 \
            PFX##_impl_sort_quicksort(_list_->buffer, _list_->cmp, 0, _list_->count - 1); \
                                                                                          \
            _list_->is_sorted = true;                                                     \
        }                                                                                 \
    }                                                                                     \
                                                                                          \
    struct SNAME *PFX##_copy_of(struct SNAME *_list_, V (*copy_func)(V))                  \
    {                                                                                     \
        struct SNAME *result = PFX##_new(_list_->capacity, _list_->cmp);                  \
                                                                                          \
        if (!result)                                                                      \
            return NULL;                                                                  \
                                                                                          \
        if (copy_func)                                                                    \
        {                                                                                 \
            for (size_t i = 0; i < _list_->count; i++)                                    \
                result->buffer[i] = copy_func(_list_->buffer[i]);                         \
        }                                                                                 \
        else                                                                              \
            memcpy(result->buffer, _list_->buffer, sizeof(V) * _list_->count);            \
                                                                                          \
        result->count = _list_->count;                                                    \
                                                                                          \
        return result;                                                                    \
    }                                                                                     \
                                                                                          \
    bool PFX##_equals(struct SNAME *_list1_, struct SNAME *_list2_)                       \
    {                                                                                     \
        if (PFX##_count(_list1_) != PFX##_count(_list2_))                                 \
            return false;                                                                 \
                                                                                          \
        PFX##_sort(_list1_);                                                              \
        PFX##_sort(_list2_);                                                              \
                                                                                          \
        for (size_t i = 0; i < PFX##_count(_list1_); i++)                                 \
        {                                                                                 \
            if (_list1_->cmp(_list1_->buffer[i], _list2_->buffer[i]) != 0)                \
                return false;                                                             \
        }                                                                                 \
                                                                                          \
        return false;                                                                     \
    }                                                                                     \
                                                                                          \
    struct cmc_string PFX##_to_string(struct SNAME *_list_)                               \
    {                                                                                     \
        struct cmc_string str;                                                            \
        struct SNAME *l_ = _list_;                                                        \
        const char *name = #SNAME;                                                        \
                                                                                          \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_sortedlist,                        \
                 name, l_, l_->buffer, l_->capacity, l_->count,                           \
                 l_->is_sorted ? "true" : "false", l_->cmp);                              \
                                                                                          \
        return str;                                                                       \
    }                                                                                     \
                                                                                          \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                             \
    {                                                                                     \
        struct SNAME##_iter *iter = malloc(sizeof(struct SNAME##_iter));                  \
                                                                                          \
        if (!iter)                                                                        \
            return NULL;                                                                  \
                                                                                          \
        PFX##_iter_init(iter, target);                                                    \
                                                                                          \
        return iter;                                                                      \
    }                                                                                     \
                                                                                          \
    void PFX##_iter_free(struct SNAME##_iter *iter)                                       \
    {                                                                                     \
        free(iter);                                                                       \
    }                                                                                     \
                                                                                          \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)                 \
    {                                                                                     \
        PFX##_sort(target);                                                               \
                                                                                          \
        iter->target = target;                                                            \
        iter->cursor = 0;                                                                 \
        iter->start = true;                                                               \
        iter->end = PFX##_empty(target);                                                  \
    }                                                                                     \
                                                                                          \
    bool PFX##_iter_start(struct SNAME##_iter *iter)                                      \
    {                                                                                     \
        return PFX##_empty(iter->target) || iter->start;                                  \
    }                                                                                     \
                                                                                          \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                                        \
    {                                                                                     \
        return PFX##_empty(iter->target) || iter->end;                                    \
    }                                                                                     \
                                                                                          \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                                   \
    {                                                                                     \
        if (!PFX##_empty(iter->target))                                                   \
        {                                                                                 \
            iter->cursor = 0;                                                             \
            iter->start = true;                                                           \
            iter->end = PFX##_empty(iter->target);                                        \
        }                                                                                 \
    }                                                                                     \
                                                                                          \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                                     \
    {                                                                                     \
        if (!PFX##_empty(iter->target))                                                   \
        {                                                                                 \
            iter->start = PFX##_empty(iter->target);                                      \
            iter->cursor = PFX##_empty(iter->target) ? 0 : iter->target->count - 1;       \
            iter->end = true;                                                             \
        }                                                                                 \
    }                                                                                     \
                                                                                          \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                                       \
    {                                                                                     \
        if (iter->end)                                                                    \
            return false;                                                                 \
                                                                                          \
        if (iter->cursor + 1 == PFX##_count(iter->target))                                \
        {                                                                                 \
            iter->end = true;                                                             \
            return false;                                                                 \
        }                                                                                 \
                                                                                          \
        iter->start = PFX##_empty(iter->target);                                          \
                                                                                          \
        iter->cursor++;                                                                   \
                                                                                          \
        return true;                                                                      \
    }                                                                                     \
                                                                                          \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                                       \
    {                                                                                     \
        if (iter->start)                                                                  \
            return false;                                                                 \
                                                                                          \
        if (iter->cursor == 0)                                                            \
        {                                                                                 \
            iter->start = true;                                                           \
            return false;                                                                 \
        }                                                                                 \
                                                                                          \
        iter->end = PFX##_empty(iter->target);                                            \
                                                                                          \
        iter->cursor--;                                                                   \
                                                                                          \
        return true;                                                                      \
    }                                                                                     \
                                                                                          \
    /* Returns true only if the iterator moved */                                         \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)                      \
    {                                                                                     \
        if (iter->end)                                                                    \
            return false;                                                                 \
                                                                                          \
        if (iter->cursor + 1 == iter->target->count)                                      \
        {                                                                                 \
            iter->end = true;                                                             \
            return false;                                                                 \
        }                                                                                 \
                                                                                          \
        if (steps == 0 || iter->cursor + steps >= PFX##_count(iter->target))              \
            return false;                                                                 \
                                                                                          \
        iter->start = PFX##_empty(iter->target);                                          \
                                                                                          \
        iter->cursor += steps;                                                            \
                                                                                          \
        return true;                                                                      \
    }                                                                                     \
                                                                                          \
    /* Returns true only if the iterator moved */                                         \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps)                       \
    {                                                                                     \
        if (iter->start)                                                                  \
            return false;                                                                 \
                                                                                          \
        if (iter->cursor == 0)                                                            \
        {                                                                                 \
            iter->start = true;                                                           \
            return false;                                                                 \
        }                                                                                 \
                                                                                          \
        if (steps == 0 || iter->cursor < steps)                                           \
            return false;                                                                 \
                                                                                          \
        iter->end = PFX##_empty(iter->target);                                            \
                                                                                          \
        iter->cursor -= steps;                                                            \
                                                                                          \
        return true;                                                                      \
    }                                                                                     \
                                                                                          \
    /* Returns true only if the iterator was able to be positioned at the given index */  \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)                        \
    {                                                                                     \
        if (index >= PFX##_count(iter->target))                                           \
            return false;                                                                 \
                                                                                          \
        if (iter->cursor > index)                                                         \
            return PFX##_iter_rewind(iter, iter->cursor - index);                         \
        else if (iter->cursor < index)                                                    \
            return PFX##_iter_advance(iter, index - iter->cursor);                        \
                                                                                          \
        return true;                                                                      \
    }                                                                                     \
                                                                                          \
    V PFX##_iter_value(struct SNAME##_iter *iter)                                         \
    {                                                                                     \
        if (PFX##_empty(iter->target))                                                    \
            return (V){0};                                                                \
                                                                                          \
        return iter->target->buffer[iter->cursor];                                        \
    }                                                                                     \
                                                                                          \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                                    \
    {                                                                                     \
        return iter->cursor;                                                              \
    }                                                                                     \
                                                                                          \
    static size_t PFX##_impl_binary_search_first(struct SNAME *_list_, V value)           \
    {                                                                                     \
        if (PFX##_empty(_list_))                                                          \
            return 1;                                                                     \
                                                                                          \
        size_t L = 0;                                                                     \
        size_t R = PFX##_count(_list_);                                                   \
                                                                                          \
        while (L < R)                                                                     \
        {                                                                                 \
            size_t M = L + (R - L) / 2;                                                   \
                                                                                          \
            if (_list_->cmp(_list_->buffer[M], value) < 0)                                \
                L = M + 1;                                                                \
            else                                                                          \
                R = M;                                                                    \
        }                                                                                 \
                                                                                          \
        if (_list_->cmp(_list_->buffer[L], value) == 0)                                   \
            return L;                                                                     \
                                                                                          \
        /* Not found */                                                                   \
        return PFX##_count(_list_);                                                       \
    }                                                                                     \
                                                                                          \
    static size_t PFX##_impl_binary_search_last(struct SNAME *_list_, V value)            \
    {                                                                                     \
        if (PFX##_empty(_list_))                                                          \
            return 1;                                                                     \
                                                                                          \
        size_t L = 0;                                                                     \
        size_t R = PFX##_count(_list_);                                                   \
                                                                                          \
        while (L < R)                                                                     \
        {                                                                                 \
            size_t M = L + (R - L) / 2;                                                   \
                                                                                          \
            if (_list_->cmp(_list_->buffer[M], value) > 0)                                \
                R = M;                                                                    \
            else                                                                          \
                L = M + 1;                                                                \
        }                                                                                 \
                                                                                          \
        if (_list_->cmp(_list_->buffer[L - 1], value) == 0)                               \
            return L - 1;                                                                 \
                                                                                          \
        /* Not found */                                                                   \
        return PFX##_count(_list_);                                                       \
    }                                                                                     \
                                                                                          \
    /* Characteristics of this quicksort implementation: */                               \
    /* - Hybrid: uses insertion sort for small arrays */                                  \
    /* - Partition: Lomuto's Method (can be optimized and use Hoare's Method) */          \
    /* - Tail recursion: minimize recursion depth */                                      \
    void PFX##_impl_sort_quicksort(V *array, int (*cmp)(V, V), size_t low, size_t high)   \
    {                                                                                     \
        while (low < high)                                                                \
        {                                                                                 \
            /* Quicksort performs poorly for smaller arrays so let insertion */           \
            /* sort do the job */                                                         \
            if (high - low < 10)                                                          \
            {                                                                             \
                PFX##_impl_sort_insertion(array, cmp, low, high);                         \
                break;                                                                    \
            }                                                                             \
            else                                                                          \
            {                                                                             \
                /* Partition */                                                           \
                V pivot = array[high];                                                    \
                                                                                          \
                size_t pindex = low;                                                      \
                                                                                          \
                for (size_t i = low; i < high; i++)                                       \
                {                                                                         \
                    if (cmp(array[i], pivot) <= 0)                                        \
                    {                                                                     \
                        V _tmp_ = array[i];                                               \
                        array[i] = array[pindex];                                         \
                        array[pindex] = _tmp_;                                            \
                                                                                          \
                        pindex++;                                                         \
                    }                                                                     \
                }                                                                         \
                                                                                          \
                V _tmp_ = array[pindex];                                                  \
                array[pindex] = array[high];                                              \
                array[high] = _tmp_;                                                      \
                                                                                          \
                /* Tail recursion */                                                      \
                if (pindex - low < high - pindex)                                         \
                {                                                                         \
                    PFX##_impl_sort_quicksort(array, cmp, low, pindex - 1);               \
                                                                                          \
                    low = pindex + 1;                                                     \
                }                                                                         \
                else                                                                      \
                {                                                                         \
                    PFX##_impl_sort_quicksort(array, cmp, pindex + 1, high);              \
                                                                                          \
                    high = pindex - 1;                                                    \
                }                                                                         \
            }                                                                             \
        }                                                                                 \
    }                                                                                     \
                                                                                          \
    void PFX##_impl_sort_insertion(V *array, int (*cmp)(V, V), size_t low, size_t high)   \
    {                                                                                     \
        for (size_t i = low + 1; i <= high; i++)                                          \
        {                                                                                 \
            V _tmp_ = array[i];                                                           \
            size_t j = i;                                                                 \
                                                                                          \
            while (j > low && cmp(array[j - 1], _tmp_) > 0)                               \
            {                                                                             \
                array[j] = array[j - 1];                                                  \
                j--;                                                                      \
            }                                                                             \
                                                                                          \
            array[j] = _tmp_;                                                             \
        }                                                                                 \
    }                                                                                     \
                                                                                          \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_list_)                  \
    {                                                                                     \
        struct SNAME##_iter iter;                                                         \
                                                                                          \
        PFX##_iter_init(&iter, _list_);                                                   \
        PFX##_iter_to_start(&iter);                                                       \
                                                                                          \
        return iter;                                                                      \
    }                                                                                     \
                                                                                          \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_list_)                    \
    {                                                                                     \
        struct SNAME##_iter iter;                                                         \
                                                                                          \
        PFX##_iter_init(&iter, _list_);                                                   \
        PFX##_iter_to_end(&iter);                                                         \
                                                                                          \
        return iter;                                                                      \
    }

#endif /* CMC_SORTEDLIST_H */
