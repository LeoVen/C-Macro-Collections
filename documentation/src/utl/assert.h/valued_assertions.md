# Valued Assertion Macros

## Contents

* [cmc_assert](#cmc_assert)
* [cmc_assert_equals](#cmc_assert_equals)
* [cmc_assert_not_equals](#cmc_assert_not_equals)
* [cmc_assert_greater](#cmc_assert_greater)
* [cmc_assert_greater_equals](#cmc_assert_greater_equals)
* [cmc_assert_lesser](#cmc_assert_lesser)
* [cmc_assert_lesser_equals](#cmc_assert_lesser_equals)
* [cmc_assert_in_range](#cmc_assert_in_range)
* [cmc_assert_not_in_range](#cmc_assert_not_in_range)

## cmc_assert

Asserts that an `expression` evaluates to `true`.

```c
#define cmc_assert(expression)
```

__Parameters__

* `expression` - An expression that needs to be evaluated to true.

## cmc_assert_equals

Asserts that a value equals another expected value.

```c
#define cmc_assert_equals(dtype, expected, actual)
```

__Parameters__

* `dtype` - One of the possible data types listed [here](index.html#dtype)
* `expected` - The expected value for this assertion
* `actual` - A variant that is checked against the expected value

## cmc_assert_not_equals

Asserts that a value does not equal another.

```c
#define cmc_assert_not_equals(dtype, not_expected, actual)
```

__Parameters__

* `dtype` - One of the possible data types listed [here](index.html#dtype)
* `not_expected` - The value that is not expected for this assertion
* `actual` - A variant that is checked against the not expected value

## cmc_assert_greater

Asserts that a value is above a certain lower boundary.

```c
#define cmc_assert_greater(dtype, boundary, actual)
```

* `dtype` - One of the possible data types listed [here](index.html#dtype)
* `boundary` - The expected lowest possible value (excluded)
* `actual` - A variant that is checked against the boundary value

## cmc_assert_greater_equals

Asserts that a value is above a certain lower boundary or equal to it.

```c
#define cmc_assert_greater_equals(dtype, boundary, actual)
```

* `dtype` - One of the possible data types listed [here](index.html#dtype)
* `boundary` - The expected lowest possible value (included)
* `actual` - A variant that is checked against the boundary value

## cmc_assert_lesser

Asserts that a value is below a certain upper boundary.

```c
#define cmc_assert_lesser(dtype, boundary, actual)
```

* `dtype` - One of the possible data types listed [here](index.html#dtype)
* `boundary` - The expected highest possible value (excluded)
* `actual` - A variant that is checked against the boundary value

## cmc_assert_lesser_equals

Asserts that a value is below a certain upper boundary or equal to it.

```c
#define cmc_assert_lesser_equals(dtype, boundary, actual)
```

* `dtype` - One of the possible data types listed [here](index.html#dtype)
* `boundary` - The expected highest possible value (excluded)
* `actual` - A variant that is checked against the boundary value

## cmc_assert_in_range

Asserts that a value is within a certain range. Range is inclusive on both ends.

```c
#define cmc_assert_in_range(dtype, lower_bound, upper_bound, actual)
```

* `dtype` - One of the possible data types listed [here](index.html#dtype)
* `lower_bound` - Smallest value of the expected range
* `upper_bound` - Highest value of the expected range
* `actual` - A variant that is checked against lower and upper boundaries value

## cmc_assert_not_in_range

Asserts that a value is not within a certain range. Range is inclusive on both ends.

```c
#define cmc_assert_not_in_range(dtype, lower_bound, upper_bound, actual)
```

* `dtype` - One of the possible data types listed [here](index.html#dtype)
* `lower_bound` - Smallest value of the not expected range
* `upper_bound` - Highest value of the not expected range
* `actual` - A variant that is checked against lower and upper boundaries value
