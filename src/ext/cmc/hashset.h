/**
 * ext/cmc/hashset.h
 *
 * Creation Date: 29/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_HASHSET_H
#define CMC_EXT_CMC_HASHSET_H

#include "../../cor/core.h"

/**
 * All the EXT parts of CMC HashSet.
 */
#define CMC_EXT_CMC_HASHSET_PARTS ITER, SETF, STR

/**
 * ITER
 */
#define CMC_EXT_CMC_HASHSET_ITER(PARAMS) \
    CMC_EXT_CMC_HASHSET_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHSET_ITER_SOURCE(PARAMS)

#define CMC_EXT_CMC_HASHSET_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHSET_ITER_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHSET_ITER_SOURCE(PARAMS) \
    CMC_EXT_CMC_HASHSET_ITER_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHSET_ITER_HEADER_(PFX, SNAME, V) \
\
    /* HashSet Iterator */ \
    struct CMC_DEF_ITER(SNAME) \
    { \
        /* Target HashSet */ \
        struct SNAME *target; \
\
        /* Cursor's position (index) */ \
        size_t cursor; \
\
        /* Keeps track of relative index to the iteration of elements */ \
        size_t index; \
\
        /* The index of the first element */ \
        size_t first; \
\
        /* The index of the last element */ \
        size_t last; \
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
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#define CMC_EXT_CMC_HASHSET_ITER_SOURCE_(PFX, SNAME, V) \
\
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target) \
    { \
        struct CMC_DEF_ITER(SNAME) iter; \
\
        iter.target = target; \
        iter.cursor = 0; \
        iter.index = 0; \
        iter.first = 0; \
        iter.last = 0; \
        iter.start = true; \
        iter.end = CMC_(PFX, _empty)(target); \
\
        if (!CMC_(PFX, _empty)(target)) \
        { \
            for (size_t i = 0; i < target->capacity; i++) \
            { \
                if (target->buffer[i].state == CMC_ES_FILLED) \
                { \
                    iter.first = i; \
                    break; \
                } \
            } \
\
            iter.cursor = iter.first; \
\
            for (size_t i = target->capacity; i > 0; i--) \
            { \
                if (target->buffer[i - 1].state == CMC_ES_FILLED) \
                { \
                    iter.last = i - 1; \
                    break; \
                } \
            } \
        } \
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
        iter.index = 0; \
        iter.first = 0; \
        iter.last = 0; \
        iter.start = CMC_(PFX, _empty)(target); \
        iter.end = true; \
\
        if (!CMC_(PFX, _empty)(target)) \
        { \
            for (size_t i = 0; i < target->capacity; i++) \
            { \
                if (target->buffer[i].state == CMC_ES_FILLED) \
                { \
                    iter.first = i; \
                    break; \
                } \
            } \
\
            for (size_t i = target->capacity; i > 0; i--) \
            { \
                if (target->buffer[i - 1].state == CMC_ES_FILLED) \
                { \
                    iter.last = i - 1; \
                    break; \
                } \
            } \
\
            iter.cursor = iter.last; \
            iter.index = target->count - 1; \
        } \
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
            iter->cursor = iter->first; \
            iter->index = 0; \
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
            iter->cursor = iter->last; \
            iter->index = iter->target->count - 1; \
            iter->start = CMC_(PFX, _empty)(iter->target); \
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
        if (iter->index + 1 == iter->target->count) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        iter->start = CMC_(PFX, _empty)(iter->target); \
\
        struct CMC_DEF_ENTRY(SNAME) *scan = &(iter->target->buffer[iter->cursor]); \
\
        iter->index++; \
\
        while (1) \
        { \
            iter->cursor++; \
            scan = &(iter->target->buffer[iter->cursor]); \
\
            if (scan->state == CMC_ES_FILLED) \
                break; \
        } \
\
        return true; \
    } \
\
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (iter->start) \
            return false; \
\
        if (iter->index == 0) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        iter->end = CMC_(PFX, _empty)(iter->target); \
\
        struct CMC_DEF_ENTRY(SNAME) *scan = &(iter->target->buffer[iter->cursor]); \
\
        iter->index--; \
