/**
 * core.h
 *
 * Creation Date: 17/03/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Core includes and structs used in the library.
 *
 * In here you will find:
 *     struct cmc_string
 *     struct cmc_alloc_node
 *     struct cmc_callbacks
 *     cmc_flags
 */

#ifndef CMC_COR_CORE_H
#define CMC_COR_CORE_H

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "callbacks.h"
#include "flags.h"

#define CMC_TO_STRING_(X) #X
#define CMC_TO_STRING(X) CMC_TO_STRING_(X)

/**
 * struct cmc_string
 *
 * Used by all collections when calling the to_string function.
 */
struct cmc_string
{
    char s[400];
};

static const size_t cmc_string_len = 400;

#endif /* CMC_COR_CORE_H */
