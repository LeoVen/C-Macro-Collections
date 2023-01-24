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

typedef union union_u
{
    enum enum_e a;
    struct struct_s b;
    double c;
    char *d;
} union_t;

typedef int(func_t)(int, double, char *);

typedef int array_int[10];

#define PFX b0
#define SNAME bitset0
#include "cmc/bitset.h"
#define PFX d0
#define SNAME deque0
#define V struct_t *
#include "cmc/deque.h"
#define PFX hbm0
#define SNAME hashbidimap0
#define K struct_t *
#define V struct_t *
#include "cmc/hashbidimap.h"
#define PFX hm0
#define SNAME hashmap0
#define K struct_t *
#define V struct_t *
#include "cmc/hashmap.h"
#define PFX hmm0
#define SNAME hashmultimap0
#define K struct_t *
#define V struct_t *
#include "cmc/hashmultimap.h"
#define PFX hms0
#define SNAME hashmultiset0
#define V struct_t *
#include "cmc/hashmultiset.h"
#define PFX hs0
#define SNAME hashset0
#define V struct_t *
#include "cmc/hashset.h"
#define PFX h0
#define SNAME heap0
#define V struct_t *
#include "cmc/heap.h"
#define PFX ih0
#define SNAME intervalheap0
#define V struct_t *
#include "cmc/intervalheap.h"
#define PFX ll0
#define SNAME linkedlist0
#define V struct_t *
#include "cmc/linkedlist.h"
#define PFX l0
#define SNAME list0
#define V struct_t *
#include "cmc/list.h"
#define PFX q0
#define SNAME queue0
#define V struct_t *
#include "cmc/queue.h"
#define PFX sl0
#define SNAME sortedlist0
#define V struct_t *
#include "cmc/sortedlist.h"
#define PFX s0
#define SNAME stack0
#define V struct_t *
#include "cmc/stack.h"
#define PFX tm0
#define SNAME treemap0
#define K struct_t *
#define V struct_t *
#include "cmc/treemap.h"
#define PFX ts0
#define SNAME treeset0
#define V struct_t *
#include "cmc/treeset.h"

#define PFX b1
#define SNAME bitset1
#include "cmc/bitset.h"
#define PFX d1
#define SNAME deque1
#define V struct_t
#include "cmc/deque.h"
#define PFX hbm1
#define SNAME hashbidimap1
#define K struct_t
#define V struct_t
#include "cmc/hashbidimap.h"
#define PFX hm1
#define SNAME hashmap1
#define K struct_t
#define V struct_t
#include "cmc/hashmap.h"
#define PFX hmm1
#define SNAME hashmultimap1
#define K struct_t
#define V struct_t
#include "cmc/hashmultimap.h"
#define PFX hms1
#define SNAME hashmultiset1
#define V struct_t
#include "cmc/hashmultiset.h"
#define PFX hs1
#define SNAME hashset1
#define V struct_t
#include "cmc/hashset.h"
#define PFX h1
#define SNAME heap1
#define V struct_t
#include "cmc/heap.h"
#define PFX ih1
#define SNAME intervalheap1
#define V struct_t
#include "cmc/intervalheap.h"
#define PFX ll1
#define SNAME linkedlist1
#define V struct_t
#include "cmc/linkedlist.h"
#define PFX l1
#define SNAME list1
#define V struct_t
#include "cmc/list.h"
#define PFX q1
#define SNAME queue1
#define V struct_t
#include "cmc/queue.h"
#define PFX sl1
#define SNAME sortedlist1
#define V struct_t
#include "cmc/sortedlist.h"
#define PFX s1
#define SNAME stack1
#define V struct_t
#include "cmc/stack.h"
#define PFX tm1
#define SNAME treemap1
#define K struct_t
#define V struct_t
#include "cmc/treemap.h"
#define PFX ts1
#define SNAME treeset1
#define V struct_t
#include "cmc/treeset.h"

