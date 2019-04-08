/**
 * string.c
 *
 * Creation Date: 08/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include <stdio.h>
#include <string.h>
#include "list.h"

LIST_GENERATE(str, string, /* FMOD */, char)

int main(int argc, char const *argv[])
{
    // How to treat a list as a mutable string without \0 at the end
    char *example1 = "Lorem ipsum";
    char *example2 = "dolor sit amet";
    char *example3 = "consectetur";

    string *my_str = str_new_from(example2, strlen(example2));

    // Add a single character to the front of the list
    str_push_front(my_str, ' ');

    // Add a sequence of characters to the front of the list
    str_prepend(my_str, example1, strlen(example1));

    // Add a charater to the back of the list
    str_push_back(my_str, ' ');

    // Add a sequence of characters to the back of the list
    str_append(my_str, example3, strlen(example3));

    // Iterate through each letter and print it
    string_iter iter;
    str_iter_new(&iter, my_str);
    size_t index;
    char result;

    for (str_iter_tostart(&iter); !str_iter_end(&iter);)
    {
        str_iter_next(&iter, &result, &index);
        putchar(result);
    }

    printf("\n");

    // You can also print it reversed
    for (str_iter_toend(&iter); !str_iter_start(&iter);)
    {
        str_iter_prev(&iter, &result, &index);
        putchar(result);
    }

    printf("\n");

    // Now remove the first letter supposing we don't know where it starts
    for (size_t i = 0; i < str_count(my_str); i++)
    {
        // Find first white space
        if (str_get(my_str, i) == ' ')
        {
            index = i;
            break;
        }
    }

    // Now that we know where the first word is, remove it, including the
    // extra space. [from, to] are inclusive.
    str_remove(my_str, 0, index);

    // Print it once again
    for (str_iter_tostart(&iter); !str_iter_end(&iter);)
    {
        str_iter_next(&iter, &result, &index);
        putchar(result);
    }

    printf("\nString length: %lu\n", str_count(my_str));

    return 0;
}
