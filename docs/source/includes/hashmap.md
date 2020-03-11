# hashmap.h

A HashMap is an implementation of a Map with unique keys, where every key is mapped to a value (K -> V). The keys are not sorted. To access a value you must specify a key. This key is hashed to a number that uniquely represents it and through this it is possible to find in which position of an array of buckets the value is.

## HashMap Implementation

The HashMap is implemented as a flat HashTable meaning that every entry is allocated when the collection is initialized, but they are all empty.

The HashTable uses [Open Addressing](https://en.wikipedia.org/wiki/Open_addressing) and [Linear Probing](https://en.wikipedia.org/wiki/Linear_probing) to resolve collisions along with [Robin Hood Hashing](https://en.wikipedia.org/wiki/Hash_table) to minimize the worst case scenarios.
