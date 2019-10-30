# C Macro Collections

<p align="center">
    <img src="https://leoven.github.io/C-Macro-Collections/images/logo.png" alt="C Macro Collections Logo" width="300"/>
</p>

<p align="center">Header only, macro generated, generic and type-safe Collections in C.</p>

<p align="center">
    <a href="https://github.com/LeoVen/C-Macro-Collections"><img src="https://img.shields.io/badge/GitHub-C%20Macro%20Collections-lightgrey.svg?logo=github" alt="LinkToRepo"/></a>
    <a href="https://leoven.github.io/C-Macro-Collections/"><img style="color: #ffffff;" src="https://img.shields.io/badge/Read%20the%20Docs-5A5A5A.svg?logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHN0eWxlPSIiPjxyZWN0IGlkPSJiYWNrZ3JvdW5kcmVjdCIgd2lkdGg9IjEwMCUiIGhlaWdodD0iMTAwJSIgeD0iMCIgeT0iMCIgZmlsbD0ibm9uZSIgc3Ryb2tlPSJub25lIi8%2BPHRpdGxlPmljbi9kb2MtdGV4dDwvdGl0bGU%2BPGcgY2xhc3M9ImN1cnJlbnRMYXllciIgc3R5bGU9IiI%2BPHRpdGxlPkxheWVyIDE8L3RpdGxlPjxwYXRoIGQ9Ik01IDFoNC4yNDRhMiAyIDAgMCAxIDEuNDM0LjYwNmwyLjc1NiAyLjgzNEEyIDIgMCAwIDEgMTQgNS44MzVWMTJhMyAzIDAgMCAxLTMgM0g1YTMgMyAwIDAgMS0zLTNWNGEzIDMgMCAwIDEgMy0zem0wIDJhMSAxIDAgMCAwLTEgMXY4YTEgMSAwIDAgMCAxIDFoNmExIDEgMCAwIDAgMS0xVjUuODM1TDkuMjQ0IDNINXptMS41IDdoM2EuNS41IDAgMSAxIDAgMWgtM2EuNS41IDAgMSAxIDAtMXptMC0yaDJhLjUuNSAwIDAgMSAwIDFoLTJhLjUuNSAwIDAgMSAwLTF6TTggMmwzIDEuOTk1TDEzIDdIOWExIDEgMCAwIDEtMS0xVjJ6IiBpZD0iYSIgY2xhc3M9IiIgZmlsbD0iI2ZmZmZmZiIgZmlsbC1vcGFjaXR5PSIxIi8%2BPC9nPjwvc3ZnPg%3D%3D" alt="LinkToDocs"/></a>
</p>

<p align="center">
    <a href="https://github.com/LeoVen/C-Macro-Collections/blob/master/LICENSE"><img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="License"/></a>
    <a href="#"><img src="https://img.shields.io/badge/Version-v1.7.3-orange.svg" alt="Version"/></a>
    <a href="https://travis-ci.org/LeoVen/C-Macro-Collections"><img src="https://travis-ci.org/LeoVen/C-Macro-Collections.svg?branch=master" alt="travis-ci"/></a>
    <a href="https://codecov.io/gh/LeoVen/C-Macro-Collections"><img src="https://codecov.io/gh/LeoVen/C-Macro-Collections/branch/master/graph/badge.svg" alt="codecov"/></a>
</p>

## Table of Contents

* Project Structure
* Available Collections
* Overall To-Do
* Design Decisions
* What to use
* How to use

## Project Structure

* __benchmarks__ - Where all benchmarks are hosted
* __examples__ - Examples separated by collections
* __src__ - All headers part of the C Macro Collections Library
    * __cmc__ - The main C Macro Collections Library
    * __dev__ - The main C Macro Collections Library for development (containing logging)
    * __sac__ - Statically  Allocated Collections
    * __utl__ - Utility like ForEach macros, logging, etc
    * __macro\_collections.h__ - Master header containing all collections and utilities
* __tests__ - Where all tests are hosted

## Available Collections

* Linear Collections
    * List, LinkedList, Deque, Stack, Queue
* Sets
    * HashSet, TreeSet, MultiSet
* Maps
    * HashMap, TreeMap, MultiMap
* Heaps
    * Heap, IntervalHeap
* Coming Soon
    * BidiMap, SortedList