\
        while (1) \
        { \
            iter->cursor--; \
            scan = &(iter->target->buffer[iter->cursor]); \
\
            if (scan->state == CMC_ES_FILLED) \
                break; \
        } \
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
        if (iter->index + 1 == iter->target->count) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        if (steps == 0 || iter->index + steps >= iter->target->count) \
            return false; \
\
        for (size_t i = 0; i < steps; i++) \
            CMC_(PFX, _iter_next)(iter); \
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
        if (iter->index == 0) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        if (steps == 0 || iter->index < steps) \
            return false; \
\
        for (size_t i = 0; i < steps; i++) \
            CMC_(PFX, _iter_prev)(iter); \
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
        if (iter->index > index) \
            return CMC_(PFX, _iter_rewind)(iter, iter->index - index); \
        else if (iter->index < index) \
            return CMC_(PFX, _iter_advance)(iter, index - iter->index); \
\
        return true; \
    } \
\
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return (V){ 0 }; \
\
        return iter->target->buffer[iter->cursor].value; \
    } \
\
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        return iter->index; \
    }

/**
 * SETF
 */
#define CMC_EXT_CMC_HASHSET_SETF(PARAMS) \
    CMC_EXT_CMC_HASHSET_SETF_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHSET_SETF_SOURCE(PARAMS)

#define CMC_EXT_CMC_HASHSET_SETF_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHSET_SETF_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHSET_SETF_SOURCE(PARAMS) \
    CMC_EXT_CMC_HASHSET_SETF_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHSET_SETF_HEADER_(PFX, SNAME, V) \
\
    /* Set Operations */ \
    struct SNAME *CMC_(PFX, _union)(struct SNAME * _set1_, struct SNAME * _set2_); \
    struct SNAME *CMC_(PFX, _intersection)(struct SNAME * _set1_, struct SNAME * _set2_); \
    struct SNAME *CMC_(PFX, _difference)(struct SNAME * _set1_, struct SNAME * _set2_); \
    struct SNAME *CMC_(PFX, _symmetric_difference)(struct SNAME * _set1_, struct SNAME * _set2_); \
    bool CMC_(PFX, _is_subset)(struct SNAME * _set1_, struct SNAME * _set2_); \
    bool CMC_(PFX, _is_superset)(struct SNAME * _set1_, struct SNAME * _set2_); \
    bool CMC_(PFX, _is_proper_subset)(struct SNAME * _set1_, struct SNAME * _set2_); \
    bool CMC_(PFX, _is_proper_superset)(struct SNAME * _set1_, struct SNAME * _set2_); \
    bool CMC_(PFX, _is_disjointset)(struct SNAME * _set1_, struct SNAME * _set2_);

#define CMC_EXT_CMC_HASHSET_SETF_SOURCE_(PFX, SNAME, V) \
\
    struct SNAME *CMC_(PFX, _union)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        /* Callbacks are added later */ \
        struct SNAME *_set_r_ = \
            CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, NULL); \
\
        if (!_set_r_) \
        { \
            _set1_->flag = CMC_FLAG_ALLOC; \
            _set2_->flag = CMC_FLAG_ALLOC; \
            return NULL; \
        } \
\
        struct CMC_DEF_ITER(SNAME) iter1 = CMC_(PFX, _iter_start)(_set1_); \
        struct CMC_DEF_ITER(SNAME) iter2 = CMC_(PFX, _iter_start)(_set2_); \
\
        /* TODO turn this into a normal loop and check for errors */ \
        for (; !CMC_(PFX, _iter_at_end)(&iter1); CMC_(PFX, _iter_next)(&iter1)) \
        { \
            CMC_(PFX, _insert)(_set_r_, CMC_(PFX, _iter_value)(&iter1)); \
        } \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter2); CMC_(PFX, _iter_next)(&iter2)) \
        { \
            CMC_(PFX, _insert)(_set_r_, CMC_(PFX, _iter_value)(&iter2)); \
        } \
\
        CMC_CALLBACKS_ASSIGN(_set_r_, _set1_->callbacks); \
\
        return _set_r_; \
    } \
\
    struct SNAME *CMC_(PFX, _intersection)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        /* Callbacks are added later */ \
        struct SNAME *_set_r_ = \
            CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, NULL); \
