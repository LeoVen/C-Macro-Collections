/**
 * cmc/linkedlist.h
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

#ifndef CMC_CMC_LINKEDLIST_H
#define CMC_CMC_LINKEDLIST_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * LinkedList specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_cmc_string_fmt_linkedlist = "struct %s<%s> "
                                                   "at %p { "
                                                   "count:%" PRIuMAX ", "
                                                   "head:%p, "
                                                   "tail:%p, "
                                                   "flag:%d, "
                                                   "f_val:%p, "
                                                   "alloc:%p, "
                                                   "callbacks:%p }";

/**
 * Core LinkedList implementation
 */
#define CMC_CMC_LINKEDLIST_CORE(PARAMS)    \
    CMC_CMC_LINKEDLIST_CORE_HEADER(PARAMS) \
    CMC_CMC_LINKEDLIST_CORE_SOURCE(PARAMS)

#define CMC_CMC_LINKEDLIST_CORE_HEADER(PARAMS) \
    CMC_CMC_LINKEDLIST_CORE_HEADER_(           \
        CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_LINKEDLIST_CORE_SOURCE(PARAMS) \
    CMC_CMC_LINKEDLIST_CORE_SOURCE_(           \
        CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_LINKEDLIST_CORE_HEADER_(PFX, SNAME, V)                        \
                                                                              \
    /* Linked List Structure */                                               \
    struct SNAME                                                              \
    {                                                                         \
        /* First node in the list */                                          \
        struct CMC_DEF_NODE(SNAME) * head;                                    \
                                                                              \
        /* Last node in the list */                                           \
        struct CMC_DEF_NODE(SNAME) * tail;                                    \
                                                                              \
        /* Current amount of elements in the list */                          \
        size_t count;                                                         \
                                                                              \
        /* Flags indicating errors or success */                              \
        int flag;                                                             \
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
    /* Doubly-linked list node */                                             \
    struct CMC_DEF_NODE(SNAME)                                                \
    {                                                                         \
        /* Node's value */                                                    \
        V value;                                                              \
                                                                              \
        /* Pointer to the next node on the linked list */                     \
        struct CMC_DEF_NODE(SNAME) * next;                                    \
                                                                              \
        /* Pointer to the previous node on the linked list */                 \
        struct CMC_DEF_NODE(SNAME) * prev;                                    \
    };                                                                        \
                                                                              \
    /* Value struct function table */                                         \
    struct CMC_DEF_FVAL(SNAME)                                                \
    {                                                                         \
        /* Comparator function */                                             \
        CMC_DEF_FTAB_CMP(V);                                                  \
                                                                              \
        /* Copy function */                                                   \
        CMC_DEF_FTAB_CPY(V);                                                  \
                                                                              \
        /* To string function */                                              \
        CMC_DEF_FTAB_STR(V);                                                  \
                                                                              \
        /* Free from memory function */                                       \
        CMC_DEF_FTAB_FREE(V);                                                 \
                                                                              \
        /* Hash function */                                                   \
        CMC_DEF_FTAB_HASH(V);                                                 \
                                                                              \
        /* Priority function */                                               \
        CMC_DEF_FTAB_PRI(V);                                                  \
    };                                                                        \
                                                                              \
    /* Linked List Iterator */                                                \
    struct CMC_DEF_ITER(SNAME)                                                \
    {                                                                         \
        /* Target Linked List */                                              \
        struct SNAME *target;                                                 \
                                                                              \
        /* Cursor's current node */                                           \
        struct CMC_DEF_NODE(SNAME) * cursor;                                  \
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
    struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FVAL(SNAME) * f_val);        \
    struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FVAL(SNAME) * f_val,  \
                                         struct cmc_alloc_node * alloc,       \
                                         struct cmc_callbacks * callbacks);   \
    void CMC_(PFX, _clear)(struct SNAME * _list_);                            \
    void CMC_(PFX, _free)(struct SNAME * _list_);                             \
    /* Customization of Allocation and Callbacks */                           \
    void CMC_(PFX, _customize)(struct SNAME * _list_,                         \
                               struct cmc_alloc_node * alloc,                 \
                               struct cmc_callbacks * callbacks);             \
    /* Collection Input and Output */                                         \
    bool CMC_(PFX, _push_front)(struct SNAME * _list_, V value);              \
    bool CMC_(PFX, _push_at)(struct SNAME * _list_, V value, size_t index);   \
    bool CMC_(PFX, _push_back)(struct SNAME * _list_, V value);               \
    bool CMC_(PFX, _pop_front)(struct SNAME * _list_);                        \
    bool CMC_(PFX, _pop_at)(struct SNAME * _list_, size_t index);             \
    bool CMC_(PFX, _pop_back)(struct SNAME * _list_);                         \
    /* Element Access */                                                      \
    V CMC_(PFX, _front)(struct SNAME * _list_);                               \
    V CMC_(PFX, _get)(struct SNAME * _list_, size_t index);                   \
    V *CMC_(PFX, _get_ref)(struct SNAME * _list_, size_t index);              \
    V CMC_(PFX, _back)(struct SNAME * _list_);                                \
    /* Collection State */                                                    \
    bool CMC_(PFX, _contains)(struct SNAME * _list_, V value);                \
    bool CMC_(PFX, _empty)(struct SNAME * _list_);                            \
    size_t CMC_(PFX, _count)(struct SNAME * _list_);                          \
    int CMC_(PFX, _flag)(struct SNAME * _list_);                              \
    /* Collection Utility */                                                  \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _list_);                 \
    bool CMC_(PFX, _equals)(struct SNAME * _list1_, struct SNAME * _list2_);  \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _list_);           \
    bool CMC_(PFX, _print)(struct SNAME * _list_, FILE * fptr);               \
                                                                              \
    /* Node Related Functions */                                              \
    /* Node Allocation and Deallocation */                                    \
    struct CMC_DEF_NODE(SNAME) *                                              \
        CMC_(PFX, _new_node)(struct SNAME * _list_, V value);                 \
    void CMC_(PFX, _free_node)(struct SNAME * _list_,                         \
                               struct CMC_DEF_NODE(SNAME) * _node_);          \
    /* Node Access Relative to a Linked List */                               \
    struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _head)(struct SNAME * _list_);     \
    struct CMC_DEF_NODE(SNAME) *                                              \
        CMC_(PFX, _get_node)(struct SNAME * _list_, size_t index);            \
    struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _tail)(struct SNAME * _list_);     \
    /* Input and Output Relative to a Node */                                 \
    bool CMC_(PFX, _add_next)(struct SNAME * _owner_,                         \
                              struct CMC_DEF_NODE(SNAME) * _node_, V value);  \
    bool CMC_(PFX, _add_prev)(struct SNAME * _owner_,                         \
                              struct CMC_DEF_NODE(SNAME) * _node_, V value);  \
    bool CMC_(PFX, _del_next)(struct SNAME * _owner_,                         \
                              struct CMC_DEF_NODE(SNAME) * _node_);           \
    bool CMC_(PFX, _del_curr)(struct SNAME * _owner_,                         \
                              struct CMC_DEF_NODE(SNAME) * _node_);           \
    bool CMC_(PFX, _del_prev)(struct SNAME * _owner_,                         \
                              struct CMC_DEF_NODE(SNAME) * _node_);           \
    /* Node Access Relative to a Linked List Node */                          \
    struct CMC_DEF_NODE(SNAME) *                                              \
        CMC_(PFX, _next_node)(struct CMC_DEF_NODE(SNAME) * _node_);           \
    struct CMC_DEF_NODE(SNAME) *                                              \
        CMC_(PFX, _prev_node)(struct CMC_DEF_NODE(SNAME) * _node_);           \
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
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter);              \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter);            \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);         \
    struct CMC_DEF_NODE(SNAME) *                                              \
        CMC_(PFX, _iter_node)(struct CMC_DEF_ITER(SNAME) * iter);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_LINKEDLIST_CORE_SOURCE_(PFX, SNAME, V)                         \
                                                                               \
    /* Implementation Detail Functions */                                      \
    /* None */                                                                 \
                                                                               \
    struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FVAL(SNAME) * f_val)          \
    {                                                                          \
        if (!f_val)                                                            \
            return NULL;                                                       \
                                                                               \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;                \
                                                                               \
        struct SNAME *_list_ = alloc->malloc(sizeof(struct SNAME));            \
                                                                               \
        if (!_list_)                                                           \
            return NULL;                                                       \
                                                                               \
        _list_->count = 0;                                                     \
        _list_->head = NULL;                                                   \
        _list_->tail = NULL;                                                   \
        _list_->flag = CMC_FLAG_OK;                                            \
        _list_->f_val = f_val;                                                 \
        _list_->alloc = alloc;                                                 \
        _list_->callbacks = NULL;                                              \
                                                                               \
        return _list_;                                                         \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FVAL(SNAME) * f_val,   \
                                         struct cmc_alloc_node * alloc,        \
                                         struct cmc_callbacks * callbacks)     \
    {                                                                          \
        if (!f_val)                                                            \
            return NULL;                                                       \
                                                                               \
        if (!alloc)                                                            \
            alloc = &cmc_alloc_node_default;                                   \
                                                                               \
        struct SNAME *_list_ = alloc->malloc(sizeof(struct SNAME));            \
                                                                               \
        if (!_list_)                                                           \
            return NULL;                                                       \
                                                                               \
        _list_->count = 0;                                                     \
        _list_->head = NULL;                                                   \
        _list_->tail = NULL;                                                   \
        _list_->flag = CMC_FLAG_OK;                                            \
        _list_->f_val = f_val;                                                 \
        _list_->alloc = alloc;                                                 \
        _list_->callbacks = callbacks;                                         \
                                                                               \
        return _list_;                                                         \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _clear)(struct SNAME * _list_)                              \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *scan = _list_->head;                       \
                                                                               \
        while (_list_->head != NULL)                                           \
        {                                                                      \
            _list_->head = _list_->head->next;                                 \
                                                                               \
            if (_list_->f_val->free)                                           \
            {                                                                  \
                _list_->f_val->free(scan->value);                              \
            }                                                                  \
                                                                               \
            _list_->alloc->free(scan);                                         \
                                                                               \
            scan = _list_->head;                                               \
        }                                                                      \
                                                                               \
        _list_->count = 0;                                                     \
        _list_->head = NULL;                                                   \
        _list_->tail = NULL;                                                   \
        _list_->flag = CMC_FLAG_OK;                                            \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _free)(struct SNAME * _list_)                               \
    {                                                                          \
        CMC_(PFX, _clear)(_list_);                                             \
                                                                               \
        _list_->alloc->free(_list_);                                           \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _customize)(struct SNAME * _list_,                          \
                               struct cmc_alloc_node * alloc,                  \
                               struct cmc_callbacks * callbacks)               \
    {                                                                          \
        if (!alloc)                                                            \
            _list_->alloc = &cmc_alloc_node_default;                           \
        else                                                                   \
            _list_->alloc = alloc;                                             \
                                                                               \
        _list_->callbacks = callbacks;                                         \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _push_front)(struct SNAME * _list_, V value)                \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *_node_ =                                   \
            CMC_(PFX, _new_node)(_list_, value);                               \
                                                                               \
        if (!_node_)                                                           \
            return false;                                                      \
                                                                               \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->head = _node_;                                             \
            _list_->tail = _node_;                                             \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            _node_->next = _list_->head;                                       \
            _list_->head->prev = _node_;                                       \
            _list_->head = _node_;                                             \
        }                                                                      \
                                                                               \
        _list_->count++;                                                       \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->create)                    \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _push_at)(struct SNAME * _list_, V value, size_t index)     \
    {                                                                          \
        if (index > _list_->count)                                             \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (index == 0)                                                        \
        {                                                                      \
            return CMC_(PFX, _push_front)(_list_, value);                      \
        }                                                                      \
        else if (index == _list_->count)                                       \
        {                                                                      \
            return CMC_(PFX, _push_back)(_list_, value);                       \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *_node_ =                                   \
            CMC_(PFX, _new_node)(_list_, value);                               \
                                                                               \
        if (!_node_)                                                           \
            return false;                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *scan =                                     \
            CMC_(PFX, _get_node)(_list_, index - 1);                           \
                                                                               \
        _node_->next = scan->next;                                             \
        _node_->prev = scan;                                                   \
        _node_->next->prev = _node_;                                           \
        _node_->prev->next = _node_;                                           \
                                                                               \
        _list_->count++;                                                       \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->create)                    \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _push_back)(struct SNAME * _list_, V value)                 \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *_node_ =                                   \
            CMC_(PFX, _new_node)(_list_, value);                               \
                                                                               \
        if (!_node_)                                                           \
            return false;                                                      \
                                                                               \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->head = _node_;                                             \
            _list_->tail = _node_;                                             \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            _node_->prev = _list_->tail;                                       \
            _list_->tail->next = _node_;                                       \
            _list_->tail = _node_;                                             \
        }                                                                      \
                                                                               \
        _list_->count++;                                                       \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->create)                    \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _pop_front)(struct SNAME * _list_)                          \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *_node_ = _list_->head;                     \
        _list_->head = _list_->head->next;                                     \
                                                                               \
        _list_->alloc->free(_node_);                                           \
                                                                               \
        if (_list_->head == NULL)                                              \
            _list_->tail = NULL;                                               \
        else                                                                   \
            _list_->head->prev = NULL;                                         \
                                                                               \
        _list_->count--;                                                       \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->delete)                    \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _pop_at)(struct SNAME * _list_, size_t index)               \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (index >= _list_->count)                                            \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (index == 0)                                                        \
        {                                                                      \
            return CMC_(PFX, _pop_front)(_list_);                              \
        }                                                                      \
        else if (index == _list_->count - 1)                                   \
        {                                                                      \
            return CMC_(PFX, _pop_back)(_list_);                               \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *_node_ =                                   \
            CMC_(PFX, _get_node)(_list_, index);                               \
                                                                               \
        if (!_node_)                                                           \
            return false;                                                      \
                                                                               \
        _node_->next->prev = _node_->prev;                                     \
        _node_->prev->next = _node_->next;                                     \
                                                                               \
        _list_->alloc->free(_node_);                                           \
                                                                               \
        _list_->count--;                                                       \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->delete)                    \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _pop_back)(struct SNAME * _list_)                           \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *_node_ = _list_->tail;                     \
        _list_->tail = _list_->tail->prev;                                     \
                                                                               \
        _list_->alloc->free(_node_);                                           \
                                                                               \
        if (_list_->tail == NULL)                                              \
            _list_->head = NULL;                                               \
        else                                                                   \
            _list_->tail->next = NULL;                                         \
                                                                               \
        _list_->count--;                                                       \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->delete)                    \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _front)(struct SNAME * _list_)                                 \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return _list_->head->value;                                            \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _get)(struct SNAME * _list_, size_t index)                     \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        if (index >= _list_->count)                                            \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *scan =                                     \
            CMC_(PFX, _get_node)(_list_, index);                               \
                                                                               \
        if (scan == NULL)                                                      \
            return (V){ 0 };                                                   \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return scan->value;                                                    \
    }                                                                          \
                                                                               \
    V *CMC_(PFX, _get_ref)(struct SNAME * _list_, size_t index)                \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (index >= _list_->count)                                            \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *scan =                                     \
            CMC_(PFX, _get_node)(_list_, index);                               \
                                                                               \
        if (scan == NULL)                                                      \
            return NULL;                                                       \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return &(scan->value);                                                 \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _back)(struct SNAME * _list_)                                  \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return _list_->tail->value;                                            \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _contains)(struct SNAME * _list_, V value)                  \
    {                                                                          \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        bool result = false;                                                   \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *scan = _list_->head;                       \
                                                                               \
        while (scan != NULL)                                                   \
        {                                                                      \
            if (_list_->f_val->cmp(scan->value, value) == 0)                   \
            {                                                                  \
                result = true;                                                 \
                break;                                                         \
            }                                                                  \
                                                                               \
            scan = scan->next;                                                 \
        }                                                                      \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _empty)(struct SNAME * _list_)                              \
    {                                                                          \
        return _list_->count == 0;                                             \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _count)(struct SNAME * _list_)                            \
    {                                                                          \
        return _list_->count;                                                  \
    }                                                                          \
                                                                               \
    int CMC_(PFX, _flag)(struct SNAME * _list_)                                \
    {                                                                          \
        return _list_->flag;                                                   \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _list_)                   \
    {                                                                          \
        struct SNAME *result = CMC_(PFX, _new_custom)(                         \
            _list_->f_val, _list_->alloc, _list_->callbacks);                  \
                                                                               \
        if (!result)                                                           \
            return NULL;                                                       \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *scan = _list_->head;                       \
                                                                               \
        while (scan != NULL)                                                   \
        {                                                                      \
            /* This allocation should never fail since it might not be */      \
            /* possible to recover from it. That is why it isn't checked */    \
            struct CMC_DEF_NODE(SNAME) * new_node;                             \
                                                                               \
            if (_list_->f_val->cpy)                                            \
                new_node = CMC_(PFX, _new_node)(                               \
                    _list_, _list_->f_val->cpy(scan->value));                  \
            else                                                               \
                new_node = CMC_(PFX, _new_node)(_list_, scan->value);          \
                                                                               \
            if (!result->head)                                                 \
            {                                                                  \
                result->head = new_node;                                       \
                result->tail = new_node;                                       \
            }                                                                  \
            else                                                               \
            {                                                                  \
                new_node->prev = result->tail;                                 \
                result->tail->next = new_node;                                 \
                result->tail = new_node;                                       \
            }                                                                  \
                                                                               \
            scan = scan->next;                                                 \
        }                                                                      \
                                                                               \
        result->count = _list_->count;                                         \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _equals)(struct SNAME * _list1_, struct SNAME * _list2_)    \
    {                                                                          \
        _list1_->flag = CMC_FLAG_OK;                                           \
        _list2_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_list1_->count != _list2_->count)                                  \
            return false;                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *scan1 = _list1_->head;                     \
        struct CMC_DEF_NODE(SNAME) *scan2 = _list2_->head;                     \
                                                                               \
        while (scan1 != NULL && scan2 != NULL)                                 \
        {                                                                      \
            if (_list1_->f_val->cmp(scan1->value, scan2->value) != 0)          \
                return false;                                                  \
                                                                               \
            scan1 = scan1->next;                                               \
            scan2 = scan2->next;                                               \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _list_)             \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *l_ = _list_;                                             \
                                                                               \
        int n = snprintf(str.s, cmc_string_len, cmc_cmc_string_fmt_linkedlist, \
                         #SNAME, #V, l_, l_->count, l_->head, l_->tail,        \
                         l_->flag, l_->f_val, l_->alloc, l_->callbacks);       \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _print)(struct SNAME * _list_, FILE * fptr)                 \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *scan = _list_->head;                       \
                                                                               \
        while (scan != NULL)                                                   \
        {                                                                      \
            if (!_list_->f_val->str(fptr, scan->value))                        \
                return false;                                                  \
                                                                               \
            scan = scan->next;                                                 \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_NODE(SNAME) *                                               \
        CMC_(PFX, _new_node)(struct SNAME * _list_, V value)                   \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *_node_ =                                   \
            _list_->alloc->malloc(sizeof(struct CMC_DEF_NODE(SNAME)));         \
                                                                               \
        if (!_node_)                                                           \
        {                                                                      \
            _list_->flag = CMC_FLAG_ALLOC;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _node_->value = value;                                                 \
        _node_->next = NULL;                                                   \
        _node_->prev = NULL;                                                   \
                                                                               \
        return _node_;                                                         \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _free_node)(struct SNAME * _list_,                          \
                               struct CMC_DEF_NODE(SNAME) * _node_)            \
    {                                                                          \
        _list_->alloc->free(_node_);                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _head)(struct SNAME * _list_)       \
    {                                                                          \
        return _list_->head;                                                   \
    }                                                                          \
                                                                               \
    struct CMC_DEF_NODE(SNAME) *                                               \
        CMC_(PFX, _get_node)(struct SNAME * _list_, size_t index)              \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (index >= _list_->count)                                            \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *_node_ = NULL;                             \
                                                                               \
        if (index <= _list_->count / 2)                                        \
        {                                                                      \
            _node_ = _list_->head;                                             \
            for (size_t i = 0; i < index; i++)                                 \
            {                                                                  \
                _node_ = _node_->next;                                         \
            }                                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            _node_ = _list_->tail;                                             \
            for (size_t i = _list_->count - 1; i > index; i--)                 \
            {                                                                  \
                _node_ = _node_->prev;                                         \
            }                                                                  \
        }                                                                      \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        return _node_;                                                         \
    }                                                                          \
                                                                               \
    struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _tail)(struct SNAME * _list_)       \
    {                                                                          \
        return _list_->tail;                                                   \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _add_next)(struct SNAME * _owner_,                          \
                              struct CMC_DEF_NODE(SNAME) * _node_, V value)    \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *new_node =                                 \
            CMC_(PFX, _new_node)(_owner_, value);                              \
                                                                               \
        if (!new_node)                                                         \
            return false;                                                      \
                                                                               \
        new_node->next = _node_->next;                                         \
                                                                               \
        if (_node_->next != NULL)                                              \
            _node_->next->prev = new_node;                                     \
        else                                                                   \
            _owner_->tail = new_node;                                          \
                                                                               \
        new_node->prev = _node_;                                               \
        _node_->next = new_node;                                               \
                                                                               \
        _owner_->count++;                                                      \
        _owner_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _add_prev)(struct SNAME * _owner_,                          \
                              struct CMC_DEF_NODE(SNAME) * _node_, V value)    \
    {                                                                          \
        struct CMC_DEF_NODE(SNAME) *new_node =                                 \
            CMC_(PFX, _new_node)(_owner_, value);                              \
                                                                               \
        if (!new_node)                                                         \
            return false;                                                      \
                                                                               \
        new_node->prev = _node_->prev;                                         \
                                                                               \
        if (_node_->prev != NULL)                                              \
            _node_->prev->next = new_node;                                     \
        else                                                                   \
            _owner_->head = new_node;                                          \
                                                                               \
        new_node->next = _node_;                                               \
        _node_->prev = new_node;                                               \
                                                                               \
        _owner_->count++;                                                      \
        _owner_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _del_next)(struct SNAME * _owner_,                          \
                              struct CMC_DEF_NODE(SNAME) * _node_)             \
    {                                                                          \
        if (_node_->next == NULL)                                              \
        {                                                                      \
            _owner_->flag = CMC_FLAG_INVALID;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *tmp = _node_->next;                        \
                                                                               \
        _node_->next = _node_->next->next;                                     \
                                                                               \
        if (tmp->next != NULL)                                                 \
            tmp->next->prev = _node_;                                          \
        else                                                                   \
            _owner_->tail = _node_;                                            \
                                                                               \
        _owner_->alloc->free(tmp);                                             \
                                                                               \
        _owner_->count--;                                                      \
        _owner_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _del_curr)(struct SNAME * _owner_,                          \
                              struct CMC_DEF_NODE(SNAME) * _node_)             \
    {                                                                          \
        if (_node_->prev != NULL)                                              \
            _node_->prev->next = _node_->next;                                 \
        else                                                                   \
            _owner_->head = _node_->next;                                      \
                                                                               \
        if (_node_->next != NULL)                                              \
            _node_->next->prev = _node_->prev;                                 \
        else                                                                   \
            _owner_->tail = _node_->prev;                                      \
                                                                               \
        _owner_->alloc->free(_node_);                                          \
                                                                               \
        _owner_->count--;                                                      \
        _owner_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _del_prev)(struct SNAME * _owner_,                          \
                              struct CMC_DEF_NODE(SNAME) * _node_)             \
    {                                                                          \
        if (_node_->prev == NULL)                                              \
        {                                                                      \
            _owner_->flag = CMC_FLAG_INVALID;                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        struct CMC_DEF_NODE(SNAME) *tmp = _node_->prev;                        \
                                                                               \
        _node_->prev = _node_->prev->prev;                                     \
                                                                               \
        if (tmp->prev != NULL)                                                 \
            tmp->prev->next = _node_;                                          \
        else                                                                   \
            _owner_->head = _node_;                                            \
                                                                               \
        _owner_->alloc->free(tmp);                                             \
                                                                               \
        _owner_->count--;                                                      \
        _owner_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_NODE(SNAME) *                                               \
        CMC_(PFX, _next_node)(struct CMC_DEF_NODE(SNAME) * _node_)             \
    {                                                                          \
        return _node_->next;                                                   \
    }                                                                          \
                                                                               \
    struct CMC_DEF_NODE(SNAME) *                                               \
        CMC_(PFX, _prev_node)(struct CMC_DEF_NODE(SNAME) * _node_)             \
    {                                                                          \
        return _node_->prev;                                                   \
    }                                                                          \
                                                                               \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target)   \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        iter.target = target;                                                  \
        iter.cursor = target->head;                                            \
        iter.index = 0;                                                        \
        iter.start = true;                                                     \
        iter.end = CMC_(PFX, _empty)(target);                                  \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target)     \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        iter.target = target;                                                  \
        iter.cursor = target->tail;                                            \
        iter.index = 0;                                                        \
        iter.start = CMC_(PFX, _empty)(target);                                \
        iter.end = true;                                                       \
                                                                               \
        if (!CMC_(PFX, _empty)(target))                                        \
            iter.index = target->count - 1;                                    \
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
            iter->cursor = iter->target->head;                                 \
            iter->index = 0;                                                   \
            iter->start = true;                                                \
            iter->end = CMC_(PFX, _empty)(iter->target);                       \
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
            iter->cursor = iter->target->tail;                                 \
            iter->index = iter->target->count - 1;                             \
            iter->start = CMC_(PFX, _empty)(iter->target);                     \
            iter->end = true;                                                  \
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
        if (iter->cursor->next == NULL)                                        \
        {                                                                      \
            iter->end = true;                                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        iter->start = CMC_(PFX, _empty)(iter->target);                         \
                                                                               \
        iter->cursor = iter->cursor->next;                                     \
        iter->index++;                                                         \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                          \
        if (iter->start)                                                       \
            return false;                                                      \
                                                                               \
        if (iter->cursor->prev == NULL)                                        \
        {                                                                      \
            iter->start = true;                                                \
            return false;                                                      \
        }                                                                      \
                                                                               \
        iter->end = CMC_(PFX, _empty)(iter->target);                           \
                                                                               \
        iter->cursor = iter->cursor->prev;                                     \
        iter->index--;                                                         \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                  size_t steps)                                \
    {                                                                          \
        if (iter->end)                                                         \
            return false;                                                      \
                                                                               \
        if (iter->cursor->next == NULL)                                        \
        {                                                                      \
            iter->end = true;                                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (steps == 0 || iter->index + steps >= iter->target->count)          \
            return false;                                                      \
                                                                               \
        iter->start = CMC_(PFX, _empty)(iter->target);                         \
                                                                               \
        iter->index += steps;                                                  \
                                                                               \
        for (size_t i = 0; i < steps; i++)                                     \
            iter->cursor = iter->cursor->next;                                 \
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
        if (iter->cursor->prev == NULL)                                        \
        {                                                                      \
            iter->start = true;                                                \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (steps == 0 || iter->index < steps)                                 \
            return false;                                                      \
                                                                               \
        iter->end = CMC_(PFX, _empty)(iter->target);                           \
                                                                               \
        iter->index -= steps;                                                  \
                                                                               \
        for (size_t i = 0; i < steps; i++)                                     \
            iter->cursor = iter->cursor->prev;                                 \
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
    struct CMC_DEF_NODE(SNAME) *                                               \
        CMC_(PFX, _iter_node)(struct CMC_DEF_ITER(SNAME) * iter)               \
    {                                                                          \
        return iter->cursor;                                                   \
    }

#endif /* CMC_CMC_LINKEDLIST_H */
