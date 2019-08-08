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
#include "cmc/queue.h"
#include <stdio.h>
#include <inttypes.h>

// Creates a queue of integers
QUEUE_GENERATE(q, queue, /* FMOD */, int)

int main(int argc, char const *argv[])
{
    // Create a new queue with an initial capacity of 100
    queue *my_queue = q_new(100);

    // Add elements to the queue
    for (int i = 0; i < 20; i++)
    {
        q_enqueue(my_queue, i);
    }

    // Iterate from the start (front) to the end (rear) of the queue
    for (queue_iter it = my_queue->it_start(my_queue); // Initialize iterator 'it' to the start of the queue
         !q_iter_end(&it);                             // Conditional stop (until it is not at the end)
         q_iter_next(&it))                             // Iterator goes one position forward
    {
        // Access the current element
        int e = q_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = q_iter_index(&it);

        // Print queue

        // Index is useful in situations like this
        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == q_count(my_queue) - 1)
            printf("%d ]\n", e);
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Index can also be used to iterate over a certain portion of a collection
    size_t limit = q_count(my_queue) / 2;
    for (queue_iter it = my_queue->it_start(my_queue); !q_iter_end(&it); q_iter_next(&it))
    {
        // Access the current element
        int e = q_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = q_iter_index(&it);

        // Print queue up until limit

        // If it is the first element
        if (i == 0)
            printf("[ %d, ", e);
        // If it is the last element
        else if (i == limit - 1) // -1 because index is 0 based and we want half of the queue
        {
            printf("%d ]\n", e);
            break;
        }
        // For elements at the middle
        else
            printf("%d, ", e);
    }

    // Going backwards
    for (queue_iter it = my_queue->it_end(my_queue); // Initialize iterator 'it' to the end of the queue
         !q_iter_start(&it);                         // Conditional stop (until it is not at the start)
         q_iter_prev(&it))                           // Iterator goes one position backwards
    {
        // Access the current element
        int e = q_iter_value(&it);

        // Access the current index (index is relative to all the elements in the iteration)
        size_t i = q_iter_index(&it);

        // Print queue
        // Note how the conditions for the first and last elements are simply swapped
        // Because iterating backwards make the first element be the index count - 1
        // and the last one be the index 0

        // If it is the first element
        if (i == q_count(my_queue) - 1)
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
    queue_iter it;

    // You have three options:
    // - Initialize it using the functions provided by the queue [ my_queue->it_start() and my_queue->it_end() ]
    // - Initialize it using the init() function (when the iterator is already allocated)
    // - Initialize it using the new() function (allocates on the heap, pointers only)

    it = my_queue->it_start(my_queue);
    it = my_queue->it_end(my_queue);

    // This is equivalent to it_start()
    q_iter_init(&it, my_queue);

    // For pointers
    // Also equivalent to it_start()
    queue_iter *iter = q_iter_new(my_queue);

    // The actual while loop
    while (!q_iter_end(iter))
    {
        // Accessing elements
        int e = q_iter_value(iter);
        size_t i = q_iter_index(iter);

        // Print index and element
        printf("queue[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator forwards
        q_iter_next(iter);
    }

    // Send the iterator to the start of the queue
    q_iter_to_start(iter);

    // Send the iterator to the end of the queue
    q_iter_to_end(iter);

    // Backwards loop
    while (!q_iter_start(iter))
    {
        // Accessing elements
        int e = q_iter_value(iter);
        size_t i = q_iter_index(iter);

        // Print index and element
        printf("queue[%2" PRIuMAX "] = %2d\n", i, e);

        // Move iterator backwards
        q_iter_prev(iter);
    }

    // Certain iterators also allow you to modify the contents it is iterating over
    // Exceptions are collections that depend of the current state of the element
    // like tree-based collections and heap (collections that have some kind of ordering).
    // You also can't change the keys of an associative collection

    // Change every element that is divisible by 3 to the number 7
    for (q_iter_to_start(&it); !q_iter_end(&it); q_iter_next(&it))
    {
        // Get a reference to the current value
        int *r = q_iter_rvalue(&it);

        if (r != NULL && *r % 3 == 0)
            *r = 7;
    }

    // Now print it to see the difference
    for (queue_iter it = my_queue->it_start(my_queue); !q_iter_end(&it); q_iter_next(&it))
    {
        int e = q_iter_value(&it);
        size_t i = q_iter_index(&it);

        if (i == 0)
            printf("[ %d, ", e);
        else if (i == q_count(my_queue) - 1)
            printf("%d ]\n", e);
        else
            printf("%d, ", e);
    }

    // Dispose the iterator called by q_iter_new()
    q_iter_free(iter);
    // Dispose the queue created
    q_free(my_queue);

    return 0;
}
