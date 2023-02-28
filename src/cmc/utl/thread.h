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
 * utl_thread.h
 *
 * Creation Date: 14/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * A very simple, header-only utility for spawning and joining threads
 *
 * Types
 *  - cmc_thread
 *  - cmc_thread_proc
 *
 * Functions
 *  - cmc_thrd_create
 *  - cmc_thrd_join
 */

#ifndef CMC_UTL_THREAD_H
#define CMC_UTL_THREAD_H

#include <inttypes.h>
#include <stdbool.h>

#include "../cor/flags.h"

#if defined(_WIN32)
#define CMC_THREAD_WINDOWS
#elif defined(__unix__)
#define CMC_THREAD_UNIX
#else
#error "Unknown platform for CMC Threads"
#endif

#if defined(CMC_THREAD_WINDOWS)
#include <windows.h>
#elif defined(CMC_THREAD_UNIX)
#include <pthread.h>
#endif

/* The type of a process run by a thread */
typedef int (*cmc_thread_proc)(void *);

/**
 * struct cmc_thread
 *
 * A thread wrapper.
 */
struct cmc_thread
{
#if defined(CMC_THREAD_WINDOWS)
    HANDLE thread;
#elif defined(CMC_THREAD_UNIX)
    pthread_t thread;
#endif
    int flag;
    void *args;
    cmc_thread_proc process;
};

/* A thread wrapper */
#if defined(CMC_THREAD_WINDOWS)
static DWORD cmc_thread_wrapper(void *arg)
#elif defined(CMC_THREAD_UNIX)
static void *cmc_thread_wrapper(void *arg)
#endif
{
    struct cmc_thread *thr = (struct cmc_thread *)arg;

    int result = thr->process(thr->args);

#if defined(CMC_THREAD_WINDOWS)
    return result;
#elif defined(CMC_THREAD_UNIX)
    return (void *)(intptr_t)result;
#endif
}

/**
 * Creates and spawns a new thread.
 *
 * \param thr  An uninitialized thread wrapper.
 * \param proc Pointer to a function that will run in a new thread.
 * \param args The arguments passed to the proc function.
 * \return True or false if the thread creation was successfull.
 */
static inline bool cmc_thrd_create(struct cmc_thread *thr, cmc_thread_proc proc, void *args)
{
#if defined(CMC_THREAD_WINDOWS)
    thr->args = args;
    thr->process = proc;
    thr->thread = CreateThread(NULL, 0, cmc_thread_wrapper, thr, 0, NULL);

    if (!thr->thread)
        thr->flag = CMC_FLAG_THREAD;
    else
        thr->flag = CMC_FLAG_OK;

    return thr->flag == CMC_FLAG_OK;

#elif defined(CMC_THREAD_UNIX)
    thr->args = args;
    thr->process = proc;
    if (pthread_create(&(thr->thread), NULL, cmc_thread_wrapper, thr) != 0)
        thr->flag = CMC_FLAG_THREAD;
    else
        thr->flag = CMC_FLAG_OK;

    return thr->flag == CMC_FLAG_OK;
#endif
}

/**
 * Blocks the current thread until the target thread terminates.
 *
 * \param thr    The target thread to wait on.
 * \param result The value returned by the target thread. Optional.
 * \return True or false if the join was successfull.
 */
static inline bool cmc_thrd_join(struct cmc_thread *thr, int *result)
{
#if defined(CMC_THREAD_WINDOWS)
    if (WaitForSingleObject(thr->thread, INFINITE) == WAIT_FAILED)
    {
        thr->flag = CMC_FLAG_THREAD;
        return false;
    }

    DWORD exit_code;

    if (result)
    {
        if (GetExitCodeThread(thr->thread, &exit_code) == 0)
        {
            thr->flag = CMC_FLAG_THREAD;
            return false;
        }
        else
            *result = (int)exit_code;
    }

    thr->flag = CMC_FLAG_OK;

    if (CloseHandle(thr->thread) == 0)
    {
        thr->flag = CMC_FLAG_THREAD;
        return false;
    }

    thr->flag = CMC_FLAG_OK;
    return true;

#elif defined(CMC_THREAD_UNIX)
    void *exit_value;

    if (pthread_join(thr->thread, &exit_value) != 0)
    {
        thr->flag = CMC_FLAG_THREAD;
        return false;
    }

    if (result)
    {
        *result = (int)(intptr_t)exit_value;
    }

    thr->flag = CMC_FLAG_OK;
    return true;

#endif
}

#endif /* CMC_UTL_THREAD_H */
