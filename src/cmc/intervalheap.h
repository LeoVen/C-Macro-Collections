/**
 * cmc/intervalheap.h
 *
 * Creation Date: 06/07/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * An interval heap is a specialized heap with:
 *
 * - O(1) - Find Min
 * - O(1) - Find Max
 * - O(log n) - Insert
 * - O(log n) - Remove Min
 * - O(log n) - Remove Max
 */

#ifndef CMC_CMC_INTERVALHEAP_H
#define CMC_CMC_INTERVALHEAP_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/**
 * Core IntervalHeap implementation
 */
#define CMC_CMC_INTERVALHEAP_CORE(PARAMS) \
    CMC_CMC_INTERVALHEAP_CORE_HEADER(PARAMS) \
    CMC_CMC_INTERVALHEAP_CORE_SOURCE(PARAMS)

#define CMC_CMC_INTERVALHEAP_CORE_HEADER(PARAMS) \
    CMC_CMC_INTERVALHEAP_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

#define CMC_CMC_INTERVALHEAP_CORE_SOURCE(PARAMS) \
    CMC_CMC_INTERVALHEAP_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_INTERVALHEAP_CORE_HEADER_(PFX, SNAME, V) \
\
    /* IntervalHeap Structure */ \
    struct SNAME \
    { \
        /* Dynamic array of elements */ \
        /* buffer[n][0] is MinHeap and buffer[n][1] is MaxHeap */ \
        V (*buffer)[2]; \
\
        /* Current array capacity  */ \
        size_t capacity; \
\
        /* Current amount of pairs of values in the array */ \
        size_t size; \
\
        /* Current amount of elements in the heap */ \
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
    struct SNAME *CMC_(PFX, _new)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val); \
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val, \
                                         struct CMC_ALLOC_NODE_NAME * alloc, struct CMC_CALLBACKS_NAME * callbacks); \
    void CMC_(PFX, _clear)(struct SNAME * _heap_); \
    void CMC_(PFX, _free)(struct SNAME * _heap_); \
    /* Customization of Allocation and Callbacks */ \
    void CMC_(PFX, _customize)(struct SNAME * _heap_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks); \
    /* Collection Input and Output */ \
    bool CMC_(PFX, _insert)(struct SNAME * _heap_, V value); \
    bool CMC_(PFX, _remove_max)(struct SNAME * _heap_); \
    bool CMC_(PFX, _remove_min)(struct SNAME * _heap_); \
    /* Collection Update */ \
    bool CMC_(PFX, _update_max)(struct SNAME * _heap_, V value); \
    bool CMC_(PFX, _update_min)(struct SNAME * _heap_, V value); \
    /* Element Access */ \
    V CMC_(PFX, _max)(struct SNAME * _heap_); \
    V CMC_(PFX, _min)(struct SNAME * _heap_); \
    /* Collection State */ \
    bool CMC_(PFX, _contains)(struct SNAME * _heap_, V value); \
    bool CMC_(PFX, _empty)(struct SNAME * _heap_); \
    bool CMC_(PFX, _full)(struct SNAME * _heap_); \
    size_t CMC_(PFX, _count)(struct SNAME * _heap_); \
    size_t CMC_(PFX, _capacity)(struct SNAME * _heap_); \
    int CMC_(PFX, _flag)(struct SNAME * _heap_); \
    /* Collection Utility */ \
    bool CMC_(PFX, _resize)(struct SNAME * _heap_, size_t capacity); \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _heap_); \
    bool CMC_(PFX, _equals)(struct SNAME * _heap1_, struct SNAME * _heap2_);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_INTERVALHEAP_CORE_SOURCE_(PFX, SNAME, V) \
\
    /* Implementation Detail Functions */ \
    static void CMC_(PFX, _impl_float_up_max)(struct SNAME * _heap_); \
    static void CMC_(PFX, _impl_float_up_min)(struct SNAME * _heap_); \
    static void CMC_(PFX, _impl_float_down_max)(struct SNAME * _heap_); \
    static void CMC_(PFX, _impl_float_down_min)(struct SNAME * _heap_); \
\
    struct SNAME *CMC_(PFX, _new)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val) \
    { \
        return CMC_(PFX, _new_custom)(capacity, f_val, NULL, NULL); \
    } \
