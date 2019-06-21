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

COLLECTION_GENERATE(LIST, l0, list0, static, /* K */, struct_t *)
COLLECTION_GENERATE(STACK, s0, stack0, static, /* K */, struct_t *)
COLLECTION_GENERATE(QUEUE, q0, queue0, static, /* K */, struct_t *)
COLLECTION_GENERATE(DEQUE, d0, deque0, static, /* K */, struct_t *)
COLLECTION_GENERATE(LINKEDLIST, ll0, linked0, static, /* K */, struct_t *)
COLLECTION_GENERATE(HEAP, h0, heap0, static, /* K */, struct_t *)
COLLECTION_GENERATE(TREESET, ts0, tset0, static, /* K */, struct_t *)
COLLECTION_GENERATE(TREEMAP, tm0, tmap0, static, struct_t *, struct_t *)
COLLECTION_GENERATE(HASHSET, hs0, hset0, static, /* K */, struct_t *)
COLLECTION_GENERATE(HASHMAP, hm0, hmap0, static, struct_t *, struct_t *)

COLLECTION_GENERATE(LIST, l1, list1, static, /* K */, struct_t)
COLLECTION_GENERATE(STACK, s1, stack1, static, /* K */, struct_t)
COLLECTION_GENERATE(QUEUE, q1, queue1, static, /* K */, struct_t)
COLLECTION_GENERATE(DEQUE, d1, deque1, static, /* K */, struct_t)
COLLECTION_GENERATE(LINKEDLIST, ll1, linked1, static, /* K */, struct_t)
COLLECTION_GENERATE(HEAP, h1, heap1, static, /* K */, struct_t)
COLLECTION_GENERATE(TREESET, ts1, tset1, static, /* K */, struct_t)
COLLECTION_GENERATE(TREEMAP, tm1, tmap1, static, struct_t, struct_t)
COLLECTION_GENERATE(HASHSET, hs1, hset1, static, /* K */, struct_t)
COLLECTION_GENERATE(HASHMAP, hm1, hmap1, static, struct_t, struct_t)

COLLECTION_GENERATE(LIST, l2, list2, static, /* K */, int)
COLLECTION_GENERATE(STACK, s2, stack2, static, /* K */, int)
COLLECTION_GENERATE(QUEUE, q2, queue2, static, /* K */, int)
COLLECTION_GENERATE(DEQUE, d2, deque2, static, /* K */, int)
COLLECTION_GENERATE(LINKEDLIST, ll2, linked2, static, /* K */, int)
COLLECTION_GENERATE(HEAP, h2, heap2, static, /* K */, int)
COLLECTION_GENERATE(TREESET, ts2, tset2, static, /* K */, int)
COLLECTION_GENERATE(TREEMAP, tm2, tmap2, static, int, int)
COLLECTION_GENERATE(HASHSET, hs2, hset2, static, /* K */, int)
COLLECTION_GENERATE(HASHMAP, hm2, hmap2, static, int, int)

COLLECTION_GENERATE(LIST, l3, list3, static, /* K */, int *)
COLLECTION_GENERATE(STACK, s3, stack3, static, /* K */, int *)
COLLECTION_GENERATE(QUEUE, q3, queue3, static, /* K */, int *)
COLLECTION_GENERATE(DEQUE, d3, deque3, static, /* K */, int *)
COLLECTION_GENERATE(LINKEDLIST, ll3, linked3, static, /* K */, int *)
COLLECTION_GENERATE(HEAP, h3, heap3, static, /* K */, int *)
COLLECTION_GENERATE(TREESET, ts3, tset3, static, /* K */, int *)
COLLECTION_GENERATE(TREEMAP, tm3, tmap3, static, int *, int *)
COLLECTION_GENERATE(HASHSET, hs3, hset3, static, /* K */, int *)
COLLECTION_GENERATE(HASHMAP, hm3, hmap3, static, int *, int *)

COLLECTION_GENERATE(LIST, l4, list4, static, /* K */, enum_t *)
COLLECTION_GENERATE(STACK, s4, stack4, static, /* K */, enum_t *)
COLLECTION_GENERATE(QUEUE, q4, queue4, static, /* K */, enum_t *)
COLLECTION_GENERATE(DEQUE, d4, deque4, static, /* K */, enum_t *)
COLLECTION_GENERATE(LINKEDLIST, ll4, linked4, static, /* K */, enum_t *)
COLLECTION_GENERATE(HEAP, h4, heap4, static, /* K */, enum_t *)
COLLECTION_GENERATE(TREESET, ts4, tset4, static, /* K */, enum_t *)
COLLECTION_GENERATE(TREEMAP, tm4, tmap4, static, enum_t *, enum_t *)
COLLECTION_GENERATE(HASHSET, hs4, hset4, static, /* K */, enum_t *)
COLLECTION_GENERATE(HASHMAP, hm4, hmap4, static, enum_t *, enum_t *)

