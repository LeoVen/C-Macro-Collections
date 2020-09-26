/**
 * cmc_linkedlist.h
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
#include "cor_core.h"

/**
 * Core LinkedList implementation
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_CMC_LINKEDLIST_CORE(ACCESS, FILE, PARAMS) \
    CMC_(CMC_(CMC_CMC_LINKEDLIST_CORE_, ACCESS), CMC_(_, FILE))(PARAMS)

/* PRIVATE or PUBLIC solver */
#define CMC_CMC_LINKEDLIST_CORE_PUBLIC_HEADER(PARAMS) \
    CMC_CMC_LINKEDLIST_CORE_STRUCT(PARAMS) \
    CMC_CMC_LINKEDLIST_CORE_HEADER(PARAMS)

#define CMC_CMC_LINKEDLIST_CORE_PUBLIC_SOURCE(PARAMS) CMC_CMC_LINKEDLIST_CORE_SOURCE(PARAMS)

#define CMC_CMC_LINKEDLIST_CORE_PRIVATE_HEADER(PARAMS) \
    struct CMC_PARAM_SNAME(PARAMS); \
    CMC_CMC_LINKEDLIST_CORE_HEADER(PARAMS)

#define CMC_CMC_LINKEDLIST_CORE_PRIVATE_SOURCE(PARAMS) \
    CMC_CMC_LINKEDLIST_CORE_STRUCT(PARAMS) \
    CMC_CMC_LINKEDLIST_CORE_SOURCE(PARAMS)

/* Lowest level API */
#define CMC_CMC_LINKEDLIST_CORE_STRUCT(PARAMS) \
    CMC_CMC_LINKEDLIST_CORE_STRUCT_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_LINKEDLIST_CORE_HEADER(PARAMS) \
    CMC_CMC_LINKEDLIST_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_LINKEDLIST_CORE_SOURCE(PARAMS) \
    CMC_CMC_LINKEDLIST_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Struct
 * ------------------------------------------------------------------------- */
#define CMC_CMC_LINKEDLIST_CORE_STRUCT_(PFX, SNAME, V) \
\
    /* Linked List Structure */ \
    struct SNAME \
    { \
        /* First node in the list */ \
        struct CMC_DEF_NODE(SNAME) * head; \
\
        /* Last node in the list */ \
        struct CMC_DEF_NODE(SNAME) * tail; \
\
        /* Current amount of elements in the list */ \
        size_t count; \
\
        /* Flags indicating errors or success */ \
        int flag; \
\
        /* Value function table */ \
        struct CMC_DEF_FVAL(SNAME) * f_val; \
\
        /* Custom allocation functions */ \
        struct CMC_ALLOC_NODE_NAME *alloc; \
\
        /* Custom callback functions */ \
        CMC_CALLBACKS_DECL; \
    }; \
\
    /* Doubly-linked list node */ \
    struct CMC_DEF_NODE(SNAME) \
    { \
        /* Node's value */ \
        V value; \
\
        /* Pointer to the next node on the linked list */ \
        struct CMC_DEF_NODE(SNAME) * next; \
\
        /* Pointer to the previous node on the linked list */ \
        struct CMC_DEF_NODE(SNAME) * prev; \
    };

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_LINKEDLIST_CORE_HEADER_(PFX, SNAME, V) \
\
    /* Value struct function table */ \
    struct CMC_DEF_FVAL(SNAME) \
    { \
        /* Comparator function */ \
        CMC_DEF_FTAB_CMP(V); \
\
        /* Copy function */ \
        CMC_DEF_FTAB_CPY(V); \
\
        /* To string function */ \
        CMC_DEF_FTAB_STR(V); \
\
        /* Free from memory function */ \
        CMC_DEF_FTAB_FREE(V); \
\
        /* Hash function */ \
        CMC_DEF_FTAB_HASH(V); \
\
        /* Priority function */ \
        CMC_DEF_FTAB_PRI(V); \
    }; \
