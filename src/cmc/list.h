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

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * List specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_string_fmt_list = "struct %s<%s> "
                                         "at %p { "
                                         "buffer:%p, "
                                         "capacity:%" PRIuMAX ", "
                                         "count:%" PRIuMAX ", "
                                         "flag:%d, "
                                         "f_val:%p, "
                                         "alloc:%p, "
                                         "callbacks:%p }";

#define CMC_GENERATE_LIST(PFX, SNAME, V)    \
    CMC_GENERATE_LIST_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_LIST_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_LIST_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_LIST_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_LIST_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_LIST_SOURCE(PFX, SNAME, V)

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_LIST_HEADER(PFX, SNAME, V)                                \
                                                                               \
    /* List Structure */                                                       \
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
        struct SNAME##_ftab_val *f_val;                                        \
                                                                               \
        /* Function that returns an iterator to the start of the list */       \
        struct SNAME##_iter (*it_start)(struct SNAME *);                       \
                                                                               \
        /* Function that returns an iterator to the end of the list */         \
        struct SNAME##_iter (*it_end)(struct SNAME *);                         \
                                                                               \
        /* Custom allocation functions */                                      \
        struct cmc_alloc_node *alloc;                                          \
                                                                               \
        /* Custom callback functions */                                        \
        struct cmc_callbacks *callbacks;                                       \
    };                                                                         \
                                                                               \
    /* Value struct function table */                                          \
    struct SNAME##_ftab_val                                                    \
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
    /* List Iterator */                                                        \
    struct SNAME##_iter                                                        \
    {                                                                          \
        /* Target list */                                                      \
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
    struct SNAME *PFX##_new(size_t capacity, struct SNAME##_ftab_val *f_val);  \
    struct SNAME *PFX##_new_custom(                                            \
        size_t capacity, struct SNAME##_ftab_val *f_val,                       \
        struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);        \
    void PFX##_clear(struct SNAME *_list_);                                    \
    void PFX##_free(struct SNAME *_list_);                                     \
    /* Customization of Allocation and Callbacks */                            \
    void PFX##_customize(struct SNAME *_list_, struct cmc_alloc_node *alloc,   \
                         struct cmc_callbacks *callbacks);                     \
    /* Collection Input and Output */                                          \
    bool PFX##_push_front(struct SNAME *_list_, V element);                    \
    bool PFX##_push_at(struct SNAME *_list_, V element, size_t index);         \
    bool PFX##_push_back(struct SNAME *_list_, V element);                     \
    bool PFX##_pop_front(struct SNAME *_list_);                                \
    bool PFX##_pop_at(struct SNAME *_list_, size_t index);                     \
    bool PFX##_pop_back(struct SNAME *_list_);                                 \
    /* Collection Sequence Input and Output */                                 \
    bool PFX##_seq_push_front(struct SNAME *_list_, V *elements, size_t size); \
    bool PFX##_seq_push_at(struct SNAME *_list_, V *elements, size_t size,     \
                           size_t index);                                      \
    bool PFX##_seq_push_back(struct SNAME *_list_, V *elements, size_t size);  \
    bool PFX##_seq_pop_at(struct SNAME *_list_, size_t from, size_t to);       \
    struct SNAME *PFX##_seq_sublist(struct SNAME *_list_, size_t from,         \
                                    size_t to);                                \
    /* Element Access */                                                       \
    V PFX##_front(struct SNAME *_list_);                                       \
    V PFX##_get(struct SNAME *_list_, size_t index);                           \
    V *PFX##_get_ref(struct SNAME *_list_, size_t index);                      \
    V PFX##_back(struct SNAME *_list_);                                        \
    size_t PFX##_index_of(struct SNAME *_list_, V element, bool from_start);   \
    /* Collection State */                                                     \
    bool PFX##_contains(struct SNAME *_list_, V element);                      \
    bool PFX##_empty(struct SNAME *_list_);                                    \
    bool PFX##_full(struct SNAME *_list_);                                     \
    size_t PFX##_count(struct SNAME *_list_);                                  \
    bool PFX##_fits(struct SNAME *_list_, size_t size);                        \
    size_t PFX##_capacity(struct SNAME *_list_);                               \
    int PFX##_flag(struct SNAME *_list_);                                      \
    /* Collection Utility */                                                   \
    bool PFX##_resize(struct SNAME *_list_, size_t capacity);                  \
    struct SNAME *PFX##_copy_of(struct SNAME *_list_);                         \
    bool PFX##_equals(struct SNAME *_list1_, struct SNAME *_list2_);           \
    struct cmc_string PFX##_to_string(struct SNAME *_list_);                   \
    bool PFX##_print(struct SNAME *_list_, FILE *fptr);                        \
                                                                               \
    /* Iterator Functions */                                                   \
    /* Iterator Allocation and Deallocation */                                 \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);                 \
    void PFX##_iter_free(struct SNAME##_iter *iter);                           \
    /* Iterator Initialization */                                              \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);     \
    /* Iterator State */                                                       \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                          \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                            \
    /* Iterator Movement */                                                    \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                       \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                         \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                           \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                           \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);          \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);           \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);            \
    /* Iterator Access */                                                      \
    V PFX##_iter_value(struct SNAME##_iter *iter);                             \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter);                           \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_LIST_SOURCE(PFX, SNAME, V)                                \
                                                                               \
    /* Implementation Detail Functions */                                      \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_list_);      \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_list_);        \
                                                                               \
    struct SNAME *PFX##_new(size_t capacity, struct SNAME##_ftab_val *f_val)   \
    {                                                                          \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;                \
                                                                               \
        if (capacity < 1)                                                      \
            return NULL;                                                       \
                                                                               \
        struct SNAME *_list_ = alloc->malloc(sizeof(struct SNAME));            \
                                                                               \
        if (!_list_)                                                           \
            return NULL;                                                       \
                                                                               \
        _list_->buffer = alloc->calloc(capacity, sizeof(V));                   \
                                                                               \
        if (!_list_->buffer)                                                   \
        {                                                                      \
            alloc->free(_list_);                                               \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _list_->capacity = capacity;                                           \
        _list_->count = 0;                                                     \
        _list_->flag = cmc_flags.OK;                                           \
        _list_->f_val = f_val;                                                 \
        _list_->alloc = alloc;                                                 \
        _list_->callbacks = NULL;                                              \
        _list_->it_start = PFX##_impl_it_start;                                \
        _list_->it_end = PFX##_impl_it_end;                                    \
                                                                               \
        return _list_;                                                         \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_new_custom(                                            \
        size_t capacity, struct SNAME##_ftab_val *f_val,                       \
        struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)         \
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
        struct SNAME *_list_ = alloc->malloc(sizeof(struct SNAME));            \
                                                                               \
        if (!_list_)                                                           \
            return NULL;                                                       \
                                                                               \
        _list_->buffer = alloc->calloc(capacity, sizeof(V));                   \
                                                                               \
        if (!_list_->buffer)                                                   \
        {                                                                      \
            alloc->free(_list_);                                               \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _list_->capacity = capacity;                                           \
        _list_->count = 0;                                                     \
        _list_->flag = cmc_flags.OK;                                           \
        _list_->f_val = f_val;                                                 \
        _list_->alloc = alloc;                                                 \
        _list_->callbacks = callbacks;                                         \
        _list_->it_start = PFX##_impl_it_start;                                \
        _list_->it_end = PFX##_impl_it_end;                                    \
                                                                               \
        return _list_;                                                         \
    }                                                                          \
                                                                               \
    void PFX##_clear(struct SNAME *_list_)                                     \
    {                                                                          \
        if (_list_->f_val->free)                                               \
        {                                                                      \
            for (size_t i = 0; i < _list_->count; i++)                         \
                _list_->f_val->free(_list_->buffer[i]);                        \
        }                                                                      \
                                                                               \
        memset(_list_->buffer, 0, sizeof(V) * _list_->capacity);               \
                                                                               \
        _list_->count = 0;                                                     \
        _list_->flag = cmc_flags.OK;                                           \
    }                                                                          \
                                                                               \
    void PFX##_free(struct SNAME *_list_)                                      \
    {                                                                          \
        if (_list_->f_val->free)                                               \
        {                                                                      \
            for (size_t i = 0; i < _list_->count; i++)                         \
                _list_->f_val->free(_list_->buffer[i]);                        \
        }                                                                      \
                                                                               \
        _list_->alloc->free(_list_->buffer);                                   \
        _list_->alloc->free(_list_);                                           \
    }                                                                          \
                                                                               \
    void PFX##_customize(struct SNAME *_list_, struct cmc_alloc_node *alloc,   \
                         struct cmc_callbacks *callbacks)                      \
    {                                                                          \
        if (alloc)                                                             \
            _list_->alloc = alloc;                                             \
                                                                               \
        if (callbacks)                                                         \
            _list_->callbacks = callbacks;                                     \
                                                                               \
        _list_->flag = cmc_flags.OK;                                           \
    }                                                                          \
                                                                               \
    bool PFX##_push_front(struct SNAME *_list_, V element)                     \
    {                                                                          \
        if (PFX##_full(_list_))                                                \
        {                                                                      \
            if (!PFX##_resize(_list_, _list_->count * 2))                      \
                return false;                                                  \
        }                                                                      \
                                                                               \
        if (!PFX##_empty(_list_))                                              \
        {                                                                      \
            memmove(_list_->buffer + 1, _list_->buffer,                        \
                    _list_->count * sizeof(V));                                \
        }                                                                      \
                                                                               \
        _list_->buffer[0] = element;                                           \
                                                                               \
        _list_->count++;                                                       \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->create)                                         \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_push_at(struct SNAME *_list_, V element, size_t index)          \
    {                                                                          \
        if (index > _list_->count)                                             \
        {                                                                      \
            _list_->flag = cmc_flags.RANGE;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (PFX##_full(_list_))                                                \
        {                                                                      \
            if (!PFX##_resize(_list_, _list_->count * 2))                      \
                return false;                                                  \
        }                                                                      \
                                                                               \
        memmove(_list_->buffer + index + 1, _list_->buffer + index,            \
                (_list_->count - index) * sizeof(V));                          \
                                                                               \
        _list_->buffer[index] = element;                                       \
        _list_->count++;                                                       \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->create)                                         \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_push_back(struct SNAME *_list_, V element)                      \
    {                                                                          \
        if (PFX##_full(_list_))                                                \
        {                                                                      \
            if (!PFX##_resize(_list_, _list_->count * 2))                      \
                return false;                                                  \
        }                                                                      \
                                                                               \
        _list_->buffer[_list_->count++] = element;                             \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->create)                                         \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_pop_front(struct SNAME *_list_)                                 \
    {                                                                          \
        if (PFX##_empty(_list_))                                               \
        {                                                                      \
            _list_->flag = cmc_flags.EMPTY;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        memmove(_list_->buffer, _list_->buffer + 1,                            \
                (_list_->count - 1) * sizeof(V));                              \
                                                                               \
        _list_->buffer[--_list_->count] = (V){ 0 };                            \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->delete)                                         \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_pop_at(struct SNAME *_list_, size_t index)                      \
    {                                                                          \
        if (PFX##_empty(_list_))                                               \
        {                                                                      \
            _list_->flag = cmc_flags.EMPTY;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (index >= _list_->count)                                            \
        {                                                                      \
            _list_->flag = cmc_flags.RANGE;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        memmove(_list_->buffer + index, _list_->buffer + index + 1,            \
                (_list_->count - index - 1) * sizeof(V));                      \
                                                                               \
        _list_->buffer[--_list_->count] = (V){ 0 };                            \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->delete)                                         \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_pop_back(struct SNAME *_list_)                                  \
    {                                                                          \
        if (PFX##_empty(_list_))                                               \
            return false;                                                      \
                                                                               \
        _list_->buffer[--_list_->count] = (V){ 0 };                            \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->delete)                                         \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_seq_push_front(struct SNAME *_list_, V *elements, size_t size)  \
    {                                                                          \
        if (size == 0)                                                         \
        {                                                                      \
            _list_->flag = cmc_flags.INVALID;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (!PFX##_fits(_list_, size))                                         \
        {                                                                      \
            if (!PFX##_resize(_list_, _list_->count + size))                   \
                return false;                                                  \
        }                                                                      \
                                                                               \
        memmove(_list_->buffer + size, _list_->buffer,                         \
                _list_->count * sizeof(V));                                    \
                                                                               \
        memcpy(_list_->buffer, elements, size * sizeof(V));                    \
                                                                               \
        _list_->count += size;                                                 \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->create)                                         \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_seq_push_at(struct SNAME *_list_, V *elements, size_t size,     \
                           size_t index)                                       \
    {                                                                          \
        if (size == 0)                                                         \
        {                                                                      \
            _list_->flag = cmc_flags.INVALID;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (index > _list_->count)                                             \
        {                                                                      \
            _list_->flag = cmc_flags.RANGE;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (index == 0)                                                        \
            return PFX##_seq_push_front(_list_, elements, size);               \
        else if (index == _list_->count)                                       \
            return PFX##_seq_push_back(_list_, elements, size);                \
                                                                               \
        if (!PFX##_fits(_list_, size))                                         \
        {                                                                      \
            if (!PFX##_resize(_list_, _list_->count + size))                   \
                return false;                                                  \
        }                                                                      \
                                                                               \
        memmove(_list_->buffer + index + size, _list_->buffer + index,         \
                (_list_->count - index) * sizeof(V));                          \
                                                                               \
        memcpy(_list_->buffer + index, elements, size * sizeof(V));            \
                                                                               \
        _list_->count += size;                                                 \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->create)                                         \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_seq_push_back(struct SNAME *_list_, V *elements, size_t size)   \
    {                                                                          \
        if (size == 0)                                                         \
        {                                                                      \
            _list_->flag = cmc_flags.INVALID;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (!PFX##_fits(_list_, size))                                         \
        {                                                                      \
            if (!PFX##_resize(_list_, _list_->count + size))                   \
                return false;                                                  \
        }                                                                      \
                                                                               \
        memcpy(_list_->buffer + _list_->count, elements, size * sizeof(V));    \
                                                                               \
        _list_->count += size;                                                 \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->create)                                         \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_seq_pop_at(struct SNAME *_list_, size_t from, size_t to)        \
    {                                                                          \
        if (from > to)                                                         \
        {                                                                      \
            _list_->flag = cmc_flags.INVALID;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (to >= _list_->count)                                               \
        {                                                                      \
            _list_->flag = cmc_flags.RANGE;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        size_t length = (to - from + 1);                                       \
                                                                               \
        memmove(_list_->buffer + from, _list_->buffer + to + 1,                \
                (_list_->count - to - 1) * sizeof(V));                         \
                                                                               \
        memset(_list_->buffer + _list_->count - length, 0,                     \
               length * sizeof(V));                                            \
                                                                               \
        _list_->count -= to - from + 1;                                        \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->delete)                                         \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_seq_sublist(struct SNAME *_list_, size_t from,         \
                                    size_t to)                                 \
    {                                                                          \
        if (from > to)                                                         \
        {                                                                      \
            _list_->flag = cmc_flags.INVALID;                                  \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (to >= _list_->count)                                               \
        {                                                                      \
            _list_->flag = cmc_flags.RANGE;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        size_t length = to - from + 1;                                         \
                                                                               \
        struct SNAME *result = PFX##_new_custom(                               \
            length, _list_->f_val, _list_->alloc, _list_->callbacks);          \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _list_->flag = cmc_flags.ALLOC;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        memcpy(result->buffer, _list_->buffer, length * sizeof(V));            \
                                                                               \
        memmove(_list_->buffer + from, _list_->buffer + to + 1,                \
                (_list_->count - to - 1) * sizeof(V));                         \
                                                                               \
        memset(_list_->buffer + _list_->count - length, 0,                     \
               length * sizeof(V));                                            \
                                                                               \
        _list_->count -= length;                                               \
        result->count = length;                                                \
                                                                               \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->enabled && _list_->callbacks->delete)           \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    V PFX##_front(struct SNAME *_list_)                                        \
    {                                                                          \
        if (PFX##_empty(_list_))                                               \
        {                                                                      \
            _list_->flag = cmc_flags.EMPTY;                                    \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->read)                                           \
            _list_->callbacks->read();                                         \
                                                                               \
        return _list_->buffer[0];                                              \
    }                                                                          \
                                                                               \
    V PFX##_get(struct SNAME *_list_, size_t index)                            \
    {                                                                          \
        if (PFX##_empty(_list_))                                               \
        {                                                                      \
            _list_->flag = cmc_flags.EMPTY;                                    \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        if (index >= _list_->count)                                            \
        {                                                                      \
            _list_->flag = cmc_flags.RANGE;                                    \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->read)                                           \
            _list_->callbacks->read();                                         \
                                                                               \
        return _list_->buffer[index];                                          \
    }                                                                          \
                                                                               \
    V *PFX##_get_ref(struct SNAME *_list_, size_t index)                       \
    {                                                                          \
        if (PFX##_empty(_list_))                                               \
        {                                                                      \
            _list_->flag = cmc_flags.EMPTY;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (index >= _list_->count)                                            \
        {                                                                      \
            _list_->flag = cmc_flags.RANGE;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->read)                                           \
            _list_->callbacks->read();                                         \
                                                                               \
        return &(_list_->buffer[index]);                                       \
    }                                                                          \
                                                                               \
    V PFX##_back(struct SNAME *_list_)                                         \
    {                                                                          \
        if (PFX##_empty(_list_))                                               \
        {                                                                      \
            _list_->flag = cmc_flags.EMPTY;                                    \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->read)                                           \
            _list_->callbacks->read();                                         \
                                                                               \
        return _list_->buffer[_list_->count - 1];                              \
    }                                                                          \
                                                                               \
    size_t PFX##_index_of(struct SNAME *_list_, V element, bool from_start)    \
    {                                                                          \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        size_t result = _list_->count;                                         \
                                                                               \
        if (from_start)                                                        \
        {                                                                      \
            for (size_t i = 0; i < _list_->count; i++)                         \
            {                                                                  \
                if (_list_->f_val->cmp(_list_->buffer[i], element) == 0)       \
                {                                                              \
                    result = i;                                                \
                    break;                                                     \
                }                                                              \
            }                                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            for (size_t i = _list_->count; i > 0; i--)                         \
            {                                                                  \
                if (_list_->f_val->cmp(_list_->buffer[i - 1], element) == 0)   \
                {                                                              \
                    result = i - 1;                                            \
                    break;                                                     \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->read)                                           \
            _list_->callbacks->read();                                         \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_contains(struct SNAME *_list_, V element)                       \
    {                                                                          \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        bool result = false;                                                   \
                                                                               \
        for (size_t i = 0; i < _list_->count; i++)                             \
        {                                                                      \
            if (_list_->f_val->cmp(_list_->buffer[i], element) == 0)           \
            {                                                                  \
                result = true;                                                 \
                break;                                                         \
            }                                                                  \
        }                                                                      \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->read)                                           \
            _list_->callbacks->read();                                         \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_empty(struct SNAME *_list_)                                     \
    {                                                                          \
        return _list_->count == 0;                                             \
    }                                                                          \
                                                                               \
    bool PFX##_full(struct SNAME *_list_)                                      \
    {                                                                          \
        return _list_->count >= _list_->capacity;                              \
    }                                                                          \
                                                                               \
    size_t PFX##_count(struct SNAME *_list_)                                   \
    {                                                                          \
        return _list_->count;                                                  \
    }                                                                          \
                                                                               \
    bool PFX##_fits(struct SNAME *_list_, size_t size)                         \
    {                                                                          \
        return _list_->count + size <= _list_->capacity;                       \
    }                                                                          \
                                                                               \
    size_t PFX##_capacity(struct SNAME *_list_)                                \
    {                                                                          \
        return _list_->capacity;                                               \
    }                                                                          \
                                                                               \
    int PFX##_flag(struct SNAME *_list_)                                       \
    {                                                                          \
        return _list_->flag;                                                   \
    }                                                                          \
                                                                               \
    bool PFX##_resize(struct SNAME *_list_, size_t capacity)                   \
    {                                                                          \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_list_->capacity == capacity)                                      \
            return true;                                                       \
                                                                               \
        if (capacity < _list_->count)                                          \
        {                                                                      \
            _list_->flag = cmc_flags.INVALID;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        V *new_buffer =                                                        \
            _list_->alloc->realloc(_list_->buffer, sizeof(V) * capacity);      \
                                                                               \
        if (!new_buffer)                                                       \
        {                                                                      \
            _list_->flag = cmc_flags.ALLOC;                                    \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* TODO zero out new slots */                                          \
                                                                               \
        _list_->buffer = new_buffer;                                           \
        _list_->capacity = capacity;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->enabled &&                 \
            _list_->callbacks->resize)                                         \
            _list_->callbacks->resize();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_copy_of(struct SNAME *_list_)                          \
    {                                                                          \
        struct SNAME *result =                                                 \
            PFX##_new_custom(_list_->capacity, _list_->f_val, _list_->alloc,   \
                             _list_->callbacks);                               \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _list_->flag = cmc_flags.ALLOC;                                    \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (_list_->f_val->cpy)                                                \
        {                                                                      \
            for (size_t i = 0; i < _list_->count; i++)                         \
                result->buffer[i] = _list_->f_val->cpy(_list_->buffer[i]);     \
        }                                                                      \
        else                                                                   \
            memcpy(result->buffer, _list_->buffer, sizeof(V) * _list_->count); \
                                                                               \
        result->count = _list_->count;                                         \
                                                                               \
        _list_->flag = cmc_flags.OK;                                           \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_equals(struct SNAME *_list1_, struct SNAME *_list2_)            \
    {                                                                          \
        _list1_->flag = cmc_flags.OK;                                          \
        _list2_->flag = cmc_flags.OK;                                          \
                                                                               \
        if (_list1_->count != _list2_->count)                                  \
            return false;                                                      \
                                                                               \
        for (size_t i = 0; i < _list1_->count; i++)                            \
        {                                                                      \
            if (_list1_->f_val->cmp(_list1_->buffer[i], _list2_->buffer[i]) != \
                0)                                                             \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct cmc_string PFX##_to_string(struct SNAME *_list_)                    \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *l_ = _list_;                                             \
                                                                               \
        int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_list, #SNAME,   \
                         #V, l_, l_->buffer, l_->capacity, l_->count,          \
                         l_->flag, l_->f_val, l_->alloc, l_->callbacks);       \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    bool PFX##_print(struct SNAME *_list_, FILE *fptr)                         \
    {                                                                          \
        for (size_t i = 0; i < _list_->count; i++)                             \
        {                                                                      \
            if (!_list_->f_val->str(fptr, _list_->buffer[i]))                  \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
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
        iter->cursor = 0;                                                      \
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
        if (!PFX##_empty(iter->target))                                        \
        {                                                                      \
            iter->cursor = 0;                                                  \
            iter->start = true;                                                \
            iter->end = PFX##_empty(iter->target);                             \
        }                                                                      \
    }                                                                          \
                                                                               \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                          \
    {                                                                          \
        if (!PFX##_empty(iter->target))                                        \
        {                                                                      \
            iter->start = PFX##_empty(iter->target);                           \
            iter->cursor =                                                     \
                PFX##_empty(iter->target) ? 0 : iter->target->count - 1;       \
            iter->end = true;                                                  \
        }                                                                      \
    }                                                                          \
                                                                               \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                            \
    {                                                                          \
        if (iter->end)                                                         \
            return false;                                                      \
                                                                               \
        if (iter->cursor + 1 == iter->target->count)                           \
        {                                                                      \
            iter->end = true;                                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        iter->start = PFX##_empty(iter->target);                               \
                                                                               \
        iter->cursor++;                                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                            \
    {                                                                          \
        if (iter->start)                                                       \
            return false;                                                      \
                                                                               \
        if (iter->cursor == 0)                                                 \
        {                                                                      \
            iter->start = true;                                                \
            return false;                                                      \
        }                                                                      \
                                                                               \
        iter->end = PFX##_empty(iter->target);                                 \
                                                                               \
        iter->cursor--;                                                        \
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
        if (iter->cursor + 1 == iter->target->count)                           \
        {                                                                      \
            iter->end = true;                                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (steps == 0 || iter->cursor + steps >= iter->target->count)         \
            return false;                                                      \
                                                                               \
        iter->start = PFX##_empty(iter->target);                               \
                                                                               \
        iter->cursor += steps;                                                 \
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
        if (iter->cursor == 0)                                                 \
        {                                                                      \
            iter->start = true;                                                \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (steps == 0 || iter->cursor < steps)                                \
            return false;                                                      \
                                                                               \
        iter->end = PFX##_empty(iter->target);                                 \
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
        if (iter->cursor > index)                                              \
            return PFX##_iter_rewind(iter, iter->cursor - index);              \
        else if (iter->cursor < index)                                         \
            return PFX##_iter_advance(iter, index - iter->cursor);             \
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
        return iter->cursor;                                                   \
    }                                                                          \
                                                                               \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_list_)       \
    {                                                                          \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _list_);                                        \
        PFX##_iter_to_start(&iter);                                            \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_list_)         \
    {                                                                          \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _list_);                                        \
        PFX##_iter_to_end(&iter);                                              \
                                                                               \
        return iter;                                                           \
    }

#endif /* CMC_LIST_H */
