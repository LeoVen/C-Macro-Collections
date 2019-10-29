/**
 * cmc_string.h
 *
 * Creation Date: 17/07/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/* A very simple fixed size string used by all collections' method to_string */

#ifndef CMC_STRING_H
#define CMC_STRING_H

#include <inttypes.h>
#include <stddef.h>

static const size_t cmc_string_len = 200;

struct cmc_string
{
    char s[200];
};

#endif /* CMC_STRING_H */
