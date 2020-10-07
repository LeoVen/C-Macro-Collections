/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Libray.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * cmc_bitset.h
 *
 * Creation Date: 30/04/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * BitSet
 *
 * A Bit Set is an array where each bit can be individually modified and queried
 * by using bitwise operators such as |, &, ^, ~, >> and << (or, and, xor, not,
 * right shift, left shift respectively).
 *
 * Implementation
 *
 * This BitSet implementation uses an array of type cmc_bitset_word which can be
 * typedefed to any unsigned type such as uint8_t, uint16_t, uint32_t, uint64_t,
 * size_t, etc. The BitSet does not make use of K or V. Because of that, it also
 * doesn't have Functions Tables.
 *
 * The BitSet is initialized with a custom capacity but, if a bit index accessed
 * is greater than the total capacity, the BitSet will resize. This means that
 * the BitSet will try to guarantee that every bit index is accessible, as long
 * as there is enough memory.
 */

#ifndef CMC_CMC_BITSET_H
#define CMC_CMC_BITSET_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "cor_core.h"

/* -------------------------------------------------------------------------
 * BitSet Implementation
 * ------------------------------------------------------------------------- */
#include "cor_bitset.h"

/**
 * Core BitSet implementation
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_CMC_BITSET_CORE(ACCESS, FILE, PARAMS) CMC_(CMC_(CMC_CMC_BITSET_CORE_, ACCESS), CMC_(_, FILE))(PARAMS)

/* PRIVATE or PUBLIC solver */
#define CMC_CMC_BITSET_CORE_PUBLIC_HEADER(PARAMS) \
    CMC_CMC_BITSET_CORE_STRUCT(PARAMS) \
    CMC_CMC_BITSET_CORE_HEADER(PARAMS)

#define CMC_CMC_BITSET_CORE_PUBLIC_SOURCE(PARAMS) CMC_CMC_BITSET_CORE_SOURCE(PARAMS)

#define CMC_CMC_BITSET_CORE_PRIVATE_HEADER(PARAMS) \
    struct CMC_PARAM_SNAME(PARAMS); \
    CMC_CMC_BITSET_CORE_HEADER(PARAMS)

#define CMC_CMC_BITSET_CORE_PRIVATE_SOURCE(PARAMS) \
    CMC_CMC_BITSET_CORE_STRUCT(PARAMS) \
    CMC_CMC_BITSET_CORE_SOURCE(PARAMS)

/* Lowest level API */
#define CMC_CMC_BITSET_CORE_STRUCT(PARAMS) CMC_CMC_BITSET_CORE_STRUCT_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS))

#define CMC_CMC_BITSET_CORE_HEADER(PARAMS) CMC_CMC_BITSET_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS))

#define CMC_CMC_BITSET_CORE_SOURCE(PARAMS) CMC_CMC_BITSET_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS))

/* -------------------------------------------------------------------------
 * Struct
 * ------------------------------------------------------------------------- */
#define CMC_CMC_BITSET_CORE_STRUCT_(PFX, SNAME) \
\
    /* BitSet Structure */ \
    struct SNAME \
    { \
        /* Dynamic Array of Bits */ \
        cmc_bitset_word *buffer; \
\
        /* Current array capacity */ \
        size_t capacity; \
\
        /* Currently used bits */ \
        /* This should always be true: */ \
        /* capacity - count < sizeof(cmc_bitset_word) * CHAR_BIT */ \
        size_t count; \
\
        /* Flags indicating errors or success */ \
        int flag; \
\
        /* Custom allocation functions */ \
        struct CMC_ALLOC_NODE_NAME *alloc; \
\
        /* Custom callback functions */ \
        CMC_CALLBACKS_DECL; \
    };

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_BITSET_CORE_HEADER_(PFX, SNAME) \
\
    /* BitSet Iterator */ \
    struct CMC_DEF_ITER(SNAME) \
    { \
        /* Target bitset */ \
        struct SNAME *target; \
\
        /* Cursor's position (index) */ \
        size_t cursor; \
\
        /* If the iterator has reached the start of the iteration */ \
        bool start; \
\
        /* If the iterator has reached the end of the iteration */ \
        bool end; \
    }; \
