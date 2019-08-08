/**
 * linkedlist.h
 *
 * Creation Date: 22/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * LinkedList
 *
 * A LinkedList is a linear collection of elements, whose order is not given by
 * their physical placement in memory. Instead, each element points to the next
 * (singly-linked list) and to a previous one (doubly-linked list). It consists
 * of nodes connected by these next or previous pointers. Unlike an array, the
 * LinkedList can indefinitely grow.
 *
 * Implementation
 *
 * The LinkedList is implemented as a doubly-linked list and allows insertions
 * and removals at both ends in O(1) and in a given index in O(N). The list has
 * a head and tail pointer. The head points to the first element in the sequence
 * and tail points to the last.
 */

#ifndef CMC_LINKEDLIST_H
#define CMC_LINKEDLIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

#define LINKEDLIST_GENERATE(PFX, SNAME, FMOD, V)    \
    LINKEDLIST_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    LINKEDLIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

#define LINKEDLIST_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V) \
    LINKEDLIST_GENERATE_HEADER(PFX, SNAME, FMOD, V)

#define LINKEDLIST_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V) \
    LINKEDLIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* HEADER ********************************************************************/
#define LINKEDLIST_GENERATE_HEADER(PFX, SNAME, FMOD, V)                              \
                                                                                     \
    /* Linked List Structure */                                                      \
    typedef struct SNAME##_s                                                         \
    {                                                                                \
        /* First node in the list */                                                 \
        struct SNAME##_node_s *head;                                                 \
                                                                                     \
        /* Last node in the list */                                                  \
        struct SNAME##_node_s *tail;                                                 \
                                                                                     \
        /* Current amount of elements in the list */                                 \
        size_t count;                                                                \
                                                                                     \
        /* Function that returns an iterator to the start of the list */             \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                       \
                                                                                     \
        /* Function that returns an iterator to the end of the list */               \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                         \
                                                                                     \
    } SNAME, *SNAME##_ptr;                                                           \
                                                                                     \
    /* Doubly-linked list node */                                                    \
    typedef struct SNAME##_node_s                                                    \
    {                                                                                \
        /* Node's data */                                                            \
        V data;                                                                      \
                                                                                     \
        /* Pointer to the next node on the linked list */                            \
        struct SNAME##_node_s *next;                                                 \
                                                                                     \
        /* Pointer to the previous node on the linked list */                        \
        struct SNAME##_node_s *prev;                                                 \
                                                                                     \
    } SNAME##_node, *SNAME##_node_ptr;                                               \
                                                                                     \
    /* Linked List Iterator */                                                       \
    typedef struct SNAME##_iter_s                                                    \
    {                                                                                \
        /* Target Linked List */                                                     \
        struct SNAME##_s *target;                                                    \
                                                                                     \
        /* Cursor's current node */                                                  \
        struct SNAME##_node_s *cursor;                                               \
                                                                                     \
        /* Keeps track of relative index to the iteration of elements */             \
        size_t index;                                                                \
                                                                                     \
        /* If the iterator has reached the start of the iteration */                 \
        bool start;                                                                  \
                                                                                     \
        /* If the iterator has reached the end of the iteration */                   \
        bool end;                                                                    \
                                                                                     \
    } SNAME##_iter, *SNAME##_iter_ptr;                                               \
                                                                                     \
    /* Collection Functions */                                                       \
    /* Collection Allocation and Deallocation */                                     \
    FMOD SNAME *PFX##_new(void);                                                     \
    FMOD void PFX##_clear(SNAME *_list_);                                            \
    FMOD void PFX##_free(SNAME *_list_);                                             \
    /* Collection Input and Output */                                                \
    FMOD bool PFX##_push_front(SNAME *_list_, V element);                            \
    FMOD bool PFX##_push(SNAME *_list_, V element, size_t index);                    \
    FMOD bool PFX##_push_back(SNAME *_list_, V element);                             \
    FMOD bool PFX##_pop_front(SNAME *_list_);                                        \
    FMOD bool PFX##_pop(SNAME *_list_, size_t index);                                \
    FMOD bool PFX##_pop_back(SNAME *_list_);                                         \
    /* Conditional Input and Output */                                               \
    FMOD bool PFX##_push_if(SNAME *_list_, V element, size_t index, bool condition); \
    FMOD bool PFX##_pop_if(SNAME *_list_, size_t index, bool condition);             \
    /* Element Access */                                                             \
    FMOD V PFX##_front(SNAME *_list_);                                               \
    FMOD V PFX##_get(SNAME *_list_, size_t index);                                   \
    FMOD V *PFX##_get_ref(SNAME *_list_, size_t index);                              \
    FMOD V PFX##_back(SNAME *_list_);                                                \
    /* Collection State */                                                           \
    FMOD bool PFX##_contains(SNAME *_list_, V element, int (*comparator)(V, V));     \
    FMOD bool PFX##_empty(SNAME *_list_);                                            \
    FMOD size_t PFX##_count(SNAME *_list_);                                          \
    /* Collection Utility */                                                         \
    FMOD cmc_string PFX##_to_string(SNAME *_list_);                                  \
                                                                                     \
    /* Node Related Functions */                                                     \
    /* Node Allocation and Deallocation */                                           \
    FMOD SNAME##_node *PFX##_new_node(V element);                                    \
    FMOD void PFX##_free_node(SNAME##_node *_node_);                                 \
    /* Node Access Relative to a Linked List */                                      \
    FMOD SNAME##_node *PFX##_head(SNAME *_list_);                                    \
    FMOD SNAME##_node *PFX##_get_node(SNAME *_list_, size_t index);                  \
    FMOD SNAME##_node *PFX##_tail(SNAME *_list_);                                    \
    /* Input and Output Relative to a Node */                                        \
    FMOD bool PFX##_insert_after(SNAME *_owner_, SNAME##_node *_node_, V element);   \
    FMOD bool PFX##_insert_before(SNAME *_owner_, SNAME##_node *_node_, V element);  \
    FMOD bool PFX##_remove_after(SNAME *_owner_, SNAME##_node *_node_);              \
    FMOD bool PFX##_remove_current(SNAME *_owner_, SNAME##_node *_node_);            \
    FMOD bool PFX##_remove_before(SNAME *_owner_, SNAME##_node *_node_);             \
    /* Node Access Relative to a Linked List Node */                                 \
    FMOD SNAME##_node *PFX##_next_node(SNAME##_node *_node_);                        \
    FMOD SNAME##_node *PFX##_prev_node(SNAME##_node *_node_);                        \
                                                                                     \
    /* Iterator Functions */                                                         \
    /* Iterator Allocation and Deallocation */                                       \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);                                \
    FMOD void PFX##_iter_free(SNAME##_iter *iter);                                   \
    /* Iterator Initialization */                                                    \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                    \
    /* Iterator State */                                                             \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                  \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                    \
    /* Iterator Movement */                                                          \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter);                               \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter);                                 \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter);                                   \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter);                                   \
    /* Iterator Access */                                                            \
    FMOD V PFX##_iter_value(SNAME##_iter *iter);                                     \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter);                                   \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter);                                \
    FMOD SNAME##_node *PFX##_iter_node(SNAME##_iter *iter);                          \
                                                                                     \
    /* Default Value */                                                              \
    static inline V PFX##_impl_default_value(void)                                   \
    {                                                                                \
        V _empty_value_;                                                             \
                                                                                     \
        memset(&_empty_value_, 0, sizeof(V));                                        \
                                                                                     \
        return _empty_value_;                                                        \
    }                                                                                \
                                                                                     \
