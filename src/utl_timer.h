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
 * utl_timer.h
 *
 * Creation Date: 12/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 *  Simple timer macros utility for timing code execution.
 */

#ifndef CMC_UTL_TIMER_H
#define CMC_UTL_TIMER_H

#include <time.h>

struct cmc_timer
{
    clock_t start;
    clock_t stop;
    double result; /* The result is given in milliseconds */
};

#define cmc_timer_start(timer) \
    do \
    { \
        struct cmc_timer *t__ = &(timer); \
        t__->start = clock(); \
    } while (0)

#define cmc_timer_stop(timer) \
    do \
    { \
        struct cmc_timer *t__ = &(timer); \
        t__->stop = clock(); \
        t__->result = (double)(t__->stop - t__->start) * 1000.0 / CLOCKS_PER_SEC; \
    } while (0)

#endif /* CMC_UTL_TIMER_H */
