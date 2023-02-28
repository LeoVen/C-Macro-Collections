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
 * list.h
 *
 * Creation Date: 12/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * List
 *
 * A List is a Dynamic Array that store its elements contiguously. It has an
 * internal buffer that is pre-allocated with a greater capacity than necessary
 * in order to foresee new elements being added. The items can be added at both
 * ends of the list and at the middle (with a given index).
 *
 * Insertions at the start are more costly as it is necessary to shift all
 * elements one position to the right to give space to a new element being added
 * at index 0. Insertions at the end are instantaneous as long as there is
 * enough space in the buffer. Insertions at the middle will be based on an
 * index. The index represents where the element being added will be located
 * once the operation ends. This operation might also need to shift elements. It
 * is also possible to add arrays of elements in the list.
 *
 * When the buffer is filled, it is reallocated with a greater capacity, usually
 * being doubled.
 *
 * Removing elements follows the same principle. Removing the front element will
 * require to shift all other elements one position to the left, thus being
 * slower than removing from the end in which is done in constant time. Removing
 * elements in the middle of the list will also require shifting elements to the
 * left. Is is also possible to remove a range of elements or extract them,
 * creating a new list with the removed items.
 *
 * The iterator is a simple structure that is capable of going back and
 * forwards. Any modifications to the target list during iteration is considered
 * undefined behavior. Its sole purpose is to facilitate navigation through a
 * list.
 */

#include "cor/core.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

#if defined(CMC_SAC) && !defined(SIZE)
#error "When using CMC_SAC, please also define SIZE"
#endif

/**
 * Used values
 * V - list data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/list/struct.h"

/* Function declaration */
#include "cmc/list/header.h"

/* Function implementation */
#include "cmc/list/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - List iterator
 * SEQ - Push and pop sequence of items
 * STR - Print helper functions
 */
#define CMC_EXT_LIST_PARTS INIT, ITER, SEQ, STR
/**/
#include "cmc/list/ext/struct.h"
/**/
#include "cmc/list/ext/header.h"
/**/
#include "cmc/list/ext/code.h"

#include "cor/undef.h"
