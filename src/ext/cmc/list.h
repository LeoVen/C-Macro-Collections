/**
 * ext/cmc/list.h
 *
 * Creation Date: 04/06/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_LIST_H
#define CMC_EXT_CMC_LIST_H

#include "../../cor/core.h"

/**
 * All the EXT parts of CMC List.
 */
#define CMC_EXT_CMC_LIST_PARTS ITER, SEQ, STR

/**
 * ITER
 */
#define CMC_EXT_CMC_LIST_ITER(PARAMS) \
    CMC_EXT_CMC_LIST_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_LIST_ITER_SOURCE(PARAMS)

#define CMC_EXT_CMC_LIST_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_LIST_ITER_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_LIST_ITER_SOURCE(PARAMS) \
    CMC_EXT_CMC_LIST_ITER_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_LIST_ITER_HEADER_(PFX, SNAME, V) \
\
    /* List Iterator */ \
    struct CMC_DEF_ITER(SNAME) \
    { \
        /* Target List */ \
        struct SNAME *target; \
\
        /* Cursor's position (index) */ \
        size_t cursor; \
\
        /* If the iterator has reached the start of the iteration */ \
        bool start; \
\
        /* If the iterator has reached the end of the iteration */ \
        bool end; \
    }; \
\
    /* Iterator Initialization */ \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target); \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target); \
    /* Iterator State */ \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter); \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter); \
    /* Iterator Movement */ \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter); \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter); \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter); \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter); \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps); \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps); \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter, size_t index); \
    /* Iterator Access */ \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter); \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter); \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#define CMC_EXT_CMC_LIST_ITER_SOURCE_(PFX, SNAME, V) \
\
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target) \
    { \
        struct CMC_DEF_ITER(SNAME) iter; \
\
        iter.target = target; \
        iter.cursor = 0; \
        iter.start = true; \
        iter.end = CMC_(PFX, _empty)(target); \
\
        return iter; \
    } \
\
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target) \
    { \
        struct CMC_DEF_ITER(SNAME) iter; \
\
        iter.target = target; \
        iter.cursor = 0; \
        iter.start = CMC_(PFX, _empty)(target); \
        iter.end = true; \
\
        if (!CMC_(PFX, _empty)(target)) \
            iter.cursor = target->count - 1; \
\
        return iter; \
    } \
\
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        return CMC_(PFX, _empty)(iter->target) || iter->start; \
    } \
\
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        return CMC_(PFX, _empty)(iter->target) || iter->end; \
    } \
\
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (!CMC_(PFX, _empty)(iter->target)) \
        { \
            iter->cursor = 0; \
            iter->start = true; \
            iter->end = CMC_(PFX, _empty)(iter->target); \
\
            return true; \
        } \
\
        return false; \
    } \
\
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (!CMC_(PFX, _empty)(iter->target)) \
        { \
            iter->start = CMC_(PFX, _empty)(iter->target); \
            iter->cursor = iter->target->count - 1; \
            iter->end = true; \
\
            return true; \
        } \
\
        return false; \
    } \
\
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (iter->end) \
            return false; \
\
        if (iter->cursor + 1 == iter->target->count) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        iter->start = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor++; \
\
        return true; \
    } \
\
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (iter->start) \
            return false; \
\
        if (iter->cursor == 0) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        iter->end = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor--; \
\
        return true; \
    } \
\
    /* Returns true only if the iterator moved */ \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps) \
    { \
        if (iter->end) \
            return false; \
\
        if (iter->cursor + 1 == iter->target->count) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        if (steps == 0 || iter->cursor + steps >= iter->target->count) \
            return false; \
\
        iter->start = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor += steps; \
\
        return true; \
    } \
\
    /* Returns true only if the iterator moved */ \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps) \
    { \
        if (iter->start) \
            return false; \
\
        if (iter->cursor == 0) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        if (steps == 0 || iter->cursor < steps) \
            return false; \
\
        iter->end = CMC_(PFX, _empty)(iter->target); \
\
        iter->cursor -= steps; \
\
        return true; \
    } \
