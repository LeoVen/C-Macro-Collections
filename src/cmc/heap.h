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
 * heap.h
 *
 * Creation Date: 25/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Heap
 *
 * The Heap is a specialized tree-based data structure which is essentially a
 * complete tree that satisfies the heap property:
 *     - MaxHeap:
 *         - The root node is the greatest element of the unique set of keys.
 *         - A node key is always greater than or equal to its children keys.
 *     - Minheap:
 *         - The root node is the smallest element of the unique set of keys.
 *         - A node Key is always greater than or equal to its children keys.
 *
 * The heap is mostly used to implement priority queues. Sometimes it can also
 * be used to sort elements. There are three main functions:
 *     - insert : Adds an element to the heap
 *     - remove_(min/max): Removes the min/max element from the heap
 *     - min/max : Accesses the min/max element from the heap
 */

#include "cor/core.h"
#include "cor/heap.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

/**
 * Used values
 * V - heap value data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/heap/struct.h"

/* Function declaration */
#include "cmc/heap/header.h"

/* Function implementation */
#include "cmc/heap/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - heap iterator
 * STR - Print helper functions
 */
#define CMC_EXT_HEAP_PARTS ITER, SETF, STR
/**/
#include "cmc/heap/ext/struct.h"
/**/
#include "cmc/heap/ext/header.h"
/**/
#include "cmc/heap/ext/code.h"

#include "cor/undef.h"
