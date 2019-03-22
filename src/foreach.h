/**
 * @file stack.h
 *
 * Last Update: 22/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

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
