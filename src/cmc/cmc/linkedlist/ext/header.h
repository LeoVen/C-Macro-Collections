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

/**
 * INIT
 *
 * The part 'INIT' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers).
 */
#ifdef CMC_EXT_INIT

#endif /* CMC_EXT_INIT */

/**
 * ITER
 *
 * List bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

/* LinkedList Iterator */
struct CMC_DEF_ITER(SNAME)
{
    /* Target Linked List */
    struct SNAME *target;
    /* Cursor's current node */
    struct CMC_DEF_NODE(SNAME) * cursor;
    /* Keeps track of relative index to the iteration of elements */
    size_t index;
    /* If the iterator has reached the start of the iteration */
    bool start;
    /* If the iterator has reached the end of the iteration */
    bool end;
};

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
struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _iter_node)(struct CMC_DEF_ITER(SNAME) * iter);

#endif /* CMC_EXT_ITER */

/**
 * NODE
 *
 * Operations relative to a linkedlist node
 */
#ifdef CMC_EXT_NODE

/* Node Allocation and Deallocation */
struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _new_node)(struct SNAME *_list_, V value);
void CMC_(PFX, _free_node)(struct SNAME *_list_, struct CMC_DEF_NODE(SNAME) * _node_);
/* Node Access Relative to a Linked List */
struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _head)(struct SNAME *_list_);
struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _get_node)(struct SNAME *_list_, size_t index);
struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _tail)(struct SNAME *_list_);
/* Input and Output Relative to a Node */
bool CMC_(PFX, _add_next)(struct SNAME *_owner_, struct CMC_DEF_NODE(SNAME) * _node_, V value);
bool CMC_(PFX, _add_prev)(struct SNAME *_owner_, struct CMC_DEF_NODE(SNAME) * _node_, V value);
bool CMC_(PFX, _del_next)(struct SNAME *_owner_, struct CMC_DEF_NODE(SNAME) * _node_);
bool CMC_(PFX, _del_curr)(struct SNAME *_owner_, struct CMC_DEF_NODE(SNAME) * _node_);
bool CMC_(PFX, _del_prev)(struct SNAME *_owner_, struct CMC_DEF_NODE(SNAME) * _node_);
/* Node Access Relative to a Linked List Node */
struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _next_node)(struct CMC_DEF_NODE(SNAME) * _node_);
struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _prev_node)(struct CMC_DEF_NODE(SNAME) * _node_);

#endif /* CMC_EXT_NODE */

/**
 * STR
 *
 * Print helper functions.
 */
#ifdef CMC_EXT_STR

bool CMC_(PFX, _to_string)(struct SNAME *_list_, FILE *fptr);
bool CMC_(PFX, _print)(struct SNAME *_list_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_EXT_STR */