\
    struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val, \
                                         struct CMC_ALLOC_NODE_NAME * alloc, struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (capacity == 0 || capacity == UINTMAX_MAX) \
            return NULL; \
\
        if (!f_val) \
            return NULL; \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        struct SNAME *_heap_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_heap_) \
            return NULL; \
\
        capacity = (capacity + capacity % 2) / 2; \
\
        _heap_->buffer = alloc->calloc(capacity, sizeof(V[2])); \
\
        if (!_heap_->buffer) \
        { \
            alloc->free(_heap_); \
            return NULL; \
        } \
\
        _heap_->capacity = capacity; \
        _heap_->size = 0; \
        _heap_->count = 0; \
        _heap_->flag = CMC_FLAG_OK; \
        _heap_->f_val = f_val; \
        _heap_->alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(_heap_, callbacks); \
\
        return _heap_; \
    } \
\
    void CMC_(PFX, _clear)(struct SNAME * _heap_) \
    { \
        if (_heap_->f_val->free) \
        { \
            for (size_t i = 0; i < _heap_->count; i++) \
            { \
                _heap_->f_val->free(_heap_->buffer[i / 2][i % 2]); \
            } \
        } \
\
        memset(_heap_->buffer, 0, sizeof(V[2]) * _heap_->capacity); \
\
        _heap_->size = 0; \
        _heap_->count = 0; \
        _heap_->flag = CMC_FLAG_OK; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _heap_) \
    { \
        if (_heap_->f_val->free) \
        { \
            for (size_t i = 0; i < _heap_->count; i++) \
            { \
                _heap_->f_val->free(_heap_->buffer[i / 2][i % 2]); \
            } \
        } \
\
        _heap_->alloc->free(_heap_->buffer); \
\
        _heap_->alloc->free(_heap_); \
    } \
\
    void CMC_(PFX, _customize)(struct SNAME * _heap_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!alloc) \
            _heap_->alloc = &cmc_alloc_node_default; \
        else \
            _heap_->alloc = alloc; \
\
        CMC_CALLBACKS_ASSIGN(_heap_, callbacks); \
\
        _heap_->flag = CMC_FLAG_OK; \
    } \
