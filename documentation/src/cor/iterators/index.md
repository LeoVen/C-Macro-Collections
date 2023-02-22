# Iterators

Every collection comes with an interface of iterators where you can easily access the elements of a collection. These can move at any direction, can start at any 'end' of a collection and can move any amount of steps per iteration. Every collection generated comes with an iterator.

By design choice these iterators do not support modifications to the collection. If a collection is modified, all iterators that have been initialized will most likely be invalidated and may cause undefined behavior if used afterwards.
