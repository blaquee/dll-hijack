#pragma once
#include "Common.h"

void WriteRelativeAddress(uAddr address, uAddr content);

#ifndef _WIN64
void WriteRelativeAddress32(u32 address, u32 content);
#endif

uAddr ReadRelativeAddress_x86_jmp(uAddr address);

template<typename T>
void WriteMemory(uAddr, T);

void WriteUInt16(uAddr address, u16 value);
void WriteInt32(uAddr address, i32 value);
void WriteByte(uAddr address, ubyte value);