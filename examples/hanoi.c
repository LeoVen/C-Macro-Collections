#include "macro_collections.h"

#define MYSTACK (stk, stack, , , int)

C_MACRO_COLLECTIONS(CMC, STACK, CORE, STRUCT, MYSTACK)
C_MACRO_COLLECTIONS(CMC, STACK, CORE, HEADER, MYSTACK)
C_MACRO_COLLECTIONS(CMC, STACK, CORE, SOURCE, MYSTACK)

struct
{
    struct stack *from;
    struct stack *aux;
    struct stack *to;
} print_aux = { NULL };

void print_stacks()
{
    for (int i = 9; i >= 0; i--)
    {
        int ia = print_aux.from->buffer[i];
        int ib = print_aux.aux->buffer[i];
        int ic = print_aux.to->buffer[i];

        printf("| %c | %c | %c |\n", ia == 0 ? 32 : ia + 48, ib == 0 ? 32 : ib + 48, ic == 0 ? 32 : ic + 48);
    }

    printf("+---+---+---+\n\n");
}

void move(struct stack *from, struct stack *to)
{
    stk_push(to, stk_top(from));

    stk_pop(from);
}

void hanoi(int n, struct stack *from, struct stack *aux, struct stack *to)
{
    if (n == 0)
        return;

    hanoi(n - 1, from, to, aux);

    move(from, to);

    print_stacks();

    hanoi(n - 1, aux, from, to);
}

int main(int argc, char const *argv[])
{
    struct stack *from = stk_new(10, &(struct stack_fval){ NULL });
    struct stack *to = stk_new(10, &(struct stack_fval){ NULL });
    struct stack *aux = stk_new(10, &(struct stack_fval){ NULL });

    for (int i = 4; i >= 1; i--)
        stk_push(from, i);

    print_aux.from = from;
    print_aux.aux = aux;
    print_aux.to = to;

    print_stacks();

    hanoi(stk_count(from), from, aux, to);

    stk_free(from);
    stk_free(to);
    stk_free(aux);

    return 0;
}
