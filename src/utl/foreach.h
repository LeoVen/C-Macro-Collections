/**
 * for_each.h
 *
 * Creation Date: 25/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * ForEach utility
 *
 * Read Documentation.md for more information
 */

#ifndef CMC_FOR_EACH
#define CMC_FOR_EACH

#define FOR_EACH(PFX, SNAME, TARGET, BODY)                                                                 \
    do                                                                                                     \
    {                                                                                                      \
        for (SNAME##_iter iter = TARGET->it_start(TARGET); !PFX##_iter_end(&iter); PFX##_iter_next(&iter)) \
        {                                                                                                  \
            BODY;                                                                                          \
        }                                                                                                  \
    } while (0)

#define FOR_EACH_REV(PFX, SNAME, TARGET, BODY)                                                             \
    do                                                                                                     \
    {                                                                                                      \
        for (SNAME##_iter iter = TARGET->it_end(TARGET); !PFX##_iter_start(&iter); PFX##_iter_prev(&iter)) \
        {                                                                                                  \
            BODY;                                                                                          \
        }                                                                                                  \
    } while (0)

#endif /* CMC_FOR_EACH */
