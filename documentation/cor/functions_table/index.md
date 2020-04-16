# Functions Table

Functions Table is a `struct` with function pointers that are used to extract behavior from a custom data type. This exists because an `int` or a `char *` or `struct my_data_type` don't have default hash functions or ways to compare between them. This could be achieved with [operator overloading](https://en.wikipedia.org/wiki/Operator_overloading) but C doesn't have that. Hence, Functions Table was created to provide a single access point to extract these behaviors for __any__ data type.

Some functions are optional and others are needed in order for a collection to operate. Say you want a HashMap. There are two required functions: `hash` and `cmp` (comparator). Without them it is impossible to know how to hash your custom that type (it could be anything). This means you can also customize the collection by using different hash functions.

Many required functions are simply mundane. Like a comparator function between integers or between floats. The header [`utl/futils.h`](../../utl/futils.h/index.html) defines many of these functions for basic C data types.

Some collections generate two functions table. One for `K` and one for `V` (maps). Others simply generate one for `V`. A Functions Table `struct` is always going to be called:

* `struct SNAME##_fkey` for the `K` type
* `struct SNAME##_fval` for the `V` type

Their definition is defined as follows:

```c
                                     \
struct SNAME##_fkey                  \
{                                    \
    /* Comparator function */        \
    int (*cmp)(K, K);                \
                                     \
    /* Copy function */              \
    K (*cpy)(K);                     \
                                     \
    /* To string function */         \
    bool (*str)(FILE *, K);          \
                                     \
    /* Free from memory function */  \
    void (*free)(K);                 \
                                     \
    /* Hash function */              \
    size_t (*hash)(K);               \
                                     \
    /* Priority function */          \
    int (*pri)(K, K);                \
};                                   \
                                     \
```

### CMP

* `K` - `int (*cmp)(K, K)`
* `V` - `int (*cmp)(V, V)`

A comparator function is used in sorted collections or when an equality is being checked like when trying to find a certain element in a list. It is responsible for taking two arguments of the same data type and comparing them. The return value is an `int` with the following definitions:

* Return `1` if the first argument is greater than the second;
* Return `0` if the first argument equals the second;
* Return `-1` if the first argument is less than the second.

Note that certain collections require a total ordering of its elements (sorted collections) while others only require to know if one data equals another. Check out the documentation for each collection to know more.

### CPY

* `K` - `K (*cpy)(K)`
* `V` - `V (*cpy)(V)`

A copy function is used when a collection is being copied. It can be used to make a deep copy of of your custom data type. It must take a single parameter and return a new copy of that same data type. If this function is absent (`NULL`) the data type will be copied by assignment (for pointers this is a shallow copy).

Note that it doesn't makes sense to define a `cpy` function for data types that are not pointers. This function is useful for deep copies of a pointer in case you don't want two collections holding the same reference to the data.

### STR

* `K` - `bool (*str)(FILE *, K)`
* `V` - `bool (*str)(FILE *, V)`

A string function is responsible for taking a `FILE` pointer and a custom data type and outputting the string representation of that data returning a `bool` indication success or failure. It is useful for debugging and doesn't necessarily needs to print all the data that the specific type holds.

### FREE

* `K` - `void (*free)(K)`
* `V` - `void (*free)(V)`

The free function is called when a collection is cleared (all elements removed) or freed (all elements removed and the collection is freed from memory) and it is responsible for completely freeing all resources that are usually acquired by your data type.

This function only makes sense to be used when the data type has dynamically allocated memory. It is the only completely optional function.

### HASH

* `K` - `size_t (*hash)(K)`
* `V` - `size_t (*hash)(V)`

This function receives a custom data type as parameter and returns a `size_t` [hash](https://en.wikipedia.org/wiki/Hash_function) that represents that data. Used by collections with an underlying hash tables.

### PRI

* `K` - `int (*cmp)(K, K)`
* `V` - `int (*cmp)(V, V)`

A priority function works much like the comparator function except that it compares the priority between two elements. It is used in collections whose structure is based on the priority of elements and not in their general comparison (heaps).

* Return `1` if the first argument has a greater priority than the second;
* Return `0` if the first argument has the same priority as second;
* Return `-1` if the first argument has a lower priority than the second.

Why use both `cmp` and `pri`? Heaps have their internal structure based in the priority of elements. This priority is not necessarily how each element is compared to each other. Maybe their equality is defined differently for an equality of priorities. Maybe the rules for their priorities is different for when comparing an element against another.

The following table shows which functions are required, optional or never used for each Collection:

| Collection | CMP | CPY | STR | FREE | HASH | PRI |
| ---------- | :-: | :-: | :-: | :--: | :--: | :-: |
| BidiMap      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Deque        | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| HashMap      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| HashSet      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Heap         | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) |
| IntervalHeap | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) |
| List         | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| LinkedList   | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| MultiMap     | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| MultiSet     | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Queue        | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| SortedList   | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Stack        | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| TreeMap      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| TreeSet      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |

<br>

| Color | Label |
| :---: | ----- |
| ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | Required for basic functionality. |
| ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | Required for specific functions. |
| ![#497edd](https://placehold.it/20/497edd/000000?text=+) | Required for non-core specific functions. |
| ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | Optional. |
| ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | Not Used. |