#define PFX b2
#define SNAME bitset2
#include "cmc/bitset.h"
#define PFX d2
#define SNAME deque2
#define V int
#include "cmc/deque.h"
#define PFX hbm2
#define SNAME hashbidimap2
#define K int
#define V int
#include "cmc/hashbidimap.h"
#define PFX hm2
#define SNAME hashmap2
#define K int
#define V int
#include "cmc/hashmap.h"
#define PFX hmm2
#define SNAME hashmultimap2
#define K int
#define V int
#include "cmc/hashmultimap.h"
#define PFX hms2
#define SNAME hashmultiset2
#define V int
#include "cmc/hashmultiset.h"
#define PFX hs2
#define SNAME hashset2
#define V int
#include "cmc/hashset.h"
#define PFX h2
#define SNAME heap2
#define V int
#include "cmc/heap.h"
#define PFX ih2
#define SNAME intervalheap2
#define V int
#include "cmc/intervalheap.h"
#define PFX ll2
#define SNAME linkedlist2
#define V int
#include "cmc/linkedlist.h"
#define PFX l2
#define SNAME list2
#define V int
#include "cmc/list.h"
#define PFX q2
#define SNAME queue2
#define V int
#include "cmc/queue.h"
#define PFX sl2
#define SNAME sortedlist2
#define V int
#include "cmc/sortedlist.h"
#define PFX s2
#define SNAME stack2
#define V int
#include "cmc/stack.h"
#define PFX tm2
#define SNAME treemap2
#define K int
#define V int
#include "cmc/treemap.h"
#define PFX ts2
#define SNAME treeset2
#define V int
#include "cmc/treeset.h"

#define PFX b3
#define SNAME bitset3
#include "cmc/bitset.h"
#define PFX d3
#define SNAME deque3
#define V int *
#include "cmc/deque.h"
#define PFX hbm3
#define SNAME hashbidimap3
#define K int *
#define V int *
#include "cmc/hashbidimap.h"
#define PFX hm3
#define SNAME hashmap3
#define K int *
#define V int *
#include "cmc/hashmap.h"
#define PFX hmm3
#define SNAME hashmultimap3
#define K int *
#define V int *
#include "cmc/hashmultimap.h"
#define PFX hms3
#define SNAME hashmultiset3
#define V int *
#include "cmc/hashmultiset.h"
#define PFX hs3
#define SNAME hashset3
#define V int *
#include "cmc/hashset.h"
#define PFX h3
#define SNAME heap3
#define V int *
#include "cmc/heap.h"
#define PFX ih3
#define SNAME intervalheap3
#define V int *
#include "cmc/intervalheap.h"
#define PFX ll3
#define SNAME linkedlist3
#define V int *
#include "cmc/linkedlist.h"
#define PFX l3
#define SNAME list3
#define V int *
#include "cmc/list.h"
#define PFX q3
#define SNAME queue3
#define V int *
#include "cmc/queue.h"
#define PFX sl3
#define SNAME sortedlist3
#define V int *
#include "cmc/sortedlist.h"
#define PFX s3
#define SNAME stack3
#define V int *
#include "cmc/stack.h"
#define PFX tm3
#define SNAME treemap3
#define K int *
#define V int *
#include "cmc/treemap.h"
#define PFX ts3
#define SNAME treeset3
#define V int *
#include "cmc/treeset.h"

#define PFX b4
#define SNAME bitset4
#include "cmc/bitset.h"
#define PFX d4
#define SNAME deque4
#define V enum_t
#include "cmc/deque.h"
#define PFX hbm4
#define SNAME hashbidimap4
#define K enum_t
#define V enum_t
#include "cmc/hashbidimap.h"
#define PFX hm4
#define SNAME hashmap4
#define K enum_t
#define V enum_t
#include "cmc/hashmap.h"
#define PFX hmm4
#define SNAME hashmultimap4
#define K enum_t
#define V enum_t
#include "cmc/hashmultimap.h"
#define PFX hms4
#define SNAME hashmultiset4
#define V enum_t
#include "cmc/hashmultiset.h"
#define PFX hs4
#define SNAME hashset4
#define V enum_t
#include "cmc/hashset.h"
#define PFX h4
#define SNAME heap4
#define V enum_t
#include "cmc/heap.h"
#define PFX ih4
#define SNAME intervalheap4
#define V enum_t
#include "cmc/intervalheap.h"
#define PFX ll4
#define SNAME linkedlist4
#define V enum_t
#include "cmc/linkedlist.h"
#define PFX l4
#define SNAME list4
#define V enum_t
#include "cmc/list.h"
#define PFX q4
#define SNAME queue4
#define V enum_t
#include "cmc/queue.h"
#define PFX sl4
#define SNAME sortedlist4
#define V enum_t
#include "cmc/sortedlist.h"
#define PFX s4
#define SNAME stack4
#define V enum_t
#include "cmc/stack.h"
#define PFX tm4
#define SNAME treemap4
#define K enum_t
#define V enum_t
#include "cmc/treemap.h"
#define PFX ts4
#define SNAME treeset4
#define V enum_t
#include "cmc/treeset.h"

