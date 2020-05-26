/**
 * flags.h
 *
 * Creation Date: 14/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_COR_FLAGS_H
#define CMC_COR_FLAGS_H

/**
 * enum cmc_flags
 *
 * Defines common error codes used by the entire library.
 */
// clang-format off
enum cmc_flags
{
    CMC_FLAG_OK        =  0, // No errors
    CMC_FLAG_ALLOC     =  2, // Allocation failed
    CMC_FLAG_EMPTY     =  3, // The collection is empty when it should not
    CMC_FLAG_NOT_FOUND =  4, // Key or value not found
    CMC_FLAG_INVALID   =  5, // Invalid argument or operation
    CMC_FLAG_RANGE     =  6, // Index out of range
    CMC_FLAG_DUPLICATE =  7, // Duplicate key or value
    CMC_FLAG_ERROR     =  8, // Generic error, usually caused by algorithm error
    CMC_FLAG_THREAD    =  9, // Generic error regarding threads
    CMC_FLAG_MUTEX     = 10  // Generic error regarding mutexes
};
// clang-format on

/**
 * cmc_flags_to_str
 *
 * Maps the error codes to their character representation.
 */
const char *cmc_flags_to_str[10] = { "OK",        "ALLOC",   "EMPTY",
                                     "NOT_FOUND", "INVALID", "RANGE",
                                     "DUPLICATE", "ERROR",   "THREAD",
                                     "MUTEX" };

#endif /* CMC_COR_FLAGS_H */
