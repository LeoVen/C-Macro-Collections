#include <stdio.h>

#define CMC_CALLBACKS

#define V int
#define PFX l
#define SNAME list
#include "cmc/list.h"

#define V int
#define PFX s
#define SNAME stack
#include "cmc/stack.h"

#define V int
#define PFX b
#define SNAME buffer
#define SIZE 100
#define CMC_SAC
#include "cmc/list.h"

void callback(const char fname[], void *self)
{
    if (fname[0] == 'l')
    {
        struct list *l = (struct list *)self;
        if (l_flag(l) != CMC_FLAG_OK)
        {
            printf("Error at %s: %s\n", fname, cmc_flags_to_str[l_flag(l)]);
        }
    }
    printf("Function called: %s for collection at %p\n", fname, self);
}

int main()
{
    struct list *l = l_new_custom(100, &(struct list_fval){}, NULL, callback);

    l_get(l, 0); /* Should error */

    l_push_back(l, 10);
    l_push_front(l, 5);

    printf("Got: %d\n", l_get(l, 1));

    l_pop_back(l);
    l_pop_back(l);
    l_pop_back(l); /* Error */

    struct stack *s = s_new_custom(100, &(struct stack_fval){}, NULL, callback);

    s_push(s, 10);
    s_push(s, 5);

    printf("Got: %d\n", s_top(s));

    struct buffer *b = b_new_custom(&(struct buffer_fval){}, NULL, callback);

    b_push_back(b, 10);
    b_push_front(b, 5);

    printf("Got: %d\n", b_get(b, 1));

    l_free(l);
    s_free(s);
    b_free(b);
}