\
        if (!_set_r_) \
        { \
            _set1_->flag = CMC_FLAG_ALLOC; \
            _set2_->flag = CMC_FLAG_ALLOC; \
            return NULL; \
        } \
\
        struct SNAME *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_; \
        struct SNAME *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_; \
\
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set_A_); \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter); \
\
            if (CMC_(PFX, _impl_get_entry)(_set_B_, value) != NULL) \
                CMC_(PFX, _insert)(_set_r_, value); \
        } \
\
        CMC_CALLBACKS_ASSIGN(_set_r_, _set1_->callbacks); \
\
        return _set_r_; \
    } \
\
    struct SNAME *CMC_(PFX, _difference)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        /* Callbacks are added later */ \
        struct SNAME *_set_r_ = \
            CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, NULL); \
\
        if (!_set_r_) \
        { \
            _set1_->flag = CMC_FLAG_ALLOC; \
            _set2_->flag = CMC_FLAG_ALLOC; \
            return NULL; \
        } \
\
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_); \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter); \
\
            if (CMC_(PFX, _impl_get_entry)(_set2_, value) == NULL) \
                CMC_(PFX, _insert)(_set_r_, value); \
        } \
\
        CMC_CALLBACKS_ASSIGN(_set_r_, _set1_->callbacks); \
\
        return _set_r_; \
    } \
\
    struct SNAME *CMC_(PFX, _symmetric_difference)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        /* Callbacks are added later */ \
        struct SNAME *_set_r_ = \
            CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, NULL); \
\
        if (!_set_r_) \
        { \
            _set1_->flag = CMC_FLAG_ALLOC; \
            _set2_->flag = CMC_FLAG_ALLOC; \
            return NULL; \
        } \
\
        struct CMC_DEF_ITER(SNAME) iter1 = CMC_(PFX, _iter_start)(_set1_); \
        struct CMC_DEF_ITER(SNAME) iter2 = CMC_(PFX, _iter_start)(_set2_); \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter1); CMC_(PFX, _iter_next)(&iter1)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter1); \
\
            if (CMC_(PFX, _impl_get_entry)(_set2_, value) == NULL) \
                CMC_(PFX, _insert)(_set_r_, value); \
        } \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter2); CMC_(PFX, _iter_next)(&iter2)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter2); \
\
            if (CMC_(PFX, _impl_get_entry)(_set1_, value) == NULL) \
                CMC_(PFX, _insert)(_set_r_, value); \
        } \
\
        CMC_CALLBACKS_ASSIGN(_set_r_, _set1_->callbacks); \
\
        return _set_r_; \
    } \
\
    /* Is _set1_ a subset of _set2_ ? */ \
    /* A set X is a subset of a set Y when: X <= Y */ \
    /* If X is a subset of Y, then Y is a superset of X */ \
    bool CMC_(PFX, _is_subset)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        _set1_->flag = CMC_FLAG_OK; \
        _set2_->flag = CMC_FLAG_OK; \
\
        /* If the cardinality of _set1_ is greater than that of _set2_, */ \
        /* then it is safe to say that _set1_ can't be a subset of _set2_ */ \
        if (_set1_->count > _set2_->count) \
            return false; \
\
        /* The empty set is a subset of all sets */ \
        if (CMC_(PFX, _empty)(_set1_)) \
            return true; \
\
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_); \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter); \
\
            if (CMC_(PFX, _impl_get_entry)(_set2_, value) == NULL) \
                return false; \
        } \
\
        return true; \
    } \
\
    /* Is _set1_ a superset of _set2_ ? */ \
    /* A set X is a superset of a set Y when: X >= Y */ \
    /* If X is a superset of Y, then Y is a subset of X */ \
    bool CMC_(PFX, _is_superset)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        return CMC_(PFX, _is_subset)(_set2_, _set1_); \
    } \
\
    /* Is _set1_ a proper subset of _set2_ ? */ \
    /* A set X is a proper subset of a set Y when: X < Y */ \
    /* If X is a proper subset of Y, then Y is a proper superset of X */ \
    bool CMC_(PFX, _is_proper_subset)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        _set1_->flag = CMC_FLAG_OK; \
        _set2_->flag = CMC_FLAG_OK; \
