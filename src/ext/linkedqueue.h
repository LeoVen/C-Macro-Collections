/**
 * linkedqueue.h
 *
 * Creation Date: 21/06/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * A node-based Queue using a Singly Linked List
 *
 * enqueue : is equivalent to a push_back from a LinkedList
 * dequeue : is equivalent to a pop_front from a LinkedList
 *
 * This Queue has a one-way iterator
 */

#ifndef CMC_LINKEDQUEUE_H
#define CMC_LINKEDQUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LINKEDQUEUE_GENERATE(PFX, SNAME, FMOD, V)    \
    LINKEDQUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V) \
    LINKEDQUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

#define LINKEDQUEUE_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V) \
    LINKEDQUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)

#define LINKEDQUEUE_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V) \
    LINKEDQUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)

/* HEADER ********************************************************************/
#define LINKEDQUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V)                           \
                                                                                   \
    /* Linked Queue Structure */                                                   \
    typedef struct SNAME##_s                                                       \
    {                                                                              \
        /* First node in the list */                                               \
        struct SNAME##_node_s *head;                                               \
                                                                                   \
        /* Last node in the list */                                                \
        struct SNAME##_node_s *tail;                                               \
                                                                                   \
        /* Current amount of elements in the list */                               \
        size_t count;                                                              \
                                                                                   \
        /* Function that returns an iterator to the start of the list */           \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                     \
                                                                                   \
    } SNAME, *SNAME##_ptr;                                                         \
                                                                                   \
    /* Singly-linked list node */                                                  \
    typedef struct SNAME##_node_s                                                  \
    {                                                                              \
        /* Node's data */                                                          \
        V data;                                                                    \
                                                                                   \
        /* Pointer to the next node on the linked list */                          \
        struct SNAME##_node_s *next;                                               \
                                                                                   \
    } SNAME##_node, *SNAME##_node_ptr;                                             \
                                                                                   \
    /* Linked Queue Iterator */                                                    \
    typedef struct SNAME##_iter_s                                                  \
    {                                                                              \
        /* Target Linked Queue */                                                  \
        struct SNAME##_s *target;                                                  \
                                                                                   \
        /* Cursor's current node */                                                \
        struct SNAME##_node_s *cursor;                                             \
                                                                                   \
        /* Keeps track of relative index to the iteration of elements */           \
        size_t index;                                                              \
                                                                                   \
        /* If the iterator is at the start of the iteration */                     \
        bool start;                                                                \
                                                                                   \
        /* If the iterator has reached the end of the iteration */                 \
        bool end;                                                                  \
                                                                                   \
    } SNAME##_iter, *SNAME##_iter_ptr;                                             \
                                                                                   \
    /* Collection Functions */                                                     \
    /* Collection Allocation and Deallocation */                                   \
    FMOD SNAME *PFX##_new(void);                                                   \
    FMOD void PFX##_clear(SNAME *_lqueue_);                                        \
    FMOD void PFX##_free(SNAME *_lqueue_);                                         \
    /* Collection Input and Output */                                              \
    FMOD bool PFX##_enqueue(SNAME *_lqueue_, V element);                           \
    FMOD bool PFX##_dequeue(SNAME *_lqueue_);                                      \
    /* Conditional Input and Output */                                             \
    FMOD bool PFX##_enqueue_if(SNAME *_lqueue_, V element, bool condition);        \
    FMOD bool PFX##_dequeue_if(SNAME *_lqueue_, bool condition);                   \
    /* Element Access */                                                           \
    FMOD V PFX##_peek(SNAME *_lqueue_);                                            \
    /* Collection State */                                                         \
    FMOD bool PFX##_contains(SNAME *_lqueue_, V element, int (*comparator)(V, V)); \
    FMOD bool PFX##_empty(SNAME *_lqueue_);                                        \
    FMOD size_t PFX##_count(SNAME *_lqueue_);                                      \
                                                                                   \
    /* Iterator Functions */                                                       \
    /* Iterator Allocation and Deallocation */                                     \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);                              \
    FMOD void PFX##_iter_free(SNAME##_iter *iter);                                 \
    /* Iterator Initialization */                                                  \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                  \
    /* Iterator State */                                                           \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                                \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                  \
    /* Iterator Movement */                                                        \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter);                             \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter);                               \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter);                                 \
    /* Iterator Access */                                                          \
    FMOD V PFX##_iter_value(SNAME##_iter *iter);                                   \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter);                                 \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter);                              \
                                                                                   \
    /* Default Value */                                                            \
    static inline V PFX##_impl_default_value(void)                                 \
    {                                                                              \
        V _empty_value_;                                                           \
                                                                                   \
        memset(&_empty_value_, 0, sizeof(V));                                      \
                                                                                   \
        return _empty_value_;                                                      \
    }                                                                              \
                                                                                   \
