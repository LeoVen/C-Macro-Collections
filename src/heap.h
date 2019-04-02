/**
 * heap.h
 *
 * Creation Date: 25/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/********************************************************************** HEAP */
/*****************************************************************************/

#ifndef CMC_HEAP_H
#define CMC_HEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum HeapOrder
{
    MaxHeap = 1,
    MinHeap = -1
} HeapOrder;

#define HEAP_GENERATE(PFX, SNAME, FMOD, K, V)    \
    HEAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
    HEAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V) \
    HEAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)

/* PRIVATE *******************************************************************/
#define HEAP_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    HEAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define HEAP_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    HEAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)             \
    HEAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* PUBLIC ********************************************************************/
#define HEAP_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    HEAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)            \
    HEAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define HEAP_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    HEAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* STRUCT ********************************************************************/
#define HEAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
                                                     \
    struct SNAME##_s                                 \
    {                                                \
        V *buffer;                                   \
        size_t capacity;                             \
        size_t count;                                \
        enum HeapOrder HO;                           \
        int (*cmp)(V, V);                            \
    };                                               \
                                                     \
    struct SNAME##_iter_s                            \
    {                                                \
        struct SNAME##_s *target;                    \
        size_t cursor;                               \
        bool start;                                  \
        bool end;                                    \
    };                                               \
                                                     \
/* HEADER ********************************************************************/
#define HEAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)                         \
                                                                             \
    typedef struct SNAME##_s SNAME;                                          \
    typedef struct SNAME##_iter_s SNAME##_iter;                              \
                                                                             \
    FMOD SNAME *PFX##_new(size_t size, HeapOrder HO, int (*compare)(V, V));  \
    FMOD void PFX##_free(SNAME *_heap_);                                     \
    FMOD bool PFX##_insert(SNAME *_heap_, V element);                        \
    FMOD bool PFX##_remove(SNAME *_heap_, V *result);                        \
    FMOD bool PFX##_insert_if(SNAME *_heap_, V element, bool condition);     \
    FMOD bool PFX##_remove_if(SNAME *_heap_, V *result, bool condition);     \
    FMOD V PFX##_peek(SNAME *_heap_);                                        \
    FMOD bool PFX##_empty(SNAME *_heap_);                                    \
    FMOD bool PFX##_full(SNAME *_heap_);                                     \
    FMOD size_t PFX##_count(SNAME *_heap_);                                  \
    FMOD size_t PFX##_capacity(SNAME *_heap_);                               \
                                                                             \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);             \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                          \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                            \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                        \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                          \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index); \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index); \
                                                                             \
