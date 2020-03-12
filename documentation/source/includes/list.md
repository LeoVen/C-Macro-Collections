# list.h

A List is a Dynamic Array that store its elements contiguously. It has an internal buffer that is pre-allocated with a greater capacity than necessary in order to foresee new elements being added. The items can be added at both ends of the list and at the middle (with a given index).

## List Implementation

Insertions at the start are more costly as it is necessary to shift all elements one position to the right to give space to a new element being added at index 0. Insertions at the end are instantaneous as long as there is enough space in the buffer. Insertions at the middle will be based on an index. The index represents where the element being added will be located once the operation ends. This operation might also need to shift elements. It is also possible to add arrays of elements in the list. When the buffer is filled, it is reallocated with a greater capacity, usually being doubled.

Removing elements follows the same principle. Removing the front element will require to shift all other elements one position to the left, thus being slower than removing from the end in which is done in constant time. Removing elements in the middle of the list will also require shifting elements to the left. Is is also possible to remove a range of elements or extract them, creating a new list with the removed items.

The iterator is a simple structure that is capable of going back and forwards. Any modifications to the target list during iteration is considered undefined behavior. Its sole purpose is to facilitate navigation through a list.
