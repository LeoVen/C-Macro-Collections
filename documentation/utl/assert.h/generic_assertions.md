# Generic Assertion Macros

## Contents

* [cmc_assert_array_equals_any](#cmc_assert_array_equals_any)
* [cmc_assert_array_within_any](#cmc_assert_array_within_any)
* [cmc_assert_array_outside_any](#cmc_assert_array_outside_any)
* [cmc_assert_array_sorted_any](#cmc_assert_array_sorted_any)

## cmc_assert_array_equals_any

Assert that two arrays are equal element by element. `from_index` and `to_index` are both inclusive.

```c
#define cmc_assert_array_equals_any(dtype, array1, array2, comparator, \
                                    from_index, to_index)
```

* `dtype` - The type of the array. Can be of any data type.
* `array1` - First array to be compared
* `array2` - Second array to be compared
* `comparator` - A comparator function pointer that has two `dtype` arguments and returns -1 if the first is less then the second, 0 if both are equal or 1 if the first is greater then the second
* `from_index` - First index from both arrays to compare (inclusive)
* `to_index` - Last index from both arrays to compare (inclusive)

## cmc_assert_array_within_any

Assert that each element in the array are within a certain boundary. Both ends are inclusive.

```c
#define cmc_assert_array_within_any(dtype, array, comparator, lower_bound, \
                                    upper_bound, from_index, to_index)
```

* `dtype` - The type of the array. Can be of any data type
* `array` - Array to be checked if its elements are within a given range
* `comparator` - A comparator function pointer that has two `dtype` arguments and returns -1 if the first is less then the second, 0 if both are equal or 1 if the first is greater then the second
* `lower_bound` - Smallest value of the given range
* `upper_bound` - Highest value of the given range
* `from_index` - First index from the array to compare (inclusive)
* `to_index` - Last index from the array to compare (inclusive)

## cmc_assert_array_outside_any

Assert that each element in the array are outside of a certain boundary. Both ends are inclusive.

```c
#define cmc_assert_array_outside_any(dtype, array, comparator, lower_bound, \
                                     upper_bound, from_index, to_index)
```

* `dtype` - The type of the array. Can be of any data type
* `array` - Array to be checked if its elements are within a given range
* `comparator` - A comparator function pointer that has two `dtype` arguments and returns -1 if the first is less then the second, 0 if both are equal or 1 if the first is greater then the second
* `lower_bound` - Smallest value of the given range
* `upper_bound` - Highest value of the given range
* `from_index` - First index from the array to compare (inclusive)
* `to_index` - Last index from the array to compare (inclusive)

## cmc_assert_array_sorted_any

Asserts that an array is sorted.

```c
#define cmc_assert_array_sorted_any(dtype, array, comparator, from_index, \
                                    to_index)
```

* `dtype` - The type of the array. Can be of any data type
* `array` - Array to be checked if it is sorted
* `comparator` - A comparator function pointer that has two `dtype` arguments and returns -1 if the first is less then the second, 0 if both are equal or 1 if the first is greater then the second
* `from_index` - First index from the array to compare (inclusive)
* `to_index` - Last index from the array to compare (inclusive)
