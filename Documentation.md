<span id="collections_index"> Collections Documentation </span>

* [List](#List)
* [LinkedList](#LinkedList)
* [Stack](#Stack)
* [Queue](#Queue)
* [Deque](#Deque)
* [Heap](#Heap)
* [TreeSet](#TreeSet)
* [TreeMap](#TreeMap)
* [HashSet](#HashSet)
* [HashMap](#HashMap)

# [List](#collections_index)

A contiguous growable array. Elements can be added and removed from both ends of the array and also in the middle. This list can also be used to implement other data structures like stack, queues and deques, although queues and deques are implemented as a circular buffer, therefore being more efficient.

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
* `struct SNAME##_iter_s` - Structure Iterator (represents a list iterator)
    * `struct SNAME##_s *target` - List being iterated over.
    * `size_t cursor` - Index pointing to the next or previous element in the iteration.
    * `bool start` - If the iterator reached the start of the list.
    * `bool end` - If the iterator reached the end of the list.

### Typedefs

* `typedef struct SNAME##_s SNAME`
* `typedef struct SNAME##_iter_s SNAME##_iter`

### <span id="list_function_index"> Defined Functions </span>

* [\_new()](#list_new)
* [\_clear()](#list_clear)
* [\_free()](#list_free)
* [\_push\_front()](#list_push_front)
* [\_push()](#list_push)
* [\_push\_back()](#list_push_back)
* [\_pop\_front()](#list_pop_front)
* [\_pop()](#list_pop)
* [\_pop\_back()](#list_pop_back)
* [\_push\_if()](#list_push_if)
* [\_pop\_if()](#list_pop_if)
* [\_front()](#list_front)
* [\_get()](#list_get)
* [\_back()](#list_back)
* [\_empty()](#list_empty)
* [\_full()](#list_full)
* [\_count()](#list_count)
* [\_capacity()](#list_capacity)
* [\_iter\_new()](#list_iter_new)
* [\_iter\_start()](#list_iter_start)
* [\_iter\_end()](#list_iter_end)
* [\_iter\_tostart()](#list_iter_tostart)
* [\_iter\_toend()](#list_iter_toend)
* [\_iter\_next()](#list_iter_next)
* [\_iter\_prev()](#list_iter_prev)

## [<span id="list_new"> \_new() </span>](#list_function_index)

Allocates and returns a new list with an internal capacity of `size`. If allocation fails, `NULL` is returned.

#### Declaration

> `FMOD SNAME *PFX##_new(size_t size);`

#### Parameters

1. `size_t size` - The initial capacity for the list.

#### Returns

1. `SNAME *` - A pointer to a heap allocated list.
2. `NULL` - If allocation fails.

#### Complexity

* O(1)

## [<span id="list_clear"> \_clear() </span>](#list_function_index)

Removes all elements in the list but does not frees the list structure.

#### Declaration

> `FMOD void PFX##_clear(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list to be cleared.

#### Complexity

* O(1)

## [<span id="list_free"> \_free() </span>](#list_function_index)

Frees from memory the list internal buffer and the structure itself. Note that if the elements inside the list are pointers to allocated memory, this function might cause memory leaks as it does not deals with its elements.

#### Declaration

> `FMOD void PFX##_free(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list to be freed from memory.

#### Complexity

* O(1)

## [<span id="list_push_front"> \_push\_front() </span>](#list_function_index)

Shifts all elements one position to the right and adds an element at the front of the list (index `0`).

#### Declaration

> `FMOD bool PFX##_push_front(SNAME *_list_, V element);`

#### Parameters

1. `SNAME *_list_` - Target list.
2. `V element` - Element to be added.

#### Returns

1. `true` - If the element was successfully added to the list.
2. `false` - If buffer reallocation failed.

#### Complexity

* O(n)

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

#### Complexity

* O(n)

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

#### Complexity

* O(1)

## [<span id="list_pop_front"> \_pop\_front() </span>](#list_function_index)

Removes the first element in the list (located at index `0`) and shifts all remaining elements to the left.

#### Declaration

> `FMOD bool PFX##_pop_front(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `true` - If the element was successfully removed from the list.
2. `false` - If the list is empty.

#### Complexity

* O(n)

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

#### Complexity

* O(n)

## [<span id="list_pop_back"> \_pop\_back() </span>](#list_function_index)

Removes the last element in the list (located at index `count - 1`). No shifts are required.

#### Declaration

> `FMOD bool PFX##_pop_back(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `true` - If the element was successfully removed from the list.
2. `false` - If the list is empty.

#### Complexity

* O(1)

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

#### Complexity

* O(n)

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

#### Complexity

* O(n)

## [<span id="list_front"> \_front() </span>](#list_function_index)

Returns the first element (located at index `0`) if the list is not empty.

#### Declaration

> `FMOD V PFX##_front(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `V` - The first element in the list.
2. `0` or `NULL` - If the list is empty.

#### Complexity

* O(1)

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

#### Complexity

* O(1)

## [<span id="list_back"> \_back() </span>](#list_function_index)

Returns the last element (located at index `count - 1`) if the list is not empty.

#### Declaration

> `FMOD V PFX##_back(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `V` - The last element in the list.
2. `0` or `NULL` - If the list is empty.

#### Complexity

* O(1)

## [<span id="list_empty"> \_empty() </span>](#list_function_index)

Returns true if the list is empty, otherwise false.

#### Declaration

> `FMOD bool PFX##_empty(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `true` - If the list is empty.
2. `false` - If there is at least one element in the list.

#### Complexity

* O(1)

## [<span id="list_full"> \_full() </span>](#list_function_index)

Returns true if the list is full, otherwise false. The list is considered full when its internal buffer is filled up, so the next element added to the list will required a resizing of the buffer, but note that the list can grow indefinitely.

#### Declaration

> `FMOD bool PFX##_full(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `true` - If the list internal buffer is full.
2. `false` - If the list internal buffer is not full.

#### Complexity

* O(1)

## [<span id="list_count"> \_count() </span>](#list_function_index)

Returns the amount of elements in the list.

#### Declaration

> `FMOD size_t PFX##_count(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `size_t` - The amount of elements in the list.

#### Complexity

* O(1)

## [<span id="list_capacity"> \_capacity() </span>](#list_function_index)

Returns the internal buffer's current capacity.

#### Declaration

> `FMOD size_t PFX##_capacity(SNAME *_list_);`

#### Parameters

1. `SNAME *_list_` - Target list.

#### Returns

1. `size_t` - The internal buffer's current capacity.

#### Complexity

* O(1)

## [<span id="list_iter_new"> \_iter\_new() </span>](#list_function_index)

Initializes an iterator with a given target list. The iterator's cursor will be positioned at the beginning of the list.

#### Declaration

> `FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be initialized.
2. `SNAME *target` - Target list.

#### Complexity

* O(1)

## [<span id="list_iter_start"> \_iter\_start() </span>](#list_function_index)

Returns true if the iterator has reached the start of the list. If false, the iterator is still possible to iterate to a previous element.

#### Declaration

> `FMOD bool PFX##_iter_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns 

1. `true` - If the iterator has reached the start of the list.
2. `false` - If the iterator has not reached the start of the list.

#### Complexity

* O(1)

## [<span id="list_iter_end"> \_iter\_end() </span>](#list_function_index)

Returns true if the iterator has reached the end of the list. If false, the iterator is still possible to iterate to a next element.

#### Declaration

> `FMOD bool PFX##_iter_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns 

1. `true` - If the iterator has reached the end of the list.
2. `false` - If the iterator has not reached the end of the list.

#### Complexity

* O(1)

## [<span id="list_iter_tostart"> \_iter\_tostart() </span>](#list_function_index)

Moves the cursor of the target iterator to the start of the list.

#### Declaration

> `FMOD void PFX##_iter_tostart(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Complexity

* O(1)

## [<span id="list_iter_toend"> \_iter\_toend() </span>](#list_function_index)

Moves the cursor of the target iterator to the end of the list.

#### Declaration

> `FMOD void PFX##_iter_toend(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Complexity

* O(1)

## [<span id="list_iter_next"> \_iter\_next() </span>](#list_function_index)

This function is used to iterate to the next element, retrieving the current one, along with an index that represents how many iterations have passed. When the index is `0` it means that the current result is the first element in the list; if it equals `count - 1` then it is the last element in the list.

#### Declaration

> `FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.
2. `V *result` - Resulting value from the list.
3. `size_t *index` - Resulting index.

#### Returns 

1. `true` - If the iterator has retrieved a valid `result` and `index`.
2. `false` - If the iterator has not retrieved a valid `result` and `index`. Here, iteration to the next element has ended.

#### Complexity

* O(1)

## [<span id="list_iter_prev"> \_iter\_prev() </span>](#list_function_index)

This function is used to iterate to the previous element, retrieving the current one, along with an index that represents how many iterations have passed. When the index is `0` it means that the current result is the first element in the list; if it equals `count - 1` then it is the last element in the list.

#### Declaration

> `FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.
2. `V *result` - Resulting value from the list.
3. `size_t *index` - Resulting index.

#### Returns 

1. `true` - If the iterator has retrieved a valid `result` and `index`.
2. `false` - If the iterator has not retrieved a valid `result` and `index`. Here, iteration to the previous element has ended.

#### Complexity

* O(1)

# [LinkedList](#collections_index)

A list where each element is linked by nodes. Elements can be added and removed from both ends of the list and also in the middle. The linked list can be used to implement other data structures like queues, stacks and deques.

# [Stack](#collections_index)

A LIFO/FILO structure backed by a growable array. All elements are added and removed from the top of the stack.

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
* `struct SNAME##_iter_s` - Structure Iterator (represents a stack iterator)
    * `struct SNAME##_s *target` - Stack being iterated over.
    * `size_t cursor` - Index pointing to the next or previous element in the iteration.
    * `bool start` - If the iterator reached the start of the stack.
    * `bool end` - If the iterator reached the end of the stack.

### Typedefs

* `typedef struct SNAME##_s SNAME`
* `typedef struct SNAME##_iter_s SNAME##_iter`

### <span id="stack_function_index"> Defined Functions </span>

* [\_new()](#stack_new)
* [\_clear()](#stack_clear)
* [\_free()](#stack_free)
* [\_push()](#stack_push)
* [\_pop()](#stack_pop)
* [\_push\_if()](#stack_push_if)
* [\_pop\_if()](#stack_pop_if)
* [\_top()](#stack_top)
* [\_empty()](#stack_empty)
* [\_full()](#stack_full)
* [\_count()](#stack_count)
* [\_capacity()](#stack_capacity)
* [\_iter\_new()](#stack_iter_new)
* [\_iter\_start()](#stack_iter_start)
* [\_iter\_end()](#stack_iter_end)
* [\_iter\_tostart()](#stack_iter_tostart)
* [\_iter\_toend()](#stack_iter_toend)
* [\_iter\_next()](#stack_iter_next)
* [\_iter\_prev()](#stack_iter_prev)

## [<span id="stack_new"> \_new() </span>](#stack_function_index)

Allocates and returns a new stack with an internal capacity of `size`. If allocation fails, `NULL` is returned.

#### Declaration

> `FMOD SNAME *PFX##_new(size_t size);`

#### Parameters

1. `size_t size` - The initial capacity for the stack.

#### Returns

1. `SNAME *` - A pointer to a heap allocated stack.
2. `NULL` - If allocation fails.

#### Complexity

* O(1)

## [<span id="stack_clear"> \_clear() </span>](#stack_function_index)

Removes all elements in the stack but does not frees the stack structure.

#### Declaration

> `FMOD void PFX##_clear(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack to be cleared.

#### Complexity

* O(1)

## [<span id="stack_free"> \_free() </span>](#stack_function_index)

Frees from memory the stack internal buffer and the structure itself. Note that if the elements inside the stack are pointers to allocated memory, this function might cause memory leaks as it does not deals with its elements.

#### Declaration

> `FMOD void PFX##_free(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack to be freed from memory.

#### Complexity

* O(1)

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

#### Complexity

* O(1)

## [<span id="stack_pop"> \_pop() </span>](#stack_function_index)

Removes the top element from the stack.

#### Declaration

> `FMOD bool PFX##_pop(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `true` - If an element was successfully removed from the stack.
2. `false` - If the stack is empty.

#### Complexity

* O(1)

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

#### Complexity

* O(1)

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

#### Complexity

* O(1)

## [<span id="stack_top"> \_top() </span>](#stack_function_index)

Returns the element at the top of the stack if available.

#### Declaration

> `FMOD V PFX##_top(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `V` - The element at the top of the stack.
2. `0` or `NULL` - If the stack is empty.

#### Complexity

* O(1)

## [<span id="stack_empty"> \_empty() </span>](#stack_function_index)

Returns true if the stack is empty, otherwise false.

#### Declaration

> `FMOD bool PFX##_empty(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `true` - If the stack is empty.
2. `false` - If there is at least one element in the stack.

#### Complexity

* O(1)

## [<span id="stack_full"> \_full() </span>](#stack_function_index)

Returns true if the stack is full, otherwise false. The stack is considered full when its internal buffer is filled up, so the next element added to the stack will required a resizing of the buffer, but note that the stack can grow indefinitely.

#### Declaration

> `FMOD bool PFX##_full(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `true` - If the stack internal buffer is full.
2. `false` - If the stack internal buffer is not full.

#### Complexity

* O(1)

## [<span id="stack_count"> \_count() </span>](#stack_function_index)

Returns the amount of elements in the stack.

#### Declaration

> `FMOD size_t PFX##_count(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `size_t` - The amount of elements in the stack.

#### Complexity

* O(1)

## [<span id="stack_capacity"> \_capacity() </span>](#stack_function_index)

Returns the internal buffer's current capacity.

#### Declaration

> `FMOD size_t PFX##_capacity(SNAME *_stack_);`

#### Parameters

1. `SNAME *_stack_` - Target stack.

#### Returns

1. `size_t` - The internal buffer's current capacity.

#### Complexity

* O(1)

## [<span id="stack_iter_new"> \_iter\_new() </span>](#stack_function_index)

Initializes an iterator with a given target stack. The iterator's cursor will be positioned at the top of the stack.

#### Declaration

> `FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be initialized.
2. `SNAME *target` - Target stack.

#### Complexity

* O(1)

## [<span id="stack_iter_start"> \_iter\_start() </span>](#stack_function_index)

Returns true if the iterator has reached the start of the stack (top element). If false, the iterator is still possible to iterate to a previous element.

#### Declaration

> `FMOD bool PFX##_iter_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns 

1. `true` - If the iterator has reached the start of the stack.
2. `false` - If the iterator has not reached the start of the stack.

#### Complexity

* O(1)

## [<span id="stack_iter_end"> \_iter\_end() </span>](#stack_function_index)

Returns true if the iterator has reached the end of the stack (bottom element). If false, the iterator is still possible to iterate to a next element.

#### Declaration

> `FMOD bool PFX##_iter_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns 

1. `true` - If the iterator has reached the end of the stack.
2. `false` - If the iterator has not reached the end of the stack.

#### Complexity

* O(1)

## [<span id="stack_iter_tostart"> \_iter\_tostart() </span>](#stack_function_index)

Moves the cursor of the target iterator to the start (top element) of the stack.

#### Declaration

> `FMOD void PFX##_iter_tostart(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Complexity

* O(1)

## [<span id="stack_iter_toend"> \_iter\_toend() </span>](#stack_function_index)

Moves the cursor of the target iterator to the end of the stack (bottom element).

#### Declaration

> `FMOD void PFX##_iter_toend(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Complexity

* O(1)

## [<span id="stack_iter_next"> \_iter\_next() </span>](#stack_function_index)

This function is used to iterate to the next element, retrieving the current one, along with an index that represents how many iterations have passed. When the index is `0` it means that the current result is the top-most element of the stack; if it equals `count - 1` then it is the bottom-most element of the stack.

#### Declaration

> `FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.
2. `V *result` - Resulting value from the stack.
3. `size_t *index` - Resulting index.

#### Returns 

1. `true` - If the iterator has retrieved a valid `result` and `index`.
2. `false` - If the iterator has not retrieved a valid `result` and `index`. Here, iteration to the next element has ended.

#### Complexity

* O(1)

## [<span id="stack_iter_prev"> \_iter\_prev() </span>](#stack_function_index)

This function is used to iterate to the previous element, retrieving the current one, along with an index that represents how many iterations have passed. When the index is `0` it means that the current result is the top-most element of the stack; if it equals `count - 1` then it is the bottom-most element of the stack.

#### Declaration

> `FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.
2. `V *result` - Resulting value from the stack.
3. `size_t *index` - Resulting index.

#### Returns 

1. `true` - If the iterator has retrieved a valid `result` and `index`.
2. `false` - If the iterator has not retrieved a valid `result` and `index`. Here, iteration to the previous element has ended.

#### Complexity

* O(1)

# [Queue](#collections_index)

A FIFO/LILO structure backed by a circular buffer. Elements are added in one end and removed from the other.

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
* `struct SNAME##_iter_s` - Structure Iterator (represents a queue iterator)
    * `struct SNAME##_s *target` - Queue being iterated over.
    * `size_t cursor` - Index pointing to the next or previous element in the iteration.
    * `size_t index` - How many iterations have passed.
    * `bool start` - If the iterator reached the start of the queue.
    * `bool end` - If the iterator reached the end of the queue.

### Typedefs

* `typedef struct SNAME##_s SNAME`
* `typedef struct SNAME##_iter_s SNAME##_iter`

### <span id="queue_function_index"> Defined Functions </span>

* [\_new()](#queue_new)
* [\_clear()](#queue_clear)
* [\_free()](#queue_free)
* [\_enqueue()](#queue_enqueue)
* [\_dequeue()](#queue_dequeue)
* [\_enqueue\_if()](#queue_enqueue_if)
* [\_dequeue\_if()](#queue_dequeue_if)
* [\_peek()](#queue_peek)
* [\_empty()](#queue_empty)
* [\_full()](#queue_full)
* [\_count()](#queue_count)
* [\_capacity()](#queue_capacity)
* [\_iter\_new()](#queue_iter_new)
* [\_iter\_start()](#queue_iter_start)
* [\_iter\_end()](#queue_iter_end)
* [\_iter\_tostart()](#queue_iter_tostart)
* [\_iter\_toend()](#queue_iter_toend)
* [\_iter\_next()](#queue_iter_next)
* [\_iter\_prev()](#queue_iter_prev)

## [<span id="queue_new"> \_new() </span>](#queue_function_index)

Allocates and returns a new queue with an internal capacity of `size`. If allocation fails, `NULL` is returned.

#### Declaration

> `FMOD SNAME *PFX##_new(size_t size);`

#### Parameters

1. `size_t size` - The initial capacity for the queue.

#### Returns

1. `SNAME *` - A pointer to a heap allocated queue.
2. `NULL` - If allocation fails.

#### Complexity

* O(1)

## [<span id="queue_clear"> \_clear() </span>](#queue_function_index)

Removes all elements in the queue but does not frees the queue structure.

#### Declaration

> `FMOD void PFX##_clear(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue to be cleared.

#### Complexity

* O(1)

## [<span id="queue_free"> \_free() </span>](#queue_function_index)

Frees from memory the queue internal buffer and the structure itself. Note that if the elements inside the queue are pointers to allocated memory, this function might cause memory leaks as it does not deals with its elements.

#### Declaration

> `FMOD void PFX##_free(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue to be freed from memory.

#### Complexity

* O(1)

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

#### Complexity

* O(1)

## [<span id="queue_dequeue"> \_dequeue() </span>](#queue_function_index)

Removes an element at the front of the queue.

#### Declaration

> `FMOD bool PFX##_dequeue(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `true` - If the front element was successfully removed from the queue.
2. `false` - If the queue is empty.

#### Complexity

* O(1)

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

#### Complexity

* O(1)

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

#### Complexity

* O(1)

## [<span id="queue_peek"> \_peek() </span>](#queue_function_index)

Returns the element at the front of the queue if available. This represents the next element to be removed from the queue.

#### Declaration

> `FMOD V PFX##_peek(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `V` - The element at the front of the queue.
2. `0` or `NULL` - If the queue is empty.

#### Complexity

* O(1)

## [<span id="queue_empty"> \_empty() </span>](#queue_function_index)

Returns true if the queue is empty, otherwise false.

#### Declaration

> `FMOD bool PFX##_empty(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `true` - If the queue is empty.
2. `false` - If there is at least one element in the queue.

#### Complexity

* O(1)

## [<span id="queue_full"> \_full() </span>](#queue_function_index)

Returns true if the queue is full, otherwise false. The queue is considered full when its internal buffer is filled up, so the next element added to the queue will required a resizing of the buffer, but note that the queue can grow indefinitely.

#### Declaration

> `FMOD bool PFX##_full(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `true` - If the queue internal buffer is full.
2. `false` - If the queue internal buffer is not full.

#### Complexity

* O(1)

## [<span id="queue_count"> \_count() </span>](#queue_function_index)

Returns the amount of elements in the queue.

#### Declaration

> `FMOD size_t PFX##_count(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `size_t` - The amount of elements in the queue.

#### Complexity

* O(1)

## [<span id="queue_capacity"> \_capacity() </span>](#queue_function_index)

Returns the internal buffer's current capacity.

#### Declaration

> `FMOD size_t PFX##_capacity(SNAME *_queue_);`

#### Parameters

1. `SNAME *_queue_` - Target queue.

#### Returns

1. `size_t` - The internal buffer's current capacity.

#### Complexity

* O(1)

## [<span id="queue_iter_new"> \_iter\_new() </span>](#queue_function_index)

Initializes an iterator with a given target queue. The iterator's cursor will be positioned at the front of the queue.

#### Declaration

> `FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be initialized.
2. `SNAME *target` - Target queue.

#### Complexity

* O(1)

## [<span id="queue_iter_start"> \_iter\_start() </span>](#queue_function_index)

Returns true if the iterator has reached the start of the queue (front element). If false, the iterator is still possible to iterate to a previous element.

#### Declaration

> `FMOD bool PFX##_iter_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns 

1. `true` - If the iterator has reached the start of the queue.
2. `false` - If the iterator has not reached the start of the queue.

#### Complexity

* O(1)

## [<span id="queue_iter_end"> \_iter\_end() </span>](#queue_function_index)

Returns true if the iterator has reached the end of the queue (rear element). If false, the iterator is still possible to iterate to a next element.

#### Declaration

> `FMOD bool PFX##_iter_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns 

1. `true` - If the iterator has reached the end of the queue.
2. `false` - If the iterator has not reached the end of the queue.

#### Complexity

* O(1)

## [<span id="queue_iter_tostart"> \_iter\_tostart() </span>](#queue_function_index)

Moves the cursor of the target iterator to the start (front element) of the queue.

#### Declaration

> `FMOD void PFX##_iter_tostart(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Complexity

* O(1)

## [<span id="queue_iter_toend"> \_iter\_toend() </span>](#queue_function_index)

Moves the cursor of the target iterator to the end of the queue (rear element).

#### Declaration

> `FMOD void PFX##_iter_toend(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Complexity

* O(1)

## [<span id="queue_iter_next"> \_iter\_next() </span>](#queue_function_index)

This function is used to iterate to the next element, retrieving the current one, along with an index that represents how many iterations have passed. When the index is `0` it means that the current result is the front element of the queue; if it equals `count - 1` then it is the rear element of the queue.

#### Declaration

> `FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.
2. `V *result` - Resulting value from the queue.
3. `size_t *index` - Resulting index.

#### Returns 

1. `true` - If the iterator has retrieved a valid `result` and `index`.
2. `false` - If the iterator has not retrieved a valid `result` and `index`. Here, iteration to the next element has ended.

#### Complexity

* O(1)

## [<span id="queue_iter_prev"> \_iter\_prev() </span>](#queue_function_index)

This function is used to iterate to the previous element, retrieving the current one, along with an index that represents how many iterations have passed. When the index is `0` it means that the current result is the front element of the queue; if it equals `count - 1` then it is the rear element of the queue.

#### Declaration

> `FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.
2. `V *result` - Resulting value from the queue.
3. `size_t *index` - Resulting index.

#### Returns 

1. `true` - If the iterator has retrieved a valid `result` and `index`.
2. `false` - If the iterator has not retrieved a valid `result` and `index`. Here, iteration to the previous element has ended.

#### Complexity

* O(1)

# [Deque](#collections_index)

A double-ended queue backed by a circular buffer. Elements can be added and removed from both ends but not in the middle. Can also be used as a queue.

### Generation Macro

* `DEQUE_GENERATE(PFX, SNAME, FMOD, V)`
    * `PFX` - Functions namespace or prefix.
    * `SNAME` - Structure name.
    * `FMOD` - Function modifier (static or empty).
    * `V` - Element type.

### Defined Structures

* `struct SNAME##_s` - Structure Name (represents a deque structure)
    * `V *buffer` - Internal storage.
    * `size_t capacity` - Storage capacity.
    * `size_t count` - Total elements in the deque.
    * `size_t front` - Front element index.
    * `size_t rear` - Rear element index.
* `struct SNAME##_iter_s` - Structure Iterator (represents a deque iterator)
    * `struct SNAME##_s *target` - Deque being iterated over.
    * `size_t cursor` - Index pointing to the next or previous element in the iteration.
    * `size_t index` - How many iterations have passed.
    * `bool start` - If the iterator reached the start of the deque.
    * `bool end` - If the iterator reached the end of the deque.

### Typedefs

* `typedef struct SNAME##_s SNAME`
* `typedef struct SNAME##_iter_s SNAME##_iter`

### <span id="deque_function_index"> Defined Functions </span>

* [\_new()](#deque_new)
* [\_clear()](#deque_clear)
* [\_free()](#deque_free)
* [\_push\_front()](#deque_push_front)
* [\_push\_back()](#deque_push_back)
* [\_pop\_front()](#deque_pop_front)
* [\_pop\_back()](#deque_pop_back)
* [\_push\_front\_if()](#deque_push_front_if)
* [\_push\_back\_if()](#deque_push_back_if)
* [\_pop\_front\_if()](#deque_pop_front_if)
* [\_pop\_back\_if()](#deque_pop_back_if)
* [\_front()](#deque_front)
* [\_back()](#deque_back)
* [\_empty()](#deque_empty)
* [\_full()](#deque_full)
* [\_count()](#deque_count)
* [\_capacity()](#deque_capacity)
* [\_iter\_new()](#deque_iter_new)
* [\_iter\_start()](#deque_iter_start)
* [\_iter\_end()](#deque_iter_end)
* [\_iter\_tostart()](#deque_iter_tostart)
* [\_iter\_toend()](#deque_iter_toend)
* [\_iter\_next()](#deque_iter_next)
* [\_iter\_prev()](#deque_iter_prev)

## [<span id="deque_new"> \_new() </span>](#deque_function_index)

Allocates and returns a new deque with an internal capacity of `size`. If allocation fails, `NULL` is returned.

#### Declaration

> `FMOD SNAME *PFX##_new(size_t size);`

#### Parameters

1. `size_t size` - The initial capacity for the deque.

#### Returns

1. `SNAME *` - A pointer to a heap allocated deque.
2. `NULL` - If allocation fails.

#### Complexity

* O(1)

## [<span id="deque_clear"> \_clear() </span>](#deque_function_index)

Removes all elements in the deque but does not frees the deque structure.

#### Declaration

> `FMOD void PFX##_clear(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque to be cleared.

#### Complexity

* O(1)

## [<span id="deque_free"> \_free() </span>](#deque_function_index)

Frees from memory the deque internal buffer and the structure itself. Note that if the elements inside the deque are pointers to allocated memory, this function might cause memory leaks as it does not deals with its elements.

#### Declaration

> `FMOD void PFX##_free(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque to be freed from memory.

#### Complexity

* O(1)

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

#### Complexity

* O(1)

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

#### Complexity

* O(1)

## [<span id="deque_pop_front"> \_pop\_front() </span>](#deque_function_index)

Removes an element from the front of the deque.

#### Declaration

> `FMOD bool PFX##_pop_front(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `true` - If the element was successfully removed from the front of the deque.
2. `false` - If the deque is empty.

#### Complexity

* O(1)

## [<span id="deque_pop_back"> \_pop\_back() </span>](#deque_function_index)

Removes an element from the back of the deque.

#### Declaration

> `FMOD bool PFX##_pop_back(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `true` - If the element was successfully removed from the back of the deque.
2. `false` - If the deque is empty.

#### Complexity

* O(1)

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

#### Complexity

* O(1)

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

#### Complexity

* O(1)

## [<span id="deque_pop_front_if"> \_pop\_front\_if() </span>](#deque_function_index)

Removes an element from the front of the deque if the condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_pop_front_if(SNAME *_deque_, bool condition);`

#### Parameters

1. `SNAME *_deque_` - Target deque.
2. `bool condition` - Condition for the element to be removed.

#### Returns

1. `true` - If the element was successfully removed from the front of the deque.
2. `false` - If the condition evaluated to false, or if the deque is empty.

#### Complexity

* O(1)

## [<span id="deque_pop_back_if"> \_pop\_back\_if() </span>](#deque_function_index)

Removes an element from the back of the deque if the condition evaluates to true.

#### Declaration

> `FMOD bool PFX##_pop_back_if(SNAME *_deque_, bool condition);`

#### Parameters

1. `SNAME *_deque_` - Target deque.
2. `bool condition` - Condition for the element to be removed.

#### Returns

1. `true` - If the element was successfully removed from the back of the deque.
2. `false` - If the condition evaluated to false, or if the deque is empty.

#### Complexity

* O(1)

## [<span id="deque_front"> \_front() </span>](#deque_function_index)

Returns the front element if the specified deque is not empty.

#### Declaration

> `FMOD V PFX##_front(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `V` - The front element of the deque.
2. `0` or `NULL` - If the deque is empty.

#### Complexity

* O(1)

## [<span id="deque_back"> \_back() </span>](#deque_function_index)

Returns the rear element if the specified deque is not empty.

#### Declaration

> `FMOD V PFX##_back(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `V` - The rear element of the deque.
2. `0` or `NULL` - If the deque is empty.

#### Complexity

* O(1)

## [<span id="deque_empty"> \_empty() </span>](#deque_function_index)

Returns true if the deque is empty, otherwise false.

#### Declaration

> `FMOD bool PFX##_empty(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `true` - If the deque is empty.
2. `false` - If there is at least one element in the deque.

#### Complexity

* O(1)

## [<span id="deque_full"> \_full() </span>](#deque_function_index)

Returns true if the deque is full, otherwise false. The deque is considered full when its internal buffer is filled up, so the next element added to the deque will required a resizing of the buffer, but note that the deque can grow indefinitely.

#### Declaration

> `FMOD bool PFX##_full(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `true` - If the deque internal buffer is full.
2. `false` - If the deque internal buffer is not full.

#### Complexity

* O(1)

## [<span id="deque_count"> \_count() </span>](#deque_function_index)

Returns the amount of elements in the deque.

#### Declaration

> `FMOD size_t PFX##_count(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `size_t` - The amount of elements in the deque.

#### Complexity

* O(1)

## [<span id="deque_capacity"> \_capacity() </span>](#deque_function_index)

Returns the internal buffer's current capacity.

#### Declaration

> `FMOD size_t PFX##_capacity(SNAME *_deque_);`

#### Parameters

1. `SNAME *_deque_` - Target deque.

#### Returns

1. `size_t` - The internal buffer's current capacity.

#### Complexity

* O(1)

## [<span id="deque_iter_new"> \_iter\_new() </span>](#deque_function_index)

Initializes an iterator with a given target deque. The iterator's cursor will be positioned at the front of the deque.

#### Declaration

> `FMOD void PFX##_iter_new(SNAME##_iter *iter, SNAME *target);`

#### Parameters

1. `SNAME##_iter *iter` - Iterator to be initialized.
2. `SNAME *target` - Target deque.

#### Complexity

* O(1)

## [<span id="deque_iter_start"> \_iter\_start() </span>](#deque_function_index)

Returns true if the iterator has reached the start of the deque (front element). If false, the iterator is still possible to iterate to a previous element.

#### Declaration

> `FMOD bool PFX##_iter_start(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns 

1. `true` - If the iterator has reached the start of the deque.
2. `false` - If the iterator has not reached the start of the deque.

#### Complexity

* O(1)

## [<span id="deque_iter_end"> \_iter\_end() </span>](#deque_function_index)

Returns true if the iterator has reached the end of the deque (rear element). If false, the iterator is still possible to iterate to a next element.

#### Declaration

> `FMOD bool PFX##_iter_end(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Returns 

1. `true` - If the iterator has reached the end of the deque.
2. `false` - If the iterator has not reached the end of the deque.

#### Complexity

* O(1)

## [<span id="deque_iter_tostart"> \_iter\_tostart() </span>](#deque_function_index)

Moves the cursor of the target iterator to the start (front element) of the deque.

#### Declaration

> `FMOD void PFX##_iter_tostart(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Complexity

* O(1)

## [<span id="deque_iter_toend"> \_iter\_toend() </span>](#deque_function_index)

Moves the cursor of the target iterator to the end (rear element) of the deque.

#### Declaration

> `FMOD void PFX##_iter_toend(SNAME##_iter *iter);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.

#### Complexity

* O(1)

## [<span id="deque_iter_next"> \_iter\_next() </span>](#deque_function_index)

This function is used to iterate to the next element, retrieving the current one, along with an index that represents how many iterations have passed. When the index is `0` it means that the current result is the front element of the deque; if it equals `count - 1` then it is the rear element of the deque.

#### Declaration

> `FMOD bool PFX##_iter_next(SNAME##_iter *iter, V *result, size_t *index);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.
2. `V *result` - Resulting value from the deque.
3. `size_t *index` - Resulting index.

#### Returns 

1. `true` - If the iterator has retrieved a valid `result` and `index`.
2. `false` - If the iterator has not retrieved a valid `result` and `index`. Here, iteration to the next element has ended.

#### Complexity

* O(1)

## [<span id="deque_iter_prev"> \_iter\_prev() </span>](#deque_function_index)

This function is used to iterate to the previous element, retrieving the current one, along with an index that represents how many iterations have passed. When the index is `0` it means that the current result is the front element of the deque; if it equals `count - 1` then it is the rear element of the deque.

#### Declaration

> `FMOD bool PFX##_iter_prev(SNAME##_iter *iter, V *result, size_t *index);`

#### Parameters

1. `SNAME##_iter *iter` - Target iterator.
2. `V *result` - Resulting value from the deque.
3. `size_t *index` - Resulting index.

#### Returns 

1. `true` - If the iterator has retrieved a valid `result` and `index`.
2. `false` - If the iterator has not retrieved a valid `result` and `index`. Here, iteration to the previous element has ended.

#### Complexity

* O(1)

# [Heap](#collections_index)

A heap is a nearly complete binary tree backed by a growable array. The first element of the array (or the root element of the tree) is the highest/lowest element. The min-heap is commonly used as a priority-queue and a max-heap is commonly used to sort an array of elements.

# [TreeSet](#collections_index)

A balanced binary tree that contains only unique elements to the set. Its elements are ordered.

# [TreeMap](#collections_index)

A balanced binary tree that maps a key `K` to a value `V`. The difference from a hashmap is that its keys are ordered.

# [HashSet](#collections_index)

A hashtable that contains only unique elements to the set. Its elements are not ordered.

# [HashMap](#collections_index)

A hashtable that maps a key `K` to a value `V`.

