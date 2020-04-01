# Single Value Assertion Macros



## cmc_assert_state

This is a global variable that can be used to test if all assertions passed. If a single assertion fails, this variable will be set to `false`. If an assertion passes this variable will not be set back to `true`.

```c
static bool cmc_assert_state = true;
```

This variable is used in [test.h](./test.html) utility to automatically pass or fail a unit test. Once the unit test finishes, this variable is set back to `true` so that another unit test may follow.

## cmc_assert

Asserts that an expression evaluates to `true`.

```c
#define cmc_assert(expression)
```

### Parameters

* `expression` - An expression that needs to be evaluated to true.

## cmc_assert_equals

Asserts that a
