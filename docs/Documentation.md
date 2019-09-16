__Check out the new documentation at__ https://leoven.github.io/C-Macro-Collections/

<span id="collections_index"> Collections Documentation </span>

* [Deque](#Deque)
* [HashMap](#HashMap)
* [HashSet](#HashSet)
* [Heap](#Heap)
* [LinkedList](#LinkedList)
* [List](#List)
* [Queue](#Queue)
* [Stack](#Stack)
* [TreeMap](#TreeMap)
* [TreeSet](#TreeSet)

Others

* [ForEach](#ForEach)

# [Deque](#collections_index)

A double-ended queue backed by a circular buffer. Elements can be added and removed from both ends but not in the middle. Can also be used as a queue.

### Generation Macro

* `DEQUE_GENERATE(PFX, SNAME, FMOD, V)`
    * `PFX` - Functions namespace or prefix.
    * `SNAME` - Structure name.
    * `FMOD` - Function modifier (static or empty).
    * `V` - Element type.

### Defined Structures

* `struct SNAME##_s` - Structure Name (represents a double-ended queue structure)
    * `V *buffer` - Dynamic circular array of elements.
    * `size_t capacity` - Current circular array capacity.
    * `size_t count` - Current amount of elements.
    * `size_t front` - Index representing the front of the deque.
    * `size_t rear` - Index representing the back of the deque.
    * `it_start` - Function that returns an iterator to the start of the deque.
        * Full definition: `struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);`.
    * `it_end` - Function that returns an iterator to the end of the deque.
        * Full definition: `struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);`.

* `struct SNAME##_iter_s` - Structure Iterator (represents a double-ended queue iterator)
    * `struct SNAME##_s *target` - Deque being iterated over.
    * `size_t cursor` - Cursor's current position (index).
    * `size_t index` - Relative index to all elements in the iteration.
    * `bool start` - If the iterator reached the start of the iteration.
    * `bool end` - If the iterator reached the end of iteration.

### Typedefs

* `typedef struct SNAME##_s SNAME`
* `typedef struct SNAME##_s *SNAME##_ptr`
* `typedef struct SNAME##_iter_s SNAME##_iter`
* `typedef struct SNAME##_iter_s *SNAME##_iter_ptr`

### <span id="deque_function_index"> Defined Functions </span>

#### Collection Allocation and Deallocation

* [\_new()](#deque_new)
* [\_clear()](#deque_clear)
* [\_free()](#deque_free)

#### Input and Output

* [\_push\_front()](#deque_push_front)
* [\_push\_back()](#deque_push_back)
* [\_pop\_front()](#deque_pop_front)
* [\_pop\_back()](#deque_pop_back)

#### Conditional Input and Output

* [\_push\_front\_if()](#deque_push_front_if)
* [\_push\_back\_if()](#deque_push_back_if)
* [\_pop\_front\_if()](#deque_pop_front_if)
* [\_pop\_back\_if()](#deque_pop_back_if)

#### Elements Access

* [\_front()](#deque_front)
* [\_back()](#deque_back)

#### Collection State

* [\_contains()](#deque_contains)
* [\_empty()](#deque_empty)
* [\_full()](#deque_full)
* [\_count()](#deque_count)
* [\_capacity()](#deque_capacity)

#### Iterator Allocation and Deallocation

* [\_iter\_new()](#deque_iter_new)
* [\_iter\_free()](#deque_iter_free)

#### Iterator Initialization

* [\_iter\_init()](#deque_iter_init)

#### Iterator State

* [\_iter\_start()](#deque_iter_start)
* [\_iter\_end()](#deque_iter_end)

#### Iterator Movement

* [\_iter\_to\_start()](#deque_iter_to_start)
* [\_iter\_to\_end()](#deque_iter_to_end)
* [\_iter\_next()](#deque_iter_next)
* [\_iter\_prev()](#deque_iter_prev)

#### Iterator Access

* [\_iter\_value()](#deque_iter_value)
* [\_iter\_rvalue()](#deque_iter_rvalue)
* [\_iter\_index()](#deque_iter_index)

## [<span id="deque_new"> \_new() </span>](#deque_function_index)

Allocates and returns a new deque with an internal capacity of the specified value. If allocation fails, `NULL` is returned.

#### Declaration

> `FMOD SNAME *PFX##_new(size_t capacity);`

#### Parameters

1. `size_t capacity` - The initial capacity for the deque.

#### Returns

1. `SNAME *` - A pointer to a heap allocated deque.
2. `NULL` - If allocation fails.

#### Example

```c
#include "deque.h"

// Generate a DEQUE of type int
DEQUE_GENERATE(d, deque, /* FMOD */, int)

int main(void)
{
    // Make a deque of integers with an initial capacity of 100
    // The deque and its internal buffer is allocated in the heap
    deque *int_deque = d_new(100);

    if (!int_deque)
        // Allocation failed
        return 1;

    // Deque is ready to be used

    // Be sure to free its resources later
    d_free(int_deque);
}
```

## [<span id="deque_clear"> \_clear() </span>](#deque_function_index)

Removes all elements in the deque but does not frees the deque structure. This will not free any element belonging to the deque.

#### Declaration

> `FMOD void PFX##_clear(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque to have all of its elements removed.

#### Undefined Behavior

* If the deque pointer is not a valid memory location.

#### Example

```c
#include "deque.h"

// Generate a DEQUE of type int
DEQUE_GENERATE(d, deque, /* FMOD */, int)

int main(void)
{
    // Make a deque of integers with an initial capacity of 100
    deque *int_deque = d_new(100);

    // Add elements to the deque
    for (int i = 0; i < 50; i++)
        d_push_front(int_deque, i);

    // empty the deque
    d_clear(int_deque);

    // deque now has a size of 0 but the deque was not freed from
    // memory and can still be used

    // Be sure to free its resources after use
    d_free(int_deque);
}
```

## [<span id="deque_free"> \_free() </span>](#deque_function_index)

Frees from memory the deque internal buffer and the structure itself. Note that if the elements inside the deque are pointers to allocated memory, this function might cause memory leaks as it does not deals with its elements.

#### Declaration

> `FMOD void PFX##_free(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque to be freed from memory.

## [<span id="deque_push_front"> \_push\_front() </span>](#deque_function_index)

Adds an element to the front of the deque.

#### Declaration

> `FMOD bool PFX##_push_front(SNAME *_deque_, V element);`

#### Parameters

1. `SNAME *_deque_` - Target deque.
2. `V element` - Element to be added.

#### Returns

1. `true` - If the element was successfully added to the front of the deque.
2. `false` - If buffer reallocation failed.

## [<span id="deque_push_back"> \_push\_back() </span>](#deque_function_index)

Adds an element to the back of the deque.

#### Declaration

> `FMOD bool PFX##_push_back(SNAME *_deque_, V element);`

#### Parameters

1. `SNAME *_deque_` - Target deque.
2. `V element` - Element to be added.

#### Returns

1. `true` - If the element was successfully added to the back of the deque.
2. `false` - If buffer reallocation failed.

## [<span id="deque_pop_front"> \_pop\_front() </span>](#deque_function_index)

Removes an element from the front of the deque. If the deque is empty, nothing happens.

#### Declaration

> `FMOD bool PFX##_pop_front(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `true` - If an element was successfully removed from the front of the deque.
2. `false` - If the deque is empty.

## [<span id="deque_pop_back"> \_pop\_back() </span>](#deque_function_index)

Removes an element from the back of the deque. If the deque is empty, nothing happens.

#### Declaration

> `FMOD bool PFX##_pop_back(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `true` - If an element was successfully removed from the back of the deque.
2. `false` - If the deque is empty.

## [<span id="deque_push_front_if"> \_push\_front\_if() </span>](#deque_function_index)

Adds an element to the front of the deque if the condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_push_front_if(SNAME *_deque_, V element, bool condition);`

#### Parameters

1. `SNAME *_deque_` - Target deque.
2. `V element` - Element to be added.
3. `bool condition` - Condition for the element to be added.

#### Returns

1. `true` - If the element was successfully added to the front of the deque.
2. `false` - If the condition evaluated to false, or if buffer reallocation failed.

## [<span id="deque_push_back_if"> \_push\_back\_if() </span>](#deque_function_index)

Adds an element to the back of the deque if the condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_push_back_if(SNAME *_deque_, V element, bool condition);`

#### Parameters

1. `SNAME *_deque_` - Target deque.
2. `V element` - Element to be added.
3. `bool condition` - Condition for the element to be added.

#### Returns

1. `true` - If the element was successfully added to the back of the deque.
2. `false` - If the condition evaluated to false, or if buffer reallocation failed.

## [<span id="deque_pop_front_if"> \_pop\_front\_if() </span>](#deque_function_index)

Removes an element from the front of the deque if the condition evaluates to true. If the deque is empty, nothing happens.

#### Declaration

> `FMOD bool PFX##_pop_front_if(SNAME *_deque_, bool condition);`

#### Parameters

1. `SNAME *_deque_` - Target deque.
2. `bool condition` - Condition for the element to be removed.

#### Returns

1. `true` - If an element was successfully removed from the front of the deque.
2. `false` - If the condition evaluated to false, or if the deque is empty.

## [<span id="deque_pop_back_if"> \_pop\_back\_if() </span>](#deque_function_index)

Removes an element from the back of the deque if the condition evaluates to true. If the deque is empty, nothing happens.

#### Declaration

> `FMOD bool PFX##_pop_back_if(SNAME *_deque_, bool condition);`

#### Parameters

1. `SNAME *_deque_` - Target deque.
2. `bool condition` - Condition for the element to be removed.

#### Returns

1. `true` - If an element was successfully removed from the back of the deque.
2. `false` - If the condition evaluated to false, or if the deque is empty.

## [<span id="deque_front"> \_front() </span>](#deque_function_index)

Returns the front element if the specified deque is not empty.

#### Declaration

> `FMOD V PFX##_front(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `V` - The front element of the deque.
2. The default value, where `V` has all of its bytes set to `0` in case the deque is empty.

## [<span id="deque_back"> \_back() </span>](#deque_function_index)

Returns the rear element if the specified deque is not empty.

#### Declaration

> `FMOD V PFX##_back(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `V` - The rear element of the deque.
2. The default value, where `V` has all of its bytes set to `0` in case the deque is empty.

## [<span id="deque_contains"> \_contains() </span>](#deque_function_index)

Check if an element is present in the deque according to a `comparator` function. This function runs in `O(n)` and might slow down the program execution if the deque has a lot of elements of if this function is called multiple times.

#### Declaration

> `FMOD bool PFX##_contains(SNAME *_deque_, V element, int (*comparator)(V, V));`

#### Parameters

1. `SNAME *_deque_` - Target deque.
2. `V element` - Element to check its presence in the deque.
3. `int (*comparator)(V, V)` - Comparison function. Returns:
    * `-1` - When the first argument is less than the second;
    * `0` - When both arguments are equal;
    * `1` - When the first argument is greater than the second.

#### Returns

1. `true` - If the element is present in the deque.
2. `false` - If the element is not present in the deque.

## [<span id="deque_empty"> \_empty() </span>](#deque_function_index)

Returns true if the deque is empty, otherwise false.

#### Declaration

> `FMOD bool PFX##_empty(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `true` - If the deque is empty.
2. `false` - If there is at least one element in the deque.

## [<span id="deque_full"> \_full() </span>](#deque_function_index)

Returns true if the deque is full, otherwise false. The deque is considered full when its internal buffer is filled up, so the next element added to the deque will required a resizing of the buffer, but note that the deque can grow indefinitely.

#### Declaration

> `FMOD bool PFX##_full(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `true` - If the deque internal buffer is full.
2. `false` - If the deque internal buffer is not full.

## [<span id="deque_count"> \_count() </span>](#deque_function_index)

Returns the amount of elements in the deque.

#### Declaration

> `FMOD size_t PFX##_count(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `size_t` - The amount of elements in the deque.

## [<span id="deque_capacity"> \_capacity() </span>](#deque_function_index)

Returns the internal buffer's current capacity.

#### Declaration

> `FMOD size_t PFX##_capacity(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `size_t` - The internal buffer's current capacity.

## [<span id="deque_iter_new"> \_iter\_new() </span>](#deque_function_index)

Creates a new iterator allocated on the heap and initializes it with a target deque. The iterator's cursor will be positioned at the front of the deque.

#### Declaration

> `FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);`

#### Parameters

1. `SNAME *target` - Target deque.

#### Returns

1. `SNAME##_iter *` - A heap allocated deque iterator.

## [<span id="deque_iter_free"> \_iter\_free() </span>](#deque_function_index)

Frees a heap allocated deque iterator from memory.

#### Declaration

> `FMOD void PFX##_iter_free(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be freed from memory.

## [<span id="deque_iter_init"> \_iter\_init() </span>](#deque_function_index)

Initializes an iterator with a given target deque. The iterator's cursor will be positioned at the front of the deque. The iterator must have been allocated already. This function simply initializes the structure.

#### Declaration

> `FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be initialized.
2. `SNAME *target` - Target deque.

## [<span id="deque_iter_start"> \_iter\_start() </span>](#deque_function_index)

Returns true if the iterator has reached the start (front element) of the deque. If false, the iterator is still possible to iterate to a previous element.

#### Declaration

> `FMOD bool PFX##_iter_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has reached the start of the deque.
2. `false` - If the iterator has not reached the start of the deque.

## [<span id="deque_iter_end"> \_iter\_end() </span>](#deque_function_index)

Returns true if the iterator has reached the end (rear element) of the deque. If false, the iterator is still possible to iterate to a next element.

#### Declaration

> `FMOD bool PFX##_iter_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has reached the end of the deque.
2. `false` - If the iterator has not reached the end of the deque.

## [<span id="deque_iter_to_start"> \_iter\_to\_start() </span>](#deque_function_index)

Moves the cursor of the target iterator to the start (front element) of the deque.

#### Declaration

> `FMOD void PFX##_iter_to_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

## [<span id="deque_iter_to_end"> \_iter\_to\_end() </span>](#deque_function_index)

Moves the cursor of the target iterator to the end (rear element) of the deque.

#### Declaration

> `FMOD void PFX##_iter_to_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

## [<span id="deque_iter_next"> \_iter\_next() </span>](#deque_function_index)

Moves the target deque iterator to the next element if possible.

#### Declaration

> `FMOD bool PFX##_iter_next(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has moved to the next element in the iteration.
2. `false` - If the iterator could not move to the next element, hence reaching the end of the iteration.

## [<span id="deque_iter_prev"> \_iter\_prev() </span>](#deque_function_index)

Moves the target deque iterator to the previous element if possible.

#### Declaration

> `FMOD bool PFX##_iter_prev(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has moved to the previous element in the iteration.
2. `false` - If the iterator could not move to the previous element, hence reaching the start of the iteration.

## [<span id="deque_iter_value"> \_iter\_value() </span>](#deque_function_index)

Returns the element pointed by the target iterator's cursor. The iterator must have been initialized already.

#### Declaration

> `FMOD V PFX##_iter_value(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `V` - The element pointed by the iterator's cursor.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized;
* If the iterator's target is invalid.

## [<span id="deque_iter_rvalue"> \_iter\_rvalue() </span>](#deque_function_index)

Returns a reference to the element pointed by the target iterator's cursor. The iterator must have been initialized already.

#### Declaration

> `FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `V *` - A reference to the element pointed by the iterator's cursor.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized;
* If the iterator's target is invalid.

## [<span id="deque_iter_index"> \_iter\_index() </span>](#deque_function_index)

Returns the current position of the iterator relative to all the elements in the iteration. The index is `0` based and goes up to `count - 1`.

#### Declaration

> `FMOD size_t PFX##_iter_index(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `size_t` - Current position of the iterator relative to all elements in the iteration.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized.

# [HashMap](#collections_index)

A HashMap is an associative container that maps a key `K` to a value `V` containing only unique keys. The value is only accessible through the key. The keys are not sorted, unlike a TreeMap. The main advantage of this container is that lookups are almost instantaneous, requiring very few comparisons (at least one) to get the value corresponding to a key.

HashMaps are one of the most used data structures as they are very useful as lookup tables.

# [HashSet](#collections_index)

A HashSet is an unordered collection of unique elements implemented as a hash-table. Searching for elements in this collection is almost instantaneous, requiring very few comparisons (at least one) to check if a certain element is present the the set.

# [Heap](#collections_index)

A heap is a nearly complete binary tree backed by a growable array. The first element of the array (or the root element of the tree) is the highest/lowest element. The min-heap is commonly used as a priority-queue and a max-heap is commonly used to sort an array of elements.

# [LinkedList](#collections_index)

A list where each element is linked by nodes. Elements can be added and removed from both ends of the list and also in the middle. The linked list can be used to implement other data structures like queues, stacks and deques.

# [List](#collections_index)

A List is a Dynamic Array that store its elements contiguously. It has an internal buffer that is pre-allocated with a greater capacity than necessary in order to foresee new elements being added. The items can be added at both ends of the list and at the middle (with a given index).

Insertions at the start are more costly as it is necessary to shift all elements one position to the right to give space to a new element being added at index 0. Insertions at the end are instantaneous as long as there is enough space in the buffer. Insertions at the middle will be based on an index. The index represents where the element being added will be located once the operation ends. This operation might also need to shift elements. It is also possible to add arrays of elements in the list.

When the buffer is filled, it is reallocated with a greater capacity, usually being doubled.

Removing elements follows the same principle. Removing the front element will require to shift all other elements one position to the left, thus being slower than removing from the end in which is done in constant time. Removing elements in the middle of the list will also require shifting elements to the left. Is is also possible to remove a range of elements or extract them, creating a new list with the removed items.

The iterator is a simple structure that is capable of going back and forwards. Any modifications to the target list during iteration is considered undefined behavior. Its sole purpose is to facilitate navigation through a list.

### Generation Macro

* `LIST_GENERATE(PFX, SNAME, FMOD, V)`
    * `PFX` - Functions namespace or prefix.
    * `SNAME` - Structure name.
    * `FMOD` - Function modifier (static or empty).
    * `V` - Element type.

### Defined Structures

* `struct SNAME##_s` - Structure Name (represents a list structure)
    * `V *buffer` - Internal storage.
    * `size_t capacity` - Storage capacity.
    * `size_t count` - Total elements in the list.
    * `it_start` - Function that returns an iterator to the start of the list.
        * Full definition: `struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);`.
    * `it_end` - Function that returns an iterator to the end of the list.
        * Full definition: `struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);`.

* `struct SNAME##_iter_s` - Structure Iterator (represents a list iterator)
    * `struct SNAME##_s *target` - List being iterated over.
    * `size_t cursor` - Index pointing to the next or previous element in the iteration.
    * `bool start` - If the iterator reached the start of the list.
    * `bool end` - If the iterator reached the end of the list.

### Typedefs

* `typedef struct SNAME##_s SNAME`
* `typedef struct SNAME##_s *SNAME##_ptr`
* `typedef struct SNAME##_iter_s SNAME##_iter`
* `typedef struct SNAME##_iter_s *SNAME##_iter_ptr`

### <span id="list_function_index"> Defined Functions </span>

#### Collection Allocation and Deallocation

* [\_new()](#list_new)
* [\_new\_from()](#list_new_from)
* [\_clear()](#list_clear)
* [\_free()](#list_free)

#### Input and Output

* [\_push\_front()](#list_push_front)
* [\_push()](#list_push)
* [\_push\_back()](#list_push_back)
* [\_pop\_front()](#list_pop_front)
* [\_pop()](#list_pop)
* [\_pop\_back()](#list_pop_back)

#### Conditional Input and Output

* [\_push\_if()](#list_push_if)
* [\_pop\_if()](#list_pop_if)

#### Range Input and Output

* [\_prepend()](#list_prepend)
* [\_insert()](#list_insert)
* [\_append()](#list_append)
* [\_remove()](#list_remove)
* [\_extract()](#list_extract)

#### Elements Access

* [\_front()](#list_front)
* [\_get()](#list_get)
* [\_get_ref()](#list_get_ref)
* [\_back()](#list_back)
* [\_indexof()](#list_indexof)

#### Collection State

* [\_contains()](#list_contains)
* [\_empty()](#list_empty)
* [\_full()](#list_full)
* [\_count()](#list_count)
* [\_fits()](#list_fits)
* [\_capacity()](#list_capacity)

#### Iterator Allocation and Deallocation

* [\_iter\_new()](#list_iter_new)
* [\_iter\_free()](#list_iter_free)

#### Iterator Initialization

* [\_iter\_init()](#list_iter_init)

#### Iterator State

* [\_iter\_start()](#list_iter_start)
* [\_iter\_end()](#list_iter_end)

#### Iterator Movement

* [\_iter\_to_start()](#list_iter_to_start)
* [\_iter\_to_end()](#list_iter_to_end)
* [\_iter\_next()](#list_iter_next)
* [\_iter\_prev()](#list_iter_prev)

#### Iterator Access

* [\_iter\_value()](#list_iter_value)
* [\_iter\_rvalue()](#list_iter_rvalue)
* [\_iter\_index()](#list_iter_index)

## [<span id="list_new"> \_new() </span>](#list_function_index)

Allocates and returns a new list with an internal capacity of the specified value. If allocation fails, `NULL` is returned.

#### Declaration

> `FMOD SNAME *PFX##_new(size_t capacity);`

#### Parameters

1. `size_t capacity` - The initial capacity for the list.

#### Returns

1. `SNAME *` - A pointer to a heap allocated list.
2. `NULL` - If allocation fails.

#### Example

```c
#include "list.h"

// Generate a LIST of type int
LIST_GENERATE(l, list, /* FMOD */, int)

int main(void)
{
    // Make a list of integers with an initial capacity of 100
    // The list and its internal buffer is allocated in the heap
    list *int_list = l_new(100);

    if (!int_list)
        // Allocation failed
        return 1;

    // List is ready to be used

    // Be sure to free its resources later
    l_free(int_list);
}
```

## [<span id="list_new_from"> \_new\_from() </span>](#list_function_index)

Allocates and returns a new list from an already existing array. The list internal buffer's size will match the size of the given array.

#### Declaration

> `FMOD SNAME *PFX##_new_from(V *elements, size_t size);`

#### Parameters

1. `V *elements` - Array of elements to be added to the list.
2. `size_t size` - Size of the array of elements.

#### Returns

1. `SNAME *` - A pointer to a heap allocated list with the given elements.
2. `NULL` - If allocation fails, or if size equals `0`.

#### Undefined Behavior

* If `size` is greater than the element array's actual size.

#### Example

```c
#include "list.h"

// Generate a LIST of type char
LIST_GENERATE(l, list, /* FMOD */, char)

int main(void)
{
    // An array of characters
    const char *letters = "abcdefghijklmnopqrstuvwxyz";

    // Make a list of characters from an already existing array
    // In this case, a string
    list *my_characters = l_new_from(letters, strlen(letters));

    if (!my_characters)
        // Allocation failed
        return 1;

    // Be sure to free its resources after using
    l_free(my_characters);
}
```

## [<span id="list_clear"> \_clear() </span>](#list_function_index)

Removes all elements in the list but does not frees the list structure. After this function the list is empty and can still be used.

#### Declaration

> `FMOD void PFX##_clear(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list to be cleared.

#### Undefined Behavior

* If the list pointer is not a valid memory location.

#### Example

```c
#include "list.h"

// Generate a LIST of type int
LIST_GENERATE(l, list, /* FMOD */, int)

int main(void)
{
    // Make a list of integers with an initial capacity of 100
    list *int_list = l_new(100);

    // Add elements to the list
    for (int i = 0; i < 50; i++)
        l_push_back(int_list, i);

    // empty the list
    l_clear(int_list);
    
    // list now has a size of 0 but the list was not freed from
    // memory and can still be used
    
    // Be sure to free its resources after use
    l_free(int_list);
}
```

## [<span id="list_free"> \_free() </span>](#list_function_index)

Frees from memory the list internal buffer and the structure itself. Note that if the elements inside the list are pointers to allocated memory, this function might cause memory leaks as it does not deals with its elements.

#### Declaration

> `FMOD void PFX##_free(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list to be freed from memory.

#### Undefined Behavior

* If the list pointer is not a valid memory location.

#### Example

```c
#include "list.h"

// Generate a LIST of type int
LIST_GENERATE(l, list, /* FMOD */, int)

int main(void)
{
    // Make a list of integers with an initial capacity of 100
    list *int_list = l_new(100);

    // Operate on the list

    // Free the list from memory
    l_free(int_list);

    // At this point the list is invalid
    // To be able to use it again, a call to l_new() must be made again
}
```

## [<span id="list_push_front"> \_push\_front() </span>](#list_function_index)

Shifts all elements one position to the right and adds an element at the front of the list (index `0`). This function has a high runtime cost and should be avoided when possible.

#### Declaration

> `FMOD bool PFX##_push_front(SNAME *_list_, V element);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `V element` - Element to be added.

#### Returns

1. `true` - If the element was successfully added to the list.
2. `false` - If buffer reallocation failed.

#### Undefined Behavior

* If the list pointer is not a valid memory location.

#### Example

```c
#include "list.h"

// Generate a LIST of type int
LIST_GENERATE(l, list, /* FMOD */, int)

int main(void)
{
    // Make a list of integers with an initial capacity of 100
    list *int_list = l_new(100);

    // For loop to add 200 elements in the list
    for (int i = 0; i < 200; i++)
    {
        // This function is really slow as it needs to shift all the elements
        // one position to the right and then add the new one at index 0
        if (!l_push_front(int_list, i))
        {
            // The element i could not be added because the buffer reallocation
            // failed.
        }
    }

    // Free the list from memory
    l_free(int_list);
}
```

## [<span id="list_push"> \_push() </span>](#list_function_index)

Adds an element at a given index in the list. After it is successfully added to the list, the element is accessible at the index passed to the function.

#### Declaration

> `FMOD bool PFX##_push(SNAME *_list_, V element, size_t index);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `V element` - Element to be added.
3. `size_t index` - Index to add the new element.
    * Can't be greater than the list `count`.

#### Returns

1. `true` - If the element was successfully added to the list.
2. `false` - If buffer reallocation failed, or if `index` is greater than the list `count`.

#### Example

```c
#include "list.h"

// Generate a LIST of type int
LIST_GENERATE(l, list, /* FMOD */, int)

int main(void)
{
    // Make a list of integers with an initial capacity of 100
    list *int_list = l_new(100);

    // For loop to add 200 elements in the list
    for (int i = 0; i < 200; i++)
    {
        // Add new elements at the middle of the list
        // This function is also quite expensive. It will require, on average,
        // to shift (n / 2) elements to the right
        if (!l_push(int_list, i, l_count(int_list) / 2))
        {
            // The element i could not be added because the buffer reallocation
            // failed or the index is greater than the list count of elements
        }
    }

    // Free the list from memory
    l_free(int_list);
}
```

## [<span id="list_push_back"> \_push\_back() </span>](#list_function_index)

Adds an element at the back of the list (index `count` where count is the amount of elements in the list). No shifts are required.

#### Declaration

> `FMOD bool PFX##_push_back(SNAME *_list_, V element);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `V element` - Element to be added.

#### Returns

1. `true` - If the element was successfully added to the list.
2. `false` - If buffer reallocation failed.

#### Example

```c
#include "list.h"

// Generate a LIST of type int
LIST_GENERATE(l, list, /* FMOD */, int)

int main(void)
{
    // Make a list of integers with an initial capacity of 100
    list *int_list = l_new(100);

    // For loop to add 200 elements in the list
    for (int i = 0; i < 200; i++)
    {
        // Adds a new element to the end of the list. This is the fastest way
        // to add an element to the list. If the list is not full, this
        // operation is done instantly
        if (!l_push_back(int_list, i)
        {
            // The element i could not be added because the buffer reallocation
            // failed.
        }
    }

    // Free the list from memory
    l_free(int_list);
}
```

## [<span id="list_pop_front"> \_pop\_front() </span>](#list_function_index)

Removes the first element in the list (located at index `0`) and shifts all remaining elements to the left.

#### Declaration

> `FMOD bool PFX##_pop_front(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `true` - If the element was successfully removed from the list.
2. `false` - If the list is empty.

## [<span id="list_pop"> \_pop() </span>](#list_function_index)

Removes the element located at a given index and shifts all the elements to its right to the left.

#### Declaration

> `FMOD bool PFX##_pop(SNAME *_list_, size_t index);`

#### Parameters

1. `SNAME *_list_` - Target list.
3. `size_t index` - The index of the element to be removed.
    * Can't be greater than or equal to the list `count`.

#### Returns

1. `true` - If the element was successfully removed from the list.
2. `false` - If the list is empty, or if `index` is greater than or equal to the list `count`.

## [<span id="list_pop_back"> \_pop\_back() </span>](#list_function_index)

Removes the last element in the list (located at index `count - 1`). No shifts are required.

#### Declaration

> `FMOD bool PFX##_pop_back(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `true` - If the element was successfully removed from the list.
2. `false` - If the list is empty.

## [<span id="list_push_if"> \_push\_if() </span>](#list_function_index)

Adds an element at a given index if the condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_push_if(SNAME *_list_, V element, size_t index, bool condition);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `V element` - Element to be added.
3. `size_t index` - Index to add the new element.
    * Can't be greater than the list `count`.
4. `bool condition` - Condition for the element to be added.

#### Returns

1. `true` - If the element was successfully added to the list.
2. `false` - If the condition evaluated to false, if `index` is greater than the list `count`, or if buffer reallocation failed.

## [<span id="list_pop_if"> \_pop\_if() </span>](#list_function_index)

Removes an element from a given index if the condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_pop_if(SNAME *_list_, size_t index, bool condition);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `size_t index` - The index of the element to be removed.
    * Can't be greater than or equal to the list `count`.
3. `bool condition` - Condition for the element to be removed.

#### Returns

1. `true` - If the element was successfully removed from the list.
2. `false` - If the condition evaluated to false, if the list is empty, or if the index is greater than or equal to the list `count`.

## [<span id="list_prepend"> \_prepend() </span>](#list_function_index)

Adds to the front of the list an array of elements with a given size.

#### Declaration

> `FMOD bool PFX##_prepend(SNAME *_list_, V *elements, size_t size);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `V *elements` - Array of elements to be added.
3. `size_t size` - The size of the array of elements.

#### Returns

1. `true` - If the elements were successfully added to the list.
2. `false` - If buffer reallocation failed, or if `size` parameter equals `0`.

## [<span id="list_insert"> \_insert() </span>](#list_function_index)

Adds to the specified list an array of elements with a given size to a specific index. The first element of the array of elements will be positioned at that index.

#### Declaration

> `FMOD bool PFX##_insert(SNAME *_list_, V *elements, size_t size, size_t index);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `V *elements` - Array of elements to be added.
3. `size_t size` - The size of the array of elements.
4. `size_t index` - Where the array of elements are to be inserted.

#### Returns

1. `true` - If the elements were successfully added to the list.
2. `false` - If buffer reallocation failed, if `size` parameter equals `0`, or if the index is greater than `count`.

## [<span id="list_append"> \_append() </span>](#list_function_index)

Adds to the back of the list an array of elements with a given size.

#### Declaration

> `FMOD bool PFX##_append(SNAME *_list_, V *elements, size_t size);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `V *elements` - Array of elements to be added.
3. `size_t size` - The size of the array of elements.

#### Returns

1. `true` - If the elements were successfully added to the list.
2. `false` - If buffer reallocation failed, or if `size` parameter equals `0`.

## [<span id="list_remove"> \_remove() </span>](#list_function_index)

Removes a range of elements starting at `from` up to `to`. Both indexes are inclusive.

#### Declaration

> `FMOD bool PFX##_remove(SNAME *_list_, size_t from, size_t to);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `size_t from` - Location of the first element.
3. `size_t to` - Location of the last element.

#### Returns

1. `true` - If the elements were successfully removed from the list.
2. `false` - If `from` is greater than `to`, or if `to` is greater than or equal to `count`.

## [<span id="list_extract"> \_extract() </span>](#list_function_index)

Removes a range of elements starting at `from` up to `to` and returns a new list containing the removed elements. Both indexes are inclusive.

#### Declaration

> `FMOD SNAME *PFX##_extract(SNAME *_list_, size_t from, size_t to);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `size_t from` - Location of the first element.
3. `size_t to` - Location of the last element.

#### Returns

1. `SNAME *` - A new heap allocated list containing the removed elements.
2. `NULL` - If `from` is greater than `to`, or if `to` is greater than or equal to `count`.

## [<span id="list_front"> \_front() </span>](#list_function_index)

Returns the first element (located at index `0`) if the list is not empty.

#### Declaration

> `FMOD V PFX##_front(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `V` - The first element in the list.
2. `0` or `NULL` - If the list is empty.

## [<span id="list_get"> \_get() </span>](#list_function_index)

Returns the element located at the given index if the list is not empty.

#### Declaration

> `FMOD V PFX##_get(SNAME *_list_, size_t index);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `size_t index` - Index to retrieve an element.
    * Can't be greater than or equal to the list `count`.

#### Returns

1. `V` - The element in the list located at the given index.
2. `0` or `NULL` - If the list is empty, or if `index` is greater than or equal to the list `count`.

## [<span id="list_get_ref"> \_get\_ref() </span>](#list_function_index)

Returns a reference to the element located at the given index if the list is not empty.

#### Declaration

> `FMOD V *PFX##_get_ref(SNAME *_list_, size_t index);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `size_t index` - Index to retrieve a reference to an element.
    * Can't be greater than or equal to the list `count`.

#### Returns

1. `V *` - A reference to the element in the list located at the given index.
2. `NULL` - If the list is empty, or if `index` is greater than or equal to the list `count`.

## [<span id="list_back"> \_back() </span>](#list_function_index)

Returns the last element (located at index `count - 1`) if the list is not empty.

#### Declaration

> `FMOD V PFX##_back(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `V` - The last element in the list.
2. `0` or `NULL` - If the list is empty.

## [<span id="list_indexof"> \_indexof() </span>](#list_function_index)

Returns the index at which a given element can be found in the list. Use a comparison function that returns `0` when the element in the list equals the one passed in by argument. Use the flag `from_start` as `true` if you want to find the index of the first matching element or `false` if you want to find the last index of a matching element. Returns `count` if the element is not present in the list.

#### Declaration

> `FMOD size_t PFX##_indexof(SNAME *_list_, V element, int (*comparator)(V, V), bool from_start);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `V element` - Element to be located in the list.
3. `int (*comparator)(V, V)` - Comparison function. Returns:
    * `-1` - When the first argument is less than the second;
    * `0` - When both arguments are equal;
    * `1` - When the first argument is greater than the second.
4. `bool from_start` - Flag that when true, the function will search for the first occurrence of the given element and when false, will search for the last occurrence.

#### Returns

1. `size_t` - The index of the given element according to `comparator` or returns `count` if the element is not present in the given list.

## [<span id="list_contains"> \_contains() </span>](#list_function_index)

Check if an element is present in the list according to a `comparator` function.

#### Declaration

> `FMOD bool PFX##_contains(SNAME *_list_, V element, int (*comparator)(V, V));`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `V element` - Element to check its presence in the list.
3. `int (*comparator)(V, V)` - Comparison function. Returns:
    * `-1` - When the first argument is less than the second;
    * `0` - When both arguments are equal;
    * `1` - When the first argument is greater than the second.

#### Returns

1. `true` - If the element is present in the list.
2. `false` - If the element is not present in the list.

## [<span id="list_empty"> \_empty() </span>](#list_function_index)

Returns true if the list is empty, otherwise false.

#### Declaration

> `FMOD bool PFX##_empty(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `true` - If the list is empty.
2. `false` - If there is at least one element in the list.

## [<span id="list_full"> \_full() </span>](#list_function_index)

Returns true if the list is full, otherwise false. The list is considered full when its internal buffer is filled up, so the next element added to the list will required a resizing of the buffer, but note that the list can grow indefinitely.

#### Declaration

> `FMOD bool PFX##_full(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `true` - If the list internal buffer is full.
2. `false` - If the list internal buffer is not full.

## [<span id="list_count"> \_count() </span>](#list_function_index)

Returns the amount of elements in the list.

#### Declaration

> `FMOD size_t PFX##_count(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `size_t` - The amount of elements in the list.

## [<span id="list_fits"> \_fits() </span>](#list_function_index)

Returns true if a given size fits into the list internal buffer without triggering it to be reallocated.

#### Declaration

> `FMOD bool PFX##_fits(SNAME *_list_, size_t size);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `size_t size` - Size of anything.

#### Returns

1. `true` - If the given size fits into the internal buffer of the list.
2. `false` - If the given size doesn't fit into the internal buffer of the list.

## [<span id="list_capacity"> \_capacity() </span>](#list_function_index)

Returns the internal buffer's current capacity.

#### Declaration

> `FMOD size_t PFX##_capacity(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `size_t` - The internal buffer's current capacity.

## [<span id="list_iter_new"> \_iter\_new() </span>](#list_function_index)

Creates a new iterator allocated on the heap and initializes it with a target list. The iterator's cursor will be positioned at the beginning of the list.

#### Declaration

> `FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);`

#### Parameters

1. `SNAME *target` - Target list.

#### Returns

1. `SNAME##_iter *` - A heap allocated list iterator.

## [<span id="list_iter_free"> \_iter\_free() </span>](#list_function_index)

Frees a heap allocated list iterator from memory.

#### Declaration

> `FMOD void PFX##_iter_free(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be freed from memory.

## [<span id="list_iter_init"> \_iter\_init() </span>](#list_function_index)

Initializes an iterator with a given target list. The iterator's cursor will be positioned at the beginning of the list. The iterator must have been allocated already. This function simply initializes the structure.

#### Declaration

> `FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be initialized.
2. `SNAME *target` - Target list.

## [<span id="list_iter_start"> \_iter\_start() </span>](#list_function_index)

Returns true if the iterator has reached the start of the list. If false, the iterator is still possible to iterate to a previous element.

#### Declaration

> `FMOD bool PFX##_iter_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has reached the start of the list.
2. `false` - If the iterator has not reached the start of the list.

## [<span id="list_iter_end"> \_iter\_end() </span>](#list_function_index)

Returns true if the iterator has reached the end of the list. If false, the iterator is still possible to iterate to a next element.

#### Declaration

> `FMOD bool PFX##_iter_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has reached the end of the list.
2. `false` - If the iterator has not reached the end of the list.

## [<span id="list_iter_to_start"> \_iter\_to\_start() </span>](#list_function_index)

Moves the cursor of the target iterator to the start of the list.

#### Declaration

> `FMOD void PFX##_iter_to_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

## [<span id="list_iter_to_end"> \_iter\_to\_end() </span>](#list_function_index)

Moves the cursor of the target iterator to the end of the list.

#### Declaration

> `FMOD void PFX##_iter_to_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

## [<span id="list_iter_next"> \_iter\_next() </span>](#list_function_index)

Moves the target list iterator to the next element if possible.

#### Declaration

> `FMOD bool PFX##_iter_next(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has moved to the next element in the iteration.
2. `false` - If the iterator could not move to the next element, hence reaching the end of the iteration.

## [<span id="list_iter_prev"> \_iter\_prev() </span>](#list_function_index)

Moves the target list iterator to the previous element if possible.

#### Declaration

> `FMOD bool PFX##_iter_prev(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has moved to the previous element in the iteration.
2. `false` - If the iterator could not move to the previous element, hence reaching the start of the iteration.

## [<span id="list_iter_value"> \_iter\_value() </span>](#list_function_index)

Returns the element pointed by the target iterator's cursor. The iterator must have been initialized already.

#### Declaration

> `FMOD V PFX##_iter_value(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `V` - The element pointed by the iterator's cursor.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized;
* If the iterator's target is invalid.

## [<span id="list_iter_rvalue"> \_iter\_rvalue() </span>](#list_function_index)

Returns a reference to the element pointed by the target iterator's cursor. The iterator must have been initialized already.

#### Declaration

> `FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `V *` - A reference to the element pointed by the iterator's cursor.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized;
* If the iterator's target is invalid.

## [<span id="list_iter_index"> \_iter\_index() </span>](#list_function_index)

Returns the current position of the iterator relative to all the elements in the iteration. The index is `0` based and goes up to `count - 1`.

#### Declaration

> `FMOD size_t PFX##_iter_index(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `size_t` - Current position of the iterator relative to all elements in the iteration.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized.

# [Queue](#collections_index)

A queue is a First-In First-out (or Last-in Last-out) data structure. It is a Dynamic Circular Array where elements are added from one end of the array and removed from the other end. The circular array here (also known as circular buffer or ring buffer) is very important so that both adding and removing elements from the queue are done instantly. The array is linear but with the modulo operator it is treated as a circular sequence of elements.

If the queue was implemented as a regular Dynamic Array, when adding or removing an element at the front, it would be necessary to shift all elements currently present in the queue and this would add up a lot of computing time. Shifting `100000` elements in memory by one position every time an element is added to the queue is simply not efficient.

The queue has two ends. The `front` and `back`. In this implementation all elements are added to the back of the queue and removed from the front, which is more or less how queues work in real life. Unlike a Stack that only has operations at one end of the buffer, the queue needs to be implemented as a circular array in order to quickly add or remove elements.

The queue has three main functions: `enqueue` which adds an element to the queue; `dequeue` which removes an element from the queue; and `peek` which return the element at the front of the queue, that is, the next element to be removed from it.

The queue is used in many applications where a resource is shared among multiple consumers and the queue is responsible for scheduling the access to the resource.

### Generation Macro

* `QUEUE_GENERATE(PFX, SNAME, FMOD, V)`
    * `PFX` - Functions namespace or prefix.
    * `SNAME` - Structure name.
    * `FMOD` - Function modifier (static or empty).
    * `V` - Element type.

### Defined Structures

* `struct SNAME##_s` - Structure Name (represents a queue structure)
    * `V *buffer` - Internal storage.
    * `size_t capacity` - Storage capacity.
    * `size_t count` - Total elements in the queue.
    * `size_t front` - Front element index.
    * `size_t rear` - Rear element index.
    * `it_start` - Function that returns an iterator to the start of the queue.
        * Full definition: `struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);`.
    * `it_end` - Function that returns an iterator to the end of the queue.
        * Full definition: `struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);`.

* `struct SNAME##_iter_s` - Structure Iterator (represents a queue iterator)
    * `struct SNAME##_s *target` - Queue being iterated over.
    * `size_t cursor` - Index pointing to the next or previous element in the iteration.
    * `size_t index` - Relative index to all elements in the iteration.
    * `bool start` - If the iterator reached the start of the queue.
    * `bool end` - If the iterator reached the end of the queue.

### Typedefs

* `typedef struct SNAME##_s SNAME`
* `typedef struct SNAME##_s *SNAME##_ptr`
* `typedef struct SNAME##_iter_s SNAME##_iter`
* `typedef struct SNAME##_iter_s *SNAME##_iter_ptr`

### <span id="queue_function_index"> Defined Functions </span>

#### Collection Allocation and Deallocation

* [\_new()](#queue_new)
* [\_clear()](#queue_clear)
* [\_free()](#queue_free)

#### Input and Output

* [\_enqueue()](#queue_enqueue)
* [\_dequeue()](#queue_dequeue)
* [\_enqueue\_if()](#queue_enqueue_if)
* [\_dequeue\_if()](#queue_dequeue_if)

#### Elements Access

* [\_peek()](#queue_peek)

#### Collection State

* [\_contains()](#queue_contains)
* [\_empty()](#queue_empty)
* [\_full()](#queue_full)
* [\_count()](#queue_count)
* [\_capacity()](#queue_capacity)

#### Iterator Allocation and Deallocation

* [\_iter\_new()](#queue_iter_new)
* [\_iter\_free()](#queue_iter_free)

#### Iterator Initialization

* [\_iter\_init()](#queue_iter_init)

#### Iterator State

* [\_iter\_start()](#queue_iter_start)
* [\_iter\_end()](#queue_iter_end)

#### Iterator Movement

* [\_iter\_to\_start()](#queue_iter_to_start)
* [\_iter\_to\_end()](#queue_iter_to_end)
* [\_iter\_next()](#queue_iter_next)
* [\_iter\_prev()](#queue_iter_prev)

#### Iterator Access

* [\_iter\_value()](#queue_iter_value)
* [\_iter\_rvalue()](#queue_iter_rvalue)
* [\_iter\_index()](#queue_iter_index)

## [<span id="queue_new"> \_new() </span>](#queue_function_index)

Allocates and returns a new queue with an internal capacity of the specified value. If allocation fails, `NULL` is returned.

#### Declaration

> `FMOD SNAME *PFX##_new(size_t capacity);`

#### Parameters

1. `size_t capacity` - The initial capacity for the queue.

#### Returns

1. `SNAME *` - A pointer to a heap allocated queue.
2. `NULL` - If allocation fails.

## [<span id="queue_clear"> \_clear() </span>](#queue_function_index)

Removes all elements in the queue but does not frees the queue structure.

#### Declaration

> `FMOD void PFX##_clear(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue to be cleared.

## [<span id="queue_free"> \_free() </span>](#queue_function_index)

Frees from memory the queue internal buffer and the structure itself. Note that if the elements inside the queue are pointers to allocated memory, this function might cause memory leaks as it does not deals with its elements.

#### Declaration

> `FMOD void PFX##_free(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue to be freed from memory.

## [<span id="queue_enqueue"> \_enqueue() </span>](#queue_function_index)

Adds an element to the rear of the queue.

#### Declaration

> `FMOD bool PFX##_enqueue(SNAME *_queue_, V element);`

#### Parameters

1. `SNAME *_queue_` - Target queue.
2. `V element` - Element to be added.

#### Returns

1. `true` - If the element was successfully added to the rear of the queue.
2. `false` - If buffer reallocation failed.

## [<span id="queue_dequeue"> \_dequeue() </span>](#queue_function_index)

Removes an element at the front of the queue.

#### Declaration

> `FMOD bool PFX##_dequeue(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `true` - If the front element was successfully removed from the queue.
2. `false` - If the queue is empty.

## [<span id="queue_enqueue_if"> \_enqueue\_if() </span>](#queue_function_index)

Adds an element to the rear of the queue if the condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_enqueue_if(SNAME *_queue_, V element, bool condition);`

#### Parameters

1. `SNAME *_queue_` - Target queue.
2. `V element` - Element to be added.
3. `bool condition` - Condition for the element to be added.

#### Returns

1. `true` - If the element was successfully added to the rear of the queue.
2. `false` - If the condition evaluated to false, or if buffer reallocation failed.

## [<span id="queue_dequeue_if"> \_dequeue\_if() </span>](#queue_function_index)

Removes an element at the front of the queue if the condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_dequeue_if(SNAME *_queue_, bool condition);`

#### Parameters

1. `SNAME *_queue_` - Target queue.
2. `bool condition` - Condition for the element to be removed.

#### Returns

1. `true` - If the front element was successfully removed from the queue.
2. `false` - If the condition evaluated to false, or if the queue is empty.

## [<span id="queue_peek"> \_peek() </span>](#queue_function_index)

Returns the element at the front of the queue if available. This represents the next element to be removed from the queue.

#### Declaration

> `FMOD V PFX##_peek(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `V` - The element at the front of the queue.
2. `0` or `NULL` - If the queue is empty.

## [<span id="queue_contains"> \_contains() </span>](#queue_function_index)

Check if an element is present in the queue according to a `comparator` function.

#### Declaration

> `FMOD bool PFX##_contains(SNAME *_queue_, V element, int (*comparator)(V, V));`

#### Parameters

1. `SNAME *_queue_` - Target queue.
2. `V element` - Element to check its presence in the queue.
3. `int (*comparator)(V, V)` - Comparison function. Returns:
    * `-1` - When the first argument is less than the second;
    * `0` - When both arguments are equal;
    * `1` - When the first argument is greater than the second.

#### Returns

1. `true` - If the element is present in the queue.
2. `false` - If the element is not present in the queue.

## [<span id="queue_empty"> \_empty() </span>](#queue_function_index)

Returns true if the queue is empty, otherwise false.

#### Declaration

> `FMOD bool PFX##_empty(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `true` - If the queue is empty.
2. `false` - If there is at least one element in the queue.

## [<span id="queue_full"> \_full() </span>](#queue_function_index)

Returns true if the queue is full, otherwise false. The queue is considered full when its internal buffer is filled up, so the next element added to the queue will required a resizing of the buffer, but note that the queue can grow indefinitely.

#### Declaration

> `FMOD bool PFX##_full(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `true` - If the queue internal buffer is full.
2. `false` - If the queue internal buffer is not full.

## [<span id="queue_count"> \_count() </span>](#queue_function_index)

Returns the amount of elements in the queue.

#### Declaration

> `FMOD size_t PFX##_count(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `size_t` - The amount of elements in the queue.

## [<span id="queue_capacity"> \_capacity() </span>](#queue_function_index)

Returns the internal buffer's current capacity.

#### Declaration

> `FMOD size_t PFX##_capacity(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `size_t` - The internal buffer's current capacity.

## [<span id="queue_iter_new"> \_iter\_new() </span>](#queue_function_index)

Creates a new iterator allocated on the heap and initializes it with a target queue.

#### Declaration

> `FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);`

#### Parameters

1. `SNAME *target` - Target queue.

#### Returns

1. `SNAME##_iter *` - A heap allocated queue iterator.

## [<span id="queue_iter_free"> \_iter\_free() </span>](#queue_function_index)

Frees a heap allocated queue iterator from memory.

#### Declaration

> `FMOD void PFX##_iter_free(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be freed from memory.

## [<span id="queue_iter_init"> \_iter\_init() </span>](#queue_function_index)

Initializes an iterator with a given target queue. The iterator's cursor will be positioned at the front of the queue. The iterator must have been allocated already. This function simply initializes the structure.

#### Declaration

> `FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be initialized.
2. `SNAME *target` - Target queue.

## [<span id="queue_iter_start"> \_iter\_start() </span>](#queue_function_index)

Returns true if the iterator has reached the start (front element) of the queue. If false, the iterator is still possible to iterate to a previous element.

#### Declaration

> `FMOD bool PFX##_iter_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has reached the start of the queue.
2. `false` - If the iterator has not reached the start of the queue.

## [<span id="queue_iter_end"> \_iter\_end() </span>](#queue_function_index)

Returns true if the iterator has reached the end (rear element) of the queue. If false, the iterator is still possible to iterate to a next element.

#### Declaration

> `FMOD bool PFX##_iter_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has reached the end of the queue.
2. `false` - If the iterator has not reached the end of the queue.

## [<span id="queue_iter_to_start"> \_iter\_to\_start() </span>](#queue_function_index)

Moves the cursor of the target iterator to the start (front element) of the queue.

#### Declaration

> `FMOD void PFX##_iter_to_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

## [<span id="queue_iter_to_end"> \_iter\_to\_end() </span>](#queue_function_index)

Moves the cursor of the target iterator to the end (rear element) of the queue.

#### Declaration

> `FMOD void PFX##_iter_to_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

## [<span id="queue_iter_next"> \_iter\_next() </span>](#queue_function_index)

Moves the target queue iterator to the next element if possible.

#### Declaration

> `FMOD bool PFX##_iter_next(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has moved to the next element in the iteration.
2. `false` - If the iterator could not move to the next element, hence reaching the end of the iteration.

## [<span id="queue_iter_prev"> \_iter\_prev() </span>](#queue_function_index)

Moves the target queue iterator to the previous element if possible.

#### Declaration

> `FMOD bool PFX##_iter_prev(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has moved to the previous element in the iteration.
2. `false` - If the iterator could not move to the previous element, hence reaching the start of the iteration.

## [<span id="queue_iter_value"> \_iter\_value() </span>](#queue_function_index)

Returns the element pointed by the target iterator's cursor. The iterator must have been initialized already.

#### Declaration

> `FMOD V PFX##_iter_value(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `V` - The element pointed by the iterator's cursor.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized;
* If the iterator's target is invalid.

## [<span id="queue_iter_rvalue"> \_iter\_rvalue() </span>](#queue_function_index)

Returns a reference to the element pointed by the target iterator's cursor. The iterator must have been initialized already.

#### Declaration

> `FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `V *` - A reference to the element pointed by the iterator's cursor.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized;
* If the iterator's target is invalid.

## [<span id="queue_iter_index"> \_iter\_index() </span>](#queue_function_index)

Returns the current position of the iterator relative to all the elements in the iteration. The index is `0` based and goes up to `count - 1`.

#### Declaration

> `FMOD size_t PFX##_iter_index(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `size_t` - Current position of the iterator relative to all elements in the iteration.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized.

# [Stack](#collections_index)

A Stack is a Last-in First-out (or First-in Last-out) data structure used in a variety of algorithms. It is a Dynamic Array that can only add or remove its elements at the end of the buffer, in this case, it represents the top of the stack.

It has three main functions: `push` which adds an element at the top of the stack; `pop` which removes the top element from the stack; and `top` which returns the top element without removing it (it is also sometimes called `peek`).

A Stack is used in algorithms like backtracking, depth-first search, expression evaluation, syntax parsing and many more.

### Generation Macro

* `STACK_GENERATE(PFX, SNAME, FMOD, V)`
    * `PFX` - Functions namespace or prefix.
    * `SNAME` - Structure name.
    * `FMOD` - Function modifier (static or empty).
    * `V` - Element type.

### Defined Structures

* `struct SNAME##_s` - Structure Name (represents a stack structure)
    * `V *buffer` - Internal storage.
    * `size_t capacity` - Storage capacity.
    * `size_t count` - Total elements in the stack.
    * `it_start` - Function that returns an iterator to the start of the stack.
        * Full definition: `struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);`.
    * `it_end` - Function that returns an iterator to the end of the stack.
        * Full definition: `struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);`.

* `struct SNAME##_iter_s` - Structure Iterator (represents a stack iterator)
    * `struct SNAME##_s *target` - Stack being iterated over.
    * `size_t cursor` - Index pointing to the next or previous element in the iteration.
    * `bool start` - If the iterator reached the start of the stack.
    * `bool end` - If the iterator reached the end of the stack.

### Typedefs

* `typedef struct SNAME##_s SNAME`
* `typedef struct SNAME##_s *SNAME##_ptr`
* `typedef struct SNAME##_iter_s SNAME##_iter`
* `typedef struct SNAME##_iter_s *SNAME##_iter_ptr`

### <span id="stack_function_index"> Defined Functions </span>

#### Collection Allocation and Deallocation

* [\_new()](#stack_new)
* [\_clear()](#stack_clear)
* [\_free()](#stack_free)

#### Input and Output

* [\_push()](#stack_push)
* [\_pop()](#stack_pop)

#### Conditional Input and Output

* [\_push\_if()](#stack_push_if)
* [\_pop\_if()](#stack_pop_if)

#### Elements Access

* [\_top()](#stack_top)

#### Collection State

* [\_contains()](#stack_contains)
* [\_empty()](#stack_empty)
* [\_full()](#stack_full)
* [\_count()](#stack_count)
* [\_capacity()](#stack_capacity)

#### Iterator Allocation and Deallocation

* [\_iter\_new()](#stack_iter_new)
* [\_iter\_free()](#stack_iter_free)

#### Iterator Initialization

* [\_iter\_init()](#stack_iter_init)

#### Iterator State

* [\_iter\_start()](#stack_iter_start)
* [\_iter\_end()](#stack_iter_end)

#### Iterator Movement

* [\_iter\_to\_start()](#stack_iter_to_start)
* [\_iter\_to\_end()](#stack_iter_to_end)
* [\_iter\_next()](#stack_iter_next)
* [\_iter\_prev()](#stack_iter_prev)

#### Iterator Access

* [\_iter\_value()](#stack_iter_value)
* [\_iter\_rvalue()](#stack_iter_rvalue)
* [\_iter\_index()](#stack_iter_index)

## [<span id="stack_new"> \_new() </span>](#stack_function_index)

Allocates and returns a new stack with an internal capacity of the specified value. If allocation fails, `NULL` is returned.

#### Declaration

> `FMOD SNAME *PFX##_new(size_t capacity);`

#### Parameters

1. `size_t capacity` - The initial capacity for the stack.

#### Returns

1. `SNAME *` - A pointer to a heap allocated stack.
2. `NULL` - If allocation fails.

## [<span id="stack_clear"> \_clear() </span>](#stack_function_index)

Removes all elements in the stack but does not frees the stack structure.

#### Declaration

> `FMOD void PFX##_clear(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack to be cleared.

## [<span id="stack_free"> \_free() </span>](#stack_function_index)

Frees from memory the stack internal buffer and the structure itself. Note that if the elements inside the stack are pointers to allocated memory, this function might cause memory leaks as it does not deals with its elements.

#### Declaration

> `FMOD void PFX##_free(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack to be freed from memory.

## [<span id="stack_push"> \_push() </span>](#stack_function_index)

Adds an element on top of the stack.

#### Declaration

> `FMOD bool PFX##_push(SNAME *_stack_, V element);`

#### Parameters

1. `SNAME *_stack_` - Target stack.
2. `V element` - Element to be added.

#### Returns

1. `true` - If the element was successfully added to the stack.
2. `false` - If buffer reallocation failed.

## [<span id="stack_pop"> \_pop() </span>](#stack_function_index)

Removes the top element from the stack.

#### Declaration

> `FMOD bool PFX##_pop(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `true` - If an element was successfully removed from the stack.
2. `false` - If the stack is empty.

## [<span id="stack_push_if"> \_push\_if() </span>](#stack_function_index)

Adds an element on top of the stack if the condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_push_if(SNAME *_stack_, V element, bool condition);`

#### Parameters

1. `SNAME *_stack_` - Target stack.
2. `V element` - Element to be added.
3. `bool condition` - Condition for the element to be added.

#### Returns

1. `true` - If the element was successfully added to the stack.
2. `false` - If the condition evaluated to false, or if buffer reallocation failed.

## [<span id="stack_pop_if"> \_pop\_if() </span>](#stack_function_index)

Removes the top element from the stack if the condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_pop_if(SNAME *_stack_, bool condition);`

#### Parameters

1. `SNAME *_stack_` - Target stack.
2. `bool condition` - Condition for the element to be removed.

#### Returns

1. `true` - If the element was successfully added to the stack.
2. `false` - If the condition evaluated to false, or if the stack is empty.

## [<span id="stack_top"> \_top() </span>](#stack_function_index)

Returns the element at the top of the stack if available.

#### Declaration

> `FMOD V PFX##_top(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `V` - The element at the top of the stack.
2. `0` or `NULL` - If the stack is empty.

## [<span id="stack_contains"> \_contains() </span>](#stack_function_index)

Check if an element is present in the stack according to a `comparator` function.

#### Declaration

> `FMOD bool PFX##_contains(SNAME *_stack_, V element, int (*comparator)(V, V));`

#### Parameters

1. `SNAME *_stack_` - Target stack.
2. `V element` - Element to check its presence in the stack.
3. `int (*comparator)(V, V)` - Comparison function. Returns:
    * `-1` - When the first argument is less than the second;
    * `0` - When both arguments are equal;
    * `1` - When the first argument is greater than the second.

#### Returns

1. `true` - If the element is present in the stack.
2. `false` - If the element is not present in the stack.

## [<span id="stack_empty"> \_empty() </span>](#stack_function_index)

Returns true if the stack is empty, otherwise false.

#### Declaration

> `FMOD bool PFX##_empty(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `true` - If the stack is empty.
2. `false` - If there is at least one element in the stack.

## [<span id="stack_full"> \_full() </span>](#stack_function_index)

Returns true if the stack is full, otherwise false. The stack is considered full when its internal buffer is filled up, so the next element added to the stack will required a resizing of the buffer, but note that the stack can grow indefinitely.

#### Declaration

> `FMOD bool PFX##_full(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `true` - If the stack internal buffer is full.
2. `false` - If the stack internal buffer is not full.

## [<span id="stack_count"> \_count() </span>](#stack_function_index)

Returns the amount of elements in the stack.

#### Declaration

> `FMOD size_t PFX##_count(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `size_t` - The amount of elements in the stack.

## [<span id="stack_capacity"> \_capacity() </span>](#stack_function_index)

Returns the internal buffer's current capacity.

#### Declaration

> `FMOD size_t PFX##_capacity(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `size_t` - The internal buffer's current capacity.

## [<span id="stack_iter_new"> \_iter\_new() </span>](#stack_function_index)

Creates a new iterator allocated on the heap and initializes it with a target stack. The iterator's cursor will be positioned at the top of the stack.

#### Declaration

> `FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);`

#### Parameters

1. `SNAME *target` - Target stack.

#### Returns

1. `SNAME##_iter *` - A heap allocated stack iterator.

## [<span id="stack_iter_free"> \_iter\_free() </span>](#stack_function_index)

Frees a heap allocated stack iterator from memory.

#### Declaration

> `FMOD void PFX##_iter_free(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be freed from memory.

## [<span id="stack_iter_init"> \_iter\_init() </span>](#stack_function_index)

Initializes an iterator with a given target stack. The iterator's cursor will be positioned at the top of the stack. The iterator must have been allocated already. This function simply initializes the structure.

#### Declaration

> `FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be initialized.
2. `SNAME *target` - Target stack.

## [<span id="stack_iter_start"> \_iter\_start() </span>](#stack_function_index)

Returns true if the iterator has reached the start (top element) of the stack. If false, the iterator is still possible to iterate to a previous element.

#### Declaration

> `FMOD bool PFX##_iter_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has reached the start of the stack.
2. `false` - If the iterator has not reached the start of the stack.

## [<span id="stack_iter_end"> \_iter\_end() </span>](#stack_function_index)

Returns true if the iterator has reached the end (bottom element) of the stack. If false, the iterator is still possible to iterate to a next element.

#### Declaration

> `FMOD bool PFX##_iter_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has reached the end of the stack.
2. `false` - If the iterator has not reached the end of the stack.

## [<span id="stack_iter_to_start"> \_iter\_to\_start() </span>](#stack_function_index)

Moves the cursor of the target iterator to the start (top element) of the stack.

#### Declaration

> `FMOD void PFX##_iter_to_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

## [<span id="stack_iter_to_end"> \_iter\_to\_end() </span>](#stack_function_index)

Moves the cursor of the target iterator to the end (bottom element) of the stack.

#### Declaration

> `FMOD void PFX##_iter_to_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

## [<span id="stack_iter_next"> \_iter\_next() </span>](#stack_function_index)

Moves the target stack iterator to the next element if possible.

#### Declaration

> `FMOD bool PFX##_iter_next(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has moved to the next element in the iteration.
2. `false` - If the iterator could not move to the next element, hence reaching the end of the iteration.

## [<span id="stack_iter_prev"> \_iter\_prev() </span>](#stack_function_index)

Moves the target stack iterator to the previous element if possible.

#### Declaration

> `FMOD bool PFX##_iter_prev(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has moved to the previous element in the iteration.
2. `false` - If the iterator could not move to the previous element, hence reaching the start of the iteration.

## [<span id="stack_iter_value"> \_iter\_value() </span>](#stack_function_index)

Returns the element pointed by the target iterator's cursor. The iterator must have been initialized already.

#### Declaration

> `FMOD V PFX##_iter_value(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `V` - The element pointed by the iterator's cursor.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized;
* If the iterator's target is invalid.

## [<span id="stack_iter_rvalue"> \_iter\_rvalue() </span>](#stack_function_index)

Returns a reference to the element pointed by the target iterator's cursor. The iterator must have been initialized already.

#### Declaration

> `FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `V *` - A reference to the element pointed by the iterator's cursor.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized;
* If the iterator's target is invalid.

## [<span id="stack_iter_index"> \_iter\_index() </span>](#stack_function_index)

Returns the current position of the iterator relative to all the elements in the iteration. The index is `0` based and goes up to `count - 1`.

#### Declaration

> `FMOD size_t PFX##_iter_index(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `size_t` - Current position of the iterator relative to all elements in the iteration.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized.

# [TreeMap](#collections_index)

A TreeMap is an associative container that maps a key `K` to a value `V` containing only unique keys. The value is only accessible through the key. The keys are also sorted, unlike a HashMap. The main advantage of this container is that its elements are sorted when using an iterator over the tree and this ordering is based on a comparator function that is passes when the structure is initialized.

The TreeMap is implemented as an AVL tree in order to guarantee a worst case lookup of `O(log n)`.

# [TreeSet](#collections_index)

A TreeSet is an ordered collection of unique elements implemented as a balanced binary tree. A binary tree is a data structure with a root node and each node has two children, often referred as `left` child and `right` child. It is a balanced tree in order to keep the height of the tree as low as possible. The height determines how fast it takes to look up for an element so there are balanced trees that when adding or removing a node from the tree will try, if needed, to balanced itself.

The TreeSet is implemented as an AVL tree in order to guarantee a worst case lookup of `O(log n)`.

### Generation Macro

* `TREESET_GENERATE(PFX, SNAME, FMOD, V)`
    * `PFX` - Functions namespace or prefix.
    * `SNAME` - Structure name.
    * `FMOD` - Function modifier (static or empty).
    * `V` - Element type.

### Defined Structures

* `struct SNAME##_s` - Structure Name (represents a tree-based set structure)
    * `struct SNAME##_node_s *root;` - Root node of the balanced binary tree.
    * `size_t count` - Total elements in the set.
    * `int (*cmp)(V, V)` - Comparator function used to sort elements. Returns:
        * `-1` - When the first argument is less than the second;
        * `0` - When both arguments are equal;
        * `1` - When the first argument is greater than the second.

* `struct SNAME##_node_s` - AVL tree node.
    * `V key` - Node's element.
    * `unsigned char height` - Height of this node. Used to balance the tree.
    * `struct SNAME##_node_s *left` - Left subtree of this node.
    * `struct SNAME##_node_s *right` - Right subtree of this node.
    * `struct SNAME##_node_s *parent` - Parent node or `NULL` if this is the root node.
    * `it_start` - Function that returns an iterator to the start of the TreeSet.
        * Full definition: `struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);`.
    * `it_end` - Function that returns an iterator to the end of the TreeSet.
        * Full definition: `struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);`.

* `struct SNAME##_iter_s` - Structure Iterator (represents a binary tree iterator)
    * `struct SNAME##_s *target` - TreeSet being iterated over.
    * `struct SNAME##_node_s *cursor` - Current AVL tree node.
    * `struct SNAME##_node_s *first` - Marks the first AVL tree node in the iteration.
    * `struct SNAME##_node_s *last` - Marks the last AVL tree node in the iteration.
    * `size_t index` - Relative index to all elements in the iteration.
    * `bool start` - If the iterator reached the start of the TreeSet.
    * `bool end` - If the iterator reached the end of the TreeSet.

### Typedefs

* `typedef struct SNAME##_s SNAME`
* `typedef struct SNAME##_s *SNAME##_ptr`
* `typedef struct SNAME##_iter_s SNAME##_iter`
* `typedef struct SNAME##_iter_s *SNAME##_iter_ptr`

### <span id="treeset_function_index"> Defined Functions </span>

#### Collection Allocation and Deallocation

* [\_new()](#treeset_new)
* [\_clear()](#treeset_clear)
* [\_free()](#treeset_free)

#### Input and Output

* [\_insert()](#treeset_insert)
* [\_remove()](#treeset_remove)

#### Conditional Input and Output

* [\_insert\_if()](#treeset_insert_if)
* [\_remove\_if()](#treeset_remove_if)

#### Elements Access

* [\_max()](#treeset_max)
* [\_min()](#treeset_min)

#### Collection State

* [\_contains()](#treeset_contains)
* [\_empty()](#treeset_empty)
* [\_count()](#treeset_count)

#### Set Operations

* [\_union()](#treeset_union)
* [\_intersection()](#treeset_intersection)
* [\_difference()](#treeset_difference)
* [\_symmetric\_difference()](#treeset_symmetric_difference)

#### Iterator Allocation and Deallocation

* [\_iter\_new()](#treeset_iter_new)
* [\_iter\_free()](#treeset_iter_free)

#### Iterator Initialization

* [\_iter\_init()](#treeset_iter_init)

#### Iterator State

* [\_iter\_start()](#treeset_iter_start)
* [\_iter\_end()](#treeset_iter_end)

#### Iterator Movement

* [\_iter\_to\_start()](#treeset_iter_to_start)
* [\_iter\_to\_end()](#treeset_iter_to_end)
* [\_iter\_next()](#treeset_iter_next)
* [\_iter\_prev()](#treeset_iter_prev)

#### Iterator Access

* [\_iter\_value()](#treeset_iter_value)
* [\_iter\_index()](#treeset_iter_index)

## [<span id="treeset_new"> \_new() </span>](#treeset_function_index)

Allocates and returns a new TreeSet with a default comparator that will determine the order of the elements in the set.

#### Declaration

> `FMOD SNAME *PFX##_new(int (*compare)(V, V));`

#### Parameters

1. `int (*compare)(V, V)` - Comparator. Takes two arguments and returns:
    * `-1` - When the first argument is less than the second;
    * `0` - When both arguments are equal;
    * `1` - When the first argument is greater than the second.

#### Returns

1. `SNAME *` - A pointer to a heap allocated TreeSet.
2. `NULL` - If allocation fails.

## [<span id="treeset_clear"> \_clear() </span>](#treeset_function_index)

Removes all elements in the TreeSet, freeing all of its nodes except for the TreeSet structure.

#### Declaration

> `FMOD void PFX##_clear(SNAME *_set_);`

#### Parameters

1. `SNAME *_set_` - TreeSet to be cleared.

## [<span id="treeset_free"> \_free() </span>](#treeset_function_index)

Frees from memory the TreeSet nodes and the structure itself. Note that if the elements inside the TreeSet are pointers to allocated memory, this function might cause memory leaks as it does not deals with its elements.

#### Declaration

> `FMOD void PFX##_free(SNAME *_set_);`

#### Parameters

1. `SNAME *_set_` - TreeSet to be freed from memory.

## [<span id="treeset_insert"> \_insert() </span>](#treeset_function_index)

Inserts a new element to the set. The element is added only if it is not present in the set, otherwise it is ignored.

#### Declaration

> `FMOD bool PFX##_insert(SNAME *_set_, V element);`

#### Parameters

1. `SNAME *_set_` - Target TreeSet.
2. `V` - Element to be added to the set.

#### Returns

1. `true` - If the element was successfully added to the set.
2. `false` - If the element was not added to the set or if node allocation failed.

## [<span id="treeset_remove"> \_remove() </span>](#treeset_function_index)

Removes a matching element from the set using the comparator function.

#### Declaration

> `FMOD bool PFX##_remove(SNAME *_set_, V element);`

#### Parameters

1. `SNAME *_set_` - Target TreeSet.
2. `V` - Element to be removed from the set.

#### Returns

1. `true` - If the element was successfully removed from the set.
2. `false` - If the element was not fount in the set.

## [<span id="treeset_insert_if"> \_insert\_if() </span>](#treeset_function_index)

Inserts a new element to the set if the condition evaluates to true. The element is added only if it is not present in the set, otherwise it is ignored.

#### Declaration

> `FMOD bool PFX##_insert_if(SNAME *_set_, V element, bool condition);`

#### Parameters

1. `SNAME *_set_` - Target TreeSet.
2. `V` - Element to be added to the set.
3. `bool condition` - Condition for the element to be added.

#### Returns

1. `true` - If the element was successfully added to the set.
2. `false` - If the element was not added to the set, if node allocation failed or if the condition evaluated to false.

## [<span id="treeset_remove_if"> \_remove\_if() </span>](#treeset_function_index)

Removes a matching element from the set using the comparator function, only if the give condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_remove_if(SNAME *_set_, V element, bool condition);`

#### Parameters

1. `SNAME *_set_` - Target TreeSet.
2. `V` - Element to be removed from the set.
3. `bool condition` - Condition for the element to be removed.

#### Returns

1. `true` - If the element was successfully removed from the set.
2. `false` - If the element was not fount in the set or if the condition evaluated to false.

## [<span id="treeset_max"> \_max() </span>](#treeset_function_index)

Retrieves the greatest element present in a given set, according to the set comparator function.

#### Declaration

> `FMOD bool PFX##_max(SNAME *_set_, V *value);`

#### Parameters

1. `SNAME *_set_` - Target TreeSet.
2. `V *value` - The resulting, greatest value in the set.

#### Returns

1. `true` - If the set is not empty and a valid value was found.
2. `false` - If the set is empty and no valid value was found.

## [<span id="treeset_min"> \_min() </span>](#treeset_function_index)

Retrieves the smallest element present in a given set, according to the set comparator function.

#### Declaration

> `FMOD bool PFX##_min(SNAME *_set_, V *value);`

#### Parameters

1. `SNAME *_set_` - Target TreeSet.
2. `V *value` - The resulting, smallest value in the set.

#### Returns

1. `true` - If the set is not empty and a valid value was found.
2. `false` - If the set is empty and no valid value was found.

## [<span id="treeset_contains"> \_contains() </span>](#treeset_function_index)

Check if an element is present in the TreeSet according to the set comparator function.

#### Declaration

> `FMOD bool PFX##_contains(SNAME *_set_, V element);`

#### Parameters

1. `SNAME *_set_` - Target TreeSet.
2. `V element` - Element to check its presence in the TreeSet.

#### Returns

1. `true` - If the element is present in the TreeSet.
2. `false` - If the element is not present in the TreeSet.

## [<span id="treeset_empty"> \_empty() </span>](#treeset_function_index)

Returns true if the TreeSet is empty, otherwise false.

#### Declaration

> `FMOD bool PFX##_empty(SNAME *_set_);`

#### Parameters

1. `SNAME *_set_` - Target TreeSet.

#### Returns

1. `true` - If the TreeSet is empty.
2. `false` - If there is at least one element in the TreeSet.

## [<span id="treeset_count"> \_count() </span>](#treeset_function_index)

Returns the amount of elements in the TreeSet.

#### Declaration

> `FMOD size_t PFX##_count(SNAME *_set_);`

#### Parameters

1. `SNAME *_set_` - Target TreeSet.

#### Returns

1. `size_t` - The amount of elements in the TreeSet.

## [<span id="treeset_union"> \_union() </span>](#treeset_function_index)

Creates a new set as the union of two sets, `_set1_` and `_set2_`. The union of two sets is formed by the elements that are present in either one of the sets, or in both.

#### Declaration

> `FMOD SNAME *PFX##_union(SNAME *_set1_, SNAME *_set2_);`

#### Parameters

1. `SNAME *_set1_` - First operand set of union operation.
2. `SNAME *_set2_` - Second operand of set union operation.

#### Returns

1. `SNAME *` - The resulting set representing the union of `_set1_` and `_set2_`.
2. `NULL` - If allocation fails.

## [<span id="treeset_intersection"> \_intersection() </span>](#treeset_function_index)

Creates a new set as the intersection of two sets, `_set1_` and `_set2_`. The intersection of two sets is formed only by the elements that are present in both sets.

#### Declaration

> `FMOD SNAME *PFX##_intersection(SNAME *_set1_, SNAME *_set2_);`

#### Parameters

1. `SNAME *_set1_` - First operand set of intersection operation.
2. `SNAME *_set2_` - Second operand of set intersection operation.

#### Returns

1. `SNAME *` - The resulting set representing the intersection of `_set1_` and `_set2_`.
2. `NULL` - If allocation fails.

## [<span id="treeset_difference"> \_difference() </span>](#treeset_function_index)

Creates a new set as the difference of `_set1_` to `_set2_`. The difference of two sets is formed by the elements that are present in the first set, but not in the second one.

#### Declaration

> `FMOD SNAME *PFX##_difference(SNAME *_set1_, SNAME *_set2_);`

#### Parameters

1. `SNAME *_set1_` - First operand set of difference operation.
2. `SNAME *_set2_` - Second operand set of difference operation.

#### Returns

1. `SNAME *` - The resulting set representing the difference of `_set1_` and `_set2_`.
2. `NULL` - If allocation fails.

## [<span id="treeset_symmetric_difference"> \_symmetric\_difference() </span>](#treeset_function_index)

Creates a new set as the symmetric difference of `_set1_` to `_set2_`. The symmetric difference of two sets is formed by the elements that are present in one of the sets, but not in the other.

#### Declaration

> `FMOD SNAME *PFX##_symmetric_difference(SNAME *_set1_, SNAME *_set2_);`

#### Parameters

1. `SNAME *_set1_` - First operand set of symmetric difference operation.
2. `SNAME *_set2_` - Second operand set of symmetric difference operation.

#### Returns

1. `SNAME *` - The resulting set representing the symmetric difference of `_set1_` and `_set2_`.
2. `NULL` - If allocation fails.

## [<span id="treeset_iter_new"> \_iter\_new() </span>](#treeset_function_index)

Creates a new iterator allocated on the heap and initializes it with a target TreeSet. The iterator's cursor will be positioned at the smallest element node of the TreeSet.

#### Declaration

> `FMOD SNAME##_iter *PFX##_iter_new(SNAME *target);`

#### Parameters

1. `SNAME *target` - Target TreeSet.

#### Returns

1. `SNAME##_iter *` - A heap allocated TreeSet iterator.

## [<span id="treeset_iter_free"> \_iter\_free() </span>](#treeset_function_index)

Frees a heap allocated TreeSet iterator from memory.

#### Declaration

> `FMOD void PFX##_iter_free(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be freed from memory.

## [<span id="treeset_iter_init"> \_iter\_init() </span>](#treeset_function_index)

Initializes an iterator with a given target TreeSet. The iterator's cursor will be positioned at the smallest element node of the TreeSet. The iterator must have been allocated already. This function simply initializes the structure.

#### Declaration

> `FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be initialized.
2. `SNAME *target` - Target TreeSet.

## [<span id="treeset_iter_start"> \_iter\_start() </span>](#treeset_function_index)

Returns true if the iterator has reached the start of the TreeSet (smallest element). If false, the iterator is still possible to iterate to a previous element.

#### Declaration

> `FMOD bool PFX##_iter_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has reached the start of the TreeSet.
2. `false` - If the iterator has not reached the start of the TreeSet.

## [<span id="treeset_iter_end"> \_iter\_end() </span>](#treeset_function_index)

Returns true if the iterator has reached the end of the TreeSet (greatest element). If false, the iterator is still possible to iterate to a next element.

#### Declaration

> `FMOD bool PFX##_iter_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has reached the end of the TreeSet.
2. `false` - If the iterator has not reached the end of the TreeSet.

## [<span id="treeset_iter_to_start"> \_iter\_to\_start() </span>](#treeset_function_index)

Moves the cursor of the target iterator to the start (smallest element) of the TreeSet.

#### Declaration

> `FMOD void PFX##_iter_to_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

## [<span id="treeset_iter_to_end"> \_iter\_to\_end() </span>](#treeset_function_index)

Moves the cursor of the target iterator to the end (greatest element) of the TreeSet.

#### Declaration

> `FMOD void PFX##_iter_to_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

## [<span id="treeset_iter_next"> \_iter\_next() </span>](#treeset_function_index)

Moves the target TreeSet iterator to the next element if possible.

#### Declaration

> `FMOD bool PFX##_iter_next(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has moved to the next element in the iteration.
2. `false` - If the iterator could not move to the next element, hence reaching the end of the iteration.

## [<span id="treeset_iter_prev"> \_iter\_prev() </span>](#treeset_function_index)

Moves the target TreeSet iterator to the previous element if possible.

#### Declaration

> `FMOD bool PFX##_iter_prev(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `true` - If the iterator has moved to the previous element in the iteration.
2. `false` - If the iterator could not move to the previous element, hence reaching the start of the iteration.

## [<span id="treeset_iter_value"> \_iter\_value() </span>](#treeset_function_index)

Returns the element pointed by the target iterator's cursor. The iterator must have been initialized already.

#### Declaration

> `FMOD V PFX##_iter_value(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `V` - The element pointed by the iterator's cursor.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized;
* If the iterator's target is invalid.

## [<span id="treeset_iter_index"> \_iter\_index() </span>](#treeset_function_index)

Returns the current position of the iterator relative to all the elements in the iteration. The index is `0` based and goes up to `count - 1`.

#### Declaration

> `FMOD size_t PFX##_iter_index(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns

1. `size_t` - Current position of the iterator relative to all elements in the iteration.

#### Undefined Behavior

* If the iterator points to `NULL`;
* If the iterator has not been properly initialized.

# [ForEach](#collections_index)

The ForEach macro is an utility that can be used to simplify iterations through a collections. There are two ForEach macros:

* `FOR_EACH` - Iterates over a collection forwards, from the start to the end*.
* `FOR_EACH_REV` - Iterates over a collection backwards, from the end to the start*.

\* Check out the documentation of each collection to know which elements are regarded as the starting point and ending point of an iteration.

### Parameters

* `PFX` - The prefix of the functions of the defined collection.
* `SNAME` - The defined name of your collection
* `TARGET` - The target collection to be iterated over.
* `BODY` - The body of your loop where a local variable called `iter` will be available.

### Example

```c
#include "hashmap.h"
#include "foreach.h" // The ForEach macros are defined here
#include <stdio.h>

// Int hash function
size_t inthash(int t)
{
    size_t a = t;
    a += ~(a << 15);
    a ^= (a >> 10);
    a += (a << 3);
    a ^= (a >> 6);
    a += ~(a << 11);
    a ^= (a >> 16);
    return a;
}

// Int compare function
int intcmp(int a, int b)
{
    return (a > b) - (a < b);
}

// Generate all the code for the hashmap
HASHMAP_GENERATE(hm, hashmap, /* FMOD */, int, double)

int main(void)
{
    // Create a new hashmap with custom hash function
    // The comparison function is only required to know if the key looked for was found
    hashmap *map = hm_new(1000, 0.6, intcmp, inthash);

    // Add keys mapped to a value
    for (int i = 0; i < 5000; i++)
    {
        hm_insert(map, i, (double)i / 1000.0);
    }

    // Iterate over the HashMap without the FOR_EACH macro
    for (hashmap_iter iter = map->it_start(map); !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        printf("MAP[%4d] = %.3lf\n", hm_iter_key(&iter), hm_iter_value(&iter));
    }

    // Iterate over the HashMap using FOR_EACH macro
    FOR_EACH(hm, hashmap, map, {
        // Inside, the variable 'iter' will be available and from it you can access
        // the key, value or index.
        printf("MAP[%4d] = %.3lf\n", hm_iter_key(&iter), hm_iter_value(&iter));
    })

    hm_free(map);

    return 0;
}
```
