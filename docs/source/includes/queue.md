# queue.h

A Queue is a First-In First-out (or Last-in Last-out) data structure. It is a Dynamic Circular Array where elements are added from one end of the array and removed from the other end. The circular array here (also known as circular buffer or ring buffer) is very important so that both adding and removing elements from the Queue are done instantly. The array is linear but with the modulo operator it is treated as a circular sequence of elements.

## Queue Implementation

If the Queue was implemented as a regular Dynamic Array, when adding or removing an element at the front, it would be necessary to shift all elements currently present in the Queue and this would add up a lot of computing time. Shifting `100000` elements in memory by one position every time an element is added to the Queue is simply not efficient.

The Queue has two ends. The `front` and `back`. In this implementation all elements are added to the back of the Queue and removed from the front, which is more or less how queues work in real life. Unlike a Stack that only has operations at one end of the buffer, the Queue needs to be implemented as a circular array in order to quickly add or remove elements.

The Queue has three main functions: `enqueue` which adds an element to the Queue; `dequeue` which removes an element from the Queue; and `peek` which return the element at the front of the Queue, that is, the next element to be removed from it.

The Queue is used in many applications where a resource is shared among multiple consumers and the Queue is responsible for scheduling the access to the resource.
