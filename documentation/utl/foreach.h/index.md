# foreach.h

For-Each macros. Since doing a for loop with iterators can be a lot to write, these macros solve that problem. There are two of them:

* `CMC_FOREACH` - Goes from the beginning of a Collection to the end
* `CMC_FOREAC_REV` - Goes from the end of a Collection to the beginning

## CMC_FOREACH

```c
#define CMC_FOREACH(PFX, SNAME, ITERNAME, TARGET)
```

* `PFX` - Functions prefix
* `SNAME` - Struct name
* `ITERNAME` - Iterator variable name
* `TARGET` - Target collection variable name

## CMC_FOREACH_REV

```c
#define CMC_FOREACH_REV(PFX, SNAME, ITERNAME, TARGET)
```

* `PFX` - Functions prefix
* `SNAME` - Struct name
* `ITERNAME` - Iterator variable name
* `TARGET` - Target collection variable name

## Example

```c
#include "cmc/list.h"
#include "utl/foreach.h"

CMC_GENERATE_LIST(i32l, i32list, int32_t)

int main(void)
{
    struct i32list *list = i32l_new(100, &(struct i32list_fval) { NULL });

    for (int i = 0; i < 100; i++)
        i32l_push_back(list, i);

    CMC_FOREACH(i32l, i32list, it, list)
    {
        int value = i32l_iter_value(&it);
        size_t index = i32l_iter_index(&it);

        if (index == 0)
            printf("[ %d, ", value);
        else if (index == i32l_count(list) - 1)
            printf("%d ]\n", value);
        else
            printf("%d, ", value);
    }

    i32l_free(list);
}
```