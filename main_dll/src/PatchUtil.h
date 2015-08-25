#pragma once
#include "Common.h"

void WriteRelativeAddress(uAddr address, uAddr content);

#ifndef _WIN64
void WriteRelativeAddress32(u32 address, u32 content);
#endif

uAddr ReadRelativeAddress_x86_jmp(uAddr address);

template<typename T>
void WriteMem(uAddr, T);

template<typename T>
void WriteMemRaw(uAddr address, T value);

void WriteUInt16(uAddr address, u16 value);
void WriteInt32(uAddr address, i32 value);
void WriteUInt32(uAddr address, u32 value);
void WriteByte(uAddr address, ubyte value);