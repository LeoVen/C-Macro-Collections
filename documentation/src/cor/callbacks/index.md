# Callbacks

Every collection can have an optional callback node. In this node there are five functions:

| Callback | Description                                                             |
| -------- | ----------------------------------------------------------------------- |
| `create` | Is called when an element was successfully added to the collection      |
| `read`   | Is called when the collection was successfully queried about an element |
| `update` | Is called when an element in the collection was successfully updated    |
| `delete` | Is called when an element was successfully removed from the collection  |
| `resize` | Is called when the collection was full and successfully resized         |

Check the documentation for each collection to see which functions call which callbacks.

## cmc_callbacks

Every collection has a pointer to this `struct`. If it is `NULL` then the collection has no callbacks. Also, individual callbacks can be optional if set to `NULL`.

```c
struct cmc_callbacks
{
    void (*create)(void);
    void (*read)(void);
    void (*update)(void);
    void (*delete)(void);
    void (*resize)(void);
};
```

## Example

```c
#include "cmc/heap.h"
#include "utl/futils.h" // cmc_i32_cmp

// Generate a heap of integers
CMC_GENERATE_HEAP(i32h, i32heap, int)

void heap_on_create(void)
{
    printf("An element was added to the heap\n");
}

int main(void)
{
    // create() is set but the other callbacks are not
    struct cmc_callbacks my_callbacks = { .create = heap_on_create, NULL };

    // Create a max heap with the callbacks
    struct i32heap *heap = i32h_new_custom(
        100, cmc_max_heap, &(struct i32heap_fval){ .cmp = cmc_i32_cmp }, NULL,
        &my_callbacks);

    i32h_insert(heap, 10);
    i32h_insert(heap, 11);
    i32h_insert(heap, 12);

    i32h_free(heap);
}
```
