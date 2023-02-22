# bidimap.h

A bidirectional map is a map that allows you to create a bijection in both directions between two sets of elements (K <-> V). It is possible to retrieve the value using a key or retrieve a key using a value. The naming is simply used to differentiate between one set of elements and the other set of elements. There is no difference between key and value since both can be keys and/or values.

## BidiMap Implementation

This implementation uses two arrays of pointers to an entry containing both the key and the value. Robin Hood hashing is used to minimize worst case scenarios.

## BidiMap Generation Macro

## BidiMap Structures

### Members

## BidiMap Functions

## BidiMap Callback Table
