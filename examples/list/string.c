/**
 * string.c
 *
 * Creation Date: 08/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/list.h"
#include <stdio.h>
#include <string.h>

LIST_GENERATE(str, string, /* FMOD */, char)

// Comparison function
int chrcmp(char a, char b)
{
    return (a > b) - (a < b);
}

void print_str(string *str)
{
    // If the buffer is not completely filled, there will be at least one null
    // byte in the end of the list's contents. This printf is possible but very
    // dangerous as you might end up printing something else from memory until
    // a null byte.
    printf("%s\n", str->buffer);

    printf("Size: %llu\n", str_count(str));
}

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
    str_iter_init(&iter, my_str);
    size_t index;
    char result;

    // Printing each character
    for (str_iter_to_start(&iter); !str_iter_end(&iter); str_iter_next(&iter))
    {
        putchar(str_iter_value(&iter));
    }

    printf("\n\n");

    // You can also print it reversed
    for (str_iter_to_end(&iter); !str_iter_start(&iter); str_iter_prev(&iter))
    {
        putchar(str_iter_value(&iter));
    }

    printf("\n\n");

    // Now remove the first letter supposing we don't know where it starts
    index = str_indexof(my_str, ' ', chrcmp, true);

    if (index == str_count(my_str))
    {
        // char not found...
        // Treat error
    }

    // Now that we know where the first word is, remove it, including the
    // extra space. [from, to] are inclusive.
    str_remove(my_str, 0, index);

    // Print it once again
    for (str_iter_to_start(&iter); !str_iter_end(&iter); str_iter_next(&iter))
    {
        putchar(str_iter_value(&iter));
    }

    printf("\nString length: %lu\n\n", str_count(my_str));

    print_str(my_str);

    // Removing last word.
    // Finding last white space:
    index = str_indexof(my_str, ' ', chrcmp, false);

    // Extract last word including white space
    string *str_out = str_extract(my_str, index, str_count(my_str) - 1);

    if (!str_out)
        printf("Could not extract string\n");
    else
    {
        // Pop white space
        str_pop_front(str_out);

        for (str_iter_init(&iter, my_str); !str_iter_end(&iter); str_iter_next(&iter))
        {
            putchar(str_iter_value(&iter));
        }

        printf("\nString length: %lu\n\n", str_count(my_str));

        for (str_iter_init(&iter, str_out); !str_iter_end(&iter); str_iter_next(&iter))
        {
            putchar(str_iter_value(&iter));
        }

        printf("\nString length: %lu\n", str_count(str_out));
    }

    // Free strings
    str_free(my_str);
    str_free(str_out);

    printf("\n----------------------------------------\n\n");

    // Another example follows
    // Using prepend, insert, append, remove and extract functions
    string *str = str_new(100);

    str_prepend(str, "Hello World!", 12);

    print_str(str);

    str_insert(str, "Awesome ", 8, 6);

    print_str(str);

    str_append(str, " Indigo.", 8);

    print_str(str);

    str_remove(str, 14, 20);

    print_str(str);

    string *str_res = str_extract(str, 5, 12);

    print_str(str);
    print_str(str_res);

    str_free(str);
    str_free(str_res);

    return 0;
}
