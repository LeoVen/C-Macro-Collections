/**
 * @file stack.h
 *
 * Last Update: 22/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#define FOR_EACH(PFX, SNAME, K, T, TARGET, BODY)       \
    do                                                 \
    {                                                  \
        size_t index;                                  \
        T var;                                         \
        SNAME##_iter _iter_;                           \
        PFX##_iter(&_iter_, TARGET);                   \
        while (PFX##_iter_next(&_iter_, &var, &index)) \
        {                                              \
            BODY;                                      \
        }                                              \
    } while (0);
