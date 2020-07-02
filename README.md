# C Macro Collections

<p align="center">
    <img src="https://leoven.github.io/C-Macro-Collections/img/logo.png" alt="C Macro Collections Logo" width="300"/>
</p>

<p align="center">Easy to use, header only, macro generated, generic and type-safe Data Structures in C.</p>

<p align="center">
    <a href="https://github.com/LeoVen/C-Macro-Collections"><img src="https://img.shields.io/badge/GitHub-C%20Macro%20Collections-2195F3.svg?logo=github" alt="LinkToRepo"/></a>
    <a href="https://github.com/LeoVen/C-Macro-Collections/stargazers"><img src="https://img.shields.io/github/stars/LeoVen/C-Macro-Collections?style=flat&color=ffa000" alt="Stars"/></a>
    <a href="https://leoven.github.io/C-Macro-Collections/"><img style="color: #ffffff;" src="https://img.shields.io/badge/Read%20the%20Docs-D13636.svg?logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHN0eWxlPSIiPjxyZWN0IGlkPSJiYWNrZ3JvdW5kcmVjdCIgd2lkdGg9IjEwMCUiIGhlaWdodD0iMTAwJSIgeD0iMCIgeT0iMCIgZmlsbD0ibm9uZSIgc3Ryb2tlPSJub25lIi8%2BPHRpdGxlPmljbi9kb2MtdGV4dDwvdGl0bGU%2BPGcgY2xhc3M9ImN1cnJlbnRMYXllciIgc3R5bGU9IiI%2BPHRpdGxlPkxheWVyIDE8L3RpdGxlPjxwYXRoIGQ9Ik01IDFoNC4yNDRhMiAyIDAgMCAxIDEuNDM0LjYwNmwyLjc1NiAyLjgzNEEyIDIgMCAwIDEgMTQgNS44MzVWMTJhMyAzIDAgMCAxLTMgM0g1YTMgMyAwIDAgMS0zLTNWNGEzIDMgMCAwIDEgMy0zem0wIDJhMSAxIDAgMCAwLTEgMXY4YTEgMSAwIDAgMCAxIDFoNmExIDEgMCAwIDAgMS0xVjUuODM1TDkuMjQ0IDNINXptMS41IDdoM2EuNS41IDAgMSAxIDAgMWgtM2EuNS41IDAgMSAxIDAtMXptMC0yaDJhLjUuNSAwIDAgMSAwIDFoLTJhLjUuNSAwIDAgMSAwLTF6TTggMmwzIDEuOTk1TDEzIDdIOWExIDEgMCAwIDEtMS0xVjJ6IiBpZD0iYSIgY2xhc3M9IiIgZmlsbD0iI2ZmZmZmZiIgZmlsbC1vcGFjaXR5PSIxIi8%2BPC9nPjwvc3ZnPg%3D%3D" alt="LinkToDocs"/></a>
</p>

<p align="center">
    <a href="https://github.com/LeoVen/C-Macro-Collections/blob/master/LICENSE"><img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="License"/></a>
    <a href="https://github.com/LeoVen/C-Macro-Collections/releases"><img src="https://img.shields.io/badge/Version-v0.24.0-orange.svg" alt="Version"/></a>
    <a href="https://travis-ci.org/LeoVen/C-Macro-Collections"><img src="https://travis-ci.org/LeoVen/C-Macro-Collections.svg?branch=master" alt="travis-ci"/></a>
    <a href="https://codecov.io/gh/LeoVen/C-Macro-Collections"><img src="https://codecov.io/gh/LeoVen/C-Macro-Collections/branch/master/graph/badge.svg" alt="codecov"/></a>
    <a href="https://github.com/LeoVen/C-Macro-Collections/actions"><img src="https://github.com/LeoVen/C-Macro-Collections/workflows/Test%20Suit/badge.svg?branch=master" alt="test_suit"/></a>
</p>

## Table of Contents

