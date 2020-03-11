# linkedlist.h

A LinkedList is a linear collection of elements, whose order is not given by their physical placement in memory. Instead, each element points to the next (singly-linked list) and to a previous one (doubly-linked list). It consists of nodes connected by these next or previous pointers. Unlike an array, the LinkedList can indefinitely grow.

## LinkedList Implementation

The LinkedList is implemented as a doubly-linked list and allows insertions and removals at both ends in O(1) and in a given index in O(N). The list has a head and tail pointer. The head points to the first element in the sequence and tail points to the last.
