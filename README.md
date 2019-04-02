# C-Macro-Collections

Generate Simple Data Structures of any type in C for you to use in your projects.

## How to use

When including `macro_collections.h` in your source code you gain access to a macro called `COLLECTION_GENERATE` with the following parameters:

* \*__C__ - Container type (*LIST*, *LINKEDLIST*, *STACK*, *QUEUE*, *DEQUE*, *HEAP*, *TREESET*, *TREEMAP*, *HASHSET*, *HASHMAP*).
* \*__P__ - Permission (PRIVATE, PUBLIC). Only matters when header and source are declared in separate files.
	* *PRIVATE* - Members of a struct are hidden in source file and only accessible through functions.
	* *PUBLIC* - Members of a struct are visible.
* \*__PFX__ - Functions prefix or namespace.
* \*__SNAME__ - Structure name (`typedef struct SNAME##_s SNAME`).
* __FMOD__ - Function modifier (static). Useful when declaring header and source in the same file.
* \*\*__K__ - Key type. Only used in *HASHMAP* and *TREEMAP*; ignored by others.
* \*__V__ - Value type. Primary type for most data structures, or value to be mapped by *HASHMAP* and *TREEMAP*.

\* Required parameters

\*\* Required only by *HASHMAP* and *TREEMAP*

<hr>

**See Documentation.md for more information. All functions not listed in it are not meant to be used at any time, only by the structure implementation.**