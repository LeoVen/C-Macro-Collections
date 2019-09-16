// Tests all containers if they accept all C data types
// Compile with -Wall -Wextra
#include "../src/macro_collections.h"
#include <stdio.h>

typedef struct struct_s
{
    int a, b;
    char *c, *d;
    double e[10];
} struct_t;

typedef enum enum_e
{
    HELLO,
    WORLD,
    NONE = 0
} enum_t;

typedef union union_u {
    enum enum_e a;
    struct struct_s b;
    double c;
    char *d;
} union_t;

typedef int (*func_t)(int, double, char *);

CMC_COLLECTION_GENERATE(LIST, l0, list0, /* K */, struct_t *)
CMC_COLLECTION_GENERATE(STACK, s0, stack0, /* K */, struct_t *)
CMC_COLLECTION_GENERATE(QUEUE, q0, queue0, /* K */, struct_t *)
CMC_COLLECTION_GENERATE(DEQUE, d0, deque0, /* K */, struct_t *)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll0, linked0, /* K */, struct_t *)
CMC_COLLECTION_GENERATE(HEAP, h0, heap0, /* K */, struct_t *)
CMC_COLLECTION_GENERATE(TREESET, ts0, tset0, /* K */, struct_t *)
CMC_COLLECTION_GENERATE(TREEMAP, tm0, tmap0, struct_t *, struct_t *)
CMC_COLLECTION_GENERATE(HASHSET, hs0, hset0, /* K */, struct_t *)
CMC_COLLECTION_GENERATE(HASHMAP, hm0, hmap0, struct_t *, struct_t *)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih0, iheap0, struct_t *, struct_t *)
CMC_COLLECTION_GENERATE(MULTIMAP, mm0, mmap0, struct_t *, struct_t *)
CMC_COLLECTION_GENERATE(MULTISET, ms0, mset0, struct_t *, struct_t *)

CMC_COLLECTION_GENERATE(LIST, l1, list1, /* K */, struct_t)
CMC_COLLECTION_GENERATE(STACK, s1, stack1, /* K */, struct_t)
CMC_COLLECTION_GENERATE(QUEUE, q1, queue1, /* K */, struct_t)
CMC_COLLECTION_GENERATE(DEQUE, d1, deque1, /* K */, struct_t)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll1, linked1, /* K */, struct_t)
CMC_COLLECTION_GENERATE(HEAP, h1, heap1, /* K */, struct_t)
CMC_COLLECTION_GENERATE(TREESET, ts1, tset1, /* K */, struct_t)
CMC_COLLECTION_GENERATE(TREEMAP, tm1, tmap1, struct_t, struct_t)
CMC_COLLECTION_GENERATE(HASHSET, hs1, hset1, /* K */, struct_t)
CMC_COLLECTION_GENERATE(HASHMAP, hm1, hmap1, struct_t, struct_t)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih1, iheap1, struct_t, struct_t)
CMC_COLLECTION_GENERATE(MULTIMAP, mm1, mmap1, struct_t, struct_t)
CMC_COLLECTION_GENERATE(MULTISET, ms1, mset1, struct_t, struct_t)

CMC_COLLECTION_GENERATE(LIST, l2, list2, /* K */, int)
CMC_COLLECTION_GENERATE(STACK, s2, stack2, /* K */, int)
CMC_COLLECTION_GENERATE(QUEUE, q2, queue2, /* K */, int)
CMC_COLLECTION_GENERATE(DEQUE, d2, deque2, /* K */, int)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll2, linked2, /* K */, int)
CMC_COLLECTION_GENERATE(HEAP, h2, heap2, /* K */, int)
CMC_COLLECTION_GENERATE(TREESET, ts2, tset2, /* K */, int)
CMC_COLLECTION_GENERATE(TREEMAP, tm2, tmap2, int, int)
CMC_COLLECTION_GENERATE(HASHSET, hs2, hset2, /* K */, int)
CMC_COLLECTION_GENERATE(HASHMAP, hm2, hmap2, int, int)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih2, iheap2, /* K */, int)
CMC_COLLECTION_GENERATE(MULTIMAP, mm2, mmap2, int, int)
CMC_COLLECTION_GENERATE(MULTISET, ms2, mset2, /* K */, int)

