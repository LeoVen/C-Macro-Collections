# Callbacks

Every collection can have an optional callback node. In this node there are five functions:

| | |
|-|-|
| `create` | Is called when an element was successfully added to the collection      |
| `read`   | Is called when the collection was successfully queried about an element |
| `update` | Is called when an element in the collection was successfully updated    |
| `delete` | Is called when an element was successfully removed from the collection  |
| `resize` | Is called when the collection was full and successfully resized         |

Check the documentation for each collection to see which functions call which callbacks.