COLLECTION_GENERATE(LIST, l5, list5, static, /* K */, enum_t)
COLLECTION_GENERATE(STACK, s5, stack5, static, /* K */, enum_t)
COLLECTION_GENERATE(QUEUE, q5, queue5, static, /* K */, enum_t)
COLLECTION_GENERATE(DEQUE, d5, deque5, static, /* K */, enum_t)
COLLECTION_GENERATE(LINKEDLIST, ll5, linked5, static, /* K */, enum_t)
COLLECTION_GENERATE(HEAP, h5, heap5, static, /* K */, enum_t)
COLLECTION_GENERATE(TREESET, ts5, tset5, static, /* K */, enum_t)
COLLECTION_GENERATE(TREEMAP, tm5, tmap5, static, enum_t, enum_t)
COLLECTION_GENERATE(HASHSET, hs5, hset5, static, /* K */, enum_t)
COLLECTION_GENERATE(HASHMAP, hm5, hmap5, static, enum_t, enum_t)

COLLECTION_GENERATE(LIST, l6, list6, static, /* K */, union_t *)
COLLECTION_GENERATE(STACK, s6, stack6, static, /* K */, union_t *)
COLLECTION_GENERATE(QUEUE, q6, queue6, static, /* K */, union_t *)
COLLECTION_GENERATE(DEQUE, d6, deque6, static, /* K */, union_t *)
COLLECTION_GENERATE(LINKEDLIST, ll6, linked6, static, /* K */, union_t *)
COLLECTION_GENERATE(HEAP, h6, heap6, static, /* K */, union_t *)
COLLECTION_GENERATE(TREESET, ts6, tset6, static, /* K */, union_t *)
COLLECTION_GENERATE(TREEMAP, tm6, tmap6, static, union_t *, union_t *)
COLLECTION_GENERATE(HASHSET, hs6, hset6, static, /* K */, union_t *)
COLLECTION_GENERATE(HASHMAP, hm6, hmap6, static, union_t *, union_t *)

COLLECTION_GENERATE(LIST, l7, list7, static, /* K */, union_t)
COLLECTION_GENERATE(STACK, s7, stack7, static, /* K */, union_t)
COLLECTION_GENERATE(QUEUE, q7, queue7, static, /* K */, union_t)
COLLECTION_GENERATE(DEQUE, d7, deque7, static, /* K */, union_t)
COLLECTION_GENERATE(LINKEDLIST, ll7, linked7, static, /* K */, union_t)
COLLECTION_GENERATE(HEAP, h7, heap7, static, /* K */, union_t)
COLLECTION_GENERATE(TREESET, ts7, tset7, static, /* K */, union_t)
COLLECTION_GENERATE(TREEMAP, tm7, tmap7, static, union_t, union_t)
COLLECTION_GENERATE(HASHSET, hs7, hset7, static, /* K */, union_t)
COLLECTION_GENERATE(HASHMAP, hm7, hmap7, static, union_t, union_t)

COLLECTION_GENERATE(LIST, l8, list8, static, /* K */, func_t)
COLLECTION_GENERATE(STACK, s8, stack8, static, /* K */, func_t)
COLLECTION_GENERATE(QUEUE, q8, queue8, static, /* K */, func_t)
COLLECTION_GENERATE(DEQUE, d8, deque8, static, /* K */, func_t)
COLLECTION_GENERATE(LINKEDLIST, ll8, linked8, static, /* K */, func_t)
COLLECTION_GENERATE(HEAP, h8, heap8, static, /* K */, func_t)
COLLECTION_GENERATE(TREESET, ts8, tset8, static, /* K */, func_t)
COLLECTION_GENERATE(TREEMAP, tm8, tmap8, static, func_t, func_t)
COLLECTION_GENERATE(HASHSET, hs8, hset8, static, /* K */, func_t)
COLLECTION_GENERATE(HASHMAP, hm8, hmap8, static, func_t, func_t)

COLLECTION_GENERATE(LIST, l9, list9, static, /* K */, func_t)
COLLECTION_GENERATE(STACK, s9, stack9, static, /* K */, func_t)
COLLECTION_GENERATE(QUEUE, q9, queue9, static, /* K */, func_t)
COLLECTION_GENERATE(DEQUE, d9, deque9, static, /* K */, func_t)
COLLECTION_GENERATE(LINKEDLIST, ll9, linked9, static, /* K */, func_t)
COLLECTION_GENERATE(HEAP, h9, heap9, static, /* K */, func_t)
COLLECTION_GENERATE(TREESET, ts9, tset9, static, /* K */, func_t)
COLLECTION_GENERATE(TREEMAP, tm9, tmap9, static, func_t, func_t)
COLLECTION_GENERATE(HASHSET, hs9, hset9, static, /* K */, func_t)
COLLECTION_GENERATE(HASHMAP, hm9, hmap9, static, func_t, func_t)

int main(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|            All Data Types Are Working            |\n");
    printf("+--------------------------------------------------+\n");

    return 0;
}