CMC_COLLECTION_GENERATE(LIST, l3, list3, /* K */, int *)
CMC_COLLECTION_GENERATE(STACK, s3, stack3, /* K */, int *)
CMC_COLLECTION_GENERATE(QUEUE, q3, queue3, /* K */, int *)
CMC_COLLECTION_GENERATE(DEQUE, d3, deque3, /* K */, int *)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll3, linked3, /* K */, int *)
CMC_COLLECTION_GENERATE(HEAP, h3, heap3, /* K */, int *)
CMC_COLLECTION_GENERATE(TREESET, ts3, tset3, /* K */, int *)
CMC_COLLECTION_GENERATE(TREEMAP, tm3, tmap3, int *, int *)
CMC_COLLECTION_GENERATE(HASHSET, hs3, hset3, /* K */, int *)
CMC_COLLECTION_GENERATE(HASHMAP, hm3, hmap3, int *, int *)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih3, iheap3, /* K */, int *)
CMC_COLLECTION_GENERATE(MULTIMAP, mm3, mmap3, int *, int *)
CMC_COLLECTION_GENERATE(MULTISET, ms3, mset3, /* K */, int *)

CMC_COLLECTION_GENERATE(LIST, l4, list4, /* K */, enum_t *)
CMC_COLLECTION_GENERATE(STACK, s4, stack4, /* K */, enum_t *)
CMC_COLLECTION_GENERATE(QUEUE, q4, queue4, /* K */, enum_t *)
CMC_COLLECTION_GENERATE(DEQUE, d4, deque4, /* K */, enum_t *)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll4, linked4, /* K */, enum_t *)
CMC_COLLECTION_GENERATE(HEAP, h4, heap4, /* K */, enum_t *)
CMC_COLLECTION_GENERATE(TREESET, ts4, tset4, /* K */, enum_t *)
CMC_COLLECTION_GENERATE(TREEMAP, tm4, tmap4, enum_t *, enum_t *)
CMC_COLLECTION_GENERATE(HASHSET, hs4, hset4, /* K */, enum_t *)
CMC_COLLECTION_GENERATE(HASHMAP, hm4, hmap4, enum_t *, enum_t *)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih4, iheap4, /* K */, enum_t *)
CMC_COLLECTION_GENERATE(MULTIMAP, mm4, mmap4, enum_t *, enum_t *)
CMC_COLLECTION_GENERATE(MULTISET, ms4, mset4, /* K */, enum_t *)

CMC_COLLECTION_GENERATE(LIST, l5, list5, /* K */, enum_t)
CMC_COLLECTION_GENERATE(STACK, s5, stack5, /* K */, enum_t)
CMC_COLLECTION_GENERATE(QUEUE, q5, queue5, /* K */, enum_t)
CMC_COLLECTION_GENERATE(DEQUE, d5, deque5, /* K */, enum_t)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll5, linked5, /* K */, enum_t)
CMC_COLLECTION_GENERATE(HEAP, h5, heap5, /* K */, enum_t)
CMC_COLLECTION_GENERATE(TREESET, ts5, tset5, /* K */, enum_t)
CMC_COLLECTION_GENERATE(TREEMAP, tm5, tmap5, enum_t, enum_t)
CMC_COLLECTION_GENERATE(HASHSET, hs5, hset5, /* K */, enum_t)
CMC_COLLECTION_GENERATE(HASHMAP, hm5, hmap5, enum_t, enum_t)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih5, iheap5, /* K */, enum_t)
CMC_COLLECTION_GENERATE(MULTIMAP, mm5, mmap5, enum_t, enum_t)
CMC_COLLECTION_GENERATE(MULTISET, ms5, mset5, /* K */, enum_t)

