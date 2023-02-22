# Library Structure

The `src` folder is subdivided in 5 other folders and one file:

* __cmc__ - The main C Macro Collections Library
* __dev__ - The main C Macro Collections Library for development
* __sac__ - Statically Allocated Collections
* __utl__ - Utility like ForEach macros, logging, etc
* __macro\_collections.h__ - Master header containing all collections and utilities

## cmc

This is where the C Macro Collections are hosted.

## dev

In this folder is an exact copy of the `cmc` Collections with the added logging utility (`./utl/log.h`). These are useful to debug your code because everything that is happening inside the data structure can be seen.

## sac

This is where the Statically Allocated Collections are hosted. These collections are just like the `cmc` Collections but they have a constant size, a C array, instead of a dynamically allocated array (yes, even for Linked List).

## utl

Utility. Here you will find things like assert macros, foreach macros, logging utility, unit test and timer.

## macro_collections.h

This is the master header. Include this big boy and all functionalities of the library will be in your hands.