/* SOURCE ********************************************************************/
#define LINKEDQUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V)                          \
                                                                                  \
    /* Implementation Detail Functions */                                         \
    FMOD SNAME##_node *PFX##_impl_new_node(V element);                            \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_lqueue_);                     \
                                                                                  \
    FMOD SNAME *PFX##_new(void)                                                   \
    {                                                                             \
        SNAME *_lqueue_ = malloc(sizeof(SNAME));                                  \
                                                                                  \
        if (!_lqueue_)                                                            \
            return NULL;                                                          \
                                                                                  \
        _lqueue_->count = 0;                                                      \
        _lqueue_->head = NULL;                                                    \
        _lqueue_->tail = NULL;                                                    \
                                                                                  \
        _lqueue_->it_start = PFX##_impl_it_start;                                 \
                                                                                  \
        return _lqueue_;                                                          \
    }                                                                             \
                                                                                  \
    FMOD void PFX##_clear(SNAME *_lqueue_)                                        \
    {                                                                             \
        SNAME##_node *scan = _lqueue_->head;                                      \
                                                                                  \
        while (_lqueue_->head != NULL)                                            \
        {                                                                         \
            _lqueue_->head = _lqueue_->head->next;                                \
            free(scan);                                                           \
            scan = _lqueue_->head;                                                \
        }                                                                         \
                                                                                  \
        _lqueue_->count = 0;                                                      \
        _lqueue_->head = NULL;                                                    \
        _lqueue_->tail = NULL;                                                    \
    }                                                                             \
                                                                                  \
    FMOD void PFX##_free(SNAME *_lqueue_)                                         \
    {                                                                             \
        PFX##_clear(_lqueue_);                                                    \
                                                                                  \
        free(_lqueue_);                                                           \
    }                                                                             \
                                                                                  \
    FMOD bool PFX##_enqueue(SNAME *_lqueue_, V element)                           \
    {                                                                             \
        SNAME##_node *_node_ = PFX##_impl_new_node(element);                      \
                                                                                  \
        if (!_node_)                                                              \
            return false;                                                         \
                                                                                  \
        if (PFX##_empty(_lqueue_))                                                \
        {                                                                         \
            _lqueue_->head = _node_;                                              \
            _lqueue_->tail = _node_;                                              \
        }                                                                         \
        else                                                                      \
        {                                                                         \
            _lqueue_->tail->next = _node_;                                        \
            _lqueue_->tail = _node_;                                              \
        }                                                                         \
                                                                                  \
        _lqueue_->count++;                                                        \
                                                                                  \
        return true;                                                              \
    }                                                                             \
                                                                                  \
    FMOD bool PFX##_dequeue(SNAME *_lqueue_)                                      \
    {                                                                             \
        if (PFX##_empty(_lqueue_))                                                \
            return false;                                                         \
                                                                                  \
        SNAME##_node *_node_ = _lqueue_->head;                                    \
        _lqueue_->head = _lqueue_->head->next;                                    \
                                                                                  \
        free(_node_);                                                             \
                                                                                  \
        if (_lqueue_->head == NULL)                                               \
            _lqueue_->tail = NULL;                                                \
                                                                                  \
        _lqueue_->count--;                                                        \
                                                                                  \
        return true;                                                              \
    }                                                                             \
                                                                                  \
    FMOD bool PFX##_enqueue_if(SNAME *_lqueue_, V element, bool condition)        \
    {                                                                             \
        if (condition)                                                            \
            return PFX##_enqueue(_lqueue_, element);                              \
                                                                                  \
        return false;                                                             \
    }                                                                             \
                                                                                  \
    FMOD bool PFX##_dequeue_if(SNAME *_lqueue_, bool condition)                   \
    {                                                                             \
        if (condition)                                                            \
            return PFX##_dequeue(_lqueue_);                                       \
                                                                                  \
        return false;                                                             \
    }                                                                             \
                                                                                  \
    FMOD V PFX##_peek(SNAME *_lqueue_)                                            \
    {                                                                             \
        if (PFX##_empty(_lqueue_))                                                \
            return PFX##_impl_default_value();                                    \
                                                                                  \
        return _lqueue_->head->data;                                              \
    }                                                                             \
                                                                                  \
    FMOD bool PFX##_contains(SNAME *_lqueue_, V element, int (*comparator)(V, V)) \
    {                                                                             \
        SNAME##_node *scan = _lqueue_->head;                                      \
                                                                                  \
        while (scan != NULL)                                                      \
        {                                                                         \
            if (comparator(scan->data, element) == 0)                             \
                return true;                                                      \
                                                                                  \
            scan = scan->next;                                                    \
        }                                                                         \
                                                                                  \
        return false;                                                             \
    }                                                                             \
                                                                                  \
    FMOD bool PFX##_empty(SNAME *_lqueue_)                                        \
    {                                                                             \
        return _lqueue_->count == 0;                                              \
    }                                                                             \
                                                                                  \
    FMOD size_t PFX##_count(SNAME *_lqueue_)                                      \
    {                                                                             \
        return _lqueue_->count;                                                   \
    }                                                                             \
                                                                                  \
    FMOD SNAME##_iter *PFX##_iter_new(SNAME *target)                              \
    {                                                                             \
        SNAME##_iter *iter = malloc(sizeof(SNAME##_iter));                        \
                                                                                  \
        if (!iter)                                                                \
            return NULL;                                                          \
                                                                                  \
        PFX##_iter_init(iter, target);                                            \
                                                                                  \
        return iter;                                                              \
    }                                                                             \
                                                                                  \
    FMOD void PFX##_iter_free(SNAME##_iter *iter)                                 \
    {                                                                             \
        free(iter);                                                               \
    }                                                                             \
                                                                                  \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                  \
    {                                                                             \
        iter->target = target;                                                    \
        iter->cursor = target->head;                                              \
        iter->index = 0;                                                          \
        iter->start = true;                                                       \
        iter->end = PFX##_empty(target);                                          \
    }                                                                             \
                                                                                  \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                \
    {                                                                             \
        return PFX##_empty(iter->target) || iter->start;                          \
    }                                                                             \
                                                                                  \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                  \
    {                                                                             \
        return PFX##_empty(iter->target) || iter->end;                            \
    }                                                                             \
                                                                                  \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter)                             \
    {                                                                             \
        iter->cursor = iter->target->head;                                        \
        iter->index = 0;                                                          \
        iter->start = true;                                                       \
        iter->end = PFX##_empty(iter->target);                                    \
    }                                                                             \
                                                                                  \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter)                               \
    {                                                                             \
        iter->cursor = iter->target->tail;                                        \
        iter->index = iter->target->count - 1;                                    \
        iter->start = PFX##_empty(iter->target);                                  \
        iter->end = true;                                                         \
    }                                                                             \
                                                                                  \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter)                                 \
    {                                                                             \
        if (iter->end)                                                            \
            return false;                                                         \
                                                                                  \
        iter->start = PFX##_empty(iter->target);                                  \
                                                                                  \
        if (iter->cursor->next == NULL)                                           \
            iter->end = true;                                                     \
        else                                                                      \
        {                                                                         \
            iter->cursor = iter->cursor->next;                                    \
            iter->index++;                                                        \
        }                                                                         \
                                                                                  \
        return true;                                                              \
    }                                                                             \
                                                                                  \
    FMOD V PFX##_iter_value(SNAME##_iter *iter)                                   \
    {                                                                             \
        if (PFX##_empty(iter->target))                                            \
            return PFX##_impl_default_value();                                    \
                                                                                  \
        return iter->cursor->data;                                                \
    }                                                                             \
                                                                                  \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter)                                 \
    {                                                                             \
        if (PFX##_empty(iter->target))                                            \
            return NULL;                                                          \
                                                                                  \
        return &(iter->cursor->data);                                             \
    }                                                                             \
                                                                                  \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter)                              \
    {                                                                             \
        return iter->index;                                                       \
    }                                                                             \
                                                                                  \
    FMOD SNAME##_node *PFX##_impl_new_node(V element)                             \
    {                                                                             \
        SNAME##_node *_node_ = malloc(sizeof(SNAME##_node));                      \
                                                                                  \
        if (!_node_)                                                              \
            return NULL;                                                          \
                                                                                  \
        _node_->data = element;                                                   \
        _node_->next = NULL;                                                      \
                                                                                  \
        return _node_;                                                            \
    }                                                                             \
                                                                                  \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_lqueue_)                      \
    {                                                                             \
        SNAME##_iter iter;                                                        \
                                                                                  \
        PFX##_iter_init(&iter, _lqueue_);                                         \
        PFX##_iter_to_start(&iter);                                               \
                                                                                  \
        return iter;                                                              \
    }

#endif /* CMC_LINKEDQUEUE_H */
