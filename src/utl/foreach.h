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

#define CMC_FOREACH(PFX, SNAME, ITERNAME, TARGET)                   \
    for (struct SNAME##_iter ITERNAME = (TARGET)->it_start(TARGET); \
         !PFX##_iter_end(&ITERNAME); PFX##_iter_next(&ITERNAME))

#define CMC_FOREACH_REV(PFX, SNAME, ITERNAME, TARGET)             \
    for (struct SNAME##_iter ITERNAME = (TARGET)->it_end(TARGET); \
         !PFX##_iter_start(&ITERNAME); PFX##_iter_prev(&ITERNAME))

#endif /* CMC_FOR_EACH_H */
