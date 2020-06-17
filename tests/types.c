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

typedef int (func_t)(int, double, char *);

C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d0,        deque0, ,           , struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,  HASHBIDIMAP, (hbm0,  hashbidimap0, , struct_t *, struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm0,      hashmap0, , struct_t *, struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm0, hashmultimap0, , struct_t *, struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms0, hashmultiset0, ,           , struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHSET, ( hs0,      hashset0, ,           , struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,         HEAP, (  h0,         heap0, ,           , struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC, INTERVALHEAP, ( ih0, intervalheap0, ,           , struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,   LINKEDLIST, ( ll0,   linkedlist0, ,           , struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,         LIST, (  l0,         list0, ,           , struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,        QUEUE, (  q0,        queue0, ,           , struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,   SORTEDLIST, ( sl0,   sortedlist0, ,           , struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,        STACK, (  s0,        stack0, ,           , struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      TREEMAP, ( tm0,      treemap0, , struct_t *, struct_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      TREESET, ( ts0,      treeset0, ,           , struct_t *))

C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d1,        deque1, ,         , struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,  HASHBIDIMAP, (hbm1,  hashbidimap1, , struct_t, struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm1,      hashmap1, , struct_t, struct_t))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm1, hashmultimap1, , struct_t, struct_t))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms1, hashmultiset1, ,         , struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHSET, ( hs1,      hashset1, ,         , struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,         HEAP, (  h1,         heap1, ,         , struct_t))
C_MACRO_COLLECTIONS_ALL(CMC, INTERVALHEAP, ( ih1, intervalheap1, ,         , struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,   LINKEDLIST, ( ll1,   linkedlist1, ,         , struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,         LIST, (  l1,         list1, ,         , struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,        QUEUE, (  q1,        queue1, ,         , struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,   SORTEDLIST, ( sl1,   sortedlist1, ,         , struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,        STACK, (  s1,        stack1, ,         , struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,      TREEMAP, ( tm1,      treemap1, , struct_t, struct_t))
C_MACRO_COLLECTIONS_ALL(CMC,      TREESET, ( ts1,      treeset1, ,         , struct_t))

C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d2,        deque2, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC,  HASHBIDIMAP, (hbm2,  hashbidimap2, , int, int))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm2,      hashmap2, , int, int))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm2, hashmultimap2, , int, int))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms2, hashmultiset2, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHSET, ( hs2,      hashset2, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC,         HEAP, (  h2,         heap2, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC, INTERVALHEAP, ( ih2, intervalheap2, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC,   LINKEDLIST, ( ll2,   linkedlist2, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC,         LIST, (  l2,         list2, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC,        QUEUE, (  q2,        queue2, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC,   SORTEDLIST, ( sl2,   sortedlist2, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC,        STACK, (  s2,        stack2, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC,      TREEMAP, ( tm2,      treemap2, , int, int))
C_MACRO_COLLECTIONS_ALL(CMC,      TREESET, ( ts2,      treeset2, ,    , int))

C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d3,        deque3, ,      , int *))
C_MACRO_COLLECTIONS_ALL(CMC,  HASHBIDIMAP, (hbm3,  hashbidimap3, , int *, int *))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm3,      hashmap3, , int *, int *))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm3, hashmultimap3, , int *, int *))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms3, hashmultiset3, ,      , int *))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHSET, ( hs3,      hashset3, ,      , int *))
C_MACRO_COLLECTIONS_ALL(CMC,         HEAP, (  h3,         heap3, ,      , int *))
C_MACRO_COLLECTIONS_ALL(CMC, INTERVALHEAP, ( ih3, intervalheap3, ,      , int *))
C_MACRO_COLLECTIONS_ALL(CMC,   LINKEDLIST, ( ll3,   linkedlist3, ,      , int *))
C_MACRO_COLLECTIONS_ALL(CMC,         LIST, (  l3,         list3, ,      , int *))
C_MACRO_COLLECTIONS_ALL(CMC,        QUEUE, (  q3,        queue3, ,      , int *))
C_MACRO_COLLECTIONS_ALL(CMC,   SORTEDLIST, ( sl3,   sortedlist3, ,      , int *))
C_MACRO_COLLECTIONS_ALL(CMC,        STACK, (  s3,        stack3, ,      , int *))
C_MACRO_COLLECTIONS_ALL(CMC,      TREEMAP, ( tm3,      treemap3, , int *, int *))
C_MACRO_COLLECTIONS_ALL(CMC,      TREESET, ( ts3,      treeset3, ,      , int *))

C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d4,        deque4, ,       , enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,  HASHBIDIMAP, (hbm4,  hashbidimap4, , enum_t, enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm4,      hashmap4, , enum_t, enum_t))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm4, hashmultimap4, , enum_t, enum_t))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms4, hashmultiset4, ,       , enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHSET, ( hs4,      hashset4, ,       , enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,         HEAP, (  h4,         heap4, ,       , enum_t))
C_MACRO_COLLECTIONS_ALL(CMC, INTERVALHEAP, ( ih4, intervalheap4, ,       , enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,   LINKEDLIST, ( ll4,   linkedlist4, ,       , enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,         LIST, (  l4,         list4, ,       , enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,        QUEUE, (  q4,        queue4, ,       , enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,   SORTEDLIST, ( sl4,   sortedlist4, ,       , enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,        STACK, (  s4,        stack4, ,       , enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,      TREEMAP, ( tm4,      treemap4, , enum_t, enum_t))
C_MACRO_COLLECTIONS_ALL(CMC,      TREESET, ( ts4,      treeset4, ,       , enum_t))

C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d5,        deque5, ,         , enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,  HASHBIDIMAP, (hbm5,  hashbidimap5, , enum_t *, enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm5,      hashmap5, , enum_t *, enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm5, hashmultimap5, , enum_t *, enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms5, hashmultiset5, ,         , enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHSET, ( hs5,      hashset5, ,         , enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,         HEAP, (  h5,         heap5, ,         , enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC, INTERVALHEAP, ( ih5, intervalheap5, ,         , enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,   LINKEDLIST, ( ll5,   linkedlist5, ,         , enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,         LIST, (  l5,         list5, ,         , enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,        QUEUE, (  q5,        queue5, ,         , enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,   SORTEDLIST, ( sl5,   sortedlist5, ,         , enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,        STACK, (  s5,        stack5, ,         , enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      TREEMAP, ( tm5,      treemap5, , enum_t *, enum_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      TREESET, ( ts5,      treeset5, ,         , enum_t *))

C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d6,        deque6, ,        , union_t))
C_MACRO_COLLECTIONS_ALL(CMC,  HASHBIDIMAP, (hbm6,  hashbidimap6, , union_t, union_t))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm6,      hashmap6, , union_t, union_t))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm6, hashmultimap6, , union_t, union_t))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms6, hashmultiset6, ,        , union_t))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHSET, ( hs6,      hashset6, ,        , union_t))
C_MACRO_COLLECTIONS_ALL(CMC,         HEAP, (  h6,         heap6, ,        , union_t))
C_MACRO_COLLECTIONS_ALL(CMC, INTERVALHEAP, ( ih6, intervalheap6, ,        , union_t))
C_MACRO_COLLECTIONS_ALL(CMC,   LINKEDLIST, ( ll6,   linkedlist6, ,        , union_t))
C_MACRO_COLLECTIONS_ALL(CMC,         LIST, (  l6,         list6, ,        , union_t))
C_MACRO_COLLECTIONS_ALL(CMC,        QUEUE, (  q6,        queue6, ,        , union_t))
C_MACRO_COLLECTIONS_ALL(CMC,   SORTEDLIST, ( sl6,   sortedlist6, ,        , union_t))
C_MACRO_COLLECTIONS_ALL(CMC,        STACK, (  s6,        stack6, ,        , union_t))
C_MACRO_COLLECTIONS_ALL(CMC,      TREEMAP, ( tm6,      treemap6, , union_t, union_t))
C_MACRO_COLLECTIONS_ALL(CMC,      TREESET, ( ts6,      treeset6, ,        , union_t))

C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d7,        deque7, ,          , union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,  HASHBIDIMAP, (hbm7,  hashbidimap7, , union_t *, union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm7,      hashmap7, , union_t *, union_t *))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm7, hashmultimap7, , union_t *, union_t *))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms7, hashmultiset7, ,          , union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHSET, ( hs7,      hashset7, ,          , union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,         HEAP, (  h7,         heap7, ,          , union_t *))
C_MACRO_COLLECTIONS_ALL(CMC, INTERVALHEAP, ( ih7, intervalheap7, ,          , union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,   LINKEDLIST, ( ll7,   linkedlist7, ,          , union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,         LIST, (  l7,         list7, ,          , union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,        QUEUE, (  q7,        queue7, ,          , union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,   SORTEDLIST, ( sl7,   sortedlist7, ,          , union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,        STACK, (  s7,        stack7, ,          , union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      TREEMAP, ( tm7,      treemap7, , union_t *, union_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      TREESET, ( ts7,      treeset7, ,          , union_t *))

// C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d8,        deque8, ,       , func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,  HASHBIDIMAP, (hbm8,  hashbidimap8, , func_t, func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm8,      hashmap8, , func_t, func_t))
// C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm8, hashmultimap8, , func_t, func_t))
// C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms8, hashmultiset8, ,       , func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,      HASHSET, ( hs8,      hashset8, ,       , func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,         HEAP, (  h8,         heap8, ,       , func_t))
// C_MACRO_COLLECTIONS_ALL(CMC, INTERVALHEAP, ( ih8, intervalheap8, ,       , func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,   LINKEDLIST, ( ll8,   linkedlist8, ,       , func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,         LIST, (  l8,         list8, ,       , func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,        QUEUE, (  q8,        queue8, ,       , func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,   SORTEDLIST, ( sl8,   sortedlist8, ,       , func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,        STACK, (  s8,        stack8, ,       , func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,      TREEMAP, ( tm8,      treemap8, , func_t, func_t))
// C_MACRO_COLLECTIONS_ALL(CMC,      TREESET, ( ts8,      treeset8, ,       , func_t))

C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d9,        deque9, ,         , func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,  HASHBIDIMAP, (hbm9,  hashbidimap9, , func_t *, func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm9,      hashmap9, , func_t *, func_t *))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm9, hashmultimap9, , func_t *, func_t *))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms9, hashmultiset9, ,         , func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHSET, ( hs9,      hashset9, ,         , func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,         HEAP, (  h9,         heap9, ,         , func_t *))
C_MACRO_COLLECTIONS_ALL(CMC, INTERVALHEAP, ( ih9, intervalheap9, ,         , func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,   LINKEDLIST, ( ll9,   linkedlist9, ,         , func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,         LIST, (  l9,         list9, ,         , func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,        QUEUE, (  q9,        queue9, ,         , func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,   SORTEDLIST, ( sl9,   sortedlist9, ,         , func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,        STACK, (  s9,        stack9, ,         , func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      TREEMAP, ( tm9,      treemap9, , func_t *, func_t *))
C_MACRO_COLLECTIONS_ALL(CMC,      TREESET, ( ts9,      treeset9, ,         , func_t *))

int main(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|            All Data Types Are Working            |\n");
    printf("+--------------------------------------------------+\n");

    return 0;
}
