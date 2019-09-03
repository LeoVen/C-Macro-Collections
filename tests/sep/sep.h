#include "../../src/macro_collections.h"

COLLECTION_GENERATE_HEADER(LIST, l, list, /* FMOD */, /* K */, int)
COLLECTION_GENERATE_HEADER(STACK, s, stack, /* FMOD */, /* K */, int)
COLLECTION_GENERATE_HEADER(QUEUE, q, queue, /* FMOD */, /* K */, int)
COLLECTION_GENERATE_HEADER(DEQUE, d, deque, /* FMOD */, /* K */, int)
COLLECTION_GENERATE_HEADER(LINKEDLIST, ll, linked, /* FMOD */, /* K */, int)
COLLECTION_GENERATE_HEADER(HEAP, h, heap, /* FMOD */, /* K */, int)
COLLECTION_GENERATE_HEADER(TREESET, ts, tset, /* FMOD */, /* K */, int)
COLLECTION_GENERATE_HEADER(TREEMAP, tm, tmap, /* FMOD */, int, int)
COLLECTION_GENERATE_HEADER(HASHSET, hs, hset, /* FMOD */, /* K */, int)
COLLECTION_GENERATE_HEADER(HASHMAP, hm, hmap, /* FMOD */, int, int)

COLLECTION_GENERATE_HEADER(INTERVALHEAP, ih, iheap, /* FMOD */, , int)
COLLECTION_GENERATE_HEADER(MULTIMAP, mm, mmap, /* FMOD */, int, int)
COLLECTION_GENERATE_HEADER(MULTISET, ms, mset, , , int)
