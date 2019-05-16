// Tests all containers if they accept a pinter type
// Compile with -Wall -Wextra
#include "../src/macro_collections.h"
#include <stdio.h>

typedef struct my_struct_s
{
    int a, b;
} my_struct;

COLLECTION_GENERATE(LIST, PUBLIC, l, list, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(STACK, PUBLIC, s, stack, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(QUEUE, PUBLIC, q, queue, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(DEQUE, PUBLIC, d, deque, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(LINKEDLIST, PUBLIC, ll, linked, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(HEAP, PUBLIC, h, heap, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(TREESET, PUBLIC, ts, tset, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(TREEMAP, PUBLIC, tm, tmap, /* FMOD */, my_struct *, my_struct *)
COLLECTION_GENERATE(HASHSET, PUBLIC, hs, hset, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(HASHMAP, PUBLIC, hm, hmap, /* FMOD */, my_struct *, my_struct *)

int main(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|           Pointer Data Type is Working           |\n");
    printf("+--------------------------------------------------+\n");

    return 0;
}