\
    /* Returns true only if the iterator was able to be positioned at the */ \
    /* given index */ \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter, size_t index) \
    { \
        if (index >= iter->target->count) \
            return false; \
\
        if (iter->cursor > index) \
            return CMC_(PFX, _iter_rewind)(iter, iter->cursor - index); \
        else if (iter->cursor < index) \
            return CMC_(PFX, _iter_advance)(iter, index - iter->cursor); \
\
        return true; \
    } \
\
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return (V){ 0 }; \
\
        return iter->target->buffer[iter->cursor]; \
    } \
\
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return NULL; \
\
        return &(iter->target->buffer[iter->cursor]); \
    } \
\
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        return iter->cursor; \
    }

/**
 * SEQ
 */
#define CMC_EXT_CMC_LIST_SEQ(PARAMS) \
    CMC_EXT_CMC_LIST_SEQ_HEADER(PARAMS) \
    CMC_EXT_CMC_LIST_SEQ_SOURCE(PARAMS)

#define CMC_EXT_CMC_LIST_SEQ_HEADER(PARAMS) \
    CMC_EXT_CMC_LIST_SEQ_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_LIST_SEQ_SOURCE(PARAMS) \
    CMC_EXT_CMC_LIST_SEQ_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_LIST_SEQ_HEADER_(PFX, SNAME, V) \
\
    /* List Sequence Input and Output */ \
    bool CMC_(PFX, _seq_push_front)(struct SNAME * _list_, V * values, size_t size); \
    bool CMC_(PFX, _seq_push_at)(struct SNAME * _list_, V * values, size_t size, size_t index); \
    bool CMC_(PFX, _seq_push_back)(struct SNAME * _list_, V * values, size_t size); \
    bool CMC_(PFX, _seq_pop_at)(struct SNAME * _list_, size_t from, size_t to); \
    struct SNAME *CMC_(PFX, _seq_sublist)(struct SNAME * _list_, size_t from, size_t to);