CMC_COLLECTION_GENERATE(LIST, l6, list6, /* K */, union_t *)
CMC_COLLECTION_GENERATE(STACK, s6, stack6, /* K */, union_t *)
CMC_COLLECTION_GENERATE(QUEUE, q6, queue6, /* K */, union_t *)
CMC_COLLECTION_GENERATE(DEQUE, d6, deque6, /* K */, union_t *)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll6, linked6, /* K */, union_t *)
CMC_COLLECTION_GENERATE(HEAP, h6, heap6, /* K */, union_t *)
CMC_COLLECTION_GENERATE(TREESET, ts6, tset6, /* K */, union_t *)
CMC_COLLECTION_GENERATE(TREEMAP, tm6, tmap6, union_t *, union_t *)
CMC_COLLECTION_GENERATE(HASHSET, hs6, hset6, /* K */, union_t *)
CMC_COLLECTION_GENERATE(HASHMAP, hm6, hmap6, union_t *, union_t *)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih6, iheap6, /* K */, union_t *)
CMC_COLLECTION_GENERATE(MULTIMAP, mm6, mmap6, union_t *, union_t *)
CMC_COLLECTION_GENERATE(MULTISET, ms6, mset6, /* K */, union_t *)

CMC_COLLECTION_GENERATE(LIST, l7, list7, /* K */, union_t)
CMC_COLLECTION_GENERATE(STACK, s7, stack7, /* K */, union_t)
CMC_COLLECTION_GENERATE(QUEUE, q7, queue7, /* K */, union_t)
CMC_COLLECTION_GENERATE(DEQUE, d7, deque7, /* K */, union_t)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll7, linked7, /* K */, union_t)
CMC_COLLECTION_GENERATE(HEAP, h7, heap7, /* K */, union_t)
CMC_COLLECTION_GENERATE(TREESET, ts7, tset7, /* K */, union_t)
CMC_COLLECTION_GENERATE(TREEMAP, tm7, tmap7, union_t, union_t)
CMC_COLLECTION_GENERATE(HASHSET, hs7, hset7, /* K */, union_t)
CMC_COLLECTION_GENERATE(HASHMAP, hm7, hmap7, union_t, union_t)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih7, iheap7, /* K */, union_t)
CMC_COLLECTION_GENERATE(MULTIMAP, mm7, mmap7, union_t, union_t)
CMC_COLLECTION_GENERATE(MULTISET, ms7, mset7, /* K */, union_t)

CMC_COLLECTION_GENERATE(LIST, l8, list8, /* K */, func_t)
CMC_COLLECTION_GENERATE(STACK, s8, stack8, /* K */, func_t)
CMC_COLLECTION_GENERATE(QUEUE, q8, queue8, /* K */, func_t)
CMC_COLLECTION_GENERATE(DEQUE, d8, deque8, /* K */, func_t)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll8, linked8, /* K */, func_t)
CMC_COLLECTION_GENERATE(HEAP, h8, heap8, /* K */, func_t)
CMC_COLLECTION_GENERATE(TREESET, ts8, tset8, /* K */, func_t)
CMC_COLLECTION_GENERATE(TREEMAP, tm8, tmap8, func_t, func_t)
CMC_COLLECTION_GENERATE(HASHSET, hs8, hset8, /* K */, func_t)
CMC_COLLECTION_GENERATE(HASHMAP, hm8, hmap8, func_t, func_t)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih8, iheap8, /* K */, func_t)
CMC_COLLECTION_GENERATE(MULTIMAP, mm8, mmap8, func_t, func_t)
CMC_COLLECTION_GENERATE(MULTISET, ms8, mset8, /* K */, func_t)

CMC_COLLECTION_GENERATE(LIST, l9, list9, /* K */, func_t)
CMC_COLLECTION_GENERATE(STACK, s9, stack9, /* K */, func_t)
CMC_COLLECTION_GENERATE(QUEUE, q9, queue9, /* K */, func_t)
CMC_COLLECTION_GENERATE(DEQUE, d9, deque9, /* K */, func_t)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll9, linked9, /* K */, func_t)
CMC_COLLECTION_GENERATE(HEAP, h9, heap9, /* K */, func_t)
CMC_COLLECTION_GENERATE(TREESET, ts9, tset9, /* K */, func_t)
CMC_COLLECTION_GENERATE(TREEMAP, tm9, tmap9, func_t, func_t)
CMC_COLLECTION_GENERATE(HASHSET, hs9, hset9, /* K */, func_t)
CMC_COLLECTION_GENERATE(HASHMAP, hm9, hmap9, func_t, func_t)

int main(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|            All Data Types Are Working            |\n");
    printf("+--------------------------------------------------+\n");

    return 0;
}
