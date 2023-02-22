# Collections Overview

| Collection                         | Abstract Data Type                  | Data Structure                  | Details                               |
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
