/* 
 * File: main.cpp
 * Created: November 5, 2015
 * Contains: Custom core types definition
 */

#pragma once
 
//------------------------ instance types ---------------------------
 
typedef int integer;  // core type for integral values
typedef float real;  // core type for fractional values
typedef size_t index;  // core type for sizes and indexes
 
//------------------------- pointer types ----------------------------
 
typedef integer* iptr;  // core type for integral pointers
typedef real* rptr;  // core type for real pointers
typedef index* indptr;  // core type for size and index pointers
typedef void* vptr;  // core type for typeless pointers

//------------------------- composite types --------------------------