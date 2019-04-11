/**
 * treeset.h
 *
 * Creation Date: 27/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/******************************************************************* TREESET */
/*****************************************************************************/

#ifndef CMC_TREESET_H
#define CMC_TREESET_H

#include <stdlib.h>
#include <stdbool.h>

#define TREESET_GENERATE(PFX, SNAME, FMOD, V)    \
    TREESET_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
    TREESET_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    TREESET_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* PRIVATE *******************************************************************/
#define TREESET_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    TREESET_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define TREESET_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    TREESET_GENERATE_STRUCT(PFX, SNAME, FMOD, V)                \
    TREESET_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* PUBLIC ********************************************************************/
#define TREESET_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    TREESET_GENERATE_STRUCT(PFX, SNAME, FMOD, V)               \
    TREESET_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define TREESET_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    TREESET_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* STRUCT ********************************************************************/
#define TREESET_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
                                                     \
    struct SNAME##_s                                 \
    {                                                \
        struct SNAME##_node_s *root;                 \
        size_t count;                                \
        int (*cmp)(V, V);                            \
    };                                               \
                                                     \
    struct SNAME##_node_s                            \
    {                                                \
        V key;                                       \
        unsigned char height;                        \
        struct SNAME##_node_s *right;                \
        struct SNAME##_node_s *left;                 \
        struct SNAME##_node_s *parent;               \
    };                                               \
                                                     \
    struct SNAME##_iter_s                            \
    {                                                \
        struct SNAME##_s *target;                    \
        struct SNAME##_node_s *cursor;               \
        struct SNAME##_node_s *first;                \
        struct SNAME##_node_s *last;                 \
        size_t index;                                \
        bool start;                                  \
        bool end;                                    \
    };                                               \
                                                     \
/* HEADER ********************************************************************/
#define TREESET_GENERATE_HEADER(PFX, SNAME, FMOD, V)                        \
                                                                            \
    typedef struct SNAME##_s SNAME;                                         \
    typedef struct SNAME##_node_s SNAME##_node;                             \
    typedef struct SNAME##_iter_s SNAME##_iter;                             \
                                                                            \
    FMOD SNAME *PFX##_new(int (*compare)(V, V));                            \
    FMOD void PFX##_clear(SNAME *_set_);                                    \
    FMOD void PFX##_free(SNAME *_set_);                                     \
    FMOD bool PFX##_insert(SNAME *_set_, V element);                        \
    FMOD bool PFX##_remove(SNAME *_set_, V element);                        \
    FMOD bool PFX##_insert_if(SNAME *_set_, V element, bool condition);     \
    FMOD bool PFX##_remove_if(SNAME *_set_, V element, bool condition);     \
    FMOD V PFX##_max(SNAME *_set_);                                         \
    FMOD V PFX##_min(SNAME *_set_);                                         \
    FMOD bool PFX##_empty(SNAME *_set_);                                    \
    FMOD size_t PFX##_count(SNAME *_set_);                                  \
                                                                            \
    FMOD SNAME *PFX##_union(SNAME *_set1_, SNAME *_set2_);                  \
    FMOD SNAME *PFX##_intersection(SNAME *_set1_, SNAME *_set2_);           \
    FMOD SNAME *PFX##_difference(SNAME *_set1_, SNAME *_set2_);             \
    FMOD SNAME *PFX##_symmetric_difference(SNAME *_set1_, SNAME *_set2_);   \
                                                                            \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);            \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                         \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                           \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                       \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                         \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *value, size_t *index); \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *value, size_t *index); \
                                                                            \
