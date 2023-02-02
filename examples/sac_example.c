/* Shows an example of how to use CMC_SAC */

/**
 * CMC_SAC makes a data structure have a fixed size buffer to store data.
 */
#define CMC_SAC

#define V int
#define SNAME list
#define PFX l
#define SIZE 100 /* This sets the buffer size */
#define CMC_EXT_ITER
#define CMC_EXT_STR
#define CMC_EXT_INIT
#define CMC_EXT_SEQ
#include "cmc/list.h"

#include "cmc/utl/futils.h"

int main()
{
    struct list list = l_init(&(struct list_fval){ .str = cmc_i32_str });

    if (l_flag(&list) != CMC_FLAG_OK)
        return 1;

    for (int i = 1; i < 1000; i++)
    {
        if (!l_push_back(&list, i))
        {
            if (l_flag(&list) == CMC_FLAG_FULL)
            {
                printf("List full, stopping...\n");
                break;
            }
            else
            {
                enum cmc_flags flag = l_flag(&list);
                fprintf(stderr, "%s : push_back failed\n", cmc_flags_to_str[flag]);
            }
        }
    }

    for (struct list_iter it = l_iter_start(&list); !l_iter_at_end(&it); l_iter_next(&it))
    {
        int *val = l_iter_rvalue(&it);

        if (val != NULL)
            *val *= 2;
    }

    if (!l_print(&list, stdout, "[", ", ", "]\n"))
        return 2;

    // Free all of its resources
    l_release(list);
}
