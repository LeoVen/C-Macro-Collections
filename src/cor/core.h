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

#ifndef CMC_CORE_H
#define CMC_CORE_H

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/**
 * struct cmc_alloc_node
 *
 * Custom allocation node. Allows collections to use custom allocation
 * functions.
 */
static struct cmc_alloc_node
{
    void *(*malloc)(size_t);
    void *(*calloc)(size_t, size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
} cmc_alloc_node_default = { malloc, calloc, realloc, free };

/**
 * struct cmc_callbacks
 *
 * Callback node. Each collection will call one of these functions when:
 * - create : an element was successfully added to the collection
 * - read   : the collection was successfully queried about an element
 * - update : an element in the collection was successfully updated
 * - delete : an element was successfully removed from the collection
 * - resize : the collection was full and successfully resized
 */
struct cmc_callbacks
{
    void (*create)(void);
    void (*read)(void);
    void (*update)(void);
    void (*delete)(void);
    void (*resize)(void);
};

#endif /* CMC_CORE_H */
