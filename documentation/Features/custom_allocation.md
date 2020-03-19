# Custom Allocation

All collections have an allocation node. This node can be modified so that every allocation and de-allocation can be done with custom functions. The allocation node has pointers to functions that correspond to:

* `calloc`
* `free`
* `malloc`
* `realloc`
