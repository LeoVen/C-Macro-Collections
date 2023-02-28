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

/* Collection Functions */
/* Collection Allocation and Deallocation */
struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FKEY(SNAME) * f_key, struct CMC_DEF_FVAL(SNAME) * f_val);
struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FKEY(SNAME) * f_key, struct CMC_DEF_FVAL(SNAME) * f_val,
                                     CMC_ALLOC_TYPE *alloc, CMC_CALLBACK_TYPE callbacks);
void CMC_(PFX, _clear)(struct SNAME *_map_);
void CMC_(PFX, _free)(struct SNAME *_map_);
/* Customization of Allocation and Callbacks */
void CMC_(PFX, _customize)(struct SNAME *_map_, CMC_ALLOC_TYPE *alloc, CMC_CALLBACK_TYPE callbacks);
/* Collection Input and Output */
bool CMC_(PFX, _insert)(struct SNAME *_map_, K key, V value);
bool CMC_(PFX, _update)(struct SNAME *_map_, K key, V new_value, V *old_value);
bool CMC_(PFX, _remove)(struct SNAME *_map_, K key, V *out_value);
/* Element Access */
bool CMC_(PFX, _max)(struct SNAME *_map_, K *key, V *value);
bool CMC_(PFX, _min)(struct SNAME *_map_, K *key, V *value);
V CMC_(PFX, _get)(struct SNAME *_map_, K key);
V *CMC_(PFX, _get_ref)(struct SNAME *_map_, K key);
/* Collection State */
bool CMC_(PFX, _contains)(struct SNAME *_map_, K key);
bool CMC_(PFX, _empty)(struct SNAME *_map_);
size_t CMC_(PFX, _count)(struct SNAME *_map_);
int CMC_(PFX, _flag)(struct SNAME *_map_);
/* Collection Utility */
struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_map_);
bool CMC_(PFX, _equals)(struct SNAME *_map1_, struct SNAME *_map2_);
