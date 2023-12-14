/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Library.
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

/* Key struct function table */
struct CMC_DEF_FKEY(SNAME)
{
    /* Comparator function */
    CMC_DEF_FTAB_CMP(K);
    /* Copy function */
    CMC_DEF_FTAB_CPY(K);
    /* To string function */
    CMC_DEF_FTAB_STR(K);
    /* Free from memory function */
    CMC_DEF_FTAB_FREE(K);
    /* Hash function */
    CMC_DEF_FTAB_HASH(K);
    /* Priority function */
    CMC_DEF_FTAB_PRI(K);
};

/* Value struct function table */
struct CMC_DEF_FVAL(SNAME)
{
    /* Comparator function */
    CMC_DEF_FTAB_CMP(V);
    /* Copy function */
    CMC_DEF_FTAB_CPY(V);
    /* To string function */
    CMC_DEF_FTAB_STR(V);
    /* Free from memory function */
    CMC_DEF_FTAB_FREE(V);
    /* Hash function */
    CMC_DEF_FTAB_HASH(V);
    /* Priority function */
    CMC_DEF_FTAB_PRI(V);
};

/* Collection Allocation and Deallocation */
struct SNAME *CMC_(PFX, _new)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
                              struct CMC_DEF_FVAL(SNAME) * f_val);
struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
                                     struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE alloc,
                                     CMC_CALLBACK_TYPE callbacks);
void CMC_(PFX, _clear)(struct SNAME *_map_);
void CMC_(PFX, _free)(struct SNAME *_map_);
/* Collection Allocation and Deallocation */
struct SNAME CMC_(PFX, _init)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
                              struct CMC_DEF_FVAL(SNAME) * f_val);
struct SNAME CMC_(PFX, _init_custom)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
                                     struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE alloc,
                                     CMC_CALLBACK_TYPE callbacks);
void CMC_(PFX, _release)(struct SNAME _map_);
/* Customization of Allocation and Callbacks */
void CMC_(PFX, _customize)(struct SNAME *_map_, CMC_ALLOC_TYPE alloc, CMC_CALLBACK_TYPE callbacks);
/* Collection Input and Output */
bool CMC_(PFX, _insert)(struct SNAME *_map_, K key, V value);
bool CMC_(PFX, _update_key)(struct SNAME *_map_, V val, K new_key);
bool CMC_(PFX, _update_val)(struct SNAME *_map_, K key, V new_val);
bool CMC_(PFX, _remove_by_key)(struct SNAME *_map_, K key, K *out_key, V *out_val);
bool CMC_(PFX, _remove_by_val)(struct SNAME *_map_, V val, K *out_key, V *out_val);
/* Element Access */
K CMC_(PFX, _get_key)(struct SNAME *_map_, V val);
V CMC_(PFX, _get_val)(struct SNAME *_map_, K key);
/* Collection State */
bool CMC_(PFX, _contains_key)(struct SNAME *_map_, K key);
bool CMC_(PFX, _contains_val)(struct SNAME *_map_, V val);
bool CMC_(PFX, _empty)(struct SNAME *_map_);
bool CMC_(PFX, _full)(struct SNAME *_map_);
size_t CMC_(PFX, _count)(struct SNAME *_map_);
size_t CMC_(PFX, _capacity)(struct SNAME *_map_);
double CMC_(PFX, _load)(struct SNAME *_map_);
int CMC_(PFX, _flag)(struct SNAME *_map_);
/* Collection Utility */
bool CMC_(PFX, _resize)(struct SNAME *_map_, size_t capacity);
struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_map_);
bool CMC_(PFX, _equals)(struct SNAME *_map1_, struct SNAME *_map2_);

/**
 * ITER
 *
 * Hashbidimap bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

/* Iterator Functions */
/* Iterator Allocation and Deallocation */
struct CMC_DEF_ITER(SNAME) * CMC_(PFX, _iter_new)(struct SNAME *target);
void CMC_(PFX, _iter_free)(struct CMC_DEF_ITER(SNAME) * iter);
/* Iterator Initialization */
void CMC_(PFX, _iter_init)(struct CMC_DEF_ITER(SNAME) * iter, struct SNAME *target);
/* Iterator State */
bool CMC_(PFX, _iter_start)(struct CMC_DEF_ITER(SNAME) * iter);
bool CMC_(PFX, _iter_end)(struct CMC_DEF_ITER(SNAME) * iter);
/* Iterator Movement */
void CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter);
void CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter);
bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter);
bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter);
bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps);
bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps);
bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter, size_t index);
/* Iterator Access */
K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter);
V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter);
size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

#endif /* CMC_EXT_ITER */

/**
 * STR
 *
 * Print helper functions.
 */
#ifdef CMC_EXT_STR

/* Debug prints the struct to fptr */
bool CMC_(PFX, _to_string)(struct SNAME *_map_, FILE *fptr);
/* Prints each item in the hashbidimap as an array */
bool CMC_(PFX, _print)(struct SNAME *_map_, FILE *fptr, const char *start, const char *separator, const char *end,
                       const char *key_val_sep);

#endif /* CMC_EXT_STR */
