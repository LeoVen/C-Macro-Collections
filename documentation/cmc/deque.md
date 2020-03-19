# deque.h

A Deque, also known as a double-ended queue, is a linear data structure that is able to add and remove elements from both ends. It can also be thought of a double-ended stack since you can push and pop elements from two ends. The Deque can also be used as a Queue. The only elements accessible from a Deque ar the two elements at its ends (front element and back element).

## Deque Implementation

This implementation uses a circular buffer (ring buffer or cyclic buffer) in order to operate on O(1) for push and pop on either ends. The only case where it takes longer than O(1) is when the buffer is reallocated. If it was implemented as a regular array, adding or removing elements from the front would take O(N) due to the need to shift all elements in the Deque.

Two indices are kept track of. The `front` index and the `rear` index. These represent the both ends of the Deque. If an index reaches one end of the real buffer, they wrap around to the other end and an element is added there. This abstracts the real buffer as a circular buffer with the cost os constantly checking for boundaries and using the modulo operator.

## Deque Generation Macro

`CMC_GENERATE_DEQUE(PFX, SNAME, V)`

|         |                               |
| ------- | ----------------------------- |
| `PFX`   | Functions namespace or prefix |
| `SNAME` | Structure name                |
| `V`     | Element type                  |

## Deque Structures

|                       |                                     |
| --------------------- | ----------------------------------- |
| `struct SNAME`        | A double-ended queue data structure |
| `struct SNAME##_iter` | A double-ended queue iterator       |

### Members

| `struct SNAME`              |                                                              |
| --------------------------- | ------------------------------------------------------------ |
| `V *buffer`                 | Dynamic circular array of elements.                          |
| `size_t capacity`           | Current circular array capacity.                             |
| `size_t count`              | Current amount of elements.                                  |
| `size_t front`              | Index representing the front of the deque.                   |
| `size_t rear`               | Index representing the back of the deque.                    |
| `it_start` <sup>\[1\]</sup> | Function that returns an iterator to the start of the deque. |
| `it_end` <sup>\[2\]</sup>   | Function that returns an iterator to the end of the deque.   |

1. Full definition: `struct SNAME##_iter (*it_start)(struct SNAME *);`.
2. Full definition: `struct SNAME##_iter (*it_end)(struct SNAME *);`.

| `struct SNAME##_iter`  |                                                     |
| ---------------------- | --------------------------------------------------- |
| `struct SNAME *target` | Deque being iterated over.                          |
| `size_t cursor`        | Cursor's current position (index).                  |
| `size_t index`         | Relative index to all elements in the iteration.    |
| `bool start`           | If the iterator reached the start of the iteration. |
| `bool end`             | If the iterator reached the end of iteration.       |

## Deque Functions

## Deque Callback Table
