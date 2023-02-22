# bitset.h

A Bit Set is an array where each bit can be individually modified and queried by using bitwise operators such as `|`, `&`, `^`, `~`, `>>` and `<<` (`or`, `and`, `xor`, `not`, `right shift`, `left shift` respectively).

## BitSet Implementation

This BitSet implementation uses an array of type `cmc_bitset_word` which can be `typedef`ed to any unsigned type such as `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`, `size_t`, etc. The BitSet does not make use of `K` or `V`. Because of that, it also doesn't have Functions Tables.

The BitSet is initialized with a custom capacity but, if a bit index accessed is greater than the total capacity, the BitSet will resize. This means that the BitSet will try to guarantee that every bit index is accessible, as long as there is enough memory.

## BitSet Generation Macros

* `CMC_GENERATE_BITSET(PFX, SNAME)` - Generate full definition.
* `CMC_GENERATE_BITSET_HEADER(PFX, SNAME)` - Generate only the header portion
* `CMC_GENERATE_BITSET_SOURCE(PFX, SNAME)` - Generate only the source portion

| Parameter | Description                   |
| --------- | ----------------------------- |
| `PFX`     | Functions namespace or prefix |
| `SNAME`   | Structure name                |

## BitSet Structures

| Declared structs          | Description                         |
| ------------------------- | ----------------------------------- |
| `struct SNAME`            | A bit set data structure            |
| `struct SNAME##_iter`     | A bit set iterator                  |

## struct SNAME

| `struct SNAME`                    | Description                                |
| --------------------------------- | ------------------------------------------ |
| `cmc_bitset_word *buffer`         | Dynamic array of bits.                     |
| `size_t capacity`                 | Current array capacity         .           |
| `int flag`                        | Flag indicating errors or success.         |
| `struct cmc_alloc_node *alloc`    | Custom allocation functions.               |
| `struct cmc_callbacks *callbacks` | Callback functions.                        |

## struct SNAME##_iter

| `struct SNAME##_iter`  | Description                                         |
| ---------------------- | --------------------------------------------------- |
| `struct SNAME *target` | BitSet being iterated over.                         |
| `size_t cursor`        | Cursor's current position (index).                  |
| `bool start`           | If the iterator reached the start of the iteration. |
| `bool end`             | If the iterator reached the end of iteration.       |

## cmc_bitset_word

The `typedef`ed data type o the underlying bit set buffer.

```c
typedef uint32_t cmc_bitset_word;
```

This `typedef` can be changed to any unsigned integer data type and the bit set will still work properly.

## Callbacks

This list associates which functions calls which callbacks.

* `create`
    * `PFX##_set()`
    * `PFX##_set_range()`
* `read`
* `update`
* `delete`
    * `PFX##_clear()`
    * `PFX##_clear_range()`
* `resize`
    * `PFX##_resize()`

## Functions Table

| CMP | CPY | STR | FREE | HASH | PRI |
| :-: | :-: | :-: | :--: | :--: | :-: |
| ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
