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
 * deque.h
 *
 * Creation Date: 20/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Deque
 *
 * A Deque (double-ended queue) is a linear data structure that is able to add
 * or remove elements from both ends. It can also be thought of a double-ended
 * stack since you can push and pop elements from two ends. The Deque can also
 * be used as a Queue.
 *
 * There is no random access for a Deque. The only elements accessible are the
 * front element and the back.
 *
 * Implementation
 *
 * This implementation uses a circular buffer (ring buffer or cyclic buffer) in
 * order to operate on O(1) for push and pop on either ends (only case where it
 * takes longer than O(1) is when the buffer is reallocated). If it was
 * implemented as a regular array, adding or removing elements from the front
 * would take O(N) due to the need to shift all elements in the deque.
 */

#include "cor/core.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

/**
 * Used values
 * V - deque data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/deque/struct.h"

/* Function declaration */
#include "cmc/deque/header.h"

/* Function implementation */
#include "cmc/deque/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - Deque iterator
 * STR - Print helper functions
 */
#define CMC_EXT_DEQUE_PARTS INIT, ITER, STR
/**/
#include "cmc/deque/ext/struct.h"
/**/
#include "cmc/deque/ext/header.h"
/**/
#include "cmc/deque/ext/code.h"

#include "cor/undef.h"
