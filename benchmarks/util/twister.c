/**
 * twister.c
 *
 * Creation Date: 09/05/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * Mersenne Twister PRNG utility
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NN 312
#define MM 156
#define MATRIX_A UINT64_C(0xB5026F5AA96619E9)
#define UM UINT64_C(0xFFFFFFFF80000000) // Most significant 33 bits
#define LM UINT64_C(0x7FFFFFFF)         // Least significant 31 bits

// State vector
typedef struct mt_state_s
{
    uint64_t mt[NN];
    int mti;
} mt_state, *mt_state_ptr;

// Initializes mt[NN] with a seed
void twist_init(mt_state_ptr st, uint64_t seed);

// Generates a random uint64_t in [0, 2^64-1] interval
uint64_t twist_uint64(mt_state_ptr st);

// Generates a double in [0, 1) interval
double twist_real(mt_state_ptr st);

//
// Implementation
//

// Initialize a state mt[NN] with a seed
void twist_init(mt_state_ptr st, uint64_t seed)
{
    st->mti = NN + 1;

    st->mt[0] = seed;

    for (st->mti = 1; st->mti < NN; st->mti++)
    {
        st->mt[st->mti] = (UINT64_C(6364136223846793005) * (st->mt[st->mti - 1] ^ (st->mt[st->mti - 1] >> 62)) + st->mti);
    }
}

// Generates a random uint64_t in [0, 2^64-1] interval
uint64_t twist_uint64(mt_state_ptr st)
{
    int i;
    uint64_t x;
    static uint64_t mag01[2] = {UINT64_C(0), MATRIX_A};

    if (st->mti >= NN)
    {
        // Generate NN words at one time if twist_init() has not been called
        // A default initial seed is used
        if (st->mti == NN + 1)
            twist_init(st, UINT64_C(5489));

        for (i = 0; i < NN - MM; i++)
        {
            x = (st->mt[i] & UM) | (st->mt[i + 1] & LM);
            st->mt[i] = st->mt[i + MM] ^ (x >> 1) ^ mag01[(int)(x & UINT64_C(1))];
        }

        for (; i < NN - 1; i++)
        {
            x = (st->mt[i] & UM) | (st->mt[i + 1] & LM);
            st->mt[i] = st->mt[i + (MM - NN)] ^ (x >> 1) ^ mag01[(int)(x & UINT64_C(1))];
        }

        x = (st->mt[NN - 1] & UM) | (st->mt[0] & LM);

        st->mt[NN - 1] = st->mt[MM - 1] ^ (x >> 1) ^ mag01[(int)(x & UINT64_C(1))];

        st->mti = 0;
    }

    x = st->mt[st->mti++];

    x ^= (x >> 29) & UINT64_C(0x5555555555555555);
    x ^= (x << 17) & UINT64_C(0x71D67FFFEDA60000);
    x ^= (x << 37) & UINT64_C(0xFFF7EEE000000000);
    x ^= (x >> 43);

    return x;
}

// Generates a double in [0, 1) interval
double twist_real(mt_state_ptr st)
{
    return (twist_uint64(st) >> 11) * (1.0 / 9007199254740992.0);
}
