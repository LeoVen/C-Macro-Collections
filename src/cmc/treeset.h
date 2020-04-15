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

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * TreeSet specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_string_fmt_treeset = "struct %s<%s> "
                                            "at %p { "
                                            "root:%p, "
                                            "count:%" PRIuMAX ", "
                                            "flag:%d, "
                                            "f_val:%p, "
                                            "alloc:%p, "
                                            "callbacks:%p }";

#define CMC_GENERATE_TREESET(PFX, SNAME, V)    \
    CMC_GENERATE_TREESET_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_TREESET_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_TREESET_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_TREESET_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_TREESET_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_TREESET_SOURCE(PFX, SNAME, V)

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_TREESET_HEADER(PFX, SNAME, V)                             \
                                                                               \
    /* Treeset Structure */                                                    \
    struct SNAME                                                               \
    {                                                                          \
        /* Root node */                                                        \
        struct SNAME##_node *root;                                             \
                                                                               \
        /* Current amount of elements */                                       \
        size_t count;                                                          \
                                                                               \
        /* Flags indicating errors or success */                               \
        int flag;                                                              \
                                                                               \
        /* Value function table */                                             \
        struct SNAME##_fval *f_val;                                            \
                                                                               \
        /* Custom allocation functions */                                      \
        struct cmc_alloc_node *alloc;                                          \
                                                                               \
        /* Custom callback functions */                                        \
        struct cmc_callbacks *callbacks;                                       \
                                                                               \
        /* Function that returns an iterator to the start of the treeset */    \
        struct SNAME##_iter (*it_start)(struct SNAME *);                       \
                                                                               \
        /* Function that returns an iterator to the end of the treeset */      \
        struct SNAME##_iter (*it_end)(struct SNAME *);                         \
    };                                                                         \
                                                                               \
    /* Treeset Node */                                                         \
    struct SNAME##_node                                                        \
    {                                                                          \
        /* Node element */                                                     \
        V value;                                                               \
                                                                               \
        /* Node height used by the AVL tree to keep it strictly balanced */    \
        unsigned char height;                                                  \
                                                                               \
        /* Right child node or subtree */                                      \
        struct SNAME##_node *right;                                            \
                                                                               \
        /* Left child node or subtree */                                       \
        struct SNAME##_node *left;                                             \
                                                                               \
        /* Parent node */                                                      \
        struct SNAME##_node *parent;                                           \
    };                                                                         \
                                                                               \
    /* Value struct function table */                                          \
    struct SNAME##_fval                                                        \
    {                                                                          \
        /* Comparator function */                                              \
        int (*cmp)(V, V);                                                      \
                                                                               \
        /* Copy function */                                                    \
        V (*cpy)(V);                                                           \
                                                                               \
        /* To string function */                                               \
        bool (*str)(FILE *, V);                                                \
                                                                               \
        /* Free from memory function */                                        \
        void (*free)(V);                                                       \
                                                                               \
        /* Hash function */                                                    \
        size_t (*hash)(V);                                                     \
                                                                               \
        /* Priority function */                                                \
        int (*pri)(V, V);                                                      \
    };                                                                         \
                                                                               \
    /* Treeset Iterator */                                                     \
    struct SNAME##_iter                                                        \
    {                                                                          \
        /* Target treeset */                                                   \
        struct SNAME *target;                                                  \
                                                                               \
        /* Cursor's current node */                                            \
        struct SNAME##_node *cursor;                                           \
                                                                               \
        /* The first node in the iteration */                                  \
        struct SNAME##_node *first;                                            \
                                                                               \
        /* The last node in the iteration */                                   \
        struct SNAME##_node *last;                                             \
                                                                               \
        /* Keeps track of relative index to the iteration of elements */       \
        size_t index;                                                          \
                                                                               \
        /* If the iterator has reached the start of the iteration */           \
        bool start;                                                            \
                                                                               \
        /* If the iterator has reached the end of the iteration */             \
        bool end;                                                              \
    };                                                                         \
                                                                               \
    /* Collection Functions */                                                 \
    /* Collection Allocation and Deallocation */                               \
    struct SNAME *PFX##_new(struct SNAME##_fval *f_val);                       \
    struct SNAME *PFX##_new_custom(struct SNAME##_fval *f_val,                 \
                                   struct cmc_alloc_node *alloc,               \
                                   struct cmc_callbacks *callbacks);           \
    void PFX##_clear(struct SNAME *_set_);                                     \
    void PFX##_free(struct SNAME *_set_);                                      \
    /* Customization of Allocation and Callbacks */                            \
    void PFX##_customize(struct SNAME *_set_, struct cmc_alloc_node *alloc,    \
                         struct cmc_callbacks *callbacks);                     \
    /* Collection Input and Output */                                          \
    bool PFX##_insert(struct SNAME *_set_, V element);                         \
    bool PFX##_remove(struct SNAME *_set_, V element);                         \
    /* Element Access */                                                       \
    bool PFX##_max(struct SNAME *_set_, V *value);                             \
    bool PFX##_min(struct SNAME *_set_, V *value);                             \
    /* Collection State */                                                     \
    bool PFX##_contains(struct SNAME *_set_, V element);                       \
    bool PFX##_empty(struct SNAME *_set_);                                     \
    size_t PFX##_count(struct SNAME *_set_);                                   \
    int PFX##_flag(struct SNAME *_set_);                                       \
    /* Collection Utility */                                                   \
    struct SNAME *PFX##_copy_of(struct SNAME *_set_);                          \
    bool PFX##_equals(struct SNAME *_set1_, struct SNAME *_set2_);             \
    struct cmc_string PFX##_to_string(struct SNAME *_set_);                    \
    bool PFX##_print(struct SNAME *_set_, FILE *fptr);                         \
                                                                               \
    /* Set Operations */                                                       \
    struct SNAME *PFX##_union(struct SNAME *_set1_, struct SNAME *_set2_);     \
    struct SNAME *PFX##_intersection(struct SNAME *_set1_,                     \
                                     struct SNAME *_set2_);                    \
    struct SNAME *PFX##_difference(struct SNAME *_set1_,                       \
                                   struct SNAME *_set2_);                      \
    struct SNAME *PFX##_symmetric_difference(struct SNAME *_set1_,             \
                                             struct SNAME *_set2_);            \
    bool PFX##_is_subset(struct SNAME *_set1_, struct SNAME *_set2_);          \
    bool PFX##_is_superset(struct SNAME *_set1_, struct SNAME *_set2_);        \
    bool PFX##_is_proper_subset(struct SNAME *_set1_, struct SNAME *_set2_);   \
    bool PFX##_is_proper_superset(struct SNAME *_set1_, struct SNAME *_set2_); \
    bool PFX##_is_disjointset(struct SNAME *_set1_, struct SNAME *_set2_);     \
                                                                               \
    /* Iterator Functions */                                                   \
    /* Iterator Allocation and Deallocation */                                 \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);                 \
    void PFX##_iter_free(struct SNAME##_iter *iter);                           \
    /* Iterator Initialization */                                              \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);     \
    /* Iterator State */                                                       \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                          \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                            \
    /* Iterator Movement */                                                    \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                       \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                         \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                           \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                           \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);          \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);           \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);            \
    /* Iterator Access */                                                      \
    V PFX##_iter_value(struct SNAME##_iter *iter);                             \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);                        \
                                                                               \
    /* Default Value */                                                        \
    static inline V PFX##_impl_default_value(void)                             \
    {                                                                          \
        V _empty_value_;                                                       \
                                                                               \
        memset(&_empty_value_, 0, sizeof(V));                                  \
                                                                               \
        return _empty_value_;                                                  \
    }

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_GENERATE_TREESET_SOURCE(PFX, SNAME, V)                             \
                                                                               \
    /* Implementation Detail Functions */                                      \
    static struct SNAME##_node *PFX##_impl_new_node(struct SNAME *_set_,       \
                                                    V element);                \
    static struct SNAME##_node *PFX##_impl_get_node(struct SNAME *_set_,       \
                                                    V element);                \
    static unsigned char PFX##_impl_h(struct SNAME##_node *node);              \
    static unsigned char PFX##_impl_hupdate(struct SNAME##_node *node);        \
    static void PFX##_impl_rotate_right(struct SNAME##_node **Z);              \
    static void PFX##_impl_rotate_left(struct SNAME##_node **Z);               \
    static void PFX##_impl_rebalance(struct SNAME *_set_,                      \
                                     struct SNAME##_node *node);               \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_set_);       \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_set_);         \
                                                                               \
    struct SNAME *PFX##_new(struct SNAME##_fval *f_val)                        \
    {                                                                          \
        if (!f_val)                                                            \
            return NULL;                                                       \
                                                                               \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;                \
                                                                               \
        struct SNAME *_set_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_set_)                                                            \
            return NULL;                                                       \
                                                                               \
        _set_->count = 0;                                                      \
        _set_->root = NULL;                                                    \
        _set_->flag = cmc_flags.OK;                                            \
        _set_->f_val = f_val;                                                  \
        _set_->alloc = alloc;                                                  \
        _set_->callbacks = NULL;                                               \
        _set_->it_start = PFX##_impl_it_start;                                 \
        _set_->it_end = PFX##_impl_it_end;                                     \
                                                                               \
        return _set_;                                                          \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_new_custom(struct SNAME##_fval *f_val,                 \
                                   struct cmc_alloc_node *alloc,               \
                                   struct cmc_callbacks *callbacks)            \
    {                                                                          \
        if (!f_val)                                                            \
            return NULL;                                                       \
                                                                               \
        if (!alloc)                                                            \
            alloc = &cmc_alloc_node_default;                                   \
                                                                               \
        struct SNAME *_set_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_set_)                                                            \
            return NULL;                                                       \
                                                                               \
        _set_->count = 0;                                                      \
        _set_->root = NULL;                                                    \
        _set_->flag = cmc_flags.OK;                                            \
        _set_->f_val = f_val;                                                  \
        _set_->alloc = alloc;                                                  \
        _set_->callbacks = callbacks;                                          \
        _set_->it_start = PFX##_impl_it_start;                                 \
        _set_->it_end = PFX##_impl_it_end;                                     \
                                                                               \
        return _set_;                                                          \
    }                                                                          \
                                                                               \
    void PFX##_clear(struct SNAME *_set_)                                      \
    {                                                                          \
        struct SNAME##_node *scan = _set_->root;                               \
        struct SNAME##_node *up = NULL;                                        \
                                                                               \
        while (scan != NULL)                                                   \
        {                                                                      \
            if (scan->left != NULL)                                            \
            {                                                                  \
                struct SNAME##_node *left = scan->left;                        \
                                                                               \
                scan->left = up;                                               \
                up = scan;                                                     \
                scan = left;                                                   \
            }                                                                  \
            else if (scan->right != NULL)                                      \
            {                                                                  \
                struct SNAME##_node *right = scan->right;                      \
                                                                               \
                scan->left = up;                                               \
                scan->right = NULL;                                            \
                up = scan;                                                     \
                scan = right;                                                  \
            }                                                                  \
            else                                                               \
            {                                                                  \
                if (up == NULL)                                                \
                {                                                              \
                    if (_set_->f_val->free)                                    \
                        _set_->f_val->free(scan->value);                       \
                                                                               \
                    _set_->alloc->free(scan);                                  \
                    scan = NULL;                                               \
                }                                                              \
                                                                               \
                while (up != NULL)                                             \
                {                                                              \
                    if (_set_->f_val->free)                                    \
                        _set_->f_val->free(scan->value);                       \
                                                                               \
                    _set_->alloc->free(scan);                                  \
                                                                               \
                    if (up->right != NULL)                                     \
                    {                                                          \
                        scan = up->right;                                      \
                        up->right = NULL;                                      \
                        break;                                                 \
                    }                                                          \
                    else                                                       \
                    {                                                          \
                        scan = up;                                             \
                        up = up->left;                                         \
                    }                                                          \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        _set_->count = 0;                                                      \
        _set_->root = NULL;                                                    \
        _set_->flag = cmc_flags.OK;                                            \
    }                                                                          \
                                                                               \
    void PFX##_free(struct SNAME *_set_)                                       \
    {                                                                          \
        PFX##_clear(_set_);                                                    \
                                                                               \
        _set_->alloc->free(_set_);                                             \
    }                                                                          \
                                                                               \
    void PFX##_customize(struct SNAME *_set_, struct cmc_alloc_node *alloc,    \
                         struct cmc_callbacks *callbacks)                      \
    {                                                                          \
        if (!alloc)                                                            \
            _set_->alloc = &cmc_alloc_node_default;                            \
        else                                                                   \
            _set_->alloc = alloc;                                              \
                                                                               \
        _set_->callbacks = callbacks;                                          \
                                                                               \
        _set_->flag = cmc_flags.OK;                                            \
    }                                                                          \
                                                                               \
    bool PFX##_insert(struct SNAME *_set_, V element)                          \
    {                                                                          \
        if (PFX##_empty(_set_))                                                \
        {                                                                      \
            _set_->root = PFX##_impl_new_node(_set_, element);                 \
                                                                               \
            if (!_set_->root)                                                  \
            {                                                                  \
                _set_->flag = cmc_flags.ALLOC;                                 \
                return false;                                                  \
            }                                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            struct SNAME##_node *scan = _set_->root;                           \
            struct SNAME##_node *parent = scan;                                \
                                                                               \
            while (scan != NULL)                                               \
            {                                                                  \
                parent = scan;                                                 \
                                                                               \
                if (_set_->f_val->cmp(scan->value, element) > 0)               \
                    scan = scan->left;                                         \
                else if (_set_->f_val->cmp(scan->value, element) < 0)          \
                    scan = scan->right;                                        \
                else                                                           \
                    return false;                                              \
            }                                                                  \
                                                                               \
            struct SNAME##_node *node;                                         \
                                                                               \
            if (_set_->f_val->cmp(parent->value, element) > 0)                 \
            {                                                                  \
                parent->left = PFX##_impl_new_node(_set_, element);            \
                                                                               \
                if (!parent->left)                                             \
                {                                                              \
                    _set_->flag = cmc_flags.ALLOC;                             \
                    return false;                                              \
                }                                                              \
                                                                               \
                parent->left->parent = parent;                                 \
                node = parent->left;                                           \
            }                                                                  \
            else                                                               \
            {                                                                  \
                parent->right = PFX##_impl_new_node(_set_, element);           \
                                                                               \
                if (!parent->right)                                            \
                {                                                              \
                    _set_->flag = cmc_flags.ALLOC;                             \
                    return false;                                              \
                }                                                              \
                                                                               \
                parent->right->parent = parent;                                \
                node = parent->right;                                          \
            }                                                                  \
                                                                               \
            PFX##_impl_rebalance(_set_, node);                                 \
        }                                                                      \
                                                                               \
        _set_->count++;                                                        \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->create)                      \
            _set_->callbacks->create();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_remove(struct SNAME *_set_, V element)                          \
    {                                                                          \
        if (PFX##_empty(_set_))                                                \
        {                                                                      \
            _set_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_node *node = PFX##_impl_get_node(_set_, element);       \
                                                                               \
        if (!node)                                                             \
        {                                                                      \
            _set_->flag = cmc_flags.NOT_FOUND;                                 \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_node *temp = NULL, *unbalanced = NULL;                  \
                                                                               \
        bool is_root = node->parent == NULL;                                   \
                                                                               \
        if (node->left == NULL && node->right == NULL)                         \
        {                                                                      \
            if (is_root)                                                       \
                _set_->root = NULL;                                            \
            else                                                               \
            {                                                                  \
                unbalanced = node->parent;                                     \
                                                                               \
                if (node->parent->right == node)                               \
                    node->parent->right = NULL;                                \
                else                                                           \
                    node->parent->left = NULL;                                 \
            }                                                                  \
                                                                               \
            _set_->alloc->free(node);                                          \
        }                                                                      \
        else if (node->left == NULL)                                           \
        {                                                                      \
            if (is_root)                                                       \
            {                                                                  \
                _set_->root = node->right;                                     \
                _set_->root->parent = NULL;                                    \
            }                                                                  \
            else                                                               \
            {                                                                  \
                unbalanced = node->parent;                                     \
                                                                               \
                node->right->parent = node->parent;                            \
                                                                               \
                if (node->parent->right == node)                               \
                    node->parent->right = node->right;                         \
                else                                                           \
                    node->parent->left = node->right;                          \
            }                                                                  \
                                                                               \
            _set_->alloc->free(node);                                          \
        }                                                                      \
        else if (node->right == NULL)                                          \
        {                                                                      \
            if (is_root)                                                       \
            {                                                                  \
                _set_->root = node->left;                                      \
                _set_->root->parent = NULL;                                    \
            }                                                                  \
            else                                                               \
            {                                                                  \
                unbalanced = node->parent;                                     \
                                                                               \
                node->left->parent = node->parent;                             \
                                                                               \
                if (node->parent->right == node)                               \
                    node->parent->right = node->left;                          \
                else                                                           \
                    node->parent->left = node->left;                           \
            }                                                                  \
                                                                               \
            _set_->alloc->free(node);                                          \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            temp = node->right;                                                \
            while (temp->left != NULL)                                         \
                temp = temp->left;                                             \
                                                                               \
            V temp_value = temp->value;                                        \
                                                                               \
            unbalanced = temp->parent;                                         \
                                                                               \
            if (temp->left == NULL && temp->right == NULL)                     \
            {                                                                  \
                if (temp->parent->right == temp)                               \
                    temp->parent->right = NULL;                                \
                else                                                           \
                    temp->parent->left = NULL;                                 \
            }                                                                  \
            else if (temp->left == NULL)                                       \
            {                                                                  \
                temp->right->parent = temp->parent;                            \
                                                                               \
                if (temp->parent->right == temp)                               \
                    temp->parent->right = temp->right;                         \
                else                                                           \
                    temp->parent->left = temp->right;                          \
            }                                                                  \
            else if (temp->right == NULL)                                      \
            {                                                                  \
                temp->left->parent = temp->parent;                             \
                                                                               \
                if (temp->parent->right == temp)                               \
                    temp->parent->right = temp->left;                          \
                else                                                           \
                    temp->parent->left = temp->left;                           \
            }                                                                  \
                                                                               \
            _set_->alloc->free(temp);                                          \
                                                                               \
            node->value = temp_value;                                          \
        }                                                                      \
                                                                               \
        if (unbalanced != NULL)                                                \
            PFX##_impl_rebalance(_set_, unbalanced);                           \
                                                                               \
        if (_set_->count == 0)                                                 \
            _set_->root = NULL;                                                \
                                                                               \
        _set_->count--;                                                        \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->delete)                      \
            _set_->callbacks->delete ();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_max(struct SNAME *_set_, V *value)                              \
    {                                                                          \
        if (PFX##_empty(_set_))                                                \
        {                                                                      \
            _set_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_node *scan = _set_->root;                               \
                                                                               \
        while (scan->right != NULL)                                            \
            scan = scan->right;                                                \
                                                                               \
        if (value)                                                             \
            *value = scan->value;                                              \
                                                                               \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->read)                        \
            _set_->callbacks->read();                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_min(struct SNAME *_set_, V *value)                              \
    {                                                                          \
        if (PFX##_empty(_set_))                                                \
        {                                                                      \
            _set_->flag = cmc_flags.EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct SNAME##_node *scan = _set_->root;                               \
                                                                               \
        while (scan->left != NULL)                                             \
            scan = scan->left;                                                 \
                                                                               \
        if (value)                                                             \
            *value = scan->value;                                              \
                                                                               \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->read)                        \
            _set_->callbacks->read();                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool PFX##_contains(struct SNAME *_set_, V element)                        \
    {                                                                          \
        bool result = PFX##_impl_get_node(_set_, element) != NULL;             \
                                                                               \
        if (_set_->callbacks && _set_->callbacks->read)                        \
            _set_->callbacks->read();                                          \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_empty(struct SNAME *_set_)                                      \
    {                                                                          \
        return _set_->count == 0;                                              \
    }                                                                          \
                                                                               \
    size_t PFX##_count(struct SNAME *_set_)                                    \
    {                                                                          \
        return _set_->count;                                                   \
    }                                                                          \
                                                                               \
    int PFX##_flag(struct SNAME *_set_)                                        \
    {                                                                          \
        return _set_->flag;                                                    \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_copy_of(struct SNAME *_set_)                           \
    {                                                                          \
        struct SNAME *result =                                                 \
            PFX##_new_custom(_set_->f_val, _set_->alloc, _set_->callbacks);    \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _set_->flag = cmc_flags.ERROR;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct SNAME##_iter iter;                                              \
        PFX##_iter_init(&iter, _set_);                                         \
                                                                               \
        if (!PFX##_empty(_set_))                                               \
        {                                                                      \
            for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter);           \
                 PFX##_iter_next(&iter))                                       \
            {                                                                  \
                if (_set_->f_val->cpy)                                         \
                    PFX##_insert(result,                                       \
                                 _set_->f_val->cpy(PFX##_iter_value(&iter)));  \
                else                                                           \
                    PFX##_insert(result, PFX##_iter_value(&iter));             \
            }                                                                  \
        }                                                                      \
                                                                               \
        _set_->flag = cmc_flags.OK;                                            \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool PFX##_equals(struct SNAME *_set1_, struct SNAME *_set2_)              \
    {                                                                          \
        _set1_->flag = cmc_flags.OK;                                           \
        _set2_->flag = cmc_flags.OK;                                           \
                                                                               \
        if (_set1_->count != _set2_->count)                                    \
            return false;                                                      \
                                                                               \
        struct SNAME##_iter iter;                                              \
        PFX##_iter_init(&iter, _set1_);                                        \
                                                                               \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter);               \
             PFX##_iter_next(&iter))                                           \
        {                                                                      \
            if (PFX##_impl_get_node(_set2_, PFX##_iter_value(&iter)) == NULL)  \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct cmc_string PFX##_to_string(struct SNAME *_set_)                     \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *s_ = _set_;                                              \
                                                                               \
        int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_treeset,        \
                         #SNAME, #V, s_, s_->root, s_->count, s_->flag,        \
                         s_->f_val, s_->alloc, s_->callbacks);                 \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    bool PFX##_print(struct SNAME *_set_, FILE *fptr)                          \
    {                                                                          \
        struct SNAME##_node *root = _set_->root;                               \
                                                                               \
        bool left_done = false;                                                \
                                                                               \
        while (root)                                                           \
        {                                                                      \
            if (!left_done)                                                    \
            {                                                                  \
                while (root->left)                                             \
                    root = root->left;                                         \
            }                                                                  \
                                                                               \
            if (!_set_->f_val->str(fptr, root->value))                         \
                return false;                                                  \
                                                                               \
            left_done = true;                                                  \
                                                                               \
            if (root->right)                                                   \
            {                                                                  \
                left_done = false;                                             \
                root = root->right;                                            \
            }                                                                  \
            else if (root->parent)                                             \
            {                                                                  \
                while (root->parent && root == root->parent->right)            \
                    root = root->parent;                                       \
                                                                               \
                if (!root->parent)                                             \
                    break;                                                     \
                                                                               \
                root = root->parent;                                           \
            }                                                                  \
            else                                                               \
                break;                                                         \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_union(struct SNAME *_set1_, struct SNAME *_set2_)      \
    {                                                                          \
        struct SNAME *_set_r_ =                                                \
            PFX##_new_custom(_set1_->f_val, _set1_->alloc, _set1_->callbacks); \
                                                                               \
        if (!_set_r_)                                                          \
            return NULL;                                                       \
                                                                               \
        struct SNAME##_iter iter1, iter2;                                      \
        PFX##_iter_init(&iter1, _set1_);                                       \
        PFX##_iter_init(&iter2, _set2_);                                       \
                                                                               \
        for (PFX##_iter_to_start(&iter1); !PFX##_iter_end(&iter1);             \
             PFX##_iter_next(&iter1))                                          \
        {                                                                      \
            PFX##_insert(_set_r_, PFX##_iter_value(&iter1));                   \
        }                                                                      \
                                                                               \
        for (PFX##_iter_to_start(&iter2); !PFX##_iter_end(&iter2);             \
             PFX##_iter_next(&iter2))                                          \
        {                                                                      \
            PFX##_insert(_set_r_, PFX##_iter_value(&iter2));                   \
        }                                                                      \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_intersection(struct SNAME *_set1_,                     \
                                     struct SNAME *_set2_)                     \
    {                                                                          \
        struct SNAME *_set_r_ =                                                \
            PFX##_new_custom(_set1_->f_val, _set1_->alloc, _set1_->callbacks); \
                                                                               \
        if (!_set_r_)                                                          \
            return NULL;                                                       \
                                                                               \
        struct SNAME *_set_A_ =                                                \
            _set1_->count < _set2_->count ? _set1_ : _set2_;                   \
        struct SNAME *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;           \
                                                                               \
        struct SNAME##_iter iter;                                              \
        PFX##_iter_init(&iter, _set_A_);                                       \
                                                                               \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter);               \
             PFX##_iter_next(&iter))                                           \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            if (PFX##_impl_get_node(_set_B_, value) != NULL)                   \
                PFX##_insert(_set_r_, value);                                  \
        }                                                                      \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_difference(struct SNAME *_set1_, struct SNAME *_set2_) \
    {                                                                          \
        struct SNAME *_set_r_ =                                                \
            PFX##_new_custom(_set1_->f_val, _set1_->alloc, _set1_->callbacks); \
                                                                               \
        if (!_set_r_)                                                          \
            return NULL;                                                       \
                                                                               \
        struct SNAME##_iter iter;                                              \
        PFX##_iter_init(&iter, _set1_);                                        \
                                                                               \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter);               \
             PFX##_iter_next(&iter))                                           \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            if (PFX##_impl_get_node(_set2_, value) == NULL)                    \
                PFX##_insert(_set_r_, value);                                  \
        }                                                                      \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    struct SNAME *PFX##_symmetric_difference(struct SNAME *_set1_,             \
                                             struct SNAME *_set2_)             \
    {                                                                          \
        struct SNAME *_set_r_ =                                                \
            PFX##_new_custom(_set1_->f_val, _set1_->alloc, _set1_->callbacks); \
                                                                               \
        if (!_set_r_)                                                          \
            return NULL;                                                       \
                                                                               \
        struct SNAME##_iter iter1, iter2;                                      \
        PFX##_iter_init(&iter1, _set1_);                                       \
        PFX##_iter_init(&iter2, _set2_);                                       \
                                                                               \
        for (PFX##_iter_to_start(&iter1); !PFX##_iter_end(&iter1);             \
             PFX##_iter_next(&iter1))                                          \
        {                                                                      \
            V value = PFX##_iter_value(&iter1);                                \
                                                                               \
            if (PFX##_impl_get_node(_set2_, value) == NULL)                    \
                PFX##_insert(_set_r_, value);                                  \
        }                                                                      \
                                                                               \
        for (PFX##_iter_to_start(&iter2); !PFX##_iter_end(&iter2);             \
             PFX##_iter_next(&iter2))                                          \
        {                                                                      \
            V value = PFX##_iter_value(&iter2);                                \
                                                                               \
            if (PFX##_impl_get_node(_set1_, value) == NULL)                    \
                PFX##_insert(_set_r_, value);                                  \
        }                                                                      \
                                                                               \
        return _set_r_;                                                        \
    }                                                                          \
                                                                               \
    /* Is _set1_ a subset of _set2_ ? */                                       \
    /* A set X is a subset of a set Y when: X <= Y */                          \
    /* If X is a subset of Y, then Y is a superset of X */                     \
    bool PFX##_is_subset(struct SNAME *_set1_, struct SNAME *_set2_)           \
    {                                                                          \
        /* If the cardinality of _set1_ is greater than that of _set2_ */      \
        /* then it is safe to say that _set1_ can't be a subset of _set2_ */   \
        if (PFX##_count(_set1_) > PFX##_count(_set2_))                         \
            return false;                                                      \
                                                                               \
        /* The empty set is a subset of all sets */                            \
        if (PFX##_empty(_set1_))                                               \
            return true;                                                       \
                                                                               \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _set1_);                                        \
                                                                               \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter);               \
             PFX##_iter_next(&iter))                                           \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            if (PFX##_impl_get_node(_set2_, value) == NULL)                    \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Is _set1_ a superset of _set2_ ? */                                     \
    /* A set X is a superset of a set Y when: X >= Y */                        \
    /* If X is a superset of Y, then Y is a subset of X */                     \
    bool PFX##_is_superset(struct SNAME *_set1_, struct SNAME *_set2_)         \
    {                                                                          \
        return PFX##_is_subset(_set2_, _set1_);                                \
    }                                                                          \
                                                                               \
    /* Is _set1_ a proper subset of _set2_ ? */                                \
    /* A set X is a proper subset of a set Y when: X < Y */                    \
    /* If X is a proper subset of Y, then Y is a proper superset of X */       \
    bool PFX##_is_proper_subset(struct SNAME *_set1_, struct SNAME *_set2_)    \
    {                                                                          \
        /* If the cardinality of _set1_ is greater than or equal to that */    \
        /* of _set2_, then it is safe to say that _set1_ can't be a proper */  \
        /* subset of _set2_ */                                                 \
        if (PFX##_count(_set1_) >= PFX##_count(_set2_))                        \
            return false;                                                      \
                                                                               \
        if (PFX##_empty(_set1_))                                               \
        {                                                                      \
            /* The empty set is a proper subset of all non-empty sets */       \
            if (!PFX##_empty(_set2_))                                          \
                return true;                                                   \
            /* The empty set is not a proper subset of itself (this is true */ \
            /* for any set) */                                                 \
            else                                                               \
                return false;                                                  \
        }                                                                      \
                                                                               \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _set1_);                                        \
                                                                               \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter);               \
             PFX##_iter_next(&iter))                                           \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            if (PFX##_impl_get_node(_set2_, value) == NULL)                    \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Is _set1_ a proper superset of _set2_ ? */                              \
    /* A set X is a proper superset of a set Y when: X > Y */                  \
    /* If X is a proper superset of Y, then Y is a proper subset of X */       \
    bool PFX##_is_proper_superset(struct SNAME *_set1_, struct SNAME *_set2_)  \
    {                                                                          \
        return PFX##_is_proper_subset(_set2_, _set1_);                         \
    }                                                                          \
                                                                               \
    /* Is _set1_ a disjointset of _set2_ ? */                                  \
    /* A set X is a disjointset of a set Y if their intersection is empty, */  \
    /* that is, if there are no elements in common between the two */          \
    bool PFX##_is_disjointset(struct SNAME *_set1_, struct SNAME *_set2_)      \
    {                                                                          \
        /* The intersection of an empty set with any other set will result */  \
        /* in an empty set */                                                  \
        if (PFX##_empty(_set1_))                                               \
            return true;                                                       \
                                                                               \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _set1_);                                        \
                                                                               \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter);               \
             PFX##_iter_next(&iter))                                           \
        {                                                                      \
            V value = PFX##_iter_value(&iter);                                 \
                                                                               \
            if (PFX##_impl_get_node(_set2_, value) != NULL)                    \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target)                  \
    {                                                                          \
        struct SNAME##_iter *iter =                                            \
            target->alloc->malloc(sizeof(struct SNAME##_iter));                \
                                                                               \
        if (!iter)                                                             \
            return NULL;                                                       \
                                                                               \
        PFX##_iter_init(iter, target);                                         \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    void PFX##_iter_free(struct SNAME##_iter *iter)                            \
    {                                                                          \
        iter->target->alloc->free(iter);                                       \
    }                                                                          \
                                                                               \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target)      \
    {                                                                          \
        memset(iter, 0, sizeof(struct SNAME##_iter));                          \
                                                                               \
        iter->target = target;                                                 \
        iter->start = true;                                                    \
        iter->end = PFX##_empty(target);                                       \
                                                                               \
        iter->cursor = target->root;                                           \
                                                                               \
        if (!PFX##_empty(target))                                              \
        {                                                                      \
            while (iter->cursor->left != NULL)                                 \
                iter->cursor = iter->cursor->left;                             \
                                                                               \
            iter->first = iter->cursor;                                        \
                                                                               \
            iter->last = target->root;                                         \
            while (iter->last->right != NULL)                                  \
                iter->last = iter->last->right;                                \
        }                                                                      \
    }                                                                          \
                                                                               \
    bool PFX##_iter_start(struct SNAME##_iter *iter)                           \
    {                                                                          \
        return PFX##_empty(iter->target) || iter->start;                       \
    }                                                                          \
                                                                               \
    bool PFX##_iter_end(struct SNAME##_iter *iter)                             \
    {                                                                          \
        return PFX##_empty(iter->target) || iter->end;                         \
    }                                                                          \
                                                                               \
    void PFX##_iter_to_start(struct SNAME##_iter *iter)                        \
    {                                                                          \
        if (!PFX##_empty(iter->target))                                        \
        {                                                                      \
            iter->index = 0;                                                   \
            iter->start = true;                                                \
            iter->end = PFX##_empty(iter->target);                             \
            iter->cursor = iter->first;                                        \
        }                                                                      \
    }                                                                          \
                                                                               \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                          \
    {                                                                          \
        if (!PFX##_empty(iter->target))                                        \
        {                                                                      \
            iter->index = iter->target->count - 1;                             \
            iter->start = PFX##_empty(iter->target);                           \
            iter->end = true;                                                  \
            iter->cursor = iter->last;                                         \
        }                                                                      \
    }                                                                          \
                                                                               \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                            \
    {                                                                          \
        if (iter->end)                                                         \
            return false;                                                      \
                                                                               \
        if (iter->cursor == iter->last)                                        \
        {                                                                      \
            iter->end = true;                                                  \
            return true;                                                       \
        }                                                                      \
                                                                               \
        iter->start = PFX##_empty(iter->target);                               \
                                                                               \
        if (iter->cursor->right != NULL)                                       \
        {                                                                      \
            iter->cursor = iter->cursor->right;                                \
                                                                               \
            while (iter->cursor->left != NULL)                                 \
                iter->cursor = iter->cursor->left;                             \
                                                                               \
            iter->index++;                                                     \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        while (true)                                                           \
        {                                                                      \
            if (iter->cursor->parent->left == iter->cursor)                    \
            {                                                                  \
                iter->cursor = iter->cursor->parent;                           \
                                                                               \
                iter->index++;                                                 \
                                                                               \
                return true;                                                   \
            }                                                                  \
                                                                               \
            iter->cursor = iter->cursor->parent;                               \
        }                                                                      \
    }                                                                          \
                                                                               \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                            \
    {                                                                          \
        if (iter->start)                                                       \
            return false;                                                      \
                                                                               \
        if (iter->cursor == iter->first)                                       \
        {                                                                      \
            iter->start = true;                                                \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        iter->end = PFX##_empty(iter->target);                                 \
                                                                               \
        if (iter->cursor->left != NULL)                                        \
        {                                                                      \
            iter->cursor = iter->cursor->left;                                 \
                                                                               \
            while (iter->cursor->right != NULL)                                \
                iter->cursor = iter->cursor->right;                            \
                                                                               \
            iter->index--;                                                     \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        while (true)                                                           \
        {                                                                      \
            if (iter->cursor->parent->right == iter->cursor)                   \
            {                                                                  \
                iter->cursor = iter->cursor->parent;                           \
                                                                               \
                iter->index--;                                                 \
                                                                               \
                return true;                                                   \
            }                                                                  \
                                                                               \
            iter->cursor = iter->cursor->parent;                               \
        }                                                                      \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)           \
    {                                                                          \
        if (iter->end)                                                         \
            return false;                                                      \
                                                                               \
        if (iter->cursor == iter->last)                                        \
        {                                                                      \
            iter->end = true;                                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (steps == 0 || iter->index + steps >= PFX##_count(iter->target))    \
            return false;                                                      \
                                                                               \
        iter->index += steps;                                                  \
                                                                               \
        for (size_t i = 0; i < steps; i++)                                     \
            PFX##_iter_next(iter);                                             \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps)            \
    {                                                                          \
        if (iter->start)                                                       \
            return false;                                                      \
                                                                               \
        if (iter->cursor == iter->first)                                       \
        {                                                                      \
            iter->start = true;                                                \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (steps == 0 || iter->index < steps)                                 \
            return false;                                                      \
                                                                               \
        iter->index -= steps;                                                  \
                                                                               \
        for (size_t i = 0; i < steps; i++)                                     \
            PFX##_iter_prev(iter);                                             \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator was able to be positioned at the */   \
    /* given index */                                                          \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index)             \
    {                                                                          \
        if (index >= PFX##_count(iter->target))                                \
            return false;                                                      \
                                                                               \
        if (iter->index > index)                                               \
            return PFX##_iter_rewind(iter, iter->index - index);               \
        else if (iter->index < index)                                          \
            return PFX##_iter_advance(iter, index - iter->index);              \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    V PFX##_iter_value(struct SNAME##_iter *iter)                              \
    {                                                                          \
        if (PFX##_empty(iter->target))                                         \
            return (V){ 0 };                                                   \
                                                                               \
        return iter->cursor->value;                                            \
    }                                                                          \
                                                                               \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                         \
    {                                                                          \
        return iter->index;                                                    \
    }                                                                          \
                                                                               \
    static struct SNAME##_node *PFX##_impl_new_node(struct SNAME *_set_,       \
                                                    V element)                 \
    {                                                                          \
        struct SNAME##_node *node =                                            \
            _set_->alloc->malloc(sizeof(struct SNAME##_node));                 \
                                                                               \
        if (!node)                                                             \
            return NULL;                                                       \
                                                                               \
        node->value = element;                                                 \
        node->right = NULL;                                                    \
        node->left = NULL;                                                     \
        node->parent = NULL;                                                   \
        node->height = 0;                                                      \
                                                                               \
        return node;                                                           \
    }                                                                          \
                                                                               \
    static struct SNAME##_node *PFX##_impl_get_node(struct SNAME *_set_,       \
                                                    V element)                 \
    {                                                                          \
        struct SNAME##_node *scan = _set_->root;                               \
                                                                               \
        while (scan != NULL)                                                   \
        {                                                                      \
            if (_set_->f_val->cmp(scan->value, element) > 0)                   \
                scan = scan->left;                                             \
            else if (_set_->f_val->cmp(scan->value, element) < 0)              \
                scan = scan->right;                                            \
            else                                                               \
                return scan;                                                   \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    static unsigned char PFX##_impl_h(struct SNAME##_node *node)               \
    {                                                                          \
        if (node == NULL)                                                      \
            return 0;                                                          \
                                                                               \
        return node->height;                                                   \
    }                                                                          \
                                                                               \
    static unsigned char PFX##_impl_hupdate(struct SNAME##_node *node)         \
    {                                                                          \
        if (node == NULL)                                                      \
            return 0;                                                          \
                                                                               \
        unsigned char h_l = PFX##_impl_h(node->left);                          \
        unsigned char h_r = PFX##_impl_h(node->right);                         \
                                                                               \
        return 1 + (h_l > h_r ? h_l : h_r);                                    \
    }                                                                          \
                                                                               \
    static void PFX##_impl_rotate_right(struct SNAME##_node **Z)               \
    {                                                                          \
        struct SNAME##_node *root = *Z;                                        \
        struct SNAME##_node *new_root = root->left;                            \
                                                                               \
        if (root->parent != NULL)                                              \
        {                                                                      \
            if (root->parent->left == root)                                    \
                root->parent->left = new_root;                                 \
            else                                                               \
                root->parent->right = new_root;                                \
        }                                                                      \
                                                                               \
        new_root->parent = root->parent;                                       \
                                                                               \
        root->parent = new_root;                                               \
        root->left = new_root->right;                                          \
                                                                               \
        if (root->left)                                                        \
            root->left->parent = root;                                         \
                                                                               \
        new_root->right = root;                                                \
                                                                               \
        root->height = PFX##_impl_hupdate(root);                               \
        new_root->height = PFX##_impl_hupdate(new_root);                       \
                                                                               \
        *Z = new_root;                                                         \
    }                                                                          \
                                                                               \
    static void PFX##_impl_rotate_left(struct SNAME##_node **Z)                \
    {                                                                          \
        struct SNAME##_node *root = *Z;                                        \
        struct SNAME##_node *new_root = root->right;                           \
                                                                               \
        if (root->parent != NULL)                                              \
        {                                                                      \
            if (root->parent->right == root)                                   \
                root->parent->right = new_root;                                \
            else                                                               \
                root->parent->left = new_root;                                 \
        }                                                                      \
                                                                               \
        new_root->parent = root->parent;                                       \
                                                                               \
        root->parent = new_root;                                               \
        root->right = new_root->left;                                          \
                                                                               \
        if (root->right)                                                       \
            root->right->parent = root;                                        \
                                                                               \
        new_root->left = root;                                                 \
                                                                               \
        root->height = PFX##_impl_hupdate(root);                               \
        new_root->height = PFX##_impl_hupdate(new_root);                       \
                                                                               \
        *Z = new_root;                                                         \
    }                                                                          \
                                                                               \
    static void PFX##_impl_rebalance(struct SNAME *_set_,                      \
                                     struct SNAME##_node *node)                \
    {                                                                          \
        struct SNAME##_node *scan = node, *child = NULL;                       \
                                                                               \
        int balance;                                                           \
        bool is_root = false;                                                  \
                                                                               \
        while (scan != NULL)                                                   \
        {                                                                      \
            if (scan->parent == NULL)                                          \
                is_root = true;                                                \
                                                                               \
            scan->height = PFX##_impl_hupdate(scan);                           \
            balance = PFX##_impl_h(scan->right) - PFX##_impl_h(scan->left);    \
                                                                               \
            if (balance >= 2)                                                  \
            {                                                                  \
                child = scan->right;                                           \
                                                                               \
                if (PFX##_impl_h(child->right) < PFX##_impl_h(child->left))    \
                    PFX##_impl_rotate_right(&(scan->right));                   \
                                                                               \
                PFX##_impl_rotate_left(&scan);                                 \
            }                                                                  \
            else if (balance <= -2)                                            \
            {                                                                  \
                child = scan->left;                                            \
                                                                               \
                if (PFX##_impl_h(child->left) < PFX##_impl_h(child->right))    \
                    PFX##_impl_rotate_left(&(scan->left));                     \
                                                                               \
                PFX##_impl_rotate_right(&scan);                                \
            }                                                                  \
                                                                               \
            if (is_root)                                                       \
            {                                                                  \
                _set_->root = scan;                                            \
                is_root = false;                                               \
            }                                                                  \
                                                                               \
            scan = scan->parent;                                               \
        }                                                                      \
    }                                                                          \
                                                                               \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_set_)        \
    {                                                                          \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _set_);                                         \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_set_)          \
    {                                                                          \
        struct SNAME##_iter iter;                                              \
                                                                               \
        PFX##_iter_init(&iter, _set_);                                         \
        PFX##_iter_to_end(&iter);                                              \
                                                                               \
        return iter;                                                           \
    }

#endif /* CMC_TREESET_H */
