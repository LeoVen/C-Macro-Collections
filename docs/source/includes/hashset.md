# hashset.h

A HashSet is an implementation of a Set with unique keys. The keys are not sorted. It is implemented as a flat hashtable with linear probing and robin hood hashing.

## HashSet Implementation

The HashSet is implemented as a flat HashTable meaning that every entry is allocated when the collection is initialized, but they are all empty.

The HashTable uses [Open Addressing](https://en.wikipedia.org/wiki/Open_addressing) and [Linear Probing](https://en.wikipedia.org/wiki/Linear_probing) to resolve collisions along with [Robin Hood Hashing](https://en.wikipedia.org/wiki/Hash_table) to minimize the worst case scenarios.
