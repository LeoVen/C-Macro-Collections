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
#include "cmc/stack.h"
#include <stdio.h>
#include <inttypes.h>

// Creates a stack of integers
STACK_GENERATE(s, stack, /* FMOD */, int)

int main(int argc, char const *argv[])
{
    // Create a new stack with an initial capacity of 100
    stack *my_stack = s_new(100);

    // Add elements to the stack
    for (int i = 0; i < 20; i++)
    {
        s_push(my_stack, i);
    }

    // Iterate from the start (top) to the end (bottom) of the stack
    for (stack_iter it = my_stack->it_start(my_stack); // Initialize iterator 'it' to the start of the stack
         !s_iter_end(&it);                             // Conditional stop (until it is not at the end)
         s_iter_next(&it))                             // Iterator goes one position forward
    {
        // Access the current element
        int e = s_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = s_iter_index(&it);

        // Print stack

        // Index is useful in situations like this
        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == s_count(my_stack) - 1)
            printf("%d ]\n", e);
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Index can also be used to iterate over a certain portion of a collection
    size_t limit = s_count(my_stack) / 2;
    for (stack_iter it = my_stack->it_start(my_stack); !s_iter_end(&it); s_iter_next(&it))
    {
        // Access the current element
        int e = s_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = s_iter_index(&it);

        // Print stack up until limit

        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == limit - 1) // -1 because index is 0 based and we want half of the stack
        {
            printf("%d ]\n", e);
            break;
        }
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Going backwards
    for (stack_iter it = my_stack->it_end(my_stack); // Initialize iterator 'it' to the end of the stack
         !s_iter_start(&it);                         // Conditional stop (until it is not at the start)
         s_iter_prev(&it))                           // Iterator goes one position backwards
    {
        // Access the current element
        int e = s_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = s_iter_index(&it);

        // Print stack
        // Note how the conditions for the first and last elements are simply swapped
        // Because iterating backwards make the first element be the index count - 1
        // and the last one be the index 0

        // If it is the first element
        if (i == s_count(my_stack) - 1)
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
    stack_iter it;

    // You have three options:
    // - Initialize it using the functions provided by the stack [ my_stack->it_start() and my_stack->it_end() ]
    // - Initialize it using the init() function (when the iterator is already allocated)
    // - Initialize it using the new() function (allocates on the heap, pointers only)

    it = my_stack->it_start(my_stack);
    it = my_stack->it_end(my_stack);

    // This is equivalent to it_start()
    s_iter_init(&it, my_stack);

    // For pointers
    // Also equivalent to it_start()
    stack_iter *iter = s_iter_new(my_stack);

    // The actual while loop
    while (!s_iter_end(iter))
    {
        // Accessing elements
        int e = s_iter_value(iter);
        size_t i = s_iter_index(iter);

        // Print index and element
        printf("stack[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator forwards
        s_iter_next(iter);
    }

    // Send the iterator to the start of the stack
    s_iter_to_start(iter);

    // Send the iterator to the end of the stack
    s_iter_to_end(iter);

    // Backwards loop
    while (!s_iter_start(iter))
    {
        // Accessing elements
        int e = s_iter_value(iter);
        size_t i = s_iter_index(iter);

        // Print index and element
        printf("stack[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator backwards
        s_iter_prev(iter);
    }

    // Certain iterators also allow you to modify the contents it is iterating over
    // Exceptions are collections that depend of the current state of the element
    // like tree-based collections and heap (collections that have some kind of ordering).
    // You also can't change the keys of an associative collection

    // Change every element that is divisible by 3 to the number 7
    for (s_iter_to_start(&it); !s_iter_end(&it); s_iter_next(&it))
    {
        // Get a reference to the current value
        int *r = s_iter_rvalue(&it);

        if (r != NULL && *r % 3 == 0)
            *r = 7;
    }

    // Now print it to see the difference
    for (stack_iter it = my_stack->it_start(my_stack); !s_iter_end(&it); s_iter_next(&it))
    {
        int e = s_iter_value(&it);
        size_t i = s_iter_index(&it);

        if (i == 0)
            printf("[ %d, ", e);
        else if (i == s_count(my_stack) - 1)
            printf("%d ]\n", e);
        else
            printf("%d, ", e);
    }

    // Dispose the iterator called by s_iter_new()
    s_iter_free(iter);
    // Dispose the stack created
    s_free(my_stack);

    return 0;
}
