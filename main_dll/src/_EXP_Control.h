#pragma once

// Export the one you want, 
// You can also export more if you wish.

#define __EXP_VERSION
#ifdef _WIN64
// Windows x64 Options
#else
#define __EXP_LPK
#define __XP_PATCH_IN_THREAD
#endif

#define __EXP_MSIMG32
// #define __EXP_WINMM
/*
	__XP_PATCH_IN_THREAD: In Windows XP, DLL will try to call do_patch
	within CreateThread instead. This can prevent some random unknown crash.


	Exports:
	__EXP_LPK
	__EXP_VERSION

	__EXP_WINMM
	__EXP_MSIMG32

	Exports lpk.dll, winmm.dll and version.dll functions.
	You _can_ export more than one.
*/