\
    bool CMC_(PFX, _insert)(struct SNAME * _heap_, V value) \
    { \
        if (CMC_(PFX, _full)(_heap_)) \
        { \
            if (!CMC_(PFX, _resize)(_heap_, _heap_->capacity * 4)) \
                return false; \
        } \
\
        if (_heap_->count % 2 == 0) \
        { \
            /* Occupying a new node */ \
            _heap_->buffer[_heap_->size][0] = value; \
            _heap_->buffer[_heap_->size][1] = (V){ 0 }; \
\
            _heap_->size++; \
        } \
        else \
        { \
            V(*node)[2] = &(_heap_->buffer[_heap_->size - 1]); \
\
            /* Decide if the new element goes into the MinHeap or MaxHeap */ \
            if (_heap_->f_val->cmp((*node)[0], value) > 0) \
            { \
                /* Swap current value and add new element to the MinHeap */ \
                (*node)[1] = (*node)[0]; \
                (*node)[0] = value; \
            } \
            else \
            { \
                /* No need to swap and add the new element to the MaxHeap */ \
                (*node)[1] = value; \
            } \
        } \
\
        _heap_->count++; \
        _heap_->flag = CMC_FLAG_OK; \
\
        if (_heap_->count > 2) \
        { \
            /* Determine wheather to do a MaxHeap insert or a MinHeap */ \
            /* insert. For any integer this index calculation results in */ \
            /* an even number representing the parent of the last value */ \
            V(*parent)[2] = &(_heap_->buffer[(_heap_->size - 2) / 2]); \
\
            if (_heap_->f_val->cmp((*parent)[0], value) > 0) \
                CMC_(PFX, _impl_float_up_min)(_heap_); \
            else if (_heap_->f_val->cmp((*parent)[1], value) < 0) \
                CMC_(PFX, _impl_float_up_max)(_heap_); \
            /* else no float up required */ \
        } \
\
        CMC_CALLBACKS_CALL(_heap_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _remove_max)(struct SNAME * _heap_) \
    { \
        if (CMC_(PFX, _empty)(_heap_)) \
        { \
            _heap_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        if (_heap_->count == 1) \
        { \
            _heap_->buffer[0][0] = (V){ 0 }; \
\
            _heap_->count--; \
            _heap_->size--; \
\
            goto success; \
        } \
\
        /* Swap the result with the last element in the MaxHeap and float */ \
        /* it down */ \
        V(*last_node)[2] = &(_heap_->buffer[_heap_->size - 1]); \
\
        if (_heap_->count % 2 == 1) \
        { \
            /* Grab from MinHeap and discard last node */ \
            _heap_->buffer[0][1] = (*last_node)[0]; \
\
            (*last_node)[0] = (V){ 0 }; \
\
            _heap_->size--; \
        } \
        else \
        { \
            _heap_->buffer[0][1] = (*last_node)[1]; \
\
            (*last_node)[1] = (V){ 0 }; \
        } \
\
        _heap_->count--; \
\
        /* FLoat Down on the MaxHeap */ \
        CMC_(PFX, _impl_float_down_max)(_heap_); \
\
    success: \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_heap_, delete); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _remove_min)(struct SNAME * _heap_) \
    { \
        if (CMC_(PFX, _empty)(_heap_)) \
        { \
            _heap_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        if (_heap_->count == 1) \
        { \
            _heap_->buffer[0][0] = (V){ 0 }; \
\
            _heap_->count--; \
            _heap_->size--; \
\
            goto success; \
        } \
\
        /* Swap the result with the last element in the MinHeap and float */ \
        /* it down */ \
        V(*last_node)[2] = &(_heap_->buffer[_heap_->size - 1]); \
\
        _heap_->buffer[0][0] = (*last_node)[0]; \
\
        if (_heap_->count % 2 == 1) \
        { \
            /* Discard last node */ \
            (*last_node)[0] = (V){ 0 }; \
\
            _heap_->size--; \
        } \
        else \
        { \
            /* Place the MaxHeap value in the MinHeap value spot */ \
            (*last_node)[0] = (*last_node)[1]; \
            (*last_node)[1] = (V){ 0 }; \
        } \
\
        _heap_->count--; \
\
        /* FLoat Down on the MinHeap */ \
        CMC_(PFX, _impl_float_down_min)(_heap_); \
\
    success: \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_heap_, delete); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _update_max)(struct SNAME * _heap_, V value) \
    { \
        if (CMC_(PFX, _empty)(_heap_)) \
        { \
            _heap_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        if (_heap_->count == 1) \
        { \
            _heap_->buffer[0][0] = value; \
        } \
        else if (_heap_->f_val->cmp(value, _heap_->buffer[0][0]) < 0) \
        { \
            /* Corner case: we are updating the Max value but it is less */ \
            /* than the Min value */ \
            _heap_->buffer[0][1] = _heap_->buffer[0][0]; \
            _heap_->buffer[0][0] = value; \
\
            CMC_(PFX, _impl_float_down_max)(_heap_); \
        } \
        else \
        { \
            /* Update Max element and float it down */ \
            _heap_->buffer[0][1] = value; \
\
            CMC_(PFX, _impl_float_down_max)(_heap_); \
        } \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_heap_, update); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _update_min)(struct SNAME * _heap_, V value) \
    { \
        if (CMC_(PFX, _empty)(_heap_)) \
        { \
            _heap_->flag = CMC_FLAG_EMPTY; \
            return false; \
        } \
\
        if (_heap_->count == 1) \
        { \
            _heap_->buffer[0][0] = value; \
        } \
        else if (_heap_->f_val->cmp(value, _heap_->buffer[0][1]) > 0) \
        { \
            /* Corner case: we are updating the Min value but it is greater */ \
            /* than the Max value. */ \
            _heap_->buffer[0][0] = _heap_->buffer[0][1]; \
            _heap_->buffer[0][1] = value; \
\
            CMC_(PFX, _impl_float_down_min)(_heap_); \
        } \
        else \
        { \
            /* Update Min element and float it down */ \
            _heap_->buffer[0][0] = value; \
\
            CMC_(PFX, _impl_float_down_min)(_heap_); \
        } \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_heap_, update); \
\
        return true; \
    } \
\
    V CMC_(PFX, _max)(struct SNAME * _heap_) \
    { \
        if (CMC_(PFX, _empty)(_heap_)) \
        { \
            _heap_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_heap_, read); \
\
        /* If there is only one element, then the maximum element is the */ \
        /* same as the one in the MinHeap */ \
        if (_heap_->count == 1) \
            return _heap_->buffer[0][0]; \
\
        return _heap_->buffer[0][1]; \
    } \
