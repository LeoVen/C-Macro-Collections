/**
 * treemap.h
 *
 * Creation Date: 28/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/******************************************************************* TREEMAP */
/*****************************************************************************/

#ifndef CMC_TREEMAP_H
#define CMC_TREEMAP_H

#include <stdlib.h>
#include <stdbool.h>

#define TREEMAP_GENERATE(PFX, SNAME, FMOD, K, V)    \
    TREEMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
    TREEMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V) \
    TREEMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)

/* PRIVATE *******************************************************************/
#define TREEMAP_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    TREEMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define TREEMAP_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    TREEMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)             \
    TREEMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* PUBLIC ********************************************************************/
#define TREEMAP_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    TREEMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V)            \
    TREEMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)
#define TREEMAP_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    TREEMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)
/* STRUCT ********************************************************************/
#define TREEMAP_GENERATE_STRUCT(PFX, SNAME, FMOD, K, V) \
                                                        \
    struct SNAME##_s                                    \
    {                                                   \
        struct SNAME##_node_s *root;                    \
        size_t count;                                   \
        int (*cmp)(K, K);                               \
    };                                                  \
                                                        \
    struct SNAME##_node_s                               \
    {                                                   \
        K key;                                          \
        V value;                                        \
        unsigned char height;                           \
        struct SNAME##_node_s *right;                   \
        struct SNAME##_node_s *left;                    \
        struct SNAME##_node_s *parent;                  \
    };                                                  \
                                                        \
    struct SNAME##_iter_s                               \
    {                                                   \
        struct SNAME##_s *target;                       \
        struct SNAME##_node_s *cursor;                  \
        struct SNAME##_node_s *first;                   \
        struct SNAME##_node_s *last;                    \
        size_t index;                                   \
        bool start;                                     \
        bool end;                                       \
    };                                                  \
                                                        \
/* HEADER ********************************************************************/
#define TREEMAP_GENERATE_HEADER(PFX, SNAME, FMOD, K, V)                             \
                                                                                    \
    typedef struct SNAME##_s SNAME;                                                 \
    typedef struct SNAME##_node_s SNAME##_node;                                     \
    typedef struct SNAME##_iter_s SNAME##_iter;                                     \
                                                                                    \
    FMOD SNAME *PFX##_new(int (*compare)(K, K));                                    \
    FMOD void PFX##_clear(SNAME *_map_);                                            \
    FMOD void PFX##_free(SNAME *_map_);                                             \
    FMOD bool PFX##_insert(SNAME *_map_, K key, V value);                           \
    FMOD bool PFX##_remove(SNAME *_map_, K key, V *value);                          \
    FMOD bool PFX##_insert_if(SNAME *_map_, K key, V value, bool condition);        \
    FMOD bool PFX##_remove_if(SNAME *_map_, K key, V *value, bool condition);       \
    FMOD bool PFX##_max(SNAME *_map_, K *key, V *value);                            \
    FMOD bool PFX##_min(SNAME *_map_, K *key, V *value);                            \
    FMOD V PFX##_get(SNAME *_map_, K key);                                          \
    FMOD V *PFX##_get_ref(SNAME *_map_, K key);                                     \
    FMOD bool PFX##_contains(SNAME *_map_, K key);                                  \
    FMOD bool PFX##_empty(SNAME *_map_);                                            \
    FMOD size_t PFX##_count(SNAME *_map_);                                          \
                                                                                    \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);                    \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                 \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                   \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                               \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                                 \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, K *key, V *value, size_t *index); \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, K *key, V *value, size_t *index); \
                                                                                    \
