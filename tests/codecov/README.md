# codecov

In order to provide a good code coverage, all macros have been expanded and added to `src/*.c`.

* `src` - Since gcov fails to detect code that was expanded by macro, all collections where manually expanded and dumped into their respective source files;
* `unt` - Unit Tests;
* `main.c` - Where all tests are compiled together and called by main().
