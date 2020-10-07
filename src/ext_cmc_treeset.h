/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Libray.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * ext_cmc_treeset.h
 *
 * Creation Date: 08/06/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_TREESET_H
#define CMC_EXT_CMC_TREESET_H

#include "cor_core.h"

/**
 * All the EXT parts of CMC TreeSet.
 */
#define CMC_EXT_CMC_TREESET_PARTS ITER, SETF, STR

/**
 * ITER
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_EXT_CMC_TREESET_ITER(ACCESS, FILE, PARAMS) \
    CMC_(CMC_(CMC_EXT_CMC_TREESET_ITER_, ACCESS), CMC_(_, FILE))(PARAMS)

#define CMC_EXT_CMC_TREESET_ITER_PUBLIC_HEADER(PARAMS) \
    CMC_EXT_CMC_TREESET_ITER_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_ITER_PUBLIC_SOURCE(PARAMS) \
    CMC_EXT_CMC_TREESET_ITER_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_ITER_PRIVATE_HEADER(PARAMS) \
    CMC_EXT_CMC_TREESET_ITER_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_ITER_PRIVATE_SOURCE(PARAMS) \
    CMC_EXT_CMC_TREESET_ITER_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_ITER_HEADER_(PFX, SNAME, V) \
\
    /* Treeset Iterator */ \
    struct CMC_DEF_ITER(SNAME) \
    { \
        /* Target treeset */ \
        struct SNAME *target; \
\
        /* Cursor's current node */ \
        struct CMC_DEF_NODE(SNAME) * cursor; \
\
        /* The first node in the iteration */ \
        struct CMC_DEF_NODE(SNAME) * first; \
\
        /* The last node in the iteration */ \
        struct CMC_DEF_NODE(SNAME) * last; \
\
        /* Keeps track of relative index to the iteration of elements */ \
        size_t index; \
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

#define CMC_EXT_CMC_TREESET_ITER_SOURCE_(PFX, SNAME, V) \
\
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target) \
    { \
        struct CMC_DEF_ITER(SNAME) iter; \
\
        iter.target = target; \
        iter.cursor = target->root; \
        iter.first = NULL; \
        iter.last = NULL; \
        iter.index = 0; \
        iter.start = true; \
        iter.end = CMC_(PFX, _empty)(target); \
\
        if (!CMC_(PFX, _empty)(target)) \
        { \
            while (iter.cursor->left != NULL) \
                iter.cursor = iter.cursor->left; \
\
            iter.first = iter.cursor; \
\
            iter.last = target->root; \
            while (iter.last->right != NULL) \
                iter.last = iter.last->right; \
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
        iter.cursor = target->root; \
        iter.first = NULL; \
        iter.last = NULL; \
        iter.index = 0; \
        iter.start = CMC_(PFX, _empty)(target); \
        iter.end = true; \
\
        if (!CMC_(PFX, _empty)(target)) \
        { \
            while (iter.cursor->right != NULL) \
                iter.cursor = iter.cursor->right; \
\
            iter.last = iter.cursor; \
\
            iter.first = target->root; \
            while (iter.first->left != NULL) \
                iter.first = iter.first->left; \
\
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
            iter->index = 0; \
            iter->start = true; \
            iter->end = CMC_(PFX, _empty)(iter->target); \
            iter->cursor = iter->first; \
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
            iter->index = iter->target->count - 1; \
            iter->start = CMC_(PFX, _empty)(iter->target); \
            iter->end = true; \
            iter->cursor = iter->last; \
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
        if (iter->cursor == iter->last) \
        { \
            iter->end = true; \
            return false; \
        } \
\
        iter->start = CMC_(PFX, _empty)(iter->target); \
\
        if (iter->cursor->right != NULL) \
        { \
            iter->cursor = iter->cursor->right; \
\
            while (iter->cursor->left != NULL) \
                iter->cursor = iter->cursor->left; \
\
            iter->index++; \
\
            return true; \
        } \
\
        while (true) \
        { \
            if (iter->cursor->parent->left == iter->cursor) \
            { \
                iter->cursor = iter->cursor->parent; \
\
                iter->index++; \
\
                return true; \
            } \
\
            iter->cursor = iter->cursor->parent; \
        } \
    } \
\
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (iter->start) \
            return false; \
\
        if (iter->cursor == iter->first) \
        { \
            iter->start = true; \
            return false; \
        } \
\
        iter->end = CMC_(PFX, _empty)(iter->target); \
\
        if (iter->cursor->left != NULL) \
        { \
            iter->cursor = iter->cursor->left; \
\
            while (iter->cursor->right != NULL) \
                iter->cursor = iter->cursor->right; \
\
            iter->index--; \
\
            return true; \
        } \
\
        while (true) \
        { \
            if (iter->cursor->parent->right == iter->cursor) \
            { \
                iter->cursor = iter->cursor->parent; \
\
                iter->index--; \
\
                return true; \
            } \
\
            iter->cursor = iter->cursor->parent; \
        } \
    } \
\
    /* Returns true only if the iterator moved */ \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps) \
    { \
        if (iter->end) \
            return false; \
\
        if (iter->cursor == iter->last) \
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
        if (iter->cursor == iter->first) \
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
        return iter->cursor->value; \
    } \