/* SOURCE ********************************************************************/
#define HEAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)                                              \
                                                                                                  \
    FMOD bool PFX##_grow(SNAME *_heap_);                                                          \
    FMOD size_t PFX##_p(size_t index);                                                            \
    FMOD size_t PFX##_l(size_t index);                                                            \
    FMOD size_t PFX##_r(size_t index);                                                            \
    FMOD bool PFX##_float_up(SNAME *_heap_, size_t index);                                        \
    FMOD bool PFX##_float_down(SNAME *_heap_, size_t index);                                      \
                                                                                                  \
    FMOD SNAME *PFX##_new(size_t size, HeapOrder HO, int (*compare)(V, V))                        \
    {                                                                                             \
        if (size < 1)                                                                             \
            return NULL;                                                                          \
                                                                                                  \
        if (HO != MinHeap && HO != MaxHeap)                                                       \
            return NULL;                                                                          \
                                                                                                  \
        SNAME *_heap_ = malloc(sizeof(SNAME));                                                    \
                                                                                                  \
        if (!_heap_)                                                                              \
            return NULL;                                                                          \
                                                                                                  \
        _heap_->buffer = malloc(sizeof(V) * size);                                                \
                                                                                                  \
        if (!_heap_->buffer)                                                                      \
        {                                                                                         \
            free(_heap_);                                                                         \
            return NULL;                                                                          \
        }                                                                                         \
                                                                                                  \
        memset(_heap_->buffer, 0, sizeof(V) * size);                                              \
                                                                                                  \
        _heap_->capacity = size;                                                                  \
        _heap_->count = 0;                                                                        \
        _heap_->HO = HO;                                                                          \
        _heap_->cmp = compare;                                                                    \
                                                                                                  \
        return _heap_;                                                                            \
    }                                                                                             \
                                                                                                  \
    FMOD void PFX##_free(SNAME *_heap_)                                                           \
    {                                                                                             \
        free(_heap_->buffer);                                                                     \
        free(_heap_);                                                                             \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_insert(SNAME *_heap_, V element)                                              \
    {                                                                                             \
        if (PFX##_full(_heap_))                                                                   \
        {                                                                                         \
            if (!PFX##_grow(_heap_))                                                              \
                return false;                                                                     \
        }                                                                                         \
                                                                                                  \
        if (_heap_->count == 0)                                                                   \
        {                                                                                         \
            _heap_->buffer[_heap_->count++] = element;                                            \
            return true;                                                                          \
        }                                                                                         \
                                                                                                  \
        _heap_->buffer[_heap_->count++] = element;                                                \
                                                                                                  \
        if (!PFX##_float_up(_heap_, _heap_->count - 1))                                           \
            return false;                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_remove(SNAME *_heap_, V *result)                                              \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return false;                                                                         \
                                                                                                  \
        *result = _heap_->buffer[0];                                                              \
        _heap_->buffer[0] = _heap_->buffer[_heap_->count - 1];                                    \
        _heap_->buffer[_heap_->count - 1] = 0;                                                    \
                                                                                                  \
        _heap_->count--;                                                                          \
                                                                                                  \
        if (!PFX##_float_down(_heap_, 0))                                                         \
            return false;                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_insert_if(SNAME *_heap_, V element, bool condition)                           \
    {                                                                                             \
        if (condition)                                                                            \
            return PFX##_insert(_heap_, element);                                                 \
                                                                                                  \
        return false;                                                                             \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_remove_if(SNAME *_heap_, V *result, bool condition)                           \
    {                                                                                             \
        if (condition)                                                                            \
            return PFX##_remove(_heap_, result);                                                  \
                                                                                                  \
        return false;                                                                             \
    }                                                                                             \
                                                                                                  \
    FMOD V PFX##_peek(SNAME *_heap_)                                                              \
    {                                                                                             \
        if (PFX##_empty(_heap_))                                                                  \
            return 0;                                                                             \
                                                                                                  \
        return _heap_->buffer[0];                                                                 \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_empty(SNAME *_heap_)                                                          \
    {                                                                                             \
        return _heap_->count == 0;                                                                \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_full(SNAME *_heap_)                                                           \
    {                                                                                             \
        return _heap_->count >= _heap_->capacity;                                                 \
    }                                                                                             \
                                                                                                  \
    FMOD size_t PFX##_count(SNAME *_heap_)                                                        \
    {                                                                                             \
        return _heap_->count;                                                                     \
    }                                                                                             \
                                                                                                  \
    FMOD size_t PFX##_capacity(SNAME *_heap_)                                                     \
    {                                                                                             \
        return _heap_->capacity;                                                                  \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_grow(SNAME *_heap_)                                                           \
    {                                                                                             \
        size_t new_capacity = _heap_->capacity * 2;                                               \
                                                                                                  \
        V *new_buffer = realloc(_heap_->buffer, sizeof(V) * new_capacity);                        \
                                                                                                  \
        if (!new_buffer)                                                                          \
            return false;                                                                         \
                                                                                                  \
        _heap_->buffer = new_buffer;                                                              \
        _heap_->capacity = new_capacity;                                                          \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    FMOD size_t PFX##_p(size_t index)                                                             \
    {                                                                                             \
        if (index == 0)                                                                           \
            return 0;                                                                             \
                                                                                                  \
        return (index - 1) / 2;                                                                   \
    }                                                                                             \
                                                                                                  \
    FMOD size_t PFX##_l(size_t index)                                                             \
    {                                                                                             \
        return 2 * index + 1;                                                                     \
    }                                                                                             \
                                                                                                  \
    FMOD size_t PFX##_r(size_t index)                                                             \
    {                                                                                             \
        return 2 * index + 2;                                                                     \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_float_up(SNAME *_heap_, size_t index)                                         \
    {                                                                                             \
        size_t C = index;                                                                         \
        V child = _heap_->buffer[C];                                                              \
        V parent = _heap_->buffer[PFX##_p(C)];                                                    \
                                                                                                  \
        int mod = _heap_->HO;                                                                     \
                                                                                                  \
        while (C > 0 && _heap_->cmp(child, parent) * mod > 0)                                     \
        {                                                                                         \
            V tmp = _heap_->buffer[C];                                                            \
            _heap_->buffer[C] = _heap_->buffer[PFX##_p(C)];                                       \
            _heap_->buffer[PFX##_p(C)] = tmp;                                                     \
                                                                                                  \
            C = PFX##_p(C);                                                                       \
                                                                                                  \
            child = _heap_->buffer[C];                                                            \
            parent = _heap_->buffer[PFX##_p(C)];                                                  \
        }                                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_float_down(SNAME *_heap_, size_t index)                                       \
    {                                                                                             \
        int mod = _heap_->HO;                                                                     \
                                                                                                  \
        while (index < _heap_->count)                                                             \
        {                                                                                         \
            size_t L = PFX##_l(index);                                                            \
            size_t R = PFX##_r(index);                                                            \
            size_t C = index;                                                                     \
                                                                                                  \
            if (L < _heap_->count && _heap_->cmp(_heap_->buffer[L], _heap_->buffer[C]) * mod > 0) \
            {                                                                                     \
                C = L;                                                                            \
            }                                                                                     \
                                                                                                  \
            if (R < _heap_->count && _heap_->cmp(_heap_->buffer[R], _heap_->buffer[C]) * mod > 0) \
            {                                                                                     \
                C = R;                                                                            \
            }                                                                                     \
                                                                                                  \
            if (C != index)                                                                       \
            {                                                                                     \
                V tmp = _heap_->buffer[index];                                                    \
                _heap_->buffer[index] = _heap_->buffer[C];                                        \
                _heap_->buffer[C] = tmp;                                                          \
                                                                                                  \
                index = C;                                                                        \
            }                                                                                     \
            else                                                                                  \
                break;                                                                            \
        }                                                                                         \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target)                                   \
    {                                                                                             \
        iter->target = target;                                                                    \
        iter->cursor = 0;                                                                         \
        iter->start = true;                                                                       \
        iter->end = PFX##_empty(target);                                                          \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                                \
    {                                                                                             \
        return iter->cursor == 0 && iter->start;                                                  \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                                  \
    {                                                                                             \
        return iter->cursor == iter->target->count - 1 && iter->end;                              \
    }                                                                                             \
                                                                                                  \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter)                                              \
    {                                                                                             \
        iter->cursor = 0;                                                                         \
        iter->start = true;                                                                       \
        iter->end = PFX##_empty(iter->target);                                                    \
    }                                                                                             \
                                                                                                  \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter)                                                \
    {                                                                                             \
        iter->cursor = iter->target->count - 1;                                                   \
        iter->start = PFX##_empty(iter->target);                                                  \
        iter->end = true;                                                                         \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index)                       \
    {                                                                                             \
        if (iter->end)                                                                            \
            return false;                                                                         \
                                                                                                  \
        *index = iter->cursor;                                                                    \
        *result = iter->target->buffer[iter->cursor];                                             \
        iter->start = false;                                                                      \
                                                                                                  \
        if (iter->cursor == iter->target->count - 1)                                              \
            iter->end = true;                                                                     \
        else                                                                                      \
            iter->cursor++;                                                                       \
                                                                                                  \
        return true;                                                                              \
    }                                                                                             \
                                                                                                  \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index)                       \
    {                                                                                             \
        if (iter->start)                                                                          \
            return false;                                                                         \
                                                                                                  \
        *index = iter->cursor;                                                                    \
        *result = iter->target->buffer[iter->cursor];                                             \
        iter->end = false;                                                                        \
                                                                                                  \
        if (iter->cursor == 0)                                                                    \
            iter->start = true;                                                                   \
        else                                                                                      \
            iter->cursor--;                                                                       \
                                                                                                  \
        return true;                                                                              \
    }

#endif /* CMC_HEAP_H */
