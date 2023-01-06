/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Libray.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * utl_futils.h
 *
 * Creation Date: 15/04/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Common functions that can be used by Functions Table, instead of writing
 * another one every time.
 */

#ifndef CMC_UTL_FUTILS_H
#define CMC_UTL_FUTILS_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * cmp
 */

// Signed Integers

static inline int cmc_i64_cmp(int64_t x1, int64_t x2)
{
    return (x1 > x2) - (x1 < x2);
}

static inline int cmc_i32_cmp(int32_t x1, int32_t x2)
{
    return cmc_i64_cmp((int64_t)x1, (int64_t)x2);
}

static inline int cmc_i16_cmp(int16_t x1, int16_t x2)
{
    return cmc_i64_cmp((int64_t)x1, (int64_t)x2);
}

static inline int cmc_i8_cmp(int8_t x1, int8_t x2)
{
    return cmc_i64_cmp((int64_t)x1, (int64_t)x2);
}

// Unsigned Integers

static inline int cmc_u64_cmp(uint64_t x1, uint64_t x2)
{
    return (x1 > x2) - (x1 < x2);
}

static inline int cmc_u32_cmp(uint32_t x1, uint32_t x2)
{
    return cmc_u64_cmp((uint64_t)x1, (uint64_t)x2);
}

static inline int cmc_u16_cmp(uint16_t x1, uint16_t x2)
{
    return cmc_u64_cmp((uint64_t)x1, (uint64_t)x2);
}

static inline int cmc_u8_cmp(uint8_t x1, uint8_t x2)
{
    return cmc_u64_cmp((uint64_t)x1, (uint64_t)x2);
}

// Other Integers

static inline int cmc_size_cmp(size_t x1, size_t x2)
{
    return (x1 > x2) - (x1 < x2);
}

static inline int cmc_imax_cmp(intmax_t x1, intmax_t x2)
{
    return (x1 > x2) - (x1 < x2);
}

static inline int cmc_umax_cmp(uintmax_t x1, uintmax_t x2)
{
    return (x1 > x2) - (x1 < x2);
}

// Floating Point

static inline int cmc_float_cmp(float x1, float x2)
{
    return (x1 > x2) - (x1 < x2);
}

static inline int cmc_double_cmp(double x1, double x2)
{
    return (x1 > x2) - (x1 < x2);
}

// String

static inline int cmc_str_cmp(char *ch1, char *ch2)
{
    // Assuming ch1 and ch2 are both null terminated character sequences
    return strcmp(ch1, ch2);
}

/**
 * cpy
 */

/* For basic data types a simple assignment is usually enough */

static inline char *cmc_str_cpy(char *str)
{
    size_t len = strlen(str) + 1;

    char *result = malloc(len);

    if (!result)
        return NULL;

    return memcpy(result, str, len);
}

/**
 * str
 */

// Signed Integers

static inline bool cmc_i8_str(FILE *file, int8_t element)
{
    return fprintf(file, "%" PRIi8 "", element) > 0;
}

static inline bool cmc_i16_str(FILE *file, int16_t element)
{
    return fprintf(file, "%" PRIi16 "", element) > 0;
}

static inline bool cmc_i32_str(FILE *file, int32_t element)
{
    return fprintf(file, "%" PRIi32 "", element) > 0;
}

static inline bool cmc_i64_str(FILE *file, int64_t element)
{
    return fprintf(file, "%" PRIi64 "", element) > 0;
}

// Unsigned Integers

static inline bool cmc_u8_str(FILE *file, uint8_t element)
{
    return fprintf(file, "%" PRIu8 "", element) > 0;
}

static inline bool cmc_u16_str(FILE *file, uint16_t element)
{
    return fprintf(file, "%" PRIu16 "", element) > 0;
}

static inline bool cmc_u32_str(FILE *file, uint32_t element)
{
    return fprintf(file, "%" PRIu32 "", element) > 0;
}

static inline bool cmc_u64_str(FILE *file, uint64_t element)
{
    return fprintf(file, "%" PRIu64 "", element) > 0;
}

// Other Integers

static inline bool cmc_size_str(FILE *file, size_t element)
{
    return fprintf(file, "%" PRIuMAX "", (uintmax_t)element) > 0;
}

static inline bool cmc_imax_str(FILE *file, intmax_t element)
{
    return fprintf(file, "%" PRIiMAX "", element) > 0;
}

static inline bool cmc_umax_str(FILE *file, uintmax_t element)
{
    return fprintf(file, "%" PRIu64 "", element) > 0;
}

// Floating Point

static inline bool cmc_float_str(FILE *file, float element)
{
    return fprintf(file, "%f", element) > 0;
}

static inline bool cmc_double_str(FILE *file, double element)
{
    return fprintf(file, "%lf", element) > 0;
}

// String

static inline bool cmc_str_str(FILE *file, char *element)
{
    return fprintf(file, "\"%s\"", element) > 0;
}

/**
 * free
 */

/* For most data types a simple free() is enough */

/**
 * hash
 */

// These integer hashing functions are based on splitmix64
// http://web.archive.org/web/20190929030557/http://xorshift.di.unimi.it/splitmix64.c

