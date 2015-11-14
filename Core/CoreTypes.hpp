/* 
 * File: main.cpp
 * Created: November 5, 2015
 * Contains: Custom core types definition
 */

#pragma once

#include <stdint.h>
 
//------------------------ instance types ---------------------------
 
typedef int integer;  // core type for native integral values
typedef float real;  // core type for fractional values
typedef size_t index;  // core type for sizes and indexes

// OS dependent size-specified integers //
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
 
//------------------------- pointer types ----------------------------
 
typedef integer* iptr;  // core type for native integral pointers
typedef real* rptr;  // core type for real pointers
typedef index* indptr;  // core type for size and index pointers
typedef void* vptr;  // core type for typeless pointers

//------------------------- composite types --------------------------
