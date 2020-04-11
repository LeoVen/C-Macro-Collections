# test.h

A simple [Unit Test](https://en.wikipedia.org/wiki/Unit_testing) utility used by the C Macro Collections Library. It has two main components: a Unit and a Test.

This utility uses two other libraries defined in the C Macro Collections library:

* [`assert.h`](../assert.h/index.html)
* [`timer.h`](../timer.h/index.html)

When a test is created inside a unit, it will first set the global variable [`cmc_assert_state`](../assert.h/index.html#cmc_assert_state) to `true`. Then, inside the test you can use the assertions provided by the `assert.h` library to pass or fail a unit test automatically. By the end of a test the variable will be checked. If `false` it means that a certain assertion failed.

The runtime of each Unit Test is calculated using the `timer.h` library.

## CMC_CREATE_UNIT

Creates a Unit that can contain tests.

```c
#define CMC_CREATE_UNIT(UNAME, VERBOSE, BODY)
```

* `UNAME` - Unit name. This needs to be a valid function name!
* `VERBOSE` - Either `true` or `false`. If `true`, prints every message of `PASSED` or `FAILED` from each test
* `BODY` - A block of code containing tests

## CMC_CREATE_TEST

Creates a test within a unit. This macro can only be used inside the `BODY` of a `CMC_CREATE_UNIT` since it uses local variables allocated by the latter.

```c
#define CMC_CREATE_TEST(TNAME, BODY)
```

* `TNAME` - Test name
* `BODY` - A block of code containing the test itself

Inside a test you can use the assertions from `assert.h` to automatically pass or fail a test.

## CMC_TEST_ABORT

Aborts a unit test. This will jump to a `goto` located at the end of the Unit Test.

## cmc_test_info

Contains information about a Unit Test. Used internally.

```c
struct cmc_test_info
{
    uintmax_t total;
    uintmax_t passed;
    uintmax_t failed;
    bool aborted;
    bool verbose;
};
```

## CMC_TEST_COLOR

Define this macro if you wish to have color output from the messages printed by each test. Note that a Unit Test need to have `VERBOSE` equal to `true`.

## Example

```c
#include "utl/test.h"

CMC_CREATE_UNIT(check_math, true, {
    CMC_CREATE_TEST(summation, {
        // assert.h already comes with test.h
        cmc_assert_equals(int32_t, 4, 2 + 2);
    });

    CMC_CREATE_TEST(subtraction, {
        cmc_assert_equals(int32_t, -1, 2 - 3);
    });

    CMC_CREATE_TEST(this will fail, {
        // test names can be text as long as they don't have a comma
        cmc_assert_greater(double, 1.0, 0.0);
    });

    CMC_CREATE_TEST(fish, {
        // You can also manually fail a test
        if (strcmp("2 + 2", "fish") != 0)
            cmc_assert_state = false;
    });
});

int main(void)
{
    // returns how many tests failed
    return check_math();
}
```
