/**
 * linkedlist.h
 *
 * Creation Date: 22/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/*****************************************************************************/
/**************************************************************** LINKEDLIST */
/*****************************************************************************/

#ifndef CMC_LINKEDLIST_H
#define CMC_LINKEDLIST_H

#include <stdlib.h>
#include <stdbool.h>

#define LINKEDLIST_GENERATE(PFX, SNAME, FMOD, V)    \
    LINKEDLIST_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
    LINKEDLIST_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    LINKEDLIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* PRIVATE *******************************************************************/
#define LINKEDLIST_GENERATE_HEADER_PRIVATE(PFX, SNAME, FMOD, K, V) \
    LINKEDLIST_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define LINKEDLIST_GENERATE_SOURCE_PRIVATE(PFX, SNAME, FMOD, K, V) \
    LINKEDLIST_GENERATE_STRUCT(PFX, SNAME, FMOD, V)                \
    LINKEDLIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* PUBLIC ********************************************************************/
#define LINKEDLIST_GENERATE_HEADER_PUBLIC(PFX, SNAME, FMOD, K, V) \
    LINKEDLIST_GENERATE_STRUCT(PFX, SNAME, FMOD, V)               \
    LINKEDLIST_GENERATE_HEADER(PFX, SNAME, FMOD, V)
#define LINKEDLIST_GENERATE_SOURCE_PUBLIC(PFX, SNAME, FMOD, K, V) \
    LINKEDLIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)
/* STRUCT ********************************************************************/
#define LINKEDLIST_GENERATE_STRUCT(PFX, SNAME, FMOD, V) \
                                                        \
    struct SNAME##_s                                    \
    {                                                   \
        struct SNAME##_node_s *head;                    \
        struct SNAME##_node_s *tail;                    \
        size_t count;                                   \
    };                                                  \
                                                        \
    struct SNAME##_node_s                               \
    {                                                   \
        V data;                                         \
        struct SNAME##_node_s *next;                    \
        struct SNAME##_node_s *prev;                    \
    };                                                  \
                                                        \
    struct SNAME##_iter_s                               \
    {                                                   \
        struct SNAME##_s *target;                       \
        struct SNAME##_node_s *cursor;                  \
        size_t index;                                   \
        bool start;                                     \
        bool end;                                       \
    };                                                  \
                                                        \
/* HEADER ********************************************************************/
#define LINKEDLIST_GENERATE_HEADER(PFX, SNAME, FMOD, V)                              \
                                                                                     \
    typedef struct SNAME##_s SNAME;                                                  \
    typedef struct SNAME##_node_s SNAME##_node;                                      \
    typedef struct SNAME##_iter_s SNAME##_iter;                                      \
                                                                                     \
    FMOD SNAME *PFX##_new(void);                                                     \
    FMOD void PFX##_clear(SNAME *_list_);                                            \
    FMOD void PFX##_free(SNAME *_list_);                                             \
    FMOD bool PFX##_push_front(SNAME *_list_, V element);                            \
    FMOD bool PFX##_push(SNAME *_list_, V element, size_t index);                    \
    FMOD bool PFX##_push_back(SNAME *_list_, V element);                             \
    FMOD bool PFX##_pop_front(SNAME *_list_);                                        \
    FMOD bool PFX##_pop(SNAME *_list_, size_t index);                                \
    FMOD bool PFX##_pop_back(SNAME *_list_);                                         \
    FMOD bool PFX##_push_if(SNAME *_list_, V element, size_t index, bool condition); \
    FMOD bool PFX##_pop_if(SNAME *_list_, size_t index, bool condition);             \
    FMOD V PFX##_front(SNAME *_list_);                                               \
    FMOD V PFX##_get(SNAME *_list_, size_t index);                                   \
    FMOD V *PFX##_get_ref(SNAME *_list_, size_t index);                              \
    FMOD V PFX##_back(SNAME *_list_);                                                \
    FMOD bool PFX##_contains(SNAME *_list_, V element, int (*comparator)(V, V));     \
    FMOD bool PFX##_empty(SNAME *_list_);                                            \
    FMOD size_t PFX##_count(SNAME *_list_);                                          \
                                                                                     \
    FMOD SNAME##_node *PFX##_new_node(V element);                                    \
    FMOD SNAME##_node *PFX##_head(SNAME *_list_);                                    \
    FMOD SNAME##_node *PFX##_get_node(SNAME *_list_, size_t index);                  \
    FMOD SNAME##_node *PFX##_tail(SNAME *_list_);                                    \
    FMOD bool PFX##_insert_after(SNAME *_owner_, SNAME##_node *_node_, V element);   \
    FMOD bool PFX##_insert_before(SNAME *_owner_, SNAME##_node *_node_, V element);  \
    FMOD bool PFX##_remove_after(SNAME *_owner_, SNAME##_node *_node_);              \
    FMOD bool PFX##_remove_current(SNAME *_owner_, SNAME##_node *_node_);            \
    FMOD bool PFX##_remove_before(SNAME *_owner_, SNAME##_node *_node_);             \
    FMOD SNAME##_node *PFX##_next_node(SNAME##_node *_node_);                        \
    FMOD SNAME##_node *PFX##_prev_node(SNAME##_node *_node_);                        \
                                                                                     \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);                                \
    FMOD void PFX##_iter_free(SNAME##_iter *iter);                                   \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                    \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                  \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                    \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter);                                \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter);                                  \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index);         \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index);         \
                                                                                     \
