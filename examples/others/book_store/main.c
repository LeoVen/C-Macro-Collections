// Welcome to the book store

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "macro_collections.h"
#include "book.c"
#include "random.c"

COLLECTION_GENERATE(LIST, l, list, , , book *)
COLLECTION_GENERATE(LIST, il, intlist, , , int)
COLLECTION_GENERATE(QUEUE, q, queue, , , book *)
COLLECTION_GENERATE(TREEMAP, map, map, , int, char *)
COLLECTION_GENERATE(TREESET, set, set, , , char *)

list *load(void);
void shuffle(list *l);

int intcmp(int a, int b)
{
    return a - b;
}

int chrcmp(char *a, char *b)
{
    return strcmp(a, b);
}

int main(int argc, char const *argv[])
{
    srand((unsigned)time(NULL));

    // Load our books from database (csv).
    list *books = load();

    // Shuffle our books.
    shuffle(books);

    // Last added books are kept here. These are the 'featured' books.
    queue *shelf = q_new(10);

    // Books in our library. Using this to fast lookup our books.
    map *library = map_new(intcmp);

    // Calculate how many unique book titles
    set *counter = set_new(chrcmp);

    // Add everything to its place.
    FOR_EACH(l, list, books, {
        book *value = l_iter_value(&iter);

        // Keep the last added elements
        if (q_count(shelf) > 7)
        {
            q_dequeue(shelf);
        }

        q_enqueue(shelf, value);

        map_insert(library, value->isbn, value->name);
        set_insert(counter, value->name);
    });

    printf("Library Size: %d\n", map_count(library));
    printf("Unique book titles: %d\n\n", set_count(counter));
    printf("Showcase Books:\n");
    FOR_EACH(q, queue, shelf, {
        book *value = q_iter_value(&iter);
        printf("[ %d : %-45s ]\n", value->isbn, value->name);
    });
    printf("\n");

    set_free(counter);

    int c, isbn;
    char buffer[100];
    while (true)
    {
        printf("\n[0] [ Exit           ]\n");
        printf("[1] [ Search by ISBN ]\n");
        printf("[2] [ Search by Name ]\n");
        printf("> ");
        scanf("%d", &c);

        if (c == 0)
            break;

        if (c == 1)
        {
            while (true)
            {
                printf("[0] [ Exit ]\n");
                printf("ISBN:\n> ");
                scanf("%d", &isbn);

                if (isbn == 0)
                    break;

                char *search = map_get(library, isbn);

                if (search == NULL)
                    printf("No book found\n");
                else
                    printf("%s\n", search);
            }
        }
        else if (c == 2)
        {
            printf("Name:\n> ");
            fflush(stdin);
            fgets(buffer, 100, stdin);
            // Remove newline character
            buffer[strcspn(buffer, "\n")] = 0;

            intlist *search_list = il_new(32);

            FOR_EACH(map, map, library, {
                int key = map_iter_key(&iter);
                char *value = map_iter_value(&iter);

                if (chrcmp(buffer, value) == 0)
                    il_push_back(search_list, key);
            });

            FOR_EACH(il, intlist, search_list, {
                printf("[ %d ]\n", il_iter_value(&iter));
            });

            il_free(search_list);
        }
    }

    // Free all memory used by struct book
    FOR_EACH(l, list, books, {
        book *b = l_iter_value(&iter);

        if (b)
            free(b->name);

        free(b);
    });

    l_free(books);
    q_free(shelf);
    map_free(library);

    return 0;
}

list *load(void)
{
    FILE *fptr = fopen("books.csv", "r");

    int isbn;
    char buffer[1000];
    list *books = l_new(1000);

    while (fscanf(fptr, "%d,%[^\n\r]s", &isbn, buffer) != EOF)
    {
        book *b = new_book(isbn, buffer);
        l_push_back(books, b);
    }

    return books;
}

void shuffle(list *l)
{
    size_t min = 0, max = l_count(l) - 1;

    for (size_t i = 0; i < max; i++)
    {
        size_t j = randsize(min, max);
        book *tmp = l->buffer[j];
        l->buffer[j] = l->buffer[i];
        l->buffer[i] = tmp;
    }
}
