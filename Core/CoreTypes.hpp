/*
* File: CoreTypes.hpp
* Created: November 5, 2015
* Contains: Custom core types definition
*
*/

#pragma once

#include <stdint.h>

//------------------------ instance types ---------------------------

typedef int integer;  // core type for native integral values
typedef float real;  // core type for fractional values
//typedef size_t ind;  // core type for sizes and indexes

//------------------------- pointer types ----------------------------

typedef integer* iptr;  // core type for native integral pointers
typedef real* rptr;  // core type for real pointers
typedef size_t* sptr;  // core type for size and index pointers
typedef void* vptr;  // core type for typeless pointers

//------------------------- composite types --------------------------