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
 * cmc_treeset.h
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
#include "cor_core.h"

/**
 * Core TreeSet implementation
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_CMC_TREESET_CORE(ACCESS, FILE, PARAMS) CMC_(CMC_(CMC_CMC_TREESET_CORE_, ACCESS), CMC_(_, FILE))(PARAMS)

/* PRIVATE or PUBLIC solver */
#define CMC_CMC_TREESET_CORE_PUBLIC_HEADER(PARAMS) \
    CMC_CMC_TREESET_CORE_STRUCT(PARAMS) \
    CMC_CMC_TREESET_CORE_HEADER(PARAMS)

#define CMC_CMC_TREESET_CORE_PUBLIC_SOURCE(PARAMS) CMC_CMC_TREESET_CORE_SOURCE(PARAMS)

#define CMC_CMC_TREESET_CORE_PRIVATE_HEADER(PARAMS) \
    struct CMC_PARAM_SNAME(PARAMS); \
    CMC_CMC_TREESET_CORE_HEADER(PARAMS)

#define CMC_CMC_TREESET_CORE_PRIVATE_SOURCE(PARAMS) \
    CMC_CMC_TREESET_CORE_STRUCT(PARAMS) \
    CMC_CMC_TREESET_CORE_SOURCE(PARAMS)

/* Lowest level API */
#define CMC_CMC_TREESET_CORE_STRUCT(PARAMS) \
    CMC_CMC_TREESET_CORE_STRUCT_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_TREESET_CORE_HEADER(PARAMS) \
    CMC_CMC_TREESET_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_TREESET_CORE_SOURCE(PARAMS) \
    CMC_CMC_TREESET_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Struct
 * ------------------------------------------------------------------------- */
#define CMC_CMC_TREESET_CORE_STRUCT_(PFX, SNAME, V) \
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
    };

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_TREESET_CORE_HEADER_(PFX, SNAME, V) \
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
    bool CMC_(PFX, _equals)(struct SNAME * _set1_, struct SNAME * _set2_);

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