/* SOURCE ********************************************************************/
#define TREEMAP_GENERATE_SOURCE(PFX, SNAME, FMOD, K, V)                            \
                                                                                   \
    FMOD SNAME##_node *PFX##_new_node(K key, V value);                             \
    FMOD SNAME##_node *PFX##_get_node(SNAME *_map_, K key);                        \
    FMOD unsigned char PFX##_h(SNAME##_node *node);                                \
    FMOD unsigned char PFX##_hupdate(SNAME##_node *node);                          \
    FMOD void PFX##_rotate_right(SNAME##_node **Z);                                \
    FMOD void PFX##_rotate_left(SNAME##_node **Z);                                 \
    FMOD void PFX##_rebalance(SNAME *_map_, SNAME##_node *node);                   \
                                                                                   \
    FMOD SNAME *PFX##_new(int (*compare)(K, K))                                    \
    {                                                                              \
        SNAME *_map_ = malloc(sizeof(SNAME));                                      \
                                                                                   \
        if (!_map_)                                                                \
            return NULL;                                                           \
                                                                                   \
        _map_->count = 0;                                                          \
        _map_->root = NULL;                                                        \
        _map_->cmp = compare;                                                      \
                                                                                   \
        return _map_;                                                              \
    }                                                                              \
                                                                                   \
    FMOD void PFX##_clear(SNAME *_map_)                                            \
    {                                                                              \
        SNAME##_node *scan = _map_->root;                                          \
        SNAME##_node *up = NULL;                                                   \
                                                                                   \
        while (scan != NULL)                                                       \
        {                                                                          \
            if (scan->left != NULL)                                                \
            {                                                                      \
                SNAME##_node *left = scan->left;                                   \
                                                                                   \
                scan->left = up;                                                   \
                up = scan;                                                         \
                scan = left;                                                       \
            }                                                                      \
            else if (scan->right != NULL)                                          \
            {                                                                      \
                SNAME##_node *right = scan->right;                                 \
                                                                                   \
                scan->left = up;                                                   \
                scan->right = NULL;                                                \
                up = scan;                                                         \
                scan = right;                                                      \
            }                                                                      \
            else                                                                   \
            {                                                                      \
                if (up == NULL)                                                    \
                {                                                                  \
                    free(scan);                                                    \
                    scan = NULL;                                                   \
                }                                                                  \
                                                                                   \
                while (up != NULL)                                                 \
                {                                                                  \
                    free(scan);                                                    \
                                                                                   \
                    if (up->right != NULL)                                         \
                    {                                                              \
                        scan = up->right;                                          \
                        up->right = NULL;                                          \
                        break;                                                     \
                    }                                                              \
                    else                                                           \
                    {                                                              \
                        scan = up;                                                 \
                        up = up->left;                                             \
                    }                                                              \
                }                                                                  \
            }                                                                      \
        }                                                                          \
                                                                                   \
        _map_->count = 0;                                                          \
        _map_->root = NULL;                                                        \
    }                                                                              \
                                                                                   \
    FMOD void PFX##_free(SNAME *_map_)                                             \
    {                                                                              \
        PFX##_clear(_map_);                                                        \
                                                                                   \
        free(_map_);                                                               \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_insert(SNAME *_map_, K key, V value)                           \
    {                                                                              \
        if (PFX##_empty(_map_))                                                    \
        {                                                                          \
            _map_->root = PFX##_new_node(key, value);                              \
                                                                                   \
            if (!_map_->root)                                                      \
                return false;                                                      \
        }                                                                          \
        else                                                                       \
        {                                                                          \
            SNAME##_node *scan = _map_->root;                                      \
            SNAME##_node *parent = scan;                                           \
                                                                                   \
            while (scan != NULL)                                                   \
            {                                                                      \
                parent = scan;                                                     \
                                                                                   \
                if (_map_->cmp(scan->key, key) > 0)                                \
                    scan = scan->left;                                             \
                else if (_map_->cmp(scan->key, key) < 0)                           \
                    scan = scan->right;                                            \
                else                                                               \
                    return false;                                                  \
            }                                                                      \
                                                                                   \
            SNAME##_node *node;                                                    \
                                                                                   \
            if (_map_->cmp(parent->key, key) > 0)                                  \
            {                                                                      \
                parent->left = PFX##_new_node(key, value);                         \
                                                                                   \
                if (!parent->left)                                                 \
                    return false;                                                  \
                                                                                   \
                parent->left->parent = parent;                                     \
                node = parent->left;                                               \
            }                                                                      \
            else                                                                   \
            {                                                                      \
                parent->right = PFX##_new_node(key, value);                        \
                                                                                   \
                if (!parent->right)                                                \
                    return false;                                                  \
                                                                                   \
                parent->right->parent = parent;                                    \
                node = parent->right;                                              \
            }                                                                      \
                                                                                   \
            PFX##_rebalance(_map_, node);                                          \
        }                                                                          \
                                                                                   \
        _map_->count++;                                                            \
                                                                                   \
        return true;                                                               \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_remove(SNAME *_map_, K key, V *value)                          \
    {                                                                              \
        SNAME##_node *node = PFX##_get_node(_map_, key);                           \
                                                                                   \
        if (!node)                                                                 \
            return false;                                                          \
                                                                                   \
        *value = node->value;                                                      \
                                                                                   \
        SNAME##_node *temp = NULL, *unbalanced = NULL;                             \
                                                                                   \
        bool is_root = node->parent == NULL;                                       \
                                                                                   \
        if (node->left == NULL && node->right == NULL)                             \
        {                                                                          \
            if (is_root)                                                           \
                _map_->root = NULL;                                                \
            else                                                                   \
            {                                                                      \
                unbalanced = node->parent;                                         \
                                                                                   \
                if (node->parent->right == node)                                   \
                    node->parent->right = NULL;                                    \
                else                                                               \
                    node->parent->left = NULL;                                     \
            }                                                                      \
                                                                                   \
            free(node);                                                            \
        }                                                                          \
        else if (node->left == NULL)                                               \
        {                                                                          \
            if (is_root)                                                           \
            {                                                                      \
                _map_->root = node->right;                                         \
                _map_->root->parent = NULL;                                        \
            }                                                                      \
            else                                                                   \
            {                                                                      \
                unbalanced = node->parent;                                         \
                                                                                   \
                node->right->parent = node->parent;                                \
                                                                                   \
                if (node->parent->right == node)                                   \
                    node->parent->right = node->right;                             \
                else                                                               \
                    node->parent->left = node->right;                              \
            }                                                                      \
                                                                                   \
            free(node);                                                            \
        }                                                                          \
        else if (node->right == NULL)                                              \
        {                                                                          \
            if (is_root)                                                           \
            {                                                                      \
                _map_->root = node->left;                                          \
                _map_->root->parent = NULL;                                        \
            }                                                                      \
            else                                                                   \
            {                                                                      \
                unbalanced = node->parent;                                         \
                                                                                   \
                node->left->parent = node->parent;                                 \
                                                                                   \
                if (node->parent->right == node)                                   \
                    node->parent->right = node->left;                              \
                else                                                               \
                    node->parent->left = node->left;                               \
            }                                                                      \
                                                                                   \
            free(node);                                                            \
        }                                                                          \
        else                                                                       \
        {                                                                          \
            temp = node->right;                                                    \
            while (temp->left != NULL)                                             \
                temp = temp->left;                                                 \
                                                                                   \
            K temp_key = temp->key;                                                \
            V temp_val = temp->value;                                              \
                                                                                   \
            unbalanced = temp->parent;                                             \
                                                                                   \
            if (temp->left == NULL && temp->right == NULL)                         \
            {                                                                      \
                if (temp->parent->right == temp)                                   \
                    temp->parent->right = NULL;                                    \
                else                                                               \
                    temp->parent->left = NULL;                                     \
            }                                                                      \
            else if (temp->left == NULL)                                           \
            {                                                                      \
                temp->right->parent = temp->parent;                                \
                                                                                   \
                if (temp->parent->right == temp)                                   \
                    temp->parent->right = temp->right;                             \
                else                                                               \
                    temp->parent->left = temp->right;                              \
            }                                                                      \
            else if (temp->right == NULL)                                          \
            {                                                                      \
                temp->left->parent = temp->parent;                                 \
                                                                                   \
                if (temp->parent->right == temp)                                   \
                    temp->parent->right = temp->left;                              \
                else                                                               \
                    temp->parent->left = temp->left;                               \
            }                                                                      \
                                                                                   \
            free(temp);                                                            \
                                                                                   \
            node->key = temp_key;                                                  \
            node->value = temp_val;                                                \
        }                                                                          \
                                                                                   \
        if (unbalanced != NULL)                                                    \
            PFX##_rebalance(_map_, unbalanced);                                    \
                                                                                   \
        _map_->count--;                                                            \
                                                                                   \
        if (_map_->count == 0)                                                     \
            _map_->root = NULL;                                                    \
                                                                                   \
        return true;                                                               \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_insert_if(SNAME *_map_, K key, V value, bool condition)        \
    {                                                                              \
        if (condition)                                                             \
            return PFX##_insert(_map_, key, value);                                \
                                                                                   \
        return false;                                                              \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_remove_if(SNAME *_map_, K key, V *value, bool condition)       \
    {                                                                              \
        if (condition)                                                             \
            return PFX##_remove(_map_, key, value);                                \
                                                                                   \
        return false;                                                              \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_max(SNAME *_map_, K *key, V *value)                            \
    {                                                                              \
        if (PFX##_empty(_map_))                                                    \
            return false;                                                          \
                                                                                   \
        SNAME##_node *scan = _map_->root;                                          \
                                                                                   \
        while (scan->right != NULL)                                                \
            scan = scan->right;                                                    \
                                                                                   \
        *key = scan->key;                                                          \
        *value = scan->value;                                                      \
                                                                                   \
        return true;                                                               \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_min(SNAME *_map_, K *key, V *value)                            \
    {                                                                              \
        if (PFX##_empty(_map_))                                                    \
            return false;                                                          \
                                                                                   \
        SNAME##_node *scan = _map_->root;                                          \
                                                                                   \
        while (scan->left != NULL)                                                 \
            scan = scan->left;                                                     \
                                                                                   \
        *key = scan->key;                                                          \
        *value = scan->value;                                                      \
                                                                                   \
        return true;                                                               \
    }                                                                              \
                                                                                   \
    FMOD V PFX##_get(SNAME *_map_, K key)                                          \
    {                                                                              \
        SNAME##_node *node = PFX##_get_node(_map_, key);                           \
                                                                                   \
        if (!node)                                                                 \
            return 0;                                                              \
                                                                                   \
        return node->value;                                                        \
    }                                                                              \
                                                                                   \
    FMOD V *PFX##_get_ref(SNAME *_map_, K key)                                     \
    {                                                                              \
        SNAME##_node *node = PFX##_get_node(_map_, key);                           \
                                                                                   \
        if (!node)                                                                 \
            return NULL;                                                           \
                                                                                   \
        return &(node->value);                                                     \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_contains(SNAME *_map_, K key)                                  \
    {                                                                              \
        SNAME##_node *scan = _map_->root;                                          \
                                                                                   \
        while (scan != NULL)                                                       \
        {                                                                          \
            if (_map_->cmp(scan->key, key) > 0)                                    \
                scan = scan->left;                                                 \
            else if (_map_->cmp(scan->key, key) < 0)                               \
                scan = scan->right;                                                \
            else                                                                   \
                return true;                                                       \
        }                                                                          \
                                                                                   \
        return false;                                                              \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_empty(SNAME *_map_)                                            \
    {                                                                              \
        return _map_->count == 0;                                                  \
    }                                                                              \
                                                                                   \
    FMOD size_t PFX##_count(SNAME *_map_)                                          \
    {                                                                              \
        return _map_->count;                                                       \
    }                                                                              \
                                                                                   \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target)                    \
    {                                                                              \
        iter->target = target;                                                     \
        iter->index = 0;                                                           \
        iter->start = true;                                                        \
        iter->end = PFX##_empty(target);                                           \
                                                                                   \
        iter->cursor = target->root;                                               \
        while (iter->cursor->left != NULL)                                         \
            iter->cursor = iter->cursor->left;                                     \
                                                                                   \
        iter->first = iter->cursor;                                                \
                                                                                   \
        iter->last = target->root;                                                 \
        while (iter->last->right != NULL)                                          \
            iter->last = iter->last->right;                                        \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                 \
    {                                                                              \
        return PFX##_empty(iter->target) || iter->start;                           \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                   \
    {                                                                              \
        return PFX##_empty(iter->target) || iter->end;                             \
    }                                                                              \
                                                                                   \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter)                               \
    {                                                                              \
        iter->index = 0;                                                           \
        iter->start = true;                                                        \
        iter->end = PFX##_empty(iter->target);                                     \
                                                                                   \
        iter->cursor = iter->first;                                                \
    }                                                                              \
                                                                                   \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter)                                 \
    {                                                                              \
        iter->index = iter->target->count - 1;                                     \
        iter->start = PFX##_empty(iter->target);                                   \
        iter->end = true;                                                          \
                                                                                   \
        iter->cursor = iter->last;                                                 \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, K *key, V *value, size_t *index) \
    {                                                                              \
        if (iter->end)                                                             \
            return false;                                                          \
                                                                                   \
        *key = iter->cursor->key;                                                  \
        *value = iter->cursor->value;                                              \
        *index = iter->index;                                                      \
                                                                                   \
        iter->start = false;                                                       \
                                                                                   \
        if (iter->cursor->right != NULL)                                           \
        {                                                                          \
            iter->cursor = iter->cursor->right;                                    \
            while (iter->cursor->left != NULL)                                     \
                iter->cursor = iter->cursor->left;                                 \
                                                                                   \
            iter->index++;                                                         \
                                                                                   \
            return true;                                                           \
        }                                                                          \
                                                                                   \
        while (true)                                                               \
        {                                                                          \
            if (iter->cursor == iter->last)                                        \
            {                                                                      \
                iter->end = true;                                                  \
                                                                                   \
                return true;                                                       \
            }                                                                      \
            if (iter->cursor->parent->left == iter->cursor)                        \
            {                                                                      \
                iter->cursor = iter->cursor->parent;                               \
                                                                                   \
                iter->index++;                                                     \
                                                                                   \
                return true;                                                       \
            }                                                                      \
                                                                                   \
            iter->cursor = iter->cursor->parent;                                   \
        }                                                                          \
    }                                                                              \
                                                                                   \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, K *key, V *value, size_t *index) \
    {                                                                              \
        if (iter->start)                                                           \
            return false;                                                          \
                                                                                   \
        *key = iter->cursor->key;                                                  \
        *value = iter->cursor->value;                                              \
        *index = iter->index;                                                      \
                                                                                   \
        iter->end = false;                                                         \
                                                                                   \
        if (iter->cursor->left != NULL)                                            \
        {                                                                          \
            iter->cursor = iter->cursor->left;                                     \
                                                                                   \
            while (iter->cursor->right != NULL)                                    \
                iter->cursor = iter->cursor->right;                                \
                                                                                   \
            iter->index--;                                                         \
                                                                                   \
            return true;                                                           \
        }                                                                          \
                                                                                   \
        while (true)                                                               \
        {                                                                          \
            if (iter->cursor == iter->first)                                       \
            {                                                                      \
                iter->start = true;                                                \
                                                                                   \
                return true;                                                       \
            }                                                                      \
            if (iter->cursor->parent->right == iter->cursor)                       \
            {                                                                      \
                iter->cursor = iter->cursor->parent;                               \
                                                                                   \
                iter->index--;                                                     \
                                                                                   \
                return true;                                                       \
            }                                                                      \
                                                                                   \
            iter->cursor = iter->cursor->parent;                                   \
        }                                                                          \
    }                                                                              \
                                                                                   \
    FMOD SNAME##_node *PFX##_new_node(K key, V value)                              \
    {                                                                              \
        SNAME##_node *node = malloc(sizeof(SNAME##_node));                         \
                                                                                   \
        if (!node)                                                                 \
            return NULL;                                                           \
                                                                                   \
        node->key = key;                                                           \
        node->value = value;                                                       \
        node->right = NULL;                                                        \
        node->left = NULL;                                                         \
        node->parent = NULL;                                                       \
        node->height = 0;                                                          \
                                                                                   \
        return node;                                                               \
    }                                                                              \
                                                                                   \
    FMOD SNAME##_node *PFX##_get_node(SNAME *_map_, K key)                         \
    {                                                                              \
        if (PFX##_empty(_map_))                                                    \
            return NULL;                                                           \
                                                                                   \
        SNAME##_node *scan = _map_->root;                                          \
                                                                                   \
        while (scan != NULL)                                                       \
        {                                                                          \
            if (_map_->cmp(scan->key, key) > 0)                                    \
                scan = scan->left;                                                 \
            else if (_map_->cmp(scan->key, key) < 0)                               \
                scan = scan->right;                                                \
            else                                                                   \
                return scan;                                                       \
        }                                                                          \
                                                                                   \
        return NULL;                                                               \
    }                                                                              \
                                                                                   \
    FMOD unsigned char PFX##_h(SNAME##_node *node)                                 \
    {                                                                              \
        if (node == NULL)                                                          \
            return 0;                                                              \
                                                                                   \
        return node->height;                                                       \
    }                                                                              \
                                                                                   \
    FMOD unsigned char PFX##_hupdate(SNAME##_node *node)                           \
    {                                                                              \
        if (node == NULL)                                                          \
            return 0;                                                              \
                                                                                   \
        unsigned char h_l = PFX##_h(node->left);                                   \
        unsigned char h_r = PFX##_h(node->right);                                  \
                                                                                   \
        return 1 + (h_l > h_r ? h_l : h_r);                                        \
    }                                                                              \
                                                                                   \
    FMOD void PFX##_rotate_right(SNAME##_node **Z)                                 \
    {                                                                              \
        SNAME##_node *root = *Z;                                                   \
        SNAME##_node *new_root = root->left;                                       \
                                                                                   \
        if (root->parent != NULL)                                                  \
        {                                                                          \
            if (root->parent->left == root)                                        \
                root->parent->left = new_root;                                     \
            else                                                                   \
                root->parent->right = new_root;                                    \
        }                                                                          \
                                                                                   \
        new_root->parent = root->parent;                                           \
                                                                                   \
        root->parent = new_root;                                                   \
        root->left = new_root->right;                                              \
                                                                                   \
        if (root->left)                                                            \
            root->left->parent = root;                                             \
                                                                                   \
        new_root->right = root;                                                    \
                                                                                   \
        root->height = PFX##_hupdate(root);                                        \
        new_root->height = PFX##_hupdate(new_root);                                \
                                                                                   \
        *Z = new_root;                                                             \
    }                                                                              \
                                                                                   \
    FMOD void PFX##_rotate_left(SNAME##_node **Z)                                  \
    {                                                                              \
        SNAME##_node *root = *Z;                                                   \
        SNAME##_node *new_root = root->right;                                      \
                                                                                   \
        if (root->parent != NULL)                                                  \
        {                                                                          \
            if (root->parent->right == root)                                       \
                root->parent->right = new_root;                                    \
            else                                                                   \
                root->parent->left = new_root;                                     \
        }                                                                          \
                                                                                   \
        new_root->parent = root->parent;                                           \
                                                                                   \
        root->parent = new_root;                                                   \
        root->right = new_root->left;                                              \
                                                                                   \
        if (root->right)                                                           \
            root->right->parent = root;                                            \
                                                                                   \
        new_root->left = root;                                                     \
                                                                                   \
        root->height = PFX##_hupdate(root);                                        \
        new_root->height = PFX##_hupdate(new_root);                                \
                                                                                   \
        *Z = new_root;                                                             \
    }                                                                              \
                                                                                   \
    FMOD void PFX##_rebalance(SNAME *_map_, SNAME##_node *node)                    \
    {                                                                              \
        SNAME##_node *scan = node, *child = NULL;                                  \
                                                                                   \
        int balance;                                                               \
        bool is_root = false;                                                      \
                                                                                   \
        while (scan != NULL)                                                       \
        {                                                                          \
            if (scan->parent == NULL)                                              \
                is_root = true;                                                    \
                                                                                   \
            scan->height = PFX##_hupdate(scan);                                    \
            balance = PFX##_h(scan->right) - PFX##_h(scan->left);                  \
                                                                                   \
            if (balance >= 2)                                                      \
            {                                                                      \
                child = scan->right;                                               \
                                                                                   \
                if (PFX##_h(child->right) < PFX##_h(child->left))                  \
                    PFX##_rotate_right(&(scan->right));                            \
                                                                                   \
                PFX##_rotate_left(&scan);                                          \
            }                                                                      \
            else if (balance <= -2)                                                \
            {                                                                      \
                child = scan->left;                                                \
                                                                                   \
                if (PFX##_h(child->left) < PFX##_h(child->right))                  \
                    PFX##_rotate_left(&(scan->left));                              \
                                                                                   \
                PFX##_rotate_right(&scan);                                         \
            }                                                                      \
                                                                                   \
            if (is_root)                                                           \
            {                                                                      \
                _map_->root = scan;                                                \
                is_root = false;                                                   \
            }                                                                      \
                                                                                   \
            scan = scan->parent;                                                   \
        }                                                                          \
    }

#endif /* CMC_TREEMAP_H */
