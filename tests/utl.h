/* Utility file to be included in the unit tests */
#ifndef CMC_TESTS_UTL
#define CMC_TESTS_UTL

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#include "cmc/cor/hashtable.h"
#include "cmc/utl/assert.h"
#include "cmc/utl/foreach.h"
#include "cmc/utl/futils.h"
#include "cmc/utl/test.h"

size_t numhash(size_t a)
{
    return a;
}

size_t hashcapminus1(size_t a)
{
    (void)a;
    return cmc_hashtable_primes[0] - 1;
}

size_t hashcapminus4(size_t a)
{
    (void)a;
    return cmc_hashtable_primes[0] - 1;
}

size_t hash0(size_t a)
{
    (void)a;
    return 0;
}

// counter ftab functions

int32_t k_total_cmp = 0;
int32_t k_total_cpy = 0;
int32_t k_total_str = 0;
int32_t k_total_free = 0;
int32_t k_total_hash = 0;
int32_t k_total_pri = 0;

int32_t v_total_cmp = 0;
int32_t v_total_cpy = 0;
int32_t v_total_str = 0;
int32_t v_total_free = 0;
int32_t v_total_hash = 0;
int32_t v_total_pri = 0;

int k_c_cmp(size_t a, size_t b)
{
    k_total_cmp++;
    return (a > b) - (a < b);
}

size_t k_c_cpy(size_t a)
{
    k_total_cpy++;
    return a;
}

bool k_c_str(FILE *fptr, size_t a)
{
    k_total_str++;
    return fprintf(fptr, "%" PRIuMAX " ", a) > 0;
}

void k_c_free(size_t a)
{
    (void)a;
    k_total_free++;
}

size_t k_c_hash(size_t x)
{
    k_total_hash++;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

int k_c_pri(size_t a, size_t b)
{
    k_total_pri++;
    return (a > b) - (a < b);
}

int v_c_cmp(size_t a, size_t b)
{
    v_total_cmp++;
    return (a > b) - (a < b);
}

size_t v_c_cpy(size_t a)
{
    v_total_cpy++;
    return a;
}

bool v_c_str(FILE *fptr, size_t a)
{
    v_total_str++;
    return fprintf(fptr, "%" PRIuMAX " ", a) > 0;
}

void v_c_free(size_t a)
{
    (void)a;
    v_total_free++;
}

size_t v_c_hash(size_t x)
{
    v_total_hash++;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

int v_c_pri(size_t a, size_t b)
{
    v_total_pri++;
    return (a > b) - (a < b);
}

void callbacks(CMC_UNUSED const char func_name[], CMC_UNUSED void *self)
{
    // EMPTY for now..
}

#endif /* CMC_TESTS_UTL */
