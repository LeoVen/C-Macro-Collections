# Files Overview

An overview of the source files (all `.h`) of the C Macro Collections library.

* `./cmc` - The main C Macro Collections library
    * `./bidimap.h` - A bi-directional map based on a hash table
    * `./deque.h` - A double-ended queue
    * `./hashmap.h` - A map based on a hash table
    * `./hashset.h` - A set based on a hash table
    * `./heap.h` - A binary heap based on a dynamic array
    * `./intervalheap.h` - A heap that is both Min and Max based on a dynamic array
    * `./linkedlist.h` - A doubly-linked list
    * `./list.h` - A dynamic array
    * `./multimap.h` - A map that accepts multiple keys based on a hash table
    * `./multiset.h` - A multiset based on a hash table
    * `./queue.h` - A FIFO based on a circular dynamic array
    * `./sortedlist.h` - A sorted list based on a dynamic array
    * `./stack.h` - A LIFO based on a dynamic array
    * `./treemap.h` - A sorted map based on an AVL tree
    * `./treeset.h` - A sorted set based on an AVL tree
* `./cor` - Core functionalities of the C Macro Collections libraries
    * `./core.h` - Core functionalities of the library
    * `./hashtable.h` - Common things used by hash table based collections
* `./dev` - A mirror of the main library to be used during development
* `./sac` - Statically Allocated Collections that don't use dynamic allocation
* `./utl` - Utilities
    * `./assert.h` - Non-abortive assert macros
    * `./foreach.h` - For Each macros
    * `./futils.h` - Common functions used by Functions Table
    * `./log.h` - Logging utility with levels of severity
    * `./test.h` - Simple Unit Test building with macros
    * `./test.h` - Timing code execution utility
