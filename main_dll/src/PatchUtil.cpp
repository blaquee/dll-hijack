#include "PatchUtil.h"

void WriteRelativeAddress(uAddr address, uAddr content)
{
	DWORD oldProtect;
	VirtualProtect(LPVOID(address), sizeof uAddr, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<uAddr *>(address) = content - address - 4;
	VirtualProtect(LPVOID(address), sizeof uAddr, oldProtect, &oldProtect);
}

#ifndef _WIN64
void WriteRelativeAddress32(u32 address, u32 content)
{
	DWORD oldProtect;
	VirtualProtect(LPVOID(address), sizeof u32, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<u32 *>(address) = content - address - 4;
	VirtualProtect(LPVOID(address), sizeof u32, oldProtect, &oldProtect);
}
#endif

uAddr ReadRelativeAddress_x86_jmp(uAddr address)
{
	// 00007FFD612A133E | E9 ED 0E 00 00 | jmp 7FFD612A2230
	// 7FFD612A2230 - 00007FFD612A133E - 5 = 0E ED

	return address + 5 + *reinterpret_cast<i32 *>(address + 1);
}

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
void WriteUInt16(uAddr address, u16 value)
{
	DWORD oldProtect;
	VirtualProtect(LPVOID(address), sizeof u16, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<u16 *>(address) = value;
	VirtualProtect(LPVOID(address), sizeof u16, oldProtect, &oldProtect);
}

void WriteInt32(uAddr address, i32 value)
{
	DWORD oldProtect;
	VirtualProtect(LPVOID(address), sizeof i32, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<i32 *>(address) = value;
	VirtualProtect(LPVOID(address), sizeof i32, oldProtect, &oldProtect);
}

void WriteUInt32(uAddr address, u32 value)
{
	DWORD oldProtect;
	VirtualProtect(LPVOID(address), sizeof u32, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<u32 *>(address) = value;
	VirtualProtect(LPVOID(address), sizeof u32, oldProtect, &oldProtect);
}

void WriteInt64(uAddr address, i64 value)
{
	DWORD oldProtect;
	VirtualProtect(LPVOID(address), sizeof i64, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<i64 *>(address) = value;
	VirtualProtect(LPVOID(address), sizeof i64, oldProtect, &oldProtect);
}

void WriteByte(uAddr address, ubyte value)
{
	DWORD oldProtect;
	VirtualProtect(LPVOID(address), sizeof ubyte, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<ubyte *>(address) = value;
	VirtualProtect(LPVOID(address), sizeof ubyte, oldProtect, &oldProtect);
}