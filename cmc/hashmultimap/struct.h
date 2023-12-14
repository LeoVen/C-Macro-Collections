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

/* HashMultimap Structure */
struct SNAME
{
    /* Array of linked list to entries */
    struct CMC_DEF_ENTRY(SNAME) * (*buffer)[2];
    /* Current array capacity */
    size_t capacity;
    /* Current amount of keys */
    size_t count;
    /* Load factor in range (0.0, infinity) */
    double load;
    /* Flags indicating errors or success */
    int flag;
    /* Key function table */
    struct CMC_DEF_FKEY(SNAME) * f_key;
    /* Value function table */
    struct CMC_DEF_FVAL(SNAME) * f_val;
    /* Custom allocation functions */
    CMC_ALLOC_TYPE alloc;
    /* Custom callback functions */
    CMC_CALLBACKS_DECL;
};

/* HashMultimap Entry */
struct CMC_DEF_ENTRY(SNAME)
{
    /* Entry Key */
    K key;
    /* Entry Value */
    V value;
    /* Next entry on the linked list */
    struct CMC_DEF_ENTRY(SNAME) * next;
    /* Previous entry on the linked list */
    struct CMC_DEF_ENTRY(SNAME) * prev;
};

/**
 * ITER
 *
 * Hashmultimap bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

/* HashMultiMap Iterator */
struct CMC_DEF_ITER(SNAME)
{
    /* Target hashmultimap */
    struct SNAME *target;
    /* Current entry */
    struct CMC_DEF_ENTRY(SNAME) * curr_entry;
    /* Cursor`s position (index) */
    size_t cursor;
    /* Keeps track of relative index to the iteration of elements */
    size_t index;
    /* The index of the first element */
    size_t first;
    /* The index of the last element */
    size_t last;
    /* If the iterator has reached the start of the iteration */
    bool start;
    /* If the iterator has reached the end of the iteration */
    bool end;
};

#endif /* CMC_EXT_ITER */
