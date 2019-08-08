/**
 * hanoi.c
 *
 * Creation Date: 03/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/stack.h"
#include <stdio.h>
#include <stdlib.h>

STACK_GENERATE(s, stack, /* static */, int)

void print_hanoi(stack *t1, stack *t2, stack *t3);
const char *draw(int value);
void move_disc(stack *from, stack *to);
void move_stack(stack *t1, stack *t2, stack *t3, size_t size);

int main(int argc, char const *argv[])
{
    // 1- Hanoi towers showcase

    stack *tower1 = s_new(10);
    stack *tower2 = s_new(10);
    stack *tower3 = s_new(10);

    for (int i = 10; i > 0; i--)
        s_push(tower1, i);

    move_stack(tower1, tower3, tower2, s_count(tower1));

    s_free(tower1);
    s_free(tower2);
    s_free(tower3);

    return 0;
}

void print_hanoi(stack *t1, stack *t2, stack *t3)
{
    const int max = 10;

    static int step = 0;
    printf("Step %d\n", ++step);

    for (int i = max - 1; i >= 0; i--)
    {

        printf("   %10s|%-10s      %10s|%-10s      %10s|%-10s\n",
               draw(t1->buffer[i]), draw(t1->buffer[i]),
               draw(t2->buffer[i]), draw(t2->buffer[i]),
               draw(t3->buffer[i]), draw(t3->buffer[i]));
    }
    printf("  -----------------------    -----------------------    -----------------------\n\n");
}

const char *draw(int value)
{
    switch (value)
    {
    case 0:
        return "";
    case 1:
        return "#";
    case 2:
        return "##";
    case 3:
        return "###";
    case 4:
        return "####";
    case 5:
        return "#####";
    case 6:
        return "######";
    case 7:
        return "#######";
    case 8:
        return "########";
    case 9:
        return "#########";
    case 10:
        return "##########";
    default:
        return "?";
    }
}

void move_disc(stack *from, stack *to)
{
    s_push(to, s_top(from));
    s_pop(from);
}

void move_stack(stack *t1, stack *t2, stack *t3, size_t size)
{
    if (size == 0)
        return;

    move_stack(t1, t3, t2, size - 1);

    move_disc(t1, t2);

    print_hanoi(t1, t2, t3);

    move_stack(t3, t2, t1, size - 1);
}
