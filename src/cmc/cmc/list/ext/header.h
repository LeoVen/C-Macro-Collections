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
 * INIT
 *
 * The part 'INIT' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers).
 */
#ifdef CMC_EXT_INIT

#ifdef CMC_SAC
struct SNAME CMC_(PFX, _init)(struct CMC_DEF_FVAL(SNAME) * f_val);
struct SNAME CMC_(PFX, _init_custom)(struct CMC_DEF_FVAL(SNAME) * f_val, struct CMC_ALLOC_NODE_NAME *alloc,
                                     struct CMC_CALLBACKS_NAME *callbacks);
#else
struct SNAME CMC_(PFX, _init)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val);
struct SNAME CMC_(PFX, _init_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,
                                     struct CMC_ALLOC_NODE_NAME *alloc, struct CMC_CALLBACKS_NAME *callbacks);
#endif
void CMC_(PFX, _release)(struct SNAME _list_);

#endif /* CMC_EXT_INIT */

/**
 * ITER
 *
 * List bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

/* Iterator Initialization */
struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME *target);
struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME *target);
/* Iterator State */
bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter);
bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter);
/* Iterator Movement */
bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter);
bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter);
bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter);
bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter);
bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps);
bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps);
bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter, size_t index);
/* Iterator Access */
V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter);
V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter);
size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#endif /* CMC_EXT_ITER */

/**
 * SEQ
 *
 * Push and pop sequence of items.
 */
#ifdef CMC_EXT_SEQ

/* List Sequence Input and Output */
bool CMC_(PFX, _seq_push_front)(struct SNAME *_list_, V *values, size_t size);
bool CMC_(PFX, _seq_push_at)(struct SNAME *_list_, V *values, size_t size, size_t index);
bool CMC_(PFX, _seq_push_back)(struct SNAME *_list_, V *values, size_t size);
bool CMC_(PFX, _seq_pop_at)(struct SNAME *_list_, size_t from, size_t to);
struct SNAME *CMC_(PFX, _seq_sublist)(struct SNAME *_list_, size_t from, size_t to);

#endif /* CMC_EXT_SEQ */

/**
 * STR
 *
 * Print helper functions.
 */
#ifdef CMC_EXT_STR

/* Debug prints the struct to fptr */
bool CMC_(PFX, _to_string)(struct SNAME *_list_, FILE *fptr);
/* Prints each item in the list as an array */
bool CMC_(PFX, _print)(struct SNAME *_list_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_EXT_STR */