\
    /* Collection Functions */ \
    /* Collection Allocation and Deallocation */ \
    struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FVAL(SNAME) * f_val); \
    struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FVAL(SNAME) * f_val, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks); \
    void CMC_(PFX, _clear)(struct SNAME * _list_); \
    void CMC_(PFX, _free)(struct SNAME * _list_); \
    /* Customization of Allocation and Callbacks */ \
    void CMC_(PFX, _customize)(struct SNAME * _list_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks); \
    /* Collection Input and Output */ \
    bool CMC_(PFX, _push_front)(struct SNAME * _list_, V value); \
    bool CMC_(PFX, _push_at)(struct SNAME * _list_, V value, size_t index); \
    bool CMC_(PFX, _push_back)(struct SNAME * _list_, V value); \
    bool CMC_(PFX, _pop_front)(struct SNAME * _list_); \
    bool CMC_(PFX, _pop_at)(struct SNAME * _list_, size_t index); \
    bool CMC_(PFX, _pop_back)(struct SNAME * _list_); \
    /* Element Access */ \
    V CMC_(PFX, _front)(struct SNAME * _list_); \
    V CMC_(PFX, _get)(struct SNAME * _list_, size_t index); \
    V *CMC_(PFX, _get_ref)(struct SNAME * _list_, size_t index); \
    V CMC_(PFX, _back)(struct SNAME * _list_); \
    /* Collection State */ \
    bool CMC_(PFX, _contains)(struct SNAME * _list_, V value); \
    bool CMC_(PFX, _empty)(struct SNAME * _list_); \
    size_t CMC_(PFX, _count)(struct SNAME * _list_); \
    int CMC_(PFX, _flag)(struct SNAME * _list_); \
    /* Collection Utility */ \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _list_); \
    bool CMC_(PFX, _equals)(struct SNAME * _list1_, struct SNAME * _list2_);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_LINKEDLIST_CORE_SOURCE_(PFX, SNAME, V) \
\
    /* Implementation Detail Functions */ \
    /* None */ \
\
    struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FVAL(SNAME) * f_val) \
    { \
        return CMC_(PFX, _new_custom)(f_val, NULL, NULL); \
    } \
\
    struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FVAL(SNAME) * f_val, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!f_val) \
            return NULL; \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        struct SNAME *_list_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_list_) \
            return NULL; \
\
        _list_->count = 0; \
        _list_->head = NULL; \
        _list_->tail = NULL; \
        _list_->flag = CMC_FLAG_OK; \
        _list_->f_val = f_val; \
        _list_->alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(_list_, callbacks); \
\
        return _list_; \
    } \
\
    void CMC_(PFX, _clear)(struct SNAME * _list_) \
    { \
        struct CMC_DEF_NODE(SNAME) *scan = _list_->head; \
\
        while (_list_->head != NULL) \
        { \
            _list_->head = _list_->head->next; \
\
            if (_list_->f_val->free) \
            { \
                _list_->f_val->free(scan->value); \
            } \
\
            _list_->alloc->free(scan); \
\
            scan = _list_->head; \
        } \
\
        _list_->count = 0; \
        _list_->head = NULL; \
        _list_->tail = NULL; \
        _list_->flag = CMC_FLAG_OK; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _list_) \
    { \
        CMC_(PFX, _clear)(_list_); \
\
        _list_->alloc->free(_list_); \
    } \
\
    void CMC_(PFX, _customize)(struct SNAME * _list_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!alloc) \
            _list_->alloc = &cmc_alloc_node_default; \
        else \
            _list_->alloc = alloc; \
\
        CMC_CALLBACKS_ASSIGN(_list_, callbacks); \
\
        _list_->flag = CMC_FLAG_OK; \
    } \
