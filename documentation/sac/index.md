# SAC

The Statically Allocated Collections. These collections have a fixed size. Its size is one of the parameters during macro expansion, producing a C array.

## CMC vs. SAC

The `cmc` collections hold their data either with nodes or a buffer that can be reallocated. The `sac` collections only have an array with fixed size and don't perform any allocation on the heap.

```c
// A CMC List
struct List
{
    T *buffer;
    /* Other fields */
};

// A SAC List
struct List
{
    T buffer[SIZE];
    /* Other fields */
};
```