\
        /* If the cardinality of _set1_ is greater than or equal to that of */ \
        /* _set2_, then it is safe to say that _set1_ can't be a proper */ \
        /* subset of _set2_ */ \
        if (_set1_->count >= _set2_->count) \
            return false; \
\
        if (CMC_(PFX, _empty)(_set1_)) \
        { \
            /* The empty set is a proper subset of all non-empty sets */ \
            if (!CMC_(PFX, _empty)(_set2_)) \
                return true; \
            /* The empty set is not a proper subset of itself (this is true */ \
            /* for any set) */ \
            else \
                return false; \
        } \
\
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_); \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter); \
\
            if (CMC_(PFX, _impl_get_entry)(_set2_, value) == NULL) \
                return false; \
        } \
\
        return true; \
    } \
\
    /* Is _set1_ a proper superset of _set2_ ? */ \
    /* A set X is a proper superset of a set Y when: X > Y */ \
    /* If X is a proper superset of Y, then Y is a proper subset of X */ \
    bool CMC_(PFX, _is_proper_superset)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        return CMC_(PFX, _is_proper_subset)(_set2_, _set1_); \
    } \
\
    /* Is _set1_ a disjointset of _set2_ ? */ \
    /* A set X is a disjointset of a set Y if their intersection is empty, */ \
    /* that is, if there are no elements in common between the two */ \
    bool CMC_(PFX, _is_disjointset)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        _set1_->flag = CMC_FLAG_OK; \
        _set2_->flag = CMC_FLAG_OK; \
\
        /* The intersection of an empty set with any other set will result */ \
        /* in an empty set */ \
        if (CMC_(PFX, _empty)(_set1_)) \
            return true; \
\
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_); \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter); \
\
            if (CMC_(PFX, _impl_get_entry)(_set2_, value) != NULL) \
                return false; \
        } \
\
        return true; \
    }

/**
 * STR
 */
#define CMC_EXT_CMC_HASHSET_STR(PARAMS) \
    CMC_EXT_CMC_HASHSET_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHSET_STR_SOURCE(PARAMS)

#define CMC_EXT_CMC_HASHSET_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHSET_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHSET_STR_SOURCE(PARAMS) \
    CMC_EXT_CMC_HASHSET_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHSET_STR_HEADER_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _set_, FILE * fptr); \
    bool CMC_(PFX, _print)(struct SNAME * _set_, FILE * fptr, const char *start, const char *separator, \
                           const char *end);

#define CMC_EXT_CMC_HASHSET_STR_SOURCE_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _set_, FILE * fptr) \
    { \
        struct SNAME *s_ = _set_; \
\
        return 0 <= fprintf(fptr, \
                            "struct %s<%s> " \
                            "at %p { " \
                            "buffer:%p, " \
                            "capacity:%" PRIuMAX ", " \
                            "count:%" PRIuMAX ", " \
                            "load:%lf, " \
                            "flag:%d, " \
                            "f_val:%p, " \
                            "alloc:%p, " \
                            "callbacks: %p }", \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(V), s_, s_->buffer, s_->capacity, s_->count, s_->load, \
                            s_->flag, s_->f_val, s_->alloc, CMC_CALLBACKS_GET(s_)); \
    } \
\
    bool CMC_(PFX, _print)(struct SNAME * _set_, FILE * fptr, const char *start, const char *separator, \
                           const char *end) \
    { \
        fprintf(fptr, "%s", start); \
\
        size_t last = 0; \
        for (size_t i = _set_->capacity; i > 0; i--) \
        { \
            if ((_set_->buffer[i - 1]).state == CMC_ES_FILLED) \
            { \
                last = i - 1; \
                break; \
            } \
        } \
\
        for (size_t i = 0; i < _set_->capacity; i++) \
        { \
            struct CMC_DEF_ENTRY(SNAME) *entry = &(_set_->buffer[i]); \
\
            if (entry->state == CMC_ES_FILLED) \
            { \
                if (!_set_->f_val->str(fptr, entry->value)) \
                    return false; \
\
                if (i + 1 < last) \
                    fprintf(fptr, "%s", separator); \
            } \
        } \
\
        fprintf(fptr, "%s", end); \
\
        return true; \
    }

#endif /* CMC_EXT_CMC_HASHSET_H */
