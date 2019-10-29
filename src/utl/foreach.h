/**
 * foreach.h
 *
 * Creation Date: 25/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_FOR_EACH_H
#define CMC_FOR_EACH_H

#define CMC_FOR_EACH(PFX, SNAME, TARGET, BODY)                                                                      \
    do                                                                                                              \
    {                                                                                                               \
        for (struct SNAME##_iter iter = (TARGET)->it_start(TARGET); !PFX##_iter_end(&iter); PFX##_iter_next(&iter)) \
        {                                                                                                           \
            BODY;                                                                                                   \
        }                                                                                                           \
    } while (0)

#define CMC_FOR_EACH_REV(PFX, SNAME, TARGET, BODY)                                                                  \
    do                                                                                                              \
    {                                                                                                               \
        for (struct SNAME##_iter iter = (TARGET)->it_end(TARGET); !PFX##_iter_start(&iter); PFX##_iter_prev(&iter)) \
        {                                                                                                           \
            BODY;                                                                                                   \
        }                                                                                                           \
    } while (0)

#endif /* CMC_FOR_EACH_H */
