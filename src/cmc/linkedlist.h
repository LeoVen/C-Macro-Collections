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
 * linkedlist.h
 *
 * Creation Date: 22/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * LinkedList
 *
 * A LinkedList is a linear collection of elements, whose order is not given by
 * their physical placement in memory. Instead, each element points to the next
 * (singly-linked list) and to a previous one (doubly-linked list). It consists
 * of nodes connected by these next or previous pointers. Unlike an array, the
 * LinkedList can indefinitely grow.
 *
 * Implementation
 *
 * The LinkedList is implemented as a doubly-linked list and allows insertions
 * and removals at both ends in O(1) and in a given index in O(N). The list has
 * a head and tail pointer. The head points to the first element in the sequence
 * and tail points to the last.
 */

#include "cor/core.h"

/**
 * Used values
 * V - linkedlist data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/linkedlist/struct.h"

/* Function declaration */
#include "cmc/linkedlist/header.h"

/* Function implementation */
#include "cmc/linkedlist/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - List iterator
 * NODE - Operations relative to a linkedlist node
 * STR - Print helper functions
 */
#define CMC_EXT_LINKEDLIST_PARTS INIT, ITER, NODE, STR
/**/
#include "cmc/linkedlist/ext/struct.h"
/**/
#include "cmc/linkedlist/ext/header.h"
/**/
#include "cmc/linkedlist/ext/code.h"

#include "cor/undef.h"
