/**
 * iterator.c
 *
 * Creation Date: 13/05/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * Iterator Example
 */
#include "cmc/deque.h"
#include <stdio.h>
#include <inttypes.h>

// Creates a deque of integers
DEQUE_GENERATE(d, deque, /* FMOD */, int)

int main(int argc, char const *argv[])
{
    // Create a new deque with an initial capacity of 100
    deque *my_deque = d_new(100);

    // Add elements to the deque
    for (int i = 0; i < 20; i++)
    {
        // Add odd numbers to the front
        d_push_front_if(my_deque, i, !(i % 2 == 0));

        // And even numbers to the back
        d_push_back_if(my_deque, i, i % 2 == 0);
    }

    // Iterate from the start (front index) to the end (rear index) of the deque
    for (deque_iter it = my_deque->it_start(my_deque); // Initialize iterator 'it' to the start of the deque
         !d_iter_end(&it);                             // Conditional stop (until it is not at the end)
         d_iter_next(&it))                             // Iterator goes one position forward
    {
        // Access the current element
        int e = d_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = d_iter_index(&it);

        // Print deque

        // Index is useful in situations like this
        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == d_count(my_deque) - 1)
            printf("%d ]\n", e);
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Index can also be used to iterate over a certain portion of a collection
    size_t limit = d_count(my_deque) / 2;
    for (deque_iter it = my_deque->it_start(my_deque); !d_iter_end(&it); d_iter_next(&it))
    {
        // Access the current element
        int e = d_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = d_iter_index(&it);

        // Print deque up until limit

        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == limit - 1) // -1 because index is 0 based and we want half of the deque
        {
            printf("%d ]\n", e);
            break;
        }
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Going backwards
    for (deque_iter it = my_deque->it_end(my_deque); // Initialize iterator 'it' to the end of the deque
         !d_iter_start(&it);                         // Conditional stop (until it is not at the start)
         d_iter_prev(&it))                           // Iterator goes one position backwards
    {
        // Access the current element
        int e = d_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = d_iter_index(&it);

        // Print deque
        // Note how the conditions for the first and last elements are simply swapped
        // Because iterating backwards make the first element be the index count - 1
        // and the last one be the index 0

        // If it is the first element
        if (i == d_count(my_deque) - 1)
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
    deque_iter it;

    // You have three options:
    // - Initialize it using the functions provided by the deque [ my_deque->it_start() and my_deque->it_end() ]
    // - Initialize it using the init() function (when the iterator is already allocated)
    // - Initialize it using the new() function (allocates on the heap, pointers only)

    it = my_deque->it_start(my_deque);
    it = my_deque->it_end(my_deque);

    // This is equivalent to it_start()
    d_iter_init(&it, my_deque);

    // For pointers
    // Also equivalent to it_start()
    deque_iter *iter = d_iter_new(my_deque);

    // The actual while loop
    while (!d_iter_end(iter))
    {
        // Accessing elements
        int e = d_iter_value(iter);
        size_t i = d_iter_index(iter);

        // Print index and element
        printf("deque[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator forwards
        d_iter_next(iter);
    }

    // Send the iterator to the start of the deque
    d_iter_to_start(iter);

    // Send the iterator to the end of the deque
    d_iter_to_end(iter);

    // Backwards loop
    while (!d_iter_start(iter))
    {
        // Accessing elements
        int e = d_iter_value(iter);
        size_t i = d_iter_index(iter);

        // Print index and element
        printf("deque[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator backwards
        d_iter_prev(iter);
    }

    // Certain iterators also allow you to modify the contents it is iterating over
    // Exceptions are collections that depend of the current state of the element
    // like tree-based collections and heap (collections that have some kind of ordering).
    // You also can't change the keys of an associative collection

    // Change every element that is divisible by 3 to the number 7
    for (d_iter_to_start(&it); !d_iter_end(&it); d_iter_next(&it))
    {
        // Get a reference to the current value
        int *r = d_iter_rvalue(&it);

        if (r != NULL && *r % 3 == 0)
            *r = 7;
    }

    // Now print it to see the difference
    for (deque_iter it = my_deque->it_start(my_deque); !d_iter_end(&it); d_iter_next(&it))
    {
        int e = d_iter_value(&it);
        size_t i = d_iter_index(&it);

        if (i == 0)
            printf("[ %d, ", e);
        else if (i == d_count(my_deque) - 1)
            printf("%d ]\n", e);
        else
            printf("%d, ", e);
    }

    // Dispose the iterator called by d_iter_new()
    d_iter_free(iter);
    // Dispose the deque created
    d_free(my_deque);

    return 0;
}
