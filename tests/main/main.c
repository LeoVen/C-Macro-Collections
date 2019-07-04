#include "unt/deque.c"
#include "unt/hashmap.c"
#include "unt/hashset.c"
#include "unt/heap.c"
#include "unt/linkedlist.c"
#include "unt/list.c"

int main(void)
{
    deque_test();
    hashmap_test();
    hashset_test();
    heap_test();
    linkedlist_test();
    list_test();

    return 0;
}
