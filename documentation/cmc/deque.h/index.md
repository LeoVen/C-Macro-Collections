# deque.h

A Deque, also known as a double-ended queue, is a linear data structure that is able to add and remove elements from both ends. It can also be thought of a double-ended stack since you can push and pop elements from two ends. The Deque can also be used as a Queue. The only elements accessible from a Deque ar the two elements at its ends (front element and back element).

## Deque Implementation

This implementation uses a circular buffer (ring buffer or cyclic buffer) in order to operate on O(1) for push and pop on either ends. The only case where it takes longer than O(1) is when the buffer is reallocated. If it was implemented as a regular array, adding or removing elements from the front would take O(N) due to the need to shift all elements in the Deque.

Two indices are kept track of. The `front` index and the `rear` index. These represent the both ends of the Deque. If an index reaches one end of the real buffer, they wrap around to the other end and an element is added there. This abstracts the real buffer as a circular buffer with the cost os constantly checking for boundaries and using the modulo operator.

## Deque Generation Macros

* `CMC_GENERATE_DEQUE(PFX, SNAME, V)` - Generate full definition.
* `CMC_GENERATE_DEQUE_HEADER(PFX, SNAME, V)` - Generate only the header portion
* `CMC_GENERATE_DEQUE_SOURCE(PFX, SNAME, V)` - Generate only the source portion

| Parameter | Description                   |
| --------- | ----------------------------- |
| `PFX`     | Functions namespace or prefix |
| `SNAME`   | Structure name                |
| `V`       | Value type                    |

## Deque Structures

| Declared structs          | Description                         |
| ------------------------- | ----------------------------------- |
| `struct SNAME`            | A double-ended queue data structure |
| `struct SNAME##_fval`     | A function table for the `V` type   |
| `struct SNAME##_iter`     | A double-ended queue iterator       |

## struct SNAME

| `struct SNAME`                    | Description                                                  |
| --------------------------------- | ------------------------------------------------------------ |
| `V *buffer`                       | Dynamic circular array of elements.                          |
| `size_t capacity`                 | Current circular array capacity.                             |
| `size_t count`                    | Current amount of elements.                                  |
| `size_t front`                    | Index representing the front of the Deque.                   |
| `size_t rear`                     | Index representing the back of the Deque.                    |
| `int flag`                        | Flag indicating errors or success.                           |
| `struct SNAME##_fval *f_val`      | Functions table for the Value type.                          |
| `struct cmc_alloc_node *alloc`    | Custom allocation functions.                                 |
| `struct cmc_callbacks *callbacks` | Callback functions.                                          |
| `it_start` <sup>\[1\]</sup>       | Returns an iterator to the start of the Deque.               |
| `it_end` <sup>\[2\]</sup>         | Returns an iterator to the end of the Deque.                 |

1. Full definition: `struct SNAME##_iter (*it_start)(struct SNAME *);`.
2. Full definition: `struct SNAME##_iter (*it_end)(struct SNAME *);`.

## struct SNAME##_fval

The Functions Table for `V`. Check out the Functions Table documentation [here](../../cor/functions_table/index.html).

## struct SNAME##_iter

| `struct SNAME##_iter`  | Description                                         |
| ---------------------- | --------------------------------------------------- |
| `struct SNAME *target` | Deque being iterated over.                          |
| `size_t cursor`        | Cursor's current position (index).                  |
| `size_t index`         | Relative index to all elements in the iteration.    |
| `bool start`           | If the iterator reached the start of the iteration. |
| `bool end`             | If the iterator reached the end of iteration.       |

## Callbacks

This list associates which functions calls which callbacks.

* `create`
    * `PFX##_push_front()`
    * `PFX##_push_back()`
* `read`
    * `PFX##_front()`
    * `PFX##_back()`
    * `PFX##_contains()`
* `update`
* `delete`
    * `PFX##_pop_front()`
    * `PFX##_pop_back()`
* `resize`
    * `PFX##_resize()`

## Functions Table

| CMP | CPY | STR | FREE | HASH | PRI |
| :-: | :-: | :-: | :--: | :--: | :-: |
| ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