/* SOURCE ********************************************************************/
#define LINKEDLIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                             \
                                                                                    \
    FMOD SNAME *PFX##_new(void)                                                     \
    {                                                                               \
        SNAME *_list_ = malloc(sizeof(SNAME));                                      \
                                                                                    \
        if (!_list_)                                                                \
            return NULL;                                                            \
                                                                                    \
        _list_->count = 0;                                                          \
        _list_->head = NULL;                                                        \
        _list_->tail = NULL;                                                        \
                                                                                    \
        return _list_;                                                              \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_clear(SNAME *_list_)                                            \
    {                                                                               \
        SNAME##_node *scan = _list_->head;                                          \
                                                                                    \
        while (_list_->head != NULL)                                                \
        {                                                                           \
            _list_->head = _list_->head->next;                                      \
            free(scan);                                                             \
            scan = _list_->head;                                                    \
        }                                                                           \
                                                                                    \
        _list_->count = 0;                                                          \
        _list_->head = NULL;                                                        \
        _list_->tail = NULL;                                                        \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_free(SNAME *_list_)                                             \
    {                                                                               \
        PFX##_clear(_list_);                                                        \
                                                                                    \
        free(_list_);                                                               \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_push_front(SNAME *_list_, V element)                            \
    {                                                                               \
        SNAME##_node *node = PFX##_new_node(element);                               \
                                                                                    \
        if (!node)                                                                  \
            return false;                                                           \
                                                                                    \
        if (PFX##_empty(_list_))                                                    \
        {                                                                           \
            _list_->head = node;                                                    \
            _list_->tail = node;                                                    \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            node->next = _list_->head;                                              \
            _list_->head->prev = node;                                              \
            _list_->head = node;                                                    \
        }                                                                           \
                                                                                    \
        _list_->count++;                                                            \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_push(SNAME *_list_, V element, size_t index)                    \
    {                                                                               \
        if (index > _list_->count)                                                  \
            return false;                                                           \
                                                                                    \
        if (index == 0)                                                             \
        {                                                                           \
            return PFX##_push_front(_list_, element);                               \
        }                                                                           \
        else if (index == _list_->count)                                            \
        {                                                                           \
            return PFX##_push_back(_list_, element);                                \
        }                                                                           \
                                                                                    \
        SNAME##_node *node = PFX##_new_node(element);                               \
                                                                                    \
        if (!node)                                                                  \
            return false;                                                           \
                                                                                    \
        SNAME##_node *scan = PFX##_get_node(_list_, index - 1);                     \
                                                                                    \
        node->next = scan->next;                                                    \
        node->prev = scan;                                                          \
        node->next->prev = node;                                                    \
        node->prev->next = node;                                                    \
                                                                                    \
        _list_->count++;                                                            \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_push_back(SNAME *_list_, V element)                             \
    {                                                                               \
        SNAME##_node *node = PFX##_new_node(element);                               \
                                                                                    \
        if (!node)                                                                  \
            return false;                                                           \
                                                                                    \
        if (PFX##_empty(_list_))                                                    \
        {                                                                           \
            _list_->head = node;                                                    \
            _list_->tail = node;                                                    \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            node->prev = _list_->tail;                                              \
            _list_->tail->next = node;                                              \
            _list_->tail = node;                                                    \
        }                                                                           \
                                                                                    \
        _list_->count++;                                                            \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_pop_front(SNAME *_list_)                                        \
    {                                                                               \
        if (PFX##_empty(_list_))                                                    \
            return false;                                                           \
                                                                                    \
        SNAME##_node *node = _list_->head;                                          \
        _list_->head = _list_->head->next;                                          \
                                                                                    \
        free(node);                                                                 \
                                                                                    \
        if (_list_->head == NULL)                                                   \
            _list_->tail = NULL;                                                    \
        else                                                                        \
            _list_->head->prev = NULL;                                              \
                                                                                    \
        _list_->count--;                                                            \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_pop(SNAME *_list_, size_t index)                                \
    {                                                                               \
        if (PFX##_empty(_list_))                                                    \
            return false;                                                           \
                                                                                    \
        if (index >= _list_->count)                                                 \
            return false;                                                           \
                                                                                    \
        if (index == 0)                                                             \
        {                                                                           \
            return PFX##_pop_front(_list_);                                         \
        }                                                                           \
        else if (index == _list_->count - 1)                                        \
        {                                                                           \
            return PFX##_pop_back(_list_);                                          \
        }                                                                           \
                                                                                    \
        SNAME##_node *node = PFX##_get_node(_list_, index);                         \
                                                                                    \
        if (!node)                                                                  \
            return false;                                                           \
                                                                                    \
        node->next->prev = node->prev;                                              \
        node->prev->next = node->next;                                              \
                                                                                    \
        free(node);                                                                 \
                                                                                    \
        _list_->count--;                                                            \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_pop_back(SNAME *_list_)                                         \
    {                                                                               \
        if (PFX##_empty(_list_))                                                    \
            return false;                                                           \
                                                                                    \
        SNAME##_node *node = _list_->tail;                                          \
        _list_->tail = _list_->tail->prev;                                          \
                                                                                    \
        free(node);                                                                 \
                                                                                    \
        if (_list_->tail == NULL)                                                   \
            _list_->head = NULL;                                                    \
        else                                                                        \
            _list_->tail->next = NULL;                                              \
                                                                                    \
        _list_->count--;                                                            \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_push_if(SNAME *_list_, V element, size_t index, bool condition) \
    {                                                                               \
        if (condition)                                                              \
            return PFX##_push(_list_, element, index);                              \
                                                                                    \
        return false;                                                               \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_pop_if(SNAME *_list_, size_t index, bool condition)             \
    {                                                                               \
        if (condition)                                                              \
            return PFX##_pop(_list_, index);                                        \
                                                                                    \
        return false;                                                               \
    }                                                                               \
                                                                                    \
    FMOD V PFX##_front(SNAME *_list_)                                               \
    {                                                                               \
        if (PFX##_empty(_list_))                                                    \
            return 0;                                                               \
                                                                                    \
        return _list_->head->data;                                                  \
    }                                                                               \
                                                                                    \
    FMOD V PFX##_get(SNAME *_list_, size_t index)                                   \
    {                                                                               \
        if (index >= _list_->count)                                                 \
            return 0;                                                               \
                                                                                    \
        if (PFX##_empty(_list_))                                                    \
            return 0;                                                               \
                                                                                    \
        SNAME##_node *scan = PFX##_get_node(_list_, index);                         \
                                                                                    \
        if (scan == NULL)                                                           \
            return 0;                                                               \
                                                                                    \
        return scan->data;                                                          \
    }                                                                               \
                                                                                    \
    FMOD V *PFX##_get_ref(SNAME *_list_, size_t index)                              \
    {                                                                               \
        if (index >= _list_->count)                                                 \
            return NULL;                                                            \
                                                                                    \
        if (PFX##_empty(_list_))                                                    \
            return NULL;                                                            \
                                                                                    \
        SNAME##_node *scan = PFX##_get_node(_list_, index);                         \
                                                                                    \
        if (scan == NULL)                                                           \
            return NULL;                                                            \
                                                                                    \
        return &(scan->data);                                                       \
    }                                                                               \
                                                                                    \
    FMOD V PFX##_back(SNAME *_list_)                                                \
    {                                                                               \
        if (PFX##_empty(_list_))                                                    \
            return 0;                                                               \
                                                                                    \
        return _list_->tail->data;                                                  \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_contains(SNAME *_list_, V element, int (*comparator)(V, V))     \
    {                                                                               \
        SNAME##_node *scan = _list_->head;                                          \
                                                                                    \
        while (scan != NULL)                                                        \
        {                                                                           \
            if (comparator(scan->data, element) == 0)                               \
                return true;                                                        \
                                                                                    \
            scan = scan->next;                                                      \
        }                                                                           \
                                                                                    \
        return false;                                                               \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_empty(SNAME *_list_)                                            \
    {                                                                               \
        return _list_->count == 0;                                                  \
    }                                                                               \
                                                                                    \
    FMOD size_t PFX##_count(SNAME *_list_)                                          \
    {                                                                               \
        return _list_->count;                                                       \
    }                                                                               \
                                                                                    \
    FMOD SNAME##_node *PFX##_new_node(V element)                                    \
    {                                                                               \
        SNAME##_node *node = malloc(sizeof(SNAME##_node));                          \
                                                                                    \
        if (!node)                                                                  \
            return NULL;                                                            \
                                                                                    \
        node->data = element;                                                       \
        node->next = NULL;                                                          \
        node->prev = NULL;                                                          \
                                                                                    \
        return node;                                                                \
    }                                                                               \
                                                                                    \
    FMOD SNAME##_node *PFX##_head(SNAME *_list_)                                    \
    {                                                                               \
        return _list_->head;                                                        \
    }                                                                               \
                                                                                    \
    FMOD SNAME##_node *PFX##_get_node(SNAME *_list_, size_t index)                  \
    {                                                                               \
        if (index >= _list_->count)                                                 \
            return NULL;                                                            \
                                                                                    \
        if (PFX##_empty(_list_))                                                    \
            return NULL;                                                            \
                                                                                    \
        SNAME##_node *scan = NULL;                                                  \
                                                                                    \
        if (index <= _list_->count / 2)                                             \
        {                                                                           \
            scan = _list_->head;                                                    \
            for (size_t i = 0; i < index; i++)                                      \
            {                                                                       \
                scan = scan->next;                                                  \
            }                                                                       \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            scan = _list_->tail;                                                    \
            for (size_t i = _list_->count - 1; i > index; i--)                      \
            {                                                                       \
                scan = scan->prev;                                                  \
            }                                                                       \
        }                                                                           \
                                                                                    \
        return scan;                                                                \
    }                                                                               \
                                                                                    \
    FMOD SNAME##_node *PFX##_tail(SNAME *_list_)                                    \
    {                                                                               \
        return _list_->tail;                                                        \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_insert_after(SNAME *_owner_, SNAME##_node *_node_, V element)   \
    {                                                                               \
        SNAME##_node *new_node = PFX##_new_node(element);                           \
                                                                                    \
        if (!new_node)                                                              \
            return false;                                                           \
                                                                                    \
        new_node->next = _node_->next;                                              \
                                                                                    \
        if (_node_->next != NULL)                                                   \
            _node_->next->prev = new_node;                                          \
        else                                                                        \
            _owner_->tail = new_node;                                               \
                                                                                    \
        new_node->prev = _node_;                                                    \
        _node_->next = new_node;                                                    \
                                                                                    \
        _owner_->count++;                                                           \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_insert_before(SNAME *_owner_, SNAME##_node *_node_, V element)  \
    {                                                                               \
        SNAME##_node *new_node = PFX##_new_node(element);                           \
                                                                                    \
        if (!new_node)                                                              \
            return false;                                                           \
                                                                                    \
        new_node->prev = _node_->prev;                                              \
                                                                                    \
        if (_node_->prev != NULL)                                                   \
            _node_->prev->next = new_node;                                          \
        else                                                                        \
            _owner_->head = new_node;                                               \
                                                                                    \
        new_node->next = _node_;                                                    \
        _node_->prev = new_node;                                                    \
                                                                                    \
        _owner_->count++;                                                           \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_remove_after(SNAME *_owner_, SNAME##_node *_node_)              \
    {                                                                               \
        if (_node_->next == NULL)                                                   \
            return false;                                                           \
                                                                                    \
        SNAME##_node *tmp = _node_->next;                                           \
                                                                                    \
        if (tmp->next != NULL)                                                      \
        {                                                                           \
            _node_->next = _node_->next->next;                                      \
            _node_->next->prev = _node_;                                            \
        }                                                                           \
        else                                                                        \
            _owner_->tail = _node_;                                                 \
                                                                                    \
        free(tmp);                                                                  \
                                                                                    \
        _owner_->count--;                                                           \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_remove_current(SNAME *_owner_, SNAME##_node *_node_)            \
    {                                                                               \
        if (_node_->prev != NULL)                                                   \
            _node_->prev->next = _node_->next;                                      \
        else                                                                        \
            _owner_->head = _node_->next;                                           \
                                                                                    \
        if (_node_->next != NULL)                                                   \
            _node_->next->prev = _node_->prev;                                      \
        else                                                                        \
            _owner_->tail = _node_->prev;                                           \
                                                                                    \
        free(_node_);                                                               \
                                                                                    \
        _owner_->count--;                                                           \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_remove_before(SNAME *_owner_, SNAME##_node *_node_)             \
    {                                                                               \
        if (_node_->prev == NULL)                                                   \
            return false;                                                           \
                                                                                    \
        SNAME##_node *tmp = _node_->prev;                                           \
                                                                                    \
        if (tmp->prev != NULL)                                                      \
        {                                                                           \
            _node_->prev = _node_->prev->prev;                                      \
            _node_->prev->next = _node_;                                            \
        }                                                                           \
        else                                                                        \
            _owner_->head = _node_;                                                 \
                                                                                    \
        free(tmp);                                                                  \
                                                                                    \
        _owner_->count--;                                                           \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD SNAME##_node *PFX##_next_node(SNAME##_node *_node_)                        \
    {                                                                               \
        return _node_->next;                                                        \
    }                                                                               \
                                                                                    \
    FMOD SNAME##_node *PFX##_prev_node(SNAME##_node *_node_)                        \
    {                                                                               \
        return _node_->prev;                                                        \
    }                                                                               \
                                                                                    \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target)                                \
    {                                                                               \
        SNAME##_iter *iter = malloc(sizeof(SNAME##_iter));                          \
                                                                                    \
        if (!iter)                                                                  \
            return NULL;                                                            \
                                                                                    \
        PFX##_iter_init(iter, target);                                              \
                                                                                    \
        return iter;                                                                \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_iter_free(SNAME##_iter *iter)                                   \
    {                                                                               \
        free(iter);                                                                 \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                    \
    {                                                                               \
        iter->target = target;                                                      \
        iter->cursor = target->head;                                                \
        iter->index = 0;                                                            \
        iter->start = true;                                                         \
        iter->end = PFX##_empty(target);                                            \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                  \
    {                                                                               \
        return PFX##_empty(iter->target) || iter->start;                            \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                    \
    {                                                                               \
        return PFX##_empty(iter->target) || iter->end;                              \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_iter_tostart(SNAME##_iter *iter)                                \
    {                                                                               \
        iter->cursor = iter->target->head;                                          \
        iter->index = 0;                                                            \
        iter->start = true;                                                         \
        iter->end = PFX##_empty(iter->target);                                      \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_iter_toend(SNAME##_iter *iter)                                  \
    {                                                                               \
        iter->cursor = iter->target->tail;                                          \
        iter->index = iter->target->count - 1;                                      \
        iter->start = PFX##_empty(iter->target);                                    \
        iter->end = true;                                                           \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index)         \
    {                                                                               \
        if (iter->end)                                                              \
            return false;                                                           \
                                                                                    \
        *index = iter->index;                                                       \
        *result = iter->cursor->data;                                               \
        iter->start = false;                                                        \
                                                                                    \
        if (iter->cursor->next == NULL)                                             \
            iter->end = true;                                                       \
        else                                                                        \
        {                                                                           \
            iter->cursor = iter->cursor->next;                                      \
            iter->index++;                                                          \
        }                                                                           \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index)         \
    {                                                                               \
        if (iter->start)                                                            \
            return false;                                                           \
                                                                                    \
        *index = iter->index;                                                       \
        *result = iter->cursor->data;                                               \
        iter->end = false;                                                          \
                                                                                    \
        if (iter->cursor->prev == NULL)                                             \
            iter->start = true;                                                     \
        else                                                                        \
        {                                                                           \
            iter->cursor = iter->cursor->prev;                                      \
            iter->index--;                                                          \
        }                                                                           \
                                                                                    \
        return true;                                                                \
    }

#endif /* CMC_LINKEDLIST_H */
