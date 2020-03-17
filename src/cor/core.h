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
 */

#ifndef CMC_CORE_H
#define CMC_CORE_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * - on_create : an element is about to be added to the collection
 * - on_read   : the collection is about to be queried about an element
 * - on_update : an element in the collection is about to be updated
 * - on_delete : element is about to be removed from the collection
 * - on_resize : the collection is full and is about to get resized
 */
struct cmc_callbacks
{
    void (*on_create)(void);
    void (*on_read)(void);
    void (*on_update)(void);
    void (*on_delete)(void);
    void (*on_resize)(void);
};

/**
 * cmc_flags
 *
 * Defines common error codes used by all collections. These are flags that
 * indicate if something went wrong in the last operation by the collection.
 */
static struct
{
    int OK;        // No errors
    int ALLOC;     // Allocation failed
    int EMPTY;     // The collection is empty when it should not
    int NOT_FOUND; // Key or value not found
    int INVALID;   // Invalid argument or operation
    int RANGE;     // Index out of range
    int DUPLICATE; // Duplicate key or value
    int ERROR;     // Generic error, usually caused by algorithm error
} cmc_flags = { 0, 1, 2, 3, 4, 5, 6, 7 };

#endif /* CMC_CORE_H */
