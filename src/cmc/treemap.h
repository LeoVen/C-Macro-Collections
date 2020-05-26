/**
 * treemap.h
 *
 * Creation Date: 28/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * TreeMap
 *
 * A TreeMap is an implementation of a Map that keeps its keys sorted. Like a
 * Map, it has only unique keys. This implementation uses a balanced binary
 * tree called AVL Tree that uses the height of nodes to keep its keys balanced.
 */

#ifndef CMC_CMC_TREEMAP_H
#define CMC_CMC_TREEMAP_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * TreeMap specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_cmc_string_fmt_treemap = "struct %s<%s, %s> "
                                                "at %p { "
                                                "root:%p, "
                                                "count:%" PRIuMAX ", "
                                                "flag:%d, "
                                                "f_val:%p, "
                                                "f_key:%p, "
                                                "alloc:%p, "
                                                "callbacks:%p }";

/**
 * Core TreeMap implementation
 */
#define CMC_CMC_TREEMAP_CORE(BODY)    \
    CMC_CMC_TREEMAP_CORE_HEADER(BODY) \
    CMC_CMC_TREEMAP_CORE_SOURCE(BODY)

#define CMC_CMC_TREEMAP_CORE_HEADER(BODY)                                    \
    CMC_CMC_TREEMAP_CORE_HEADER_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                                 CMC_PARAM_K(BODY), CMC_PARAM_V(BODY))

