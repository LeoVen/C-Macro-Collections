// Welcome to the book store

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "../../src/macro_collections.h"
#include "book.c"
#include "random.c"

COLLECTION_GENERATE(LIST, PUBLIC, l, list, , , book *)
COLLECTION_GENERATE(QUEUE, PUBLIC, q, queue, , , book *)
COLLECTION_GENERATE(TREEMAP, PUBLIC, map, map, , int, char *)
COLLECTION_GENERATE(TREESET, PUBLIC, set, set, , , char *)

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
    FOR_EACH(l, list, book *, books, {
        // Keep the last added elements
        if (q_count(shelf) > 7)
        {
            q_dequeue(shelf);
        }

        q_enqueue(shelf, var);

        map_insert(library, var->isbn, var->name);
        set_insert(counter, var->name);
    })

    printf("Library Size: %d\n", map_count(library));
    printf("Unique book titles: %d\n\n", set_count(counter));
    printf("Showcase Books:\n");
    FOR_EACH(q, queue, book *, shelf, {
        printf("[ %d : %-45s ]\n", var->isbn, var->name);
    })
    printf("\n");
    printf("[0 to exit]\n\n");

    while (true)
    {
        int isbn;
        printf("Query ISBN:\n> ");
        scanf("%d", &isbn);

        if (isbn == 0)
            break;

        char *search = map_get(library, isbn);

        if (search == NULL)
            printf("Not found\n");
        else
            printf("%s\n", search);
    }

    // Free all memory used by struct book
    FOR_EACH(l, list, book *, books, {
        free(var->name);
        free(var);
    })

    l_free(books);
    q_free(shelf);
    map_free(library);
    set_free(counter);

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
