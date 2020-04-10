# Meanings

What does each Log Level means? It depends on the application. In here you will read more about how the C Macro Collections library uses each of these Log Levels, but this does not mean you should use them like described.

* __TRACE__ - Trace is used to describe the inner workings of a function or an algorithm. These messages can be used extensively, meaning that their content is probably not very useful, even for debugging.
* __DEBUG__ - Mainly used for debugging, tracking certain variables, checking if pointers were deallocated or anything useful to help visualizing what your program is doing in general.
* __INFO__ - Info can be used as a heads up. Something that is important to note, but is not quite a warning or an error.
* __WARN__ - Warnings are attributed to all cases where the function can recover from and usually treated by it. In most cases the user can treat these warnings himself.
* __ERROR__ - Something really bad happened. Your program will not crash yet but depending on what comes next, it will. This might be attributed to pointers that shouldn't be null because later functions might use it.
* __FATAL__ - This will probably be the last logging message before your program crashes or goes into madness. Fatal errors are commonly attributed to dereferencing NULL pointers.
