#include "PatchUtil.h"

void WriteRelativeAddress(uint address, int content)
{
	WriteInt(address, content - address - 4);
}

template<typename T>
void WriteMemory(uint address, T value)
{
	*reinterpret_cast<T *>(address) = value;
}

void WriteInt(uint address, int value)
{
	*reinterpret_cast<int *>(address) = value;
}

void WriteByte(uint address, ubyte value)
{
	*reinterpret_cast<ubyte *>(address) = value;
}