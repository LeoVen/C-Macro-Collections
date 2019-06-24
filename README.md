# C-Macro-Collections

Generate Simple Data Structures of any type in C for you to use in your projects.

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![Version](https://img.shields.io/badge/Version-v1.5.1-orange.svg)
![Build](https://travis-ci.org/LeoVen/C-Macro-Collections.svg?branch=master)

## Project Structure

* __benchmarks__ - Where all benchmarks are hosted
* __examples__ - Examples separated by collections
* __src__ - All headers part of the C Macro Collections Library
    * __cmc__ - The main C Macro Collections Library
    * __dev__ - The main C Macro Collections Library for development (containing logging)
    * __ext__ - Extra collections
    * __sac__ - Stack Allocated Collections
    * __utl__ - Utility like ForEach macros, logging, etc
    * __macro\_collections.h__ - Master header containing all collections and utilities
* __tests__ - Where all tests are hosted

Only Collections located in __cmc__ are to be documented at *Documentation.md*.

## Available Collections

* Main C Macro Collections Library
    * Deque
    * HashMap
    * HashSet
    * Heap
    * LinkedList
    * List
    * Queue
    * Stack
    * TreeMap
    * TreeSet
* Extra Collections Library
    * LinkedQueue
* Stack Allocated Collections Library
    * Queue
    * Stack

### More to Come

* MultiMap
* MultiSet
* TreeHashMap
* Heap will be an IntervalHeap
* Stack Allocated Collections

Check out **Documentation.md** for more information about each collection.

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

* __C__ - Container name in uppercase (*LIST*, *LINKEDLIST*, *STACK*, *QUEUE*, *DEQUE*, *HEAP*, *TREESET*, *TREEMAP*, *HASHSET*, *HASHMAP*).
* __PFX__ - Functions prefix or namespace.
* __SNAME__ - Structure name (`typedef struct SNAME##_s SNAME`).
* __FMOD__ - Function modifier (`static` or empty).
* \*__K__ - Key type. Only used in *HASHMAP* and *TREEMAP*; ignored by others.
* __V__ - Value type. Primary type for most collections, or value to be mapped by *HASHMAP* and *TREEMAP*.

\* Required only by *HASHMAP* and *TREEMAP*

**In fact, all macros follow this pattern.** So whenever you see a macro with a bunch of parameters and you don't know what they are, you can check out the above list.

When including `foreach.h` in your source code you gain access to all for-each macros with the following parameters:

* __PFX__ - Functions prefix or namespace.
* __SNAME__ - Structure name.
* __TARGET__ - The variable name of the collection you wish to iterate over.
* __BODY__ - Block of code.

Inside body you will have access to the iterator variable. With it you can use functions to access the key, value or index from the iterator. Checkout the documentation for more details.
