/* Shows an example of how to use CMC_DEV */

/**
 * CMC_DEV is a macro that enables DEV mode. It comes with tracing of functions
 * and other things that are printed in the terminal in order to debug the code.
 *
 * You can always compile this with -DCMC_DEV, but in this example we explicitly
 * define it in the file.
 *
 * CMC_DEV uses the cmc/utl/log.h utility. Through it, you can customize the log
 * levels and when to enable it.
 */
#define CMC_DEV
#define CMC_LOG_COLOR /* Optional */

#define V int
#define SNAME list
#define PFX l
#define CMC_EXT_ITER
#define CMC_EXT_STR
#include "cmc/list.h"

#include "cmc/utl/futils.h"

int main()
{
    struct list *list = l_new(32, &(struct list_fval){ .str = cmc_i32_str });

    if (!list || l_flag(list) != CMC_FLAG_OK)
    {
#ifdef CMC_DEV
        cmc_log_error("error: %s", cmc_flags_to_str[l_flag(list)]);
#endif
        return 1;
    }

#ifdef CMC_DEV
    /* We don't want to debug this, so disable it for now */
    cmc_log_config.enabled = false;
#endif

    for (int i = 0; i < 100; i++)
    {
        if (!l_push_back(list, i))
        {
            enum cmc_flags flag = l_flag(list);
            fprintf(stderr, "%s : push_back failed\n", cmc_flags_to_str[flag]);
        }
    }

    /* Turn it back on on the operations that we actually care to debug */
#ifdef CMC_DEV
    cmc_log_config.enabled = true;
#endif

    for (struct list_iter it = l_iter_start(list); !l_iter_at_end(&it); l_iter_next(&it))
    {
        int *val = l_iter_rvalue(&it);

        if (val != NULL)
            *val *= 2;
    }

    if (!l_print(list, stdout, "[", ", ", "]\n"))
        return 2;

    // Free all of its resources
    l_free(list);
}