\
    /* Collection Functions */ \
    /* Collection Allocation and Deallocation */ \
    struct SNAME *CMC_(PFX, _new)(size_t n_bits); \
    struct SNAME *CMC_(PFX, _new_custom)(size_t n_bits, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks); \
    struct SNAME CMC_(PFX, _init)(size_t n_bits); \
    struct SNAME CMC_(PFX, _init_custom)(size_t n_bits, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks); \
    void CMC_(PFX, _free)(struct SNAME * _bitset_); \
    void CMC_(PFX, _release)(struct SNAME _bitset_); \
    void CMC_(PFX, _customize)(struct SNAME * _bitset_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks); \
    /* Collection Input and Output */ \
    bool CMC_(PFX, _set)(struct SNAME * _bitset_, size_t bit_index); \
    bool CMC_(PFX, _set_range)(struct SNAME * _bitset_, size_t from, size_t to); \
    bool CMC_(PFX, _clear)(struct SNAME * _bitset_, size_t bit_index); \
    bool CMC_(PFX, _clear_range)(struct SNAME * _bitset_, size_t from, size_t to); \
    bool CMC_(PFX, _flip)(struct SNAME * _bitset_, size_t bit_index); \
    bool CMC_(PFX, _flip_range)(struct SNAME * _bitset_, size_t from, size_t to); \
    bool CMC_(PFX, _put)(struct SNAME * _bitset_, size_t bit_index, bool state); \
    bool CMC_(PFX, _put_range)(struct SNAME * _bitset_, size_t from, size_t to, bool state); \
    bool CMC_(PFX, _set_all)(struct SNAME * _bitset_); \
    bool CMC_(PFX, _clear_all)(struct SNAME * _bitset_); \
    bool CMC_(PFX, _flip_all)(struct SNAME * _bitset_); \
    /* Element Access */ \
    bool CMC_(PFX, _get)(struct SNAME * _bitset_, size_t bit_index); \
    /* Collection State */ \
    /* Collection Utility */ \
    bool CMC_(PFX, _resize)(struct SNAME * _bitset_, size_t n_bits); \
\
    /* Iterator Functions */ \
    /* Iterator Initialization */ \
    /* Iterator State */ \
    /* Iterator Movement */ \
    /* Iterator Access */

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_BITSET_CORE_SOURCE_(PFX, SNAME) \
\
    /* Implementation Detail Functions */ \
    bool CMC_(PFX, _impl_resize)(struct SNAME * _bitset_, size_t n_bits, bool do_resize); \
\
    struct SNAME *CMC_(PFX, _new)(size_t n_bits) \
    { \
        return CMC_(PFX, _new_custom)(n_bits, NULL, NULL); \
    } \
\
    struct SNAME *CMC_(PFX, _new_custom)(size_t n_bits, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (n_bits < 1) \
            return NULL; \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        size_t capacity = cmc_bidx_to_widx(n_bits - 1) + 1; \
\
        struct SNAME *_bitset_ = alloc->malloc(sizeof(struct SNAME)); \
\
        if (!_bitset_) \
            return NULL; \
\
        _bitset_->buffer = alloc->calloc(capacity, sizeof(cmc_bitset_word)); \
\
        if (!_bitset_->buffer) \
        { \
            alloc->free(_bitset_); \
            return NULL; \
        } \
\
        _bitset_->capacity = capacity; \
        _bitset_->flag = CMC_FLAG_OK; \
        _bitset_->alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(_bitset_, callbacks); \
\
        return _bitset_; \
    } \
\
    struct SNAME CMC_(PFX, _init)(size_t n_bits) \
    { \
        return CMC_(PFX, _init_custom)(n_bits, NULL, NULL); \
    } \
\
    struct SNAME CMC_(PFX, _init_custom)(size_t n_bits, struct CMC_ALLOC_NODE_NAME * alloc, \
                                         struct CMC_CALLBACKS_NAME * CMC_UNUSED(callbacks)) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        struct SNAME _bitset_ = { 0 }; \
\
        if (n_bits < 1) \
            return _bitset_; \
\
        if (!alloc) \
            alloc = &cmc_alloc_node_default; \
\
        size_t capacity = cmc_bidx_to_widx(n_bits - 1) + 1; \
\
        _bitset_.buffer = alloc->calloc(capacity, sizeof(cmc_bitset_word)); \
\
        if (!_bitset_.buffer) \
            return _bitset_; \
\
        _bitset_.capacity = capacity; \
        _bitset_.flag = CMC_FLAG_OK; \
        _bitset_.alloc = alloc; \
        CMC_CALLBACKS_ASSIGN(&_bitset_, callbacks); \
\
        return _bitset_; \
    } \
\
    void CMC_(PFX, _free)(struct SNAME * _bitset_) \
    { \
        free(_bitset_->buffer); \
        free(_bitset_); \
    } \
\
    void CMC_(PFX, _release)(struct SNAME _bitset_) \
    { \
        free(_bitset_.buffer); \
    } \