\
    V CMC_(PFX, _min)(struct SNAME * _heap_) \
    { \
        if (CMC_(PFX, _empty)(_heap_)) \
        { \
            _heap_->flag = CMC_FLAG_EMPTY; \
            return (V){ 0 }; \
        } \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_heap_, read); \
\
        return _heap_->buffer[0][0]; \
    } \
\
    bool CMC_(PFX, _contains)(struct SNAME * _heap_, V value) \
    { \
        _heap_->flag = CMC_FLAG_OK; \
\
        bool result = false; \
\
        for (size_t i = 0; i < _heap_->count; i++) \
        { \
            if (_heap_->f_val->cmp(_heap_->buffer[i / 2][i % 2], value) == 0) \
            { \
                result = true; \
                break; \
            } \
        } \
\
        CMC_CALLBACKS_CALL(_heap_, read); \
\
        return result; \
    } \
\
    bool CMC_(PFX, _empty)(struct SNAME * _heap_) \
    { \
        return _heap_->count == 0; \
    } \
\
    bool CMC_(PFX, _full)(struct SNAME * _heap_) \
    { \
        /* The heap is full if all nodes are completely filled */ \
        return _heap_->size >= _heap_->capacity && _heap_->count % 2 == 0; \
    } \
\
    size_t CMC_(PFX, _count)(struct SNAME * _heap_) \
    { \
        return _heap_->count; \
    } \
\
    size_t CMC_(PFX, _capacity)(struct SNAME * _heap_) \
    { \
        return _heap_->capacity; \
    } \
\
    int CMC_(PFX, _flag)(struct SNAME * _heap_) \
    { \
        return _heap_->flag; \
    } \
\
    bool CMC_(PFX, _resize)(struct SNAME * _heap_, size_t capacity) \
    { \
        if (_heap_->capacity == capacity) \
            goto success; \
\
        if (capacity < _heap_->count) \
        { \
            _heap_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        /* Prevent overflow */ \
        if (capacity == UINTMAX_MAX) \
        { \
            _heap_->flag = CMC_FLAG_ERROR; \
            return false; \
        } \
\
        capacity += capacity % 2; \
\
        V(*new_buffer)[2] = _heap_->alloc->realloc(_heap_->buffer, sizeof(V[2]) * capacity); \
\
        if (!new_buffer) \
        { \
            _heap_->flag = CMC_FLAG_ALLOC; \
            return false; \
        } \
\
        if (capacity > _heap_->capacity) \
        { \
            memset(new_buffer + _heap_->capacity, 0, sizeof(V[2]) * (capacity - _heap_->capacity)); \
        } \
\
        _heap_->buffer = new_buffer; \
        _heap_->capacity = capacity; \
\
    success: \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_heap_, resize); \
\
        return true; \
    } \
\
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _heap_) \
    { \
        struct SNAME *result = _heap_->alloc->malloc(sizeof(struct SNAME)); \
\
        if (!result) \
        { \
            _heap_->flag = CMC_FLAG_ALLOC; \
            return NULL; \
        } \
\
        memcpy(result, _heap_, sizeof(struct SNAME)); \
\
        result->buffer = _heap_->alloc->malloc(sizeof(V[2]) * _heap_->capacity); \
\
        if (!result->buffer) \
        { \
            _heap_->alloc->free(result); \
            _heap_->flag = CMC_FLAG_ALLOC; \
            return NULL; \
        } \
\
        if (_heap_->f_val->cpy) \
        { \
            for (size_t i = 0; i < _heap_->count; i++) \
                result->buffer[i / 2][i % 2] = _heap_->f_val->cpy(_heap_->buffer[i / 2][i % 2]); \
        } \
        else \
            memcpy(result->buffer, _heap_->buffer, sizeof(V[2]) * _heap_->capacity); \
\
        result->capacity = _heap_->capacity; \
        result->size = _heap_->size; \
        result->count = _heap_->count; \
        result->flag = CMC_FLAG_OK; \
        result->f_val = _heap_->f_val; \
        result->alloc = _heap_->alloc; \
        CMC_CALLBACKS_ASSIGN(result, _heap_->callbacks); \
\
        _heap_->flag = CMC_FLAG_OK; \
\
        return result; \
    } \
