# How to install

To start using the library you need first to download the source code. You can either fork the repository or download a zip file containing everything. Either way, after you unzip everything you will end up with the root folder (probably something called `C-Macro-Collections`) and inside it is the `src` folder.

With the `src` folder you can start including the library headers to your project with `-I path/to/library/src`.

The library has no external dependencies other than the C standard library.

```c
// Include header files like this
#include <cmc/list.h>
#include <dev/deque.h>
#include <utl/assert.h>
```

```c
// Not like this
#include <list.h>
#include <deque.h>  // dev or cmc?
#include <assert.h> // this will import from the standard library
```
