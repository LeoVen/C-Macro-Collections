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

/* Value function table */
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
struct SNAME *CMC_(PFX, _new)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val);
struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE alloc,
                                     CMC_CALLBACK_TYPE callbacks);
void CMC_(PFX, _clear)(struct SNAME *_queue_);
void CMC_(PFX, _free)(struct SNAME *_queue_);
/* Customization of Allocation and Callbacks */
void CMC_(PFX, _customize)(struct SNAME *_queue_, CMC_ALLOC_TYPE alloc, CMC_CALLBACK_TYPE callbacks);
/* Collection Input and Output */
bool CMC_(PFX, _enqueue)(struct SNAME *_queue_, V value);
bool CMC_(PFX, _dequeue)(struct SNAME *_queue_);
/* Element Access */
V CMC_(PFX, _peek)(struct SNAME *_queue_);
/* Collection State */
bool CMC_(PFX, _contains)(struct SNAME *_queue_, V value);
bool CMC_(PFX, _empty)(struct SNAME *_queue_);
bool CMC_(PFX, _full)(struct SNAME *_queue_);
size_t CMC_(PFX, _count)(struct SNAME *_queue_);
size_t CMC_(PFX, _capacity)(struct SNAME *_queue_);
int CMC_(PFX, _flag)(struct SNAME *_queue_);
/* Collection Utility */
bool CMC_(PFX, _resize)(struct SNAME *_queue_, size_t capacity);
struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_queue_);
bool CMC_(PFX, _equals)(struct SNAME *_queue1_, struct SNAME *_queue2_);
