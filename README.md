# C-Macro-Collections

Generate Simple Data Structures of any type in C for you to use in your projects.

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![Version](https://img.shields.io/badge/Version-v1.4.0-orange.svg)
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

## Available Collections in future updates

* Multimap
* Multiset
* Treehashmap
* Heap will be a MinMaxHeap

Check out **Documentation.md** for more information about each collection.

## What to use

The following table shows how each collection is implemented and how well they do when using as common [abstract data types](https://en.wikipedia.org/wiki/Abstract_data_type).

* **Ideal** - The collection implements correctly the abstract data type;
* **Not Ideal** - The implementation is fulfilled but some functionalities are either not part of the ADT or not present;
* **Bad** - It can be done, but its a bad idea.

![DataStructuresDiagram](https://i.imgur.com/hFqBSlC.png)

![GoodColor](https://img.shields.io/badge/Ideal_Implementation-%236abf69.svg)
![AverageColor](https://img.shields.io/badge/Not_Ideal_Implementation-%2363a4ff.svg)
![BadColor](https://img.shields.io/badge/Bad_Implementation-%23ff6659.svg)

## How to use

To generate the collection, all you need to do is to include the necessary header files. You can include the containers you want to use individually or you can include the master header, `macro_collections.h`, that comes with all collections and other functionalities like the `FOR_EACH` macro.

There are three C standard headers that all collections use:

* `<stdlib.h>`
* `<stdbool.h>`
* `<string.h>`

With the last one being used only by array based containers. These headers are already included by default.

### Macros

> Note here that `SNAME` represents the uppercase name of the collection.

Every collection is separated by two parts:

* `HEADER` - Contains all struct definitions and function definitions.
* `SOURCE` - Contains all function implementations.

All collections have three main macros:

* `SNAME_GENERATE` - Generates `SNAME_GENERATE_HEADER` and `SNAME_GENERATE_SOURCE`.

Or you can generate each part individually:

* `SNAME_GENERATE_HEADER` - Generates all struct definitions and function definitions.
* `SNAME_GENERATE_SOURCE` - Generates all function implementations.

### For Each

There are 2 for-each macros:

* `FOR_EACH` - Starts at the start of the collection towards the end.
* `FOR_EACH_REV` - Starts at the end of the collection towards the start.

Check out the **Documentation.md** to know exactly what represents the *end* and the *start* of each collection.

### Parameters

When including `macro_collections.h` in your source code you gain access to a macro called `COLLECTION_GENERATE` with the following parameters:

* \*__C__ - Container name in uppercase (*LIST*, *LINKEDLIST*, *STACK*, *QUEUE*, *DEQUE*, *HEAP*, *TREESET*, *TREEMAP*, *HASHSET*, *HASHMAP*).
* \*__PFX__ - Functions prefix or namespace.
* \*__SNAME__ - Structure name (`typedef struct SNAME##_s SNAME`).
* __FMOD__ - Function modifier (`static` or empty).
* \*\*__K__ - Key type. Only used in *HASHMAP* and *TREEMAP*; ignored by others.
* \*__V__ - Value type. Primary type for most collections, or value to be mapped by *HASHMAP* and *TREEMAP*.

\* Required parameters

\*\* Required only by *HASHMAP* and *TREEMAP*

**In fact, all macros follow this pattern.** So whenever you see a macro with a bunch of parameters and you don't know what they are, you can check out the above list.

When including `foreach.h` in your source code you gain access to all for-each macros with the following parameters:

* __PFX__ - Functions prefix or namespace.
* __SNAME__ - Structure name.
* __TARGET__ - The variable name of the collection you wish to iterate over.
* __BODY__ - Block of code.

Inside body you will have access to the iterator variable. With it you can use functions to access the key, value or index from the iterator.
