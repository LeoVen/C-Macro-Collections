/**
 * @file for_each.h
 *
 * Last Update: 22/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_FOR_EACH
#define CMC_FOR_EACH

#include <stdlib.h>
#include <stdbool.h>

#define FOR_EACH(PFX, SNAME, K, T, TARGET, BODY)     \
    do                                               \
    {                                                \
        size_t index;                                \
        T var;                                       \
        SNAME##_iter iter;                           \
        PFX##_iter(&iter, TARGET);                   \
        while (PFX##_iter_next(&iter, &var, &index)) \
        {                                            \
            BODY;                                    \
        }                                            \
    } while (0);

#define FOR_EACH_REV(PFX, SNAME, K, T, TARGET, BODY) \
    do                                               \
    {                                                \
        size_t index;                                \
        T var;                                       \
        SNAME##_iter iter;                           \
        PFX##_iter(&iter, TARGET);                   \
        PFX##_iter_toend(&iter);                     \
        while (PFX##_iter_prev(&iter, &var, &index)) \
        {                                            \
            BODY;                                    \
        }                                            \
    } while (0);

#endif /* CMC_FOR_EACH */