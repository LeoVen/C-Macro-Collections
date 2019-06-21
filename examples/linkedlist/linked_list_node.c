/**
 * linked_list_node.c
 *
 * Creation Date: 26/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/linkedlist.h"
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>

LINKEDLIST_GENERATE(l, list, /* static */, int)

int main(void)
{
    list *l = l_new();

    for (int i = 0; i < 97; i++)
        l_push_back(l, i);

    // Add 99 after and 33 before every node where its data is divisible by 3
    for (list_node *node = l->head; node != NULL; node = node->next)
    {
        if (node->data % 3 == 0)
        {
            l_insert_before(l, node, 33);
            l_insert_after(l, node, 99);
            node = node->next; // skip 99
        }
    }

    size_t s = 0;
    for (list_node *node = l_head(l); node != NULL; node = l_next_node(node), s++)
    {
        if (node->prev == NULL)
            printf("[ %d, ", node->data);
        else if (node->next == NULL)
            printf("%d ]\n", node->data);
        else
            printf("%d, ", node->data);
    }

    printf("\n\n");

    s = 0;
    for (list_node *node = l_tail(l); node != NULL; node = l_prev_node(node), s++)
    {
        if (node->next == NULL)
            printf("[ %d, ", node->data);
        else if (node->prev == NULL)
            printf("%d ]\n", node->data);
        else
            printf("%d, ", node->data);
    }

    printf("\n\n");

    printf("List Head: %d\nList Tail: %d\nList Count: %" PRIuMAX "\n", l->head->data, l->tail->data, l->count);

    assert(s == l->count);

    l_free(l);

    return 0;
}