/* SOURCE ********************************************************************/
#define LINKEDLIST_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                             \
                                                                                    \
    /* Implementation Detail Functions */                                           \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_list_);                         \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_list_);                           \
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
        _list_->it_start = PFX##_impl_it_start;                                     \
        _list_->it_end = PFX##_impl_it_end;                                         \
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
        SNAME##_node *_node_ = PFX##_new_node(element);                             \
                                                                                    \
        if (!_node_)                                                                \
            return false;                                                           \
                                                                                    \
        if (PFX##_empty(_list_))                                                    \
        {                                                                           \
            _list_->head = _node_;                                                  \
            _list_->tail = _node_;                                                  \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            _node_->next = _list_->head;                                            \
            _list_->head->prev = _node_;                                            \
            _list_->head = _node_;                                                  \
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
        SNAME##_node *_node_ = PFX##_new_node(element);                             \
                                                                                    \
        if (!_node_)                                                                \
            return false;                                                           \
                                                                                    \
        SNAME##_node *scan = PFX##_get_node(_list_, index - 1);                     \
                                                                                    \
        _node_->next = scan->next;                                                  \
        _node_->prev = scan;                                                        \
        _node_->next->prev = _node_;                                                \
        _node_->prev->next = _node_;                                                \
                                                                                    \
        _list_->count++;                                                            \
                                                                                    \
        return true;                                                                \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_push_back(SNAME *_list_, V element)                             \
    {                                                                               \
        SNAME##_node *_node_ = PFX##_new_node(element);                             \
                                                                                    \
        if (!_node_)                                                                \
            return false;                                                           \
                                                                                    \
        if (PFX##_empty(_list_))                                                    \
        {                                                                           \
            _list_->head = _node_;                                                  \
            _list_->tail = _node_;                                                  \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            _node_->prev = _list_->tail;                                            \
            _list_->tail->next = _node_;                                            \
            _list_->tail = _node_;                                                  \
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
        SNAME##_node *_node_ = _list_->head;                                        \
        _list_->head = _list_->head->next;                                          \
                                                                                    \
        free(_node_);                                                               \
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
        SNAME##_node *_node_ = PFX##_get_node(_list_, index);                       \
                                                                                    \
        if (!_node_)                                                                \
            return false;                                                           \
                                                                                    \
        _node_->next->prev = _node_->prev;                                          \
        _node_->prev->next = _node_->next;                                          \
                                                                                    \
        free(_node_);                                                               \
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
        SNAME##_node *_node_ = _list_->tail;                                        \
        _list_->tail = _list_->tail->prev;                                          \
                                                                                    \
        free(_node_);                                                               \
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
            return PFX##_impl_default_value();                                      \
                                                                                    \
        return _list_->head->data;                                                  \
    }                                                                               \
                                                                                    \
    FMOD V PFX##_get(SNAME *_list_, size_t index)                                   \
    {                                                                               \
        if (index >= _list_->count || PFX##_empty(_list_))                          \
            return PFX##_impl_default_value();                                      \
                                                                                    \
        SNAME##_node *scan = PFX##_get_node(_list_, index);                         \
                                                                                    \
        if (scan == NULL)                                                           \
            return PFX##_impl_default_value();                                      \
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
            return PFX##_impl_default_value();                                      \
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
    FMOD cmc_string PFX##_to_string(SNAME *_list_)                                  \
    {                                                                               \
        cmc_string str;                                                             \
        SNAME *l_ = _list_;                                                         \
        const char *name = #SNAME;                                                  \
                                                                                    \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_linkedlist,                  \
                 name, l_, l_->count, l_->head, l_->tail);                          \
                                                                                    \
        return str;                                                                 \
    }                                                                               \
                                                                                    \
    FMOD SNAME##_node *PFX##_new_node(V element)                                    \
    {                                                                               \
        SNAME##_node *_node_ = malloc(sizeof(SNAME##_node));                        \
                                                                                    \
        if (!_node_)                                                                \
            return NULL;                                                            \
                                                                                    \
        _node_->data = element;                                                     \
        _node_->next = NULL;                                                        \
        _node_->prev = NULL;                                                        \
                                                                                    \
        return _node_;                                                              \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_free_node(SNAME##_node *_node_)                                 \
    {                                                                               \
        free(_node_);                                                               \
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
        SNAME##_node *_node_ = NULL;                                                \
                                                                                    \
        if (index <= _list_->count / 2)                                             \
        {                                                                           \
            _node_ = _list_->head;                                                  \
            for (size_t i = 0; i < index; i++)                                      \
            {                                                                       \
                _node_ = _node_->next;                                              \
            }                                                                       \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            _node_ = _list_->tail;                                                  \
            for (size_t i = _list_->count - 1; i > index; i--)                      \
            {                                                                       \
                _node_ = _node_->prev;                                              \
            }                                                                       \
        }                                                                           \
                                                                                    \
        return _node_;                                                              \
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
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter)                               \
    {                                                                               \
        iter->cursor = iter->target->head;                                          \
        iter->index = 0;                                                            \
        iter->start = true;                                                         \
        iter->end = PFX##_empty(iter->target);                                      \
    }                                                                               \
                                                                                    \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter)                                 \
    {                                                                               \
        iter->cursor = iter->target->tail;                                          \
        iter->index = iter->target->count - 1;                                      \
        iter->start = PFX##_empty(iter->target);                                    \
        iter->end = true;                                                           \
    }                                                                               \
                                                                                    \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter)                                   \
    {                                                                               \
        if (iter->end)                                                              \
            return false;                                                           \
                                                                                    \
        iter->start = PFX##_empty(iter->target);                                    \
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
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter)                                   \
    {                                                                               \
        if (iter->start)                                                            \
            return false;                                                           \
                                                                                    \
        iter->end = PFX##_empty(iter->target);                                      \
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
    }                                                                               \
                                                                                    \
    FMOD V PFX##_iter_value(SNAME##_iter *iter)                                     \
    {                                                                               \
        if (PFX##_empty(iter->target))                                              \
            return PFX##_impl_default_value();                                      \
                                                                                    \
        return iter->cursor->data;                                                  \
    }                                                                               \
                                                                                    \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter)                                   \
    {                                                                               \
        if (PFX##_empty(iter->target))                                              \
            return NULL;                                                            \
                                                                                    \
        return &(iter->cursor->data);                                               \
    }                                                                               \
                                                                                    \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter)                                \
    {                                                                               \
        return iter->index;                                                         \
    }                                                                               \
                                                                                    \
    FMOD SNAME##_node *PFX##_iter_node(SNAME##_iter *iter)                          \
    {                                                                               \
        return iter->cursor;                                                        \
    }                                                                               \
                                                                                    \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_list_)                          \
    {                                                                               \
        SNAME##_iter iter;                                                          \
                                                                                    \
        PFX##_iter_init(&iter, _list_);                                             \
        PFX##_iter_to_start(&iter);                                                 \
                                                                                    \
        return iter;                                                                \
    }                                                                               \
                                                                                    \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_list_)                            \
    {                                                                               \
        SNAME##_iter iter;                                                          \
                                                                                    \
        PFX##_iter_init(&iter, _list_);                                             \
        PFX##_iter_to_end(&iter);                                                   \
                                                                                    \
        return iter;                                                                \
    }

#endif /* CMC_LINKEDLIST_H */
