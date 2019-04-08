# C-Macro-Collections

Generate Simple Data Structures of any type in C for you to use in your projects.

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![Version](https://img.shields.io/badge/Version-v1.1.0-orange.svg)
![Build](https://travis-ci.org/LeoVen/C-Macro-Collections.svg?branch=master)

## Available Collections

* List
* LinkedList
* Stack
* Queue
* Deque
* Heap
* TreeSet
* TreeMap
* HashSet
* HashMap

Check out **Documentation.md** for more information about each data structure.

## What to use

The following table shows how each data structure is implemented and how well they do when using as common [abstract data types](https://en.wikipedia.org/wiki/Abstract_data_type).

* **Ideal** - The data structure implements correctly the abstract data type;
* **Not Ideal** - The implementation is fulfilled but some functionalities are either not part of the ADT or not present;
* **Bad** - It can be done, but its a bad idea.

![DataStructuresDiagram](https://i.imgur.com/hFqBSlC.png)

![GoodColor](https://img.shields.io/badge/Ideal_Implementation-%236abf69.svg)
![AverageColor](https://img.shields.io/badge/Not_Ideal_Implementation-%2363a4ff.svg)
![BadColor](https://img.shields.io/badge/Bad_Implementation-%23ff6659.svg)

## How to use

To generate the data structures, all you need to do is to include the necessary header files. You can include the containers you want to use individually or you can include the master header, `macro_collections.h`, that comes with all collections and other functionalities like the `FOR_EACH` macro.

There are three C standard headers that all collections use:

* `<stdlib.h>`
* `<stdbool.h>`
* `<string.h>`

With the last one being used only by array based containers. These headers are already included by default.

### Macros

> Note here that `SNAME` represents the uppercase name of the data structure.

Every collection is separated by three parts:

* `STRUCT` - Contains all structure definitions.
* `HEADER` - Contains all functions definitions.
* `SOURCE` - Contains all functions implementations.

All collections have five main macros:

* PUBLIC
    * `SNAME_GENERATE_HEADER_PUBLIC` - Generates both `STRUCT` and `HEADER`
    * `SNAME_GENERATE_SOURCE_PUBLIC` - Generates `SOURCE`
* PRIVATE
    * `SNAME_GENERATE_HEADER_PRIVATE` - Generates `HEADER`
    * `SNAME_GENERATE_SOURCE_PRIVATE` - Generates both `STRUCT` and `SOURCE`
* Standalone
    * `SNAME_GENERATE` - Generates `STRUCT`, `HEADER` and `SOURCE`.

Or you can generate each part individually:

* `SNAME_GENERATE_STRUCT` - Generates all structure definitions.
* `SNAME_GENERATE_HEADER` - Generates all functions definitions.
* `SNAME_GENERATE_SOURCE` - Generates all functions implementations.

### For Each

There are 4 for-each macros:

* `FOR_EACH` - Starts at the start of the data structure towards the end.
* `FOR_EACH_REV` - Starts at the end of the data structure towards the start.
* `FOR_EACH_MAP` - Like `FOR_EACH` but for associative data structures like *TREEMAP* and *HASHMAP*.
* `FOR_EACH_MAP_REV` - Like `FOR_EACH_REV` but for associative data structures like *TREEMAP* and *HASHMAP*.

Check out the **Documentation.md** to know exactly what represents the *end* and the *start* of each data structure.

### Parameters

When including `macro_collections.h` in your source code you gain access to a macro called `COLLECTION_GENERATE` with the following parameters:

* \*__C__ - Container name in uppercase (*LIST*, *LINKEDLIST*, *STACK*, *QUEUE*, *DEQUE*, *HEAP*, *TREESET*, *TREEMAP*, *HASHSET*, *HASHMAP*).
* \*__P__ - Permission (PRIVATE, PUBLIC). Only matters when header and source are declared in separate files.
	* *PRIVATE* - Members of a struct are hidden in source file and only accessible through functions.
	* *PUBLIC* - Members of a struct are visible.
* \*__PFX__ - Functions prefix or namespace.
* \*__SNAME__ - Structure name (`typedef struct SNAME##_s SNAME`).
* __FMOD__ - Function modifier (`static` or empty).
* \*\*__K__ - Key type. Only used in *HASHMAP* and *TREEMAP*; ignored by others.
* \*__V__ - Value type. Primary type for most data structures, or value to be mapped by *HASHMAP* and *TREEMAP*.

\* Required parameters

\*\* Required only by *HASHMAP* and *TREEMAP*

**In fact, all macros follow this pattern.** So whenever you see a macro with a bunch of parameters and you don't know what they are, you can check out the above list.

When including `foreach.h` in your source code you gain access to all for-each macros with the following parameters:

* __PFX__ - Functions prefix or namespace.
* __SNAME__ - Structure name.
* __V__ - Value type.
* \*__K__ - Key type.
* __TARGET__ - The variable name of the data structure you wish to iterate over.
* __BODY__ - Block of code.

\* Only available for `FOR_EACH_MAP` and `FOR_EACH_MAP_REV`.

Inside body you will have access to three new variables:

* `iter` - Represents the iterator.
* \*`key` - Represents the resulting key on each iteration.
* `value` - Represents the resulting value on each iteration.

\* Only available in `FOR_EACH_MAP` and `FOR_EACH_MAP_REV`.

### Separate Header and Source

Below is an example of how to hide the struct members when using a separate source file.

**header.h**

```c
#include "macro_collection.h"

COLLECTION_GENERATE_HEADER(LIST, PRIVATE, l, list, , , int)
```

**source.c**

```c
#include "header.h"

COLLECTION_GENERATE_SOURCE(LIST, PRIVATE, l, list, , , int)
```

**main.c**


```c
#include "header.h"

int main(void)
{
    list *my_list = l_new(100);

    // Valid code
    size_t len1 = l_count(my_list);

    // Invalid code
    size_t len2 = my_list->count;

    l_free(my_list);
}
```

**Compilation process**

```
gcc -c source.c -I ./path/to/source
gcc -c main.c -I ./path/to/source
gcc source.o main.o
```

The container members won't be visible to the file `main.c`. For this to work, all parameters must be identical, with the only difference being the macro name.