#define PFX b5
#define SNAME bitset5
#include "cmc/bitset.h"
#define PFX d5
#define SNAME deque5
#define V enum_t *
#include "cmc/deque.h"
#define PFX hbm5
#define SNAME hashbidimap5
#define K enum_t *
#define V enum_t *
#include "cmc/hashbidimap.h"
#define PFX hm5
#define SNAME hashmap5
#define K enum_t *
#define V enum_t *
#include "cmc/hashmap.h"
#define PFX hmm5
#define SNAME hashmultimap5
#define K enum_t *
#define V enum_t *
#include "cmc/hashmultimap.h"
#define PFX hms5
#define SNAME hashmultiset5
#define V enum_t *
#include "cmc/hashmultiset.h"
#define PFX hs5
#define SNAME hashset5
#define V enum_t *
#include "cmc/hashset.h"
#define PFX h5
#define SNAME heap5
#define V enum_t *
#include "cmc/heap.h"
#define PFX ih5
#define SNAME intervalheap5
#define V enum_t *
#include "cmc/intervalheap.h"
#define PFX ll5
#define SNAME linkedlist5
#define V enum_t *
#include "cmc/linkedlist.h"
#define PFX l5
#define SNAME list5
#define V enum_t *
#include "cmc/list.h"
#define PFX q5
#define SNAME queue5
#define V enum_t *
#include "cmc/queue.h"
#define PFX sl5
#define SNAME sortedlist5
#define V enum_t *
#include "cmc/sortedlist.h"
#define PFX s5
#define SNAME stack5
#define V enum_t *
#include "cmc/stack.h"
#define PFX tm5
#define SNAME treemap5
#define K enum_t *
#define V enum_t *
#include "cmc/treemap.h"
#define PFX ts5
#define SNAME treeset5
#define V enum_t *
#include "cmc/treeset.h"

#define PFX b6
#define SNAME bitset6
#include "cmc/bitset.h"
#define PFX d6
#define SNAME deque6
#define V union_t
#include "cmc/deque.h"
#define PFX hbm6
#define SNAME hashbidimap6
#define K union_t
#define V union_t
#include "cmc/hashbidimap.h"
#define PFX hm6
#define SNAME hashmap6
#define K union_t
#define V union_t
#include "cmc/hashmap.h"
#define PFX hmm6
#define SNAME hashmultimap6
#define K union_t
#define V union_t
#include "cmc/hashmultimap.h"
#define PFX hms6
#define SNAME hashmultiset6
#define V union_t
#include "cmc/hashmultiset.h"
#define PFX hs6
#define SNAME hashset6
#define V union_t
#include "cmc/hashset.h"
#define PFX h6
#define SNAME heap6
#define V union_t
#include "cmc/heap.h"
#define PFX ih6
#define SNAME intervalheap6
#define V union_t
#include "cmc/intervalheap.h"
#define PFX ll6
#define SNAME linkedlist6
#define V union_t
#include "cmc/linkedlist.h"
#define PFX l6
#define SNAME list6
#define V union_t
#include "cmc/list.h"
#define PFX q6
#define SNAME queue6
#define V union_t
#include "cmc/queue.h"
#define PFX sl6
#define SNAME sortedlist6
#define V union_t
#include "cmc/sortedlist.h"
#define PFX s6
#define SNAME stack6
#define V union_t
#include "cmc/stack.h"
#define PFX tm6
#define SNAME treemap6
#define K union_t
#define V union_t
#include "cmc/treemap.h"
#define PFX ts6
#define SNAME treeset6
#define V union_t
#include "cmc/treeset.h"

