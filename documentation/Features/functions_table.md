# Functions Table

Functions table are like a methods table for you custom data type. It is a `struct` of function pointers that you will need to define in order to some collections to work. Some functions are optional and others are needed in order to a collection to operate.

### CMP

A comparator function is used in sorted collections or when an equality is being checked like when trying to find a certain element in a list. It is responsible for taking two arguments of the same data type and comparing them. The return value is an `int` with the following definitions:

* Return `1` if the first argument is greater than the second;
* Return `0` if the first argument equals the second;
* Return `-1` if the first argument is less than the second.

### CPY

A copy function is used when a collection is being copied. It can be used to make a deep copy of of your custom data type. It must take a single parameter and return a new copy of that same data type. If this function is absent (`NULL`) the data type will be copied by assignment (for pointers this is a shallow copy).

### STR

A string function is responsible for taking a `FILE` pointer and a custom data type and outputting the string representation of that data returning a `bool` indication success or failure. It is useful for debugging.

### FREE

The free function is called when a collection is cleared (all elements removed) or freed (all elements removed and freed from memory) and it is responsible for completely freeing all resources that are usually acquired by your data type.

### HASH

This function receives a custom data type as parameter and returns a `size_t` hash of that data. Used in hashtables.

### PRI

A priority function works much like the comparator function except that it compares the priority between two elements. It is used in collections whose structure is based on the priority of elements and not in their general comparison.

* Return `1` if the first argument has a greater priority than the second;
* Return `0` if the first argument has the same priority as second;
* Return `-1` if the first argument has a lower priority than the second.

The following table shows which functions are required, optional or never used for each Collection:

| Collection | CMP | CPY | STR | FREE | HASH | PRI |
| ---------- | :-: | :-: | :-: | :--: | :--: | :-: |
| BidiMap      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Deque        | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| HashMap      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| HashSet      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Heap         | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) |
| IntervalHeap | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) |
| List         | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| LinkedList   | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| MultiMap     | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| MultiSet     | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Queue        | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| SortedList   | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| Stack        | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| TreeMap      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |
| TreeSet      | ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | ![#497edd](https://placehold.it/20/497edd/000000?text=+) | ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) |

<br>

| Color | Label |
| :---: | ----- |
| ![#b82b28](https://placehold.it/20/b82b28/000000?text=+) | Required for basic functionality. |
| ![#9f3b94](https://placehold.it/20/9f3b94/000000?text=+) | Required for specific functions. |
| ![#497edd](https://placehold.it/20/497edd/000000?text=+) | Required for non-core specific functions. |
| ![#00d3eb](https://placehold.it/20/00d3eb/000000?text=+) | Optional. |
| ![#2ef625](https://placehold.it/20/2ef625/000000?text=+) | Not Used. |