* [Installation](#installation)
* [Contributing](#contributing)
* [Usage](#usage)
* [Features](#features)
* [Project Structure](#project-structure)
* [Available Collections](#available-collections)
* [Other Features](#other-features)
* [Design Decisions](#design-decisions)
* [What to use](#what-to-use)
* [Code Review](#code-review)

## Installation

No installation is required. The entire library is made of header files and can be directly included into your project.

## Contributing

There is a lot to be done. You can check the `TODO` file in the root of the repository or the issues in the github page. Also, tests and documentation are in constant development. Also check out the `STATUS` file for the status on the development of the library.

## Usage

The header `macro_collections.h` includes every feature from the library and comes with higher level APIs that help you generate collections from specific sub-libraries with specific extensions.

Below is a minimal example that makes use of some core functionalities. And a lot of explanation too!

```c
// This is the master header. This will include every feature of the library.
// This comes with many helper macros (high level API) and utility functions.
#include "macro_collections.h"

// A PARAM is a standard way to pass required parameters to the lower level API.
// It is a tuple of form (PFX, SNAME, SIZE, K, V). In this case, not all of them
// are used, so we can leave them empty. We are creating a list of value int.
#define MY_LIST_PARAMS (intl, int_list, , , int)

// High level API. Generate a LIST from the CMC library with the STR extension
// using our previously defined PARAMs. Every collections has a CORE part that
// needs to be generated first. Then, we can add other parts after. This macro
// does all of that for us. The STR part will provide us with a function that
// will be used later.
C_MACRO_COLLECTIONS_GEN(CMC, LIST, MY_LIST_PARAMS, (STR))

int main(void)
{
    // Our list type is defined by SNAME and all functions are prefixed by PFX
    // (PFX can also be thought as the function's namespace). Also, nodes,
    // entries, iterators and other structures are prefixed by SNAME. So PFX is
    // for functions and SNAME is for structs, where the main one is simply
    // 'struct SNAME'.
    // To initialize a list we need to pass in an initial capacity and something
    // that is called a function table for the V type.
    // This function table is a struct with methods that will extract some
    // (sometimes) necessary behaviour from your custom data type. Things like
    // hash, comparison and printing. A list doesn't require any of these
    // functions. That is, the CORE module doesn't use any of them.
    // But since we are using the STR module, we will need to define the 'str'
    // function. luckily, for the 'int' type, the library already provides such
    // function (cmc_i32_str), provided by the /utl/futils.h header file.
    struct int_list *list = intl_new(32, &(struct int_list_fval){ .str = cmc_i32_str, NULL });

    // Check if the list was successfully initialized. It could fail if the
    // initial capacity is too big or if 'struct int_list_fval *' is NULL,
    // because every data structure must have a valid function table.
    if (!list)
        return 1;

    // Add some items to the list. The CMC data structures are all dynamically
    // sized. So there can be as many items as you want as long as you have
    // enough memory.
    for (int i = 0; i < 100; i++)
    {
        // Try to add an element to the list. If it fails you can check what
        // caused it by getting its flag.
        if (!intl_push_back(list, i))
        {
            enum cmc_flags flag = intl_flag(list);
            // Use cmc_flags_to_str to map the enum error to a string.
            fprintf(stderr, "%s : push_back failed\n", cmc_flags_to_str[flag]);
        }
    }

    // Now we will use the STR module, the _print() function. This is where the
    // '.str' from the function table comes into play. If we haven't defined it,
    // the program would've crashed. We also need to define to which file we
    // will be printing the list's content. In this case, the terminal. Also,
    // some strings need to be defined. They will be printed: before all elements,
    // between each one, and after all elements. This is very usefull and can
    // print a data structure very nicely.
    intl_print(list, stdout, "[ ", ", ", " ]\n");

    // You should see a nicely printed list in your terminal.

    // Free all of its resources
    intl_free(list);
}
```

Now all you have to do is to compile the source code with `-I /path/to/library/src`. There is no required installation or pre-compilation.

## Features

The C Macro Collections library is organized into many other sub-libraries. The following table is a quick overview.

| Library | Name                             | Description                                                                                 |
| :-----: | -------------------------------- | ------------------------------------------------------------------------------------------- |
|   CMC   | Macro Collections                | The main library with dynamically-sized collections                                         |
|   COR   | Core                             | Core functionalities used by more than one collection, usually from different sub-libraries |
|   EXT   | Extensions                       | Extension to collections from CMC, SAC and TSC libraries                                    |
|   INT   | Integrations                     | Macros that facilitate the creation of code that involves more than one type of collection  |
|   SAC   | Statically Allocated Collections | Collections with a fixed sized array that don't use any heap allocation                     |
|   TSC   | Thread-Safe Collections          | Collections that allow multiple operations from multiple threads                            |
|   UTL   | Utilities                        | General utilities                                                                           |

Every macro that generates code for a certain collection can be found with the following template. Some exceptions exist as certain collections don't have or can't have some features. One big example is the `UTL` library, which does not follow this pattern.

```
macro_name := CMC_[ lib ]_[ collection ]_[ part ]_[ file ]

lib := CMC | COR | DEV | EXT | INT | SAC | TSC
collection := BITSET | DEQUE | HASHBIDIMAP | ... | TREEMULTISET | TREESET
part := CORE | ITER | INIT | ... | SETF | NODE
file := HEADER | SOURCE
```

__Some `collection`s might not be present in a certain `lib`. Check the documentation.__

Every macro is suffixed by `CMC` and each section is separated by an underscore (`_`). The first section is the library (`lib`). The second is the collection name in all uppercase. Then the `part` (or which module) that you wish to generate. And last, if it is code that should belong to a header file or code that should belong to a source file.

## Project Structure

* __benchmarks__ - Where all benchmarks are hosted
* __docs__ - A folder hosting the generated documentation by [mdBook](https://github.com/rust-lang/mdBook)
* __documentation__ - The markdowns used by [mdBook](https://github.com/rust-lang/mdBook) to generate the website
* __examples__ - Examples using the C Macro Collections Library
* __src__ - All headers part of the C Macro Collections Library
    * __cmc__ - The main C Macro Collections Library
    * __cor__ - Core includes in the C Macro Collections Library
    * __ext__ - Extensions to the CMC, SAC and TSC collections
    * __int__ - Integrations between Collections
    * __sac__ - Statically Allocated Collections
    * __tsc__ - Thread Safe Collections
    * __utl__ - Utilities like ForEach macros, logging, etc
    * __macro\_collections.h__ - Master header containing all collections and utilities
* __tests__ - Where all tests are hosted

## Available Collections

Check out the STATUS file at the root of the project for more information.

__The following table is an overview of all the currently available or upcoming data structures__:

|    Collection <img width=250/>     | Abstract Data Type <img width=250/> | Data Structure <img width=250/> |                                                                    Details                                                                     |
| :--------------------------------: | :---------------------------------: | :-----------------------------: | :--------------------------------------------------------------------------------------------------------------------------------------------: |
|    BitSet       <br> _bitset.h_    |                 Set                 |          Dynamic Array          |                          A set of bits that can be individually modified and queried, each identified by a bit index                           |
|    Deque        <br> _deque.h_     |         Double-Ended Queue          |     Dynamic Circular Array      |                               A circular array that allows `push` and `pop` on both ends (only) at constant time                               |
| HashBidiMap  <br> _hashbidimap.h_  |          Bidirectional Map          |         Two Hashtables          |                          A bijection between two sets of unique keys and unique values `K <-> V` using two hashtables                          |
|   HashMap      <br> _hashmap.h_    |                 Map                 |         Flat Hashtable          | A unique set of keys associated with a value `K -> V` with constant time look up using a hashtable with open addressing and robin hood hashing |
| HashMultiMap <br> _hashmultimap.h_ |              Multimap               |            Hashtable            |                           A mapping of multiple keys with one node per key using a hashtable with separate chaining                            |
| HashMultiSet <br> _hashmultiset.h_ |              Multiset               |         Flat Hashtable          |                    A mapping of a value and its multiplicity using a hashtable with open addressing and robin hood hashing                     |
|   HashSet      <br> _hashset.h_    |                 Set                 |         Flat Hashtable          |                A unique set of values with constant time look up  using a hashtable with open addressing and robin hood hashing                |
|     Heap         <br> _heap.h_     |           Priority Queue            |          Dynamic Array          |                                         A binary heap as a dynamic array as an implicit data structure                                         |
| IntervalHeap <br> _intervalheap.h_ |     Double-Ended Priority Queue     |      Custom Dynamic Array       |                           A dynamic array of nodes, each hosting one value from the MinHeap and one from the MaxHeap                           |
|  LinkedList   <br> _linkedlist.h_  |                List                 |       Doubly-Linked List        |                                                          A default doubly-linked list                                                          |
|     List         <br> _list.h_     |                List                 |          Dynamic Array          |                                          A dynamic array with `push` and `pop` anywhere on the array                                           |
|    Queue        <br> _queue.h_     |                FIFO                 |     Dynamic Circular Array      |                      A queue using a circular array with `enqueue` at the `back` index and `dequeue` at the `front` index                      |
|     SkipList       <br> _WIP_      |             Sorted List             |            Skip List            |                              A sorted Linked List with average O(log n) search, insertion and deletion complexity                              |
|  SortedList   <br> _sortedlist.h_  |             Sorted List             |      Sorted Dynamic Array       |                                        A lazily sorted dynamic array that is sorted only when necessary                                        |
|    Stack        <br> _stack.h_     |                FILO                 |          Dynamic Array          |                                            A stack with push and pop at the end of a dynamic array                                             |
|      TreeBidiMap  <br> _WIP_       |      Sorted Bidirectional Map       |          Two AVL Trees          |                       A sorted bijection between two sets of unique keys and unique values `K <-> V` using two AVL trees                       |
|   TreeMap      <br> _treemap.h_    |             Sorted Map              |            AVL Tree             |               A unique set of keys associated with a value `K -> V` using an AVL tree with `log(n)` look up and sorted iteration               |
|      TreeMultiMap <br> _WIP_       |           Sorted Multimap           |            AVL Tree             |                           A sorted mapping of multiple keys with one node per key using an AVL Tree of linked-lists                            |
|      TreeMultiSet <br> _WIP_       |           Sorted Multiset           |            AVL Tree             |                                       A sorted mapping of a value and its multiplicity using an AVL tree                                       |
|   TreeSet      <br> _treeset.h_    |             Sorted Set              |            AVL Tree             |                               A unique set of keys using an AVL tree with `log(n)` look up and sorted iteration                                |

## Other Features

These are some features within the library that are implemented by all collections.

### Two-way iterators

All collections come with a two-way iterator. You can go back and forwards in constant time and access elements in constant time.

### Custom Allocation

All collections have a `cmc_alloc_node` which provides pointers to the four dynamic memory allocation functions in C: `malloc`, `calloc`, `realloc` and `free`. These pointers can be customized for each individual collection created or a default can be used, as specified in `cmc_alloc_node_default`.

### Callbacks

Every function that operates on a collection can be separated in 5 different types. Create, Read, Update, Delete and (an extra one besides CRUD) Resize. You can define one callback function for each operation. Check out the documentation to see when each callback function is called.

### Functions Table

Functions table is a `struct` of function pointers containing 'methods' for a custom data type. Some methods are optional and others are needed in order to a collection to operate. They are:

#### CMP

A comparator function is used in sorted collections or when an equality is being checked like when trying to find a certain element in a list. It is responsible for taking two arguments of the same data type and comparing them. The return value is an `int` with the following definitions:

* Return `1` if the first argument is greater than the second;
* Return `0` if the first argument equals the second;
* Return `-1` if the first argument is less than the second.

#### CPY

A copy function is used when a collection is being copied. It can be used to make a deep copy of of your custom data type. It must take a single parameter and return a new copy of that same data type. If this function is absent (`NULL`) the data type will be copied by assignment (for pointers this is a shallow copy).

#### STR

A string function is responsible for taking a `FILE` pointer and a custom data type and outputting the string representation of that data returning a `bool` indication success or failure. It is useful for debugging.

#### FREE

The free function is called when a collection is cleared (all elements removed) or freed (all elements removed and freed from memory) and it is responsible for completely freeing all resources that are usually acquired by your data type.

#### HASH

This function receives a custom data type as parameter and returns a `size_t` hash of that data. Used in hashtables.

#### PRI

A priority function works much like the comparator function except that it compares the priority between two elements. It is used in collections whose structure is based on the priority of elements and not in their general comparison.

* Return `1` if the first argument has a greater priority than the second;
* Return `0` if the first argument has the same priority as second;
* Return `-1` if the first argument has a lower priority than the second.

The following table shows which functions are required, optional or never used for each Collection:

| Collection   |                           CMP                            |                           CPY                            |                           STR                            |                           FREE                           |                           HASH                           |                           PRI                            |
| ------------ | :------------------------------------------------------: | :------------------------------------------------------: | :------------------------------------------------------: | :------------------------------------------------------: | :------------------------------------------------------: | :------------------------------------------------------: |
| Deque        | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| HashMap      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| HashBidiMap  | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| HashMultiMap | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| HashMultiSet | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| HashSet      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Heap         | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) |
| IntervalHeap | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) |
| List         | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| LinkedList   | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Queue        | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| SortedList   | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Stack        | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| TreeMap      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| TreeSet      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |

|                          Color                           | Label                                     |
| :------------------------------------------------------: | ----------------------------------------- |
| ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | Required for basic functionality.         |
| ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | Required for specific functions.          |
| ![#497edd](https://placehold.it/20/497edd/000000?text=+) | Required for non-core specific functions. |
| ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | Optional.                                 |
| ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | Not Used.                                 |

## Design Decisions

### Stack vs Heap Allocation

Currently all collections need to be allocated on the heap. Iterators have both options but it is encouraged to allocate them on the stack since they don't require dynamic memory.

### Some collections overlap others in terms of functionality

Yes, you can use a Deque as a Queue or a List as a Stack without any major cost, but the idea is to have the least amount of code to fulfill the needs of a collection.

Take for example the Stack. It is simple, small and doesn't have many functions. If you generate a List to be used (only) as a Stack (which is one of the bulkiest collections) you'll end up with a lot of code generated and compiled for nothing.

The Deque versus Queue situation is a little less problematic, but again, you need to be careful when generating a lot of code as compilation times might go up to 15 seconds even with modern ultra-fast compilers.

Another example is using a HashMap/TreeMap as a HashSet/TreeSet (with a dummy value that is never used), but I just think that this is a bad thing to do and you would be wasting some memory. Also, the sets generate a lot of code related to set theory, whereas maps don't.

### But what about the LinkedList ?

You can use them as Stacks, Queues and Deques, but with modern memory hierarchy models, array-based data structures have a significantly faster runtime due to caching, so I didn't bother to have specific implementations of those aforementioned collections.

### You can't structurally modify a collection when iterating over it

Modifying a collection will possibly invalidate all iterators currently initialized by it. Currently, the only collection that allows this is the LinkedList (using the node-based functions, not the iterator).

## What to use

(Outdated)

The following table shows how each collection is implemented and how well they do when using as common [abstract data types](https://en.wikipedia.org/wiki/Abstract_data_type).

* **Ideal** - The collection implements correctly the abstract data type;
* **Not Ideal** - The implementation is fulfilled but some functionalities are either not part of the ADT or not present;
* **Bad** - It can be done, but its a bad idea.

![DataStructuresDiagram](https://i.imgur.com/hFqBSlC.png)

![GoodColor](https://img.shields.io/badge/Ideal_Implementation-%236abf69.svg)
![AverageColor](https://img.shields.io/badge/Not_Ideal_Implementation-%2363a4ff.svg)
![BadColor](https://img.shields.io/badge/Bad_Implementation-%23ff6659.svg)

## Code Review

Check out some code reviews that covers some parts the project. Most of them were posted a long time ago and are probably outdated.

| About                                | Link                                                                                                                      |
| ------------------------------------ | ------------------------------------------------------------------------------------------------------------------------- |
| Unit Test *./utl/test.h*             | [![Code Review](http://www.zomis.net/codereview/shield/?qid=222954)](http://codereview.stackexchange.com/q/222954/178948) |
| Interval Heap *./cmc/intervalheap.h* | [![Code Review](http://www.zomis.net/codereview/shield/?qid=223595)](http://codereview.stackexchange.com/q/223595/178948) |
| Hash Set *./cmc/hashset.h*           | [![Code Review](http://www.zomis.net/codereview/shield/?qid=217333)](http://codereview.stackexchange.com/q/217333/178948) |
| Linked List *./cmc/linkedlist.h*     | [![Code Review](http://www.zomis.net/codereview/shield/?qid=216737)](http://codereview.stackexchange.com/q/216737/178948) |
| Others                               | [![Code Review](http://www.zomis.net/codereview/shield/?qid=213553)](http://codereview.stackexchange.com/q/213553/178948) |
