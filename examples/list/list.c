/**
 * list.c
 *
 * Creation Date: 08/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/list.h"
#include <stdio.h>

LIST_GENERATE(l, list, /* FMOD */, int)

int main(int argc, char const *argv[])
{
    // Create a list with internal capacity of 100
    list *my_list = l_new(100);

    // - push_front (index: 0)         [Complexity: O(N)]
    // - push       (index: give)      [Complexity: O(N)*]
    // - push_back  (index: count)     [Complexity: O(1)]
    //
    // - pop_front  (index: 0)         [Complexity: O(N)]
    // - pop        (index: given)     [Complexity: O(N)*]
    // - push_back  (index: count - 1) [Complexity: O(1)]
    //
    // *average
    //
    // Average list Input/Output: O(N)

    for (int i = 0; i < 200; i++)
    {
        // Add elements to the list
        l_push_back(my_list, i);
    }

    // Erase all values from the list
    l_clear(my_list);

    // Now that list is empty, add only even numbers
    for (int i = 0; i < 200; i++)
    {
        // Add even numbers to a given index. In this case they are being added
        // to the end of the list (count index == push_back). With a condition
        // of being even (i % 2 == 0)
        bool added = l_push_if(my_list, i, l_count(my_list), i % 2 == 0);

        // If added is true, the element was added to the list
    }

    // Iterator
    list_iter iter;
    int result, sum = 0;
    size_t index;

    // Iterate over each element starting from the start (iter_new positions
    // the iterator to the start of the list) until the end of the list.
    for (l_iter_init(&iter, my_list); !l_iter_end(&iter); l_iter_next(&iter))
    {
        // Here index corresponds to the position of the result relative to all
        // other elements in the iteration. It is only a coincidence that the
        // index here is the same as the result's position in the list.
        index = l_iter_index(&iter);
        result = l_iter_value(&iter);

        // Do whatever you want with result and index, e.g.
        sum += result * index;
    }

    // Release resources
    l_free(my_list);

    return 0;
}
