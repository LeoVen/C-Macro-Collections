# Understanding the Library

Every macro within the library is prefixed by `CMC` or `cmc`. If you have [intellisense](https://code.visualstudio.com/docs/editor/intellisense) you can easily check all the features of the library once you include the necessary headers.

When you generate code for a certain collection you will need to pass four very important parameters: `SNAME`, `PFX`, `K` and `V`.

## SNAME

`SNAME` is short for struct name. This parameter will define your collection so that its type name becomes:

```c
struct SNAME;
```

No `typedef`s are used within the code that is generated in order to not pollute the global namespace. If you wish to `typedef` feel free to give your own naming conventions.
__Every other `struct` that is generated as part of the implementation will be prefixed by `SNAME`__. So iterators and nodes will become:

```c
struct SNAME##_iter;
struct SNAME##_node;
```

Note that the `##` is a [Token Pasting Operator](https://en.wikipedia.org/wiki/C_preprocessor#Token_concatenation).

## PFX

`PFX` is a short for prefix. __Every function generated will be within this namespace__. When using [Intellisense](https://code.visualstudio.com/docs/editor/intellisense), write whichever prefix you gave to your functions and everything will be in there.

Functions that are part of the implementation and shouldn't be called directly are prefixed by `PFX` and then by `_impl_`. Every iterator function will be prefixed by `PFX` and then by `_iter_`. For example:

```c
// A function that you shouldn't use directly
size_t PFX##_impl_quick_sort(struct SNAME *list);
```

```c
// A function that is associated with the iterator
struct SNAME##_iter *PFX##_iter_new(struct SNAME *target);
```

## K

`K`, short for Key, is the data type that is only used by associative collections to map a key to a value.

## V

`V`, short for Value, is the primary data type for most collections.

## Summarizing

With `SNAME` and `PFX` a common function definition looks like:

`size_t PFX##_count(struct SNAME *list);`

So if input `SNAME` as `i32list` and `PFX` as `i32l` you will have the following function definition:

`size_t i32l_count(struct i32list *list);`
