# multimap.h

The MultiMap is a Map that allows for multiple keys. This is a data structure that has a very narrow usage. A Map (either TreeMap or HashMap) can also work like a MultiMap if a certain key is mapped to another collection.

`Map<K = int, V = List<int>>` maps an integer to a list of integer.

`MultiMap<K = int, V = int>` maps many integer keys to integer values.

The difference is that in a MultiMap you can store keys that are the same but might be different instances of the same value. This is also relevant if your data type is more complex like a struct where its ID is the same but some other members of this data type are different.

## MultiMap Implementation

The map uses separate chaining and robin hood hashing. Its internal buffer is made of a pair of pointers to entries that represent a linked list. So accessing map->buffer[0] would give the first pair of pointers. Each entry has both a pointer to a previous entry and a next entry. This design choice was made so that every collection in this library has a two-way iterator with a very few exceptions.

Each entry is composed of a Key and a Value. Entries with the same key should always hash to the same linked list. Also, keys that hash to the same bucket will also be in the same linked list.

The order of inserting and removing the same keys will behave like a FIFO. So the first key added will be the first to be removed.
