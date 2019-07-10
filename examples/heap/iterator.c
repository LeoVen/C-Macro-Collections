/**
 * iterator.c
 *
 * Creation Date: 15/05/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * Iterator Example
 */
#include "cmc/heap.h"
#include <stdio.h>
#include <inttypes.h>

// Int compare function
int intcmp(int a, int b)
{
    return a - b;
}

// Creates a heap of integers
HEAP_GENERATE(h, heap, /* FMOD */, int)

int main(int argc, char const *argv[])
{
    // Create a new max heap with an initial capacity of 100
    heap *my_heap = h_new(100, cmc_max_heap, intcmp);

    // Add elements to the heap
    for (int i = 0; i < 20; i++)
    {
        h_insert(my_heap, i);
    }

    // Iterate from the start (index 0) to the end (index count - 1) of the heap
    for (heap_iter it = my_heap->it_start(my_heap); // Initialize iterator 'it' to the start of the heap
         !h_iter_end(&it);                          // Conditional stop (until it is not at the end)
         h_iter_next(&it))                          // Iterator goes one position forward
    {
        // Access the current element
        int e = h_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = h_iter_index(&it);

        // Print heap

        // Index is useful in situations like this
        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == h_count(my_heap) - 1)
            printf("%d ]\n", e);
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Index can also be used to iterate over a certain portion of a collection
    size_t limit = h_count(my_heap) / 2;
    for (heap_iter it = my_heap->it_start(my_heap); !h_iter_end(&it); h_iter_next(&it))
    {
        // Access the current element
        int e = h_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = h_iter_index(&it);

        // Print heap up until limit

        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == limit - 1) // -1 because index is 0 based and we want half of the heap
        {
            printf("%d ]\n", e);
            break;
        }
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Going backwards
    for (heap_iter it = my_heap->it_end(my_heap); // Initialize iterator 'it' to the end of the heap
         !h_iter_start(&it);                      // Conditional stop (until it is not at the start)
         h_iter_prev(&it))                        // Iterator goes one position backwards
    {
        // Access the current element
        int e = h_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = h_iter_index(&it);

        // Print heap
        // Note how the conditions for the first and last elements are simply swapped
        // Because iterating backwards make the first element be the index count - 1
        // and the last one be the index 0

        // If it is the first element
        if (i == h_count(my_heap) - 1)
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
    heap_iter it;

    // You have three options:
    // - Initialize it using the functions provided by the heap [ my_heap->it_start() and my_heap->it_end() ]
    // - Initialize it using the init() function (when the iterator is already allocated)
    // - Initialize it using the new() function (allocates on the heap, pointers only)

    it = my_heap->it_start(my_heap);
    it = my_heap->it_end(my_heap);

    // This is equivalent to it_start()
    h_iter_init(&it, my_heap);

    // For pointers
    // Also equivalent to it_start()
    heap_iter *iter = h_iter_new(my_heap);

    // The actual while loop
    while (!h_iter_end(iter))
    {
        // Accessing elements
        int e = h_iter_value(iter);
        size_t i = h_iter_index(iter);

        // Print index and element
        printf("heap[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator forwards
        h_iter_next(iter);
    }

    // Send the iterator to the start of the heap
    h_iter_to_start(iter);

    // Send the iterator to the end of the heap
    h_iter_to_end(iter);

    // Backwards loop
    while (!h_iter_start(iter))
    {
        // Accessing elements
        int e = h_iter_value(iter);
        size_t i = h_iter_index(iter);

        // Print index and element
        printf("heap[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator backwards
        h_iter_prev(iter);
    }

    // Certain iterators also allow you to modify the contents it is iterating over
    // Exceptions are collections that depend of the current state of the element
    // like tree-based collections and heap (collections that have some kind of ordering).
    // You also can't change the keys of an associative collection

    // In the case of a heap this is not possible because the heap have its
    // elements ordered. Changing them could cause the heap to be in an invalid
    // state.

    // Dispose the iterator called by h_iter_new()
    h_iter_free(iter);
    // Dispose the heap created
    h_free(my_heap);

    return 0;
}
