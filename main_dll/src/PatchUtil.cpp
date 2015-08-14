#include "PatchUtil.h"

void WriteRelativeAddress(uAddr address, uAddr content)
{
	*reinterpret_cast<uAddr *>(address) = content - address - 4;
}

void WriteRelativeAddress32(u32 address, u32 content)
{
	*reinterpret_cast<u32 *>(address) = content - address - 4;
}

uAddr ReadRelativeAddress_x86_jmp(uAddr address)
{
	// 00007FFD612A133E | E9 ED 0E 00 00 | jmp 7FFD612A2230
	// 7FFD612A2230 - 00007FFD612A133E - 5 = 0E ED

	return address + 5 + *reinterpret_cast<i32 *>(address + 1);
}

template<typename T>
void WriteMemory(uAddr address, T value)
{
	*reinterpret_cast<T *>(address) = value;
}

void WriteUInt16(uAddr address, u16 value)
{
	*reinterpret_cast<u16 *>(address) = value;
}

void WriteInt32(uAddr address, i32 value)
{
	*reinterpret_cast<i32 *>(address) = value;
}

void WriteInt64(uAddr address, i64 value)
{
	*reinterpret_cast<i64 *>(address) = value;
}

void WriteByte(uAddr address, ubyte value)
{
	*reinterpret_cast<ubyte *>(address) = value;
}