\
    bool CMC_(PFX, _push_front)(struct SNAME * _list_, V value) \
    { \
        struct CMC_DEF_NODE(SNAME) *_node_ = CMC_(PFX, _new_node)(_list_, value); \
\
        if (!_node_) \
            return false; \
\
        if (CMC_(PFX, _empty)(_list_)) \
        { \
            _list_->head = _node_; \
            _list_->tail = _node_; \
        } \
        else \
        { \
            _node_->next = _list_->head; \
            _list_->head->prev = _node_; \
            _list_->head = _node_; \
        } \
\
        _list_->count++; \
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _push_at)(struct SNAME * _list_, V value, size_t index) \
    { \
        if (index > _list_->count) \
        { \
            _list_->flag = CMC_FLAG_RANGE; \
            return false; \
        } \
\
        if (index == 0) \
        { \
            return CMC_(PFX, _push_front)(_list_, value); \
        } \
        else if (index == _list_->count) \
        { \
            return CMC_(PFX, _push_back)(_list_, value); \
        } \
\
        struct CMC_DEF_NODE(SNAME) *_node_ = CMC_(PFX, _new_node)(_list_, value); \
\
        if (!_node_) \
            return false; \
\
        struct CMC_DEF_NODE(SNAME) *scan = CMC_(PFX, _get_node)(_list_, index - 1); \
\
        _node_->next = scan->next; \
        _node_->prev = scan; \
        _node_->next->prev = _node_; \
        _node_->prev->next = _node_; \
\
        _list_->count++; \
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _push_back)(struct SNAME * _list_, V value) \
    { \
        struct CMC_DEF_NODE(SNAME) *_node_ = CMC_(PFX, _new_node)(_list_, value); \
\
        if (!_node_) \
            return false; \
\
        if (CMC_(PFX, _empty)(_list_)) \
        { \
            _list_->head = _node_; \
            _list_->tail = _node_; \
        } \
        else \
        { \
            _node_->prev = _list_->tail; \
            _list_->tail->next = _node_; \
            _list_->tail = _node_; \
        } \
\
        _list_->count++; \
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _pop_front)(struct SNAME * _list_) \
    { \
        if (CMC_(PFX, _empty)(_list_)) \
        { \
            _list_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        struct CMC_DEF_NODE(SNAME) *_node_ = _list_->head; \
        _list_->head = _list_->head->next; \
\
        _list_->alloc->free(_node_); \
\
        if (_list_->head == NULL) \
            _list_->tail = NULL; \
        else \
            _list_->head->prev = NULL; \
\
        _list_->count--; \
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, delete); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _pop_at)(struct SNAME * _list_, size_t index) \
    { \
        if (CMC_(PFX, _empty)(_list_)) \
        { \
            _list_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        if (index >= _list_->count) \
        { \
            _list_->flag = CMC_FLAG_RANGE; \
            return false; \
        } \
\
        if (index == 0) \
        { \
            return CMC_(PFX, _pop_front)(_list_); \
        } \
        else if (index == _list_->count - 1) \
        { \
            return CMC_(PFX, _pop_back)(_list_); \
        } \
\
        struct CMC_DEF_NODE(SNAME) *_node_ = CMC_(PFX, _get_node)(_list_, index); \
\
        if (!_node_) \
            return false; \
\
        _node_->next->prev = _node_->prev; \
        _node_->prev->next = _node_->next; \
\
        _list_->alloc->free(_node_); \
\
        _list_->count--; \
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, delete); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _pop_back)(struct SNAME * _list_) \
    { \
        if (CMC_(PFX, _empty)(_list_)) \
        { \
            _list_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        struct CMC_DEF_NODE(SNAME) *_node_ = _list_->tail; \
        _list_->tail = _list_->tail->prev; \
\
        _list_->alloc->free(_node_); \
\
        if (_list_->tail == NULL) \
            _list_->head = NULL; \
        else \
            _list_->tail->next = NULL; \
\
        _list_->count--; \
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, delete); \
\
        return true; \
    } \
\
    V CMC_(PFX, _front)(struct SNAME * _list_) \
    { \
        if (CMC_(PFX, _empty)(_list_)) \
        { \
            _list_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, read); \
\
        return _list_->head->value; \
    } \
