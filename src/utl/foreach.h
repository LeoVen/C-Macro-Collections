/**
 * foreach.h
 *
 * Creation Date: 25/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * For-Each macros that work with all Collections within the library.
 *
 * PFX      - Functions prefix
 * SNAME    - Struct name
 * ITERNAME - Iterator variable name
 * TARGET   - Target variable
 */

#ifndef CMC_FOREACH_H
#define CMC_FOREACH_H

#define CMC_FOREACH(PFX, SNAME, ITERNAME, TARGET)                 \
    for (struct SNAME##_iter ITERNAME = PFX##_iter_start(TARGET); \
         !PFX##_iter_at_end(&ITERNAME); PFX##_iter_next(&ITERNAME))

#define CMC_FOREACH_REV(PFX, SNAME, ITERNAME, TARGET)           \
    for (struct SNAME##_iter ITERNAME = PFX##_iter_end(TARGET); \
         !PFX##_iter_at_start(&ITERNAME); PFX##_iter_prev(&ITERNAME))

#endif /* CMC_FOREACH_H */
