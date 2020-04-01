# assert.h

Definition of many typed non-abortive assertions. Every assertion that fails will print a message to `stderr` but will not abort the program execution. The macro parameters are first assigned to a temporary variable of the specified `dtype` in order to generate warnings from the compiler in case of type mismatch.

The advantage of these typed macros is that when an assertion fails, it is possible to print to `stderr` the actual value that was passed to the function and check the expected result(s) all in one message.

There are assertions that work with a single value to be tested and others that work with arrays. Single value assertions work with a selected list of `dtype`, while the assertion with arrays work with any `dtype` that is a valid data type.

The macro parameters can be an lvalue or an rvalue.
