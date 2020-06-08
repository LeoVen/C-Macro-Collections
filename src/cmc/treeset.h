/**
 * cmc/treeset.h
 *
 * Creation Date: 27/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * TreeSet
 *
 * A TreeSet is an implementation of a Set that keeps its elements sorted. Like
 * a Set it has only unique keys. This implementation uses a balanced binary
 * tree called AVL Tree that uses the height of nodes to keep its keys balanced.
 */

#ifndef CMC_CMC_TREESET_H
#define CMC_CMC_TREESET_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/**
 * Core TreeSet implementation
 */
#define CMC_CMC_TREESET_CORE(PARAMS) \
    CMC_CMC_TREESET_CORE_HEADER(PARAMS) \
    CMC_CMC_TREESET_CORE_SOURCE(PARAMS)

#define CMC_CMC_TREESET_CORE_HEADER(PARAMS) \
    CMC_CMC_TREESET_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_TREESET_CORE_SOURCE(PARAMS) \
    CMC_CMC_TREESET_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_TREESET_CORE_HEADER_(PFX, SNAME, V) \
\
    /* Treeset Structure */ \
    struct SNAME \
    { \
        /* Root node */ \
        struct CMC_DEF_NODE(SNAME) * root; \
\
        /* Current amount of elements */ \
        size_t count; \
\
        /* Flags indicating errors or success */ \
        int flag; \
\
        /* Value function table */ \
        struct CMC_DEF_FVAL(SNAME) * f_val; \
\
        /* Custom allocation functions */ \
        struct CMC_ALLOC_NODE_NAME *alloc; \
\
        /* Custom callback functions */ \
        CMC_CALLBACKS_DECL; \
    }; \
\
    /* Treeset Node */ \
    struct CMC_DEF_NODE(SNAME) \
    { \
        /* Node element */ \
        V value; \
\
        /* Node height used by the AVL tree to keep it strictly balanced */ \
        unsigned char height; \
\
        /* Right child node or subtree */ \
        struct CMC_DEF_NODE(SNAME) * right; \
\
        /* Left child node or subtree */ \
        struct CMC_DEF_NODE(SNAME) * left; \
\
        /* Parent node */ \
        struct CMC_DEF_NODE(SNAME) * parent; \
    }; \
\
    /* Value struct function table */ \
    struct CMC_DEF_FVAL(SNAME) \
    { \
        /* Comparator function */ \
        CMC_DEF_FTAB_CMP(V); \
\
        /* Copy function */ \
        CMC_DEF_FTAB_CPY(V); \
\
        /* To string function */ \
        CMC_DEF_FTAB_STR(V); \
\
        /* Free from memory function */ \
        CMC_DEF_FTAB_FREE(V); \
\
        /* Hash function */ \
        CMC_DEF_FTAB_HASH(V); \
\
        /* Priority function */ \
        CMC_DEF_FTAB_PRI(V); \
    }; \
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
    /* Collection Functions */ \
    /* Collection Allocation and Deallocation */ \
    struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FVAL(SNAME) * f_val); \
    struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FVAL(SNAME) * f_val, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks); \
    void CMC_(PFX, _clear)(struct SNAME * _set_); \
    void CMC_(PFX, _free)(struct SNAME * _set_); \
    /* Customization of Allocation and Callbacks */ \
    void CMC_(PFX, _customize)(struct SNAME * _set_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks); \
    /* Collection Input and Output */ \
    bool CMC_(PFX, _insert)(struct SNAME * _set_, V value); \
    bool CMC_(PFX, _remove)(struct SNAME * _set_, V value); \
    /* Element Access */ \
    bool CMC_(PFX, _max)(struct SNAME * _set_, V * value); \
    bool CMC_(PFX, _min)(struct SNAME * _set_, V * value); \
    /* Collection State */ \
    bool CMC_(PFX, _contains)(struct SNAME * _set_, V value); \
    bool CMC_(PFX, _empty)(struct SNAME * _set_); \
    size_t CMC_(PFX, _count)(struct SNAME * _set_); \
    int CMC_(PFX, _flag)(struct SNAME * _set_); \
    /* Collection Utility */ \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _set_); \
    bool CMC_(PFX, _equals)(struct SNAME * _set1_, struct SNAME * _set2_); \
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
    bool CMC_(PFX, _is_disjointset)(struct SNAME * _set1_, struct SNAME * _set2_); \
