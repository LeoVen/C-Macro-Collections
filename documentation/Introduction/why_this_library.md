# Why this library

C is a low level language and lacks all the data structures that we commonly use. Many high level languages already come with these collections so that we don't have to implement everything ourselves. A lot of third party libraries that implement these missing data structures for the C language usually make use of a void pointers and a lot of macros. __This is why the C Macro Collections Library was created.__ All you need to do is to write down one macro and which data type you wish to work with. The library currently provides many data structures, such as:

* __Linear Collections__ : List, LinkedList, Deque, Stack, Queue, SortedList
* __Sets__               : HashSet, TreeSet, MultiSet
* __Maps__               : HashMap, TreeMap, MultiMap, BidiMap
* __Heaps__              : Heap, IntervalHeap

All with the following features:

* __Type-safe__ - No `void *` pointers are used. A collection of type `int` will only accept integers;
* __Customization__ - Custom struct name and function namespace;
* __Bidirectional Iterators__ - Full support for iterators;
* __Nesting__ - Collections can be nested (List of Lists, HashSet of Stacks, etc);
* __One macro to rule them all__ - Only one macro to generate everything and that's it.

and many other powerful features (see [Features](../Features/features.html)).
