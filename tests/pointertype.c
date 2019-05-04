// Tests all containers if they accept a pinter type
// Compile with -Wall -Wextra
#include "../src/macro_collections.h"
#include <stdio.h>

COLLECTION_GENERATE(LIST, PUBLIC, l, list, /* FMOD */, /* K */, char *)
COLLECTION_GENERATE(STACK, PUBLIC, s, stack, /* FMOD */, /* K */, char *)
COLLECTION_GENERATE(QUEUE, PUBLIC, q, queue, /* FMOD */, /* K */, char *)
COLLECTION_GENERATE(DEQUE, PUBLIC, d, deque, /* FMOD */, /* K */, char *)
COLLECTION_GENERATE(LINKEDLIST, PUBLIC, ll, linked, /* FMOD */, /* K */, char *)
COLLECTION_GENERATE(HEAP, PUBLIC, h, heap, /* FMOD */, /* K */, char *)
COLLECTION_GENERATE(TREESET, PUBLIC, ts, tset, /* FMOD */, /* K */, char *)
COLLECTION_GENERATE(TREEMAP, PUBLIC, tm, tmap, /* FMOD */, char *, char *)
COLLECTION_GENERATE(HASHSET, PUBLIC, hs, hset, /* FMOD */, /* K */, char *)
COLLECTION_GENERATE(HASHMAP, PUBLIC, hm, hmap, /* FMOD */, char *, char *)

int main(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|           Pointer Data Type is Working           |\n");
    printf("+--------------------------------------------------+\n");

    return 0;
}
