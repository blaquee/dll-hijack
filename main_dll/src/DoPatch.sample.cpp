#include "Common.h"
#include "DoPatch.h"
#include "PatchUtil.h"
#include "SnR_Engine.h"

uAddr uBase;

void do_patch(HMODULE base) {
	wchar_t szExePath[MAX_PATH];
	GetModuleFileName(nullptr, szExePath, MAX_PATH);
	if (0 != wcscmp(wcsrchr(szExePath, L'\\'), L"\\my_target.exe"))
		// Not my_target
		return;

	uBase = uAddr(base);

	// Method 1:
	// Just write some random stuff to target memory.
	WriteInt32(uBase + 0xcafe, 0xdeadbeaf);

	// Method 2:
	// Use search and replace engine
	// to create a more generic patch, 
	// that may work with a future version.
	auto engine = new SnR_Engine::SnR_Engine(base);
	ubyte patchSig1[] = {
		// 0000000010000000 | 89 D8 | mov eax, ebx         |
		SnR_Engine::SearchMode_Search, 5,
		0x89, 0xD8,

		// 0000000010000002 | 85 C0 | test eax, eax        |
		0x85, 0xC0, 


		// 0000000010000004 | 74 0E | je 0000000010000014  |
		0x74,
		SnR_Engine::SearchMode_Skip, 1,

		// 0000000010000006 | 48 31 C0 | xor rax,rax       |
		SnR_Engine::SearchMode_Search, 4,
		0x48, 0x31, 0xC0,

		// 0000000010000009 | C3       | ret               |
		0xC3,


		SnR_Engine::SearchMode_EOF
	};
	
	// Patch data begin from where search signature begin.
	ubyte patchData1[] = {
		SnR_Engine::SearchMode_Skip, 4,

		// je -> jmp
		SnR_Engine::SearchMode_Replace, 1,
		0xEB,


		SnR_Engine::SearchMode_EOF
	};

	if (engine->doSearchAndReplace(patchSig1, patchData1) == 0)
	{
#ifdef _DEBUG
		printf("Didn't patch anything.");
#endif
	}

	// Continue patching / clean up ..
}