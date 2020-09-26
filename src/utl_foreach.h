/**
 * utl_foreach.h
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

#ifndef CMC_UTL_FOREACH_H
#define CMC_UTL_FOREACH_H

#include "cor_core.h"

#define CMC_FOREACH(PFX, SNAME, ITERNAME, TARGET) \
    for (struct CMC_DEF_ITER(SNAME) ITERNAME = CMC_(PFX, _iter_start)(TARGET); !CMC_(PFX, _iter_at_end)(&ITERNAME); \
         CMC_(PFX, _iter_next)(&ITERNAME))

#define CMC_FOREACH_REV(PFX, SNAME, ITERNAME, TARGET) \
    for (struct CMC_DEF_ITER(SNAME) ITERNAME = CMC_(PFX, _iter_end)(TARGET); !CMC_(PFX, _iter_at_start)(&ITERNAME); \
         CMC_(PFX, _iter_prev)(&ITERNAME))

#endif /* CMC_UTL_FOREACH_H */
