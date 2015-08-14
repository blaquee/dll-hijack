#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>

typedef __int8  i8;
typedef __int16 i16;
typedef __int32 i32;
typedef __int64 i64;

typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

#ifdef _WIN64
typedef unsigned __int64 uAddr;
#else
typedef unsigned __int32 uAddr;
#endif

typedef u8  ubyte;
typedef u16 ushort;
typedef u32 uint;
typedef u64 ulong;