\
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        return iter->index; \
    }

/**
 * SETF
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_EXT_CMC_TREESET_SETF(ACCESS, FILE, PARAMS) \
    CMC_(CMC_(CMC_EXT_CMC_TREESET_SETF_, ACCESS), CMC_(_, FILE))(PARAMS)

#define CMC_EXT_CMC_TREESET_SETF_PUBLIC_HEADER(PARAMS) \
    CMC_EXT_CMC_TREESET_SETF_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_SETF_PUBLIC_SOURCE(PARAMS) \
    CMC_EXT_CMC_TREESET_SETF_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_SETF_PRIVATE_HEADER(PARAMS) \
    CMC_EXT_CMC_TREESET_SETF_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_SETF_PRIVATE_SOURCE(PARAMS) \
    CMC_EXT_CMC_TREESET_SETF_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_SETF_HEADER_(PFX, SNAME, V) \
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

#define CMC_EXT_CMC_TREESET_SETF_SOURCE_(PFX, SNAME, V) \
\
    struct SNAME *CMC_(PFX, _union)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        struct SNAME *_set_r_ = CMC_(PFX, _new_custom)(_set1_->f_val, _set1_->alloc, NULL); \
\
        if (!_set_r_) \
            return NULL; \
\
        struct CMC_DEF_ITER(SNAME) iter1 = CMC_(PFX, _iter_start)(_set1_); \
        struct CMC_DEF_ITER(SNAME) iter2 = CMC_(PFX, _iter_start)(_set2_); \
\
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
        struct SNAME *_set_r_ = CMC_(PFX, _new_custom)(_set1_->f_val, _set1_->alloc, NULL); \
\
        if (!_set_r_) \
            return NULL; \
\
        /** TODO Should this compare count or capacity? */ \
        struct SNAME *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_; \
        struct SNAME *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_; \
\
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set_A_); \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter); \
\
            if (CMC_(PFX, _impl_get_node)(_set_B_, value) != NULL) \
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
        struct SNAME *_set_r_ = CMC_(PFX, _new_custom)(_set1_->f_val, _set1_->alloc, NULL); \
\
        if (!_set_r_) \
            return NULL; \
\
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_); \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter); \
\
            if (CMC_(PFX, _impl_get_node)(_set2_, value) == NULL) \
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
        struct SNAME *_set_r_ = CMC_(PFX, _new_custom)(_set1_->f_val, _set1_->alloc, NULL); \
\
        if (!_set_r_) \
            return NULL; \
