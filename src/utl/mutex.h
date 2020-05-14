/**
 * mutex.h
 *
 * Creation Date: 14/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * A very simple, header-only and minimalistic cross-platform mutex
 *
 * Types
 *  - cmc_mutex
 *
 * Functions
 *  - cmc_mtx_init
 *  - cmc_mtx_destroy
 *  - cmc_mtx_lock
 *  - cmc_mtx_unlock
 *  - cmc_mtx_trylock
 */

#include <stdbool.h>

#include "../cor/flags.h"

#if defined(_WIN32)
#define CMC_MUTEX_WINDOWS
#elif defined(__unix__)
#define CMC_MUTEX_UNIX
#else
#error "Unknown platform for CMC Mutex"
#endif

/* Platform specific includes */
#if defined(CMC_MUTEX_WINDOWS)
#include <windows.h>
#elif defined(CMC_MUTEX_UNIX)
#include <errno.h>
#include <pthread.h>
#endif

/**
 * struct cmc_mutex
 *
 * A mutex wrapper.
 */
struct cmc_mutex
{
#if defined(CMC_MUTEX_WINDOWS)
    HANDLE mutex;
#elif defined(CMC_MUTEX_UNIX)
    pthread_mutex_t mutex;
#endif
    int flag;
};

/**
 * Acquire resources for a mutex.
 *
 * \param mtx An uninitialized mutex wrapper.
 * \return True or false if the mutex was successfully initialized.
 */
static inline bool cmc_mtx_init(struct cmc_mutex *mtx)
{
#if defined(CMC_MUTEX_WINDOWS)
    mtx->mutex = CreateMutex(NULL, FALSE, NULL);

    if (mtx->mutex == NULL)
        mtx->flag = cmc_flags.MUTEX;
    else
        mtx->flag = cmc_flags.OK;

    return mtx->mutex != NULL;

#elif defined(CMC_MUTEX_UNIX)
    int err = pthread_mutex_init(&(mtx->mutex), NULL);

    if (err != 0)
        mtx->flag = cmc_flags.MUTEX;
    else
        mtx->flag = cmc_flags.OK;

    return err == 0;
#endif
}

/**
 * Release all resources from a mutex. Calling this function on a locked mutex
 * causes undefined behavior.
 *
 * \param mtx A mutex to be destroyed.
 * \return True or false if the mutex was successfully destroyed.
 */
static inline bool cmc_mtx_destroy(struct cmc_mutex *mtx)
{
#if defined(CMC_MUTEX_WINDOWS)
    if (!CloseHandle(mtx->mutex))
    {
        mtx->flag = cmc_flags.MUTEX;
        return false;
    }

    mtx->flag = cmc_flags.OK;
    return true;

#elif defined(CMC_MUTEX_UNIX)
    if (pthread_mutex_destroy(&(mtx->mutex)) != 0)
    {
        mtx->flag = cmc_flags.MUTEX;
        return false;
    }

    mtx->flag = cmc_flags.OK;
    return true;
#endif
}

/**
 * Locks a mutex. If the mutex is already locked, the thread is blocked until
 * the mutex can be acquired.
 *
 * \param mtx A mutex to be locked.
 * \return True or false if the lock on the mutex was successfully acquired.
 */
static inline bool cmc_mtx_lock(struct cmc_mutex *mtx)
{
#if defined(CMC_MUTEX_WINDOWS)
    DWORD result = WaitForSingleObject(mtx->mutex, INFINITE);

    if (result == WAIT_FAILED || result == WAIT_ABANDONED)
    {
        mtx->flag = cmc_flags.MUTEX;
        return false;
    }

    mtx->flag = cmc_flags.OK;
    return true;

#elif defined(CMC_MUTEX_UNIX)
    if (pthread_mutex_lock(&mtx->mutex) != 0)
    {
        mtx->flag = cmc_flags.MUTEX;
        return false;
    }

    mtx->flag = cmc_flags.OK;
    return true;
#endif
}

/**
 * Unlocks a mutex. If the mutex is already locked, the thread is blocked until
 * the mutex can be acquired.
 *
 * \param mtx A mutex to be unlocked.
 * \return True or false if the mutex was successfully unlocked.
 */
static inline bool cmc_mtx_unlock(struct cmc_mutex *mtx)
{
#if defined(CMC_MUTEX_WINDOWS)
    if (!ReleaseMutex(mtx->mutex))
        mtx->flag = cmc_flags.MUTEX;
    else
        mtx->flag = cmc_flags.OK;

    return mtx->flag == cmc_flags.OK;

#elif defined(CMC_MUTEX_UNIX)
    if (pthread_mutex_unlock(&mtx->mutex) != 0)
        mtx->flag = cmc_flags.MUTEX;
    else
        mtx->flag = cmc_flags.OK;

    return mtx->flag == cmc_flags.OK;
#endif
}

/**
 * Tries to lock a mutex. If the mutex is already locked, the thread is not
 * blocked and the function returns false.
 *
 * \param mtx A mutex to try to lock.
 * \return True or false if the lock on the mutex was successfully acquired.
 */
static inline bool cmc_mtx_trylock(struct cmc_mutex *mtx)
{
#if defined(CMC_MUTEX_WINDOWS)
    DWORD result = WaitForSingleObject(mtx->mutex, 0);

    if (result == WAIT_FAILED || result == WAIT_ABANDONED)
    {
        mtx->flag = cmc_flags.MUTEX;
        return false;
    }

    mtx->flag = cmc_flags.OK;
    return result != WAIT_TIMEOUT;

#elif defined(CMC_MUTEX_UNIX)
    int err = pthread_mutex_trylock(&mtx->mutex);

    if (err == EINVAL || err == EFAULT)
        mtx->flag = cmc_flags.MUTEX;
    else
        mtx->flag = cmc_flags.OK;

    return err == 0;
#endif
}