// Integers

static inline size_t cmc_i64_hash(int64_t e)
{
    size_t x = (size_t)e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

static inline size_t cmc_i32_hash(int32_t e)
{
    return cmc_i64_hash((int64_t)e);
}

static inline size_t cmc_i16_hash(int16_t e)
{
    return cmc_i64_hash((int64_t)e);
}

static inline size_t cmc_i8_hash(int8_t e)
{
    return cmc_i64_hash((int64_t)e);
}

// Unsigned Integers

static inline size_t cmc_u64_hash(uint64_t e)
{
    size_t x = (size_t)e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

static inline size_t cmc_u32_hash(uint32_t e)
{
    return cmc_u64_hash((uint64_t)e);
}

static inline size_t cmc_u16_hash(uint16_t e)
{
    return cmc_u64_hash((uint64_t)e);
}

static inline size_t cmc_u8_hash(uint8_t e)
{
    return cmc_u64_hash((uint64_t)e);
}

// Other Integers

static inline size_t cmc_size_hash(size_t e)
{
    size_t x = (size_t)e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

static inline size_t cmc_imax_hash(intmax_t e)
{
    size_t x = (size_t)e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

static inline size_t cmc_umax_hash(uintmax_t e)
{
    size_t x = (size_t)e;

    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);

    return x;
}

// Floating Point

static inline size_t cmc_float_hash(float e)
{
    // Make both representations of 0.0 hash to the same value
    // 0.0 and -0.0
    // Since -0.0 has a different binary representation than 0.0
    // And 0.0 == -0.0 is always true
    if (e == 0.0)
        e = 0.0; // if e == -0.0, now it is 0.0

    union
    {
        float a;
        size_t b;
    } x;

    x.a = e;

    return x.b & 0xFFFFFFF8;
}

static inline size_t cmc_double_hash(double e)
{
    // Make both representations of 0.0 hash to the same value
    // 0.0 and -0.0
    // Since -0.0 has a different binary representation than 0.0
    // And 0.0 == -0.0 is always true
    if (e == 0.0)
        e = 0.0; // if e == -0.0, now it is 0.0

    union
    {
        double a;
        size_t b;
    } x;

    x.a = e;

    return x.b & 0xFFFFFFFFFFFFFFF0;
}

// String

// Both djb2 and java are based on a Linear Congruential Generator
// https://en.wikipedia.org/wiki/Linear_congruential_generator

// http://www.cse.yorku.ca/~oz/hash.html
static inline size_t cmc_str_hash_djb2(char *str)
{
    size_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

// http://www.cse.yorku.ca/~oz/hash.html
static inline size_t cmc_str_hash_sdbm(char *str)
{
    size_t hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

// Based on Java's String.hashCode()
static inline size_t cmc_str_hash_java(char *str)
{
    size_t hash = UINT64_C(1125899906842597); // prime
    int c;

    while ((c = *str++))
        hash = 31 * hash + c;

    return hash;
}

// https://en.wikipedia.org/wiki/MurmurHash
// https://github.com/aappleby/smhasher/blob/61a0530f28277f2e850bfc39600ce61d02b518de/src/MurmurHash3.cpp#L81
static inline size_t cmc_str_hash_murmur3(uint64_t e)
{
    size_t x = (size_t)e;

    x ^= (x >> 33);
    x *= 0xff51afd7ed558ccd;
    x ^= (x >> 33);
    x *= 0xc4ceb9fe1a85ec53;
    x ^= (x >> 33);

    return x;
}

// A variant or murmurhash3 from
// http://web.archive.org/web/20200310023308/http://zimbry.blogspot.com/2011/09/better-bit-mixing-improving-on.html
static inline size_t cmc_str_hash_murmur3_variant(uint64_t e)
{
    size_t x = (size_t)e;

    x ^= (x >> 31);
    x *= 0x7fb5d329728ea185;
    x ^= (x >> 27);
    x *= 0x81dadef4bc2dd44d;
    x ^= (x >> 33);

    return x;
}

// Others

// These two originally come from
// http://web.archive.org/web/20071223173210/http://www.concentric.net/~Ttwang/tech/inthash.htm
static inline size_t cmc_i64_hash_mix(int64_t element)
{
    size_t e = (size_t)element;

    e = (~e) + (e << 21);
    e = e ^ (e >> 24);
    e = (e + (e << 3)) + (e << 8);
    e = e ^ (e >> 14);
    e = (e + (e << 2)) + (e << 4);
    e = e ^ (e >> 28);
    e = e + (e << 31);
    return e;
}

static inline size_t cmc_u64_hash_mix(uint64_t element)
{
    size_t e = (size_t)element;

    e = (~e) + (e << 21);
    e = e ^ (e >> 24);
    e = (e + (e << 3)) + (e << 8);
    e = e ^ (e >> 14);
    e = (e + (e << 2)) + (e << 4);
    e = e ^ (e >> 28);
    e = e + (e << 31);
    return e;
}

/**
 * pri
 */

/* Can simply use cmp for basic data types */

#endif /* CMC_UTL_FUTILS_H */