\
    bool CMC_(PFX, _equals)(struct SNAME * _heap1_, struct SNAME * _heap2_) \
    { \
        _heap1_->flag = CMC_FLAG_OK; \
        _heap2_->flag = CMC_FLAG_OK; \
\
        if (_heap1_->count != _heap2_->count) \
            return false; \
\
        for (size_t i = 0; i < _heap1_->count; i++) \
        { \
            V value1 = _heap1_->buffer[i / 2][i % 2]; \
            V value2 = _heap2_->buffer[i / 2][i % 2]; \
\
            if (_heap1_->f_val->cmp(value1, value2) != 0) \
                return false; \
        } \
\
        return true; \
    } \
\
    static void CMC_(PFX, _impl_float_up_max)(struct SNAME * _heap_) \
    { \
        size_t index = _heap_->size - 1; \
\
        V(*curr_node)[2] = &(_heap_->buffer[index]); \
\
        /* FLoat Up on the MaxHeap */ \
        while (index > 0) \
        { \
            /* Parent index */ \
            size_t P_index = (index - 1) / 2; \
\
            V(*parent)[2] = &(_heap_->buffer[P_index]); \
\
            if (index == _heap_->size - 1 && _heap_->count % 2 != 0) \
            { \
                /* In this case, the current node has no MaxHeap value so */ \
                /* we instead compare with the MinHeap value */ \
                if (_heap_->f_val->cmp((*curr_node)[0], (*parent)[1]) < 0) \
                    break; \
\
                /* Since the comparison above passed now we need to swap   */ \
                /* the current node with the parent but it will have to be */ \
                /* done with the node's MinHeap value since the MaxHeap */ \
                /* value doesn't exist */ \
                V tmp = (*curr_node)[0]; \
                (*curr_node)[0] = (*parent)[1]; \
                (*parent)[1] = tmp; \
            } \
            else \
            { \
                /* Usual case, just compare both MaxHeap values */ \
                if (_heap_->f_val->cmp((*curr_node)[1], (*parent)[1]) < 0) \
                    break; \
\
                /* Swap with parent and repeat */ \
                V tmp = (*curr_node)[1]; \
                (*curr_node)[1] = (*parent)[1]; \
                (*parent)[1] = tmp; \
            } \
\
            /* Update indexe and node */ \
            index = P_index; \
            curr_node = parent; \
        } \
    } \
\
    static void CMC_(PFX, _impl_float_up_min)(struct SNAME * _heap_) \
    { \
        size_t index = _heap_->size - 1; \
\
        V(*curr_node)[2] = &(_heap_->buffer[index]); \
\
        /* FLoat Up on the MinHeap */ \
        while (index > 0) \
        { \
            /* Parent index */ \
            size_t P_index = (index - 1) / 2; \
\
            V(*parent)[2] = &(_heap_->buffer[P_index]); \
\
            if (_heap_->f_val->cmp((*curr_node)[0], (*parent)[0]) >= 0) \
                break; \
\
            /* Swap with parent and repeat */ \
            V tmp = (*curr_node)[0]; \
            (*curr_node)[0] = (*parent)[0]; \
            (*parent)[0] = tmp; \
\
            /* Update indexe and node */ \
            index = P_index; \
            curr_node = parent; \
        } \
    } \