\
    /* Iterator Functions */ \
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

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_TREESET_CORE_SOURCE_(PFX, SNAME, V) \
\
    /* Implementation Detail Functions */ \
    static struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_new_node)(struct SNAME * _set_, V value); \
    static struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_get_node)(struct SNAME * _set_, V value); \
    static unsigned char CMC_(PFX, _impl_h)(struct CMC_DEF_NODE(SNAME) * node); \
    static unsigned char CMC_(PFX, _impl_hupdate)(struct CMC_DEF_NODE(SNAME) * node); \
    static void CMC_(PFX, _impl_rotate_right)(struct CMC_DEF_NODE(SNAME) * *Z); \
    static void CMC_(PFX, _impl_rotate_left)(struct CMC_DEF_NODE(SNAME) * *Z); \
    static void CMC_(PFX, _impl_rebalance)(struct SNAME * _set_, struct CMC_DEF_NODE(SNAME) * node); \
\
    struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FVAL(SNAME) * f_val) \
    { \
        return CMC_(PFX, _new_custom)(f_val, NULL, NULL); \
    } \
\
    struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FVAL(SNAME) * f_val, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!f_val) \
            return NULL; \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        struct SNAME *_set_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_set_) \
            return NULL; \
\
        _set_->count = 0; \
        _set_->root = NULL; \
        _set_->flag = CMC_FLAG_OK; \
        _set_->f_val = f_val; \
        _set_->alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(_set_, callbacks); \
\
        return _set_; \
    } \
\
    void CMC_(PFX, _clear)(struct SNAME * _set_) \
    { \
        struct CMC_DEF_NODE(SNAME) *scan = _set_->root; \
        struct CMC_DEF_NODE(SNAME) *up = NULL; \
\
        while (scan != NULL) \
        { \
            if (scan->left != NULL) \
            { \
                struct CMC_DEF_NODE(SNAME) *left = scan->left; \
\
                scan->left = up; \
                up = scan; \
                scan = left; \
            } \
            else if (scan->right != NULL) \
            { \
                struct CMC_DEF_NODE(SNAME) *right = scan->right; \
\
                scan->left = up; \
                scan->right = NULL; \
                up = scan; \
                scan = right; \
            } \
            else \
            { \
                if (up == NULL) \
                { \
                    if (_set_->f_val->free) \
                        _set_->f_val->free(scan->value); \
\
                    _set_->alloc->free(scan); \
                    scan = NULL; \
                } \
\
                while (up != NULL) \
                { \
                    if (_set_->f_val->free) \
                        _set_->f_val->free(scan->value); \
\
                    _set_->alloc->free(scan); \
\
                    if (up->right != NULL) \
                    { \
                        scan = up->right; \
                        up->right = NULL; \
                        break; \
                    } \
                    else \
                    { \
                        scan = up; \
                        up = up->left; \
                    } \
                } \
            } \
        } \
\
        _set_->count = 0; \
        _set_->root = NULL; \
        _set_->flag = CMC_FLAG_OK; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _set_) \
    { \
        CMC_(PFX, _clear)(_set_); \
\
        _set_->alloc->free(_set_); \
    } \
\
    void CMC_(PFX, _customize)(struct SNAME * _set_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!alloc) \
            _set_->alloc = &cmc_alloc_node_default; \
        else \
            _set_->alloc = alloc; \
\
        CMC_CALLBACKS_ASSIGN(_set_, callbacks); \
\
        _set_->flag = CMC_FLAG_OK; \
    } \
