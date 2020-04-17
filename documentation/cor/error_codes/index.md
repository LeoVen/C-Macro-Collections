# Error Codes

Error codes are ways to signal errors or success. It is accessed through the `flag` component of a `struct` or through a function that is present in every collection:

```c
int PFX##_flag(struct SNAME *_collection_);
```

Some functions have a return type of `bool` indicating if it exited successfully or not. These error codes can be used for further error handling.

## cmc_flags

It is a global `struct` with default values for error codes.

```c
static struct
{
    int OK;
    int ALLOC;
    int EMPTY;
    int NOT_FOUND;
    int INVALID;
    int RANGE;
    int DUPLICATE;
    int ERROR;
} cmc_flags = { 0, 1, 2, 3, 4, 5, 6, 7 };
```

* `OK` - No errors. The operation exited successfully.
* `ALLOC` - Allocation failed.
* `EMPTY` - The collection is empty when it shouldn't.
* `NOT_FOUND` - Key or value not found.
* `INVALID` - Invalid argument or operation given the collection's state
* `RANGE` - Index out of range.
* `DUPLICATE` - Duplicate key or value.
* `ERROR` - Generic error. Usually caused by algorithm errors.

## cmc_flags_to_str

Maps the integer representation of error codes to their character representation.

```c
const char *cmc_flags_to_str[8] = { "OK",        "ALLOC",   "EMPTY",
                                    "NOT_FOUND", "INVALID", "RANGE",
                                    "DUPLICATE", "ERROR" };
```

## Example

```c
#include "cmc/treeset.h"
#include "utl/futils.h" // cmc_i32_cmp

// Generate a sorted set of integers
CMC_GENERATE_TREESET(tset, sortedset, int)

int main(void)
{
    struct sortedset *set =
        tset_new(&(struct sortedset_fval){ .cmp = cmc_i32_cmp });

    if (!tset_insert(set, 10))
        printf("Error! %s\n", cmc_flags_to_str[tset_flag(set)]);

    if (!tset_insert(set, 10))
        printf("Error! %s\n", cmc_flags_to_str[tset_flag(set)]);

    if (!tset_remove(set, 10))
        printf("Error! %s\n", cmc_flags_to_str[tset_flag(set)]);

    if (!tset_remove(set, 10))
        printf("Error! %s\n", cmc_flags_to_str[tset_flag(set)]);

    tset_free(set);
}
```