\
    static void CMC_(PFX, _impl_float_down_max)(struct SNAME * _heap_) \
    { \
        /* Floats Down from the MinHeap */ \
        size_t index = 0; \
\
        V(*curr_node)[2] = &(_heap_->buffer[index]); \
\
        while (true) \
        { \
            /* If there are no children it is done */ \
            if (2 * index + 1 >= _heap_->size) \
                break; \
\
            /* Child index to be compared with current value */ \
            size_t child; \
            size_t L_index = index * 2 + 1; \
            size_t R_index = index * 2 + 2; \
\
            /* If there are two children, pick the smallest one */ \
            if (R_index < _heap_->size) \
            { \
                V(*L)[2] = &(_heap_->buffer[L_index]); \
                V(*R)[2] = &(_heap_->buffer[R_index]); \
\
                /* If the right child is the last node and there is no */ \
                /* MaxHeap value */ \
                /* then do the comparison with the MinHeap value */ \
                if (R_index == _heap_->size - 1 && _heap_->count % 2 != 0) \
                    child = _heap_->f_val->cmp((*L)[1], (*R)[0]) > 0 ? L_index : R_index; \
                else \
                    child = _heap_->f_val->cmp((*L)[1], (*R)[1]) > 0 ? L_index : R_index; \
            } \
            /* Pick the only one available */ \
            else \
                child = L_index; \
\
            V(*child_node)[2] = &(_heap_->buffer[child]); \
\
            /* Again, check if the child node is the last one and has no */ \
            /* MaxHeap value */ \
            if (child == _heap_->size - 1 && _heap_->count % 2 != 0) \
            { \
                /* Odd case, compare with MinHeap value */ \
                /* If current value is not less than the child node's */ \
                /* value it is done */ \
                if (_heap_->f_val->cmp((*curr_node)[1], (*child_node)[0]) >= 0) \
                    break; \
\
                /* Otherwise swap and continue */ \
                V tmp = (*child_node)[0]; \
                (*child_node)[0] = (*curr_node)[1]; \
                (*curr_node)[1] = tmp; \
            } \
            else \
            { \
                /* If current value is not less than the child node's   */ \
                /* value it is done */ \
                if (_heap_->f_val->cmp((*curr_node)[1], (*child_node)[1]) >= 0) \
                    break; \
\
                /* Otherwise swap and continue */ \
                V tmp = (*child_node)[1]; \
                (*child_node)[1] = (*curr_node)[1]; \
                (*curr_node)[1] = tmp; \
\
                /* Check if the MinHeap and MaxHeap values need to be */ \
                /* swapped                  */ \
                if (_heap_->f_val->cmp((*child_node)[0], (*child_node)[1]) > 0) \
                { \
                    /* Swap because the MinHeap value is greater than the */ \
                    /* MaxHeap value */ \
                    tmp = (*child_node)[0]; \
                    (*child_node)[0] = (*child_node)[1]; \
                    (*child_node)[1] = tmp; \
                } \
            } \
\
            /* Update index and node */ \
            index = child; \
            curr_node = child_node; \
        } \
    } \
\
    static void CMC_(PFX, _impl_float_down_min)(struct SNAME * _heap_) \
    { \
        /* Floats Down from the MinHeap */ \
        size_t index = 0; \
\
        V(*curr_node)[2] = &(_heap_->buffer[index]); \
\
        while (true) \
        { \
            /* If there are no children it is done */ \
            if (2 * index + 1 >= _heap_->size) \
                break; \
\
            /* Child index to be compared with current value */ \
            size_t child; \
            size_t L_index = index * 2 + 1; \
            size_t R_index = index * 2 + 2; \
\
            /* If there are two children, pick the smallest one */ \
            if (R_index < _heap_->size) \
            { \
                V(*L)[2] = &(_heap_->buffer[L_index]); \
                V(*R)[2] = &(_heap_->buffer[R_index]); \
\
                child = _heap_->f_val->cmp((*L)[0], (*R)[0]) < 0 ? L_index : R_index; \
            } \
            /* Pick the only one available */ \
            else \
                child = L_index; \
\
            V(*child_node)[2] = &(_heap_->buffer[child]); \
\
            /* If current value is smaller than the child node's value, it */ \
            /* is done */ \
            if (_heap_->f_val->cmp((*curr_node)[0], (*child_node)[0]) < 0) \
                break; \
\
            /* Otherwise swap and continue */ \
            V tmp = (*child_node)[0]; \
            (*child_node)[0] = (*curr_node)[0]; \
            (*curr_node)[0] = tmp; \
\
            /* If the child node is the last node check if the MinHeap and */ \
            /* MaxHeap values need to be swapped */ \
            if (child != _heap_->size - 1 || _heap_->count % 2 == 0) \
            { \
                if (_heap_->f_val->cmp((*child_node)[0], (*child_node)[1]) > 0) \
                { \
                    /* Swap because the MinHeap value is greater than the */ \
                    /* MaxHeap value */ \
                    tmp = (*child_node)[0]; \
                    (*child_node)[0] = (*child_node)[1]; \
                    (*child_node)[1] = tmp; \
                } \
            } \
\
            /* Update index and node */ \
            index = child; \
            curr_node = child_node; \
        } \
    }

#endif /* CMC_CMC_INTERVALHEAP_H */