\
    bool CMC_(PFX, _insert)(struct SNAME * _set_, V value) \
    { \
        if (CMC_(PFX, _empty)(_set_)) \
        { \
            _set_->root = CMC_(PFX, _impl_new_node)(_set_, value); \
\
            if (!_set_->root) \
            { \
                _set_->flag = CMC_FLAG_ALLOC; \
                return false; \
            } \
        } \
        else \
        { \
            struct CMC_DEF_NODE(SNAME) *scan = _set_->root; \
            struct CMC_DEF_NODE(SNAME) *parent = scan; \
\
            while (scan != NULL) \
            { \
                parent = scan; \
\
                if (_set_->f_val->cmp(scan->value, value) > 0) \
                    scan = scan->left; \
                else if (_set_->f_val->cmp(scan->value, value) < 0) \
                    scan = scan->right; \
                else \
                { \
                    _set_->flag = CMC_FLAG_DUPLICATE; \
                    return false; \
                } \
            } \
\
            struct CMC_DEF_NODE(SNAME) * node; \
\
            if (_set_->f_val->cmp(parent->value, value) > 0) \
            { \
                parent->left = CMC_(PFX, _impl_new_node)(_set_, value); \
\
                if (!parent->left) \
                { \
                    _set_->flag = CMC_FLAG_ALLOC; \
                    return false; \
                } \
\
                parent->left->parent = parent; \
                node = parent->left; \
            } \
            else \
            { \
                parent->right = CMC_(PFX, _impl_new_node)(_set_, value); \
\
                if (!parent->right) \
                { \
                    _set_->flag = CMC_FLAG_ALLOC; \
                    return false; \
                } \
\
                parent->right->parent = parent; \
                node = parent->right; \
            } \
\
            CMC_(PFX, _impl_rebalance)(_set_, node); \
        } \
\
        _set_->count++; \
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _remove)(struct SNAME * _set_, V value) \
    { \
        if (CMC_(PFX, _empty)(_set_)) \
        { \
            _set_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        struct CMC_DEF_NODE(SNAME) *node = CMC_(PFX, _impl_get_node)(_set_, value); \
\
        if (!node) \
        { \
            _set_->flag = CMC_FLAG_NOT_FOUND; \
            return false; \
        } \
\
        struct CMC_DEF_NODE(SNAME) *temp = NULL, *unbalanced = NULL; \
\
        bool is_root = node->parent == NULL; \
\
        if (node->left == NULL && node->right == NULL) \
        { \
            if (is_root) \
                _set_->root = NULL; \
            else \
            { \
                unbalanced = node->parent; \
\
                if (node->parent->right == node) \
                    node->parent->right = NULL; \
                else \
                    node->parent->left = NULL; \
            } \
\
            _set_->alloc->free(node); \
        } \
        else if (node->left == NULL) \
        { \
            if (is_root) \
            { \
                _set_->root = node->right; \
                _set_->root->parent = NULL; \
            } \
            else \
            { \
                unbalanced = node->parent; \
\
                node->right->parent = node->parent; \
\
                if (node->parent->right == node) \
                    node->parent->right = node->right; \
                else \
                    node->parent->left = node->right; \
            } \
\
            _set_->alloc->free(node); \
        } \
        else if (node->right == NULL) \
        { \
            if (is_root) \
            { \
                _set_->root = node->left; \
                _set_->root->parent = NULL; \
            } \
            else \
            { \
                unbalanced = node->parent; \
\
                node->left->parent = node->parent; \
\
                if (node->parent->right == node) \
                    node->parent->right = node->left; \
                else \
                    node->parent->left = node->left; \
            } \
\
            _set_->alloc->free(node); \
        } \
        else \
        { \
            temp = node->right; \
            while (temp->left != NULL) \
                temp = temp->left; \
\
            V temp_value = temp->value; \
\
            unbalanced = temp->parent; \
\
            if (temp->left == NULL && temp->right == NULL) \
            { \
                if (temp->parent->right == temp) \
                    temp->parent->right = NULL; \
                else \
                    temp->parent->left = NULL; \
            } \
            else if (temp->left == NULL) \
            { \
                temp->right->parent = temp->parent; \
\
                if (temp->parent->right == temp) \
                    temp->parent->right = temp->right; \
                else \
                    temp->parent->left = temp->right; \
            } \
            else if (temp->right == NULL) \
            { \
                temp->left->parent = temp->parent; \
\
                if (temp->parent->right == temp) \
                    temp->parent->right = temp->left; \
                else \
                    temp->parent->left = temp->left; \
            } \
\
            _set_->alloc->free(temp); \
\
            node->value = temp_value; \
        } \
\
        if (unbalanced != NULL) \
            CMC_(PFX, _impl_rebalance)(_set_, unbalanced); \
\
        if (_set_->count == 0) \
            _set_->root = NULL; \
\
        _set_->count--; \
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, delete); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _max)(struct SNAME * _set_, V * value) \
    { \
        if (CMC_(PFX, _empty)(_set_)) \
        { \
            _set_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        struct CMC_DEF_NODE(SNAME) *scan = _set_->root; \
\
        while (scan->right != NULL) \
            scan = scan->right; \
\
        if (value) \
            *value = scan->value; \
\
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, read); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _min)(struct SNAME * _set_, V * value) \
    { \
        if (CMC_(PFX, _empty)(_set_)) \
        { \
            _set_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        struct CMC_DEF_NODE(SNAME) *scan = _set_->root; \
\
        while (scan->left != NULL) \
            scan = scan->left; \
\
        if (value) \
            *value = scan->value; \
\
        _set_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_set_, read); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _contains)(struct SNAME * _set_, V value) \
    { \
        bool result = CMC_(PFX, _impl_get_node)(_set_, value) != NULL; \
\
        CMC_CALLBACKS_CALL(_set_, read); \
\
        return result; \
    } \
