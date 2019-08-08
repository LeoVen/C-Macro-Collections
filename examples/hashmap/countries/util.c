#include <stdlib.h>

int chrcmp(char *a, char *b)
{
    return strcmp(a, b);
}

size_t chrhash(char *str)
{
    size_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
