/**
 * list.h
 *
 * Creation Date: 12/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/********************************************************************** LIST */
/*****************************************************************************/

#ifndef CMC_LIST_H
#define CMC_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LIST_GENERATE(PFX, SNAME, FMOD, K, V)    \
    LIST_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_HEADER(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)

/* PRIVATE *******************************************************************/
#define LIST_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define LIST_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)             \
    LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* PUBLIC ********************************************************************/
#define LIST_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)            \
    LIST_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define LIST_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* STRUCT ********************************************************************/
#define LIST_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
                                                     \
    struct SNAME##_s                                 \
    {                                                \
        V *buffer;                                   \
        size_t capacity;                             \
        size_t count;                                \
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
#define LIST_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)                                 \
                                                                                     \
    typedef struct SNAME##_s SNAME;                                                  \
    typedef struct SNAME##_iter_s SNAME##_iter;                                      \
                                                                                     \
    FMOD SNAME *PFX##_new(size_t size);                                              \
    FMOD void PFX##_free(SNAME *_list_);                                             \
    FMOD bool PFX##_push_front(SNAME *_list_, V element);                            \
    FMOD bool PFX##_push(SNAME *_list_, V element, size_t index);                    \
    FMOD bool PFX##_push_back(SNAME *_list_, V element);                             \
    FMOD bool PFX##_pop_front(SNAME *_list_);                                        \
    FMOD bool PFX##_pop(SNAME *_list_, size_t index);                                \
    FMOD bool PFX##_pop_back(SNAME *_list_);                                         \
    FMOD bool PFX##_push_if(SNAME *_list_, V element, size_t index, bool condition); \
    FMOD bool PFX##_pop_if(SNAME *_list_, size_t index, bool condition);             \
    FMOD V PFX##_back(SNAME *_list_);                                                \
    FMOD V PFX##_get(SNAME *_list_, size_t index);                                   \
    FMOD V PFX##_front(SNAME *_list_);                                               \
    FMOD bool PFX##_empty(SNAME *_list_);                                            \
    FMOD bool PFX##_full(SNAME *_list_);                                             \
    FMOD size_t PFX##_count(SNAME *_list_);                                          \
    FMOD size_t PFX##_capacity(SNAME *_list_);                                       \
                                                                                     \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);                     \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                  \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                    \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                                \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                                  \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index);         \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index);         \
                                                                                     \