\
    bool CMC_(PFX, _empty)(struct SNAME * _set_) \
    { \
        return _set_->count == 0; \
    } \
\
    size_t CMC_(PFX, _count)(struct SNAME * _set_) \
    { \
        return _set_->count; \
    } \
\
    int CMC_(PFX, _flag)(struct SNAME * _set_) \
    { \
        return _set_->flag; \
    } \
\
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _set_) \
    { \
        struct SNAME *result = CMC_(PFX, _new_custom)(_set_->f_val, _set_->alloc, NULL); \
\
        if (!result) \
        { \
            _set_->flag = CMC_FLAG_ERROR; \
            return NULL; \
        } \
\
        CMC_CALLBACKS_ASSIGN(result, _set_->callbacks); \
\
        if (!CMC_(PFX, _empty)(_set_)) \
        { \
            struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set_); \
\
            for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter)) \
            { \
                if (_set_->f_val->cpy) \
                    CMC_(PFX, _insert) \
                (result, _set_->f_val->cpy(CMC_(PFX, _iter_value)(&iter))); \
                else CMC_(PFX, _insert)(result, CMC_(PFX, _iter_value)(&iter)); \
            } \
        } \
\
        _set_->flag = CMC_FLAG_OK; \
\
        return result; \
    } \
\
    bool CMC_(PFX, _equals)(struct SNAME * _set1_, struct SNAME * _set2_) \
    { \
        _set1_->flag = CMC_FLAG_OK; \
        _set2_->flag = CMC_FLAG_OK; \
\
        if (_set1_->count != _set2_->count) \
            return false; \
\
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_); \
\
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter)) \
        { \
            if (CMC_(PFX, _impl_get_node)(_set2_, CMC_(PFX, _iter_value)(&iter)) == NULL) \
                return false; \
        } \
\
        return true; \
    } \
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
    } \
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
    } \
\
    static struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_new_node)(struct SNAME * _set_, V value) \
    { \
        struct CMC_DEF_NODE(SNAME) *node = _set_->alloc->malloc(sizeof(struct CMC_DEF_NODE(SNAME))); \
\
        if (!node) \
            return NULL; \
\
        node->value = value; \
        node->right = NULL; \
        node->left = NULL; \
        node->parent = NULL; \
        node->height = 0; \
\
        return node; \
    } \
\
    static struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_get_node)(struct SNAME * _set_, V value) \
    { \
        struct CMC_DEF_NODE(SNAME) *scan = _set_->root; \
\
        while (scan != NULL) \
        { \
            if (_set_->f_val->cmp(scan->value, value) > 0) \
                scan = scan->left; \
            else if (_set_->f_val->cmp(scan->value, value) < 0) \
                scan = scan->right; \
            else \
                return scan; \
        } \
\
        return NULL; \
    } \