/* SOURCE ********************************************************************/
#define TREESET_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                       \
                                                                           \
    FMOD SNAME##_node *PFX##_new_node(V element);                          \
    FMOD SNAME##_node *PFX##_get_node(SNAME *_set_, V element);            \
    FMOD unsigned char PFX##_h(SNAME##_node *node);                        \
    FMOD unsigned char PFX##_hupdate(SNAME##_node *node);                  \
    FMOD void PFX##_rotate_right(SNAME##_node **Z);                        \
    FMOD void PFX##_rotate_left(SNAME##_node **Z);                         \
    FMOD void PFX##_rebalance(SNAME *_set_, SNAME##_node *node);           \
                                                                           \
    FMOD SNAME *PFX##_new(int (*compare)(V, V))                            \
    {                                                                      \
        SNAME *_set_ = malloc(sizeof(SNAME));                              \
                                                                           \
        if (!_set_)                                                        \
            return NULL;                                                   \
                                                                           \
        _set_->count = 0;                                                  \
        _set_->root = NULL;                                                \
        _set_->cmp = compare;                                              \
                                                                           \
        return _set_;                                                      \
    }                                                                      \
                                                                           \
    FMOD void PFX##_clear(SNAME *_set_)                                    \
    {                                                                      \
        SNAME##_node *scan = _set_->root;                                  \
        SNAME##_node *up = NULL;                                           \
                                                                           \
        while (scan != NULL)                                               \
        {                                                                  \
            if (scan->left != NULL)                                        \
            {                                                              \
                SNAME##_node *left = scan->left;                           \
                                                                           \
                scan->left = up;                                           \
                up = scan;                                                 \
                scan = left;                                               \
            }                                                              \
            else if (scan->right != NULL)                                  \
            {                                                              \
                SNAME##_node *right = scan->right;                         \
                                                                           \
                scan->left = up;                                           \
                scan->right = NULL;                                        \
                up = scan;                                                 \
                scan = right;                                              \
            }                                                              \
            else                                                           \
            {                                                              \
                if (up == NULL)                                            \
                {                                                          \
                    free(scan);                                            \
                    scan = NULL;                                           \
                }                                                          \
                                                                           \
                while (up != NULL)                                         \
                {                                                          \
                    free(scan);                                            \
                                                                           \
                    if (up->right != NULL)                                 \
                    {                                                      \
                        scan = up->right;                                  \
                        up->right = NULL;                                  \
                        break;                                             \
                    }                                                      \
                    else                                                   \
                    {                                                      \
                        scan = up;                                         \
                        up = up->left;                                     \
                    }                                                      \
                }                                                          \
            }                                                              \
        }                                                                  \
                                                                           \
        _set_->count = 0;                                                  \
        _set_->root = NULL;                                                \
    }                                                                      \
                                                                           \
    FMOD void PFX##_free(SNAME *_set_)                                     \
    {                                                                      \
        PFX##_clear(_set_);                                                \
                                                                           \
        free(_set_);                                                       \
    }                                                                      \
                                                                           \
    FMOD bool PFX##_insert(SNAME *_set_, V element)                        \
    {                                                                      \
        if (PFX##_empty(_set_))                                            \
        {                                                                  \
            _set_->root = PFX##_new_node(element);                         \
                                                                           \
            if (!_set_->root)                                              \
                return false;                                              \
        }                                                                  \
        else                                                               \
        {                                                                  \
            SNAME##_node *scan = _set_->root;                              \
            SNAME##_node *parent = scan;                                   \
                                                                           \
            while (scan != NULL)                                           \
            {                                                              \
                parent = scan;                                             \
                                                                           \
                if (_set_->cmp(scan->key, element) > 0)                    \
                    scan = scan->left;                                     \
                else if (_set_->cmp(scan->key, element) < 0)               \
                    scan = scan->right;                                    \
                else                                                       \
                    return false;                                          \
            }                                                              \
                                                                           \
            SNAME##_node *node;                                            \
                                                                           \
            if (_set_->cmp(parent->key, element) > 0)                      \
            {                                                              \
                parent->left = PFX##_new_node(element);                    \
                                                                           \
                if (!parent->left)                                         \
                    return false;                                          \
                                                                           \
                parent->left->parent = parent;                             \
                node = parent->left;                                       \
            }                                                              \
            else                                                           \
            {                                                              \
                parent->right = PFX##_new_node(element);                   \
                                                                           \
                if (!parent->right)                                        \
                    return false;                                          \
                                                                           \
                parent->right->parent = parent;                            \
                node = parent->right;                                      \
            }                                                              \
                                                                           \
            PFX##_rebalance(_set_, node);                                  \
        }                                                                  \
                                                                           \
        _set_->count++;                                                    \
                                                                           \
        return true;                                                       \
    }                                                                      \
                                                                           \
    FMOD bool PFX##_remove(SNAME *_set_, V element)                        \
    {                                                                      \
        SNAME##_node *node = PFX##_get_node(_set_, element);               \
                                                                           \
        if (!node)                                                         \
            return false;                                                  \
                                                                           \
        SNAME##_node *temp = NULL, *unbalanced = NULL;                     \
                                                                           \
        bool is_root = node->parent == NULL;                               \
                                                                           \
        if (node->left == NULL && node->right == NULL)                     \
        {                                                                  \
            if (is_root)                                                   \
                _set_->root = NULL;                                        \
            else                                                           \
            {                                                              \
                unbalanced = node->parent;                                 \
                                                                           \
                if (node->parent->right == node)                           \
                    node->parent->right = NULL;                            \
                else                                                       \
                    node->parent->left = NULL;                             \
            }                                                              \
                                                                           \
            free(node);                                                    \
        }                                                                  \
        else if (node->left == NULL)                                       \
        {                                                                  \
            if (is_root)                                                   \
            {                                                              \
                _set_->root = node->right;                                 \
                _set_->root->parent = NULL;                                \
            }                                                              \
            else                                                           \
            {                                                              \
                unbalanced = node->parent;                                 \
                                                                           \
                node->right->parent = node->parent;                        \
                                                                           \
                if (node->parent->right == node)                           \
                    node->parent->right = node->right;                     \
                else                                                       \
                    node->parent->left = node->right;                      \
            }                                                              \
                                                                           \
            free(node);                                                    \
        }                                                                  \
        else if (node->right == NULL)                                      \
        {                                                                  \
            if (is_root)                                                   \
            {                                                              \
                _set_->root = node->left;                                  \
                _set_->root->parent = NULL;                                \
            }                                                              \
            else                                                           \
            {                                                              \
                unbalanced = node->parent;                                 \
                                                                           \
                node->left->parent = node->parent;                         \
                                                                           \
                if (node->parent->right == node)                           \
                    node->parent->right = node->left;                      \
                else                                                       \
                    node->parent->left = node->left;                       \
            }                                                              \
                                                                           \
            free(node);                                                    \
        }                                                                  \
        else                                                               \
        {                                                                  \
            temp = node->right;                                            \
            while (temp->left != NULL)                                     \
                temp = temp->left;                                         \
                                                                           \
            V temp_key = temp->key;                                        \
                                                                           \
            unbalanced = temp->parent;                                     \
                                                                           \
            if (temp->left == NULL && temp->right == NULL)                 \
            {                                                              \
                if (temp->parent->right == temp)                           \
                    temp->parent->right = NULL;                            \
                else                                                       \
                    temp->parent->left = NULL;                             \
            }                                                              \
            else if (temp->left == NULL)                                   \
            {                                                              \
                temp->right->parent = temp->parent;                        \
                                                                           \
                if (temp->parent->right == temp)                           \
                    temp->parent->right = temp->right;                     \
                else                                                       \
                    temp->parent->left = temp->right;                      \
            }                                                              \
            else if (temp->right == NULL)                                  \
            {                                                              \
                temp->left->parent = temp->parent;                         \
                                                                           \
                if (temp->parent->right == temp)                           \
                    temp->parent->right = temp->left;                      \
                else                                                       \
                    temp->parent->left = temp->left;                       \
            }                                                              \
                                                                           \
            free(temp);                                                    \
                                                                           \
            node->key = temp_key;                                          \
        }                                                                  \
                                                                           \
        if (unbalanced != NULL)                                            \
            PFX##_rebalance(_set_, unbalanced);                            \
                                                                           \
        _set_->count--;                                                    \
                                                                           \
        return true;                                                       \
    }                                                                      \
                                                                           \
    FMOD bool PFX##_insert_if(SNAME *_set_, V element, bool condition)     \
    {                                                                      \
        if (condition)                                                     \
            return PFX##_insert(_set_, element);                           \
                                                                           \
        return false;                                                      \
    }                                                                      \
                                                                           \
    FMOD bool PFX##_remove_if(SNAME *_set_, V element, bool condition)     \
    {                                                                      \
        if (condition)                                                     \
            return PFX##_remove(_set_, element);                           \
                                                                           \
        return false;                                                      \
    }                                                                      \
                                                                           \
    FMOD V PFX##_max(SNAME *_set_)                                         \
    {                                                                      \
        if (PFX##_empty(_set_))                                            \
            return 0;                                                      \
                                                                           \
        SNAME##_node *scan = _set_->root;                                  \
                                                                           \
        while (scan->right != NULL)                                        \
            scan = scan->right;                                            \
                                                                           \
        return scan->key;                                                  \
    }                                                                      \
                                                                           \
    FMOD V PFX##_min(SNAME *_set_)                                         \
    {                                                                      \
        if (PFX##_empty(_set_))                                            \
            return 0;                                                      \
                                                                           \
        SNAME##_node *scan = _set_->root;                                  \
                                                                           \
        while (scan->left != NULL)                                         \
            scan = scan->left;                                             \
                                                                           \
        return scan->key;                                                  \
    }                                                                      \
                                                                           \
    FMOD bool PFX##_empty(SNAME *_set_)                                    \
    {                                                                      \
        return _set_->count == 0;                                          \
    }                                                                      \
                                                                           \
    FMOD size_t PFX##_count(SNAME *_set_)                                  \
    {                                                                      \
        return _set_->count;                                               \
    }                                                                      \
                                                                           \
    FMOD SNAME *PFX##_union(SNAME *_set1_, SNAME *_set2_)                  \
    {                                                                      \
        SNAME##_iter iter1, iter2;                                         \
        size_t index;                                                      \
        V value;                                                           \
                                                                           \
        SNAME *_set_r_ = PFX##_new(_set1_->cmp);                           \
                                                                           \
        if (!_set_r_)                                                      \
            return false;                                                  \
                                                                           \
        PFX##_iter_new(&iter1, _set1_);                                    \
        PFX##_iter_new(&iter2, _set2_);                                    \
                                                                           \
        for (PFX##_iter_tostart(&iter1); !PFX##_iter_end(&iter1);)         \
        {                                                                  \
            PFX##_iter_next(&iter1, &value, &index);                       \
            PFX##_insert(_set_r_, value);                                  \
        }                                                                  \
                                                                           \
        for (PFX##_iter_tostart(&iter2); !PFX##_iter_end(&iter2);)         \
        {                                                                  \
            PFX##_iter_next(&iter2, &value, &index);                       \
            PFX##_insert(_set_r_, value);                                  \
        }                                                                  \
                                                                           \
        return _set_r_;                                                    \
    }                                                                      \
                                                                           \
    FMOD SNAME *PFX##_intersection(SNAME *_set1_, SNAME *_set2_)           \
    {                                                                      \
        SNAME##_iter iter;                                                 \
        size_t index;                                                      \
        V value;                                                           \
                                                                           \
        SNAME *_set_r_ = PFX##_new(_set1_->cmp);                           \
                                                                           \
        if (!_set_r_)                                                      \
            return false;                                                  \
                                                                           \
        SNAME *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;  \
        SNAME *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;              \
                                                                           \
        PFX##_iter_new(&iter, _set_A_);                                    \
                                                                           \
        for (PFX##_iter_tostart(&iter); !PFX##_iter_end(&iter);)           \
        {                                                                  \
            PFX##_iter_next(&iter, &value, &index);                        \
            if (PFX##_get_node(_set_B_, value) != NULL)                    \
                PFX##_insert(_set_r_, value);                              \
        }                                                                  \
                                                                           \
        return _set_r_;                                                    \
    }                                                                      \
                                                                           \
    FMOD SNAME *PFX##_difference(SNAME *_set1_, SNAME *_set2_)             \
    {                                                                      \
        SNAME##_iter iter;                                                 \
        size_t index;                                                      \
        V value;                                                           \
                                                                           \
        SNAME *_set_r_ = PFX##_new(_set1_->cmp);                           \
                                                                           \
        if (!_set_r_)                                                      \
            return false;                                                  \
                                                                           \
        PFX##_iter_new(&iter, _set1_);                                     \
                                                                           \
        for (PFX##_iter_tostart(&iter); !PFX##_iter_end(&iter);)           \
        {                                                                  \
            PFX##_iter_next(&iter, &value, &index);                        \
            if (PFX##_get_node(_set2_, value) == NULL)                     \
                PFX##_insert(_set_r_, value);                              \
        }                                                                  \
                                                                           \
        return _set_r_;                                                    \
    }                                                                      \
                                                                           \
    FMOD SNAME *PFX##_symmetric_difference(SNAME *_set1_, SNAME *_set2_)   \
    {                                                                      \
        SNAME##_iter iter1, iter2;                                         \
        size_t index;                                                      \
        V value;                                                           \
                                                                           \
        SNAME *_set_r_ = PFX##_new(_set1_->cmp);                           \
                                                                           \
        if (!_set_r_)                                                      \
            return false;                                                  \
                                                                           \
        PFX##_iter_new(&iter1, _set1_);                                    \
        PFX##_iter_new(&iter2, _set2_);                                    \
                                                                           \
        for (PFX##_iter_tostart(&iter1); !PFX##_iter_end(&iter1);)         \
        {                                                                  \
            PFX##_iter_next(&iter1, &value, &index);                       \
            if (PFX##_get_node(_set2_, value) == NULL)                     \
                PFX##_insert(_set_r_, value);                              \
        }                                                                  \
                                                                           \
        for (PFX##_iter_tostart(&iter2); !PFX##_iter_end(&iter2);)         \
        {                                                                  \
            PFX##_iter_next(&iter2, &value, &index);                       \
            if (PFX##_get_node(_set1_, value) == NULL)                     \
                PFX##_insert(_set_r_, value);                              \
        }                                                                  \
                                                                           \
        return _set_r_;                                                    \
    }                                                                      \
                                                                           \
    FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target)            \
    {                                                                      \
        iter->target = target;                                             \
        iter->index = 0;                                                   \
        iter->start = true;                                                \
        iter->end = PFX##_empty(target);                                   \
                                                                           \
        iter->cursor = target->root;                                       \
        while (iter->cursor->left != NULL)                                 \
            iter->cursor = iter->cursor->left;                             \
                                                                           \
        iter->first = iter->cursor;                                        \
                                                                           \
        iter->last = target->root;                                         \
        while (iter->last->right != NULL)                                  \
            iter->last = iter->last->right;                                \
    }                                                                      \
                                                                           \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                         \
    {                                                                      \
        return PFX##_empty(iter->target) || iter->start;                   \
    }                                                                      \
                                                                           \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                           \
    {                                                                      \
        return PFX##_empty(iter->target) || iter->end;                     \
    }                                                                      \
                                                                           \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter)                       \
    {                                                                      \
        iter->index = 0;                                                   \
        iter->start = true;                                                \
        iter->end = PFX##_empty(iter->target);                             \
                                                                           \
        iter->cursor = iter->first;                                        \
    }                                                                      \
                                                                           \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter)                         \
    {                                                                      \
        iter->index = iter->target->count - 1;                             \
        iter->start = PFX##_empty(iter->target);                           \
        iter->end = true;                                                  \
                                                                           \
        iter->cursor = iter->last;                                         \
    }                                                                      \
                                                                           \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *value, size_t *index) \
    {                                                                      \
        if (iter->end)                                                     \
            return false;                                                  \
                                                                           \
        *value = iter->cursor->key;                                        \
        *index = iter->index;                                              \
        iter->start = false;                                               \
                                                                           \
        if (iter->cursor->right != NULL)                                   \
        {                                                                  \
            iter->cursor = iter->cursor->right;                            \
            while (iter->cursor->left != NULL)                             \
                iter->cursor = iter->cursor->left;                         \
                                                                           \
            iter->index++;                                                 \
                                                                           \
            return true;                                                   \
        }                                                                  \
                                                                           \
        while (true)                                                       \
        {                                                                  \
            if (iter->cursor == iter->last)                                \
            {                                                              \
                iter->end = true;                                          \
                                                                           \
                return true;                                               \
            }                                                              \
            if (iter->cursor->parent->left == iter->cursor)                \
            {                                                              \
                iter->cursor = iter->cursor->parent;                       \
                                                                           \
                iter->index++;                                             \
                                                                           \
                return true;                                               \
            }                                                              \
                                                                           \
            iter->cursor = iter->cursor->parent;                           \
        }                                                                  \
    }                                                                      \
                                                                           \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *value, size_t *index) \
    {                                                                      \
        if (iter->start)                                                   \
            return false;                                                  \
                                                                           \
        *value = iter->cursor->key;                                        \
        *index = iter->index;                                              \
        iter->end = false;                                                 \
                                                                           \
        if (iter->cursor->left != NULL)                                    \
        {                                                                  \
            iter->cursor = iter->cursor->left;                             \
            while (iter->cursor->right != NULL)                            \
                iter->cursor = iter->cursor->right;                        \
                                                                           \
            iter->index--;                                                 \
                                                                           \
            return true;                                                   \
        }                                                                  \
                                                                           \
        while (true)                                                       \
        {                                                                  \
            if (iter->cursor == iter->first)                               \
            {                                                              \
                iter->start = true;                                        \
                                                                           \
                return true;                                               \
            }                                                              \
            if (iter->cursor->parent->right == iter->cursor)               \
            {                                                              \
                iter->cursor = iter->cursor->parent;                       \
                                                                           \
                iter->index--;                                             \
                                                                           \
                return true;                                               \
            }                                                              \
                                                                           \
            iter->cursor = iter->cursor->parent;                           \
        }                                                                  \
    }                                                                      \
                                                                           \
    FMOD SNAME##_node *PFX##_new_node(V element)                           \
    {                                                                      \
        SNAME##_node *node = malloc(sizeof(SNAME##_node));                 \
                                                                           \
        if (!node)                                                         \
            return NULL;                                                   \
                                                                           \
        node->key = element;                                               \
        node->right = NULL;                                                \
        node->left = NULL;                                                 \
        node->parent = NULL;                                               \
        node->height = 0;                                                  \
                                                                           \
        return node;                                                       \
    }                                                                      \
                                                                           \
    FMOD SNAME##_node *PFX##_get_node(SNAME *_set_, V element)             \
    {                                                                      \
        if (PFX##_empty(_set_))                                            \
            return NULL;                                                   \
                                                                           \
        SNAME##_node *scan = _set_->root;                                  \
                                                                           \
        while (scan != NULL)                                               \
        {                                                                  \
            if (_set_->cmp(scan->key, element) > 0)                        \
                scan = scan->left;                                         \
            else if (_set_->cmp(scan->key, element) < 0)                   \
                scan = scan->right;                                        \
            else                                                           \
                return scan;                                               \
        }                                                                  \
                                                                           \
        return NULL;                                                       \
    }                                                                      \
                                                                           \
    FMOD unsigned char PFX##_h(SNAME##_node *node)                         \
    {                                                                      \
        if (node == NULL)                                                  \
            return 0;                                                      \
                                                                           \
        return node->height;                                               \
    }                                                                      \
                                                                           \
    FMOD unsigned char PFX##_hupdate(SNAME##_node *node)                   \
    {                                                                      \
        if (node == NULL)                                                  \
            return 0;                                                      \
                                                                           \
        unsigned char h_l = PFX##_h(node->left);                           \
        unsigned char h_r = PFX##_h(node->right);                          \
                                                                           \
        return 1 + (h_l > h_r ? h_l : h_r);                                \
    }                                                                      \
                                                                           \
    FMOD void PFX##_rotate_right(SNAME##_node **Z)                         \
    {                                                                      \
        SNAME##_node *root = *Z;                                           \
        SNAME##_node *new_root = root->left;                               \
                                                                           \
        if (root->parent != NULL)                                          \
        {                                                                  \
            if (root->parent->left == root)                                \
                root->parent->left = new_root;                             \
            else                                                           \
                root->parent->right = new_root;                            \
        }                                                                  \
                                                                           \
        new_root->parent = root->parent;                                   \
                                                                           \
        root->parent = new_root;                                           \
        root->left = new_root->right;                                      \
                                                                           \
        if (root->left)                                                    \
            root->left->parent = root;                                     \
                                                                           \
        new_root->right = root;                                            \
                                                                           \
        root->height = PFX##_hupdate(root);                                \
        new_root->height = PFX##_hupdate(new_root);                        \
                                                                           \
        *Z = new_root;                                                     \
    }                                                                      \
                                                                           \
    FMOD void PFX##_rotate_left(SNAME##_node **Z)                          \
    {                                                                      \
        SNAME##_node *root = *Z;                                           \
        SNAME##_node *new_root = root->right;                              \
                                                                           \
        if (root->parent != NULL)                                          \
        {                                                                  \
            if (root->parent->right == root)                               \
                root->parent->right = new_root;                            \
            else                                                           \
                root->parent->left = new_root;                             \
        }                                                                  \
                                                                           \
        new_root->parent = root->parent;                                   \
                                                                           \
        root->parent = new_root;                                           \
        root->right = new_root->left;                                      \
                                                                           \
        if (root->right)                                                   \
            root->right->parent = root;                                    \
                                                                           \
        new_root->left = root;                                             \
                                                                           \
        root->height = PFX##_hupdate(root);                                \
        new_root->height = PFX##_hupdate(new_root);                        \
                                                                           \
        *Z = new_root;                                                     \
    }                                                                      \
                                                                           \
    FMOD void PFX##_rebalance(SNAME *_set_, SNAME##_node *node)            \
    {                                                                      \
        SNAME##_node *scan = node, *child = NULL;                          \
                                                                           \
        int balance;                                                       \
        bool is_root = false;                                              \
                                                                           \
        while (scan != NULL)                                               \
        {                                                                  \
            if (scan->parent == NULL)                                      \
                is_root = true;                                            \
                                                                           \
            scan->height = PFX##_hupdate(scan);                            \
            balance = PFX##_h(scan->right) - PFX##_h(scan->left);          \
                                                                           \
            if (balance >= 2)                                              \
            {                                                              \
                child = scan->right;                                       \
                                                                           \
                if (PFX##_h(child->right) < PFX##_h(child->left))          \
                    PFX##_rotate_right(&(scan->right));                    \
                                                                           \
                PFX##_rotate_left(&scan);                                  \
            }                                                              \
            else if (balance <= -2)                                        \
            {                                                              \
                child = scan->left;                                        \
                                                                           \
                if (PFX##_h(child->left) < PFX##_h(child->right))          \
                    PFX##_rotate_left(&(scan->left));                      \
                                                                           \
                PFX##_rotate_right(&scan);                                 \
            }                                                              \
                                                                           \
            if (is_root)                                                   \
            {                                                              \
                _set_->root = scan;                                        \
                is_root = false;                                           \
            }                                                              \
                                                                           \
            scan = scan->parent;                                           \
        }                                                                  \
    }

#endif /* CMC_TREESET_H */
