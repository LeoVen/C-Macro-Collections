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
#include "cmc/list.h"
#include <stdio.h>
#include <inttypes.h>

// Creates a list of integers
LIST_GENERATE(l, list, /* FMOD */, int)

int main(int argc, char const *argv[])
{
    // Create a new list with an initial capacity of 100
    list *my_list = l_new(100);

    // Add elements to the list
    for (int i = 0; i < 20; i++)
    {
        l_push_back(my_list, i);
    }

    // Iterate from the start (index 0) to the end (index count - 1) of the list
    for (list_iter it = my_list->it_start(my_list); // Initialize iterator 'it' to the start of the list
         !l_iter_end(&it);                          // Conditional stop (until it is not at the end)
         l_iter_next(&it))                          // Iterator goes one position forward
    {
        // Access the current element
        int e = l_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = l_iter_index(&it);

        // Print list

        // Index is useful in situations like this
        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == l_count(my_list) - 1)
            printf("%d ]\n", e);
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Index can also be used to iterate over a certain portion of a collection
    size_t limit = l_count(my_list) / 2;
    for (list_iter it = my_list->it_start(my_list); !l_iter_end(&it); l_iter_next(&it))
    {
        // Access the current element
        int e = l_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = l_iter_index(&it);

        // Print list up until limit

        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == limit - 1) // -1 because index is 0 based and we want half of the list
        {
            printf("%d ]\n", e);
            break;
        }
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Going backwards
    for (list_iter it = my_list->it_end(my_list); // Initialize iterator 'it' to the end of the list
         !l_iter_start(&it);                      // Conditional stop (until it is not at the start)
         l_iter_prev(&it))                        // Iterator goes one position backwards
    {
        // Access the current element
        int e = l_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = l_iter_index(&it);

        // Print list
        // Note how the conditions for the first and last elements are simply swapped
        // Because iterating backwards make the first element be the index count - 1
        // and the last one be the index 0

        // If it is the first element
        if (i == l_count(my_list) - 1)
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
    list_iter it;

    // You have three options:
    // - Initialize it using the functions provided by the list [ my_list->it_start() and my_list->it_end() ]
    // - Initialize it using the init() function (when the iterator is already allocated)
    // - Initialize it using the new() function (allocates on the heap, pointers only)

    it = my_list->it_start(my_list);
    it = my_list->it_end(my_list);

    // This is equivalent to it_start()
    l_iter_init(&it, my_list);

    // For pointers
    // Also equivalent to it_start()
    list_iter *iter = l_iter_new(my_list);

    // The actual while loop
    while (!l_iter_end(iter))
    {
        // Accessing elements
        int e = l_iter_value(iter);
        size_t i = l_iter_index(iter);

        // Print index and element
        printf("list[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator forwards
        l_iter_next(iter);
    }

    // Send the iterator to the start of the list
    l_iter_to_start(iter);

    // Send the iterator to the end of the list
    l_iter_to_end(iter);

    // Backwards loop
    while (!l_iter_start(iter))
    {
        // Accessing elements
        int e = l_iter_value(iter);
        size_t i = l_iter_index(iter);

        // Print index and element
        printf("list[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator backwards
        l_iter_prev(iter);
    }

    // Certain iterators also allow you to modify the contents it is iterating over
    // Exceptions are collections that depend of the current state of the element
    // like tree-based collections and heap (collections that have some kind of ordering).
    // You also can't change the keys of an associative collection

    // Change every element that is divisible by 2 to the number 7
    for (l_iter_to_start(&it); !l_iter_end(&it); l_iter_next(&it))
    {
        // Get a reference to the current value
        int *r = l_iter_rvalue(&it);

        if (r != NULL && *r % 2 == 0)
            *r = 7;
    }

    // Now print it to see the difference
    for (list_iter it = my_list->it_start(my_list); !l_iter_end(&it); l_iter_next(&it))
    {
        int e = l_iter_value(&it);
        size_t i = l_iter_index(&it);

        if (i == 0)
            printf("[ %d, ", e);
        else if (i == l_count(my_list) - 1)
            printf("%d ]\n", e);
        else
            printf("%d, ", e);
    }

    // Dispose the iterator called by l_iter_new()
    l_iter_free(iter);
    // Dispose the list created
    l_free(my_list);

    return 0;
}
