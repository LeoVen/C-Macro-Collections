#include "cmc/hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include "util.c"

HASHMAP_GENERATE(hm, hmap, , char *, size_t)

hmap *load(const char *fname);

int main(int argc, char const *argv[])
{
    hmap *map = load("data.csv");

    if (!map)
        return 1;

    printf("+--------------------------------------------------+\n");
    printf("| Search Country Population by Country Name        |\n");
    printf("|                                                  |\n");
    printf("|     - Type 'exit' to exit                        |\n");
    printf("|     - Type country name to search its population |\n");
    printf("+--------------------------------------------------+\n\n");

    // If you wish to list all items
    // char *key;
    // size_t value, index;

    // hmap_iter iter;
    // for (hm_iter_new(&iter, map); !hm_iter_end(&iter);)
    // {
    //     hm_iter_next(&iter, &key, &value, &index);
    //     printf("%30s : %" PRIuMAX "\n", key, value);

    //     free(key);
    // }

    char sbuffer[1000];

    while (1)
    {
        printf("Query  > ");
        fflush(stdin);
        scanf("%[^\n]s", sbuffer);

        if (strcmp(sbuffer, "exit") == 0)
            break;

        size_t *result = hm_get_ref(map, sbuffer);

        if (result == NULL)
            printf("Result > %s Not Found\n", sbuffer);
        else
            printf("Result > %" PRIuMAX "\n", *result);
    }

    hm_free(map);

    return 0;
}

hmap *load(const char *fname)
{
    FILE *fptr = fopen(fname, "r");

    if (!fptr)
        return NULL;

    hmap *map = hm_new(10000, 0.7, chrcmp, chrhash);

    char buff[400];
    size_t value;

    while (fscanf(fptr, "%[^,],%" PRIuMAX "\n", buff, &value) != EOF)
    {
        char *name = strdup(buff);

        if (!hm_insert(map, name, value))
        {
            printf("\n\n\nERROR\n");
            printf("%s : %" PRIuMAX "", name, value);

            exit(2);
        }
    }

    return map;
}