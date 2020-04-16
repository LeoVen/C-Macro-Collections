# Custom Allocation

All collections have an allocation node. This node can be modified so that every allocation and de-allocation can be done with custom functions. Every custom allocation function must follow the same prototype of the `stdlib.h` functions.

## cmc_alloc_node

```c
struct cmc_alloc_node
{
    void *(*malloc)(size_t);
    void *(*calloc)(size_t, size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
};
```

## cmc_alloc_node_default

The default allocation node. If a collections is not initialized with a custom allocation node, this will be the default one using the functions from the C standard library.

```c
static struct cmc_alloc_node cmc_alloc_node_default = { malloc, calloc,
                                                        realloc, free };
```

## Example

```c
#include "cmc/heap.h"
#include "utl/futils.h" // cmc_i32_cmp

// Total bytes allocated
size_t total = 0;

void *my_malloc(size_t size)
{
    void *result = malloc(size);

    if (!result)
        return result;

    total += size;
    return result;
}

void *my_calloc(size_t count, size_t size)
{
    void *result = calloc(count, size);

    if (!result)
        return result;

    total += count * size;
    return result;
}

void *my_realloc(void *block, size_t size)
{
    void *result = realloc(block, size);

    if (!result)
        return result;

    total += size;
    return result;
}

// Generate a heap of integers
CMC_GENERATE_HEAP(i32h, i32heap, int)

int main(void)
{
    // My custom allocation node
    struct cmc_alloc_node node = { .malloc = my_malloc,
                                   .calloc = my_calloc,
                                   .realloc = my_realloc,
                                   .free = free };

    // Create a max heap with the custom allocation node
    struct i32heap *heap = i32h_new_custom(
        100, cmc_max_heap, &(struct i32heap_fval){ .cmp = cmc_i32_cmp }, &node,
        NULL);

    for (int i = 0; i < 100000; i++)
        i32h_insert(heap, i);

    i32h_free(heap);

    printf("Total bytes allocated : %" PRIuMAX "\n", total);
}
```