\
    void CMC_(PFX, _customize)(struct SNAME * _bitset_, struct CMC_ALLOC_NODE_NAME * alloc, \
                               struct CMC_CALLBACKS_NAME * callbacks) \
    { \
        CMC_CALLBACKS_MAYBE_UNUSED(callbacks); \
\
        if (!alloc) \
            _bitset_->alloc = &cmc_alloc_node_default; \
        else \
            _bitset_->alloc = alloc; \
\
        CMC_CALLBACKS_ASSIGN(_bitset_, callbacks); \
\
        _bitset_->flag = CMC_FLAG_OK; \
    } \
\
    bool CMC_(PFX, _set)(struct SNAME * _bitset_, size_t bit_index) \
    { \
        if (!CMC_(PFX, _impl_resize)(_bitset_, bit_index + 1, false)) \
            return false; \
\
        /* How many bits in a word */ \
        const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT; \
\
        size_t i = cmc_bidx_to_widx(bit_index); \
\
        _bitset_->buffer[i] |= ((cmc_bitset_word)1) << (bit_index % bits); \
\
        _bitset_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_bitset_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _set_range)(struct SNAME * _bitset_, size_t from, size_t to) \
    { \
        if (to < from) \
        { \
            _bitset_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        if (!CMC_(PFX, _impl_resize)(_bitset_, to + 1, false)) \
            return false; \
\
        size_t start_index = cmc_bidx_to_widx(from); \
        size_t end_index = cmc_bidx_to_widx(to); \
\
        /* How many bits in a word */ \
        const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT; \
\
        /* 1111 ... 1111 */ \
        const cmc_bitset_word ones = ~((cmc_bitset_word)0); \
\
        cmc_bitset_word shift_start = from % bits; \
        cmc_bitset_word shift_end = (cmc_bitset_word)(-(to + 1)) % bits; \
\
        cmc_bitset_word start_mask = ones << shift_start; \
        cmc_bitset_word end_mask = ones >> shift_end; \
\
        if (start_index == end_index) \
        { \
            _bitset_->buffer[end_index] |= (start_mask & end_mask); \
        } \
        else \
        { \
            _bitset_->buffer[start_index] |= start_mask; \
\
            for (size_t i = start_index + 1; i < end_index; i++) \
                _bitset_->buffer[i] |= ~((cmc_bitset_word)0); \
\
            _bitset_->buffer[end_index] |= end_mask; \
        } \
\
        _bitset_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_bitset_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _clear)(struct SNAME * _bitset_, size_t bit_index) \
    { \
        if (!CMC_(PFX, _impl_resize)(_bitset_, bit_index + 1, false)) \
            return false; \
\
        /* How many bits in a word */ \
        const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT; \
\
        size_t i = cmc_bidx_to_widx(bit_index); \
\
        _bitset_->buffer[i] &= ~(((cmc_bitset_word)1) << (bit_index % bits)); \
\
        _bitset_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_bitset_, delete); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _clear_range)(struct SNAME * _bitset_, size_t from, size_t to) \
    { \
        if (to < from) \
        { \
            _bitset_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        if (!CMC_(PFX, _impl_resize)(_bitset_, to + 1, false)) \
            return false; \
\
        size_t start_index = cmc_bidx_to_widx(from); \
        size_t end_index = cmc_bidx_to_widx(to); \
\
        /* How many bits in a word */ \
        const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT; \
\
        /* 1111 ... 1111 */ \
        const cmc_bitset_word ones = ~((cmc_bitset_word)0); \
\
        cmc_bitset_word shift_start = from % bits; \
        cmc_bitset_word shift_end = (cmc_bitset_word)(-(to + 1)) % bits; \
\
        cmc_bitset_word start_mask = ones << shift_start; \
        cmc_bitset_word end_mask = ones >> shift_end; \
\
        if (start_index == end_index) \
        { \
            _bitset_->buffer[end_index] &= ~(start_mask & end_mask); \
        } \
        else \
        { \
            _bitset_->buffer[start_index] &= ~start_mask; \
\
            for (size_t i = start_index + 1; i < end_index; i++) \
                _bitset_->buffer[i] = 0; \
\
            _bitset_->buffer[end_index] &= ~end_mask; \
        } \
\
        _bitset_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_bitset_, delete); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _flip)(struct SNAME * _bitset_, size_t bit_index) \
    { \
        if (!CMC_(PFX, _impl_resize)(_bitset_, bit_index + 1, false)) \
            return false; \
\
        /* How many bits in a word */ \
        const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT; \
\
        size_t i = cmc_bidx_to_widx(bit_index); \
\
        _bitset_->buffer[i] ^= ((cmc_bitset_word)1) << (bit_index % bits); \
\
        _bitset_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_bitset_, update); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _flip_range)(struct SNAME * _bitset_, size_t from, size_t to) \
    { \
        if (to < from) \
        { \
            _bitset_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        if (!CMC_(PFX, _impl_resize)(_bitset_, to + 1, false)) \
            return false; \
\
        size_t start_index = cmc_bidx_to_widx(from); \
        size_t end_index = cmc_bidx_to_widx(to); \
\
        /* How many bits in a word */ \
        const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT; \
\
        /* 1111 ... 1111 */ \
        const cmc_bitset_word ones = ~((cmc_bitset_word)0); \
\
        cmc_bitset_word shift_start = from % bits; \
        cmc_bitset_word shift_end = (cmc_bitset_word)(-(to + 1)) % bits; \
\
        cmc_bitset_word start_mask = ones << shift_start; \
        cmc_bitset_word end_mask = ones >> shift_end; \
\
        if (start_index == end_index) \
        { \
            _bitset_->buffer[end_index] ^= (start_mask & end_mask); \
        } \
        else \
        { \
            _bitset_->buffer[start_index] ^= start_mask; \
\
            for (size_t i = start_index + 1; i < end_index; i++) \
                _bitset_->buffer[i] ^= ones; \
\
            _bitset_->buffer[end_index] ^= end_mask; \
        } \
\
        _bitset_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_bitset_, update); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _put)(struct SNAME * _bitset_, size_t bit_index, bool state) \
    { \
        if (state) \
            return CMC_(PFX, _set)(_bitset_, bit_index); \
        else \
            return CMC_(PFX, _clear)(_bitset_, bit_index); \
    } \