\
        struct CMC_DEF_ITER(SNAME) iter1 = CMC_(PFX, _iter_start)(_set1_); \
        struct CMC_DEF_ITER(SNAME) iter2 = CMC_(PFX, _iter_start)(_set2_); \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter1); CMC_(PFX, _iter_next)(&iter1)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter1); \
\
            if (CMC_(PFX, _impl_get_node)(_set2_, value) == NULL) \
                CMC_(PFX, _insert)(_set_r_, value); \
        } \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter2); CMC_(PFX, _iter_next)(&iter2)) \
        { \
            V value = CMC_(PFX, _iter_value)(&iter2); \
\
            if (CMC_(PFX, _impl_get_node)(_set1_, value) == NULL) \
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
        /* If the cardinality of _set1_ is greater than that of _set2_ */ \
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
            if (CMC_(PFX, _impl_get_node)(_set2_, value) == NULL) \
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
        /* If the cardinality of _set1_ is greater than or equal to that */ \
        /* of _set2_, then it is safe to say that _set1_ can't be a proper */ \
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
            if (CMC_(PFX, _impl_get_node)(_set2_, value) == NULL) \
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
            if (CMC_(PFX, _impl_get_node)(_set2_, value) != NULL) \
                return false; \
        } \
\
        return true; \
    }

/**
 * STR
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_EXT_CMC_TREESET_STR(ACCESS, FILE, PARAMS) \
    CMC_(CMC_(CMC_EXT_CMC_TREESET_STR_, ACCESS), CMC_(_, FILE))(PARAMS)

#define CMC_EXT_CMC_TREESET_STR_PUBLIC_HEADER(PARAMS) \
    CMC_EXT_CMC_TREESET_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_STR_PUBLIC_SOURCE(PARAMS) \
    CMC_EXT_CMC_TREESET_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_STR_PRIVATE_HEADER(PARAMS) \
    CMC_EXT_CMC_TREESET_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_STR_PRIVATE_SOURCE(PARAMS) \
    CMC_EXT_CMC_TREESET_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_STR_HEADER_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _set_, FILE * fptr); \
    bool CMC_(PFX, _print)(struct SNAME * _set_, FILE * fptr, const char *start, const char *separator, \
                           const char *end);

#define CMC_EXT_CMC_TREESET_STR_SOURCE_(PFX, SNAME, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _set_, FILE * fptr) \
    { \
        struct SNAME *s_ = _set_; \
\
        return 0 <= fprintf(fptr, \
                            "struct %s<%s> " \
                            "at %p { " \
                            "root:%p, " \
                            "count:%" PRIuMAX ", " \
                            "flag:%d, " \
                            "f_val:%p, " \
                            "alloc:%p, " \
                            "callbacks:%p }", \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(V), s_, s_->root, s_->count, s_->flag, s_->f_val, \
                            s_->alloc, CMC_CALLBACKS_GET(s_)); \
    } \
\
    bool CMC_(PFX, _print)(struct SNAME * _set_, FILE * fptr, const char *start, const char *separator, \
                           const char *end) \
    { \
        fprintf(fptr, "%s", start); \
\
        struct CMC_DEF_NODE(SNAME) *root = _set_->root; \
\
        bool left_done = false; \
\
        size_t i = 0; \
        while (root) \
        { \
            if (!left_done) \
            { \
                while (root->left) \
                    root = root->left; \
            } \
\
            if (!_set_->f_val->str(fptr, root->value)) \
                return false; \
\
            if (++i < _set_->count) \
                fprintf(fptr, "%s", separator); \
\
            left_done = true; \
\
            if (root->right) \
            { \
                left_done = false; \
                root = root->right; \
            } \
            else if (root->parent) \
            { \
                while (root->parent && root == root->parent->right) \
                    root = root->parent; \
\
                if (!root->parent) \
                    break; \
\
                root = root->parent; \
            } \
            else \
                break; \
        } \
\
        fprintf(fptr, "%s", end); \
\
        return true; \
    }

#endif /* CMC_EXT_CMC_TREESET_H */