#define CMC_EXT_CMC_LIST_SEQ_SOURCE_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _seq_push_front)(struct SNAME * _list_, V * values, size_t size) \
    { \
        if (size == 0) \
        { \
            _list_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        if (!CMC_(PFX, _fits)(_list_, size)) \
        { \
            if (!CMC_(PFX, _resize)(_list_, _list_->count + size)) \
                return false; \
        } \
\
        memmove(_list_->buffer + size, _list_->buffer, _list_->count * sizeof(V)); \
\
        memcpy(_list_->buffer, values, size * sizeof(V)); \
\
        _list_->count += size; \
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _seq_push_at)(struct SNAME * _list_, V * values, size_t size, size_t index) \
    { \
        if (size == 0) \
        { \
            _list_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        if (index > _list_->count) \
        { \
            _list_->flag = CMC_FLAG_RANGE; \
            return false; \
        } \
\
        if (index == 0) \
            return CMC_(PFX, _seq_push_front)(_list_, values, size); \
        else if (index == _list_->count) \
            return CMC_(PFX, _seq_push_back)(_list_, values, size); \
\
        if (!CMC_(PFX, _fits)(_list_, size)) \
        { \
            if (!CMC_(PFX, _resize)(_list_, _list_->count + size)) \
                return false; \
        } \
\
        memmove(_list_->buffer + index + size, _list_->buffer + index, (_list_->count - index) * sizeof(V)); \
\
        memcpy(_list_->buffer + index, values, size * sizeof(V)); \
\
        _list_->count += size; \
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _seq_push_back)(struct SNAME * _list_, V * values, size_t size) \
    { \
        if (size == 0) \
        { \
            _list_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        if (!CMC_(PFX, _fits)(_list_, size)) \
        { \
            if (!CMC_(PFX, _resize)(_list_, _list_->count + size)) \
                return false; \
        } \
\
        memcpy(_list_->buffer + _list_->count, values, size * sizeof(V)); \
\
        _list_->count += size; \
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _seq_pop_at)(struct SNAME * _list_, size_t from, size_t to) \
    { \
        if (from > to) \
        { \
            _list_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        if (to >= _list_->count) \
        { \
            _list_->flag = CMC_FLAG_RANGE; \
            return false; \
        } \
\
        size_t length = (to - from + 1); \
\
        memmove(_list_->buffer + from, _list_->buffer + to + 1, (_list_->count - to - 1) * sizeof(V)); \
\
        memset(_list_->buffer + _list_->count - length, 0, length * sizeof(V)); \
\
        _list_->count -= to - from + 1; \
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, delete); \
\
        return true; \
    } \
\
    struct SNAME *CMC_(PFX, _seq_sublist)(struct SNAME * _list_, size_t from, size_t to) \
    { \
        if (from > to) \
        { \
            _list_->flag = CMC_FLAG_INVALID; \
            return NULL; \
        } \
\
        if (to >= _list_->count) \
        { \
            _list_->flag = CMC_FLAG_RANGE; \
            return NULL; \
        } \
\
        size_t length = to - from + 1; \
\
        struct SNAME *result = CMC_(PFX, _new_custom)(length, _list_->f_val, _list_->alloc, CMC_CALLBACKS_GET(_list_)); \
\
        if (!result) \
        { \
            _list_->flag = CMC_FLAG_ALLOC; \
            return NULL; \
        } \
\
        memcpy(result->buffer, _list_->buffer, length * sizeof(V)); \
\
        memmove(_list_->buffer + from, _list_->buffer + to + 1, (_list_->count - to - 1) * sizeof(V)); \
\
        memset(_list_->buffer + _list_->count - length, 0, length * sizeof(V)); \
\
        _list_->count -= length; \
        result->count = length; \
\
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, delete); \
\
        return result; \
    }

/**
 * STR
 */
#define CMC_EXT_CMC_LIST_STR(PARAMS) \
    CMC_EXT_CMC_LIST_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_LIST_STR_SOURCE(PARAMS)

#define CMC_EXT_CMC_LIST_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_LIST_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_LIST_STR_SOURCE(PARAMS) \
    CMC_EXT_CMC_LIST_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_LIST_STR_HEADER_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _list_, FILE * fptr); \
    bool CMC_(PFX, _print)(struct SNAME * _list_, FILE * fptr, const char *start, const char *separator, \
                           const char *end);

#define CMC_EXT_CMC_LIST_STR_SOURCE_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _list_, FILE * fptr) \
    { \
        struct SNAME *l_ = _list_; \
\
        return 0 <= fprintf(fptr, \
                            "struct %s<%s> " \
                            "at %p { " \
                            "buffer:%p, " \
                            "capacity:%" PRIuMAX ", " \
                            "count:%" PRIuMAX ", " \
                            "flag:%d, " \
                            "f_val:%p, " \
                            "alloc:%p, " \
                            "callbacks:%p }", \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(V), l_, l_->buffer, l_->capacity, l_->count, l_->flag, \
                            l_->f_val, l_->alloc, CMC_CALLBACKS_GET(l_)); \
    } \
\
    bool CMC_(PFX, _print)(struct SNAME * _list_, FILE * fptr, const char *start, const char *separator, \
                           const char *end) \
    { \
        fprintf(fptr, "%s", start); \
\
        for (size_t i = 0; i < _list_->count; i++) \
        { \
            if (!_list_->f_val->str(fptr, _list_->buffer[i])) \
                return false; \
\
            if (i + 1 < _list_->count) \
                fprintf(fptr, "%s", separator); \
        } \
\
        fprintf(fptr, "%s", end); \
\
        return true; \
    }

#endif /* CMC_EXT_CMC_LIST_H */
