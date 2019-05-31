// Tests all containers if they accept a pinter type
// Compile with -Wall -Wextra
#include "../src/macro_collections.h"
#include <stdio.h>

typedef struct my_struct_s
{
    int a, b;
} my_struct;

COLLECTION_GENERATE(LIST, l, list, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(STACK, s, stack, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(QUEUE, q, queue, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(DEQUE, d, deque, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(LINKEDLIST, ll, linked, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(HEAP, h, heap, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(TREESET, ts, tset, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(TREEMAP, tm, tmap, /* FMOD */, my_struct *, my_struct *)
COLLECTION_GENERATE(HASHSET, hs, hset, /* FMOD */, /* K */, my_struct *)
COLLECTION_GENERATE(HASHMAP, hm, hmap, /* FMOD */, my_struct *, my_struct *)

int main(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|           Pointer Data Type is Working           |\n");
    printf("+--------------------------------------------------+\n");

    return 0;
}