/* SOURCE ********************************************************************/
#define LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)                                \
                                                                                    \
    FMOD bool PFX##_grow(SNAME *_list_);                                            \
                                                                                    \
    FMOD SNAME *PFX##_new(size_t size)                                              \
    {                                                                               \
        if (size < 1)                                                               \
            return NULL;                                                            \
                                                                                    \
        SNAME *_list_ = malloc(sizeof(SNAME));                                      \
                                                                                    \
        if (!_list_)                                                                \
            return NULL;                                                            \
                                                                                    \
        _list_->buffer = malloc(sizeof(V) * size);                                  \
                                                                                    \
        if (!_list_->buffer)                                                        \
        {                                                                           \
            free(_list_);                                                           \
            return NULL;                                                            \
        }                                                                           \
                                                                                    \
        memset(_list_->buffer, 0, sizeof(V) * size);                                \
                                                                                    \
        for (size_t i = 0; i < size; i++)                                           \
        {                                                                           \
            _list_->buffer[i] = 0;                                                  \
        }                                                                           \
                                                                                    \
        _list_->capacity = size;                                                    \
        _list_->count = 0;                                                          \
                                                                                    \
        return _list_;                                                              \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_free(SNAME *_list_)                                             \
    {                                                                               \
        free(_list_->buffer);                                                       \
        free(_list_);                                                               \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_push_front(SNAME *_list_, V element)                            \
    {                                                                               \
        if (PFX##_full(_list_))                                                     \
        {                                                                           \
            if (!PFX##_grow(_list_))                                                \
                return false;                                                       \
        }                                                                           \
                                                                                    \
        if (!PFX##_empty(_list_))                                                   \
        {                                                                           \
            for (size_t i = _list_->count; i > 0; i--)                              \
            {                                                                       \
                _list_->buffer[i] = _list_->buffer[i - 1];                          \
            }                                                                       \
        }                                                                           \
                                                                                    \
        _list_->buffer[0] = element;                                                \
                                                                                    \
        _list_->count++;                                                            \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_push(SNAME *_list_, V element, size_t index)                    \
    {                                                                               \
        if (index > _list_->count)                                                  \
            return false;                                                           \
                                                                                    \
        if (index == 0)                                                             \
        {                                                                           \
            return PFX##_push_front(_list_, element);                               \
        }                                                                           \
        else if (index == _list_->count)                                            \
        {                                                                           \
            return PFX##_push_back(_list_, element);                                \
        }                                                                           \
                                                                                    \
        if (PFX##_full(_list_))                                                     \
        {                                                                           \
            if (!PFX##_grow(_list_))                                                \
                return false;                                                       \
        }                                                                           \
                                                                                    \
        for (size_t i = _list_->count; i > index; i--)                              \
        {                                                                           \
            _list_->buffer[i] = _list_->buffer[i - 1];                              \
        }                                                                           \
                                                                                    \
        _list_->buffer[index] = element;                                            \
                                                                                    \
        _list_->count++;                                                            \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_push_back(SNAME *_list_, V element)                             \
    {                                                                               \
        if (PFX##_full(_list_))                                                     \
        {                                                                           \
            if (!PFX##_grow(_list_))                                                \
                return false;                                                       \
        }                                                                           \
                                                                                    \
        _list_->buffer[_list_->count++] = element;                                  \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_pop_front(SNAME *_list_)                                        \
    {                                                                               \
        if (PFX##_empty(_list_))                                                    \
            return false;                                                           \
                                                                                    \
        for (size_t i = 0; i < _list_->count; i++)                                  \
        {                                                                           \
            _list_->buffer[i] = _list_->buffer[i + 1];                              \
        }                                                                           \
                                                                                    \
        _list_->buffer[--_list_->count] = 0;                                        \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_pop(SNAME *_list_, size_t index)                                \
    {                                                                               \
        if (PFX##_empty(_list_))                                                    \
            return false;                                                           \
                                                                                    \
        if (index == 0)                                                             \
        {                                                                           \
            return PFX##_pop_front(_list_);                                         \
        }                                                                           \
        else if (index == _list_->count - 1)                                        \
        {                                                                           \
            return PFX##_pop_back(_list_);                                          \
        }                                                                           \
                                                                                    \
        for (size_t i = index; i < _list_->count - 1; i++)                          \
        {                                                                           \
            _list_->buffer[i] = _list_->buffer[i + 1];                              \
        }                                                                           \
                                                                                    \
        _list_->buffer[--_list_->count] = 0;                                        \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_pop_back(SNAME *_list_)                                         \
    {                                                                               \
        if (PFX##_empty(_list_))                                                    \
            return false;                                                           \
                                                                                    \
        _list_->buffer[--_list_->count] = 0;                                        \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_push_if(SNAME *_list_, V element, size_t index, bool condition) \
    {                                                                               \
        if (condition)                                                              \
            return PFX##_push(_list_, element, index);                              \
                                                                                    \
        return false;                                                               \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_pop_if(SNAME *_list_, size_t index, bool condition)             \
    {                                                                               \
        if (condition)                                                              \
            return PFX##_pop(_list_, index);                                        \
                                                                                    \
        return false;                                                               \
    }                                                                               \
                                                                                    \
    FMOD V PFX##_front(SNAME *_list_)                                               \
    {                                                                               \
        if (PFX##_empty(_list_))                                                    \
            return 0;                                                               \
                                                                                    \
        return _list_->buffer[0];                                                   \
    }                                                                               \
                                                                                    \
    FMOD V PFX##_get(SNAME *_list_, size_t index)                                   \
    {                                                                               \
        if (index >= _list_->count)                                                 \
            return 0;                                                               \
                                                                                    \
        if (PFX##_empty(_list_))                                                    \
            return 0;                                                               \
                                                                                    \
        return _list_->buffer[index];                                               \
    }                                                                               \
                                                                                    \
    FMOD V PFX##_back(SNAME *_list_)                                                \
    {                                                                               \
        if (PFX##_empty(_list_))                                                    \
            return 0;                                                               \
                                                                                    \
        return _list_->buffer[_list_->count - 1];                                   \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_empty(SNAME *_list_)                                            \
    {                                                                               \
        return _list_->count == 0;                                                  \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_full(SNAME *_list_)                                             \
    {                                                                               \
        return _list_->count >= _list_->capacity;                                   \
    }                                                                               \
                                                                                    \
    FMOD size_t PFX##_count(SNAME *_list_)                                          \
    {                                                                               \
        return _list_->count;                                                       \
    }                                                                               \
                                                                                    \
    FMOD size_t PFX##_capacity(SNAME *_list_)                                       \
    {                                                                               \
        return _list_->capacity;                                                    \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_grow(SNAME *_list_)                                             \
    {                                                                               \
        size_t new_capacity = _list_->capacity * 2;                                 \
                                                                                    \
        V *new_buffer = realloc(_list_->buffer, sizeof(V) * new_capacity);          \
                                                                                    \
        if (!new_buffer)                                                            \
            return false;                                                           \
                                                                                    \
        _list_->buffer = new_buffer;                                                \
        _list_->capacity = new_capacity;                                            \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target)                     \
    {                                                                               \
        iter->target = target;                                                      \
        iter->cursor = 0;                                                           \
        iter->start = true;                                                         \
        iter->end = PFX##_empty(target);                                            \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                  \
    {                                                                               \
        return iter->cursor == 0 && iter->start;                                    \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                    \
    {                                                                               \
        return iter->cursor == iter->target->count - 1 && iter->end;                \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter)                                \
    {                                                                               \
        iter->cursor = 0;                                                           \
        iter->start = true;                                                         \
        iter->end = PFX##_empty(iter->target);                                      \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter)                                  \
    {                                                                               \
        iter->cursor = iter->target->count - 1;                                     \
        iter->start = PFX##_empty(iter->target);                                    \
        iter->end = true;                                                           \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index)         \
    {                                                                               \
        if (iter->end)                                                              \
            return false;                                                           \
                                                                                    \
        *index = iter->cursor;                                                      \
        *result = iter->target->buffer[iter->cursor];                               \
        iter->start = false;                                                        \
                                                                                    \
        if (iter->cursor == iter->target->count - 1)                                \
            iter->end = true;                                                       \
        else                                                                        \
            iter->cursor++;                                                         \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index)         \
    {                                                                               \
        if (iter->start)                                                            \
            return false;                                                           \
                                                                                    \
        *index = iter->cursor;                                                      \
        *result = iter->target->buffer[iter->cursor];                               \
        iter->end = false;                                                          \
                                                                                    \
        if (iter->cursor == 0)                                                      \
            iter->start = true;                                                     \
        else                                                                        \
            iter->cursor--;                                                         \
                                                                                    \
        return true;                                                                \
    }

#endif /* CMC_LIST_H */
