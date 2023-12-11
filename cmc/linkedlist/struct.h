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

/* Linked List Structure */
struct SNAME
{
    /* First node in the list */
    struct CMC_DEF_NODE(SNAME) * head;

    /* Last node in the list */
    struct CMC_DEF_NODE(SNAME) * tail;

    /* Current amount of elements in the list */
    size_t count;

    /* Flags indicating errors or success */
    int flag;

    /* Value function table */
    struct CMC_DEF_FVAL(SNAME) * f_val;

    /* Custom allocation functions */
    CMC_ALLOC_TYPE alloc;

    /* Custom callback functions */
    CMC_CALLBACKS_DECL;
};

/* Doubly-linked list node */
struct CMC_DEF_NODE(SNAME)
{
    /* Node's value */
    V value;

    /* Pointer to the next node on the linked list */
    struct CMC_DEF_NODE(SNAME) * next;

    /* Pointer to the previous node on the linked list */
    struct CMC_DEF_NODE(SNAME) * prev;
};
