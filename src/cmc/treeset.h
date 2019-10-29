/**
 * treeset.h
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

#ifndef CMC_TREESET_H
#define CMC_TREESET_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_treeset = "%s at %p { root:%p, count:%" PRIuMAX ", cmp:%p }";

#define CMC_GENERATE_TREESET(PFX, SNAME, V)    \
    CMC_GENERATE_TREESET_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_TREESET_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_TREESET_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_TREESET_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_TREESET_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_TREESET_SOURCE(PFX, SNAME, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_TREESET_HEADER(PFX, SNAME, V)                                        \
                                                                                          \
    /* Treeset Structure */                                                               \
    struct SNAME                                                                          \
    {                                                                                     \
        /* Root node */                                                                   \
        struct SNAME##_node *root;                                                        \
                                                                                          \
        /* Current amount of elements */                                                  \
        size_t count;                                                                     \
                                                                                          \
        /* Element comparison function */                                                 \
        int (*cmp)(V, V);                                                                 \
                                                                                          \
        /* Function that returns an iterator to the start of the treeset */               \
        struct SNAME##_iter (*it_start)(struct SNAME *);                                  \
                                                                                          \
        /* Function that returns an iterator to the end of the treeset */                 \
        struct SNAME##_iter (*it_end)(struct SNAME *);                                    \
    };                                                                                    \
                                                                                          \
    /* Treeset Node */                                                                    \
    struct SNAME##_node                                                                   \
    {                                                                                     \
        /* Node element */                                                                \
        V value;                                                                          \
                                                                                          \
        /* Node height used by the AVL tree to keep it strictly balanced */               \
        unsigned char height;                                                             \
                                                                                          \
        /* Right child node or subtree */                                                 \
        struct SNAME##_node *right;                                                       \
                                                                                          \
        /* Left child node or subtree */                                                  \
        struct SNAME##_node *left;                                                        \
                                                                                          \
        /* Parent node */                                                                 \
        struct SNAME##_node *parent;                                                      \
    };                                                                                    \
                                                                                          \
    /* Treeset Iterator */                                                                \
    struct SNAME##_iter                                                                   \
    {                                                                                     \
        /* Target treeset */                                                              \
        struct SNAME *target;                                                             \
                                                                                          \
        /* Cursor's current node */                                                       \
        struct SNAME##_node *cursor;                                                      \
                                                                                          \
        /* The first node in the iteration */                                             \
        struct SNAME##_node *first;                                                       \
                                                                                          \
        /* The last node in the iteration */                                              \
        struct SNAME##_node *last;                                                        \
                                                                                          \
        /* Keeps track of relative index to the iteration of elements */                  \
        size_t index;                                                                     \
                                                                                          \
        /* If the iterator has reached the start of the iteration */                      \
        bool start;                                                                       \
                                                                                          \
        /* If the iterator has reached the end of the iteration */                        \
        bool end;                                                                         \
    };                                                                                    \
                                                                                          \
    /* Collection Functions */                                                            \
    /* Collection Allocation and Deallocation */                                          \
    struct SNAME *PFX##_new(int (*compare)(V, V));                                        \
    void PFX##_clear(struct SNAME *_set_, void (*deallocator)(V));                        \
    void PFX##_free(struct SNAME *_set_, void (*deallocator)(V));                         \
    /* Collection Input and Output */                                                     \
    bool PFX##_insert(struct SNAME *_set_, V element);                                    \
    bool PFX##_remove(struct SNAME *_set_, V element);                                    \
    /* Element Access */                                                                  \
    bool PFX##_max(struct SNAME *_set_, V *value);                                        \
    bool PFX##_min(struct SNAME *_set_, V *value);                                        \
    /* Collection State */                                                                \
    bool PFX##_contains(struct SNAME *_set_, V element);                                  \
    bool PFX##_empty(struct SNAME *_set_);                                                \
    size_t PFX##_count(struct SNAME *_set_);                                              \
    /* Collection Utility */                                                              \
    struct SNAME *PFX##_copy_of(struct SNAME *_set_, V (*copy_func)(V));                  \
    bool PFX##_equals(struct SNAME *_set1_, struct SNAME *_set2_);                        \
    struct cmc_string PFX##_to_string(struct SNAME *_set_);                               \
                                                                                          \
    /* Set Operations */                                                                  \
    struct SNAME *PFX##_union(struct SNAME *_set1_, struct SNAME *_set2_);                \
    struct SNAME *PFX##_intersection(struct SNAME *_set1_, struct SNAME *_set2_);         \
    struct SNAME *PFX##_difference(struct SNAME *_set1_, struct SNAME *_set2_);           \
    struct SNAME *PFX##_symmetric_difference(struct SNAME *_set1_, struct SNAME *_set2_); \
    bool PFX##_is_subset(struct SNAME *_set1_, struct SNAME *_set2_);                     \
    bool PFX##_is_superset(struct SNAME *_set1_, struct SNAME *_set2_);                   \
    bool PFX##_is_proper_subset(struct SNAME *_set1_, struct SNAME *_set2_);              \
    bool PFX##_is_proper_superset(struct SNAME *_set1_, struct SNAME *_set2_);            \
    bool PFX##_is_disjointset(struct SNAME *_set1_, struct SNAME *_set2_);                \
                                                                                          \
    /* Iterator Functions */                                                              \
    /* Iterator Allocation and Deallocation */                                            \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);                            \
    void PFX##_iter_free(struct SNAME##_iter *iter);                                      \
    /* Iterator Initialization */                                                         \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);                \
    /* Iterator State */                                                                  \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                                     \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                                       \
    /* Iterator Movement */                                                               \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                                  \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                                    \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                                      \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                                      \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);                     \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);                      \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);                       \
    /* Iterator Access */                                                                 \
    V PFX##_iter_value(struct SNAME##_iter *iter);                                        \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);                                   \
                                                                                          \
    /* Default Value */                                                                   \
    static inline V PFX##_impl_default_value(void)                                        \
    {                                                                                     \
        V _empty_value_;                                                                  \
                                                                                          \
        memset(&_empty_value_, 0, sizeof(V));                                             \
                                                                                          \
        return _empty_value_;                                                             \
    }                                                                                     \
                                                                                          \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_TREESET_SOURCE(PFX, SNAME, V)                                           \
                                                                                             \
    /* Implementation Detail Functions */                                                    \
    static struct SNAME##_node *PFX##_impl_new_node(V element);                              \
    static struct SNAME##_node *PFX##_impl_get_node(struct SNAME *_set_, V element);         \
    static unsigned char PFX##_impl_h(struct SNAME##_node *node);                            \
    static unsigned char PFX##_impl_hupdate(struct SNAME##_node *node);                      \
    static void PFX##_impl_rotate_right(struct SNAME##_node **Z);                            \
    static void PFX##_impl_rotate_left(struct SNAME##_node **Z);                             \
    static void PFX##_impl_rebalance(struct SNAME *_set_, struct SNAME##_node *node);        \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_set_);                     \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_set_);                       \
                                                                                             \
    struct SNAME *PFX##_new(int (*compare)(V, V))                                            \
    {                                                                                        \
        struct SNAME *_set_ = malloc(sizeof(struct SNAME));                                  \
                                                                                             \
        if (!_set_)                                                                          \
            return NULL;                                                                     \
                                                                                             \
        _set_->count = 0;                                                                    \
        _set_->root = NULL;                                                                  \
        _set_->cmp = compare;                                                                \
                                                                                             \
        _set_->it_start = PFX##_impl_it_start;                                               \
        _set_->it_end = PFX##_impl_it_end;                                                   \
                                                                                             \
        return _set_;                                                                        \
    }                                                                                        \
                                                                                             \
    void PFX##_clear(struct SNAME *_set_, void (*deallocator)(V))                            \
    {                                                                                        \
        struct SNAME##_node *scan = _set_->root;                                             \
        struct SNAME##_node *up = NULL;                                                      \
                                                                                             \
        while (scan != NULL)                                                                 \
        {                                                                                    \
            if (scan->left != NULL)                                                          \
            {                                                                                \
                struct SNAME##_node *left = scan->left;                                      \
                                                                                             \
                scan->left = up;                                                             \
                up = scan;                                                                   \
                scan = left;                                                                 \
            }                                                                                \
            else if (scan->right != NULL)                                                    \
            {                                                                                \
                struct SNAME##_node *right = scan->right;                                    \
                                                                                             \
                scan->left = up;                                                             \
                scan->right = NULL;                                                          \
                up = scan;                                                                   \
                scan = right;                                                                \
            }                                                                                \
            else                                                                             \
            {                                                                                \
                if (up == NULL)                                                              \
                {                                                                            \
                    if (deallocator)                                                         \
                        deallocator(scan->value);                                            \
                                                                                             \
                    free(scan);                                                              \
                    scan = NULL;                                                             \
                }                                                                            \
                                                                                             \
                while (up != NULL)                                                           \
                {                                                                            \
                    if (deallocator)                                                         \
                        deallocator(scan->value);                                            \
                                                                                             \
                    free(scan);                                                              \
                                                                                             \
                    if (up->right != NULL)                                                   \
                    {                                                                        \
                        scan = up->right;                                                    \
                        up->right = NULL;                                                    \
                        break;                                                               \
                    }                                                                        \
                    else                                                                     \
                    {                                                                        \
                        scan = up;                                                           \
                        up = up->left;                                                       \
                    }                                                                        \
                }                                                                            \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        _set_->count = 0;                                                                    \
        _set_->root = NULL;                                                                  \
    }                                                                                        \
                                                                                             \
    void PFX##_free(struct SNAME *_set_, void (*deallocator)(V))                             \
    {                                                                                        \
        PFX##_clear(_set_, deallocator);                                                     \
                                                                                             \
        free(_set_);                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_insert(struct SNAME *_set_, V element)                                        \
    {                                                                                        \
        if (PFX##_empty(_set_))                                                              \
        {                                                                                    \
            _set_->root = PFX##_impl_new_node(element);                                      \
                                                                                             \
            if (!_set_->root)                                                                \
                return false;                                                                \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            struct SNAME##_node *scan = _set_->root;                                         \
            struct SNAME##_node *parent = scan;                                              \
                                                                                             \
            while (scan != NULL)                                                             \
            {                                                                                \
                parent = scan;                                                               \
                                                                                             \
                if (_set_->cmp(scan->value, element) > 0)                                    \
                    scan = scan->left;                                                       \
                else if (_set_->cmp(scan->value, element) < 0)                               \
                    scan = scan->right;                                                      \
                else                                                                         \
                    return false;                                                            \
            }                                                                                \
                                                                                             \
            struct SNAME##_node *node;                                                       \
                                                                                             \
            if (_set_->cmp(parent->value, element) > 0)                                      \
            {                                                                                \
                parent->left = PFX##_impl_new_node(element);                                 \
                                                                                             \
                if (!parent->left)                                                           \
                    return false;                                                            \
                                                                                             \
                parent->left->parent = parent;                                               \
                node = parent->left;                                                         \
            }                                                                                \
            else                                                                             \
            {                                                                                \
                parent->right = PFX##_impl_new_node(element);                                \
                                                                                             \
                if (!parent->right)                                                          \
                    return false;                                                            \
                                                                                             \
                parent->right->parent = parent;                                              \
                node = parent->right;                                                        \
            }                                                                                \
                                                                                             \
            PFX##_impl_rebalance(_set_, node);                                               \
        }                                                                                    \
                                                                                             \
        _set_->count++;                                                                      \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_remove(struct SNAME *_set_, V element)                                        \
    {                                                                                        \
        struct SNAME##_node *node = PFX##_impl_get_node(_set_, element);                     \
                                                                                             \
        if (!node)                                                                           \
            return false;                                                                    \
                                                                                             \
        struct SNAME##_node *temp = NULL, *unbalanced = NULL;                                \
                                                                                             \
        bool is_root = node->parent == NULL;                                                 \
                                                                                             \
        if (node->left == NULL && node->right == NULL)                                       \
        {                                                                                    \
            if (is_root)                                                                     \
                _set_->root = NULL;                                                          \
            else                                                                             \
            {                                                                                \
                unbalanced = node->parent;                                                   \
                                                                                             \
                if (node->parent->right == node)                                             \
                    node->parent->right = NULL;                                              \
                else                                                                         \
                    node->parent->left = NULL;                                               \
            }                                                                                \
                                                                                             \
            free(node);                                                                      \
        }                                                                                    \
        else if (node->left == NULL)                                                         \
        {                                                                                    \
            if (is_root)                                                                     \
            {                                                                                \
                _set_->root = node->right;                                                   \
                _set_->root->parent = NULL;                                                  \
            }                                                                                \
            else                                                                             \
            {                                                                                \
                unbalanced = node->parent;                                                   \
                                                                                             \
                node->right->parent = node->parent;                                          \
                                                                                             \
                if (node->parent->right == node)                                             \
                    node->parent->right = node->right;                                       \
                else                                                                         \
                    node->parent->left = node->right;                                        \
            }                                                                                \
                                                                                             \
            free(node);                                                                      \
        }                                                                                    \
        else if (node->right == NULL)                                                        \
        {                                                                                    \
            if (is_root)                                                                     \
            {                                                                                \
                _set_->root = node->left;                                                    \
                _set_->root->parent = NULL;                                                  \
            }                                                                                \
            else                                                                             \
            {                                                                                \
                unbalanced = node->parent;                                                   \
                                                                                             \
                node->left->parent = node->parent;                                           \
                                                                                             \
                if (node->parent->right == node)                                             \
                    node->parent->right = node->left;                                        \
                else                                                                         \
                    node->parent->left = node->left;                                         \
            }                                                                                \
                                                                                             \
            free(node);                                                                      \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            temp = node->right;                                                              \
            while (temp->left != NULL)                                                       \
                temp = temp->left;                                                           \
                                                                                             \
            V temp_value = temp->value;                                                      \
                                                                                             \
            unbalanced = temp->parent;                                                       \
                                                                                             \
            if (temp->left == NULL && temp->right == NULL)                                   \
            {                                                                                \
                if (temp->parent->right == temp)                                             \
                    temp->parent->right = NULL;                                              \
                else                                                                         \
                    temp->parent->left = NULL;                                               \
            }                                                                                \
            else if (temp->left == NULL)                                                     \
            {                                                                                \
                temp->right->parent = temp->parent;                                          \
                                                                                             \
                if (temp->parent->right == temp)                                             \
                    temp->parent->right = temp->right;                                       \
                else                                                                         \
                    temp->parent->left = temp->right;                                        \
            }                                                                                \
            else if (temp->right == NULL)                                                    \
            {                                                                                \
                temp->left->parent = temp->parent;                                           \
                                                                                             \
                if (temp->parent->right == temp)                                             \
                    temp->parent->right = temp->left;                                        \
                else                                                                         \
                    temp->parent->left = temp->left;                                         \
            }                                                                                \
                                                                                             \
            free(temp);                                                                      \
                                                                                             \
            node->value = temp_value;                                                        \
        }                                                                                    \
                                                                                             \
        if (unbalanced != NULL)                                                              \
            PFX##_impl_rebalance(_set_, unbalanced);                                         \
                                                                                             \
        _set_->count--;                                                                      \
                                                                                             \
        if (_set_->count == 0)                                                               \
            _set_->root = NULL;                                                              \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_max(struct SNAME *_set_, V *value)                                            \
    {                                                                                        \
        if (PFX##_empty(_set_))                                                              \
            return false;                                                                    \
                                                                                             \
        struct SNAME##_node *scan = _set_->root;                                             \
                                                                                             \
        while (scan->right != NULL)                                                          \
            scan = scan->right;                                                              \
                                                                                             \
        *value = scan->value;                                                                \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_min(struct SNAME *_set_, V *value)                                            \
    {                                                                                        \
        if (PFX##_empty(_set_))                                                              \
            return false;                                                                    \
                                                                                             \
        struct SNAME##_node *scan = _set_->root;                                             \
                                                                                             \
        while (scan->left != NULL)                                                           \
            scan = scan->left;                                                               \
                                                                                             \
        *value = scan->value;                                                                \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_contains(struct SNAME *_set_, V element)                                      \
    {                                                                                        \
        struct SNAME##_node *scan = _set_->root;                                             \
                                                                                             \
        while (scan != NULL)                                                                 \
        {                                                                                    \
            if (_set_->cmp(scan->value, element) > 0)                                        \
                scan = scan->left;                                                           \
            else if (_set_->cmp(scan->value, element) < 0)                                   \
                scan = scan->right;                                                          \
            else                                                                             \
                return true;                                                                 \
        }                                                                                    \
                                                                                             \
        return false;                                                                        \
    }                                                                                        \
                                                                                             \
    bool PFX##_empty(struct SNAME *_set_)                                                    \
    {                                                                                        \
        return _set_->count == 0;                                                            \
    }                                                                                        \
                                                                                             \
    size_t PFX##_count(struct SNAME *_set_)                                                  \
    {                                                                                        \
        return _set_->count;                                                                 \
    }                                                                                        \
                                                                                             \
    struct SNAME *PFX##_copy_of(struct SNAME *_set_, V (*copy_func)(V))                      \
    {                                                                                        \
        struct SNAME *result = PFX##_new(_set_->cmp);                                        \
                                                                                             \
        if (!result)                                                                         \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME##_iter iter;                                                            \
        PFX##_iter_init(&iter, _set_);                                                       \
                                                                                             \
        if (!PFX##_empty(_set_))                                                             \
        {                                                                                    \
            for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter)) \
            {                                                                                \
                if (copy_func)                                                               \
                    PFX##_insert(result, copy_func(PFX##_iter_value(&iter)));                \
                else                                                                         \
                    PFX##_insert(result, PFX##_iter_value(&iter));                           \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        return result;                                                                       \
    }                                                                                        \
                                                                                             \
    bool PFX##_equals(struct SNAME *_set1_, struct SNAME *_set2_)                            \
    {                                                                                        \
        if (PFX##_count(_set1_) != PFX##_count(_set2_))                                      \
            return false;                                                                    \
                                                                                             \
        struct SNAME##_iter iter;                                                            \
        PFX##_iter_init(&iter, _set1_);                                                      \
                                                                                             \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))     \
        {                                                                                    \
            if (PFX##_impl_get_node(_set2_, PFX##_iter_value(&iter)) == NULL)                \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    struct cmc_string PFX##_to_string(struct SNAME *_set_)                                   \
    {                                                                                        \
        struct cmc_string str;                                                               \
        struct SNAME *s_ = _set_;                                                            \
        const char *name = #SNAME;                                                           \
                                                                                             \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_treeset,                              \
                 name, s_, s_->root, s_->count, s_->cmp);                                    \
                                                                                             \
        return str;                                                                          \
    }                                                                                        \
                                                                                             \
    struct SNAME *PFX##_union(struct SNAME *_set1_, struct SNAME *_set2_)                    \
    {                                                                                        \
        struct SNAME *_set_r_ = PFX##_new(_set1_->cmp);                                      \
                                                                                             \
        if (!_set_r_)                                                                        \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME##_iter iter1, iter2;                                                    \
        PFX##_iter_init(&iter1, _set1_);                                                     \
        PFX##_iter_init(&iter2, _set2_);                                                     \
                                                                                             \
        for (PFX##_iter_to_start(&iter1); !PFX##_iter_end(&iter1); PFX##_iter_next(&iter1))  \
        {                                                                                    \
            PFX##_insert(_set_r_, PFX##_iter_value(&iter1));                                 \
        }                                                                                    \
                                                                                             \
        for (PFX##_iter_to_start(&iter2); !PFX##_iter_end(&iter2); PFX##_iter_next(&iter2))  \
        {                                                                                    \
            PFX##_insert(_set_r_, PFX##_iter_value(&iter2));                                 \
        }                                                                                    \
                                                                                             \
        return _set_r_;                                                                      \
    }                                                                                        \
                                                                                             \
    struct SNAME *PFX##_intersection(struct SNAME *_set1_, struct SNAME *_set2_)             \
    {                                                                                        \
        struct SNAME *_set_r_ = PFX##_new(_set1_->cmp);                                      \
                                                                                             \
        if (!_set_r_)                                                                        \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;             \
        struct SNAME *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;                         \
                                                                                             \
        struct SNAME##_iter iter;                                                            \
        PFX##_iter_init(&iter, _set_A_);                                                     \
                                                                                             \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))     \
        {                                                                                    \
            V value = PFX##_iter_value(&iter);                                               \
                                                                                             \
            if (PFX##_impl_get_node(_set_B_, value) != NULL)                                 \
                PFX##_insert(_set_r_, value);                                                \
        }                                                                                    \
                                                                                             \
        return _set_r_;                                                                      \
    }                                                                                        \
                                                                                             \
    struct SNAME *PFX##_difference(struct SNAME *_set1_, struct SNAME *_set2_)               \
    {                                                                                        \
        struct SNAME *_set_r_ = PFX##_new(_set1_->cmp);                                      \
                                                                                             \
        if (!_set_r_)                                                                        \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME##_iter iter;                                                            \
        PFX##_iter_init(&iter, _set1_);                                                      \
                                                                                             \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))     \
        {                                                                                    \
            V value = PFX##_iter_value(&iter);                                               \
                                                                                             \
            if (PFX##_impl_get_node(_set2_, value) == NULL)                                  \
                PFX##_insert(_set_r_, value);                                                \
        }                                                                                    \
                                                                                             \
        return _set_r_;                                                                      \
    }                                                                                        \
                                                                                             \
    struct SNAME *PFX##_symmetric_difference(struct SNAME *_set1_, struct SNAME *_set2_)     \
    {                                                                                        \
        struct SNAME *_set_r_ = PFX##_new(_set1_->cmp);                                      \
                                                                                             \
        if (!_set_r_)                                                                        \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME##_iter iter1, iter2;                                                    \
        PFX##_iter_init(&iter1, _set1_);                                                     \
        PFX##_iter_init(&iter2, _set2_);                                                     \
                                                                                             \
        for (PFX##_iter_to_start(&iter1); !PFX##_iter_end(&iter1); PFX##_iter_next(&iter1))  \
        {                                                                                    \
            V value = PFX##_iter_value(&iter1);                                              \
                                                                                             \
            if (PFX##_impl_get_node(_set2_, value) == NULL)                                  \
                PFX##_insert(_set_r_, value);                                                \
        }                                                                                    \
                                                                                             \
        for (PFX##_iter_to_start(&iter2); !PFX##_iter_end(&iter2); PFX##_iter_next(&iter2))  \
        {                                                                                    \
            V value = PFX##_iter_value(&iter2);                                              \
                                                                                             \
            if (PFX##_impl_get_node(_set1_, value) == NULL)                                  \
                PFX##_insert(_set_r_, value);                                                \
        }                                                                                    \
                                                                                             \
        return _set_r_;                                                                      \
    }                                                                                        \
                                                                                             \
    /* Is _set1_ a subset of _set2_ ? */                                                     \
    /* A set X is a subset of a set Y when: X <= Y */                                        \
    /* If X is a subset of Y, then Y is a superset of X */                                   \
    bool PFX##_is_subset(struct SNAME *_set1_, struct SNAME *_set2_)                         \
    {                                                                                        \
        /* If the cardinality of _set1_ is greater than that of _set2_, then it is safe */   \
        /* to say that _set1_ can't be a subset of _set2_ */                                 \
        if (PFX##_count(_set1_) > PFX##_count(_set2_))                                       \
            return false;                                                                    \
                                                                                             \
        /* The empty set is a subset of all sets */                                          \
        if (PFX##_empty(_set1_))                                                             \
            return true;                                                                     \
                                                                                             \
        struct SNAME##_iter iter;                                                            \
                                                                                             \
        PFX##_iter_init(&iter, _set1_);                                                      \
                                                                                             \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))     \
        {                                                                                    \
            V value = PFX##_iter_value(&iter);                                               \
                                                                                             \
            if (!PFX##_contains(_set2_, value))                                              \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    /* Is _set1_ a superset of _set2_ ? */                                                   \
    /* A set X is a superset of a set Y when: X >= Y */                                      \
    /* If X is a superset of Y, then Y is a subset of X */                                   \
    bool PFX##_is_superset(struct SNAME *_set1_, struct SNAME *_set2_)                       \
    {                                                                                        \
        return PFX##_is_subset(_set2_, _set1_);                                              \
    }                                                                                        \
                                                                                             \
    /* Is _set1_ a proper subset of _set2_ ? */                                              \
    /* A set X is a proper subset of a set Y when: X < Y */                                  \
    /* If X is a proper subset of Y, then Y is a proper superset of X */                     \
    bool PFX##_is_proper_subset(struct SNAME *_set1_, struct SNAME *_set2_)                  \
    {                                                                                        \
        /* If the cardinality of _set1_ is greater than or equal to that of _set2_, then */  \
        /* it is safe to say that _set1_ can't be a proper subset of _set2_ */               \
        if (PFX##_count(_set1_) >= PFX##_count(_set2_))                                      \
            return false;                                                                    \
                                                                                             \
        if (PFX##_empty(_set1_))                                                             \
        {                                                                                    \
            /* The empty set is a proper subset of all non-empty sets */                     \
            if (!PFX##_empty(_set2_))                                                        \
                return true;                                                                 \
            /* The empty set is not a proper subset of itself (this is true for any set) */  \
            else                                                                             \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        struct SNAME##_iter iter;                                                            \
                                                                                             \
        PFX##_iter_init(&iter, _set1_);                                                      \
                                                                                             \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))     \
        {                                                                                    \
            V value = PFX##_iter_value(&iter);                                               \
                                                                                             \
            if (!PFX##_contains(_set2_, value))                                              \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    /* Is _set1_ a proper superset of _set2_ ? */                                            \
    /* A set X is a proper superset of a set Y when: X > Y */                                \
    /* If X is a proper superset of Y, then Y is a proper subset of X */                     \
    bool PFX##_is_proper_superset(struct SNAME *_set1_, struct SNAME *_set2_)                \
    {                                                                                        \
        return PFX##_is_proper_subset(_set2_, _set1_);                                       \
    }                                                                                        \
                                                                                             \
    /* Is _set1_ a disjointset of _set2_ ? */                                                \
    /* A set X is a disjointset of a set Y if their intersection is empty, that is, if */    \
    /* there are no elements in common between the two */                                    \
    bool PFX##_is_disjointset(struct SNAME *_set1_, struct SNAME *_set2_)                    \
    {                                                                                        \
        /* The intersection of an empty set with any other set will result in an empty */    \
        /* set */                                                                            \
        if (PFX##_empty(_set1_))                                                             \
            return true;                                                                     \
                                                                                             \
        struct SNAME##_iter iter;                                                            \
                                                                                             \
        PFX##_iter_init(&iter, _set1_);                                                      \
                                                                                             \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))     \
        {                                                                                    \
            V value = PFX##_iter_value(&iter);                                               \
                                                                                             \
            if (PFX##_contains(_set2_, value))                                               \
                return false;                                                                \
        }                                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                                \
    {                                                                                        \
        struct SNAME##_iter *iter = malloc(sizeof(struct SNAME##_iter));                     \
                                                                                             \
        if (!iter)                                                                           \
            return NULL;                                                                     \
                                                                                             \
        PFX##_iter_init(iter, target);                                                       \
                                                                                             \
        return iter;                                                                         \
    }                                                                                        \
                                                                                             \
    void PFX##_iter_free(struct SNAME##_iter *iter)                                          \
    {                                                                                        \
        free(iter);                                                                          \
    }                                                                                        \
                                                                                             \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)                    \
    {                                                                                        \
        memset(iter, 0, sizeof(struct SNAME##_iter));                                        \
                                                                                             \
        iter->target = target;                                                               \
        iter->start = true;                                                                  \
        iter->end = PFX##_empty(target);                                                     \
                                                                                             \
        iter->cursor = target->root;                                                         \
                                                                                             \
        if (!PFX##_empty(target))                                                            \
        {                                                                                    \
            while (iter->cursor->left != NULL)                                               \
                iter->cursor = iter->cursor->left;                                           \
                                                                                             \
            iter->first = iter->cursor;                                                      \
                                                                                             \
            iter->last = target->root;                                                       \
            while (iter->last->right != NULL)                                                \
                iter->last = iter->last->right;                                              \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    bool PFX##_iter_start(struct SNAME##_iter *iter)                                         \
    {                                                                                        \
        return PFX##_empty(iter->target) || iter->start;                                     \
    }                                                                                        \
                                                                                             \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                                           \
    {                                                                                        \
        return PFX##_empty(iter->target) || iter->end;                                       \
    }                                                                                        \
                                                                                             \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                                      \
    {                                                                                        \
        if (!PFX##_empty(iter->target))                                                      \
        {                                                                                    \
            iter->index = 0;                                                                 \
            iter->start = true;                                                              \
            iter->end = PFX##_empty(iter->target);                                           \
            iter->cursor = iter->first;                                                      \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                                        \
    {                                                                                        \
        if (!PFX##_empty(iter->target))                                                      \
        {                                                                                    \
            iter->index = iter->target->count - 1;                                           \
            iter->start = PFX##_empty(iter->target);                                         \
            iter->end = true;                                                                \
            iter->cursor = iter->last;                                                       \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                                          \
    {                                                                                        \
        if (iter->end)                                                                       \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor == iter->last)                                                      \
        {                                                                                    \
            iter->end = true;                                                                \
            return true;                                                                     \
        }                                                                                    \
                                                                                             \
        iter->start = PFX##_empty(iter->target);                                             \
                                                                                             \
        if (iter->cursor->right != NULL)                                                     \
        {                                                                                    \
            iter->cursor = iter->cursor->right;                                              \
                                                                                             \
            while (iter->cursor->left != NULL)                                               \
                iter->cursor = iter->cursor->left;                                           \
                                                                                             \
            iter->index++;                                                                   \
                                                                                             \
            return true;                                                                     \
        }                                                                                    \
                                                                                             \
        while (true)                                                                         \
        {                                                                                    \
            if (iter->cursor->parent->left == iter->cursor)                                  \
            {                                                                                \
                iter->cursor = iter->cursor->parent;                                         \
                                                                                             \
                iter->index++;                                                               \
                                                                                             \
                return true;                                                                 \
            }                                                                                \
                                                                                             \
            iter->cursor = iter->cursor->parent;                                             \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                                          \
    {                                                                                        \
        if (iter->start)                                                                     \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor == iter->first)                                                     \
        {                                                                                    \
            iter->start = true;                                                              \
                                                                                             \
            return true;                                                                     \
        }                                                                                    \
                                                                                             \
        iter->end = PFX##_empty(iter->target);                                               \
                                                                                             \
        if (iter->cursor->left != NULL)                                                      \
        {                                                                                    \
            iter->cursor = iter->cursor->left;                                               \
                                                                                             \
            while (iter->cursor->right != NULL)                                              \
                iter->cursor = iter->cursor->right;                                          \
                                                                                             \
            iter->index--;                                                                   \
                                                                                             \
            return true;                                                                     \
        }                                                                                    \
                                                                                             \
        while (true)                                                                         \
        {                                                                                    \
            if (iter->cursor->parent->right == iter->cursor)                                 \
            {                                                                                \
                iter->cursor = iter->cursor->parent;                                         \
                                                                                             \
                iter->index--;                                                               \
                                                                                             \
                return true;                                                                 \
            }                                                                                \
                                                                                             \
            iter->cursor = iter->cursor->parent;                                             \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    /* Returns true only if the iterator moved */                                            \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)                         \
    {                                                                                        \
        if (iter->end)                                                                       \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor == iter->last)                                                      \
        {                                                                                    \
            iter->end = true;                                                                \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        if (steps == 0 || iter->index + steps >= PFX##_count(iter->target))                  \
            return false;                                                                    \
                                                                                             \
        iter->index += steps;                                                                \
                                                                                             \
        for (size_t i = 0; i < steps; i++)                                                   \
            PFX##_iter_next(iter);                                                           \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    /* Returns true only if the iterator moved */                                            \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps)                          \
    {                                                                                        \
        if (iter->start)                                                                     \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor == iter->first)                                                     \
        {                                                                                    \
            iter->start = true;                                                              \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        if (steps == 0 || iter->index < steps)                                               \
            return false;                                                                    \
                                                                                             \
        iter->index -= steps;                                                                \
                                                                                             \
        for (size_t i = 0; i < steps; i++)                                                   \
            PFX##_iter_prev(iter);                                                           \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    /* Returns true only if the iterator was able to be positioned at the given index */     \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)                           \
    {                                                                                        \
        if (index >= PFX##_count(iter->target))                                              \
            return false;                                                                    \
                                                                                             \
        if (iter->index > index)                                                             \
            return PFX##_iter_rewind(iter, iter->index - index);                             \
        else if (iter->index < index)                                                        \
            return PFX##_iter_advance(iter, index - iter->index);                            \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    V PFX##_iter_value(struct SNAME##_iter *iter)                                            \
    {                                                                                        \
        if (PFX##_empty(iter->target))                                                       \
            return (V){0};                                                                   \
                                                                                             \
        return iter->cursor->value;                                                          \
    }                                                                                        \
                                                                                             \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                                       \
    {                                                                                        \
        return iter->index;                                                                  \
    }                                                                                        \
                                                                                             \
    static struct SNAME##_node *PFX##_impl_new_node(V element)                               \
    {                                                                                        \
        struct SNAME##_node *node = malloc(sizeof(struct SNAME##_node));                     \
                                                                                             \
        if (!node)                                                                           \
            return NULL;                                                                     \
                                                                                             \
        node->value = element;                                                               \
        node->right = NULL;                                                                  \
        node->left = NULL;                                                                   \
        node->parent = NULL;                                                                 \
        node->height = 0;                                                                    \
                                                                                             \
        return node;                                                                         \
    }                                                                                        \
                                                                                             \
    static struct SNAME##_node *PFX##_impl_get_node(struct SNAME *_set_, V element)          \
    {                                                                                        \
        if (PFX##_empty(_set_))                                                              \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME##_node *scan = _set_->root;                                             \
                                                                                             \
        while (scan != NULL)                                                                 \
        {                                                                                    \
            if (_set_->cmp(scan->value, element) > 0)                                        \
                scan = scan->left;                                                           \
            else if (_set_->cmp(scan->value, element) < 0)                                   \
                scan = scan->right;                                                          \
            else                                                                             \
                return scan;                                                                 \
        }                                                                                    \
                                                                                             \
        return NULL;                                                                         \
    }                                                                                        \
                                                                                             \
    static unsigned char PFX##_impl_h(struct SNAME##_node *node)                             \
    {                                                                                        \
        if (node == NULL)                                                                    \
            return 0;                                                                        \
                                                                                             \
        return node->height;                                                                 \
    }                                                                                        \
                                                                                             \
    static unsigned char PFX##_impl_hupdate(struct SNAME##_node *node)                       \
    {                                                                                        \
        if (node == NULL)                                                                    \
            return 0;                                                                        \
                                                                                             \
        unsigned char h_l = PFX##_impl_h(node->left);                                        \
        unsigned char h_r = PFX##_impl_h(node->right);                                       \
                                                                                             \
        return 1 + (h_l > h_r ? h_l : h_r);                                                  \
    }                                                                                        \
                                                                                             \
    static void PFX##_impl_rotate_right(struct SNAME##_node **Z)                             \
    {                                                                                        \
        struct SNAME##_node *root = *Z;                                                      \
        struct SNAME##_node *new_root = root->left;                                          \
                                                                                             \
        if (root->parent != NULL)                                                            \
        {                                                                                    \
            if (root->parent->left == root)                                                  \
                root->parent->left = new_root;                                               \
            else                                                                             \
                root->parent->right = new_root;                                              \
        }                                                                                    \
                                                                                             \
        new_root->parent = root->parent;                                                     \
                                                                                             \
        root->parent = new_root;                                                             \
        root->left = new_root->right;                                                        \
                                                                                             \
        if (root->left)                                                                      \
            root->left->parent = root;                                                       \
                                                                                             \
        new_root->right = root;                                                              \
                                                                                             \
        root->height = PFX##_impl_hupdate(root);                                             \
        new_root->height = PFX##_impl_hupdate(new_root);                                     \
                                                                                             \
        *Z = new_root;                                                                       \
    }                                                                                        \
                                                                                             \
    static void PFX##_impl_rotate_left(struct SNAME##_node **Z)                              \
    {                                                                                        \
        struct SNAME##_node *root = *Z;                                                      \
        struct SNAME##_node *new_root = root->right;                                         \
                                                                                             \
        if (root->parent != NULL)                                                            \
        {                                                                                    \
            if (root->parent->right == root)                                                 \
                root->parent->right = new_root;                                              \
            else                                                                             \
                root->parent->left = new_root;                                               \
        }                                                                                    \
                                                                                             \
        new_root->parent = root->parent;                                                     \
                                                                                             \
        root->parent = new_root;                                                             \
        root->right = new_root->left;                                                        \
                                                                                             \
        if (root->right)                                                                     \
            root->right->parent = root;                                                      \
                                                                                             \
        new_root->left = root;                                                               \
                                                                                             \
        root->height = PFX##_impl_hupdate(root);                                             \
        new_root->height = PFX##_impl_hupdate(new_root);                                     \
                                                                                             \
        *Z = new_root;                                                                       \
    }                                                                                        \
                                                                                             \
    static void PFX##_impl_rebalance(struct SNAME *_set_, struct SNAME##_node *node)         \
    {                                                                                        \
        struct SNAME##_node *scan = node, *child = NULL;                                     \
                                                                                             \
        int balance;                                                                         \
        bool is_root = false;                                                                \
                                                                                             \
        while (scan != NULL)                                                                 \
        {                                                                                    \
            if (scan->parent == NULL)                                                        \
                is_root = true;                                                              \
                                                                                             \
            scan->height = PFX##_impl_hupdate(scan);                                         \
            balance = PFX##_impl_h(scan->right) - PFX##_impl_h(scan->left);                  \
                                                                                             \
            if (balance >= 2)                                                                \
            {                                                                                \
                child = scan->right;                                                         \
                                                                                             \
                if (PFX##_impl_h(child->right) < PFX##_impl_h(child->left))                  \
                    PFX##_impl_rotate_right(&(scan->right));                                 \
                                                                                             \
                PFX##_impl_rotate_left(&scan);                                               \
            }                                                                                \
            else if (balance <= -2)                                                          \
            {                                                                                \
                child = scan->left;                                                          \
                                                                                             \
                if (PFX##_impl_h(child->left) < PFX##_impl_h(child->right))                  \
                    PFX##_impl_rotate_left(&(scan->left));                                   \
                                                                                             \
                PFX##_impl_rotate_right(&scan);                                              \
            }                                                                                \
                                                                                             \
            if (is_root)                                                                     \
            {                                                                                \
                _set_->root = scan;                                                          \
                is_root = false;                                                             \
            }                                                                                \
                                                                                             \
            scan = scan->parent;                                                             \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_set_)                      \
    {                                                                                        \
        struct SNAME##_iter iter;                                                            \
                                                                                             \
        PFX##_iter_init(&iter, _set_);                                                       \
                                                                                             \
        return iter;                                                                         \
    }                                                                                        \
                                                                                             \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_set_)                        \
    {                                                                                        \
        struct SNAME##_iter iter;                                                            \
                                                                                             \
        PFX##_iter_init(&iter, _set_);                                                       \
        PFX##_iter_to_end(&iter);                                                            \
                                                                                             \
        return iter;                                                                         \
    }

#endif /* CMC_TREESET_H */
