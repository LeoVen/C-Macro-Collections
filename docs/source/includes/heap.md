# heap.h

The Heap is a specialized tree-based data structure which is essentially a complete tree that satisfies the heap property:

__MaxHeap__

- \- The root node is the greatest element of the unique set of keys.
- \- A node key is always greater than or equal to its children keys.

__Minheap__

- \- The root node is the smallest element of the unique set of keys.
- \- A node Key is always greater than or equal to its children keys.

The heap is mostly used to implement priority queues. Sometimes it can also be used to sort elements. There are three main functions:

- insert : Adds an element to the heap
- remove_(min/max): Removes the min/max element from the heap
- min/max : Accesses the min/max element from the heap