\
    bool CMC_(PFX, _put_range)(struct SNAME * _bitset_, size_t from, size_t to, bool state) \
    { \
        if (state) \
            return CMC_(PFX, _set_range)(_bitset_, from, to); \
        else \
            return CMC_(PFX, _clear_range)(_bitset_, from, to); \
    } \
\
    bool CMC_(PFX, _set_all)(struct SNAME * _bitset_) \
    { \
        for (size_t i = 0; i < _bitset_->capacity; i++) \
            _bitset_->buffer[i] = ~((cmc_bitset_word)0); \
\
        _bitset_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_bitset_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _clear_all)(struct SNAME * _bitset_) \
    { \
        for (size_t i = 0; i < _bitset_->capacity; i++) \
            _bitset_->buffer[i] = 0; \
\
        _bitset_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_bitset_, delete); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _flip_all)(struct SNAME * _bitset_) \
    { \
        for (size_t i = 0; i < _bitset_->capacity; i++) \
            _bitset_->buffer[i] ^= ~((cmc_bitset_word)0); \
\
        _bitset_->flag = CMC_FLAG_OK; \
\
        CMC_CALLBACKS_CALL(_bitset_, create); \
\
        return true; \
    } \
\
    bool CMC_(PFX, _get)(struct SNAME * _bitset_, size_t bit_index) \
    { \
        /* How many bits in a word */ \
        const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT; \
\
        cmc_bitset_word w = _bitset_->buffer[cmc_bidx_to_widx(bit_index)]; \
\
        return w & ((cmc_bitset_word)1 << (bit_index % bits)); \
    } \
\
    bool CMC_(PFX, _resize)(struct SNAME * _bitset_, size_t n_bits) \
    { \
        return CMC_(PFX, _impl_resize)(_bitset_, n_bits, true); \
    } \
\
    /* When do_resize is false, then we just want to assure that the bitset */ \
    /* can accommodate n_bits */ \
    bool CMC_(PFX, _impl_resize)(struct SNAME * _bitset_, size_t n_bits, bool do_resize) \
    { \
        if (n_bits == 0) \
        { \
            _bitset_->flag = CMC_FLAG_INVALID; \
            return false; \
        } \
\
        /* -1 because n_bits is 1-based and we need to pass a 0-based index */ \
        /* +1 to have a 1-based result */ \
        size_t words = cmc_bidx_to_widx(n_bits - 1) + 1; \
\
        /* Be sure we are not always shrinking when we just want to make */ \
        /* sure that we have enough size for n_bits */ \
        if (!do_resize && words <= _bitset_->capacity) \
            return true; \
\
        cmc_bitset_word *new_buffer = realloc(_bitset_->buffer, words * sizeof(cmc_bitset_word)); \
\
        if (!new_buffer) \
        { \
            _bitset_->flag = CMC_FLAG_ALLOC; \
            return false; \
        } \
\
        _bitset_->buffer = new_buffer; \
\
        /* Make new bits be all zeroes */ \
        if (_bitset_->capacity < words) \
            memset(_bitset_->buffer + _bitset_->capacity, 0, (words - _bitset_->capacity) * sizeof(cmc_bitset_word)); \
\
        _bitset_->capacity = words; \
\
        CMC_CALLBACKS_CALL(_bitset_, resize); \
\
        _bitset_->flag = CMC_FLAG_OK; \
\
        return true; \
    }

#endif /* CMC_CMC_BITSET_H */
