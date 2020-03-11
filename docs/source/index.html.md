THIS IS A SAFE COPY OF THE MARKDOWN FOR THE DOCUMENTATION LOCATED AT https://leoven.github.io/C-Macro-Collections/




---
title: CMC Documentation

toc_footers:
  - <a href='https://github.com/lord/slate'>Documentation Powered by Slate</a>

language_tabs:
    - C

includes:
    - bidimap
    - deque
    - hashmap
    - hashset
    - heap
    - intervalheap
    - linkedlist
    - list
    - multimap
    - multiset
    - queue
    - sortedlist
    - stack
    - treemap
    - treeset

search: true
---

# Introduction

[![github](https://img.shields.io/badge/GitHub-C%20Macro%20Collections-lightgrey.svg?logo=github)](https://github.com/LeoVen/C-Macro-Collections)

![License](https://img.shields.io/badge/License-MIT-blue.svg)
[![Build](https://travis-ci.org/LeoVen/C-Macro-Collections.svg?branch=master)](https://travis-ci.org/LeoVen/C-Macro-Collections)
[![codecov](https://codecov.io/gh/LeoVen/C-Macro-Collections/branch/master/graph/badge.svg)](https://codecov.io/gh/LeoVen/C-Macro-Collections)

The C Macro Collections Library is a compilation of macros that can be used to generate common data structures for any desired type. These data structures are [type safe](https://en.wikipedia.org/wiki/Type_safety).

The documentation is currently being written in hopes that in the future it can be used to help you get to know better this amazing C library.

## Why this library

C is a low level language and lacks all the data structures that we commonly use. Many high level languages already come with these collections so that we don't have to implement everything ourselves. A lot of third party libraries that implement these missing data structures for the C language usually make use of a void pointers and a lot of macros. __This is why the C Macro Collections Library was created.__ All you need to do is to write down one macro and which data type you wish to work with. The library currently provides many data structures, such as:

| | |
|-|-|
| Linear Collections | List, LinkedList, Deque, Stack, Queue, SortedList |
| Sets               | HashSet, TreeSet, MultiSet                        |
| Maps               | HashMap, TreeMap, MultiMap, BidiMap               |
| Heaps              | Heap, IntervalHeap                                |

All with the following features:

* __Type-safe__ - No `void *` pointers are used. A collection of type `int` will only accept integers;
* __Customization__ - Custom struct name and function namespace;
* __Bidirectional Iterators__ - Full support for iterators;
* __Nesting__ - Collections can be nested (List of Lists, HashSet of Stacks, etc).

## Library Structure

The `src` folder is subdivided in 5 other folders and one file:

> The `dev` folder is an exact copy of `cmc` but it is filled with logging information regarding the code execution and debugging.

> The `sac` library contains fixed-length collections with an internal C array. They are allocated on the stack so be careful when using these.

* __cmc__ - The main C Macro Collections Library
* __dev__ - The main C Macro Collections Library for development
* __sac__ - Statically  Allocated Collections
* __utl__ - Utility like ForEach macros, logging, etc
* __macro\_collections.h__ - Master header containing all collections and utilities

## How to install

> Include header files like this

```c
#include <cmc/list.h>
#include <dev/deque.h>
#include <utl/assert.h>
```
> Not like this

```c
#include <list.h>
#include <deque.h>  // dev or cmc?
#include <assert.h> // this will import from the standard library
```

No installation is required. All you have to do is include the appropriate header files, generate any collections that you desire and then compile your program. It is recommended that you include the headers relative to the `src` folder and include it with `-I path/to/library/src` as shown in the following example.

The library has no external dependencies other than the C standard library.

## Understanding the Library

Every macro within the library is prefixed by `CMC` or `cmc`. If you have intellisense you can easily check all the features of the library once you include the necessary headers.

When you generate code for a certain collection you will need to pass two very important parameters: `SNAME` and `PFX`.

`SNAME` is a short for struct name. This parameter will define your collection so that its type name becomes:

`struct SNAME`

No `typedef`s are used within the code that is generated in order to not pollute the global namespace. If you wish to `typedef` feel free to give your own naming conventions. __Every other `struct` that is generated as part of the implementation will be prefixed by `SNAME`__.

`PFX` is a short for prefix. __Every function generated will be within this namespace__. Just write whichever prefix you gave to your functions and everything will be in there. Functions that are part of the implementation and shouldn't be called directly are prefixed by `_impl_` (`li32_impl_` for example where `PFX` = `li32`). Every iterator function will be prefixed by `_iter_` (`li32_iter_` for example where `PFX` = `li32`).

# Features

The C Macro Collections Library comes with powerful collections. In this section you will better understand many features that are common to all data structures.

## Two-Way Iterators

Every collection comes with an interface of iterators where you can easily access the elements of a collection. These can move at any direction, can start at any 'end' of a collection and can move any amount of steps per iteration. Every collection generated comes with an iterator.

By design choice these iterators do not support modifications to the collection. If a collection is modified, all iterators that have been initialized will most likely be invalidated and may cause undefined behavior if used afterwards.

## Custom Allocation

All collections have an allocation node. This node can be modified so that every allocation and de-allocation can be done with custom functions. The allocation node has pointers to functions that correspond to:

* `calloc`
* `free`
* `malloc`
* `realloc`

## Callbacks

Every collection can have an optional callback node. In this node there are five functions:

| | |
|-|-|
| `on_create` | Is called when an element is about to be added to the collection      |
| `on_read`   | Is called when the collection is about to be queried about an element |
| `on_update` | Is called when an element in the collection is about to be updated    |
| `on_delete` | Is called when an element is about to be removed from the collection  |
| `on_resize` | Is called when the collection is full and is about to get resized     |

Check the documentation for each collection to see which functions call which callbacks.

## Functions Table

Functions table is a `struct` of function pointers containing 'methods' for a custom data type. Some methods are optional and others are needed in order to a collection to operate. They are:

### CMP

A comparator function is used in sorted collections or when an equality is being checked like when trying to find a certain element in a list. It is responsible for taking two arguments of the same data type and comparing them. The return value is an `int` with the following definitions:

* Return `1` if the first argument is greater than the second;
* Return `0` if the first argument equals the second;
* Return `-1` if the first argument is less than the second.

### CPY

A copy function is used when a collection is being copied. It can be used to make a deep copy of of your custom data type. It must take a single parameter and return a new copy of that same data type. If this function is absent (`NULL`) the data type will be copied by assignment (for pointers this is a shallow copy).

### STR

A string function is responsible for taking a `FILE` pointer and a custom data type and outputting the string representation of that data returning a `bool` indication success or failure. It is useful for debugging.

### FREE

The free function is called when a collection is cleared (all elements removed) or freed (all elements removed and freed from memory) and it is responsible for completely freeing all resources that are usually acquired by your data type.

### HASH

This function receives a custom data type as parameter and returns a `size_t` hash of that data. Used in hashtables.

### PRI

A priority function works much like the comparator function except that it compares the priority between two elements. It is used in collections whose structure is based on the priority of elements and not in their general comparison.

* Return `1` if the first argument has a greater priority than the second;
* Return `0` if the first argument has the same priority as second;
* Return `-1` if the first argument has a lower priority than the second.

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

| Color | Label |
| :---: | ----- |
| ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | Required for basic functionality. |
| ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | Required for specific functions. |
| ![#497edd](https://placehold.it/20/497edd/000000?text=+) | Required for non-core specific functions. |
| ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | Optional. |
| ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | Not Used. |

## Minimal Example

(TODO this example is currently outdated)

Lets create a list of type `int`, add some elements to it and get the sum of all the elements printed on the screen.

### Include headers

```c
#include <cmc/list.h>
```

First you need to include only the necessary headers. It is not recommended that you include everything with `macro_collections.h`, only in cases where you might be using almost all features from the library.

### Generating a list

```c
// CMC_GENERATE_LIST(PFX, SNAME, V)
CMC_GENERATE_LIST(il, int_list, int)
```

The List is also known as `vector`, `ArrayList` and dynamic array. In fact the latter is the best description for this collection. It is a dynamic array that can grow and shrink as needed. To generate it we can simply call the macro `CMC_GENERATE_LIST` after our includes. It has three parameters:

| Parameter name | Purpose |
| :------------: | ------- |
| __PFX__        | Functions prefix (namespace) |
| __SNAME__      | Struct name |
| __V__          | The list value type (can be any valid data type) |

<aside class="notice">
All collections can be generated by using the macro CMC_GENERATE_ and then the name of the include (in this case list.h) but in uppercase without the `.h`.
</aside>

### Initializing and adding elements

```c
// Allocate an int_list with an initial capacity of 100
int_list *list = il_new(100);

// Adding elements to the back of the list
for (int i = 0; i <= 1000; i++)
{
    if (!il_push_back(list, i))
    {
        // Something went wrong (check out documentation)
    }
}
```

To initialize any collection we call a function `_new`. Since the `PFX` parameter is set to `il` all functions related to our list of integer will be prefixed by `il`, so `_new` becomes `il_new`.

The list name is `int_list` as defined by `SNAME`. This type is a typedef like:

`typedef struct SNAME##_s SNAME;`

Which expands to:

`typedef struct int_list_s int_list`

To add elements to the back of a list we can use `il_push_back`. This function will first check if the list has enough space for a new element. If not, reallocate the internal buffer with doubled size and then add the new element.

### Iteration

> It is recommended that the iterator is allocated on the stack

```c
// Resulting sum will be stored here
int sum = 0;

// Declare our iterator (allocating it on the stack)
int_list_iter it;

// Loop for each element
for (il_iter_init(&it, list); !il_iter_end(&it); il_iter_next(&it))
{
    // Accessing the value
    int elem = il_iter_value(&it);

    sum += elem;
}

printf("%d\n", sum);
```

There are 6 (2 optional) steps to iterate over the elements of any collection:

| Step | Description | Function |
| :--: | ----------- | -------- |
| 1    | Allocate the iterator (optional) | `il_iter_new`   |
| 2    | Initialize the iterator given a target list | `il_iter_init`  |
| 3    | Access the iterator's elements (value, index, etc) | `il_iter_value` |
| 4    | Go to the next element | `il_iter_next`  |
| 5    | Check if we haven't reached the end of the list | `il_iter_end`   |
| 6    | Free the iterator if it was allocated on the heap | `il_iter_free`  |

Note that all iterator functions are prefixed by the user defined prefix and then by `iter` (`il + iter = il_iter`). Also, the steps __1__ and __6__ are optional so `il_iter_new` and `il_iter_free` are not used in the example.

By the end of the example we have the variable `sum` with the sum of all integers inside our list. We can then print it and this example is almost done.

### Freeing resources

```c
// void il_free(int_list *_list_, void(*deallocator)(int))
il_free(list, NULL);
```

The list is currently allocated in memory. To deallocate it we call `il_free`. This function takes a pointer to the allocated list and a pointer to a `deallocator` function that will be responsible for deallocating each element in the list. This last parameter is optional and we won't be using it since our data type `int` is not allocated in the heap. This optional parameter can be set to `NULL` to be ignored by the `il_free` function.

### Compile and run

> source.c

```c
#include <cmc/list.h>

CMC_GENERATE_LIST(il, int_list, int)

int main(void)
{
    int_list *list = il_new(100);

    for (int i = 0; i <= 1000; i++)
    {
        if (!il_push_back(list, i))
        {
            // Something went wrong (check out documentation)
        }
    }

    int sum = 0;

    int_list_iter it;

    for (il_iter_init(&it, list); !il_iter_end(&it); il_iter_next(&it))
    {
        int elem = il_iter_value(&it);
        sum += elem;
    }

    printf("%d\n", sum);

    il_free(list, NULL);
}
```

If the source file is `source.c`, all you have to do is to include the `src` folder to the include path and compile. The following example uses `gcc`:

`gcc source.c -I path/to/library/src`

### Conclusion

The C Macro Collections Library is prepared to accept any data type and is highly customizable. It is also very easy to integrate and no previous compilation is required. The library also comes with many utilities in the `utl` folder that are designed to work well with all collections.
