# assert.h

Definition of many typed non-abortive assertions. Every assertion that fails will print a message to `stderr` but will not abort the program execution. The macro parameters are first assigned to a temporary variable of the specified `dtype` in order to generate warnings from the compiler in case of type mismatch.

The advantage of these typed macros is that when an assertion fails, it is possible to print to `stderr` the actual value that was passed to the function and check the expected result(s) all in one message.

There are assertions that work with [predefined values](valued_assertions.html) and others that work with [generic values](generic_assertions.html). Valued assertions work with a selected list of `dtype` (see further below), while the generic assertions work with any data type.

The macro parameters can be an *lvalue* or an *rvalue* because the parameters will be first assigned to local variables of the selected data type. Then, these local variables will be tested for whichever test that needs to be checked.

Assigning the macro parameter to a local variable can be useful. These local variables will be of type `dtype` and if the parameters don't match that type a warning will pop up.

Ever assertion is inside a block of `do { } while (0)`.

## Printed Messages

Whenever an assertion fails, a message will be printed to `stderr`. The message will look something like this:

```
Assertion Failed at FILE:FUNCTION:LINE for { ACTUAL }: DETAILS
```

The first information is the file where that assertion failed. Then the function name and the line number. Then in place of `ACTUAL`, a stringified version of the `actual` parameter, which is the variable being checked in the assertion, will be printed.

In `DETAILS` is where more information about the assertion will be printed. This is the most useful part of these macros. In here, the actual value of the variable will be printed along with the expected value or range or lower bound, etc. __In here you will be able to debug your application to check the expected value(s) against the actual one.__

__Example__

```c
#include "utl/assert.h"

void is_one_hundred(int variable)
{
    cmc_assert_equals(int32_t, 100, variable);
}

int main(void)
{
    int my_var = 9;
    is_one_hundred(my_var);
}
```

This will print the following message:

`Assertion Failed at main.c:is_one_hundred:5 for { variable }: Expected: 100 Actual: 9`

## cmc_assert_state

This is a global variable that can be used to test if all assertions passed. If any assertion fails, this variable will be set to `false`. If an assertion passes this variable will __not__ be set back to `true`. If you wish to, you can set the state back to `true` manually.

```c
static bool cmc_assert_state = true;
```

This variable is used in [test.h](../test.h/index.html) utility to automatically pass or fail a unit test. Once the unit test finishes, this variable is set back to `true` so that another unit test may follow.

## dtype

These are the data types supported by the Valued Assertion macros:

* `int8_t`
* `int16_t`
* `int32_t`
* `int64_t`
* `uint8_t`
* `uint16_t`
* `uint32_t`
* `uint64_t`
* `intmax_t`
* `uintmax_t`
* `size_t`
* `float`
* `double`

The following two are only accepted by `cmc_assert_equals` and `cmc_assert_not_equals`:

* `ptr`
* `bool`

Note that `ptr` is used for any pointers. They are first casted to `void *` and then their address is compared.

Note that bool will be expanded to `_Bool`.
