/**
 * ext/cmc/treeset.h
 *
 * Creation Date: 08/06/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_TREESET_H
#define CMC_EXT_CMC_TREESET_H

#include "../../cor/core.h"

/**
 * All the EXT parts of CMC TreeSet.
 */
#define CMC_EXT_CMC_TREESET_PARTS ITER, STR

/**
 * ITER
 */
#define CMC_EXT_CMC_TREESET_ITER(PARAMS) \
    CMC_EXT_CMC_TREESET_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_TREESET_ITER_SOURCE(PARAMS)

#define CMC_EXT_CMC_TREESET_ITER_HEADER(PARAMS) \
    CMC_EXT_CMC_TREESET_ITER_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_ITER_SOURCE(PARAMS) \
    CMC_EXT_CMC_TREESET_ITER_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_ITER_HEADER_(PFX, SNAME, V)

#define CMC_EXT_CMC_TREESET_ITER_SOURCE_(PFX, SNAME, V)

/**
 * STR
 */
#define CMC_EXT_CMC_TREESET_STR(PARAMS) \
    CMC_EXT_CMC_TREESET_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_TREESET_STR_SOURCE(PARAMS)

#define CMC_EXT_CMC_TREESET_STR_HEADER(PARAMS) \
    CMC_EXT_CMC_TREESET_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_TREESET_STR_SOURCE(PARAMS) \
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
