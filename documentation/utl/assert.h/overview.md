# Overview

## Single Value Assertion Macros

* cmc_assert
* cmc_assert_equals
* cmc_assert_not_equals
* cmc_assert_greater
* cmc_assert_greater_equals
* cmc_assert_lesser
* cmc_assert_lesser_equals
* cmc_assert_in_range
* cmc_assert_not_in_range

## Array Assertion Macros

* cmc_assert_array_equals_any
* cmc_assert_array_within_any
* cmc_assert_array_outside_any
* cmc_assert_array_sorted_any

## dtype

These are the data types supported by the single value assertion macros:

* int8_t
* int16_t
* int32_t
* int64_t
* uint8_t
* uint16_t
* uint32_t
* uint64_t
* intmax_t
* uintmax_t
* size_t
* float
* double

The following two are only accepted by `cmc_assert_equals` and `cmc_assert_not_equals`:

* ptr
* bool

Note that `ptr` is used for any pointers. They are first casted to `void *` and then their address is compared.
