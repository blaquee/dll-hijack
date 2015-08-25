#pragma once
#include "Common.h"

void WriteRelativeAddress(uAddr address, uAddr content);

template<typename T>
void WriteMem(uAddr address, T value)
{
	DWORD oldProtect;
	VirtualProtect(LPVOID(address), sizeof T, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<T *>(address) = value;
	VirtualProtect(LPVOID(address), sizeof T, oldProtect, &oldProtect);
}

template<typename T>
void WriteMemRaw(uAddr address, T value)
{
	*reinterpret_cast<T *>(address) = value;
}

#define WriteInt(address, value)  WriteMem(address, int(value))
#define WriteUInt(address, value)  WriteMem(address, uint(value))
#define WriteUAddr(address, value)  WriteMem(address, uAddr(value))

#define WriteShort(address, value)   WriteMem(address, short(value))
#define WriteUShort(address, value)  WriteMem(address, ushort(value))

#define WriteByte(address, value)  WriteMem(address, byte(value))


#define WriteIntRaw(address, value)  WriteMemRaw(address, int(value))
#define WriteUIntRaw(address, value)  WriteMemRaw(address, uint(value))
#define WriteUAddrRaw(address, value)  WriteMemRaw(address, uAddr(value))

#define WriteShortRaw(address, value)   WriteMemRaw(address, short(value))
#define WriteUShortRaw(address, value)  WriteMemRaw(address, ushort(value))

#define WriteByteRaw(address, value)  WriteMemRaw(address, byte(value))
