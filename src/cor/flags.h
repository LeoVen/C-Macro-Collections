/**
 * flags.h
 *
 * Creation Date: 14/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_FLAGS_H
#define CMC_FLAGS_H

/**
 * cmc_flags
 *
 * Defines common error codes used by the entire library.
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
    int THREAD;    // Generic error regarding threads
    int MUTEX;     // Generic error regarding mutexes
} cmc_flags = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

/**
 * cmc_flags_to_str
 *
 * Maps the error codes to their character representation.
 */
const char *cmc_flags_to_str[10] = { "OK",        "ALLOC",   "EMPTY",
                                     "NOT_FOUND", "INVALID", "RANGE",
                                     "DUPLICATE", "ERROR",   "THREAD",
                                     "MUTEX" };

#endif /* CMC_FLAGS_H */