#define CMC_CMC_TREEMAP_CORE_SOURCE(BODY)                                    \
    CMC_CMC_TREEMAP_CORE_SOURCE_(CMC_PARAM_PFX(BODY), CMC_PARAM_SNAME(BODY), \
                                 CMC_PARAM_K(BODY), CMC_PARAM_V(BODY))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_TREEMAP_CORE_HEADER_(PFX, SNAME, K, V)                        \
                                                                              \
    /* Treemap Structure */                                                   \
    struct SNAME                                                              \
    {                                                                         \
        /* Root node */                                                       \
        struct CMC_DEF_NODE(SNAME) * root;                                    \
                                                                              \
        /* Current amount of keys */                                          \
        size_t count;                                                         \
                                                                              \
        /* Flags indicating errors or success */                              \
        int flag;                                                             \
                                                                              \
        /* Key function table */                                              \
        struct CMC_DEF_FKEY(SNAME) * f_key;                                   \
                                                                              \
        /* Value function table */                                            \
        struct CMC_DEF_FVAL(SNAME) * f_val;                                   \
                                                                              \
        /* Custom allocation functions */                                     \
        struct cmc_alloc_node *alloc;                                         \
                                                                              \
        /* Custom callback functions */                                       \
        struct cmc_callbacks *callbacks;                                      \
    };                                                                        \
                                                                              \
    /* Treemap Node */                                                        \
    struct CMC_DEF_NODE(SNAME)                                                \
    {                                                                         \
        /* Node Key */                                                        \
        K key;                                                                \
                                                                              \
        /* Node Value */                                                      \
        V value;                                                              \
                                                                              \
        /* Node height used by the AVL tree to keep it strictly balanced */   \
        unsigned char height;                                                 \
                                                                              \
        /* Right child node or subtree */                                     \
        struct CMC_DEF_NODE(SNAME) * right;                                   \
                                                                              \
        /* Left child node or subtree */                                      \
        struct CMC_DEF_NODE(SNAME) * left;                                    \
                                                                              \
        /* Parent node */                                                     \
        struct CMC_DEF_NODE(SNAME) * parent;                                  \
    };                                                                        \
                                                                              \
    /* Key struct function table */                                           \
    struct CMC_DEF_FKEY(SNAME)                                                \
    {                                                                         \
        /* Comparator function */                                             \
        int (*cmp)(K, K);                                                     \
                                                                              \
        /* Copy function */                                                   \
        K (*cpy)(K);                                                          \
                                                                              \
        /* To string function */                                              \
        bool (*str)(FILE *, K);                                               \
                                                                              \
        /* Free from memory function */                                       \
        void (*free)(K);                                                      \
                                                                              \
        /* Hash function */                                                   \
        size_t (*hash)(K);                                                    \
                                                                              \
        /* Priority function */                                               \
        int (*pri)(K, K);                                                     \
    };                                                                        \
                                                                              \
    /* Value struct function table */                                         \
    struct CMC_DEF_FVAL(SNAME)                                                \
    {                                                                         \
        /* Comparator function */                                             \
        int (*cmp)(V, V);                                                     \
                                                                              \
        /* Copy function */                                                   \
        V (*cpy)(V);                                                          \
                                                                              \
        /* To string function */                                              \
        bool (*str)(FILE *, V);                                               \
                                                                              \
        /* Free from memory function */                                       \
        void (*free)(V);                                                      \
                                                                              \
        /* Hash function */                                                   \
        size_t (*hash)(V);                                                    \
                                                                              \
        /* Priority function */                                               \
        int (*pri)(V, V);                                                     \
    };                                                                        \
                                                                              \
    /* Treemap Iterator */                                                    \
    struct CMC_DEF_ITER(SNAME)                                                \
    {                                                                         \
        /* Target treemap */                                                  \
        struct SNAME *target;                                                 \
                                                                              \
        /* Cursor's current node */                                           \
        struct CMC_DEF_NODE(SNAME) * cursor;                                  \
                                                                              \
        /* The first node in the iteration */                                 \
        struct CMC_DEF_NODE(SNAME) * first;                                   \
                                                                              \
        /* The last node in the iteration */                                  \
        struct CMC_DEF_NODE(SNAME) * last;                                    \
                                                                              \
        /* Keeps track of relative index to the iteration of elements */      \
        size_t index;                                                         \
                                                                              \
        /* If the iterator has reached the start of the iteration */          \
        bool start;                                                           \
                                                                              \
        /* If the iterator has reached the end of the iteration */            \
        bool end;                                                             \
    };                                                                        \
                                                                              \
    /* Collection Functions */                                                \
    /* Collection Allocation and Deallocation */                              \
    struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FKEY(SNAME) * f_key,         \
                                  struct CMC_DEF_FVAL(SNAME) * f_val);        \
    struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FKEY(SNAME) * f_key,  \
                                         struct CMC_DEF_FVAL(SNAME) * f_val,  \
                                         struct cmc_alloc_node * alloc,       \
                                         struct cmc_callbacks * callbacks);   \
    void CMC_(PFX, _clear)(struct SNAME * _map_);                             \
    void CMC_(PFX, _free)(struct SNAME * _map_);                              \
    /* Customization of Allocation and Callbacks */                           \
    void CMC_(PFX, _customize)(struct SNAME * _map_,                          \
                               struct cmc_alloc_node * alloc,                 \
                               struct cmc_callbacks * callbacks);             \
    /* Collection Input and Output */                                         \
    bool CMC_(PFX, _insert)(struct SNAME * _map_, K key, V value);            \
    bool CMC_(PFX, _update)(struct SNAME * _map_, K key, V new_value,         \
                            V * old_value);                                   \
    bool CMC_(PFX, _remove)(struct SNAME * _map_, K key, V * out_value);      \
    /* Element Access */                                                      \
    bool CMC_(PFX, _max)(struct SNAME * _map_, K * key, V * value);           \
    bool CMC_(PFX, _min)(struct SNAME * _map_, K * key, V * value);           \
    V CMC_(PFX, _get)(struct SNAME * _map_, K key);                           \
    V *CMC_(PFX, _get_ref)(struct SNAME * _map_, K key);                      \
    /* Collection State */                                                    \
    bool CMC_(PFX, _contains)(struct SNAME * _map_, K key);                   \
    bool CMC_(PFX, _empty)(struct SNAME * _map_);                             \
    size_t CMC_(PFX, _count)(struct SNAME * _map_);                           \
    int CMC_(PFX, _flag)(struct SNAME * _map_);                               \
    /* Collection Utility */                                                  \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _map_);                  \
    bool CMC_(PFX, _equals)(struct SNAME * _map1_, struct SNAME * _map2_);    \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _map_);            \
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr);                \
                                                                              \
    /* Iterator Functions */                                                  \
    /* Iterator Initialization */                                             \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target); \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target);   \
    /* Iterator State */                                                      \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter);        \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter);          \
    /* Iterator Movement */                                                   \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter);        \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter);          \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter);            \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter);            \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,          \
                                  size_t steps);                              \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                 size_t steps);                               \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter,            \
                                size_t index);                                \
    /* Iterator Access */                                                     \
    K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter);                \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter);              \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter);            \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_TREEMAP_CORE_SOURCE_(PFX, SNAME, K, V)                         \
                                                                               \
    /* Implementation Detail Functions */                                      \
    static struct CMC_DEF_NODE(SNAME) *                                        \
        CMC_(PFX, _impl_new_node)(struct SNAME * _map_, K key, V value);       \
    static struct CMC_DEF_NODE(SNAME) *                                        \
        CMC_(PFX, _impl_get_node)(struct SNAME * _map_, K key);                \
    static unsigned char CMC_(PFX,                                             \
                              _impl_h)(struct CMC_DEF_NODE(SNAME) * node);     \
    static unsigned char CMC_(PFX, _impl_hupdate)(struct CMC_DEF_NODE(SNAME) * \
                                                  node);                       \
    static void CMC_(PFX,                                                      \
                     _impl_rotate_right)(struct CMC_DEF_NODE(SNAME) * *Z);     \
    static void CMC_(PFX, _impl_rotate_left)(struct CMC_DEF_NODE(SNAME) * *Z); \
    static void CMC_(PFX, _impl_rebalance)(struct SNAME * _map_,               \
                                           struct CMC_DEF_NODE(SNAME) * node); \
                                                                               \
    struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FKEY(SNAME) * f_key,          \
                                  struct CMC_DEF_FVAL(SNAME) * f_val)          \
    {                                                                          \
        if (!f_key || !f_val)                                                  \
            return NULL;                                                       \
                                                                               \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;                \
                                                                               \
        struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_map_)                                                            \
            return NULL;                                                       \
                                                                               \
        _map_->count = 0;                                                      \
        _map_->root = NULL;                                                    \
        _map_->flag = CMC_FLAG_OK;                                             \
        _map_->f_key = f_key;                                                  \
        _map_->f_val = f_val;                                                  \
        _map_->alloc = alloc;                                                  \
        _map_->callbacks = NULL;                                               \
                                                                               \
        return _map_;                                                          \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FKEY(SNAME) * f_key,   \
                                         struct CMC_DEF_FVAL(SNAME) * f_val,   \
                                         struct cmc_alloc_node * alloc,        \
                                         struct cmc_callbacks * callbacks)     \
    {                                                                          \
        if (!f_key || !f_val)                                                  \
            return NULL;                                                       \
                                                                               \
        if (!alloc)                                                            \
            alloc = &cmc_alloc_node_default;                                   \
                                                                               \
        struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME));             \
                                                                               \
        if (!_map_)                                                            \
            return NULL;                                                       \
                                                                               \
        _map_->count = 0;                                                      \
        _map_->root = NULL;                                                    \
        _map_->flag = CMC_FLAG_OK;                                             \
        _map_->f_key = f_key;                                                  \
        _map_->f_val = f_val;                                                  \
        _map_->alloc = alloc;                                                  \
        _map_->callbacks = callbacks;                                          \
                                                                               \
        return _map_;                                                          \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _clear)(struct SNAME * _map_)                               \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *scan = _map_->root;                        \
        struct CMC_DEF_NODE(SNAME) *up = NULL;                                 \
                                                                               \
        while (scan != NULL)                                                   \
        {                                                                      \
            if (scan->left != NULL)                                            \
            {                                                                  \
                struct CMC_DEF_NODE(SNAME) *left = scan->left;                 \
                                                                               \
                scan->left = up;                                               \
                up = scan;                                                     \
                scan = left;                                                   \
            }                                                                  \
            else if (scan->right != NULL)                                      \
            {                                                                  \
                struct CMC_DEF_NODE(SNAME) *right = scan->right;               \
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
                    if (_map_->f_key->free)                                    \
                        _map_->f_key->free(scan->key);                         \
                    if (_map_->f_val->free)                                    \
                        _map_->f_val->free(scan->value);                       \
                                                                               \
                    _map_->alloc->free(scan);                                  \
                    scan = NULL;                                               \
                }                                                              \
                                                                               \
                while (up != NULL)                                             \
                {                                                              \
                    if (_map_->f_key->free)                                    \
                        _map_->f_key->free(scan->key);                         \
                    if (_map_->f_val->free)                                    \
                        _map_->f_val->free(scan->value);                       \
                                                                               \
                    _map_->alloc->free(scan);                                  \
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
        _map_->count = 0;                                                      \
        _map_->root = NULL;                                                    \
        _map_->flag = CMC_FLAG_OK;                                             \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _free)(struct SNAME * _map_)                                \
    {                                                                          \
        CMC_(PFX, _clear)(_map_);                                              \
                                                                               \
        _map_->alloc->free(_map_);                                             \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _customize)(struct SNAME * _map_,                           \
                               struct cmc_alloc_node * alloc,                  \
                               struct cmc_callbacks * callbacks)               \
    {                                                                          \
        if (!alloc)                                                            \
            _map_->alloc = &cmc_alloc_node_default;                            \
        else                                                                   \
            _map_->alloc = alloc;                                              \
                                                                               \
        _map_->callbacks = callbacks;                                          \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _insert)(struct SNAME * _map_, K key, V value)              \
    {                                                                          \
        if (CMC_(PFX, _empty)(_map_))                                          \
        {                                                                      \
            _map_->root = CMC_(PFX, _impl_new_node)(_map_, key, value);        \
                                                                               \
            if (!_map_->root)                                                  \
            {                                                                  \
                _map_->flag = CMC_FLAG_ALLOC;                                  \
                return false;                                                  \
            }                                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            struct CMC_DEF_NODE(SNAME) *scan = _map_->root;                    \
            struct CMC_DEF_NODE(SNAME) *parent = scan;                         \
                                                                               \
            while (scan != NULL)                                               \
            {                                                                  \
                parent = scan;                                                 \
                                                                               \
                if (_map_->f_key->cmp(scan->key, key) > 0)                     \
                    scan = scan->left;                                         \
                else if (_map_->f_key->cmp(scan->key, key) < 0)                \
                    scan = scan->right;                                        \
                else                                                           \
                {                                                              \
                    _map_->flag = CMC_FLAG_DUPLICATE;                          \
                    return false;                                              \
                }                                                              \
            }                                                                  \
                                                                               \
            struct CMC_DEF_NODE(SNAME) * node;                                 \
                                                                               \
            if (_map_->f_key->cmp(parent->key, key) > 0)                       \
            {                                                                  \
                parent->left = CMC_(PFX, _impl_new_node)(_map_, key, value);   \
                                                                               \
                if (!parent->left)                                             \
                {                                                              \
                    _map_->flag = CMC_FLAG_ALLOC;                              \
                    return false;                                              \
                }                                                              \
                                                                               \
                parent->left->parent = parent;                                 \
                node = parent->left;                                           \
            }                                                                  \
            else                                                               \
            {                                                                  \
                parent->right = CMC_(PFX, _impl_new_node)(_map_, key, value);  \
                                                                               \
                if (!parent->right)                                            \
                {                                                              \
                    _map_->flag = CMC_FLAG_ALLOC;                              \
                    return false;                                              \
                }                                                              \
                                                                               \
                parent->right->parent = parent;                                \
                node = parent->right;                                          \
            }                                                                  \
                                                                               \
            CMC_(PFX, _impl_rebalance)(_map_, node);                           \
        }                                                                      \
                                                                               \
        _map_->count++;                                                        \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->create)                      \
            _map_->callbacks->create();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _update)(struct SNAME * _map_, K key, V new_value,          \
                            V * old_value)                                     \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *node =                                     \
            CMC_(PFX, _impl_get_node)(_map_, key);                             \
                                                                               \
        if (!node)                                                             \
        {                                                                      \
            _map_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (old_value)                                                         \
            *old_value = node->value;                                          \
                                                                               \
        node->value = new_value;                                               \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->update)                      \
            _map_->callbacks->update();                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _remove)(struct SNAME * _map_, K key, V * out_value)        \
    {                                                                          \
        if (CMC_(PFX, _empty)(_map_))                                          \
        {                                                                      \
            _map_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *node =                                     \
            CMC_(PFX, _impl_get_node)(_map_, key);                             \
                                                                               \
        if (!node)                                                             \
        {                                                                      \
            _map_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (out_value)                                                         \
            *out_value = node->value;                                          \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *temp = NULL, *unbalanced = NULL;           \
                                                                               \
        bool is_root = node->parent == NULL;                                   \
                                                                               \
        if (node->left == NULL && node->right == NULL)                         \
        {                                                                      \
            if (is_root)                                                       \
                _map_->root = NULL;                                            \
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
            _map_->alloc->free(node);                                          \
        }                                                                      \
        else if (node->left == NULL)                                           \
        {                                                                      \
            if (is_root)                                                       \
            {                                                                  \
                _map_->root = node->right;                                     \
                _map_->root->parent = NULL;                                    \
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
            _map_->alloc->free(node);                                          \
        }                                                                      \
        else if (node->right == NULL)                                          \
        {                                                                      \
            if (is_root)                                                       \
            {                                                                  \
                _map_->root = node->left;                                      \
                _map_->root->parent = NULL;                                    \
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
            _map_->alloc->free(node);                                          \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            temp = node->right;                                                \
            while (temp->left != NULL)                                         \
                temp = temp->left;                                             \
                                                                               \
            K temp_key = temp->key;                                            \
            V temp_val = temp->value;                                          \
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
            _map_->alloc->free(temp);                                          \
                                                                               \
            node->key = temp_key;                                              \
            node->value = temp_val;                                            \
        }                                                                      \
                                                                               \
        if (unbalanced != NULL)                                                \
            CMC_(PFX, _impl_rebalance)(_map_, unbalanced);                     \
                                                                               \
        if (_map_->count == 0)                                                 \
            _map_->root = NULL;                                                \
                                                                               \
        _map_->count--;                                                        \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->delete)                      \
            _map_->callbacks->delete ();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _max)(struct SNAME * _map_, K * key, V * value)             \
    {                                                                          \
        if (CMC_(PFX, _empty)(_map_))                                          \
        {                                                                      \
            _map_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *scan = _map_->root;                        \
                                                                               \
        while (scan->right != NULL)                                            \
            scan = scan->right;                                                \
                                                                               \
        if (key)                                                               \
            *key = scan->key;                                                  \
        if (value)                                                             \
            *value = scan->value;                                              \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _min)(struct SNAME * _map_, K * key, V * value)             \
    {                                                                          \
        if (CMC_(PFX, _empty)(_map_))                                          \
        {                                                                      \
            _map_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *scan = _map_->root;                        \
                                                                               \
        while (scan->left != NULL)                                             \
            scan = scan->left;                                                 \
                                                                               \
        if (key)                                                               \
            *key = scan->key;                                                  \
        if (value)                                                             \
            *value = scan->value;                                              \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _get)(struct SNAME * _map_, K key)                             \
    {                                                                          \
        if (CMC_(PFX, _empty)(_map_))                                          \
        {                                                                      \
            _map_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *node =                                     \
            CMC_(PFX, _impl_get_node)(_map_, key);                             \
                                                                               \
        if (!node)                                                             \
        {                                                                      \
            _map_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return node->value;                                                    \
    }                                                                          \
                                                                               \
    V *CMC_(PFX, _get_ref)(struct SNAME * _map_, K key)                        \
    {                                                                          \
        if (CMC_(PFX, _empty)(_map_))                                          \
        {                                                                      \
            _map_->flag = CMC_FLAG_EMPTY;                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *node =                                     \
            CMC_(PFX, _impl_get_node)(_map_, key);                             \
                                                                               \
        if (!node)                                                             \
        {                                                                      \
            _map_->flag = CMC_FLAG_NOT_FOUND;                                  \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return &(node->value);                                                 \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _contains)(struct SNAME * _map_, K key)                     \
    {                                                                          \
        bool result = CMC_(PFX, _impl_get_node)(_map_, key) != NULL;           \
                                                                               \
        if (_map_->callbacks && _map_->callbacks->read)                        \
            _map_->callbacks->read();                                          \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _empty)(struct SNAME * _map_)                               \
    {                                                                          \
        return _map_->count == 0;                                              \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _count)(struct SNAME * _map_)                             \
    {                                                                          \
        return _map_->count;                                                   \
    }                                                                          \
                                                                               \
    int CMC_(PFX, _flag)(struct SNAME * _map_)                                 \
    {                                                                          \
        return _map_->flag;                                                    \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _map_)                    \
    {                                                                          \
        /* Callback will be added later */                                     \
        struct SNAME *result = CMC_(PFX, _new_custom)(                         \
            _map_->f_key, _map_->f_val, _map_->alloc, NULL);                   \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _map_->flag = CMC_FLAG_ERROR;                                      \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        /* TODO turn this into a normal loop */                                \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_map_);       \
                                                                               \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            K key = CMC_(PFX, _iter_key)(&iter);                               \
            V value = CMC_(PFX, _iter_value)(&iter);                           \
                                                                               \
            if (_map_->f_key->cpy)                                             \
                key = _map_->f_key->cpy(key);                                  \
            if (_map_->f_val->cpy)                                             \
                value = _map_->f_val->cpy(value);                              \
                                                                               \
            /* TODO check this for errors */                                   \
            CMC_(PFX, _insert)(result, key, value);                            \
        }                                                                      \
                                                                               \
        result->callbacks = _map_->callbacks;                                  \
        _map_->flag = CMC_FLAG_OK;                                             \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _equals)(struct SNAME * _map1_, struct SNAME * _map2_)      \
    {                                                                          \
        _map1_->flag = CMC_FLAG_OK;                                            \
        _map2_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_map1_->count != _map2_->count)                                    \
            return false;                                                      \
                                                                               \
        /* TODO turn this into a normal loop */                                \
        struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_map1_);      \
                                                                               \
        for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))  \
        {                                                                      \
            struct CMC_DEF_NODE(SNAME) *node = CMC_(PFX, _impl_get_node)(      \
                _map2_, CMC_(PFX, _iter_key)(&iter));                          \
                                                                               \
            if (node == NULL)                                                  \
                return false;                                                  \
                                                                               \
            if (_map1_->f_val->cmp(node->value,                                \
                                   CMC_(PFX, _iter_value)(&iter)) != 0)        \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _map_)              \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *m_ = _map_;                                              \
                                                                               \
        int n = snprintf(str.s, cmc_string_len, cmc_cmc_string_fmt_treemap,    \
                         #SNAME, #K, #V, m_, m_->root, m_->count, m_->flag,    \
                         m_->f_key, m_->f_val, m_->alloc, m_->callbacks);      \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr)                  \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *root = _map_->root;                        \
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
            if (!_map_->f_key->str(fptr, root->key) &&                         \
                !_map_->f_val->str(fptr, root->value))                         \
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
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target)   \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        iter.target = target;                                                  \
        iter.cursor = target->root;                                            \
        iter.first = NULL;                                                     \
        iter.last = NULL;                                                      \
        iter.index = 0;                                                        \
        iter.start = true;                                                     \
        iter.end = CMC_(PFX, _empty)(target);                                  \
                                                                               \
        if (!CMC_(PFX, _empty)(target))                                        \
        {                                                                      \
            while (iter.cursor->left != NULL)                                  \
                iter.cursor = iter.cursor->left;                               \
                                                                               \
            iter.first = iter.cursor;                                          \
                                                                               \
            iter.last = target->root;                                          \
            while (iter.last->right != NULL)                                   \
                iter.last = iter.last->right;                                  \
        }                                                                      \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target)     \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        iter.target = target;                                                  \
        iter.cursor = target->root;                                            \
        iter.first = NULL;                                                     \
        iter.last = NULL;                                                      \
        iter.index = 0;                                                        \
        iter.start = CMC_(PFX, _empty)(target);                                \
        iter.end = true;                                                       \
                                                                               \
        if (!CMC_(PFX, _empty)(target))                                        \
        {                                                                      \
            while (iter.cursor->right != NULL)                                 \
                iter.cursor = iter.cursor->right;                              \
                                                                               \
            iter.last = iter.cursor;                                           \
                                                                               \
            iter.first = target->root;                                         \
            while (iter.first->left != NULL)                                   \
                iter.first = iter.first->left;                                 \
                                                                               \
            iter.index = target->count - 1;                                    \
        }                                                                      \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                          \
        return CMC_(PFX, _empty)(iter->target) || iter->start;                 \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                          \
        return CMC_(PFX, _empty)(iter->target) || iter->end;                   \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                          \
        if (!CMC_(PFX, _empty)(iter->target))                                  \
        {                                                                      \
            iter->index = 0;                                                   \
            iter->start = true;                                                \
            iter->end = CMC_(PFX, _empty)(iter->target);                       \
            iter->cursor = iter->first;                                        \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                          \
        if (!CMC_(PFX, _empty)(iter->target))                                  \
        {                                                                      \
            iter->index = iter->target->count - 1;                             \
            iter->start = CMC_(PFX, _empty)(iter->target);                     \
            iter->end = true;                                                  \
            iter->cursor = iter->last;                                         \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)              \
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
        iter->start = CMC_(PFX, _empty)(iter->target);                         \
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
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)              \
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
        iter->end = CMC_(PFX, _empty)(iter->target);                           \
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
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                  size_t steps)                                \
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
        if (steps == 0 || iter->index + steps >= iter->target->count)          \
            return false;                                                      \
                                                                               \
        for (size_t i = 0; i < steps; i++)                                     \
            CMC_(PFX, _iter_next)(iter);                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter,            \
                                 size_t steps)                                 \
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
        for (size_t i = 0; i < steps; i++)                                     \
            CMC_(PFX, _iter_prev)(iter);                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator was able to be positioned at the */   \
    /* given index */                                                          \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter,             \
                                size_t index)                                  \
    {                                                                          \
        if (index >= iter->target->count)                                      \
            return false;                                                      \
                                                                               \
        if (iter->index > index)                                               \
            return CMC_(PFX, _iter_rewind)(iter, iter->index - index);         \
        else if (iter->index < index)                                          \
            return CMC_(PFX, _iter_advance)(iter, index - iter->index);        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter)                  \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
            return (K){ 0 };                                                   \
                                                                               \
        return iter->cursor->key;                                              \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)                \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
            return (V){ 0 };                                                   \
                                                                               \
        return iter->cursor->value;                                            \
    }                                                                          \
                                                                               \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
            return NULL;                                                       \
                                                                               \
        return &(iter->cursor->value);                                         \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)           \
    {                                                                          \
        return iter->index;                                                    \
    }                                                                          \
                                                                               \
    static struct CMC_DEF_NODE(SNAME) *                                        \
        CMC_(PFX, _impl_new_node)(struct SNAME * _map_, K key, V value)        \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *node =                                     \
            _map_->alloc->malloc(sizeof(struct CMC_DEF_NODE(SNAME)));          \
                                                                               \
        if (!node)                                                             \
            return NULL;                                                       \
                                                                               \
        node->key = key;                                                       \
        node->value = value;                                                   \
        node->right = NULL;                                                    \
        node->left = NULL;                                                     \
        node->parent = NULL;                                                   \
        node->height = 0;                                                      \
                                                                               \
        return node;                                                           \
    }                                                                          \
                                                                               \
    static struct CMC_DEF_NODE(SNAME) *                                        \
        CMC_(PFX, _impl_get_node)(struct SNAME * _map_, K key)                 \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *scan = _map_->root;                        \
                                                                               \
        while (scan != NULL)                                                   \
        {                                                                      \
            if (_map_->f_key->cmp(scan->key, key) > 0)                         \
                scan = scan->left;                                             \
            else if (_map_->f_key->cmp(scan->key, key) < 0)                    \
                scan = scan->right;                                            \
            else                                                               \
                return scan;                                                   \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    static unsigned char CMC_(PFX, _impl_h)(struct CMC_DEF_NODE(SNAME) * node) \
    {                                                                          \
        if (node == NULL)                                                      \
            return 0;                                                          \
                                                                               \
        return node->height;                                                   \
    }                                                                          \
                                                                               \
    static unsigned char CMC_(PFX, _impl_hupdate)(struct CMC_DEF_NODE(SNAME) * \
                                                  node)                        \
    {                                                                          \
        if (node == NULL)                                                      \
            return 0;                                                          \
                                                                               \
        unsigned char h_l = CMC_(PFX, _impl_h)(node->left);                    \
        unsigned char h_r = CMC_(PFX, _impl_h)(node->right);                   \
                                                                               \
        return 1 + (h_l > h_r ? h_l : h_r);                                    \
    }                                                                          \
                                                                               \
    static void CMC_(PFX, _impl_rotate_right)(struct CMC_DEF_NODE(SNAME) * *Z) \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *root = *Z;                                 \
        struct CMC_DEF_NODE(SNAME) *new_root = root->left;                     \
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
        root->height = CMC_(PFX, _impl_hupdate)(root);                         \
        new_root->height = CMC_(PFX, _impl_hupdate)(new_root);                 \
                                                                               \
        *Z = new_root;                                                         \
    }                                                                          \
                                                                               \
    static void CMC_(PFX, _impl_rotate_left)(struct CMC_DEF_NODE(SNAME) * *Z)  \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *root = *Z;                                 \
        struct CMC_DEF_NODE(SNAME) *new_root = root->right;                    \
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
        root->height = CMC_(PFX, _impl_hupdate)(root);                         \
        new_root->height = CMC_(PFX, _impl_hupdate)(new_root);                 \
                                                                               \
        *Z = new_root;                                                         \
    }                                                                          \
                                                                               \
    static void CMC_(PFX, _impl_rebalance)(struct SNAME * _map_,               \
                                           struct CMC_DEF_NODE(SNAME) * node)  \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *scan = node, *child = NULL;                \
                                                                               \
        int balance;                                                           \
        bool is_root = false;                                                  \
                                                                               \
        while (scan != NULL)                                                   \
        {                                                                      \
            if (scan->parent == NULL)                                          \
                is_root = true;                                                \
                                                                               \
            scan->height = CMC_(PFX, _impl_hupdate)(scan);                     \
            balance = CMC_(PFX, _impl_h)(scan->right) -                        \
                      CMC_(PFX, _impl_h)(scan->left);                          \
                                                                               \
            if (balance >= 2)                                                  \
            {                                                                  \
                child = scan->right;                                           \
                                                                               \
                if (CMC_(PFX, _impl_h)(child->right) <                         \
                    CMC_(PFX, _impl_h)(child->left))                           \
                    CMC_(PFX, _impl_rotate_right)(&(scan->right));             \
                                                                               \
                CMC_(PFX, _impl_rotate_left)(&scan);                           \
            }                                                                  \
            else if (balance <= -2)                                            \
            {                                                                  \
                child = scan->left;                                            \
                                                                               \
                if (CMC_(PFX, _impl_h)(child->left) <                          \
                    CMC_(PFX, _impl_h)(child->right))                          \
                    CMC_(PFX, _impl_rotate_left)(&(scan->left));               \
                                                                               \
                CMC_(PFX, _impl_rotate_right)(&scan);                          \
            }                                                                  \
                                                                               \
            if (is_root)                                                       \
            {                                                                  \
                _map_->root = scan;                                            \
                is_root = false;                                               \
            }                                                                  \
                                                                               \
            scan = scan->parent;                                               \
        }                                                                      \
    }

#endif /* CMC_CMC_TREEMAP_H */
