/**
 * iterator.c
 *
 * Creation Date: 10/05/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * Iterator Example
 */
#include "cmc/linkedlist.h"
#include <stdio.h>
#include <inttypes.h>

// Creates a linkedlist of integers
LINKEDLIST_GENERATE(ll, linkedlist, /* FMOD */, int)

int main(int argc, char const *argv[])
{
    // Create a new linkedlist
    linkedlist *my_linkedlist = ll_new();

    // Add elements to the linkedlist
    for (int i = 0; i < 20; i++)
    {
        ll_push_back(my_linkedlist, i);
    }

    // Iterate from the start (index 0) to the end (index count - 1) of the linkedlist
    for (linkedlist_iter it = my_linkedlist->it_start(my_linkedlist); // Initialize iterator 'it' to the start of the linkedlist
         !ll_iter_end(&it);                                           // Conditional stop (until it is not at the end)
         ll_iter_next(&it))                                           // Iterator goes one position forward
    {
        // Access the current element
        int e = ll_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = ll_iter_index(&it);

        // Print linkedlist

        // Index is useful in situations like this
        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == ll_count(my_linkedlist) - 1)
            printf("%d ]\n", e);
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Index can also be used to iterate over a certain portion of a collection
    size_t limit = ll_count(my_linkedlist) / 2;
    for (linkedlist_iter it = my_linkedlist->it_start(my_linkedlist); !ll_iter_end(&it); ll_iter_next(&it))
    {
        // Access the current element
        int e = ll_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = ll_iter_index(&it);

        // Print linkedlist up until limit

        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == limit - 1) // -1 because index is 0 based and we want half of the linkedlist
        {
            printf("%d ]\n", e);
            break;
        }
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Going backwards
    for (linkedlist_iter it = my_linkedlist->it_end(my_linkedlist); // Initialize iterator 'it' to the end of the linkedlist
         !ll_iter_start(&it);                                       // Conditional stop (until it is not at the start)
         ll_iter_prev(&it))                                         // Iterator goes one position backwards
    {
        // Access the current element
        int e = ll_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = ll_iter_index(&it);

        // Print linkedlist
        // Note how the conditions for the first and last elements are simply swapped
        // Because iterating backwards make the first element be the index count - 1
        // and the last one be the index 0

        // If it is the first element
        if (i == ll_count(my_linkedlist) - 1)
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
    linkedlist_iter it;

    // You have three options:
    // - Initialize it using the functions provided by the linkedlist [ my_linkedlist->it_start() and my_linkedlist->it_end() ]
    // - Initialize it using the init() function (when the iterator is already allocated)
    // - Initialize it using the new() function (allocates on the heap, pointers only)

    it = my_linkedlist->it_start(my_linkedlist);
    it = my_linkedlist->it_end(my_linkedlist);

    // This is equivalent to it_start()
    ll_iter_init(&it, my_linkedlist);

    // For pointers
    // Also equivalent to it_start()
    linkedlist_iter *iter = ll_iter_new(my_linkedlist);

    // The actual while loop
    while (!ll_iter_end(iter))
    {
        // Accessing elements
        int e = ll_iter_value(iter);
        size_t i = ll_iter_index(iter);

        // Print index and element
        printf("linkedlist[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator forwards
        ll_iter_next(iter);
    }

    // Send the iterator to the start of the linkedlist
    ll_iter_to_start(iter);

    // Send the iterator to the end of the linkedlist
    ll_iter_to_end(iter);

    // Backwards loop
    while (!ll_iter_start(iter))
    {
        // Accessing elements
        int e = ll_iter_value(iter);
        size_t i = ll_iter_index(iter);

        // Print index and element
        printf("linkedlist[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator backwards
        ll_iter_prev(iter);
    }

    // Certain iterators also allow you to modify the contents it is iterating over
    // Exceptions are collections that depend of the current state of the element
    // like tree-based collections and heap (collections that have some kind of ordering).
    // You also can't change the keys of an associative collection

    // Change every element that is divisible by 2 to the number 7
    for (ll_iter_to_start(&it); !ll_iter_end(&it); ll_iter_next(&it))
    {
        // Get a reference to the current value
        int *r = ll_iter_rvalue(&it);

        if (r != NULL && *r % 2 == 0)
            *r = 7;
    }

    // Now print it to see the difference
    for (linkedlist_iter it = my_linkedlist->it_start(my_linkedlist); !ll_iter_end(&it); ll_iter_next(&it))
    {
        int e = ll_iter_value(&it);
        size_t i = ll_iter_index(&it);

        if (i == 0)
            printf("[ %d, ", e);
        else if (i == ll_count(my_linkedlist) - 1)
            printf("%d ]\n", e);
        else
            printf("%d, ", e);
    }

    // Dispose the iterator called by ll_iter_new()
    ll_iter_free(iter);
    // Dispose the linkedlist created
    ll_free(my_linkedlist);

    return 0;
}
