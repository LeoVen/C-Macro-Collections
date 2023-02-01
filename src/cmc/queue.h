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
 * queue.h
 *
 * Creation Date: 15/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Queue
 *
 * A Queue is a First-In First-out (or Last-in Last-out) data structure. It is
 * a Dynamic Circular Array where elements are added from one end of the array
 * and removed from the other end. The circular array here (also known as
 * circular buffer or ring buffer) is very important so that both adding and
 * removing elements from the Queue are done instantly. The array is linear but
 * with the modulo operator it is treated as a circular sequence of elements.
 *
 * If the Queue was implemented as a regular Dynamic Array, when adding or
 * removing an element at the front, it would be necessary to shift all elements
 * currently present in the Queue and this would add up a lot of computing time.
 * Shifting `100000` elements in memory by one position every time an element is
 * added to the Queue is simply not efficient.
 *
 * The Queue has two ends. The `front` and `back`. In this implementation all
 * elements are added to the back of the Queue and removed from the front, which
 * is more or less how queues work in real life. Unlike a Stack that only has
 * operations at one end of the buffer, the Queue needs to be implemented as a
 * circular array in order to quickly add or remove elements.
 *
 * The Queue has three main functions: `enqueue` which adds an element to the
 * Queue; `dequeue` which removes an element from the Queue; and `peek` which
 * return the element at the front of the Queue, that is, the next element to
 * be removed from it.
 *
 * The Queue is used in many applications where a resource is shared among
 * multiple consumers and the Queue is responsible for scheduling the access to
 * the resource.
 */

#include "cor/core.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

/**
 * Used values
 * V - queue data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/queue/struct.h"

/* Function declaration */
#include "cmc/queue/header.h"

/* Function implementation */
#include "cmc/queue/code.h"

/**
 * Extensions
 *
 * ITER - List iterator
 * STR - Print helper functions
 */
#define CMC_EXT_QUEUE_PARTS INIT, ITER, STR
/**/
#include "cmc/queue/ext/struct.h"
/**/
#include "cmc/queue/ext/header.h"
/**/
#include "cmc/queue/ext/code.h"

#include "cor/undef.h"
