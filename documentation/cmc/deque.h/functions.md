# Index

__Deque__

* [`_new()`](#_new)
* [`_new_custom()`](#_new_custom)
* `_clear()`
* `_free()`
* `_customize()`
* `_push_front()`
* `_push_back()`
* `_pop_front()`
* `_front()`
* `_back()`
* `_contains()`
* `_empty()`
* `_full()`
* `_count()`
* `_capacity()`
* `_flag()`
* `_resize()`
* `_copy_of()`
* `_equals()`
* `_to_string()`
* `_print()`

__Deque Iterator__

WIP.

## [<span id="_new"> \_new() </span>](#index)

Allocates on the heap and returns a new Deque with an internal capacity of the specified value. The Deque will be empty, using the default allocator `cmc_alloc_node_default` and no callbacks will be set, that is, `callbacks` will be set to `NULL`. Its `flag` will be initially `cmc_flags.OK`.

__Declaration__

`struct SNAME *PFX##_new(size_t capacity, struct SNAME##_fval *f_val);`

__Parameters__

| Parameter                    | Required | Description                          |
| ---------------------------- | -------- | ------------------------------------ |
| `size_t capacity`            | Yes      | The initial Deque capacity           |
| `struct SNAME##_fval *f_val` | Yes      | A Functions Table for `V`            |

__Returns__

| Returns          | When                                                             |
| ---------------- | ---------------------------------------------------------------- |
| `struct SNAME *` | If operation was successful                                      |
| `NULL`           | If allocation failed, if capacity is `0` or if `f_val` is `NULL` |

## [<span id="_new_custom"> \_new\_custom() </span>](#index)

Like [\_new()](#_new) but allows for custom allocation node and callbacks. The allocation node is optional. If present, the function will immediately use it to allocate the new Deque. If `NULL`, `cmc_alloc_node_default` will instead be used.

__Declaration__

```
struct SNAME *PFX##_new_custom(size_t capacity, struct SNAME##_fval *f_val, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
```

__Parameters__

| Parameter                         | Required | Description                          |
| --------------------------------- | -------- | ------------------------------------ |
| `size_t capacity`                 | Yes      | The initial Deque capacity           |
| `struct SNAME##_fval *f_val`      | Yes      | A Functions Table for `V`            |
| `struct cmc_alloc_node *alloc`    | No       | A custom allocation node             |
| `struct cmc_callbacks *callbacks` | No       | A custom callbacks struct            |

__Returns__

| Returns          | When                                                             |
| ---------------- | ---------------------------------------------------------------- |
| `struct SNAME *` | If operation was successful                                      |
| `NULL`           | If allocation failed, if capacity is `0` or if `f_val` is `NULL` |


