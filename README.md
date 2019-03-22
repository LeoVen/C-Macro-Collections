# C-Macro-Collections

Generate Simple Data Structures of any type in C for you to use in your projects.

## How to use

All macros have a series of default parameters. Some might not be required or might not be used by the data structure.

* \*__C__ - Container type (*LIST*, *LINKEDLIST*, *STACK*, *QUEUE*, *DEQUE*, *HASHMAP*, *TREEMAP*, *HASHSET*, *TREESET*, *HEAP*).
* \*__P__ - Permission (PRIVATE, PUBLIC). Only matters when header and source are declared in separate files.
	* *PRIVATE* - Members of a struct are hidden in source file and only accessible through functions.
	* *PUBLIC* - Members of a struct are visible.
* \*__PFX__ - Functions prefix or namespace.
* \*__SNAME__ - Structure name (`typedef struct SNAME##_s SNAME`).
* __FMOD__ - Function modifier (static). Useful when declaring header and source in the same file.
* \*\*__K__ - Key type. Only used in *HASHMAP* and *TREEMAP*; ignored by others.
* \*__V__ - Value type. Primary type for most data structures, or value to be mapped by *HASHMAP* and *TREEMAP*.

\* Required parameters

\*\* Required only by *HASHMAP* and *TREEMAP*

## List

A contiguous growable array. Elements can be added and removed from both ends of the array and also in the middle. This list can also be used to implement other data structures like stack, queues and deques, although queues and deques are implemented as a circular buffer, therefore being more efficient.

## LinkedList

A list where each element is linked by nodes. Elements can be added and removed from both ends of the list and also in the middle. The linked list can be used to implement other data structures like queues, stacks and deques.

## Stack

A LIFO/FILO structure backed by a growable array. All elements are added and removed from one end.

## Queue

A FIFO/LILO structure backed by a circular buffer. Elements are added in one end and removed from the other.

## Deque

A double-ended queue backed by a circular buffer. Elements can be added and removed from both ends but not in the middle. Can also be used as a queue.

## HashMap

A hashtable that maps a key `K` to a value `V`.

## TreeMap

A balanced binary tree that maps a key `K` to a value `V`. The difference from a hashmap is that its keys are ordered.

## HashSet

A hashtable that contains only unique elements to the set. Its elements are not ordered.

## TreeSet

A balanced binary tree that contains only unique elements to the set. Its elements are ordered.

## Heap

A heap is a nearly complete binary tree backed by a growable array. The first element of the array (or the root element of the tree) is the highest/lowest element. The min-heap is commonly used as a priority-queue and a max-heap is commonly used to sort an array of elements.
