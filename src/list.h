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

#define LIST_GENERATE(PFX, SNAME, FMOD, V)    \
    LIST_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
    LIST_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* PRIVATE *******************************************************************/
#define LIST_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define LIST_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_STRUCT(PFX, SNAME, FMOD, V)                \
    LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* PUBLIC ********************************************************************/
#define LIST_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_STRUCT(PFX, SNAME, FMOD, V)               \
    LIST_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define LIST_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* STRUCT ********************************************************************/
#define LIST_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
                                                  \
    struct SNAME##_s                              \
    {                                             \
        V *buffer;                                \
        size_t capacity;                          \
        size_t count;                             \
    };                                            \
                                                  \
    struct SNAME##_iter_s                         \
    {                                             \
        struct SNAME##_s *target;                 \
        size_t cursor;                            \
        bool start;                               \
        bool end;                                 \
    };                                            \
                                                  \
/* HEADER ********************************************************************/
#define LIST_GENERATE_HEADER(PFX, SNAME, FMOD, V)                                    \
                                                                                     \
    typedef struct SNAME##_s SNAME;                                                  \
    typedef struct SNAME##_iter_s SNAME##_iter;                                      \
                                                                                     \
    FMOD SNAME *PFX##_new(size_t size);                                              \
    FMOD SNAME *PFX##_new_from(V *elements, size_t size);                            \
    FMOD void PFX##_clear(SNAME *_list_);                                            \
    FMOD void PFX##_free(SNAME *_list_);                                             \
    FMOD bool PFX##_push_front(SNAME *_list_, V element);                            \
    FMOD bool PFX##_push(SNAME *_list_, V element, size_t index);                    \
    FMOD bool PFX##_push_back(SNAME *_list_, V element);                             \
    FMOD bool PFX##_pop_front(SNAME *_list_);                                        \
    FMOD bool PFX##_pop(SNAME *_list_, size_t index);                                \
    FMOD bool PFX##_pop_back(SNAME *_list_);                                         \
    FMOD bool PFX##_push_if(SNAME *_list_, V element, size_t index, bool condition); \
    FMOD bool PFX##_pop_if(SNAME *_list_, size_t index, bool condition);             \
    FMOD bool PFX##_prepend(SNAME *_list_, V *elements, size_t size);                \
    FMOD bool PFX##_insert(SNAME *_list_, V *elements, size_t size, size_t index);   \
    FMOD bool PFX##_append(SNAME *_list_, V *elements, size_t size);                 \
    FMOD bool PFX##_remove(SNAME *_list_, size_t from, size_t to);                   \
    FMOD V PFX##_front(SNAME *_list_);                                               \
    FMOD V PFX##_get(SNAME *_list_, size_t index);                                   \
    FMOD V PFX##_back(SNAME *_list_);                                                \
    FMOD bool PFX##_empty(SNAME *_list_);                                            \
    FMOD bool PFX##_full(SNAME *_list_);                                             \
    FMOD size_t PFX##_count(SNAME *_list_);                                          \
    FMOD bool PFX##_fits(SNAME *_list_, size_t size);                                \
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
#define LIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                                   \
                                                                                    \
    FMOD bool PFX##_grow(SNAME *_list_, size_t required);                           \
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
    FMOD SNAME *PFX##_new_from(V *elements, size_t size)                            \
    {                                                                               \
        if (size == 0)                                                              \
            return NULL;                                                            \
                                                                                    \
        SNAME *_list_ = PFX##_new(size);                                            \
                                                                                    \
        if (!_list_)                                                                \
            return NULL;                                                            \
                                                                                    \
        for (size_t i = 0; i < size; i++)                                           \
        {                                                                           \
            if (!PFX##_push_back(_list_, elements[i]))                              \
            {                                                                       \
                PFX##_free(_list_);                                                 \
                return NULL;                                                        \
            }                                                                       \
        }                                                                           \
                                                                                    \
        return _list_;                                                              \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_clear(SNAME *_list_)                                            \
    {                                                                               \
        memset(_list_->buffer, 0, sizeof(V) * _list_->capacity);                    \
                                                                                    \
        _list_->count = 0;                                                          \
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
            if (!PFX##_grow(_list_, _list_->count + 1))                             \
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
            if (!PFX##_grow(_list_, _list_->count + 1))                             \
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
            if (!PFX##_grow(_list_, _list_->count + 1))                             \
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
        if (index >= _list_->count)                                                 \
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
    FMOD bool PFX##_prepend(SNAME *_list_, V *elements, size_t size)                \
    {                                                                               \
        if (size == 0)                                                              \
            return false;                                                           \
                                                                                    \
        if (!PFX##_fits(_list_, size))                                              \
        {                                                                           \
            if (!PFX##_grow(_list_, _list_->count + size))                          \
                return false;                                                       \
        }                                                                           \
                                                                                    \
        for (size_t i = _list_->count; i > 0; i--)                                  \
        {                                                                           \
            _list_->buffer[i + size - 1] = _list_->buffer[i - 1];                   \
        }                                                                           \
                                                                                    \
        for (size_t i = 0; i < size; i++)                                           \
        {                                                                           \
            _list_->buffer[i] = elements[i];                                        \
        }                                                                           \
                                                                                    \
        _list_->count += size;                                                      \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_insert(SNAME *_list_, V *elements, size_t size, size_t index)   \
    {                                                                               \
        if (size == 0 || index > _list_->count)                                     \
            return false;                                                           \
                                                                                    \
        if (index == 0)                                                             \
            return PFX##_prepend(_list_, elements, size);                           \
        else if (index == _list_->count)                                            \
            return PFX##_append(_list_, elements, size);                            \
        else                                                                        \
        {                                                                           \
            if (!PFX##_fits(_list_, size))                                          \
            {                                                                       \
                if (!PFX##_grow(_list_, _list_->count + size))                      \
                    return false;                                                   \
            }                                                                       \
                                                                                    \
            for (size_t i = _list_->count; i >= index; i--)                         \
            {                                                                       \
                _list_->buffer[i + size] = _list_->buffer[i];                       \
            }                                                                       \
                                                                                    \
            for (size_t i = index, j = 0; j < size; i++, j++)                       \
            {                                                                       \
                _list_->buffer[i] = elements[j];                                    \
            }                                                                       \
                                                                                    \
            _list_->count += size;                                                  \
        }                                                                           \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_append(SNAME *_list_, V *elements, size_t size)                 \
    {                                                                               \
        if (size == 0)                                                              \
            return false;                                                           \
                                                                                    \
        if (!PFX##_fits(_list_, size))                                              \
        {                                                                           \
            if (!PFX##_grow(_list_, _list_->count + size))                          \
                return false;                                                       \
        }                                                                           \
                                                                                    \
        for (size_t i = _list_->count, j = 0; j < size; i++, j++)                   \
        {                                                                           \
            _list_->buffer[i] = elements[j];                                        \
        }                                                                           \
                                                                                    \
        _list_->count += size;                                                      \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_remove(SNAME *_list_, size_t from, size_t to)                   \
    {                                                                               \
        if (from > to || to >= _list_->count)                                       \
            return false;                                                           \
                                                                                    \
        for (size_t i = from, j = to + 1; i < _list_->count; i++, j++)              \
        {                                                                           \
            _list_->buffer[i] = _list_->buffer[j];                                  \
        }                                                                           \
                                                                                    \
        _list_->count -= to - from + 1;                                             \
                                                                                    \
        return true;                                                                \
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
    FMOD bool PFX##_fits(SNAME *_list_, size_t size)                                \
    {                                                                               \
        return _list_->count + size <= _list_->capacity;                            \
    }                                                                               \
                                                                                    \
    FMOD size_t PFX##_capacity(SNAME *_list_)                                       \
    {                                                                               \
        return _list_->capacity;                                                    \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_grow(SNAME *_list_, size_t required)                            \
    {                                                                               \
        size_t new_capacity = _list_->capacity * 2;                                 \
                                                                                    \
        if (new_capacity < required)                                                \
            new_capacity = required;                                                \
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