| Collection <img width=250/>        | Abstract Data Type <img width=250/> | Data Structure <img width=250/> | Details                               |
| :--------------------------------: | :---------------------------------: | :-----------------------------: | :-----------------------------------: |
| BidiMap      <br> _bidimap.h_      | Bidirectional Map                   | Two Hashtables                  | A bijection between two sets of unique keys and unique values `K <-> V` using two hashtables |
| Deque        <br> _deque.h_        | Double-Ended Queue                  | Dynamic Circular Array          | A circular array that allows `push` and `pop` on both ends (only) at constant time |
| HashMap      <br> _hashmap.h_      | Map                                 | Hashtable                       | A unique set of keys associated with a value `K -> V` with constant time look up using a hashtable with open addressing and robin hood hashing |
| HashSet      <br> _hashset.h_      | Set                                 | Hashtable                       | A unique set of values with constant time look up  using a hashtable with open addressing and robin hood hashing |
| Heap         <br> _heap.h_         | Priority Queue                      | Dynamic Array                   | A binary heap as a dynamic array as an implicit data structure |
| IntervalHeap <br> _intervalheap.h_ | Double-Ended Priority Queue         | Custom Dynamic Array            | A dynamic array of nodes, each hosting one value from the MinHeap and one from the MaxHeap |
| LinkedList   <br> _linkedlist.h_   | List                                | Doubly-Linked List              | A default doubly-linked list |
| List         <br> _list.h_         | List                                | Dynamic Array                   | A dynamic array with `push` and `pop` anywhere on the array |
| MultiMap     <br> _multimap.h_     | Multimap                            | Custom Hashtable                | A mapping of multiple keys with one node per key using a hashtable with separate chaining |
| Multiset     <br> _multiset.h_     | Multiset                            | Hashtable                       | A mapping of a value and its multiplicity using a hashtable with open addressing and robin hood hashing |
| Queue        <br> _queue.h_        | FIFO                                | Dynamic Circular Array          | A queue using a circular array with `enqueue` at the `back` index and `dequeue` at the `front` index |
| SortedList   <br> _sortedlist.h_   | Sorted List                         | Sorted Dynamic Array            | A lazily sorted dynamic array that is sorted only when necessary |
| Stack        <br> _stack.h_        | FILO                                | Dynamic Array                   | A stack with push and pop at the end of a dynamic array |
| TreeMap      <br> _treemap.h_      | Sorted Map                          | AVL Tree                        | A unique set of keys associated with a value `K -> V` using an AVL tree with `log(n)` look up and sorted iteration |
| TreeSet      <br> _treeset.h_      | Sorted Set                          | AVL Tree                        | A unique set of keys using an AVL tree with `log(n)` look up and sorted iteration |

## Overall To-Do

In the long term, these are the steps left for the completion of this library:

* Complete the implementation of all the functions in the scope of the TODO file for the main collections;
* Reorganize and complete all tests for the `cmc` collections;
* Make an exact copy of all collections to `dev` with many logging utility, for them to be used under development;
* Port all of these collections to be statically allocated and be part of the `sac` library;
* Complete all tests for `sac`.

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

To generate the collection, all you need to do is to include the necessary header files. You can include the containers you want to use individually or you can include the master header, `macro_collections.h`, that comes with the entire C-Macro-Collections library.

### Macros

> Note here that `SNAME` represents the uppercase name of the collection.

Every collection is separated by two parts:

* `HEADER` - Contains all struct definitions and function definitions.
* `SOURCE` - Contains all function implementations.

All collections have three main macros:

* `CMC_GENERATE_SNAME` - Generates `CMC_GENERATE_SNAME_HEADER` and `CMC_GENERATE_SNAME_SOURCE`.

Or you can generate each part individually:

* `CMC_GENERATE_SNAME_HEADER` - Generates all struct definitions and function definitions.
* `CMC_GENERATE_SNAME_SOURCE` - Generates all function implementations.

### Parameters

When including `macro_collections.h` in your source code you gain access to a macro called `CMC_COLLECTION_GENERATE` with the following parameters:

* __C__ - Container name in uppercase (*LIST*, *LINKEDLIST*, *STACK*, *QUEUE*, *DEQUE*, *HEAP*, *TREESET*, *TREEMAP*, *HASHSET*, *HASHMAP*).
* __PFX__ - Functions prefix or namespace.
* __SNAME__ - Structure name (`typedef struct SNAME##_s SNAME`).
* __K__ - Key type. Only used in *HASHMAP*, *TREEMAP*, *MULTIMAP* and *BIDIMAP*; ignored by others.
* __V__ - Value type. Primary type for most collections, or value to be mapped by *HASHMAP*, *TREEMAP*, *MULTIMAP* and *BIDIMAP*.

**In fact, all macros follow this pattern.** So whenever you see a macro with a bunch of parameters and you don't know what they are, you can check out the above list.

### For Each

There are 2 for-each macros:

* `CMC_FOR_EACH` - Starts at the start of the collection towards the end.
* `CMC_FOR_EACH_REV` - Starts at the end of the collection towards the start.

When including `foreach.h` in your source code you gain access to all for-each macros with the following parameters:

* __PFX__ - Functions prefix or namespace.
* __SNAME__ - Structure name.
* __TARGET__ - The variable name of the collection you wish to iterate over.
* __BODY__ - Block of code.

Inside body you will have access to the iterator variable `iter`. With it you can use functions to access the key, value or index from the iterator. Checkout the documentation for more details.

<hr>

Check out some code reviews that covers some parts the project:

| About | Link |
|-------|--------|
| Unit Test *./utl/test.h* | [![Code Review](http://www.zomis.net/codereview/shield/?qid=222954)](http://codereview.stackexchange.com/q/222954/178948) |
| Interval Heap *./cmc/intervalheap.h* | [![Code Review](http://www.zomis.net/codereview/shield/?qid=223595)](http://codereview.stackexchange.com/q/223595/178948) |
| Hash Set *./cmc/hashset.h* | [![Code Review](http://www.zomis.net/codereview/shield/?qid=217333)](http://codereview.stackexchange.com/q/217333/178948) |
| Linked List *./cmc/linkedlist.h* | [![Code Review](http://www.zomis.net/codereview/shield/?qid=216737)](http://codereview.stackexchange.com/q/216737/178948) |
| Others | [![Code Review](http://www.zomis.net/codereview/shield/?qid=213553)](http://codereview.stackexchange.com/q/213553/178948) |