\
    V CMC_(PFX, _get)(struct SNAME * _list_, size_t index) \
    { \
        if (CMC_(PFX, _empty)(_list_)) \
        { \
            _list_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        if (index >= _list_->count) \
        { \
            _list_->flag = CMC_FLAG_RANGE; \
            return (V){ 0 }; \
        } \
\
        struct CMC_DEF_NODE(SNAME) *scan = CMC_(PFX, _get_node)(_list_, index); \
\
        if (scan == NULL) \
            return (V){ 0 }; \
\
        CMC_CALLBACKS_CALL(_list_, read); \
\
        return scan->value; \
    } \
\
    V *CMC_(PFX, _get_ref)(struct SNAME * _list_, size_t index) \
    { \
        if (CMC_(PFX, _empty)(_list_)) \
        { \
            _list_->flag = CMC_FLAG_EMPTY; \
            return NULL; \
        } \
\
        if (index >= _list_->count) \
        { \
            _list_->flag = CMC_FLAG_RANGE; \
            return NULL; \
        } \
\
        struct CMC_DEF_NODE(SNAME) *scan = CMC_(PFX, _get_node)(_list_, index); \
\
        if (scan == NULL) \
            return NULL; \
\
        CMC_CALLBACKS_CALL(_list_, read); \
\
        return &(scan->value); \
    } \
\
    V CMC_(PFX, _back)(struct SNAME * _list_) \
    { \
        if (CMC_(PFX, _empty)(_list_)) \
        { \
            _list_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        _list_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_list_, read); \
\
        return _list_->tail->value; \
    } \
\
    bool CMC_(PFX, _contains)(struct SNAME * _list_, V value) \
    { \
        _list_->flag = CMC_FLAG_OK; \
\
        bool result = false; \
\
        struct CMC_DEF_NODE(SNAME) *scan = _list_->head; \
\
        while (scan != NULL) \
        { \
            if (_list_->f_val->cmp(scan->value, value) == 0) \
            { \
                result = true; \
                break; \
            } \
\
            scan = scan->next; \
        } \
\
        CMC_CALLBACKS_CALL(_list_, read); \
\
        return result; \
    } \
\
    bool CMC_(PFX, _empty)(struct SNAME * _list_) \
    { \
        return _list_->count == 0; \
    } \
\
    size_t CMC_(PFX, _count)(struct SNAME * _list_) \
    { \
        return _list_->count; \
    } \
\
    int CMC_(PFX, _flag)(struct SNAME * _list_) \
    { \
        return _list_->flag; \
    } \
\
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _list_) \
    { \
        struct SNAME *result = CMC_(PFX, _new_custom)(_list_->f_val, _list_->alloc, NULL); \
\
        if (!result) \
            return NULL; \
\
        CMC_CALLBACKS_ASSIGN(result, _list_->callbacks); \
\
        struct CMC_DEF_NODE(SNAME) *scan = _list_->head; \
\
        while (scan != NULL) \
        { \
            /* This allocation should never fail since it might not be */ \
            /* possible to recover from it. That is why it isn't checked */ \
            struct CMC_DEF_NODE(SNAME) * new_node; \
\
            if (_list_->f_val->cpy) \
                new_node = CMC_(PFX, _new_node)(_list_, _list_->f_val->cpy(scan->value)); \
            else \
                new_node = CMC_(PFX, _new_node)(_list_, scan->value); \
\
            if (!result->head) \
            { \
                result->head = new_node; \
                result->tail = new_node; \
            } \
            else \
            { \
                new_node->prev = result->tail; \
                result->tail->next = new_node; \
                result->tail = new_node; \
            } \
\
            scan = scan->next; \
        } \
\
        result->count = _list_->count; \
\
        _list_->flag = CMC_FLAG_OK; \
\
        return result; \
    } \
\
    bool CMC_(PFX, _equals)(struct SNAME * _list1_, struct SNAME * _list2_) \
    { \
        _list1_->flag = CMC_FLAG_OK; \
        _list2_->flag = CMC_FLAG_OK; \
\
        if (_list1_->count != _list2_->count) \
            return false; \
\
        struct CMC_DEF_NODE(SNAME) *scan1 = _list1_->head; \
        struct CMC_DEF_NODE(SNAME) *scan2 = _list2_->head; \
\
        while (scan1 != NULL && scan2 != NULL) \
        { \
            if (_list1_->f_val->cmp(scan1->value, scan2->value) != 0) \
                return false; \
\
            scan1 = scan1->next; \
            scan2 = scan2->next; \
        } \
\
        return true; \
    }

#endif /* CMC_CMC_LINKEDLIST_H */
