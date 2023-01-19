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

/* Collection Allocation and Deallocation */
struct SNAME *CMC_(PFX, _new)(size_t n_bits);
struct SNAME *CMC_(PFX, _new_custom)(size_t n_bits, struct CMC_ALLOC_NODE_NAME *alloc,
                                     struct CMC_CALLBACKS_NAME *callbacks);
struct SNAME CMC_(PFX, _init)(size_t n_bits);
struct SNAME CMC_(PFX, _init_custom)(size_t n_bits, struct CMC_ALLOC_NODE_NAME *alloc,
                                     struct CMC_CALLBACKS_NAME *callbacks);
void CMC_(PFX, _free)(struct SNAME *_bitset_);
void CMC_(PFX, _release)(struct SNAME _bitset_);
void CMC_(PFX, _customize)(struct SNAME *_bitset_, struct CMC_ALLOC_NODE_NAME *alloc,
                           struct CMC_CALLBACKS_NAME *callbacks);
/* Collection Input and Output */
bool CMC_(PFX, _set)(struct SNAME *_bitset_, size_t bit_index);
bool CMC_(PFX, _set_range)(struct SNAME *_bitset_, size_t from, size_t to);
bool CMC_(PFX, _clear)(struct SNAME *_bitset_, size_t bit_index);
bool CMC_(PFX, _clear_range)(struct SNAME *_bitset_, size_t from, size_t to);
bool CMC_(PFX, _flip)(struct SNAME *_bitset_, size_t bit_index);
bool CMC_(PFX, _flip_range)(struct SNAME *_bitset_, size_t from, size_t to);
bool CMC_(PFX, _put)(struct SNAME *_bitset_, size_t bit_index, bool state);
bool CMC_(PFX, _put_range)(struct SNAME *_bitset_, size_t from, size_t to, bool state);
bool CMC_(PFX, _set_all)(struct SNAME *_bitset_);
bool CMC_(PFX, _clear_all)(struct SNAME *_bitset_);
bool CMC_(PFX, _flip_all)(struct SNAME *_bitset_);
/* Element Access */
bool CMC_(PFX, _get)(struct SNAME *_bitset_, size_t bit_index);
/* Collection State */
/* Collection Utility */
bool CMC_(PFX, _resize)(struct SNAME *_bitset_, size_t n_bits);