\
    static unsigned char CMC_(PFX, _impl_h)(struct CMC_DEF_NODE(SNAME) * node) \
    { \
        if (node == NULL) \
            return 0; \
\
        return node->height; \
    } \
\
    static unsigned char CMC_(PFX, _impl_hupdate)(struct CMC_DEF_NODE(SNAME) * node) \
    { \
        if (node == NULL) \
            return 0; \
\
        unsigned char h_l = CMC_(PFX, _impl_h)(node->left); \
        unsigned char h_r = CMC_(PFX, _impl_h)(node->right); \
\
        return 1 + (h_l > h_r ? h_l : h_r); \
    } \
\
    static void CMC_(PFX, _impl_rotate_right)(struct CMC_DEF_NODE(SNAME) * *Z) \
    { \
        struct CMC_DEF_NODE(SNAME) *root = *Z; \
        struct CMC_DEF_NODE(SNAME) *new_root = root->left; \
\
        if (root->parent != NULL) \
        { \
            if (root->parent->left == root) \
                root->parent->left = new_root; \
            else \
                root->parent->right = new_root; \
        } \
\
        new_root->parent = root->parent; \
\
        root->parent = new_root; \
        root->left = new_root->right; \
\
        if (root->left) \
            root->left->parent = root; \
\
        new_root->right = root; \
\
        root->height = CMC_(PFX, _impl_hupdate)(root); \
        new_root->height = CMC_(PFX, _impl_hupdate)(new_root); \
\
        *Z = new_root; \
    } \
\
    static void CMC_(PFX, _impl_rotate_left)(struct CMC_DEF_NODE(SNAME) * *Z) \
    { \
        struct CMC_DEF_NODE(SNAME) *root = *Z; \
        struct CMC_DEF_NODE(SNAME) *new_root = root->right; \
\
        if (root->parent != NULL) \
        { \
            if (root->parent->right == root) \
                root->parent->right = new_root; \
            else \
                root->parent->left = new_root; \
        } \
\
        new_root->parent = root->parent; \
\
        root->parent = new_root; \
        root->right = new_root->left; \
\
        if (root->right) \
            root->right->parent = root; \
\
        new_root->left = root; \
\
        root->height = CMC_(PFX, _impl_hupdate)(root); \
        new_root->height = CMC_(PFX, _impl_hupdate)(new_root); \
\
        *Z = new_root; \
    } \
\
    static void CMC_(PFX, _impl_rebalance)(struct SNAME * _set_, struct CMC_DEF_NODE(SNAME) * node) \
    { \
        struct CMC_DEF_NODE(SNAME) *scan = node, *child = NULL; \
\
        int balance; \
        bool is_root = false; \
\
        while (scan != NULL) \
        { \
            if (scan->parent == NULL) \
                is_root = true; \
\
            scan->height = CMC_(PFX, _impl_hupdate)(scan); \
            balance = CMC_(PFX, _impl_h)(scan->right) - CMC_(PFX, _impl_h)(scan->left); \
\
            if (balance >= 2) \
            { \
                child = scan->right; \
\
                if (CMC_(PFX, _impl_h)(child->right) < CMC_(PFX, _impl_h)(child->left)) \
                    CMC_(PFX, _impl_rotate_right)(&(scan->right)); \
\
                CMC_(PFX, _impl_rotate_left)(&scan); \
            } \
            else if (balance <= -2) \
            { \
                child = scan->left; \
\
                if (CMC_(PFX, _impl_h)(child->left) < CMC_(PFX, _impl_h)(child->right)) \
                    CMC_(PFX, _impl_rotate_left)(&(scan->left)); \
\
                CMC_(PFX, _impl_rotate_right)(&scan); \
            } \
\
            if (is_root) \
            { \
                _set_->root = scan; \
                is_root = false; \
            } \
\
            scan = scan->parent; \
        } \
    }

#endif /* CMC_CMC_TREESET_H */