#define PFX b7
#define SNAME bitset7
#include "cmc/bitset.h"
#define PFX d7
#define SNAME deque7
#define V union_t *
#include "cmc/deque.h"
#define PFX hbm7
#define SNAME hashbidimap7
#define K union_t *
#define V union_t *
#include "cmc/hashbidimap.h"
#define PFX hm7
#define SNAME hashmap7
#define K union_t *
#define V union_t *
#include "cmc/hashmap.h"
#define PFX hmm7
#define SNAME hashmultimap7
#define K union_t *
#define V union_t *
#include "cmc/hashmultimap.h"
#define PFX hms7
#define SNAME hashmultiset7
#define V union_t *
#include "cmc/hashmultiset.h"
#define PFX hs7
#define SNAME hashset7
#define V union_t *
#include "cmc/hashset.h"
#define PFX h7
#define SNAME heap7
#define V union_t *
#include "cmc/heap.h"
#define PFX ih7
#define SNAME intervalheap7
#define V union_t *
#include "cmc/intervalheap.h"
#define PFX ll7
#define SNAME linkedlist7
#define V union_t *
#include "cmc/linkedlist.h"
#define PFX l7
#define SNAME list7
#define V union_t *
#include "cmc/list.h"
#define PFX q7
#define SNAME queue7
#define V union_t *
#include "cmc/queue.h"
#define PFX sl7
#define SNAME sortedlist7
#define V union_t *
#include "cmc/sortedlist.h"
#define PFX s7
#define SNAME stack7
#define V union_t *
#include "cmc/stack.h"
#define PFX tm7
#define SNAME treemap7
#define K union_t *
#define V union_t *
#include "cmc/treemap.h"
#define PFX ts7
#define SNAME treeset7
#define V union_t *
#include "cmc/treeset.h"

#define PFX b8
#define SNAME bitset8
#include "cmc/bitset.h"
#define PFX d8
#define SNAME deque8
#define V func_t *
#include "cmc/deque.h"
#define PFX hbm8
#define SNAME hashbidimap8
#define K func_t *
#define V func_t *
#include "cmc/hashbidimap.h"
#define PFX hm8
#define SNAME hashmap8
#define K func_t *
#define V func_t *
#include "cmc/hashmap.h"
#define PFX hmm8
#define SNAME hashmultimap8
#define K func_t *
#define V func_t *
#include "cmc/hashmultimap.h"
#define PFX hms8
#define SNAME hashmultiset8
#define V func_t *
#include "cmc/hashmultiset.h"
#define PFX hs8
#define SNAME hashset8
#define V func_t *
#include "cmc/hashset.h"
#define PFX h8
#define SNAME heap8
#define V func_t *
#include "cmc/heap.h"
#define PFX ih8
#define SNAME intervalheap8
#define V func_t *
#include "cmc/intervalheap.h"
#define PFX ll8
#define SNAME linkedlist8
#define V func_t *
#include "cmc/linkedlist.h"
#define PFX l8
#define SNAME list8
#define V func_t *
#include "cmc/list.h"
#define PFX q8
#define SNAME queue8
#define V func_t *
#include "cmc/queue.h"
#define PFX sl8
#define SNAME sortedlist8
#define V func_t *
#include "cmc/sortedlist.h"
#define PFX s8
#define SNAME stack8
#define V func_t *
#include "cmc/stack.h"
#define PFX tm8
#define SNAME treemap8
#define K func_t *
#define V func_t *
#include "cmc/treemap.h"
#define PFX ts8
#define SNAME treeset8
#define V func_t *
#include "cmc/treeset.h"

int main(void)
{
    printf("+--------------------------------------------------+\n");
    printf("|            All Data Types Are Working            |\n");
    printf("+--------------------------------------------------+\n");

    return 0;
}
