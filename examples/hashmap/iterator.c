/**
 * iterator.c
 *
 * Creation Date: 14/05/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * Iterator Example
 */
#include "cmc/hashmap.h"
#include <stdio.h>
#include <inttypes.h>

// Int hash function
size_t inthash(int t)
{
    size_t a = t;
    a += ~(a << 15);
    a ^= (a >> 10);
    a += (a << 3);
    a ^= (a >> 6);
    a += ~(a << 11);
    a ^= (a >> 16);
    return a;
}

// Int compare function
int intcmp(int a, int b)
{
    return a - b;
}

// Creates a hashmap of integers
HASHMAP_GENERATE(hm, hashmap, /* FMOD */, int, double)

int main(int argc, char const *argv[])
{
    // Create a new hashmap with an initial capacity of 100 and load factor of 0.6
    hashmap *my_map = hm_new(100, 0.6, intcmp, inthash);

    // Add pairs to the hashmap
    for (int i = 0; i < 20; i++)
    {
        // Map integers to doubles
        hm_insert(my_map, i, (double)i / 10.0);
    }

    // Iterate from the start (index 0) to the end (index count - 1) of the hashmap
    for (hashmap_iter it = my_map->it_start(my_map); // Initialize iterator 'it' to the start of the hashmap
         !hm_iter_end(&it);                          // Conditional stop (until it is not at the end)
         hm_iter_next(&it))                          // Iterator goes one position forward
    {
        // Access the current key
        int k = hm_iter_key(&it);

        // Access the current value
        double v = hm_iter_value(&it);

        // Access the current index (index is relative to all the pairs in the iteration)
        size_t i = hm_iter_index(&it);

        // Print hashmap

        // Index is useful in situations like this
        // If it is the first pair
        if (i == 0)
            printf("[ {%d:%.2lf}, ", k, v);
        // If it is the last pair
        else if (i == hm_count(my_map) - 1)
            printf("{%d:%.2lf} ]\n", k, v);
        // For pairs at the middle
        else
            printf("{%d:%.2lf}, ", k, v);
    }

    // Index can also be used to iterate over a certain portion of a collection
    size_t limit = hm_count(my_map) / 2;
    for (hashmap_iter it = my_map->it_start(my_map); !hm_iter_end(&it); hm_iter_next(&it))
    {
        // Access the current key
        int k = hm_iter_key(&it);

        // Access the current value
        double v = hm_iter_value(&it);

        // Access the current index (index is relative to all the pairs in the iteration)
        size_t i = hm_iter_index(&it);

        // Print hashmap up until limit

        // If it is the first pair
        if (i == 0)
            printf("[ {%d:%.2lf}, ", k, v);
        // If it is the last pair
        else if (i == limit - 1) // -1 because index is 0 based and we want half of the hashmap
        {
            printf("{%d:%.2lf} ]\n", k, v);
            break;
        }
        // For pairs at the middle
        else
            printf("{%d:%.2lf}, ", k, v);
    }

    // Going backwards
    for (hashmap_iter it = my_map->it_end(my_map); // Initialize iterator 'it' to the end of the hashmap
         !hm_iter_start(&it);                      // Conditional stop (until it is not at the start)
         hm_iter_prev(&it))                        // Iterator goes one position backwards
    {
        // Access the current key
        int k = hm_iter_key(&it);

        // Access the current value
        double v = hm_iter_value(&it);

        // Access the current index (index is relative to all the pairs in the iteration)
        size_t i = hm_iter_index(&it);

        // Print hashmap
        // Note how the conditions for the first and last pairs are simply swapped
        // Because iterating backwards make the first pair be the index count - 1
        // and the last one be the index 0

        // If it is the first pair
        if (i == hm_count(my_map) - 1)
            printf("[ {%d:%.2lf}, ", k, v);
        // If it is the last pair
        else if (i == 0)
            printf("{%d:%.2lf} ]\n", k, v);
        // For pairs at the middle
        else
            printf("{%d:%.2lf}, ", k, v);
    }

    // You can also use iterators in a while loop
    // But to do so you must declare the iterator struct outside the loop first
    hashmap_iter it;

    // You have three options:
    // - Initialize it using the functions provided by the hashmap [ my_map->it_start() and my_map->it_end() ]
    // - Initialize it using the init() function (when the iterator is already allocated)
    // - Initialize it using the new() function (allocates on the heap, pointers only)

    it = my_map->it_start(my_map);
    it = my_map->it_end(my_map);

    // This is equivalent to it_start()
    hm_iter_init(&it, my_map);

    // For pointers
    // Also equivalent to it_start()
    hashmap_iter *iter = hm_iter_new(my_map);

    // The actual while loop
    while (!hm_iter_end(iter))
    {
        // Accessing pairs
        int k = hm_iter_key(iter);
        double v = hm_iter_value(iter);
        size_t i = hm_iter_index(iter);

        // Print index and key value pair
        printf("hashmap[%2" PRIuMAX "] = { %2d : %.2lf }\n", i, k, v);

        // Move iterator forwards
        hm_iter_next(iter);
    }

    // Send the iterator to the start of the hashmap
    hm_iter_to_start(iter);

    // Send the iterator to the end of the hashmap
    hm_iter_to_end(iter);

    // Backwards loop
    while (!hm_iter_start(iter))
    {
        // Accessing pairs
        int k = hm_iter_key(iter);
        double v = hm_iter_value(iter);
        size_t i = hm_iter_index(iter);

        // Print index and the key value pair
        printf("hashmap[%2" PRIuMAX "] = { %2d : %.2lf }\n", i, k, v);

        // Move iterator backwards
        hm_iter_prev(iter);
    }

    // Certain iterators also allow you to modify the contents it is iterating over
    // Exceptions are collections that depend of the current state of the pair
    // like tree-based collections and heap (collections that have some kind of ordering).
    // You also can't change the keys of an associative collection

    // Change every value where its key is divisible by 3 to the number 7
    for (hm_iter_to_start(&it); !hm_iter_end(&it); hm_iter_next(&it))
    {
        // Get current key
        int k = hm_iter_key(&it);

        // Get a reference to the current value
        double *v = hm_iter_rvalue(&it);

        if (k % 3 == 0)
            *v = 7;
    }

    // Now print it to see the difference
    for (hashmap_iter it = my_map->it_start(my_map); !hm_iter_end(&it); hm_iter_next(&it))
    {
        int k = hm_iter_key(&it);
        double v = hm_iter_value(&it);
        size_t i = hm_iter_index(&it);

        if (i == 0)
            printf("[ {%d:%.2lf}, ", k, v);
        else if (i == hm_count(my_map) - 1)
            printf("{%d:%.2lf} ]\n", k, v);
        else
            printf("{%d:%.2lf}, ", k, v);
    }

    // Dispose the iterator called by hm_iter_new()
    hm_iter_free(iter);
    // Dispose the hashmap created
    hm_free(my_map);

    return 0;
}
