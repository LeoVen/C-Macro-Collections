#include <stdio.h>
#include <assert.h>
#include "../src/linkedlist.h"

LINKEDLIST_GENERATE(l, list, , int)

int main(int argc, char const *argv[])
{
    list *l = l_new();

    for (int i = 0; i < 97; i++)
        l_push_back(l, i);

    // Add 99 after and 33 before every node where its data is divisible by 3
    for (list_node *node = l->head; node != NULL; node = node->next)
    {
        if (node->data % 3 == 0)
        {
            l_insert_prv(node, 33);
            l_insert_nxt(node, 99);
            node = node->next; // skip 99
        }
    }

    size_t s = 0;
    for (list_node *node = l_front_node(l); node != NULL; node = l_next_node(node), s++)
    {
        if (node->prev == NULL)
            printf("[ %d, ", node->data);
        else if (node->next == NULL)
            printf("%d ]\n", node->data);
        else
            printf("%d, ", node->data);
    }

    printf("List Head: %d\nList Tail: %d\nList Count: %d\n", l->head->data, l->tail->data, l->count);

    assert(s == l->count);

    l_free(l);

    return 0;
}
