#include "PatchUtil.h"

void WriteRelativeAddress(uAddr address, uAddr content)
{
	WriteUAddr(address, content - address - 4);
}

uAddr ReadRelativeAddress_x86_jmp(uAddr address)
{
	// 00007FFD612A133E | E9 ED 0E 00 00 | jmp 7FFD612A2230
	// 7FFD612A2230 - 00007FFD612A133E - 5 = 0E ED

	return address + 5 + *reinterpret_cast<i32 *>(address + 1);
}
