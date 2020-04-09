# Overview

## Single Value Assertion Macros

* __cmc_assert__ - Asserts that an expression evaluates to true;
* __cmc_assert_equals__ - Asserts that a value from type `dtype` equals another;
* __cmc_assert_not_equals__ - Asserts that value from type `dtype` does not equals another;
* __cmc_assert_greater__ - Asserts that a value from type `dtype` is greater than a lower bound;
* __cmc_assert_greater_equals__ - Asserts that a value from type `dtpype` is greater than or equal to a lower bound;
* __cmc_assert_lesser__ - Asserts that a value from type `dtpype` is lesser than an upper bound;
* __cmc_assert_lesser_equals__ - Asserts that a value from type `dtpype` is lesser than or equal to an upper bound;
* __cmc_assert_in_range__ - Asserts that a value from type `dtpype` is within a certain range;
* __cmc_assert_not_in_range__ - Asserts that a value from type `dtpype` is outside of a certain range.

## Array Assertion Macros

* __cmc_assert_array_equals_any__ - Assert that two arrays are equal element by element;
* __cmc_assert_array_within_any__ - Assert that each element in the array are within a bounds;
* __cmc_assert_array_outside_any__ - Assert that each element in the array are out of bounds;
* __cmc_assert_array_sorted_any__ - Assert that the array is sorted.
