/**
 * iterator.c
 *
 * Creation Date: 16/05/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * Iterator Example
 */
#include "cmc/treeset.h"
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

// Creates a treeset of integers
TREESET_GENERATE(ts, treeset, /* FMOD */, int)

int main(int argc, char const *argv[])
{
    // Create a new treeset with an initial capacity of 100 and load factor of 0.6
    treeset *my_treeset = ts_new(intcmp);

    // Add elements to the treeset
    for (int i = 0; i < 20; i++)
    {
        ts_insert(my_treeset, i);
    }

    // Iterate from the start (index 0) to the end (index count - 1) of the treeset
    for (treeset_iter it = my_treeset->it_start(my_treeset); // Initialize iterator 'it' to the start of the treeset
         !ts_iter_end(&it);                                  // Conditional stop (until it is not at the end)
         ts_iter_next(&it))                                  // Iterator goes one position forward
    {
        // Access the current element
        int e = ts_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = ts_iter_index(&it);

        // Print treeset

        // Index is useful in situations like this
        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == ts_count(my_treeset) - 1)
            printf("%d ]\n", e);
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Index can also be used to iterate over a certain portion of a collection
    size_t limit = ts_count(my_treeset) / 2;
    for (treeset_iter it = my_treeset->it_start(my_treeset); !ts_iter_end(&it); ts_iter_next(&it))
    {
        // Access the current element
        int e = ts_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = ts_iter_index(&it);

        // Print treeset up until limit

        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == limit - 1) // -1 because index is 0 based and we want half of the treeset
        {
            printf("%d ]\n", e);
            break;
        }
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Going backwards
    for (treeset_iter it = my_treeset->it_end(my_treeset); // Initialize iterator 'it' to the end of the treeset
         !ts_iter_start(&it);                              // Conditional stop (until it is not at the start)
         ts_iter_prev(&it))                                // Iterator goes one position backwards
    {
        // Access the current element
        int e = ts_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = ts_iter_index(&it);

        // Print treeset
        // Note how the conditions for the first and last elements are simply swapped
        // Because iterating backwards make the first element be the index count - 1
        // and the last one be the index 0

        // If it is the first element
        if (i == ts_count(my_treeset) - 1)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == 0)
            printf("%d ]\n", e);
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // You can also use iterators in a while loop
    // But to do so you must declare the iterator struct outside the loop first
    treeset_iter it;

    // You have three options:
    // - Initialize it using the functions provided by the treeset [ my_treeset->it_start() and my_treeset->it_end() ]
    // - Initialize it using the init() function (when the iterator is already allocated)
    // - Initialize it using the new() function (allocates on the heap, pointers only)

    it = my_treeset->it_start(my_treeset);
    it = my_treeset->it_end(my_treeset);

    // This is equivalent to it_start()
    ts_iter_init(&it, my_treeset);

    // For pointers
    // Also equivalent to it_start()
    treeset_iter *iter = ts_iter_new(my_treeset);

    // The actual while loop
    while (!ts_iter_end(iter))
    {
        // Accessing elements
        int e = ts_iter_value(iter);
        size_t i = ts_iter_index(iter);

        // Print index and element
        printf("treeset[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator forwards
        ts_iter_next(iter);
    }

    // Send the iterator to the start of the treeset
    ts_iter_to_start(iter);

    // Send the iterator to the end of the treeset
    ts_iter_to_end(iter);

    // Backwards loop
    while (!ts_iter_start(iter))
    {
        // Accessing elements
        int e = ts_iter_value(iter);
        size_t i = ts_iter_index(iter);

        // Print index and element
        printf("treeset[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator backwards
        ts_iter_prev(iter);
    }

    // Certain iterators also allow you to modify the contents it is iterating over
    // Exceptions are collections that depend of the current state of the element
    // like tree-based collections and heap (collections that have some kind of ordering).
    // You also can't change the keys of an associative collection

    // In the case of a treeset or any set, this is not possible because the
    // values of a set are its keys. Changing the elements could cause the set
    // to be in an invalid state.

    // Dispose the iterator called by ts_iter_new()
    ts_iter_free(iter);
    // Dispose the treeset created
    ts_free(my_treeset);

    return 0;
}
