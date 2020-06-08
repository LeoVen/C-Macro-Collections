/**
 * ext/cmc/treemap.h
 *
 * Creation Date: 08/06/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_TREEMAP_H
#define CMC_EXT_CMC_TREEMAP_H

#include "../../cor/core.h"

/**
 * All the EXT parts of CMC TreeMap.
 */
#define CMC_EXT_CMC_TREEMAP_PARTS ITER, STR

/**
 * ITER
 */
#define CMC_EXT_CMC_TREEMAP_ITER(PARAMS) \
    CMC_EXT_CMC_TREEMAP_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_TREEMAP_ITER_SOURCE(PARAMS)

#define CMC_EXT_CMC_TREEMAP_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_TREEMAP_ITER_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                     CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREEMAP_ITER_SOURCE(PARAMS) \
    CMC_EXT_CMC_TREEMAP_ITER_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                     CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREEMAP_ITER_HEADER_(PFX, SNAME, K, V) \
\
    /* Treemap Iterator */ \
    struct CMC_DEF_ITER(SNAME) \
    { \
        /* Target treemap */ \
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
    K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter); \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter); \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter); \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#define CMC_EXT_CMC_TREEMAP_ITER_SOURCE_(PFX, SNAME, K, V) \
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
    K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return (K){ 0 }; \
\
        return iter->cursor->key; \
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
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        if (CMC_(PFX, _empty)(iter->target)) \
            return NULL; \
\
        return &(iter->cursor->value); \
    } \
\
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter) \
    { \
        return iter->index; \
    }

/**
 * STR
 */
#define CMC_EXT_CMC_TREEMAP_STR(PARAMS) \
    CMC_EXT_CMC_TREEMAP_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_TREEMAP_STR_SOURCE(PARAMS)

#define CMC_EXT_CMC_TREEMAP_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_TREEMAP_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                    CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREEMAP_STR_SOURCE(PARAMS) \
    CMC_EXT_CMC_TREEMAP_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                    CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREEMAP_STR_HEADER_(PFX, SNAME, K, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _map_, FILE * fptr); \
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr, const char *start, const char *separator, \
                           const char *end, const char *key_val_sep);

#define CMC_EXT_CMC_TREEMAP_STR_SOURCE_(PFX, SNAME, K, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _map_, FILE * fptr) \
    { \
        struct SNAME *m_ = _map_; \
\
        return 0 <= fprintf(fptr, \
                            "struct %s<%s, %s> " \
                            "at %p { " \
                            "root:%p, " \
                            "count:%" PRIuMAX ", " \
                            "flag:%d, " \
                            "f_val:%p, " \
                            "f_key:%p, " \
                            "alloc:%p, " \
                            "callbacks:%p }", \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(K), CMC_TO_STRING(V), m_, m_->root, m_->count, \
                            m_->flag, m_->f_key, m_->f_val, m_->alloc, CMC_CALLBACKS_GET(m_)); \
    } \
\
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr, const char *start, const char *separator, \
                           const char *end, const char *key_val_sep) \
    { \
        fprintf(fptr, "%s", start); \
\
        struct CMC_DEF_NODE(SNAME) *root = _map_->root; \
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
            if (!_map_->f_key->str(fptr, root->key)) \
                return false; \
\
            fprintf(fptr, "%s", key_val_sep); \
\
            if (!_map_->f_val->str(fptr, root->value)) \
                return false; \
\
            if (++i < _map_->count) \
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

#endif /* CMC_EXT_CMC_TREEMAP_H */
