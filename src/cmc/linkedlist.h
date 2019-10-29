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

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_linkedlist = "%s at %p { count:%" PRIuMAX ", head:%p, tail:%p }";

#define CMC_GENERATE_LINKEDLIST(PFX, SNAME, V)    \
    CMC_GENERATE_LINKEDLIST_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_LINKEDLIST_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_LINKEDLIST_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_LINKEDLIST_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_LINKEDLIST_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_LINKEDLIST_SOURCE(PFX, SNAME, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_LINKEDLIST_HEADER(PFX, SNAME, V)                                         \
                                                                                              \
    /* Linked List Structure */                                                               \
    struct SNAME                                                                              \
    {                                                                                         \
        /* First node in the list */                                                          \
        struct SNAME##_node *head;                                                            \
                                                                                              \
        /* Last node in the list */                                                           \
        struct SNAME##_node *tail;                                                            \
                                                                                              \
        /* Current amount of elements in the list */                                          \
        size_t count;                                                                         \
                                                                                              \
        /* Function that returns an iterator to the start of the list */                      \
        struct SNAME##_iter (*it_start)(struct SNAME *);                                      \
                                                                                              \
        /* Function that returns an iterator to the end of the list */                        \
        struct SNAME##_iter (*it_end)(struct SNAME *);                                        \
    };                                                                                        \
                                                                                              \
    /* Doubly-linked list node */                                                             \
    struct SNAME##_node                                                                       \
    {                                                                                         \
        /* Node's data */                                                                     \
        V data;                                                                               \
                                                                                              \
        /* Pointer to the next node on the linked list */                                     \
        struct SNAME##_node *next;                                                            \
                                                                                              \
        /* Pointer to the previous node on the linked list */                                 \
        struct SNAME##_node *prev;                                                            \
    };                                                                                        \
                                                                                              \
    /* Linked List Iterator */                                                                \
    struct SNAME##_iter                                                                       \
    {                                                                                         \
        /* Target Linked List */                                                              \
        struct SNAME *target;                                                                 \
                                                                                              \
        /* Cursor's current node */                                                           \
        struct SNAME##_node *cursor;                                                          \
                                                                                              \
        /* Keeps track of relative index to the iteration of elements */                      \
        size_t index;                                                                         \
                                                                                              \
        /* If the iterator has reached the start of the iteration */                          \
        bool start;                                                                           \
                                                                                              \
        /* If the iterator has reached the end of the iteration */                            \
        bool end;                                                                             \
    };                                                                                        \
                                                                                              \
    /* Collection Functions */                                                                \
    /* Collection Allocation and Deallocation */                                              \
    struct SNAME *PFX##_new(void);                                                            \
    void PFX##_clear(struct SNAME *_list_, void (*deallocator)(V));                           \
    void PFX##_free(struct SNAME *_list_, void (*deallocator)(V));                            \
    /* Collection Input and Output */                                                         \
    bool PFX##_push_front(struct SNAME *_list_, V element);                                   \
    bool PFX##_push_at(struct SNAME *_list_, V element, size_t index);                        \
    bool PFX##_push_back(struct SNAME *_list_, V element);                                    \
    bool PFX##_pop_front(struct SNAME *_list_);                                               \
    bool PFX##_pop_at(struct SNAME *_list_, size_t index);                                    \
    bool PFX##_pop_back(struct SNAME *_list_);                                                \
    /* Element Access */                                                                      \
    V PFX##_front(struct SNAME *_list_);                                                      \
    V PFX##_get(struct SNAME *_list_, size_t index);                                          \
    V *PFX##_get_ref(struct SNAME *_list_, size_t index);                                     \
    V PFX##_back(struct SNAME *_list_);                                                       \
    /* Collection State */                                                                    \
    bool PFX##_contains(struct SNAME *_list_, V element, int (*comparator)(V, V));            \
    bool PFX##_empty(struct SNAME *_list_);                                                   \
    size_t PFX##_count(struct SNAME *_list_);                                                 \
    /* Collection Utility */                                                                  \
    struct SNAME *PFX##_copy_of(struct SNAME *_list_, V (*copy_func)(V));                     \
    bool PFX##_equals(struct SNAME *_list1_, struct SNAME *_list2_, int (*comparator)(V, V)); \
    struct cmc_string PFX##_to_string(struct SNAME *_list_);                                  \
                                                                                              \
    /* Node Related Functions */                                                              \
    /* Node Allocation and Deallocation */                                                    \
    struct SNAME##_node *PFX##_new_node(V element);                                           \
    void PFX##_free_node(struct SNAME##_node *_node_);                                        \
    /* Node Access Relative to a Linked List */                                               \
    struct SNAME##_node *PFX##_head(struct SNAME *_list_);                                    \
    struct SNAME##_node *PFX##_get_node(struct SNAME *_list_, size_t index);                  \
    struct SNAME##_node *PFX##_tail(struct SNAME *_list_);                                    \
    /* Input and Output Relative to a Node */                                                 \
    bool PFX##_insert_after(struct SNAME *_owner_, struct SNAME##_node *_node_, V element);   \
    bool PFX##_insert_before(struct SNAME *_owner_, struct SNAME##_node *_node_, V element);  \
    bool PFX##_remove_after(struct SNAME *_owner_, struct SNAME##_node *_node_);              \
    bool PFX##_remove_current(struct SNAME *_owner_, struct SNAME##_node *_node_);            \
    bool PFX##_remove_before(struct SNAME *_owner_, struct SNAME##_node *_node_);             \
    /* Node Access Relative to a Linked List Node */                                          \
    struct SNAME##_node *PFX##_next_node(struct SNAME##_node *_node_);                        \
    struct SNAME##_node *PFX##_prev_node(struct SNAME##_node *_node_);                        \
                                                                                              \
    /* Iterator Functions */                                                                  \
    /* Iterator Allocation and Deallocation */                                                \
    struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);                                \
    void PFX##_iter_free(struct SNAME##_iter *iter);                                          \
    /* Iterator Initialization */                                                             \
    void PFX##_iter_init(struct SNAME##_iter *iter, struct SNAME *target);                    \
    /* Iterator State */                                                                      \
    bool PFX##_iter_start(struct SNAME##_iter *iter);                                         \
    bool PFX##_iter_end(struct SNAME##_iter *iter);                                           \
    /* Iterator Movement */                                                                   \
    void PFX##_iter_to_start(struct SNAME##_iter *iter);                                      \
    void PFX##_iter_to_end(struct SNAME##_iter *iter);                                        \
    bool PFX##_iter_next(struct SNAME##_iter *iter);                                          \
    bool PFX##_iter_prev(struct SNAME##_iter *iter);                                          \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps);                         \
    bool PFX##_iter_rewind(struct SNAME##_iter *iter, size_t steps);                          \
    bool PFX##_iter_go_to(struct SNAME##_iter *iter, size_t index);                           \
    /* Iterator Access */                                                                     \
    V PFX##_iter_value(struct SNAME##_iter *iter);                                            \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter);                                          \
    size_t PFX##_iter_index(struct SNAME##_iter *iter);                                       \
    struct SNAME##_node *PFX##_iter_node(struct SNAME##_iter *iter);                          \
                                                                                              \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_LINKEDLIST_SOURCE(PFX, SNAME, V)                                        \
                                                                                             \
    /* Implementation Detail Functions */                                                    \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_list_);                    \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_list_);                      \
                                                                                             \
    struct SNAME *PFX##_new(void)                                                            \
    {                                                                                        \
        struct SNAME *_list_ = malloc(sizeof(struct SNAME));                                 \
                                                                                             \
        if (!_list_)                                                                         \
            return NULL;                                                                     \
                                                                                             \
        _list_->count = 0;                                                                   \
        _list_->head = NULL;                                                                 \
        _list_->tail = NULL;                                                                 \
                                                                                             \
        _list_->it_start = PFX##_impl_it_start;                                              \
        _list_->it_end = PFX##_impl_it_end;                                                  \
                                                                                             \
        return _list_;                                                                       \
    }                                                                                        \
                                                                                             \
    void PFX##_clear(struct SNAME *_list_, void (*deallocator)(V))                           \
    {                                                                                        \
        struct SNAME##_node *scan = _list_->head;                                            \
                                                                                             \
        while (_list_->head != NULL)                                                         \
        {                                                                                    \
            _list_->head = _list_->head->next;                                               \
                                                                                             \
            if (deallocator)                                                                 \
            {                                                                                \
                deallocator(scan->data);                                                     \
            }                                                                                \
                                                                                             \
            free(scan);                                                                      \
                                                                                             \
            scan = _list_->head;                                                             \
        }                                                                                    \
                                                                                             \
        _list_->count = 0;                                                                   \
        _list_->head = NULL;                                                                 \
        _list_->tail = NULL;                                                                 \
    }                                                                                        \
                                                                                             \
    void PFX##_free(struct SNAME *_list_, void (*deallocator)(V))                            \
    {                                                                                        \
        PFX##_clear(_list_, deallocator);                                                    \
                                                                                             \
        free(_list_);                                                                        \
    }                                                                                        \
                                                                                             \
    bool PFX##_push_front(struct SNAME *_list_, V element)                                   \
    {                                                                                        \
        struct SNAME##_node *_node_ = PFX##_new_node(element);                               \
                                                                                             \
        if (!_node_)                                                                         \
            return false;                                                                    \
                                                                                             \
        if (PFX##_empty(_list_))                                                             \
        {                                                                                    \
            _list_->head = _node_;                                                           \
            _list_->tail = _node_;                                                           \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            _node_->next = _list_->head;                                                     \
            _list_->head->prev = _node_;                                                     \
            _list_->head = _node_;                                                           \
        }                                                                                    \
                                                                                             \
        _list_->count++;                                                                     \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_push_at(struct SNAME *_list_, V element, size_t index)                        \
    {                                                                                        \
        if (index > _list_->count)                                                           \
            return false;                                                                    \
                                                                                             \
        if (index == 0)                                                                      \
        {                                                                                    \
            return PFX##_push_front(_list_, element);                                        \
        }                                                                                    \
        else if (index == _list_->count)                                                     \
        {                                                                                    \
            return PFX##_push_back(_list_, element);                                         \
        }                                                                                    \
                                                                                             \
        struct SNAME##_node *_node_ = PFX##_new_node(element);                               \
                                                                                             \
        if (!_node_)                                                                         \
            return false;                                                                    \
                                                                                             \
        struct SNAME##_node *scan = PFX##_get_node(_list_, index - 1);                       \
                                                                                             \
        _node_->next = scan->next;                                                           \
        _node_->prev = scan;                                                                 \
        _node_->next->prev = _node_;                                                         \
        _node_->prev->next = _node_;                                                         \
                                                                                             \
        _list_->count++;                                                                     \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_push_back(struct SNAME *_list_, V element)                                    \
    {                                                                                        \
        struct SNAME##_node *_node_ = PFX##_new_node(element);                               \
                                                                                             \
        if (!_node_)                                                                         \
            return false;                                                                    \
                                                                                             \
        if (PFX##_empty(_list_))                                                             \
        {                                                                                    \
            _list_->head = _node_;                                                           \
            _list_->tail = _node_;                                                           \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            _node_->prev = _list_->tail;                                                     \
            _list_->tail->next = _node_;                                                     \
            _list_->tail = _node_;                                                           \
        }                                                                                    \
                                                                                             \
        _list_->count++;                                                                     \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_pop_front(struct SNAME *_list_)                                               \
    {                                                                                        \
        if (PFX##_empty(_list_))                                                             \
            return false;                                                                    \
                                                                                             \
        struct SNAME##_node *_node_ = _list_->head;                                          \
        _list_->head = _list_->head->next;                                                   \
                                                                                             \
        free(_node_);                                                                        \
                                                                                             \
        if (_list_->head == NULL)                                                            \
            _list_->tail = NULL;                                                             \
        else                                                                                 \
            _list_->head->prev = NULL;                                                       \
                                                                                             \
        _list_->count--;                                                                     \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_pop_at(struct SNAME *_list_, size_t index)                                    \
    {                                                                                        \
        if (PFX##_empty(_list_))                                                             \
            return false;                                                                    \
                                                                                             \
        if (index >= _list_->count)                                                          \
            return false;                                                                    \
                                                                                             \
        if (index == 0)                                                                      \
        {                                                                                    \
            return PFX##_pop_front(_list_);                                                  \
        }                                                                                    \
        else if (index == _list_->count - 1)                                                 \
        {                                                                                    \
            return PFX##_pop_back(_list_);                                                   \
        }                                                                                    \
                                                                                             \
        struct SNAME##_node *_node_ = PFX##_get_node(_list_, index);                         \
                                                                                             \
        if (!_node_)                                                                         \
            return false;                                                                    \
                                                                                             \
        _node_->next->prev = _node_->prev;                                                   \
        _node_->prev->next = _node_->next;                                                   \
                                                                                             \
        free(_node_);                                                                        \
                                                                                             \
        _list_->count--;                                                                     \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_pop_back(struct SNAME *_list_)                                                \
    {                                                                                        \
        if (PFX##_empty(_list_))                                                             \
            return false;                                                                    \
                                                                                             \
        struct SNAME##_node *_node_ = _list_->tail;                                          \
        _list_->tail = _list_->tail->prev;                                                   \
                                                                                             \
        free(_node_);                                                                        \
                                                                                             \
        if (_list_->tail == NULL)                                                            \
            _list_->head = NULL;                                                             \
        else                                                                                 \
            _list_->tail->next = NULL;                                                       \
                                                                                             \
        _list_->count--;                                                                     \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    V PFX##_front(struct SNAME *_list_)                                                      \
    {                                                                                        \
        if (PFX##_empty(_list_))                                                             \
            return (V){0};                                                                   \
                                                                                             \
        return _list_->head->data;                                                           \
    }                                                                                        \
                                                                                             \
    V PFX##_get(struct SNAME *_list_, size_t index)                                          \
    {                                                                                        \
        if (index >= _list_->count || PFX##_empty(_list_))                                   \
            return (V){0};                                                                   \
                                                                                             \
        struct SNAME##_node *scan = PFX##_get_node(_list_, index);                           \
                                                                                             \
        if (scan == NULL)                                                                    \
            return (V){0};                                                                   \
                                                                                             \
        return scan->data;                                                                   \
    }                                                                                        \
                                                                                             \
    V *PFX##_get_ref(struct SNAME *_list_, size_t index)                                     \
    {                                                                                        \
        if (index >= _list_->count)                                                          \
            return NULL;                                                                     \
                                                                                             \
        if (PFX##_empty(_list_))                                                             \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME##_node *scan = PFX##_get_node(_list_, index);                           \
                                                                                             \
        if (scan == NULL)                                                                    \
            return NULL;                                                                     \
                                                                                             \
        return &(scan->data);                                                                \
    }                                                                                        \
                                                                                             \
    V PFX##_back(struct SNAME *_list_)                                                       \
    {                                                                                        \
        if (PFX##_empty(_list_))                                                             \
            return (V){0};                                                                   \
                                                                                             \
        return _list_->tail->data;                                                           \
    }                                                                                        \
                                                                                             \
    bool PFX##_contains(struct SNAME *_list_, V element, int (*comparator)(V, V))            \
    {                                                                                        \
        struct SNAME##_node *scan = _list_->head;                                            \
                                                                                             \
        while (scan != NULL)                                                                 \
        {                                                                                    \
            if (comparator(scan->data, element) == 0)                                        \
                return true;                                                                 \
                                                                                             \
            scan = scan->next;                                                               \
        }                                                                                    \
                                                                                             \
        return false;                                                                        \
    }                                                                                        \
                                                                                             \
    bool PFX##_empty(struct SNAME *_list_)                                                   \
    {                                                                                        \
        return _list_->count == 0;                                                           \
    }                                                                                        \
                                                                                             \
    size_t PFX##_count(struct SNAME *_list_)                                                 \
    {                                                                                        \
        return _list_->count;                                                                \
    }                                                                                        \
                                                                                             \
    struct SNAME *PFX##_copy_of(struct SNAME *_list_, V (*copy_func)(V))                     \
    {                                                                                        \
        struct SNAME *result = PFX##_new();                                                  \
                                                                                             \
        if (!result)                                                                         \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME##_node *scan = _list_->head;                                            \
                                                                                             \
        while (scan != NULL)                                                                 \
        {                                                                                    \
            /* This allocation should never fail since it might not be */                    \
            /* possible to recover from it. That is why it isn't checked */                  \
            struct SNAME##_node *new_node;                                                   \
                                                                                             \
            if (copy_func)                                                                   \
                new_node = PFX##_new_node(copy_func(scan->data));                            \
            else                                                                             \
                new_node = PFX##_new_node(scan->data);                                       \
                                                                                             \
            if (!result->head)                                                               \
            {                                                                                \
                result->head = new_node;                                                     \
                result->tail = new_node;                                                     \
            }                                                                                \
            else                                                                             \
            {                                                                                \
                new_node->prev = result->tail;                                               \
                result->tail->next = new_node;                                               \
                result->tail = new_node;                                                     \
            }                                                                                \
                                                                                             \
            scan = scan->next;                                                               \
        }                                                                                    \
                                                                                             \
        result->count = _list_->count;                                                       \
                                                                                             \
        return result;                                                                       \
    }                                                                                        \
                                                                                             \
    bool PFX##_equals(struct SNAME *_list1_, struct SNAME *_list2_, int (*comparator)(V, V)) \
    {                                                                                        \
        if (PFX##_count(_list1_) != PFX##_count(_list2_))                                    \
            return false;                                                                    \
                                                                                             \
        struct SNAME##_node *scan1 = _list1_->head;                                          \
        struct SNAME##_node *scan2 = _list2_->head;                                          \
                                                                                             \
        while (scan1 != NULL && scan2 != NULL)                                               \
        {                                                                                    \
            if (comparator(scan1->data, scan2->data) != 0)                                   \
                return false;                                                                \
                                                                                             \
            scan1 = scan1->next;                                                             \
            scan2 = scan2->next;                                                             \
        }                                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    struct cmc_string PFX##_to_string(struct SNAME *_list_)                                  \
    {                                                                                        \
        struct cmc_string str;                                                               \
        struct SNAME *l_ = _list_;                                                           \
        const char *name = #SNAME;                                                           \
                                                                                             \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_linkedlist,                           \
                 name, l_, l_->count, l_->head, l_->tail);                                   \
                                                                                             \
        return str;                                                                          \
    }                                                                                        \
                                                                                             \
    struct SNAME##_node *PFX##_new_node(V element)                                           \
    {                                                                                        \
        struct SNAME##_node *_node_ = malloc(sizeof(struct SNAME##_node));                   \
                                                                                             \
        if (!_node_)                                                                         \
            return NULL;                                                                     \
                                                                                             \
        _node_->data = element;                                                              \
        _node_->next = NULL;                                                                 \
        _node_->prev = NULL;                                                                 \
                                                                                             \
        return _node_;                                                                       \
    }                                                                                        \
                                                                                             \
    void PFX##_free_node(struct SNAME##_node *_node_)                                        \
    {                                                                                        \
        free(_node_);                                                                        \
    }                                                                                        \
                                                                                             \
    struct SNAME##_node *PFX##_head(struct SNAME *_list_)                                    \
    {                                                                                        \
        return _list_->head;                                                                 \
    }                                                                                        \
                                                                                             \
    struct SNAME##_node *PFX##_get_node(struct SNAME *_list_, size_t index)                  \
    {                                                                                        \
        if (index >= _list_->count)                                                          \
            return NULL;                                                                     \
                                                                                             \
        if (PFX##_empty(_list_))                                                             \
            return NULL;                                                                     \
                                                                                             \
        struct SNAME##_node *_node_ = NULL;                                                  \
                                                                                             \
        if (index <= _list_->count / 2)                                                      \
        {                                                                                    \
            _node_ = _list_->head;                                                           \
            for (size_t i = 0; i < index; i++)                                               \
            {                                                                                \
                _node_ = _node_->next;                                                       \
            }                                                                                \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            _node_ = _list_->tail;                                                           \
            for (size_t i = _list_->count - 1; i > index; i--)                               \
            {                                                                                \
                _node_ = _node_->prev;                                                       \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        return _node_;                                                                       \
    }                                                                                        \
                                                                                             \
    struct SNAME##_node *PFX##_tail(struct SNAME *_list_)                                    \
    {                                                                                        \
        return _list_->tail;                                                                 \
    }                                                                                        \
                                                                                             \
    bool PFX##_insert_after(struct SNAME *_owner_, struct SNAME##_node *_node_, V element)   \
    {                                                                                        \
        struct SNAME##_node *new_node = PFX##_new_node(element);                             \
                                                                                             \
        if (!new_node)                                                                       \
            return false;                                                                    \
                                                                                             \
        new_node->next = _node_->next;                                                       \
                                                                                             \
        if (_node_->next != NULL)                                                            \
            _node_->next->prev = new_node;                                                   \
        else                                                                                 \
            _owner_->tail = new_node;                                                        \
                                                                                             \
        new_node->prev = _node_;                                                             \
        _node_->next = new_node;                                                             \
                                                                                             \
        _owner_->count++;                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_insert_before(struct SNAME *_owner_, struct SNAME##_node *_node_, V element)  \
    {                                                                                        \
        struct SNAME##_node *new_node = PFX##_new_node(element);                             \
                                                                                             \
        if (!new_node)                                                                       \
            return false;                                                                    \
                                                                                             \
        new_node->prev = _node_->prev;                                                       \
                                                                                             \
        if (_node_->prev != NULL)                                                            \
            _node_->prev->next = new_node;                                                   \
        else                                                                                 \
            _owner_->head = new_node;                                                        \
                                                                                             \
        new_node->next = _node_;                                                             \
        _node_->prev = new_node;                                                             \
                                                                                             \
        _owner_->count++;                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_remove_after(struct SNAME *_owner_, struct SNAME##_node *_node_)              \
    {                                                                                        \
        if (_node_->next == NULL)                                                            \
            return false;                                                                    \
                                                                                             \
        struct SNAME##_node *tmp = _node_->next;                                             \
                                                                                             \
        if (tmp->next != NULL)                                                               \
        {                                                                                    \
            _node_->next = _node_->next->next;                                               \
            _node_->next->prev = _node_;                                                     \
        }                                                                                    \
        else                                                                                 \
            _owner_->tail = _node_;                                                          \
                                                                                             \
        free(tmp);                                                                           \
                                                                                             \
        _owner_->count--;                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_remove_current(struct SNAME *_owner_, struct SNAME##_node *_node_)            \
    {                                                                                        \
        if (_node_->prev != NULL)                                                            \
            _node_->prev->next = _node_->next;                                               \
        else                                                                                 \
            _owner_->head = _node_->next;                                                    \
                                                                                             \
        if (_node_->next != NULL)                                                            \
            _node_->next->prev = _node_->prev;                                               \
        else                                                                                 \
            _owner_->tail = _node_->prev;                                                    \
                                                                                             \
        free(_node_);                                                                        \
                                                                                             \
        _owner_->count--;                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_remove_before(struct SNAME *_owner_, struct SNAME##_node *_node_)             \
    {                                                                                        \
        if (_node_->prev == NULL)                                                            \
            return false;                                                                    \
                                                                                             \
        struct SNAME##_node *tmp = _node_->prev;                                             \
                                                                                             \
        if (tmp->prev != NULL)                                                               \
        {                                                                                    \
            _node_->prev = _node_->prev->prev;                                               \
            _node_->prev->next = _node_;                                                     \
        }                                                                                    \
        else                                                                                 \
            _owner_->head = _node_;                                                          \
                                                                                             \
        free(tmp);                                                                           \
                                                                                             \
        _owner_->count--;                                                                    \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    struct SNAME##_node *PFX##_next_node(struct SNAME##_node *_node_)                        \
    {                                                                                        \
        return _node_->next;                                                                 \
    }                                                                                        \
                                                                                             \
    struct SNAME##_node *PFX##_prev_node(struct SNAME##_node *_node_)                        \
    {                                                                                        \
        return _node_->prev;                                                                 \
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
        iter->target = target;                                                               \
        iter->cursor = target->head;                                                         \
        iter->index = 0;                                                                     \
        iter->start = true;                                                                  \
        iter->end = PFX##_empty(target);                                                     \
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
            iter->cursor = iter->target->head;                                               \
            iter->index = 0;                                                                 \
            iter->start = true;                                                              \
            iter->end = PFX##_empty(iter->target);                                           \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    void PFX##_iter_to_end(struct SNAME##_iter *iter)                                        \
    {                                                                                        \
        if (!PFX##_empty(iter->target))                                                      \
        {                                                                                    \
            iter->cursor = iter->target->tail;                                               \
            iter->index = iter->target->count - 1;                                           \
            iter->start = PFX##_empty(iter->target);                                         \
            iter->end = true;                                                                \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    bool PFX##_iter_next(struct SNAME##_iter *iter)                                          \
    {                                                                                        \
        if (iter->end)                                                                       \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor->next == NULL)                                                      \
        {                                                                                    \
            iter->end = true;                                                                \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        iter->start = PFX##_empty(iter->target);                                             \
                                                                                             \
        iter->cursor = iter->cursor->next;                                                   \
        iter->index++;                                                                       \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    bool PFX##_iter_prev(struct SNAME##_iter *iter)                                          \
    {                                                                                        \
        if (iter->start)                                                                     \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor->prev == NULL)                                                      \
        {                                                                                    \
            iter->start = true;                                                              \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        iter->end = PFX##_empty(iter->target);                                               \
                                                                                             \
        iter->cursor = iter->cursor->prev;                                                   \
        iter->index--;                                                                       \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    /* Returns true only if the iterator moved */                                            \
    bool PFX##_iter_advance(struct SNAME##_iter *iter, size_t steps)                         \
    {                                                                                        \
        if (iter->end)                                                                       \
            return false;                                                                    \
                                                                                             \
        if (iter->cursor->next == NULL)                                                      \
        {                                                                                    \
            iter->end = true;                                                                \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        if (steps == 0 || iter->index + steps >= PFX##_count(iter->target))                  \
            return false;                                                                    \
                                                                                             \
        iter->start = PFX##_empty(iter->target);                                             \
                                                                                             \
        iter->index += steps;                                                                \
                                                                                             \
        for (size_t i = 0; i < steps; i++)                                                   \
            iter->cursor = iter->cursor->next;                                               \
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
        if (iter->cursor->prev == NULL)                                                      \
        {                                                                                    \
            iter->start = true;                                                              \
            return false;                                                                    \
        }                                                                                    \
                                                                                             \
        if (steps == 0 || iter->index < steps)                                               \
            return false;                                                                    \
                                                                                             \
        iter->end = PFX##_empty(iter->target);                                               \
                                                                                             \
        iter->index -= steps;                                                                \
                                                                                             \
        for (size_t i = 0; i < steps; i++)                                                   \
            iter->cursor = iter->cursor->prev;                                               \
                                                                                             \
        return true;                                                                         \
    }                                                                                        \
                                                                                             \
    /* Returns true only if the iterator was able to be positioned at the given */           \
    /* index */                                                                              \
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
        return iter->cursor->data;                                                           \
    }                                                                                        \
                                                                                             \
    V *PFX##_iter_rvalue(struct SNAME##_iter *iter)                                          \
    {                                                                                        \
        if (PFX##_empty(iter->target))                                                       \
            return NULL;                                                                     \
                                                                                             \
        return &(iter->cursor->data);                                                        \
    }                                                                                        \
                                                                                             \
    size_t PFX##_iter_index(struct SNAME##_iter *iter)                                       \
    {                                                                                        \
        return iter->index;                                                                  \
    }                                                                                        \
                                                                                             \
    struct SNAME##_node *PFX##_iter_node(struct SNAME##_iter *iter)                          \
    {                                                                                        \
        return iter->cursor;                                                                 \
    }                                                                                        \
                                                                                             \
    static struct SNAME##_iter PFX##_impl_it_start(struct SNAME *_list_)                     \
    {                                                                                        \
        struct SNAME##_iter iter;                                                            \
                                                                                             \
        PFX##_iter_init(&iter, _list_);                                                      \
        PFX##_iter_to_start(&iter);                                                          \
                                                                                             \
        return iter;                                                                         \
    }                                                                                        \
                                                                                             \
    static struct SNAME##_iter PFX##_impl_it_end(struct SNAME *_list_)                       \
    {                                                                                        \
        struct SNAME##_iter iter;                                                            \
                                                                                             \
        PFX##_iter_init(&iter, _list_);                                                      \
        PFX##_iter_to_end(&iter);                                                            \
                                                                                             \
        return iter;                                                                         \
    }

#endif /* CMC_LINKEDLIST_H */
